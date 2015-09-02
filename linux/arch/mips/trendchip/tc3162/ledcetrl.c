/*
** $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/linux/arch/mips/trendchip/tc3162/ledcetrl.c#1 $
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
** $Log: ledcetrl.c,v $
** Revision 1.1.1.1  2010/04/09 09:39:13  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.1.1.1  2009/12/17 01:43:39  josephxu
** 20091217, from Hinchu ,with VoIP
**
** Revision 1.1.1.1  2007/10/12 08:25:04  ian
** Fix Kernel for CVS crash
**
** Revision 1.3  2007/05/30 15:05:46  ian
** Remove the hard-coded LAN reset in initialization
**
** Revision 1.2  2007/05/11 03:24:17  here
** Support the  led.conf file to confiugre  Switch IC Model.
**
** Revision 1.1.1.1  2007/04/12 09:42:02  ian
** TCLinuxTurnkey2007
**
** Revision 1.3  2006/07/06 07:24:23  lino
** update copyright year
**
** Revision 1.2  2006/07/06 05:54:00  lino
** add kernel module support
**
** Revision 1.1.1.1  2005/11/02 05:45:16  lino
** no message
**
** Revision 1.5  2005/03/04 09:40:23  lino
** add high active gpio support
**
** Revision 1.4  2005/02/14 06:35:35  lino
** add led turn on/off in timer
**
** Revision 1.3  2004/08/09 12:26:22  lino
** add tc3162 support
**
*/

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>
#include <asm/addrspace.h>
#include <asm/tc3162/tc3162.h>	
#include <asm/tc3162/ledcetrl.h>	

#define printf	printk
#define HW_LEN 32
/*__________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      constant definition
%%________________________________________________________________________*/
//#define USE_LED_TASK			1

#define LED_MAX_NO				32

#define LED_SPEED_FAST_BNO		2
#define LED_SPEED_MED_BNO		5
#define LED_SPEED_SLOW_BNO		10
#define LED_SPEED_VERY_SLOW_BNO	20
#define LED_SPEED_SLOWLY_BNO	30
 
/*__________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      macro definition
%%________________________________________________________________________*/
#define LED_OFF(x) 		VPint(CR_GPIO_DATA) |= (1<<(x))	
#define LED_ON(x) 		VPint(CR_GPIO_DATA) &= ~(1<<(x))
#define LED_OEN(x)		do { (x > 15) ? (VPint(CR_GPIO_CTRL1) |= (1<<(x-16)*2)) : (VPint(CR_GPIO_CTRL) |= (1<<((x)*2))); VPint(CR_GPIO_ODRAIN) |= (1<<(x)); } while(0)

// debug macro
#if 1
#define LED_DBG(x) 		do { } while (0)
#define LED_DBG_W(x, y)	do { } while (0)
#define LED_DBG_B(x, y)	do { } while (0)
#define LED_DBG_L(x, y)	do { } while (0)
#else
static int led_dbg = 0;

#define LED_DBG 		if(led_dbg&0x01) dbg_pline_1
#define LED_DBG_W 		if(led_dbg&0x01) dbg_plinew_1
#define LED_DBG_B 		if(led_dbg&0x01) dbg_plineb_1
#define LED_DBG_L 		if(led_dbg&0x01) dbg_plinel_1
#endif
/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      Type Declaration
%%____________________________________________________________________________*/

typedef struct {
	uint8 gpio;
	uint8 mode;
	uint8 speed;
	uint8 onoff;
} ledctrl_t;

typedef struct {
	uint8 data;
	uint8 onoff;
	uint8 blink_no;
	uint8 def_blink_no;
} ledvar_t;

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%	data declaration
%%____________________________________________________________________________*/
/*----- global variables -----------------------------------------------*/

/*----- local variables -----------------------------------------------*/

static ledctrl_t ledCtrl[LED_MAX_NO];
static ledvar_t ledVar[LED_MAX_NO];
static uint8 ledDefBNO[4] = {
	LED_SPEED_FAST_BNO, 
	LED_SPEED_MED_BNO, 
	LED_SPEED_SLOW_BNO, 
	LED_SPEED_VERY_SLOW_BNO
};
static uint8 gpioOnOff[16];
static uint8 gpioNoAct[16];
static uint8 led_stop = 0;
static uint8	toggleflag = 1; // 0 = LED On; 1 = LED off
static uint8	CI_blink_flag = 0;

static ledctrl_t ledDefCtrl[LED_MAX_NO] = {
/* empty configuration */
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
{0,       0,       0,       0},
#ifdef CONFIG_MIPS_TC3262
{0,       0,       0,       0},
#else
{0,       1,       0,       0},
#endif

};

static struct timer_list led_timer;
static int ppp_led = 0;
/* detect reset button */
static struct timer_list reset_button_timer;
static int reset_button_stats = 0;

/*__________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%      function declaration
%%________________________________________________________________________*/

//static void ledTimer(void);
static void ledChk(void);
static void ledDump(void);
static void LED_Blink(void);

/*______________________________________________________________________________
**	ledInit
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ledInit(void)
{
	uint8  i;
	uint32 word;
#ifndef CONFIG_MIPS_TC3262
	/* LED USB, XDSLLINK, XDSLACT, SYS, ROM_WRPT: output pin */
	VPint(CR_GPIO_CTRL) = (1<<(5*2))|(1<<(3*2))|(1<<(2*2))|(1<<(1*2))|(1<<(0*2));
	VPint(CR_GPIO_ODRAIN) = (1<<5)|(1<<3)|(1<<2)|(1<<1)|(1<<0);

	// set gpio6 as input pin; gpio6 is reset button
	word = VPint(CR_GPIO_CTRL);
	word &= 0xffffcfff;
	VPint(CR_GPIO_CTRL) = word;
#endif
	for (i = 0; i < 16; i++) {
		gpioOnOff[i] = 0;
		gpioNoAct[i] = 0;
	}

	for (i = 0; i < LED_MAX_NO; i++) {
		ledCtrl[i].mode = LED_MODE_NOT_USED;

		ledVar[i].data = 0;
		ledVar[i].onoff = 0;
		ledVar[i].blink_no = 0; 
		ledVar[i].def_blink_no = 0;
	}

	memcpy(ledCtrl, ledDefCtrl, sizeof(ledctrl_t) * LED_MAX_NO);

	for (i = 0; i < LED_MAX_NO; i++) {
		if (ledCtrl[i].mode != LED_MODE_NOT_USED) 
			LED_OEN(ledCtrl[i].gpio);
	} 

	//VPint(CR_GPIO_DATA) |= ((1<<3)|(1<<2)|(1<<1)|(1<<0));
	for (i = 0; i < LED_MAX_NO; i++) {
		if ((ledCtrl[i].mode == LED_MODE_ONOFF) && (i != LED_LAN_RESET))  
			ledTurnOff(i);
	} 
}

static ledctrl_t sysBootLedCtrl;

/*______________________________________________________________________________
**	ledSysInitOn
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ledSysInitOn(void)
{ 
	uint8  i; 

	if (ledCtrl[LED_SYS_INIT_STATUS].mode == LED_MODE_NOT_USED)
		return;
	
	sysBootLedCtrl = ledCtrl[LED_SYS_BOOT_STATUS];
	ledCtrl[LED_SYS_BOOT_STATUS].mode = LED_MODE_NOT_USED;

	for (i = 0; i < LED_MAX_NO; i++) {
		if ((((ledCtrl[i].mode != LED_MODE_NOT_USED) && (ledCtrl[i].mode != LED_MODE_NOACT)) 
				|| (i == LED_SYS_BOOT_STATUS)) && (i != LED_LAN_RESET)) {
			if (ledCtrl[i].onoff)
				LED_OFF(ledCtrl[i].gpio);
			else
				LED_ON(ledCtrl[i].gpio);
			//LED_ON(ledCtrl[i].gpio);
		}
	}
	mdelay(500);
}


/*______________________________________________________________________________
**	ledSysInitOff
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ledSysInitOff(void) 
{
	uint8  i;  

	if (ledCtrl[LED_SYS_INIT_STATUS].mode == LED_MODE_NOT_USED)
		return;
	
	for (i = 0; i < LED_MAX_NO; i++) {
		if (((ledCtrl[i].mode != LED_MODE_NOT_USED) && (ledCtrl[i].mode != LED_MODE_NOACT)) 
				&& (i != LED_LAN_RESET)) {
			if (ledCtrl[i].onoff) 
				LED_ON(ledCtrl[i].gpio);
			else
				LED_OFF(ledCtrl[i].gpio);
			//LED_OFF(ledCtrl[i].gpio);
		}
	}
	ledCtrl[LED_SYS_BOOT_STATUS] = sysBootLedCtrl;
	//mdelay(500);
}

/*______________________________________________________________________________
**	ledTurnOn
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ledTurnOn(uint8 led_no)
{
	if (led_stop)
		return;

	switch(ledCtrl[led_no].mode) {
		case LED_MODE_NOT_USED:
			return;
		case LED_MODE_ONOFF:
			LED_DBG_B("\r\n on  led_no=", led_no);
			LED_DBG_B(" gpio=", ledCtrl[led_no].gpio);
			if (led_no == LED_SYS_STATUS) {					/* special case for sys led */
				if (ledCtrl[led_no].onoff) {
					LED_ON(ledCtrl[led_no].gpio);
					gpioOnOff[ledCtrl[led_no].gpio] = 1;
				} else {
					LED_OFF(ledCtrl[led_no].gpio);
					gpioOnOff[ledCtrl[led_no].gpio] = 0;
				}
			} else {
				if (ledCtrl[led_no].onoff) {
					LED_OFF(ledCtrl[led_no].gpio);
					gpioOnOff[ledCtrl[led_no].gpio] = 0;
				} else {
					LED_ON(ledCtrl[led_no].gpio);
					gpioOnOff[ledCtrl[led_no].gpio] = 1;
				}
			}
			break;
		case LED_MODE_BLINK:
			ledVar[led_no].data = 1;
			ledVar[led_no].def_blink_no = ledDefBNO[ledCtrl[led_no].speed];
			break;
		case LED_MODE_NOACT:
			ledVar[led_no].data = 1;
			//edVar[led_no].def_blink_no = ledDefBNO[LED_SPEED_VERY_SLOW];
			ledVar[led_no].def_blink_no = LED_SPEED_SLOWLY_BNO;
			break;
		case LED_MODE_PULSE:
			ledVar[led_no].data = 1;
			ledVar[led_no].onoff = 1;
			ledVar[led_no].def_blink_no = ledDefBNO[LED_SPEED_SLOW];
			break;
	}
}

/*______________________________________________________________________________
**	ledTurnOff
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by: 
**	call:
**	revision:
**____________________________________________________________________________*/
void ledTurnOff(uint8 led_no)
{
	if (led_stop)
		return;

	switch(ledCtrl[led_no].mode) {
		case LED_MODE_NOT_USED:
			return;
		case LED_MODE_ONOFF:
			LED_DBG_B("\r\n off led_no=", led_no);
			LED_DBG_B(" gpio=", ledCtrl[led_no].gpio);
			if (ledCtrl[led_no].onoff) {
				LED_ON(ledCtrl[led_no].gpio);
				gpioOnOff[ledCtrl[led_no].gpio] = 1;
			} else {
				LED_OFF(ledCtrl[led_no].gpio);
				gpioOnOff[ledCtrl[led_no].gpio] = 0;
			}
			break;
		case LED_MODE_BLINK:
			LED_DBG_B("\r\n blink off led_no=", led_no);
			LED_DBG_B(" gpio=", ledCtrl[led_no].gpio);
			ledVar[led_no].data = 0;
			if (ledCtrl[led_no].onoff) {
				LED_ON(ledCtrl[led_no].gpio);
				gpioOnOff[ledCtrl[led_no].gpio] = 1;
			} else {
				LED_OFF(ledCtrl[led_no].gpio);
				gpioOnOff[ledCtrl[led_no].gpio] = 0;
			}
			break;
		case LED_MODE_NOACT:
			ledVar[led_no].data = 0;
 			break;
   		case LED_MODE_PULSE:
			ledVar[led_no].data = 0;
			LED_OFF(ledCtrl[led_no].gpio);
			ledVar[led_no].onoff = 0;
			break;
	}
}

/*______________________________________________________________________________
**	ledGetMode
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by: 
**	call:
**	revision:
**____________________________________________________________________________*/
uint8 ledGetMode(uint8 led_no)
{
	return ledCtrl[led_no].mode;
}

/*______________________________________________________________________________
**	ledTimer
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ledTimer(unsigned long data)
{
	ledChk();
	mod_timer(&led_timer, jiffies + 10); 
}

/*______________________________________________________________________________
**	ledChk
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ledChk(void)
{
	uint8 i;

		if (led_stop)
			return;

		if (CI_blink_flag) {
			LED_Blink();
		}
		
		for (i = 0; i < LED_MAX_NO; i++) { 
			if ((ledCtrl[i].mode == LED_MODE_NOT_USED) ||
				(ledCtrl[i].mode == LED_MODE_ONOFF) ||
				((ledCtrl[i].mode == LED_MODE_PULSE) && (ledVar[i].data == 0)))
				continue;

			switch (ledCtrl[i].mode) {
				case LED_MODE_BLINK:
					if (++ledVar[i].blink_no >= ledVar[i].def_blink_no) {
						if (ledVar[i].data) {
							LED_DBG_B("\r\n data=1 i=", i);
							LED_DBG_B(" blink_no=", ledVar[i].blink_no);
							ledVar[i].data = 0;
							gpioOnOff[ledCtrl[i].gpio] ^= 1;
							if (gpioOnOff[ledCtrl[i].gpio]) {
								LED_ON(ledCtrl[i].gpio);
								LED_DBG(" on");
							} else {
								LED_OFF(ledCtrl[i].gpio);
								LED_DBG(" off");
							} 
							gpioNoAct[ledCtrl[i].gpio] = 0;
						} else {
							gpioNoAct[ledCtrl[i].gpio]++;
						}
						ledVar[i].blink_no = 0;
					} 
					break;
				case LED_MODE_NOACT:
					if (ledVar[i].data) {
						if ((ledCtrl[i].onoff && !gpioOnOff[ledCtrl[i].gpio]) ||
							(!ledCtrl[i].onoff && gpioOnOff[ledCtrl[i].gpio])) {
							if (gpioNoAct[ledCtrl[i].gpio] >= 1) {
								LED_DBG_B("\r\n noact data=1 i=", i);
								LED_DBG_B(" blink_no=", ledVar[i].blink_no);
								if (ledCtrl[i].onoff) {
									LED_ON(ledCtrl[i].gpio);
									gpioOnOff[ledCtrl[i].gpio] = 1;
									LED_DBG(" on");
								} else {
									LED_OFF(ledCtrl[i].gpio);
									gpioOnOff[ledCtrl[i].gpio] = 0;
									LED_DBG(" off");
								}
								LED_DBG_B(" gpioNoAct=", gpioNoAct[ledCtrl[i].gpio]);
							}
						}
					}
					break;
				case LED_MODE_PULSE:
					if (ledVar[i].onoff) {
						LED_ON(ledCtrl[i].gpio);
						ledVar[i].onoff = 0;
						ledVar[i].blink_no = 0;
					} else {
						if (++ledVar[i].blink_no >= ledVar[i].def_blink_no) {
							LED_OFF(ledCtrl[i].gpio);
							ledVar[i].data = 0;
						}
					}
					break;
			}
		}
}

/*______________________________________________________________________________
**	ledDump
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ledDump(void)
{
	uint8 i;

	printf("led_no\tgpio\tmode\tspeed\tonoff\r\n");
	for (i = 0; i < LED_MAX_NO; i++) {
		printf("%d\t%d\t%d\t%d\t%d\r\n",
			i,
			ledCtrl[i].gpio,
			ledCtrl[i].mode,
			ledCtrl[i].speed,
			ledCtrl[i].onoff);
	}
}

int
getLEDDef(char *stateLED)
{
	uint16	index=0;
	uint8 i;

	index += sprintf( stateLED+index, "led_no\tgpio\tmode\tspeed\tonoff\n");
	for (i = 0; i < LED_MAX_NO; i++) {
		index += sprintf(stateLED+index, "%d\t%d\t%d\t%d\t%d\n",
			i,
			ledCtrl[i].gpio,
			ledCtrl[i].mode,
			ledCtrl[i].speed,
			ledCtrl[i].onoff);
	}
	return index;
}

void LED_Blink(void) 
{
	uint8 i;
	if (toggleflag) {
		for (i = 0; i < LED_MAX_NO; i++) {
			if ((ledCtrl[i].mode != LED_MODE_NOT_USED) || (i == LED_SYS_BOOT_STATUS))
				LED_ON(ledCtrl[i].gpio);
		}
		toggleflag=0;
	}
	else {
		for (i = 0; i < LED_MAX_NO; i++) {
			if (ledCtrl[i].mode != LED_MODE_NOT_USED) 
				LED_OFF(ledCtrl[i].gpio);
		}
		toggleflag=1;
	}
}

void resetButtonTimer(unsigned long data)
{
	if ((VPint(CR_GPIO_DATA) & (1<<6)) == 0) {		/* reset button is pressed */
		reset_button_stats++;
	} else {
		reset_button_stats = 0;
	}
	mod_timer(&reset_button_timer, jiffies + (HZ/2)); 
}

static int led_def_read_proc(char *buf, char **start, off_t off, int count,
                 int *eof, void *data)
{
	int i;
	int len = getLEDDef(buf);
	if (len <= off+count) 
		*eof = 1;
	*start = buf + off;
	len -= off;
	if (len>count) 
		len = count;
	if (len<0) 
		len = 0;
	
	memcpy(ledCtrl, ledDefCtrl, sizeof(ledctrl_t) * LED_MAX_NO);

	for (i = 0; i < LED_MAX_NO; i++) {
		if (ledCtrl[i].mode != LED_MODE_NOT_USED) 
			LED_OEN(ledCtrl[i].gpio);
	} 
	
	return len;
}

static int led_def_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	int len;
	int set_info[5];
	char get_buf[HW_LEN];

/* do a range checking, don't overflow buffers in kernel modules */
  if(count > HW_LEN)
    len = HW_LEN;
  else
    len = count;

/* use the copy_from_user function to copy buffer data to
 * to our get_buf */
  if(copy_from_user(get_buf, buffer, len)) {
    return -EFAULT;
  }

/* zero terminate get_buf */
 get_buf[len]='\0';
 
 if (sscanf(get_buf, "%d %d %d %d %d", set_info, (set_info+1), (set_info+2), (set_info+3), (set_info+4)) != 5) {
		printf("usage: <led_no> <gpio> <mode> <speed> <onoff>\n");
	return count;
}

	//set ledDefCtrl information
	ledCtrl[set_info[0]].gpio  = set_info[1];
	ledCtrl[set_info[0]].mode  = set_info[2];
	ledCtrl[set_info[0]].speed = set_info[3];
	ledCtrl[set_info[0]].onoff = set_info[4];
	
	//reset LAN side LDE 
	LED_OEN(ledCtrl[set_info[0]].gpio);
	LED_ON(ledCtrl[set_info[0]].gpio);
	LED_OFF(ledCtrl[set_info[0]].gpio);
	gpioOnOff[ledCtrl[set_info[0]].gpio] = 0;
	
	return len;
}

static int led_ppp_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", ppp_led);

	len -= off;
	*start = page + off;

	if (len > count)
		len = count;
	else
		*eof = 1;

	if (len < 0)
		len = 0;

	return len;
}

static int led_ppp_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	char val_string[8];
	unsigned long val;

	if (count > sizeof(val_string) - 1)
		return -EINVAL;

	if (copy_from_user(val_string, buffer, count))
		return -EFAULT;

	val_string[count] = '\0';

	val = simple_strtoul(val_string, NULL, 10);
	if (val != 0)
		ppp_led = 1;
	else
		ppp_led = 0;

	if (ppp_led) {
		ledTurnOn(LED_PPP_STATUS);
		ledTurnOn(LED_PPP_NOACT_STATUS);
	} else {
		ledTurnOff(LED_PPP_STATUS);
		ledTurnOff(LED_PPP_ACT_STATUS);
		ledTurnOff(LED_PPP_NOACT_STATUS);
	}
	return count;
}

static int reset_button_proc_stats(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", reset_button_stats);

	len -= off;
	*start = page + off;

	if (len > count)
		len = count;
	else
		*eof = 1;

	if (len < 0)
		len = 0;

	return len;
}

static int __init tc3162_led_init(void)
{
	struct proc_dir_entry *led_proc;
	
	printk("TC3162 LED Manager 0.1 init\n");

	ledInit();

	init_timer(&led_timer);
	led_timer.expires = jiffies + (HZ * 2);
	led_timer.function = ledTimer;
	led_timer.data = 0x0;
	add_timer(&led_timer);

	/* led definition */
	led_proc = create_proc_entry("tc3162/led_def", 0, NULL);
	led_proc->read_proc = led_def_read_proc;
	led_proc->write_proc = led_def_write_proc;

	/* pppoe/pppoa led */
	led_proc = create_proc_entry("tc3162/led_ppp", 0, NULL);
	led_proc->read_proc = led_ppp_read_proc;
	led_proc->write_proc = led_ppp_write_proc;

	/* reset button status */
	init_timer(&reset_button_timer);
	reset_button_timer.expires = jiffies + (HZ * 2);
	reset_button_timer.function = resetButtonTimer;
	reset_button_timer.data = 0x0;
	add_timer(&reset_button_timer);

	create_proc_read_entry("tc3162/reset_button", 0, NULL, reset_button_proc_stats, NULL);

	return 0;
}

static void __exit tc3162_led_exit(void)
{
	printk("TC3162 LED Manager 0.1 exit\n");

	del_timer_sync(&led_timer);

	remove_proc_entry("tc3162/led_def", NULL);
	remove_proc_entry("tc3162/led_ppp", NULL);
}

EXPORT_SYMBOL(ledTurnOn);
EXPORT_SYMBOL(ledTurnOff);

module_init (tc3162_led_init);
module_exit (tc3162_led_exit);
