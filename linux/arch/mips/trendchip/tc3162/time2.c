/*
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 1999,2000 MIPS Technologies, Inc.  All rights reserved.
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * Setting up the clock on the MIPS boards.
 */

#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel_stat.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/timex.h>

#include <asm/mipsregs.h>
#include <asm/mipsmtregs.h>
#include <asm/hardirq.h>
#include <asm/irq.h>
#include <asm/div64.h>
#include <asm/cpu.h>
#include <asm/time.h>

#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/TCIfSetQuery_os.h>

unsigned long cpu_khz;

static int mips_cpu_timer_irq;
extern int cp0_perfcount_irq;
extern void smtc_timer_broadcast(int);
#ifdef CONFIG_PCI
extern int pcieRegInitConfig(void);
extern void pcieReset(void);
extern void setahbstat(int val);
#endif

static void delay1ms(int ms)
{
	volatile uint32 timer_now, timer_last;
	volatile uint32 tick_acc;
	uint32 one_tick_unit = 1 * SYS_HCLK * 1000 / 2;
	volatile uint32 tick_wait = ms * one_tick_unit; 
	volatile uint32 timer1_ldv = VPint(CR_TIMER1_LDV);

	tick_acc = 0;
	timer_last = VPint(CR_TIMER1_VLR);
	do {
		timer_now = VPint(CR_TIMER1_VLR);
	  	if (timer_last >= timer_now) 
	  		tick_acc += timer_last - timer_now;
		else
			tick_acc += timer1_ldv - timer_now + timer_last;
		timer_last = timer_now;
	} while (tick_acc < tick_wait);
}

void
timer_Configure(
	uint8  timer_no, 
	uint8 timer_enable, 
	uint8 timer_mode, 
	uint8 timer_halt
)
{
    uint32 word,word1;
    word = VPint(CR_TIMER_CTL);
    word1 = (timer_enable << timer_no)|(timer_mode << (timer_no + 8))|(timer_halt << (timer_no + 26));
    word |= word1;
    VPint (CR_TIMER_CTL)=word;    
} 

void 
timerSet(
	uint32 timer_no,
	uint32 timerTime, 
	uint32 enable,
	uint32 mode, 
	uint32 halt
)
{   
    uint32 word;

	/* when SYS_HCLK is large, it will cause overflow. The calculation will be wrong */
    /* word = (timerTime * SYS_HCLK) * 1000 / 2; */
    word = (timerTime * SYS_HCLK) * 500; 
    timerLdvSet(timer_no,word);
    timerCtlSet(timer_no,enable,mode,halt);
}

void
timer_WatchDogConfigure (
	uint8 tick_enable, 
	uint8 watchdog_enable
)
{
    uint32 word;
    word = VPint(CR_TIMER_CTL);
#if !defined(TCSUPPORT_CT) 
	word &= 0xfdffffdf;
    word |= ( tick_enable << 5)|(watchdog_enable<<25);
#endif
    VPint (CR_TIMER_CTL)=word;
}

/************************************************************************
*                      C P U  T I M E R  
*************************************************************************
*/

/* how many counter cycles in a jiffy */
static unsigned long cycles_per_jiffy __read_mostly;

/* expirelo is the count value for next CPU timer interrupt */
static unsigned int expirelo;

static void cputmr_timer_ack(void)
{
	unsigned int count;

	/* Ack this timer interrupt and set the next one.  */
	expirelo += cycles_per_jiffy;
	VPint(CR_CPUTMR_CMR0) = expirelo;

	/* Check to see if we have missed any timer interrupts.  */
	while (((count = VPint(CR_CPUTMR_CNT0)) - expirelo) < 0x7fffffff) {
		/* missed_timer_count++; */
		expirelo = count + cycles_per_jiffy;
		VPint(CR_CPUTMR_CMR0) = expirelo;
	}
}

static cycle_t cputmr_hpt_read(void)
{
	return VPint(CR_CPUTMR_CNT0);
}

static void __init cputmr_hpt_timer_init(void)
{
	VPint(CR_CPUTMR_CNT0) = 0x0;
	VPint(CR_CPUTMR_CNT1) = 0x0;

	expirelo = VPint(CR_CPUTMR_CNT0) + cycles_per_jiffy;
	VPint(CR_CPUTMR_CMR0) = expirelo;

	VPint(CR_CPUTMR_CMR1) = expirelo;

	VPint(CR_CPUTMR_CTL) |= (1<<1)|(1<<0);
}

irqreturn_t cputmr_timer_interrupt(int irq, void *dev_id)
{
	int cpu = smp_processor_id();

#ifdef CONFIG_MIPS_MT_SMTC
	/*
	 *  In an SMTC system, one Count/Compare set exists per VPE.
	 *  Which TC within a VPE gets the interrupt is essentially
	 *  random - we only know that it shouldn't be one with
	 *  IXMT set. Whichever TC gets the interrupt needs to
	 *  send special interprocessor interrupts to the other
	 *  TCs to make sure that they schedule, etc.
	 *
	 *  That code is specific to the SMTC kernel, not to
	 *  the a particular platform, so it's invoked from
	 *  the general MIPS timer_interrupt routine.
	 */

	/*
	 * There are things we only want to do once per tick
	 * in an "MP" system.   One TC of each VPE will take
	 * the actual timer interrupt.  The others will get
	 * timer broadcast IPIs. We use whoever it is that takes
	 * the tick on VPE 0 to run the full timer_interrupt().
	 */
	if (cpu_data[cpu].vpe_id == 0) {
		timer_interrupt(irq, NULL);
	} else {
		VPint(CR_CPUTMR_CMR1) = (VPint(CR_CPUTMR_CNT1) +
						 (mips_hpt_frequency/HZ));
		local_timer_interrupt(irq, dev_id);
	}
	smtc_timer_broadcast(cpu_data[cpu].vpe_id);
#else /* CONFIG_MIPS_MT_SMTC */
	if (cpu == 0) {
		/*
		 * CPU 0 handles the global timer interrupt job and process
		 * accounting resets count/compare registers to trigger next
		 * timer int.
		 */
		timer_interrupt(irq, NULL);
	} else {
		/* Everyone else needs to reset the timer int here as
		   ll_local_timer_interrupt doesn't */
		/*
		 * FIXME: need to cope with counter underflow.
		 * More support needs to be added to kernel/time for
		 * counter/timer interrupts on multiple CPU's
		 */
		VPint(CR_CPUTMR_CMR1) = (VPint(CR_CPUTMR_CNT1) + (mips_hpt_frequency/HZ));

		/*
		 * Other CPUs should do profiling and process accounting
		 */
		local_timer_interrupt(irq, dev_id);
	}
#endif /* CONFIG_MIPS_MT_SMTC */

	return IRQ_HANDLED;
}

/************************************************************************
*                   W A T C H D O G   T I M E R  
*************************************************************************
*/

irqreturn_t watchdog_timer_interrupt(int irq, void *dev_id)
{
	uint32 word;

	word = VPint(CR_TIMER_CTL); 
	word &= 0xffc0ffff;
	word |= 0x00200000;
	VPint(CR_TIMER_CTL)=word;

	printk("watchdog timer interrupt\n");

#ifdef CONFIG_TC3162_ADSL
    /* stop adsl */
	if (adsl_dev_ops)
	    adsl_dev_ops->set(ADSL_SET_DMT_CLOSE, NULL, NULL); 
#endif

	dump_stack();

	return IRQ_HANDLED;
}

static struct irqaction watchdog_timer_irqaction = {
	.handler = watchdog_timer_interrupt,
	.flags = IRQF_DISABLED ,
	.name = "watchdog",
};

static void watchdog_timer_dispatch(void)
{
	do_IRQ(TIMER5_INT);
}

/************************************************************************
*                   B U S  T I M E O U T  I N T E R R U P T  
*************************************************************************
*/

irqreturn_t bus_timeout_interrupt(int irq, void *dev_id)
{
	uint32 reg;

	/* read to clear interrupt */
	reg = VPint(CR_PRATIR);

	printk("bus timeout interrupt ERR ADDR=%08lx\n", VPint(CR_ERR_ADDR));
	dump_stack();	

#ifdef CONFIG_PCI
	pcieReset();
	pcieRegInitConfig();
	setahbstat(1);
#endif

	return IRQ_HANDLED;
}

static struct irqaction bus_timeout_irqaction = {
	.handler = bus_timeout_interrupt,
	.flags = IRQF_DISABLED ,
	.name = "bus timeout",
};

static void bus_timeout_dispatch(void)
{
	do_IRQ(BUS_TOUT_INT);
}

/************************************************************************
*                        T I M E R
*************************************************************************
*/

static void mips_timer_dispatch(void)
{
	do_IRQ(SI_TIMER_INT);
}

static void mips_perf_dispatch(void)
{
	do_IRQ(cp0_perfcount_irq);
}

/*
 * Redeclare until I get around mopping the timer code insanity on MIPS.
 */
extern int null_perf_irq(void);

extern int (*perf_irq)(void);

#if	defined(CONFIG_PCI) && defined(CONFIG_MIPS_TC3262)
extern void ahbErrChk(void);
#endif

irqreturn_t mips_timer_interrupt(int irq, void *dev_id)
{
	int cpu = smp_processor_id();

#if	defined(CONFIG_PCI) && defined(CONFIG_MIPS_TC3262)
	ahbErrChk();
#endif

#ifdef CONFIG_MIPS_MT_SMTC
	/*
	 *  In an SMTC system, one Count/Compare set exists per VPE.
	 *  Which TC within a VPE gets the interrupt is essentially
	 *  random - we only know that it shouldn't be one with
	 *  IXMT set. Whichever TC gets the interrupt needs to
	 *  send special interprocessor interrupts to the other
	 *  TCs to make sure that they schedule, etc.
	 *
	 *  That code is specific to the SMTC kernel, not to
	 *  the a particular platform, so it's invoked from
	 *  the general MIPS timer_interrupt routine.
	 */

	if (read_c0_cause() & (1 << 30)) {
		/*
		 * There are things we only want to do once per tick
		 * in an "MP" system.   One TC of each VPE will take
		 * the actual timer interrupt.  The others will get
		 * timer broadcast IPIs. We use whoever it is that takes
		 * the tick on VPE 0 to run the full timer_interrupt().
		 */
		if (cpu_data[cpu].vpe_id == 0) {
			timer_interrupt(irq, NULL);
		} else {
			write_c0_compare(read_c0_count() +
			                 (mips_hpt_frequency/HZ));
			local_timer_interrupt(irq, dev_id);
		}
		smtc_timer_broadcast(cpu_data[cpu].vpe_id);
	}
#else /* CONFIG_MIPS_MT_SMTC */
	if (cpu == 0) {
		/*
		 * CPU 0 handles the global timer interrupt job and process
		 * accounting resets count/compare registers to trigger next
		 * timer int.
		 */
		timer_interrupt(irq, NULL);
	} else {
		/* Everyone else needs to reset the timer int here as
		   ll_local_timer_interrupt doesn't */
		/*
		 * FIXME: need to cope with counter underflow.
		 * More support needs to be added to kernel/time for
		 * counter/timer interrupts on multiple CPU's
		 */
		write_c0_compare(read_c0_count() + (mips_hpt_frequency/HZ));

		/*
		 * Other CPUs should do profiling and process accounting
		 */
		local_timer_interrupt(irq, dev_id);
	}
#endif /* CONFIG_MIPS_MT_SMTC */

	return IRQ_HANDLED;
}

/*
 * Estimate CPU frequency.  Sets mips_hpt_frequency as a side-effect
 */
static unsigned int __init estimate_cpu_frequency(void)
{
	unsigned int count;
	unsigned long flags;
	unsigned int start;

	local_irq_save(flags);

	/* Start r4k counter. */
	start = read_c0_count();

	/* delay 1 second */
	delay1ms(1000);

	count = read_c0_count() - start;

	/* restore interrupts */
	local_irq_restore(flags);

	count += 5000;    /* round */
	count -= count%10000;

	mips_hpt_frequency = count;

	/* on 34K, 2 cycles per count */
	count *= 2;

	return count;
}

void __init tc3162_time_init(void)
{
	unsigned int est_freq;

	timerSet(1, TIMERTICKS_10MS, ENABLE, TIMER_TOGGLEMODE, TIMER_HALTDISABLE);

	est_freq = estimate_cpu_frequency ();

	if (isRT63165 || isRT63365) {
		/* enable CPU external timer */
		clocksource_mips.read = cputmr_hpt_read;
		mips_hpt_frequency = CPUTMR_CLK;

		mips_timer_ack = cputmr_timer_ack;
		/* Calculate cache parameters.  */
		cycles_per_jiffy =
			(mips_hpt_frequency + HZ / 2) / HZ;

		cputmr_hpt_timer_init();

		/* watchdog timer */
		/* set count down 3 seconds to issue interrupt */
   		VPint(CR_WDOG_THSLD) = (3 * TIMERTICKS_1S * SYS_HCLK) * 1000 / 2; 

		if (cpu_has_vint)
			set_vi_handler(TIMER5_INT, watchdog_timer_dispatch);
#ifdef CONFIG_MIPS_MT_SMTC
		setup_irq_smtc(TIMER5_INT, &watchdog_timer_irqaction, 0x0);
#else
		setup_irq(TIMER5_INT, &watchdog_timer_irqaction);
#endif

		/* setup bus timeout interrupt */
   		//VPint(CR_MON_TMR) |= ((1<<30) | (0xff));

		if (cpu_has_vint)
			set_vi_handler(BUS_TOUT_INT, bus_timeout_dispatch);
#ifdef CONFIG_MIPS_MT_SMTC
		setup_irq_smtc(BUS_TOUT_INT, &bus_timeout_irqaction, 0x0);
#else
		setup_irq(BUS_TOUT_INT, &bus_timeout_irqaction);
#endif
	} 

	printk("CPU frequency %d.%02d MHz\n", est_freq/1000000,
		   (est_freq%1000000)*100/1000000);

    cpu_khz = est_freq / 1000;
}

irqreturn_t mips_perf_interrupt(int irq, void *dev_id)
{
	return perf_irq();
}

static struct irqaction perf_irqaction = {
	.handler = mips_perf_interrupt,
	.flags = IRQF_DISABLED | IRQF_PERCPU,
	.name = "performance",
};

void __init plat_perf_setup(struct irqaction *irq)
{
	cp0_perfcount_irq = SI_PC_INT;

	if (cp0_perfcount_irq >= 0) {
		if (cpu_has_vint)
			set_vi_handler(cp0_perfcount_irq, mips_perf_dispatch);
#ifdef CONFIG_MIPS_MT_SMTC
		setup_irq_smtc(cp0_perfcount_irq, irq,
		               0x100 << cp0_perfcount_irq);
#else
		setup_irq(cp0_perfcount_irq, irq);
#endif /* CONFIG_MIPS_MT_SMTC */
		VPint(CR_INTC_IMR) |=  (1 << (SI_PC1_INT-1));
#ifdef CONFIG_SMP
		set_irq_handler(cp0_perfcount_irq, handle_percpu_irq);
#endif
	}
}

void __init plat_timer_setup(struct irqaction *irq)
{
	set_vi_handler (SI_TIMER_INT, mips_timer_dispatch);
	mips_cpu_timer_irq = SI_TIMER_INT;

	/* we are using the cpu counter for timer interrupts */
	if (isRT63165 || isRT63365) 
		irq->handler = cputmr_timer_interrupt;	/* we use our own handler */
	else
		irq->handler = mips_timer_interrupt;	/* we use our own handler */
#ifdef CONFIG_MIPS_MT_SMTC
	setup_irq_smtc(mips_cpu_timer_irq, irq, 0x100 << cp0_compare_irq);
#else
	setup_irq(SI_TIMER_INT, irq);
#endif /* CONFIG_MIPS_MT_SMTC */
	VPint(CR_INTC_IMR) |=  (1 << (SI_TIMER1_INT-1));
#ifdef CONFIG_SMP
	set_irq_handler(SI_TIMER_INT, handle_percpu_irq);
#endif

	plat_perf_setup(&perf_irqaction);
}

