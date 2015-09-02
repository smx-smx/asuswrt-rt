#include <linux/errno.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/param.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/time.h>
#include <linux/types.h>

#include <asm/bootinfo.h>
#include <asm/cpu.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/mipsregs.h>
#include <asm/sections.h>
#include <asm/time.h>
#include <asm/tc3162/tc3162.h>

irqreturn_t watchdog_timer_interrupt(int irq, void *dev_id)
{
	uint32 word;

	word = VPint(CR_TIMER_CTL); 
	word &= 0xffc0ffff;
	word |= 0x00200000;
	VPint(CR_TIMER_CTL)=word;

	printk("watchdog timer interrupt\n");
	dump_stack();
	return IRQ_HANDLED;
}

static struct irqaction watchdog_timer_irqaction = {
	.handler = watchdog_timer_interrupt,
	.flags = IRQF_DISABLED ,
	.name = "watchdog",
};


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

    word = (timerTime * SYS_HCLK) * 1000 /2; 
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

static void tc3162_timer_ack(void)
{
	uint32 word;

	word = VPint(CR_TIMER_CTL);
	word &= 0xffc0ffff;
	word |= 0x00020000;
	VPint(CR_TIMER_CTL) = word; 
}

void __init tc3162_time_init(void)
{
	if(isRT63260){
		/* watchdog timer */
		/* set count down 5 seconds to issue interrupt */
		VPint(CR_WDOG_THSLD) = (5 * TIMERTICKS_1S * SYS_HCLK) * 1000 / 2;
						    
		setup_irq(TIMER5_INT, &watchdog_timer_irqaction);
	}

	//mips_timer_state = tc3162_timer_state;
	mips_timer_ack = tc3162_timer_ack;

	timerSet(1, TIMERTICKS_10MS, ENABLE, TIMER_TOGGLEMODE, TIMER_HALTDISABLE);
}

void __init plat_timer_setup(struct irqaction *irq)
{
	setup_irq(TIMER1_INT, irq);
}
