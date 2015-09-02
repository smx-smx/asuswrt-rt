/*
 * Cache operations for the TC3162
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/mipsregs.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/system.h>
#include <asm/mmu_context.h>
#include <asm/isadep.h>
#include <asm/io.h>
#include <asm/wbflush.h>
#include <asm/cpu.h>
#include <asm/asm.h>
#include <asm/cacheops.h>

#include <asm/r4kcache.h>

/**
 * The icace and dcache size variables are currently set to hard values,
 * as there is no way to size the Lexra caches through software.
 */
unsigned long icache_size, dcache_size;
__DMEM unsigned long icache_lsize, dcache_lsize;

extern int r3k_have_wired_reg;	/* in r3k-tlb.c */

static inline void tc3162_blast_dcache_page(unsigned long addr)
{
	blast_dcache16_page(addr);
}

static inline void tc3162_blast_icache(void)
{
	unsigned int controlReg;
	unsigned long flags;

	local_irq_save(flags);
	controlReg = read_c0_cctl();

	write_c0_cctl(controlReg & ~CCTL_IINVAL);
	write_c0_cctl(controlReg | CCTL_IINVAL);

	/* delay to allow cache to be flushed */
	__asm__ __volatile__(".set\tnoreorder\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t" "nop\n\t" ".set\treorder\n\t");

	write_c0_cctl(controlReg);
	local_irq_restore(flags);
}

static void tc3162_blast_dcache(void)
{
	unsigned int controlReg;
	unsigned long flags;

	local_irq_save(flags);
	controlReg = read_c0_cctl();

	write_c0_cctl(controlReg & ~CCTL_DWBINVAL);
	write_c0_cctl(controlReg | CCTL_DWBINVAL);

	/* delay to allow cache to be flushed */
	__asm__ __volatile__(".set\tnoreorder\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t"
			     "nop\n\t" "nop\n\t" ".set\treorder\n\t");

	write_c0_cctl(controlReg);
	local_irq_restore(flags);
}

static void tc3162_flush_icache_all(void)
{
	tc3162_blast_icache();
}

static inline void tc3162_blast_icache_page(unsigned long addr)
{
	tc3162_blast_icache();
}

static inline void tc3162_flush_cache_all(void)
{
	if (!cpu_has_dc_aliases)
		return;

	tc3162_blast_dcache();
}

static inline void tc3162___flush_cache_all(void)
{
	tc3162_blast_dcache();
	tc3162_blast_icache();
}

static void tc3162_flush_cache_mm(struct mm_struct *mm)
{
	if (!cpu_has_dc_aliases)
		return;

	if (cpu_context(smp_processor_id(), mm) != 0)
		tc3162_blast_dcache();
}

static void tc3162_flush_cache_range(struct vm_area_struct *vma,
	unsigned long start, unsigned long end)
{
	if (!cpu_has_dc_aliases)
		return;
	if (!(cpu_context(smp_processor_id(), vma->vm_mm)))
		return;

	tc3162_blast_dcache();
}

static void tc3162_flush_cache_page(struct vm_area_struct *vma, unsigned long page, unsigned long pfn)
{
	int exec = vma->vm_flags & VM_EXEC;
	struct mm_struct *mm = vma->vm_mm;
	pgd_t *pgdp;
	pud_t *pudp;
	pmd_t *pmdp;
	pte_t *ptep;

	/*
	 * If ownes no valid ASID yet, cannot possibly have gotten
	 * this page into the cache.
	 */
	if (cpu_context(smp_processor_id(), mm) == 0)
		return;

	page &= PAGE_MASK;
	pgdp = pgd_offset(mm, page);
	pudp = pud_offset(pgdp, page);
	pmdp = pmd_offset(pudp, page);
	ptep = pte_offset(pmdp, page);

	/*
	 * If the page isn't marked valid, the page cannot possibly be
	 * in the cache.
	 */
	if (!(pte_val(*ptep) & _PAGE_PRESENT))
		return;

	/*
	 * Doing flushes for another ASID than the current one is
	 * too difficult since stupid R4k caches do a TLB translation
	 * for every cache flush operation.  So we do indexed flushes
	 * in that case, which doesn't overly flush the cache too much.
	 */
	if ((mm == current->active_mm) && (pte_val(*ptep) & _PAGE_VALID)) {
		if (cpu_has_dc_aliases || exec)
			tc3162_blast_dcache_page(page);
		if (exec)
			tc3162_blast_icache_page(page);

		return;
	}
}

static void local_tc3162_flush_data_cache_page(void * addr)
{
	tc3162_blast_dcache_page((unsigned long)addr);
}

static void tc3162_flush_data_cache_page(unsigned long addr)
{
	tc3162_blast_dcache_page(addr);
}

static void tc3162_flush_icache_range(unsigned long start, unsigned long end)
{
	protected_blast_dcache_range(start, end);
	tc3162_blast_icache();
}

static void tc3162_flush_cache_sigtramp(unsigned long addr)
{
	protected_writeback_dcache_line(addr & ~(dcache_lsize - 1)); 
	tc3162_blast_icache();
}

__IMEM static void tc3162_dma_cache_wback_inv(unsigned long addr, unsigned long size)
{
#if 1
	blast_dcache_range(addr, addr + size);
#else
	unsigned long end;

	if (((size | addr) & (PAGE_SIZE - 1)) == 0) {
		end = addr + size;
		do {
			tc3162_blast_dcache_page(addr);
			addr += PAGE_SIZE;
		} while(addr != end);
	} else if (size > dcache_size) {
		tc3162_blast_dcache();
	} else {
		blast_dcache_range(addr, addr + size);
	}
#endif
}

static void tc3162_dma_cache_inv(unsigned long addr, unsigned long size)
{
#if 1
	blast_inv_dcache_range(addr, addr + size);
#else
	unsigned long end;

	if (((size | addr) & (PAGE_SIZE - 1)) == 0) {
		end = addr + size;
		do {
			tc3162_blast_dcache_page(addr);
			addr += PAGE_SIZE;
		} while(addr != end);
	} else if (size > dcache_size) {
		tc3162_blast_dcache();
	} else {
		blast_inv_dcache_range(addr, addr + size);
	}
#endif
}

static void __init tc3162_probe_cache(void)
{
	dcache_size = 1024*8;
	if (dcache_size)
		dcache_lsize = 16;
	icache_size = 1024*32;
	if (icache_size)
		icache_lsize = 16;

	current_cpu_data.icache.ways = 2;
	current_cpu_data.icache.linesz = 16;
	current_cpu_data.dcache.ways = 2;
	current_cpu_data.dcache.linesz = 16;
}

void __init r3k_cache_init(void)
{
	extern void build_clear_page(void);
	extern void build_copy_page(void);

	/*
	 * These variables are unused on this architecture.
	 */
	tc3162_probe_cache();
	
	r3k_have_wired_reg = 1;

	flush_cache_all = tc3162_flush_cache_all;
	__flush_cache_all = tc3162___flush_cache_all; 
	flush_cache_mm = tc3162_flush_cache_mm;
	flush_cache_range = tc3162_flush_cache_range;
	flush_cache_page = tc3162_flush_cache_page;
	flush_icache_range = tc3162_flush_icache_range;

	flush_cache_sigtramp = tc3162_flush_cache_sigtramp;
	flush_icache_all = tc3162_flush_icache_all;
	local_flush_data_cache_page = local_tc3162_flush_data_cache_page;
	flush_data_cache_page = tc3162_flush_data_cache_page;

	_dma_cache_wback_inv = tc3162_dma_cache_wback_inv;
	_dma_cache_wback = tc3162_dma_cache_wback_inv;
	_dma_cache_inv = tc3162_dma_cache_inv;

	/*
	shm_align_mask = max_t(unsigned long,
			(dcache_size / current_cpu_data.dcache.ways) - 1,
		   		PAGE_SIZE - 1);
	*/

	current_cpu_data.icache.waysize = icache_size / current_cpu_data.icache.ways;
	current_cpu_data.dcache.waysize = dcache_size / current_cpu_data.dcache.ways;

	current_cpu_data.icache.sets =
		current_cpu_data.icache.waysize / current_cpu_data.icache.linesz;
	current_cpu_data.dcache.sets =
		current_cpu_data.dcache.waysize / current_cpu_data.dcache.linesz;

	/* TC3162 is Physically-indexed, physically-tagged cache policy, don't have dcache alias
	   problem */
	/*
	if (current_cpu_data.dcache.waysize > PAGE_SIZE)
		current_cpu_data.dcache.flags |= MIPS_CACHE_ALIASES;
	*/

	current_cpu_data.icache.waybit = 0;
	current_cpu_data.dcache.waybit = 0;

	printk("Primary instruction cache %ldkB, linesize %ld bytes.\n",
		icache_size >> 10, icache_lsize);
	printk("Primary data cache %ldkB, linesize %ld bytes.\n",
		dcache_size >> 10, dcache_lsize);

	build_clear_page();
	build_copy_page();
	tc3162_blast_icache();
}

