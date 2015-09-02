/*
** $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/linux/arch/mips/trendchip/tc3162/tcwdog.c#1 $
*/
/************************************************************************
 *
 *	Copyright (C) 2006 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
/*
** $Log: tcwdog.c,v $
** Revision 1.5  2011/09/23 02:04:50  shnwind
** Add rt63365 support
**
** Revision 1.4  2011/07/07 07:55:51  shnwind
** RT63260 & RT63260 auto_bench support
**
** Revision 1.3  2011/06/03 02:04:23  lino
** add RT65168 support
**
** Revision 1.2  2010/12/09 13:18:35  xmdai_nj
** #7955:When doing upgrade firmware in web page, it can not reboot.
**
** Revision 1.1.1.1  2010/04/09 09:39:13  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.1.1.1  2009/12/17 01:43:39  josephxu
** 20091217, from Hinchu ,with VoIP
**
** Revision 1.1.1.1  2007/04/12 09:42:02  ian
** TCLinuxTurnkey2007
**
** Revision 1.2  2006/07/06 07:24:23  lino
** update copyright year
**
** Revision 1.1.1.1  2005/11/02 05:45:19  lino
** no message
**
*/
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/smp_lock.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/tc3162/tc3162.h>
#include <asm/tc3162/TCIfSetQuery_os.h>

#ifdef CONFIG_WATCHDOG_NOWAYOUT
static int nowayout = 1;
#else
static int nowayout = 0;
#endif

#if 0
MODULE_PARM(nowayout,"i");
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default=CONFIG_WATCHDOG_NOWAYOUT)");
#endif

static int watchdog_enabled = 0;

extern void timer_Configure(uint8  timer_no, uint8 timer_enable, uint8 timer_mode, uint8 timer_halt);
extern void timerSet(uint32 timer_no, uint32 timerTime, uint32 enable, uint32 mode, uint32 halt);

extern void timer_WatchDogConfigure(uint8 tick_enable, uint8 watchdog_enable);

void watchDogReset(void)
{
#ifdef CONFIG_TC3162_ADSL
    /* stop adsl */
	if (adsl_dev_ops)
	    adsl_dev_ops->set(ADSL_SET_DMT_CLOSE, NULL, NULL); 
#endif

/*watchdog reset*/
	timerSet(5, 10 * TIMERTICKS_10MS, ENABLE, TIMER_TOGGLEMODE, TIMER_HALTDISABLE);
	timer_WatchDogConfigure(ENABLE, ENABLE);

	while(1);
	_machine_restart();
}

void tc3162wdog_kick(void)
{
	/* Clear Watchdog timer counter */
	uint32 word;

	if (isRT63165 || isRT63365 || isRT63260) {
		VPint(CR_WDOG_RLD) = 0x1;
	} else {
		word = VPint(CR_TIMER_CTL); 
		word &= 0xffc0ffff;
		word |= 0x00200000;
		VPint(CR_TIMER_CTL)=word;
	}
}
EXPORT_SYMBOL(tc3162wdog_kick);

/* handle open device */

static int tc3162wdog_open(struct inode *inode, struct file *file)
{
	/*	Allow only one person to hold it open */
	if(watchdog_enabled)
		return -EBUSY;
	if (nowayout) {
		printk("Watchdog cannot be stopped once started. \n");
	}
  
	timerSet(5, 2000 * TIMERTICKS_10MS, ENABLE, TIMER_TOGGLEMODE, TIMER_HALTDISABLE);
	timer_WatchDogConfigure(ENABLE, ENABLE);

	watchdog_enabled=1;
	printk("TC3162 hardware watchdog initialized\n");
	return 0;
}

static int tc3162wdog_release(struct inode *inode, struct file *file)
{
	/*
	 *	Shut off the watchdog
	 * 	Lock it in if it's a module and we set nowayout
	 */
	if (nowayout) {
		printk(KERN_CRIT "Watchdog cannot be stopped once started! \n");
	} else {
		/* Stop watchdog timer */
		timer_WatchDogConfigure(DISABLE, DISABLE);

		watchdog_enabled = 0;
		printk("TC3162 hardware watchdog stopped\n");
	}
	return 0;
}

static ssize_t tc3162wdog_write(struct file *file, const char *data, size_t len, loff_t *ppos)
{
	if (len) {
		tc3162wdog_kick();
	}
	return len;
}

static struct file_operations tc3162wdog_fops = {
	owner:		THIS_MODULE,
	write:		tc3162wdog_write,
	open:		tc3162wdog_open,
	release:	tc3162wdog_release,
};

static struct miscdevice tc3162wdog_miscdev = {
	minor:		WATCHDOG_MINOR,
	name:		"watchdog",
	fops:		&tc3162wdog_fops,
};

static int watchdog_reset_read_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data){

	return 0;
}
static int watchdog_reset_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	watchDogReset();	
	return 0;
}



static int __init tc3162_watchdog_init(void)
{
	struct proc_dir_entry *watchdog_proc;
	watchdog_proc = create_proc_entry("watchdog_reset", 0, NULL);
	watchdog_proc->read_proc = watchdog_reset_read_proc;
	watchdog_proc->write_proc = watchdog_reset_write_proc;

	misc_register(&tc3162wdog_miscdev);
	printk("TC3162 hardware watchdog module loaded.\n");
	return 0;
}

static void __exit tc3162_watchdog_exit(void)
{
	misc_deregister(&tc3162wdog_miscdev);
}

module_init(tc3162_watchdog_init);
module_exit(tc3162_watchdog_exit);

