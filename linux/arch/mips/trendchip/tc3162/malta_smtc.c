/*
 * Malta Platform-specific hooks for SMP operation
 */
#include <linux/irq.h>
#include <linux/init.h>

#include <asm/mipsregs.h>
#include <asm/mipsmtregs.h>
#include <asm/smtc.h>
#include <asm/smtc_ipi.h>

#include <asm/tc3162/tc3162.h>

/* VPE/SMP Prototype implements platform interfaces directly */

/*
 * Cause the specified action to be performed on a targeted "CPU"
 */

void core_send_ipi(int cpu, unsigned int action)
{
	/* "CPU" may be TC of same VPE, VPE of same CPU, or different CPU */
	smtc_send_ipi(cpu, LINUX_SMP_IPI, action);
}

/*
 * Platform "CPU" startup hook
 */

void prom_boot_secondary(int cpu, struct task_struct *idle)
{
	smtc_boot_secondary(cpu, idle);
}

/*
 * Post-config but pre-boot cleanup entry point
 */

void prom_init_secondary(void)
{
        void smtc_init_secondary(void);
	int myvpe;

	/* Don't enable Malta I/O interrupts (IP2) for secondary VPEs */
	myvpe = read_c0_tcbind() & TCBIND_CURVPE;
	if (myvpe != 0) {
		/* Ideally, this should be done only once per VPE, but... */
#ifdef CONFIG_MIPS_TC3262
		clear_c0_status(STATUSF_IP2 | STATUSF_IP3 | STATUSF_IP4 | STATUSF_IP5 | STATUSF_IP6
				| STATUSF_IP7);
		set_c0_status(STATUSF_IP0 | STATUSF_IP1);
#else
		clear_c0_status(STATUSF_IP2);
		set_c0_status(STATUSF_IP0 | STATUSF_IP1 | STATUSF_IP3
				| STATUSF_IP4 | STATUSF_IP5 | STATUSF_IP6
				| STATUSF_IP7);
#endif
	}

        smtc_init_secondary();
}

/*
 * Platform SMP pre-initialization
 *
 * As noted above, we can assume a single CPU for now
 * but it may be multithreaded.
 */

void plat_smp_setup(void)
{
	if (read_c0_config3() & (1<<2))
		mipsmt_build_cpu_map(0);
}

void __init plat_prepare_cpus(unsigned int max_cpus)
{
	if (read_c0_config3() & (1<<2))
		mipsmt_prepare_cpus();
}

/*
 * SMP initialization finalization entry point
 */

void prom_smp_finish(void)
{
	smtc_smp_finish();
}

/*
 * Hook for after all CPUs are online
 */

void prom_cpus_done(void)
{
}

#ifdef CONFIG_MIPS_MT_SMTC_IRQAFF
/*
 * IRQ affinity hook
 */


void plat_set_irq_affinity(unsigned int irq, cpumask_t affinity)
{
	cpumask_t tmask = affinity;
	int cpu = 0;
	void smtc_set_irq_affinity(unsigned int irq, cpumask_t aff);
	int irq_vpe0 = 0;
	int irq_vpe1 = 0;
	unsigned int offset1, offset2;

	for_each_cpu_mask(cpu, affinity) {
		if (!cpu_online(cpu))
			cpu_clear(cpu, tmask);
		else {
			if (cpu_data[cpu].vpe_id == 0)
				irq_vpe0++;
			else 
				irq_vpe1++;
		}
	}
	irq_desc[irq].affinity = tmask;

	/* change IRQ binding to VPE0 or VPE1 */
	offset1= 32 - irq;
	offset2 = ((irq - 1) % 4) * 8 + 4;
	offset1 = (offset1 >> 2) << 2;
	if (irq_vpe0 >= irq_vpe1) 
		VPint(CR_INTC_IVSR0 + offset1) &= ~(1<<offset2);
	else
		VPint(CR_INTC_IVSR0 + offset1) |= (1<<offset2);

	if (cpus_empty(tmask))
		/*
		 * We could restore a default mask here, but the
		 * runtime code can anyway deal with the null set
		 */
		printk(KERN_WARNING
			"IRQ affinity leaves no legal CPU for IRQ %d\n", irq);

	/* Do any generic SMTC IRQ affinity setup */
	smtc_set_irq_affinity(irq, tmask);
}
#endif /* CONFIG_MIPS_MT_SMTC_IRQAFF */
