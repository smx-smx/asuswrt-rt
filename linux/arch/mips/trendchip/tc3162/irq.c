




/*
 *  Interrupt service routines for Trendchip board
 */
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/mipsregs.h>
#include <asm/tc3162/tc3162.h>
#include <linux/sched.h>

#ifdef CONFIG_MIPS_TC3262
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/kernel_stat.h>
#include <linux/kernel.h>
#include <linux/random.h>
#include <asm/mipsmtregs.h>
#else
#include <linux/io.h>
#include <asm/irq_cpu.h>
#endif





extern asmlinkage void tc3162_handle_int(void);
extern irq_desc_t irq_desc[NR_IRQS];

#define ALLINTS (IE_SW0 | IE_SW1 | IE_IRQ0 | IE_IRQ1 | IE_IRQ2 | IE_IRQ3 | IE_IRQ4 | IE_IRQ5)

#ifdef CONFIG_MIPS_TC3262

static DEFINE_SPINLOCK(tc3162_irq_lock);

static inline void unmask_mips_mt_irq(unsigned int irq)
{
	unsigned int vpflags = dvpe();
	int cpu_irq = 0;

	if ((irq == SI_SWINT1_INT1) || (irq == SI_SWINT_INT1))  
		cpu_irq = 1;

	set_c0_status(0x100 << cpu_irq);
	irq_enable_hazard();
	evpe(vpflags);
}

static inline void mask_mips_mt_irq(unsigned int irq)
{
	unsigned int vpflags = dvpe();
	int cpu_irq = 0;

	if ((irq == SI_SWINT1_INT1) || (irq == SI_SWINT_INT1))  
		cpu_irq = 1;

	clear_c0_status(0x100 << cpu_irq);
	irq_disable_hazard();
	evpe(vpflags);
}

static unsigned int mips_mt_cpu_irq_startup(unsigned int irq)
{
	unsigned int vpflags = dvpe();
	int cpu_irq = 0;

	if ((irq == SI_SWINT1_INT1) || (irq == SI_SWINT_INT1))  
		cpu_irq = 1;

	VPint(CR_INTC_IMR) |=  (1 << (irq-1));
	if (irq == SI_SWINT_INT0)
		VPint(CR_INTC_IMR) |=  (1 << (SI_SWINT1_INT0-1));
	else if (irq == SI_SWINT_INT1)
		VPint(CR_INTC_IMR) |=  (1 << (SI_SWINT1_INT1-1));

	clear_c0_cause(0x100 << cpu_irq);
	evpe(vpflags);
	unmask_mips_mt_irq(irq);

	return 0;
}

/*
 * While we ack the interrupt interrupts are disabled and thus we don't need
 * to deal with concurrency issues.  Same for mips_cpu_irq_end.
 */
static void mips_mt_cpu_irq_ack(unsigned int irq)
{
	unsigned int vpflags = dvpe();
	int cpu_irq = 0;

	if ((irq == SI_SWINT1_INT1) || (irq == SI_SWINT_INT1))  
		cpu_irq = 1;

	clear_c0_cause(0x100 << cpu_irq);
	evpe(vpflags);
	mask_mips_mt_irq(irq);
}

static struct irq_chip mips_mt_cpu_irq_controller = {
	.name		= "MIPS",
	.startup	= mips_mt_cpu_irq_startup,
	.ack		= mips_mt_cpu_irq_ack,
	.mask		= mask_mips_mt_irq,
	.mask_ack	= mips_mt_cpu_irq_ack,
	.unmask		= unmask_mips_mt_irq,
	.eoi		= unmask_mips_mt_irq,
};

#define __BUILD_IRQ_DISPATCH(irq_n) \
static void __tc3262_irq_dispatch##irq_n(void) \
{								\
	do_IRQ(irq_n);				\
}	

#define __BUILD_IRQ_DISPATCH_FUNC(irq_n)  __tc3262_irq_dispatch##irq_n 

/* pre-built 64 irq dispatch function */
__BUILD_IRQ_DISPATCH(0)
__BUILD_IRQ_DISPATCH(1)
__BUILD_IRQ_DISPATCH(2)
__BUILD_IRQ_DISPATCH(3)
__BUILD_IRQ_DISPATCH(4)
__BUILD_IRQ_DISPATCH(5)
__BUILD_IRQ_DISPATCH(6)
__BUILD_IRQ_DISPATCH(7)
__BUILD_IRQ_DISPATCH(8)
__BUILD_IRQ_DISPATCH(9)
__BUILD_IRQ_DISPATCH(10)
__BUILD_IRQ_DISPATCH(11)
__BUILD_IRQ_DISPATCH(12)
__BUILD_IRQ_DISPATCH(13)
__BUILD_IRQ_DISPATCH(14)
__BUILD_IRQ_DISPATCH(15)
__BUILD_IRQ_DISPATCH(16)
__BUILD_IRQ_DISPATCH(17)
__BUILD_IRQ_DISPATCH(18)
__BUILD_IRQ_DISPATCH(19)
__BUILD_IRQ_DISPATCH(20)
__BUILD_IRQ_DISPATCH(21)
__BUILD_IRQ_DISPATCH(22)
__BUILD_IRQ_DISPATCH(23)
__BUILD_IRQ_DISPATCH(24)
__BUILD_IRQ_DISPATCH(25)
__BUILD_IRQ_DISPATCH(26)
__BUILD_IRQ_DISPATCH(27)
__BUILD_IRQ_DISPATCH(28)
__BUILD_IRQ_DISPATCH(29)
__BUILD_IRQ_DISPATCH(30)
__BUILD_IRQ_DISPATCH(31)
__BUILD_IRQ_DISPATCH(32)
__BUILD_IRQ_DISPATCH(33)
__BUILD_IRQ_DISPATCH(34)
__BUILD_IRQ_DISPATCH(35)
__BUILD_IRQ_DISPATCH(36)
__BUILD_IRQ_DISPATCH(37)
__BUILD_IRQ_DISPATCH(38)
__BUILD_IRQ_DISPATCH(39)
__BUILD_IRQ_DISPATCH(40)
__BUILD_IRQ_DISPATCH(41)
__BUILD_IRQ_DISPATCH(42)
__BUILD_IRQ_DISPATCH(43)
__BUILD_IRQ_DISPATCH(44)
__BUILD_IRQ_DISPATCH(45)
__BUILD_IRQ_DISPATCH(46)
__BUILD_IRQ_DISPATCH(47)
__BUILD_IRQ_DISPATCH(48)
__BUILD_IRQ_DISPATCH(49)
__BUILD_IRQ_DISPATCH(50)
__BUILD_IRQ_DISPATCH(51)
__BUILD_IRQ_DISPATCH(52)
__BUILD_IRQ_DISPATCH(53)
__BUILD_IRQ_DISPATCH(54)
__BUILD_IRQ_DISPATCH(55)
__BUILD_IRQ_DISPATCH(56)
__BUILD_IRQ_DISPATCH(57)
__BUILD_IRQ_DISPATCH(58)
__BUILD_IRQ_DISPATCH(59)
__BUILD_IRQ_DISPATCH(60)
__BUILD_IRQ_DISPATCH(61)
__BUILD_IRQ_DISPATCH(62)
__BUILD_IRQ_DISPATCH(63)

/* register pre-built 64 irq dispatch function */
static void (*irq_dispatch_tab[])(void) =
{
__BUILD_IRQ_DISPATCH_FUNC(0),
__BUILD_IRQ_DISPATCH_FUNC(1),
__BUILD_IRQ_DISPATCH_FUNC(2),
__BUILD_IRQ_DISPATCH_FUNC(3),
__BUILD_IRQ_DISPATCH_FUNC(4),
__BUILD_IRQ_DISPATCH_FUNC(5),
__BUILD_IRQ_DISPATCH_FUNC(6),
__BUILD_IRQ_DISPATCH_FUNC(7),
__BUILD_IRQ_DISPATCH_FUNC(8),
__BUILD_IRQ_DISPATCH_FUNC(9),
__BUILD_IRQ_DISPATCH_FUNC(10),
__BUILD_IRQ_DISPATCH_FUNC(11),
__BUILD_IRQ_DISPATCH_FUNC(12),
__BUILD_IRQ_DISPATCH_FUNC(13),
__BUILD_IRQ_DISPATCH_FUNC(14),
__BUILD_IRQ_DISPATCH_FUNC(15),
__BUILD_IRQ_DISPATCH_FUNC(16),
__BUILD_IRQ_DISPATCH_FUNC(17),
__BUILD_IRQ_DISPATCH_FUNC(18),
__BUILD_IRQ_DISPATCH_FUNC(19),
__BUILD_IRQ_DISPATCH_FUNC(20),
__BUILD_IRQ_DISPATCH_FUNC(21),
__BUILD_IRQ_DISPATCH_FUNC(22),
__BUILD_IRQ_DISPATCH_FUNC(23),
__BUILD_IRQ_DISPATCH_FUNC(24),
__BUILD_IRQ_DISPATCH_FUNC(25),
__BUILD_IRQ_DISPATCH_FUNC(26),
__BUILD_IRQ_DISPATCH_FUNC(27),
__BUILD_IRQ_DISPATCH_FUNC(28),
__BUILD_IRQ_DISPATCH_FUNC(29),
__BUILD_IRQ_DISPATCH_FUNC(30),
__BUILD_IRQ_DISPATCH_FUNC(31),
__BUILD_IRQ_DISPATCH_FUNC(32),
__BUILD_IRQ_DISPATCH_FUNC(33),
__BUILD_IRQ_DISPATCH_FUNC(34),
__BUILD_IRQ_DISPATCH_FUNC(35),
__BUILD_IRQ_DISPATCH_FUNC(36),
__BUILD_IRQ_DISPATCH_FUNC(37),
__BUILD_IRQ_DISPATCH_FUNC(38),
__BUILD_IRQ_DISPATCH_FUNC(39),
__BUILD_IRQ_DISPATCH_FUNC(40),
__BUILD_IRQ_DISPATCH_FUNC(41),
__BUILD_IRQ_DISPATCH_FUNC(42),
__BUILD_IRQ_DISPATCH_FUNC(43),
__BUILD_IRQ_DISPATCH_FUNC(44),
__BUILD_IRQ_DISPATCH_FUNC(45),
__BUILD_IRQ_DISPATCH_FUNC(46),
__BUILD_IRQ_DISPATCH_FUNC(47),
__BUILD_IRQ_DISPATCH_FUNC(48),
__BUILD_IRQ_DISPATCH_FUNC(49),
__BUILD_IRQ_DISPATCH_FUNC(50),
__BUILD_IRQ_DISPATCH_FUNC(51),
__BUILD_IRQ_DISPATCH_FUNC(52),
__BUILD_IRQ_DISPATCH_FUNC(53),
__BUILD_IRQ_DISPATCH_FUNC(54),
__BUILD_IRQ_DISPATCH_FUNC(55),
__BUILD_IRQ_DISPATCH_FUNC(56),
__BUILD_IRQ_DISPATCH_FUNC(57),
__BUILD_IRQ_DISPATCH_FUNC(58),
__BUILD_IRQ_DISPATCH_FUNC(59),
__BUILD_IRQ_DISPATCH_FUNC(60),
__BUILD_IRQ_DISPATCH_FUNC(61),
__BUILD_IRQ_DISPATCH_FUNC(62),
__BUILD_IRQ_DISPATCH_FUNC(63)
};

#endif

__IMEM
static inline void unmask_mips_irq(unsigned int irq)
{
#ifdef CONFIG_MIPS_TC3262
	unsigned long flags;
	int cpu = smp_processor_id();

	spin_lock_irqsave(&tc3162_irq_lock, flags);
#ifdef CONFIG_MIPS_MT_SMTC
	if (cpu_data[cpu].vpe_id != 0) {
#else
	if (cpu != 0) {
#endif
		if (irq == SI_TIMER_INT)
			irq = SI_TIMER1_INT;
	}
	if (irq <= 32)
		VPint(CR_INTC_IMR) |=  (1 << (irq-1));
	else
		VPint(CR_INTC_IMR_1) |=  (1 << (irq-33));
	spin_unlock_irqrestore(&tc3162_irq_lock, flags);
#else
	VPint(CR_INTC_IMR) |=  (1 << irq);
#endif
}

__IMEM
static inline void mask_mips_irq(unsigned int irq)
{
#ifdef CONFIG_MIPS_TC3262
	unsigned long flags;
	int cpu = smp_processor_id();

	spin_lock_irqsave(&tc3162_irq_lock, flags);
#ifdef CONFIG_MIPS_MT_SMTC
	if (cpu_data[cpu].vpe_id != 0) {
#else
	if (cpu != 0) {
#endif
		if (irq == SI_TIMER_INT)
			irq = SI_TIMER1_INT;
	}

	if (irq <= 32)
		VPint(CR_INTC_IMR) &= ~(1 << (irq-1));
	else
		VPint(CR_INTC_IMR_1) &= ~(1 << (irq-33));
	spin_unlock_irqrestore(&tc3162_irq_lock, flags);
#else
	VPint(CR_INTC_IMR) &= ~(1 << irq);
#endif
}

void tc3162_enable_irq(unsigned int irq)
{
	unmask_mips_irq(irq);
}
EXPORT_SYMBOL(tc3162_enable_irq);

void tc3162_disable_irq(unsigned int irq)
{
	mask_mips_irq(irq);
}
EXPORT_SYMBOL(tc3162_disable_irq);

static struct irq_chip tc3162_irq_chip = {
	.name		= "MIPS",
	.ack		= mask_mips_irq,
	.mask		= mask_mips_irq,
	.mask_ack	= mask_mips_irq,
	.unmask		= unmask_mips_irq,
	.eoi		= unmask_mips_irq,
#ifdef CONFIG_MIPS_MT_SMTC_IRQAFF
	.set_affinity	= plat_set_irq_affinity,
#endif /* CONFIG_MIPS_MT_SMTC_IRQAFF */
};

void __init arch_init_irq(void)
{
	unsigned int i;

	/* Disable all hardware interrupts */
	clear_c0_status(ST0_IM);
	clear_c0_cause(CAUSEF_IP);

	/* Initialize IRQ action handlers */
	for (i = 0; i < NR_IRQS; i++) {
#ifdef CONFIG_MIPS_TC3262
		/*
	 	 * Only MT is using the software interrupts currently, so we just
	 	 * leave them uninitialized for other processors.
	 	 */
		if (cpu_has_mipsmt) {
			if ((i == SI_SWINT1_INT0) || (i == SI_SWINT1_INT1) ||
				(i == SI_SWINT_INT0) || (i == SI_SWINT_INT1)) { 
				set_irq_chip(i, &mips_mt_cpu_irq_controller);
				continue;
			}
		}

		if ((i == SI_TIMER_INT) || (i == SI_TIMER1_INT))
			set_irq_chip_and_handler(i, &tc3162_irq_chip,
					 handle_percpu_irq);
		else
			set_irq_chip_and_handler(i, &tc3162_irq_chip,
					 handle_level_irq);
#else
		set_irq_chip_and_handler(i, &tc3162_irq_chip,
					 handle_level_irq);
#endif
	}

#ifdef CONFIG_MIPS_TC3262
	if (cpu_has_veic || cpu_has_vint) {
		write_c0_status((read_c0_status() & ~ST0_IM ) |
			                (STATUSF_IP0 | STATUSF_IP1)); 

		/* register irq dispatch functions */
		for (i = 0; i < NR_IRQS; i++)
			set_vi_handler(i, irq_dispatch_tab[i]);
	} else {
		change_c0_status(ST0_IM, ALLINTS);
	}
#else
	/* Enable all interrupts */
	change_c0_status(ST0_IM, ALLINTS);
#endif
}

__IMEM asmlinkage void plat_irq_dispatch(void)
{
#ifdef CONFIG_MIPS_TC3262
	int irq = ((read_c0_cause() & ST0_IM) >> 10);

	do_IRQ(irq);
#else
	do_IRQ(VPint(CR_INTC_IVR));
#endif
}

