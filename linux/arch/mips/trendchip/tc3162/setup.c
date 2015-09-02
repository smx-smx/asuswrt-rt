#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/delay.h>

#include <asm/mips-boards/prom.h>
#include <asm/reboot.h>
#include <asm/time.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/TCIfSetQuery_os.h>

extern void tc3162_time_init(void);
static void tc3162_machine_restart(char *command);
static void tc3162_machine_halt(void);
static void tc3162_machine_power_off(void);

void (*back_to_prom)(void) = (void (*)(void))0xbfc00000;

extern void timerSet(uint32 timer_no, uint32 timerTime, uint32 enable, uint32 mode, uint32 halt);
extern void timer_WatchDogConfigure(uint8 tick_enable, uint8 watchdog_enable);

#ifdef CONFIG_TC3162_ADSL
adsldev_ops *adsl_dev_ops = NULL;
EXPORT_SYMBOL(adsl_dev_ops);
#ifdef CONFIG_RALINK_VDSL
vdsldev_ops *vdsl_dev_ops = NULL;
EXPORT_SYMBOL(vdsl_dev_ops);
#endif
#endif

static void hw_reset(void)
{
#ifdef CONFIG_TC3162_ADSL
    /* stop adsl */
	if (adsl_dev_ops)
	    adsl_dev_ops->set(ADSL_SET_DMT_CLOSE, NULL, NULL); 
#endif

    /* stop each module dma task */
	VPint(CR_INTC_IMR) = 0x0;
	VPint(CR_TIMER_CTL) = 0x0;

	/* stop mac dma */
#ifndef CONFIG_MIPS_TC3262
	VPint(CR_MAC_MACCR) = 0;
#endif

	/* stop atm sar dma */	
	TSARM_GFR &= ~((1 << 1) | (1 << 0));

	/* reset USB */
	/* reset USB DMA */
	VPint(CR_USB_SYS_CTRL_REG) |= (1 << 31);
	/* reset USB SIE */
	VPint(CR_USB_DEV_CTRL_REG) |= (1 << 30);
	mdelay(5);

	/* restore USB SIE */
	VPint(CR_USB_DEV_CTRL_REG) &= ~(1 << 30);
	mdelay(5);
	VPint(CR_USB_SYS_CTRL_REG) &= ~(1 << 31);

#ifdef CONFIG_MIPS_TC3162U
	/*stop pcie*/
	VPint(CR_AHB_PCIC) &= 0x9fffffff;
	/*reset usb 2.0 device*/
	/*stop interrupt*/
	VPint(CR_USB20_INTR_ENABLE_REG) = 0x0;
	/*do usb reset*/
	VPint(CR_USB20_SYS_CTRL_REG) |= (1 << 31);
	mdelay(1);
	VPint(CR_USB20_SYS_CTRL_REG) &= ~(1 << 31);
	/*sw disconnect*/
	VPint(CR_USB20_DEV_CTRL_REG) |= (1 << 31);
#endif

	/* watchdog reset */
//#ifdef CONFIG_MIPS_TC3262
	timerSet(5, 10 * TIMERTICKS_10MS, ENABLE, TIMER_TOGGLEMODE, TIMER_HALTDISABLE);
	timer_WatchDogConfigure(ENABLE, ENABLE);

	while (1);
//#endif
}

#define	RAND_MAX		32767
/*_____________________________________________________________________________
**      function name: reverseLong
**      descriptions:
**          Reverse the bytes ordering of the input value.
**             
**      parameters:
**            ul: Specify the 4 bytes value that you want to reverse the ordering.
**             
**      global:
**            None
**             
**      return:
**           Reverse value.
**	     
**      call:
**            None
**      	
**      revision:
**      1. Here 2008/08/24
**____________________________________________________________________________
*/
static uint32
reverseLong(
	uint32 ul
)
{
	uint8 z[4];

	z[3] = *((uint8 *)&ul + 0);
	z[2] = *((uint8 *)&ul + 1);
	z[1] = *((uint8 *)&ul + 2);
	z[0] = *((uint8 *)&ul + 3);

	return *((uint32 *)z);
}/*end reverseLong*/

/*_____________________________________________________________________________
**      function name: scramble
**      descriptions:
**           Scramble the input 32bits value. 
**             
**      parameters:
**            None
**             
**      global:
**            None
**             
**      return:
**            After Scramble the value
**	     
**      call:
**            reverseLong
**      	
**      revision:
**      1. Here 2008/08/24
**____________________________________________________________________________
*/
static uint32
scramble(uint32 checkCode)
{
	uint32 a[6];

	a[1] = (checkCode & 0x0000001F) << 0x0C;
	a[2] = (checkCode & 0x03E003E0) << 0x01;
	a[3] = (checkCode & 0xF8000400) >> 0x0A;
	a[4] = (checkCode & 0x0000F800) << 0x10;
	a[5] = (checkCode & 0x041F0000) >> 0x0F;
	checkCode = a[1] + a[2] + a[3] + a[4] + a[5];

	/* ICQ's check code is little-endian. Change the endian style */
	checkCode = reverseLong(checkCode);

	return checkCode;
}/*end scramble*/

/*_____________________________________________________________________________
**      function name: rand
**      descriptions:
**           Random value generation. 
**             
**      parameters:
**            None
**             
**      global:
**            None
**             
**      return:
**            Random value
**	     
**      call:
**            timerVlrGet
**      	
**      revision:
**      1. Here 2008/08/24
**____________________________________________________________________________
*/
uint32
rand(void){
	uint32 vlr;
	timerVlrGet(1, vlr);
	scramble(vlr);
	return (vlr & RAND_MAX);
}/*end rand*/
EXPORT_SYMBOL(rand);

static void tc3162_machine_restart(char *command)
{
	printk("Machine restart ... \n");
	hw_reset();
	back_to_prom();
}

static void tc3162_machine_halt(void)
{
	printk("Machine halted ... \n");
	hw_reset();
	while (1);
}

static void tc3162_machine_power_off(void)
{
	printk("Machine poweroff ... \n");
	hw_reset();
	while (1);
}

static int panic_event(struct notifier_block *this, unsigned long event,
		       void *ptr)
{
	machine_restart(NULL);

	return NOTIFY_DONE;
}

static struct notifier_block panic_block = {
	.notifier_call = panic_event,
};

void __init plat_mem_setup(void)
{
	_machine_restart = tc3162_machine_restart;
	_machine_halt = tc3162_machine_halt;
	pm_power_off = tc3162_machine_power_off;

	board_time_init = tc3162_time_init;

	atomic_notifier_chain_register(&panic_notifier_list, &panic_block);
}
