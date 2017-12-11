/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
 * All Rights Reserved.
 *
 * TrendChip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of TrendChip Technologies, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * TrendChip Technologies, Corp.
 */
/******************************************************************************/
#ifndef _MAINTENANCE_H
#define _MAINTENANCE_H

#include "cfg_manager.h"
/*
#define ACCOUNT		"Account"
#define ACCOUNT_ENTRY 	"Account_Entry"
*/
#define ACCOUNT_PATH 	"/etc/passwd.conf"
#define WEBPASSWD_PATH 	"/etc/passwd"
#define CONSOLE_PATH 	"/etc/usertty"
#define SLAT "$1$"
#define ACCOUNT_DEFAULT_STRING "%s:%s:0:0:root:/root/:/bin/sh\n"
#define ACCOUNT_WEB_INDEX 0
#define ACCOUNT_CONSOLE_INDEX 1

/*
#define TIMEZONE_ENTRY 	"Timezone_Entry"
*/
#define TIMEZONE	"Timezone"
#define TZ_NTP_TYPE	0
#define TZ_PC_TYPE	1
#define TZ_MANUAL_TYPE	2
#if 0  /*Orginal ci-command mode */
#define TIMEZONE_PATH "/etc/timezone.conf"
#endif
#define NTP_PATH "/etc/ntp.sh"
#define NTP_DEF_SERVER 	"pool.ntp.org" //Paul modify 2013/10/9
#define NTP_DEF_SERVERIP 	"0.0.0.0"
//#define NTP_CMD	"/userfs/bin/ntpclient -s -c 3 -l -h %s &"
#if defined(TCSUPPORT_2NTP_SERVER) || defined(TCSUPPORT_3NTP_SERVER)
#define NTP_CMD	"/userfs/bin/ntpclient -s -c 1 -l -h "
#else
#define NTP_CMD	"/userfs/bin/ntpclient -s -i 3 -l -h "
#endif
#define TIMEZONE_ATTR_SERVER 	"SERVER"
#ifdef TCSUPPORT_2NTP_SERVER
//#define	NTP_DBUG
#define TIMEZONE_ATTR_SERVER2 	"SERVER2"
#endif
#ifdef TCSUPPORT_3NTP_SERVER
//#define	NTP_DBUG
#define TIMEZONE_ATTR_SERVER2 	"SERVER2"
#define TIMEZONE_ATTR_SERVER3 	"SERVER3"
#endif
#define MAX_DIFFERENT_TIME 30
#define CRON_PATH "/etc/crontabs/"
#define NTP_SUCCESS "Success"
#define NTP_FAIL "Fail"
enum NTP_STATUS{
	NTP_STATUS_SUCCESS = 0,
	NTP_STATUS_FAIL,
	NTP_STATUS_CONNECTING,
};

/*
#define AUTOEXEC	"Autoexec"
#define AUTOEXEC_ENTRY "Autoexec_Entry"
#define AUTOEXEC_ATTR "command%d"
*/
#define AUTOEXEC_PATH "/etc/autoexec.sh"

/*
#define MAC	"Mac"
#define MAC_ENTRY "Mac_Entry"
*/
#define MAC_PATH "/etc/mac.conf"
/*
#define LED	"Led"
#define LED_ENTRY "Led_Entry"
*/
#define LED_PATH "/userfs/led.conf"
#define LED_PROC_PATH "/proc/tc3162/led_def"

//Andy Chiu, 2014/11/06
#ifdef RTCONFIG_ARPSTORM
#define ARPSTORM_NODE		"Arpstorm"
#define ARPSTORM_ENTRY	"Entry"
#define ARPSTORM_RUNIN	"runin"
#endif

#if 0  /*Orginal ci-command mode */
#define _XOPEN_SOURCE

char *
crypt(const char*key, const char*slat);

int
account_read(mxml_node_t *node);
#endif

int
account_init(void);
int
account_write(mxml_node_t *top, mxml_node_t *parant);
int
account_verify(mxml_node_t *node);
int
account_boot(mxml_node_t *top);
int
account_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if 0  /*Orginal ci-command mode */
int
timezone_read(mxml_node_t *node);
#endif
int
timezone_init(void);
int
timezone_boot(mxml_node_t *top);
int
timezone_write(mxml_node_t *top, mxml_node_t *parant);
int
timezone_verify(mxml_node_t *node);
int
timezone_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#ifdef TCSUPPORT_REBOOT_SCHEDULE
int
rebootSchedule_init(void);
#endif

#if 0  /*Orginal ci-command mode */
int
mac_read(mxml_node_t *node);
#endif
int
mac_init(void);
int
mac_write(mxml_node_t *top, mxml_node_t *parant);
int
mac_verify(mxml_node_t *node);
int
mac_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if 0  /*Orginal ci-command mode */
int
led_read(mxml_node_t *node);
#endif
int
led_init(void);
int
led_write(mxml_node_t *top, mxml_node_t *parant);
int
led_verify(mxml_node_t *node);
int
led_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#if 0  /*Orginal ci-command mode */
int
autoexec_read(mxml_node_t *node);
#endif

int
autoexec_boot(mxml_node_t *top);
int
autoexec_init(void);
int
autoexec_write(mxml_node_t *top, mxml_node_t *parant);
int
autoexec_verify(mxml_node_t *node);
#if 0
int
autoexec_execute(mxml_node_t *node,char *name);
#endif

int
correct_sys_time(mxml_node_t *top);

void
correctPPPtime(int difference);

int
vendorCfgFile_init(void);

//Andy Chiu, 2014/11/06
#ifdef RTCONFIG_ARPSTORM
int arpstorm_boot(mxml_node_t *top);
int arpstorm_init(void);
#endif

#endif
