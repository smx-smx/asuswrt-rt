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
#ifndef _INTREFACE_H
#define _INTREFACE_H
#include "mxml.h"
#include "common.h"

#define VOIP_ADVANCED   "VoIPAdvanced"
#define VOIP_ATTR_PVCID "pvcId"

#define WAN_NAME  "WAN"
#define WAN_COMMON  "Common"
#define WAN_ATTR_TRANS_MODE  "TransMode"
#define WAN_ATM     "ATM"
#define WAN_PTM     "PTM"
#define WAN_ATM_NUM "0"
#define WAN_PTM_NUM "8"
#define WAN_ETHER   "Ethernet"
#define WAN_ETHER_NUM   "10"
#define WAN_USB_MODEM_NUM   "11"
#define WAN_ETHER_LAN   "LAN"
#define WAN_ETHER_LAN_NUM   "12"
#define WAN_PVC0    "PVC0"
#define WAN_PVC8    "PVC8"
#define WAN_PVC9    "PVC9" //Ren
#define WAN_PVC10   "PVC10"
#define WAN_PVC12   "PVC12"
// frank added 20110520
#define WAN_ETHER_IF  "nas10"
#define WAN_ETHER_LAN_IF  "nas12"
#define LAN_NAME  "LAN"
/*LAN Page */
#define WAN	"Wan"
#define WAN_PATH "/etc/isp%d.conf"
#define WAN_ISP	"ISP"
#define WAN_RESERVED "PVCScanReserved"
#define WAN_VPI "VPI"
#define WAN_VCI "VCI"
#define WAN_ENCAP "ENCAP"
#define WAN_LLC "LLC"
#define WAN_VC "VC"
/*#define WAN_STATE	"ACTIVE"*/
#define WAN_ACTIVE    "Yes"
#define WAN_DEACTIVE	"No"
#ifdef TCSUPPORT_IPV6_PRIVACYADDRS 
#define WAN_IPV6_PRIVACTADDRS	"IPv6Extension"
#endif
#if !defined(TCSUPPORT_C1_NEW_GUI) 
#define WAN_ENCAP_DYN   0
#define WAN_ENCAP_STATIC        1
#define WAN_ENCAP_PPP   2
#define WAN_ENCAP_BRIDGE        3
#define WAN_ENCAP_NONE    4  /*rodney_20090506 added*/
#endif

#ifdef TCSUPPORT_WPA_SUPPLICANT
#define WAN_CERT_PATH  "/usr/etc/802_1X/Certificates/%s"
#define WAN_CA_PATH    "/usr/etc/802_1X/CA/%s"
#define WAN_PKEY_PATH  "/usr/etc/802_1X/PKEY/%s"
#define WAN_PKEYPASSWD_PATH  "/usr/etc/802_1X/PKEYPASSWD/%s"
#endif

#ifdef IPV6 //jrchen add for IPV6
#define WAN_IPV6 "IP6"
#define WAN_PREFIXV6 "PREFIX6"
#define DHCP6C_PATH "/etc/dhcp6c_%s.conf"
#endif

#if defined(TCSUPPORT_ETH4_WAN_PORT)
#define PORT4AS2WAN_PATH "/etc/port4.conf"
#define PORT4_START_SH "/usr/script/port4_start.sh"
#define PORT4_STOP_SH "/usr/script/port4_stop.sh"
#endif


#ifdef TCSUPPORT_VLAN_TAG
#if !defined(TCSUPPORT_VTAG_ALLDEV)
#define WAN_8021Q_ACTIVE	"dot1q"
#define WAN_8021Q_VID	"VLANID"
#define WAN_PVC_IFNAME	"IFName"
#define WAN_IPOA_LLC	"1483 Routed IP LLC(IPoA)"
#define WAN_IPOA_VC	"1483 Routed IP VC-Mux"
#define WAN_PPPOA_LLC	"PPPoA LLC"
#define WAN_PPPOA_VC	"PPPoA VC-Mux"
#endif
#endif

#define LAN	"Lan"
#define LAN_ENTRY0	"Lan_Entry0"
#define LAN_ENTRY_0	"Entry0"
#define LAN_IP	"IP"
#define LAN_MASK	"netmask"
#define LAN_CFG_IP_INDEX	2
#define LAN_ATTR_MASK_INDEX	3
#define LAN_CFG_MASK_INDEX	4
#define LAN_IFCONFG_CMD  "/sbin/ifconfig"
#define LAN_IF_INDEX  0
#define LAN_MASK_NAME_INDEX  1
#define LAN_MASK_INDEX  2
#define LAN_IF  "br0"
#define LAN_PATH  "/etc/lanconfig.sh"
#define LAN_DHCP	"Dhcp"
#define LAN_DHCP_TYPE	"type"
#ifdef IPV6
#define LAN_IPV6	"IP6"
#define LAN_PREFIXV6    "PREFIX6"
//added by rclv 20100517

/* features of radvd*/
#define IPV6_CONFPATH "/etc/radvd.conf"
#define RADVD_PID_PATH	"/var/run/radvd.pid"
#define RADVD_START_CMD	"/userfs/bin/radvd -C /etc/radvd.conf -p /var/run/radvd.pid &"
#define PREFIX_SIMBOL "PrefixIPv6"
#define PREFIX_LEN_SIMBOL "Prefixv6Len"
#define IPV6_ENABLE_SIMBOL "Enable"

#define IPV6_RADVD 			"Radvd"
#define RADVD_MODE "Mode"
/* common features by radvd & dhcp6s */
#define PREFFERED_LIFETIME_SIMBOL "PreferredLifetime"
#define VALID_LIFETIME_SIMBOL "ValidLifetime"

/* dhcp6s feature */
#define DHCP6S_PID_PATH	"/var/run/dhcp6s.pid"
#define DHCP6S_START_CMD "/userfs/bin/dhcp6s -c /etc/dhcp6s.conf br0 -p /var/run/dhcp6s.pid &"
#define IPV6_DHCP6S			"Dhcp6s"
#define DHCP6S_CONFPATH "/etc/dhcp6s.conf"
#define DHCP6S_ENABLE_SIMBOL "Enable"
#define DHCP6S_MODE "Mode"
#define DNS1_SERVER "DNSserver"
#define DNS2_SERVER "SecDNSserver"

#define GLOBAL_IPV6_PATH  "/tmp/global_ipv6"
#define RADVD_PREFIX_PATH  "/tmp/del_radvd_prefix.sh"
#define DHCP6S_PREFIX_PATH  "/tmp/del_dhcp6s_prefix.sh"
//end of added code

#endif

#define DHCPDISABLEETH "/userfs/bin/ethphxcmd eth0 lanchip disable"
#define DHCPENABLEETH	"/userfs/bin/ethphxcmd eth0 lanchip enable"
#ifdef ALIAS_IP
#define LAN_ALIAS "LanAlias"
#define LAN_ALIAS_NUM 1
#define LAN_ALIAS_PATH "/etc/lanAlias%d.conf"
#define LAN_ALIAS_START_SH "/usr/script/lanAlias_start.sh"
#define LAN_ALIAS_STOP_SH "/usr/script/lanAlias_stop.sh"
#endif
#ifdef TCSUPPORT_WLAN_WDS
#define MAX_WDS_ENTRY 4
#endif

#define DHCPD_PATH "/etc/udhcpd.conf"
#ifdef DHCP_PROFILE
#define DHCPD_OPTION_PATH "/etc/udhcpd_option.conf"
#define SUB_NODE_OPT60_NAME "Option60"
#define MAX_VENDOR_CLASS_ID_LEN	64
#define SUB_NODE_OPT240_NAME "Option240"
#endif
#define DHCPD_STATIC_ARP_PATH		"/var/static_arp_tbl.sh"

#ifdef  TCSUPPORT_DHCP_PORT_FLT

void 
port_filter_rule(mxml_node_t *top, int type);


//#define	DHCP_PORT_DBG	
struct port_mask
{
	char *if_name;
	unsigned int if_mask;
};

/* lan device name */
#ifndef LAN_PORT_ITF_PREFIX
#define		LAN_PORT_ITF_PREFIX	"elan."	//eth0.
#endif
#define		LAN_DEV			"eth0"
#if 0
#define		LAN1			"eth0.1"
#define		LAN2			"eth0.2"
#define		LAN3			"eth0.3"
#define		LAN4			"eth0.4"
#else
#define		LAN1			"elan.1"
#define		LAN2			"elan.2"
#define		LAN3			"elan.3"
#define		LAN4			"elan.4"
#endif
#define		SSID1			"ra0"
#define		SSID2 			"ra1"
#define		SSID3			"ra2"
#define		SSID4			"ra3"
#define		USB0			"usb0"

#ifdef RTL8367B_SDK
#ifdef TCSUPPORT_CPU_RT63368	//for RT63368, DSL-N55U-C1, DSL-N16U
#define LAN_PORT_1			4	/* P4 */
#define LAN_PORT_2			3	/* P3 */
#define LAN_PORT_3			2	/* P2 */
#define LAN_PORT_4			1	/* P1 */
#else
#define LAN_PORT_1			1	/* P1 */
#define LAN_PORT_2			2	/* P2 */
#define LAN_PORT_3			3	/* P3 */
#define LAN_PORT_4			4	/* P4 */
#endif
#define LAN_PORT_1_MASK			(1U << LAN_PORT_1)
#define LAN_PORT_2_MASK			(1U << LAN_PORT_2)
#define LAN_PORT_3_MASK			(1U << LAN_PORT_3)
#define LAN_PORT_4_MASK			(1U << LAN_PORT_4)
#endif

/* lan device mask */
#define 	LAN1_MASK		0x10000000
#define 	LAN2_MASK		0x20000000
#define 	LAN3_MASK		0x30000000
#define 	LAN4_MASK		0x40000000
#ifdef TCSUPPORT_WLAN
#define 	SSID1_MASK		0x50000000
#define 	SSID2_MASK		0x60000000
#define 	SSID3_MASK		0x70000000
#define 	SSID4_MASK		0x80000000
#endif
#define 	USB0_MASK		0x90000000
#define		IF_MASK			0xf0000000

#define		ADD_PORT_FLT	1
#define		DEL_PORT_FLT	0

#define 	CMD_LEN			128
#define		BUF_LENTH		16
#endif

#define DHCPD_PUT_INS "%s %s\n"
#define DHCPD_ATTR_INDEX	0
#define DHCPD_VALUE_INDEX	1
#define DHCPD_ATTR_IF_INDEX	0
#define DHCPD_ATTR_LEASE_INDEX	1
#if 1//def STATIC_DHCP
#define DHCPD "Dhcpd"
#define MAX_STATIC_NUM 32
#endif
#define DHCPLEASE "DhcpLease"
#define MAX_LEASE_NUM 254
#define DHCPLEASE_PATH "/etc/udhcp_lease"
//added by rclv_nj 20100517
#define RADVDCONF_PATH  "/etc/radvd.conf"
// added end

/*Add for LanHost TR069 Nodes*/
#define MAX_LANHOST_NUM	254
#define LANHOST			"LanHost"
#define LEASENUM_ATTR		"LeaseNum"
#define WLAN_COMMON_NODE	"WLan_Common"
#define WIFIMACTAB_COMMON_NODE	"wifiMacTab_Common"
#define ARP_INFO_PATH		"/tmp/arp_info"
#define HOST_MODE_DHCP	0
#define HOST_MODE_STATIC	1
#define HOST_MODE_AUTOIP	1
#define LANHOST_GET_INTERVAL	3	//seconds

#define DHCPRELAY_PATH 	"/etc/config/dhcprelay.sh"
#define DHCPRELAY_CMD 	"/userfs/bin/dhcrelay -pf /var/log/dhcrelay.pid %s %s\n"

#define DPROXY          "Dproxy"  /*rodney_20090506 added*/
#define DPROXY_PATH 	"/etc/dproxy.conf"
#define DPROXYAUTO_PATH 	"/etc/dproxy.auto"
#define DPROXY_TMP_PATH "/etc/dproxy.conf.tmp"
#define DPROXY_TYPE_INDEX	0

#define IGMPPROXY_CONF_PATH  "/etc/igmpproxy.conf"  /*rodney_20090420 add*/
#define IGMPPROXY_DEA_PATH   "/userfs/bin/igmpproxy" /*rodney_20090420 add*/

/*#define ZEBRA_ENTRY 	"Zebra_Entry"*/
#define ZEBRA_PATH "/etc/zebra.conf"
#define ZEBRA_IF "interface"

#define LANRIPD	"LanRipd"
#define LANRIPD_PATH 	"/etc/lan_rip.conf"
/*#define LANRIPD_ENTRY 	"LanRipd_Entry"*/
#define RIPD_PATH 	"/etc/ripd.conf"
#define RIPD_VERSION	"RIPVERSION"
#define RIPD_DIRECTION	"DIRECTION"
#define RIPD_DIRECTION_NONE	"None"
#define RIPD_DEFAULT_LANIF	"br0"

#ifdef TCSUPPORT_IPV6_RIPNG
#define RIPNGD_PATH 		"/etc/ripngd.conf"
#define RIPNGD_ENABLE		"RIPNGENABLE"
#define RIPNGD_DIRECTION 	"RIPNGDIRECTION"
#define RIPNGD_ROUTE_CMD	"router ripng\n"
#define RIPNGD_ACCESS_DENY_CMD	"ipv6 access-list DENYV6 deny any\n"
#define RIPNGD_DENY_LIST	"distribute-list DENYV6 %s %s\n"
#define RIPNGD_REDISTRIBUTE_CMD	"redistribute connected\n"
#endif 

#define RIPD_VER1	"RIP1"
#define RIPD_NO_AUTH_MODE	"no ip rip authentication mode\n"
#define RIPD_ROUTE_CMD	"router rip\n"
#define RIPD_REDISTRIBUTE_CMD	"redistribute connected\n"
#define RIPD_NETWORK	"network"
#define RIPD_ACCESS_DENY_CMD	"access-list DENY deny any\n"
//Compare string is wrong. shnwind 20110415. 
#define RIPD_IN_ONLY_DIRECTION "IN Only"
#define RIPD_OUT_ONLY_DIRECTION "OUT Only"
#define RIPD_IN_DIRECTION "in"
#define RIPD_OUT_DIRECTION "out"
#define RIPD_V1	1
#define RIPD_V2	2
#define RIPD_IS_DENY 1

#define RIPD_MULTICAST "multicast"
#define RIPD_NO	"no"
#define RIPD_DENY_LIST	"distribute-list DENY %s %s\n"

/*WLAN page*/
#define WLAN	"WLan"
#define WLANSWITCHNAME  "APOn"
#define WLAN_COMMON "WLan_Common"
#define WLAN_ENTRY_0	"Entry0"
#define WLAN_BUTTON_PATH "/proc/tc3162/wlan_button"
#define WLAN_HT_EXTCHA	"HT_EXTCHA"
#define WLAN_CHANNEL  "Channel"
#define WLAN_HT_BW		"HT_BW"
#define NOT_AUTO_CHANNEL	0
#define IS_AUTO_CHANNEL	1

/*SysInfo node*/
#define SYSINFO "SysInfo"

/*Info node*/
#define INFO	"Info"
#define CURRENT_CHANNEL	"CurrentChannel"

/*
#define WLAN_ENTRY 	"WLan_Entry"
#define WLAN_KETSTR_K_CHAR	'K'
#define WLAN_KETSTR_S_CHAR	'S'
*/
#if	defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7610E) || defined(MT7612E) || defined(MT7601E) || defined(MT7592) || defined(RT5592)
//#define WLAN_PATH "/etc/Wireless/RT2860AP/RT2860AP.dat"

#if defined(TCSUPPORT_WLAN_RT6856) //For RT-6856. Javier
	#define WLAN_PATH_0 "/etc/Wireless/RT6856/iNIC_ap.dat"
	#define WLAN_PATH_1 "/etc/Wireless/RT6856/iNIC_ap1.dat"
	#define WIFI_2G "ra00_0"
	#define WIFI_5G "ra01_0"
#elif defined(TCSUPPORT_DUAL_WLAN_RT5592_RT3593)	//Dual band.
	#define WLAN_PATH_0 "/etc/Wireless/RT2860AP/RT2860AP0.dat"
	#define WLAN_PATH_1 "/etc/Wireless/RT2860AP/RT2860AP1.dat"
	#define WIFI_2G "ra0"
	#define WIFI_5G "rai0"
#elif defined(TCSUPPORT_WLAN_AC)
	#define WLAN_PATH_0 "/etc/Wireless/RT2860AP/RT2860AP.dat"
	#define WLAN_PATH_1 "/etc/Wireless/RT2860AP_AC/RT2860AP.dat"
	#define WIFI_2G "ra0"
	#define WIFI_5G "rai0"
#else
	#define WLAN_PATH_0 "/etc/Wireless/RT2860AP/RT2860AP.dat"
	#define WIFI_2G "ra0"
#if defined(DSL_N55U_D1)
	#define WLAN_PATH_1 "/etc/Wireless/RT2860AP/RT2860AP_5g.dat"
	#define WIFI_5G "rai0"
#endif
#endif

#else
	#define WLAN_PATH "/etc/Wireless/RT61AP/RT61AP.dat"
#endif

/*	//javi
#if defined(TCSUPPORT_WLAN_AC)
#define WLAN_AC_PATH "/etc/Wireless/RT2860AP_AC/RT2860AP.dat"
#endif
*/

#define RIP_INTERFACE 0
#define RIP_VERSION 1
#define RIP_DIRECTION 2

#ifdef TCSUPPORT_IPV6_RIPNG
#define RIPNG_INTERFACE 0
#define RIPNG_ENABLE 1
#define RIPNG_DIRECTION 2
#endif

//add by xyyou
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592)
#define	PHY_MODE_BG_MIXED	0
#define	PHY_MODE_B_ONLY	1
#define	PHY_MODE_G_ONLY	4
#define	PHY_MODE_N_ONLY	6
#define	PHY_MODE_GN_MIXED	7
#define	PHY_MODE_BGN_MIXED	9
#else
#define	PHY_MODE_BG_MIXED	0
#define	PHY_MODE_B_ONLY	1
#define	PHY_MODE_G_ONLY	2
#endif
//end xyyou

#if defined(TCSUPPORT_WLAN_AC)
#define PHY_MODE_11A_ONLY 2
#define PHY_MODE_11AN_MIXED 8
#define PHY_11VHT_N_A_MIXED 14 
#define PHY_11VHT_N_MIXED 15
#endif

#if 0  /*Orginal ci-command mode */
int
wan_read(mxml_node_t *node);
#endif


int
wan_boot(mxml_node_t *top);
int
wan_init(void);
int
wan_write(mxml_node_t *top, mxml_node_t *parant);
int
wan_verify(mxml_node_t *node);
int
wan_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
wan_check_filter(mxml_node_t *top);
#if 0  /*Orginal ci-command mode */
int
lan_read(mxml_node_t *node);
#endif

#if defined(TCSUPPORT_ETH4_WAN_PORT)
int
port4As2Wan_init(void);
int
port4As2Wan_write(mxml_node_t *top, mxml_node_t *parant);
int
port4As2Wan_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
port4As2Wan_verify(mxml_node_t *node);
#endif
int
lan_boot(mxml_node_t *top);
int
lan_init(void);
int
lan_write(mxml_node_t *top, mxml_node_t *parant);
int
lan_verify(mxml_node_t *node);
int
lan_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if 0  /*Orginal ci-command mode */
int
wlan_read(mxml_node_t *node);
#endif

#ifdef ALIAS_IP
int
lanAlias_boot(mxml_node_t *top);
int
lanAlias_init(void);
int
lanAlias_write(mxml_node_t *top, mxml_node_t *parant);
int
lanAlias_verify(mxml_node_t *node);
int
lanAlias_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
dhcp_main_alias(const char *mainMask, const char *mainIP, const char *aliasMask, const char *aliasIP, const char *startIP, short *main_or_alias);
#endif

#ifdef TCSUPPORT_WLAN
int
wlan_init(void);
int
wlan_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int
wlan_write(mxml_node_t *top, mxml_node_t *parant);
int
wlan_verify(mxml_node_t *node);
int
wlan_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
wlan_boot(mxml_node_t *top);
void
dowscd(mxml_node_t *top);
#ifdef TCSUPPORT_SYSLOG_ENHANCE
extern void check_wireless_card(char *retval);/* check the CPE has wireless card or not */
int check_wlan_exist_for_syslog(mxml_node_t *top);
#endif

#endif

#if 0  /*Orginal ci-command mode */
int
ripd_read(mxml_node_t *node);
#endif

int
ripd_init(void);
int
ripd_write(mxml_node_t *top, mxml_node_t *parant);
int
ripd_verify(mxml_node_t *node);
int
ripd_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#if 0  /*Orginal ci-command mode */
int
dproxy_read(mxml_node_t *node);
#endif


int
dproxy_init(void);
int
dproxy_write(mxml_node_t *top, mxml_node_t *parant);
int
dproxy_verify(mxml_node_t *node);
int
dproxy_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if 0  /*Orginal ci-command mode */
int
dhcpd_read(mxml_node_t *node);
#endif

int
dhcpd_boot(mxml_node_t *top);
int
dhcpd_init(void);
#ifdef STATIC_DHCP
int
dhcpd_read(mxml_node_t *top, char nodeName[][MAX_NODE_NAME], char *attr);
#endif
int
dhcpd_write(mxml_node_t *top, mxml_node_t *parant);
int
dhcpd_verify(mxml_node_t *node);
int
dhcpd_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
dhcpLease_init(void);
int
dhcpLease_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);

#if 0  /*Orginal ci-command mode */
int
dhcpRelay_read(mxml_node_t *node);
#endif

#ifdef IPV6

int
radvdCall_boot(mxml_node_t *top);
int
radvdCall_init(void);
int
radvdCall_write(mxml_node_t *top, mxml_node_t *parant);
int
radvdCall_verify(mxml_node_t *node);
int
radvdCall_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);



int
dhcp6sCall_boot(mxml_node_t *top);
int
dhcp6sCall_init(void);
int
dhcp6sCall_write(mxml_node_t *top, mxml_node_t *parant);
int
dhcp6sCall_verify(mxml_node_t *node);
int
dhcp6sCall_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int setRadvdFunc(mxml_node_t *top);
int setdhcp6sFunc(mxml_node_t *top);
#endif

#if defined(TCSUPPORT_IGMP_PROXY_V3)
int
igmpproxy_init(void);
int
igmpproxy_write(mxml_node_t *top, mxml_node_t *parant);
int
igmpproxy_verify(mxml_node_t *node);
int
igmpproxy_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

int
dhcpRelay_boot(mxml_node_t *top);
int
dhcpRelay_init(void);
int
dhcpRelay_write(mxml_node_t *top, mxml_node_t *parant);
int
dhcpRelay_verify(mxml_node_t *node);
int
dhcpRelay_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
create_rip_conf(mxml_node_t *tree);
///////*******shnwind add ***************///
#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7610E) || defined(MT7612E) || defined(MT7601E) || defined(MT7592)//add by xyyou
//#define WLAN_SCRIPT_PATH "/etc/Wireless/RT2860AP/WLAN_exec.sh"
#define WLAN_MCS_SCRIPT_PATH "/etc/Wireless/RT2860AP/WLAN_mcs_exec.sh"

#define RESTART_BOA_SCRIPT "/tmp/var/restart_boa.sh"	//Andy Chiu, 2015/06/15.
#if defined(TCSUPPORT_WLAN_RT6856)
#define WLAN_SCRIPT_PATH "/etc/Wireless/RT6856/WLAN_exec.sh"	//For RT-6856. Javier.20120813
#define WLAN_SCRIPT_PREFIX "/userfs/bin/iwpriv ra0%d_0 set %s=%s\n"	//For RT-6856. Javier.20120813
#define AIRPLAY_SCRIPT_PREFIX "/userfs/bin/iwpriv ra0%d_%d set IgmpAdd=01:00:5e:7f:ff:fa\n/userfs/bin/iwpriv ra0%d_%d set IgmpAdd=01:00:5e:00:00:09\n/userfs/bin/iwpriv ra0%d_%d set IgmpAdd=01:00:5e:00:00:fb\n"
#else /* Non-iNIC */
#if defined(TCSUPPORT_DUAL_WLAN)	//dual band.
#define WLAN_SCRIPT_PATH "/etc/Wireless/RT2860AP/WLAN_exec.sh"
#define WLAN_SCRIPT_PREFIX "/userfs/bin/iwpriv ra%d set %s=%s\n"
#define WLAN_SCRIPT_PREFIX_0 "/userfs/bin/iwpriv ra0 set %s=%s\n"
#define WLAN_SCRIPT_PREFIX_1 "/userfs/bin/iwpriv rai0 set %s=%s\n"
#define AIRPLAY_SCRIPT_PREFIX "/userfs/bin/iwpriv ra%d set IgmpAdd=01:00:5e:7f:ff:fa\n/userfs/bin/iwpriv ra%d set IgmpAdd=01:00:5e:00:00:09\n/userfs/bin/iwpriv ra%d set IgmpAdd=01:00:5e:00:00:fb\n"
#define AIRPLAY_SCRIPT_PREFIX_1 "/userfs/bin/iwpriv rai%d set IgmpAdd=01:00:5e:7f:ff:fa\n/userfs/bin/iwpriv rai%d set IgmpAdd=01:00:5e:00:00:09\n/userfs/bin/iwpriv rai%d set IgmpAdd=01:00:5e:00:00:fb\n"
#else
#define WLAN_SCRIPT_PATH "/etc/Wireless/RT2860AP/WLAN_exec.sh"
#define WLAN_SCRIPT_PREFIX "/userfs/bin/iwpriv ra%d set %s=%s\n"
#define AIRPLAY_SCRIPT_PREFIX "/userfs/bin/iwpriv ra%d set IgmpAdd=01:00:5e:7f:ff:fa\n/userfs/bin/iwpriv ra%d set IgmpAdd=01:00:5e:00:00:09\n/userfs/bin/iwpriv ra%d set IgmpAdd=01:00:5e:00:00:fb\n"
#endif
#endif

#define WLAN_SCRIPT_TEMPLATE "/userfs/bin/iwpriv %s set %s=%s\n"
#if defined(TCSUPPORT_WLAN_RT6856)
#define CONTROL_INTF_2G "ra00_0"
#define CONTROL_INTF_5G "ra01_0"
#else
#define CONTROL_INTF_2G "ra0"
#define CONTROL_INTF_5G "rai0"
#endif

#else
#define WLAN_SCRIPT_PATH "/etc/Wireless/RT61AP/WLAN_exec.sh"
#endif

//#define WLAN_APON_PATH "/etc/Wireless/RT61AP/WLAN_APOn"
#define WLAN_APON_PATH "/etc/Wireless/WLAN_APOn"
#define WAN_START_SH "/usr/script/wan_start.sh"
#define WAN_STOP_SH "/usr/script/wan_stop.sh"
#define ROUTE_PATH 	"/etc/route.sh"
#define ROUTE2_PATH 	"/etc/route2.sh"
#define AIRPLAY_SCRIPT_PATH "/etc/script/AirPlay.sh"
#define BSSIDKEYNAME	"Bssid_num=" /*krammer add for bug 1093*/
#define CHANNELKEYNAME	"Channel=" /*xyyou add for AutoChannel*/
#define WLAN_MAC_NUM 	8
#define KEY_NUM 4
#if 0
#define WLAN_MBSSID_ENTRY_NUM 14
#ifdef RT3390//add by xyyou
#ifdef WSC_AP_SUPPORT
#define WLAN_MBSSID_ENTRY_NUM 19
#else
#define WLAN_MBSSID_ENTRY_NUM 16
#endif
#else
#ifdef WSC_AP_SUPPORT
#define WLAN_MBSSID_ENTRY_NUM 18
#else
#define WLAN_MBSSID_ENTRY_NUM 15
#endif
#endif
#endif
#define WLAN_APON "1"
#define WLAN_APOFF "0"
#ifdef TCSUPPORT_WLAN_PERSSID_SWITCH
#define WLAN_SSID_ON "1"
#define WLAN_SSID_OFF "0"
#endif
int  write_wlan_config(mxml_node_t *top, int BssidNum, int freq);

//is not used anymore, mark it for better readibility
#if 0
int  write_wlan_exe_sh(mxml_node_t *top, int BssidNum);
#endif
int Is11nWirelessMode(mxml_node_t *top);
int txBurst_or_not(mxml_node_t *top, int BssidNum);
#ifdef WSC_AP_SUPPORT//add by xyyou	
int switch_wps(mxml_node_t *top);
int run_wps(mxml_node_t *top, int freq);
int wps_oob(mxml_node_t *top, int freq);
int isWPSRunning(mxml_node_t *top);
#endif
int reset_counter(mxml_node_t *top, int freq);
#ifdef WSC_AP_SUPPORT
int wps_genpincode(mxml_node_t *top, int freq);
#endif

int write_dhcpd_config(mxml_node_t *top, mxml_node_t *parant, char *configFile);

#ifdef TCSUPPORT_IGMP_SNOOPING
void operateIgmpSnooping(mxml_node_t *top);
#endif

#ifdef TCSUPPORT_MLD_SNOOPING
void operateMldSnooping(mxml_node_t *top);
#endif

void operateEtherMedia(mxml_node_t *top);

#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E) || defined(MT7592)
int
doWlanRate(mxml_node_t *top, char *Rate);
int
doWlanMcs(mxml_node_t *top, char *Mcs);
extern int
doCheckHtExtcha(mxml_node_t *top,int isAuto);
#endif

#if defined(TCSUPPORT_MULTI_LANGUAGE) || defined(TCSUPPORT_GENERAL_MULTILANGUAGE)
int LanguageSwitch_boot(mxml_node_t *top);
int LanguageSwitch_init(void);
int LanguageSwitch_write(mxml_node_t *top, mxml_node_t *parant);
int LanguageSwitch_verify(mxml_node_t *node);
int LanguageSwitch_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif
#ifdef TCSUPPORT_SSH
int
sshd_init(void);
int
sshd_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
sshd_boot(mxml_node_t *top);
#endif
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
void ppponDemandRestartDnsmasq(mxml_node_t *top,int pvc_number, int serv);
#else
void ppponDemandRestartDnsmasq(mxml_node_t *top,int pvc_number);

#endif
#if defined(TCSUPPORT_6RD)
#define IPV6RD_NAME "ipv6rd"
#define IPV6RD_SUBNODE_NAME "ipv6rd_Entry"
#define CE_IPV4_ADDR "CEIPv4Addr"
#define BR_IPV4_ADDR "BRIPv4Addr"
#define IPV4_MASK_LEN "IPv4MaskLen"
#define PREFIX_LEN "PrefixLen"
#define IPV6RD_PREFIX "Prefix"
#define LAN_PD "6rdPD"

#define IPV6RD_START_SH "/usr/script/ipv6rd_start.sh"

int
ipv6rd_boot(mxml_node_t *top);
int
ipv6rd_init(void);
int
ipv6rd_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif 
int
lanHost_init(void);
int
lanHost_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);

int wanduck_init(void);
int wanduck_boot(mxml_node_t *top);
int wanduck_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

void update_wan_state(char *prefix, int state, int reason);

int iptv_init(void);
int iptv_boot(mxml_node_t *top);
int iptv_write(mxml_node_t *top, mxml_node_t *parant);
int iptv_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#ifdef TCSUPPORT_WEBSERVER_SSL
int https_init(void);
int https_write(mxml_node_t *top, mxml_node_t *parant);
int https_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#ifdef RTCONFIG_DUALWAN
int dualwan_init(void);
int dualwan_boot(mxml_node_t *top);
int dualwan_write(mxml_node_t *top, mxml_node_t *parant);
int dualwan_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
void reset_bridge_wan(const int vc, const int serv);
#else
void reset_bridge_wan(const int vc);
#endif

#endif
