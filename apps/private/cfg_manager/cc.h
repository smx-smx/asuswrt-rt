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
#ifndef _CC_H
#define _CC_H


void start_cc(void* unused);
#ifndef PURE_BRIDGE
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
void restart_mldproxy(char *interface,int input_pvc,int cur_wan_isp,  int input_serv);
void reset_fork_or_not(int pvc, int serv);
void restart_nat(int PVC, int serv);
#else
void restart_mldproxy(char *interface,int input_pvc,int cur_wan_isp);
void reset_fork_or_not(int pvc);  /*rodney_20090420 add*/
void restart_nat(int PVC);
#endif
#ifdef QOS_DROP  /*Rodney_20091115*/
void qos_drop_restart(void);
#endif
#endif

#if defined(TCSUPPORT_WAN_ATM) || defined(TCSUPPORT_WAN_PTM) || defined(TCSUPPORT_WAN_ETHER)
void restart_ddns(int pvc);
#else
void restart_ddns();
#endif
void restart_PPP(int PVC);
void restart_dhcp(int pvc);
/*void upgrade_firmware(int sig);*/
#if defined(TCSUPPORT_ETH4_WAN_PORT)
#define ETH04_STATUS 		"/etc/eth04_status"
#define PORT4_LINK_STATUS "/etc/port4_link_status"
int check_eth04_status(void);
int check_port4_link_status(void);
#endif
void check_USB(void);
void check_LAN_port(void);
void check_WAN_port(void);
void check_reset(void);
#ifdef TCSUPPORT_WLAN
void checkWlanBtn(void);
void timecheck(void);
#endif
#ifdef WSC_AP_SUPPORT
void check_wpsbtn(void);
#endif
#if defined(TCSUPPORT_LED_BTN_CHECK)
#if defined(TCSUPPORT_BTN_CHECK)
#define PTS_NUMBER 5
void check_button_type(void);
#endif
#endif
void clock_sync(long uptime);
void start_upnp(char *ppp_interface);
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
void check_route(int PVC, int serv, int isp);
#else
void check_route(int PVC, int isp);
#endif
#if defined(TCSUPPORT_WLAN_8021X)
void rtdot1xd_start();
void rtdot1xd_stop();
#if defined(TCSUPPORT_WLAN_8021X_EXT)
void set_radius_ip();
#endif
#endif
/*krammer add for bug 1127*/
void dproxy_restart();

void dhcpRelay_restart(void);

void start_autoPVC();
void
close_autopvc();
int
read_and_set_bit_map(void);
int
autopvcIsActive();
void
commit_and_save_autopvc(int *replayce);
void
set_success_pvc(int pvcIndex,int vcpoolIndex);
void
autopvc_state_machine(int adslState);
#if defined(TCSUPPORT_WAN_ATM) || defined(TCSUPPORT_WAN_PTM) || defined(TCSUPPORT_WAN_ETHER)
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
int
get_defRouteServNum(void);
void
set_defRouteServNum(int serv);
#endif

void
set_AtmPtmFlag(int pvc);
int
get_defRouteNum(void);
void
set_defRouteNum(int pvc);
void
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
set_ipv6GwFlag(int pvc, int serv);
#else
set_ipv6GwFlag(int pvc);
#endif
#endif
#if defined(TCSUPPORT_WAN_ETHER)
void
set_Wan0DownFlag(int pvc);
#endif

#define IPADDRSTEPCOUNT 2
#define DEFAULTNETMASK "255.255.255.0"
#define DEFAULTDNSSERVER_PPPONDEMAND "192.168.1.123"


#define AUTOPVC_WAIT_TIME 4
#define MAX_REPEAT_TIME 1

#define AUTOPVC_STATE_STOP 0
#define AUTOPVC_STATE_PROBE_PVC 1
#define AUTOPVC_STATE_PROBE_VC_POOL 2
#define AUTOPVC_STATE_FINISH 3

#define LAN_MAC_KEYWORD "LAN_MAC="
#define MAC_PATH "/etc/mac.conf"


#if defined(TCSUPPORT_SNMP_FULL)
#define SNMP_SIGNAL_CMD	"killall -SIGUSR2 snmpd"
#endif

#ifdef TCSUPPORT_DSLITE
#define DSLITE_START_PATH "/usr/script/dslite_start.sh %s %s %s"
#endif
#ifdef TR068_LED
#define ISP_BRIDGE		"3"
void
check_internet_connect(int adsl_state);
#endif

#define LAN_PORT_NUM	4
#define LINK_UP		1
#define LINK_DOWN	0

#ifdef TCSUPPORT_IGMP_SNOOPING
#define BR_INDEX	0//br0
#endif

#ifdef RA_PARENTALCONTROL
typedef struct {
	unsigned char parental_enable;
	char parental_macaddr[18];
#if defined(TCSUPPORT_PARENTAL_URLBLOCK)
	unsigned char parental_blocktype;
	char parental_url[49];
#endif
	unsigned char parental_day[7];
	unsigned short parental_start[7];
	unsigned short parental_inter[7];
	unsigned char parental_block;
} parental_t;
void excute_parentalControl();
#endif/*RA_PARENTALCONTROL*/

#define ATMQOS_UPDATE_CMD		"echo %d >/proc/tc3162/tsarm_qosred_update"
#endif
