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
#ifndef _WEB_H
#define _WEB_H

#ifdef TCSUPPORT_PRODUCTIONLINE
#include "../../public/mtd/tc_partition.h"
#endif

typedef signed char CHAR;
typedef signed short SHORT;
typedef signed int INT;
typedef signed long LONG;

typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned int UINT32;
typedef unsigned long ULONG;

//the index of dynamic display
#define DYN_DISP_MAIN_INDEX				0
#define DYN_DISP_MAIN_VOIP_INDEX			6

#define DYN_DISP_INTERFACE_INDEX			2
#define DYN_DISP_INTERFACE_WLAN_NDEX		3
#define DYN_DISP_ADVANCED_INDEX            3
#define DYN_DISP_ADVANCED_PVID_SETTING_INDEX    7

#define DYN_DISP_ACCESS_INDEX				4
#define DYN_DISP_ACCESS_CWMP_INDEX		6
#define DYN_DISP_ACCESS_CWMP_PARENTALCONTROL_INDEX	7

#define DYN_DISP_MAINTENANCE_INDEX		5
#define DYN_DISP_MAINTENANCE_SAVE_INDEX	6


#define ACCOUNT_NUM 3
#define ACCOUNT_NODE		"Account"
#define ACCOUNT_DISPLAY_MASK	"display_mask"

#define SEC_PER_DAY 86400 /*60*60*24  for ppp link-up time*/
#define SEC_PER_HOUR 3600 /*60*60         shnwind 2008.4.14*/
#define SEC_PER_MIN 60

#define MAX_BUF_SIZE	2048
#define ARRAY_NUM_LEN	48
#define ADSL_STATUS_UP	"up"

#define SYSTEMLOG_PATH	"/var/log/kmsg"
#ifndef PURE_BRIDGE
#define ETHERINFO_PATH	"/proc/tc3162/eth_stats"
#else
#define USBINFO_PATH	"/proc/tc3162/usb_link_state"
#endif
#define ADSLINFO_PATH	"/proc/tc3162/tsarm_stats"
#ifdef TCSUPPORT_WAN_PTM
#define VDSLINFO_B0_PATH "/proc/tc3162/ptm_b0_stats"
#define VDSLINFO_B1_PATH "/proc/tc3162/ptm_b1_stats"
#if 1 //defined(TCSUPPORT_CWMP_VDSL)
#define VDSL_INTERFACE_CFG_PATH "/proc/tc3162/vdsl_interface_config"
#endif
#ifdef TCSUPPORT_CPU_MT7510
#define VDSL_DIAGNOSTIC_PATH "/proc/tc3162/vdsl_diagnostic"
#endif /* TCSUPPORT_CPU_MT7510 */
#endif

#define ETHER_LAN		0
#define ETHER_WAN		1

#ifdef TCSUPPORT_WAN_PTM
#define OPMODE_ANSI_T1_413				"ANSI T1.413"
#define OPMODE_ITU_G_992_1_GDMT			"ITU G.992.1(G.DMT)"
#define OPMODE_ITU_G_992_2_GLITE		"ITU G.992.2(G.Lite)"
#define OPMODE_ITU_G_992_3_ADSL2		"ITU G.992.3(ADSL2)"
#define OPMODE_ITU_G_992_5_ADSL2PLUS	"ITU G.992.5(ADSL2PLUS)"
#define OPMODE_ITU_G_993_2_VDSL2		"ITU G.993.2(VDSL2)"
#endif

#define DSL_ADSL		0
#define DSL_VDSL		1

#ifdef TCSUPPORT_WAN_ETHER
//#define ETH1INFO_PATH "/proc/tc3162/eth1_stats"
#define ETH1INFO_PATH "/proc/tc3162/eth_stats"	//since Wan port come from eht0 now, Sam 2012/07/05
#endif

#define ADSL_FWVER_PATH	"/proc/tc3162/adsl_fwver"
#define ADSL_STATE_PATH	"/proc/tc3162/adsl_stats"
//#define WAN0_STATE_PATH "/proc/tc3162/eth1_link_st"
#define WAN0_STATE_PATH "/proc/tc3162/eth_link_st"	//since Wan port come from eht0 now, Sam 2012/07/05
#define WLANINFO_PATH	"/tmp/wlan_stats"
#if defined(CWMP) || defined(TCSUPPORT_SNMP_ATMADSL)
#define ADSL_STATE_SHOW_PATH "/proc/tc3162/adsl_stas_show"
#define ADSL_STATE_LAST_PATH "/proc/tc3162/adsl_stas_last"
#define ADSL_STATE_TOTAL_PATH "/proc/tc3162/adsl_stas_total"
#define ADSL_STATE_CURDAYST_PATH "/proc/tc3162/adsl_stas_curdaySt"
#define ADSL_STATE_QUTHOURST_PATH "/proc/tc3162/adsl_stas_quthourSt"
#endif
#if defined(TCSUPPORT_SNMP_ATMADSL)
#define ADSL_STATE_PREVDAYST_PATH "/proc/tc3162/adsl_stas_prevdaySt"
#define ADSL_STATE_INTERVALST_PATH "/proc/tc3162/adsl_stas_intervaldaySt"

#endif
/*xyzhu_091116:Get if stats from ifconfig,ra0-ra3/nas0-nas7*/
#define IFINFO_PATH		"/tmp/ifinfo_stats"
#define TIME_PATH	"/tmp/time"
#define SYSROUTE_PATH	"/tmp/route"
#define OAM_PING_PATH	"/proc/tc3162/oam_ping"
#define FWVER_PATH	"/etc/fwver.conf"
#define EXTENDNO_PATH	"/etc/extendno.conf"
/*xyzhu_100114: get default device info*/
#define DEVDEF_PATH	"/etc/devInf.conf"
#define DEFAULT_WAN_PATH	"/etc/defaultWan.conf"
#define DEFAULT_WAN	"DefaultWan"

#define SYSTEM		"System"
#define DEVICEINFO	"DeviceInfo"
#define DIAGNOSTIC	"Diagnostic"
#define WEBCURSET	"WebCurSet"
#define DYNDISP		"dynDisp"
#define WEB_ATTR_WAN_PVC    "wan_pvc"
#define WEB_ATTR_NAT_PVC    "nat_pvc"
#define WEB_ATTR_DIAG_PVC    "diag_pvc"
#define WEB_ATTR_DEV_PVC    "dev_pvc"
#ifdef TCSUPPORT_VLAN_TAG
#if !defined(TCSUPPORT_VTAG_ALLDEV)
#define DOT1QINFO	"dot1q"
#define DISP_WAN_DOT1Q	"disp_wan_8021q"
#endif
#endif

#ifdef TCSUPPORT_WPA_SUPPLICANT
#define TEMPSETTING "IPOEDot1xInfo"
#endif
#define WEBCURT     "WebCustom"
#define SUPPORTED_YES   "Yes"

#if defined(TCSUPPORT_SNMP_ATMADSL)
#define SNMP_SUPPORTED      "isSNMPSupported"
#endif

#if defined(TCSUPPORT_SNMP_FULL)
#define SNMP_FULL_SUPPORT   "isSNMPFullSupported"
#endif
#if !defined(TCSUPPORT_WAN_ATM) && !defined(TCSUPPORT_WAN_PTM) && !defined(TCSUPPORT_WAN_ETHER)
#define HAVE_ATM                    "haveAtm"
#define NO_WAN_MODE_DEFINED         "noWanModeDefined"
#else
#if defined(TCSUPPORT_WAN_ATM)
#define HAVE_ATM                    "haveAtm"
#endif
#if defined(TCSUPPORT_WAN_PTM)
#define HAVE_PTM                    "havePtm"
#endif
#if defined(TCSUPPORT_WAN_ETHER)
#define HAVE_WAN0                   "haveWan0"
#endif
#endif

/* frank added for hwnat qos bug */
#if defined(TCSUPPORT_HWNAT)
#define HAVE_HWNAT		     "haveHwnat"
#endif
/* end frank added for hwnat qos bug */








#if defined(TCSUPPORT_WEB_SAVE)
#define WEB_SAVE_SUPPORT	"isWebSaveSupport"
#endif


#define TIME_CMD	"/bin/date > /tmp/time"
#define SYSROUTE_CMD	"/sbin/route -n > /tmp/route"
#define IPVERSION_IPV4	0
#define IPVERSION_IPV6	1
#ifdef IPV6
#define IP6_SYSROUTE_CMD	"/sbin/route -A inet6 > /tmp/routeip6"
#define IP6_SYSROUTE_PATH	"/tmp/routeip6"
#define IP6_ROUTE_CMD "/usr/bin/ip -6 route > /tmp/ip6route"
#define IP6_ROUTE_PATH "/tmp/ip6route"
#ifdef TCSUPPORT_DSLITE
#define DSLITE_ADDR_TYPE 0
#define DSLITE_NAME_TYPE 1
#endif
#endif
#define SKBMGR_CMD	"echo %s > /proc/net/skbmgr_hot_list_len"
#define REBOOT_CMD	"/userfs/bin/mtd -r write %s romfile"
#define PING_CMD	"/bin/ping -c 1 -W 2 %s > %s"
/*krammer add*/
#define ISPCONFIG_PATH "/etc/isp%d.conf"
/*krammer add*/

#define ETH_PORT_STS    "/proc/tc3162/eth_port_status"
#define ETHLINK_PATH	"/proc/tc3162/eth_link_st"
//#define ETH1LINK_PATH   "/proc/tc3162/eth1_link_st"
#define ETH1LINK_PATH "/proc/tc3162/eth_link_st"	//since Wan port come from eht0 now, Sam 2012/07/05
#define ADSLSTATE_PATH	"/proc/tc3162/adsl_stats"
#define YAHOO_DNS	"www.yahoo.com"
#define TMP_PING_DNS_PATH "/tmp/pingDns"
#define TMP_PING_YAH_PATH "/tmp/pingYah"
#define TMP_PING_OTHER_PATH "tmp/pingOth"  /*rodney_20090506 added*/
#define TMP_PING_TS_PATH  "/tmp/pingTs"  /*rodney_20090507 added*/

#define ETHINFO_RXFRAM	"rxFrames"
#define ETHINFO_TXFRAM	"txFrames"
#ifndef PURE_BRIDGE
/*krammer add*/
#define ETHINFO_IP "ip"
#define ETHINFO_MAC "mac"
#else
#define USBINFO_IP "ip"
#define USBINFO_MAC "mac"
#endif
#define PPPoE_LLC "PPPoE LLC"
#define PPPoE_VC_Mux "PPPoE VC-Mux"
#define PPPoA_LLC "PPPoA LLC"
#define PPPoA_VC_Mux "PPPoA VC-Mux"
/*krammer add*/
//added by  renyu_20091223
#define CONNECT_TYPE_MANUALLY "Connect_Manually"
#define PPP_START_SH "/usr/script/ppp_start.sh"

#if defined (CWMP) && defined(TR111)
#define DEVICE_INFO_FILE         "/etc/devices.conf"
#define DEVICE_HOSTINDEX_INFO_FILE         "/etc/device_hostindex.conf"
#define COUNT_LENGTH 8
#define OUI_LENGTH 6
#define SN_LENGTH 64
#define PCLASS_LENGTH 64
#define HOSTCOUNT_LENGTH 8
#define HOSTINDEX_LENGTH 8*10+7
#endif

#define ATM_OAM_PING_SEGM	0
#define ATM_OAM_PING_E2E	1

/*shnwind add. for html_post_parser()*/
#define GET_BOUNDARY        0x01
#define GET_DESCRIBE    0x02
#define GET_CRLF            0x04
#define GET_DATA            0x08
#define GET_VAL             0x10

#define NOT_CONNECTED	"0"
#define CONNECTED	"1"
#define CONNECT_DISP_BUTTON "1"
#define CONNECT    1
#define DISCONNECT 2

#define RENEW_IP	1
#define RELEASE_IP	2
#ifdef	IPV6
#define RENEW_IP6	3
#define RELEASE_IP6	4
#define PREDELEG_ATTR	"prefix-delegation"
#define IP6WANIP	"IP6WanIP"
#endif

#define ROUTE_ATTR_METRIC "metric"
#define ROUTE_ATTR_USE "Use"

#define TEST_OK	"PASS"
#define TEST_SKIP "Skipped"
#define TEST_FAIL	"Fail"
#define RECV1ICMPPKT	"1 packets received"

#ifdef TCSUPPORT_IPV6_PRIVACYADDRS
#define IPV6_PRIVACYADDRS_PATH	"/tmp/privacyaddrs"
#endif
#define DEF_GW_FLAG	"UG"
#define TMP_IF_PATH	"/tmp/if.conf"
#define TMP_CHECK_IF "tmp/iface.conf"
#define DNS_INFO_PATH	"/etc/resolv.conf"
#define DNS_NAS_INFO_PATH   "/etc/resolv_%s.conf"
#define DNS_PPP_INFO_PATH   "/etc/ppp/resolv_%s.conf"
#define PPP_DNS_INFO_PATH	"/etc/ppp/resolv.conf"
#define DNS_ATTR	"nameserver"
#define ADDROUTE_CMD_GW	"route add -host %s gw %s dev %s"
#define DELROUTE_CMD_GW	"route del -host %s gw %s dev %s"
#define ADDROUTE_CMD	"route add -host %s dev %s"
#define DELROUTE_CMD	"route del -host %s dev %s"

#ifdef IPV6
#define UDHCP6C_PID_PATH	"/var/run/dhcp6c_%s.pid"
#define	DHCP6C_RESTART_CMD	"/userfs/bin/dhcp6c -c /etc/dhcp6c_%s.conf %s  -p  /var/run/dhcp6c_%s.pid"
#endif
#define UDHCPC_PID_PATH	"/var/run/udhcpc-nas%s.pid"
#if defined(TCSUPPORT_ETH4_WAN_PORT)
#define UDHCPC_ETH4_PID_PATH	"/var/run/udhcpc-elan.4.pid"
#endif
/*We should get wireless state by use  /userfs/bin/iwpriv ra0 stat. shnwind modify*/
//#define GET_WLAN_INFO_CMD	"/userfs/bin/iwpriv ra0 stat"
#if defined(TCSUPPORT_WLAN_RT6856)
#define GET_WLAN_INFO_CMD_0	"/userfs/bin/iwpriv ra00_0 stat"	//For RT-6856. Javier.20120813
#define GET_WLAN_INFO_CMD_1	"/userfs/bin/iwpriv ra01_0 stat"	//For RT-6856. Javier.20120813
#else	/* Non-iNIC */
#if defined(TCSUPPORT_DUAL_WLAN)	//dual band.
#define GET_WLAN_INFO_CMD_0	"/userfs/bin/iwpriv ra0 stat"
#define GET_WLAN_INFO_CMD_1	"/userfs/bin/iwpriv rai0 stat"
#else
#define GET_WLAN_INFO_CMD	"/userfs/bin/iwpriv ra0 stat"
#endif
#endif
/*krammer add 2008 11 4*/
#define ISEXIST "isExist"
/*krammer add*/

/*System cfg node*/
#define BOOT2CURSET	1
#define BOOT2DEFSET	2
#define WARM_REBOOT	3
#define ATEBOOT2DEFSET	4
#define BAKROM_ON	1
#define UG_ROMFILE	1
#if 0
#define UG_KERNEL	2
#define UG_FS	3
#endif
#define UG_TCLINUX	4

/*shnwind add for firmware upgrade.2009.4.3*/
#define NO_OPERATION  	0
#define NO_HEADER	1
#define HTML_HEADER	2
#define HTMLFINDEND_TEMP_LEN 4096
#define STRING_BUF_SIZE	256
#define MULTI_TO_NORMAL_TEMP "/tmp/parse_temp"
/*end System cfg node*/

/*pork 20090309 added*/
#define SYS_INFO_NODE_NAME "SysInfo"
#define SYSTEMSTARTUPTIME "sysStartTime"
#define INTERLEAVEMODE "0"
#define FASTMODE "1"
#define ADSLNODE "Adsl"
#define SUB_NODE_NAME "Entry"
#define ADSL_ATTR_MODE	"MODULATIONTYPE"
#define ADSL_ATTR_TYPE	"ANNEXTYPEA"
#define MODULATIONTYPE_T1_413 "T1.413"
#define MODULATIONTYPE_G_DMT "G.DMT"
#define MODULATIONTYPE_G_LITE "G.lite"
#define MODULATIONTYPE_ADSL2 "ADSL2"
#define MODULATIONTYPE_ADSL2PLUS "ADSL2+"
#define MTENSTANDARD_ADSL2 "G.dmt.bis"
#define MTENSTANDARD_ADSL2PLUS "G.dmt.bisplus"
#define ANNEXTYPEA_L	"Annex L"
#define ANNEXTYPEA_M	"Annex M"
#define INIT			"0"
//added by xyzhu_nj_20091209
#define GET_WLAN_MAC_TABLE	"/userfs/bin/iwpriv ra%d get_mac_table > %s"
#define WLAN_MAC_TABLE_PATH		"/tmp/mac_table_info"
#define ARP_TABLE_PATH			"/tmp/arp_tab"
#define MAX_MAC_ENTRY	32
#define WIFIMACTABLE	"wifiMacTab"
#define WIFIMACTABLE_NUM	"NUM"
#define WIFIMACTABLE_IP		"IP"
#define WIFIMACTABLE_MAC	"MAC"
#define COMMON			"Common"
#define ISCHANGED		"0"
#define	MACTABLEDATA	"1"

//added by ybyue

#define WANDUCK_NODE_NAME	"Wanduck"
#define WANDUCK_SUBNODE_NAME	"Common"

//add by brian
//#ifdef CWMP
#if defined(CWMP) || defined(RTCONFIG_TR069)
#define DSL_DIAGNOSTIC_PATH "/proc/tc3162/adsl_cwmp_diagnostic"
#define MAX_SPLIT_LEN 		512
#define MAX_SPLIT_OFFSET 	9// mod 512
#define CWMPDSLDIAGNOSTIC	"CDSLDiagnostic"
#define DIAGNOSTICSUB		"Common"
//because the statistic data is very large,so we use more proc file to transfer from kernal space to user space
#define OTHER_PROFILE_INDEX 		0
#define HLINpsds_PROFILE_INDEX_1 	1
#define HLINpsds_PROFILE_INDEX_2 	2
#define QLINpsds_PROFILE_INDEX 		3
#define SNRpsds_PROFILE_INDEX 		4
#define BITSpsds_PROFILE_INDEX 		5
#define GAINSpsds_PROFILE_INDEX 	6

#define ATM_F5LOOPBACK_DIAGNOSTIC_PATH "/proc/tc3162/atm_f5_loopback_diagnostic"
#define ATM_F5LOOPBACK_DIAGNOSTIC_RESET_PATH "/proc/tc3162/atm_f5_loopback_reset_diagnostic"
#define CWMPATMDIAGNOSTIC	"CATMDiagnostic"

#endif

#ifdef TCSUPPORT_PRODUCTIONLINE
#define PRODUCTLINECWMPFILEPATH "/tmp/productlinecwmpfile"
#define PRODUCTLINEPARAMAGIC 0x12344321

#define PL_CP_PRODUCTCLASS_FLAG (1<<0)
#define PL_CP_MANUFACUREROUI_FLAG (1<<1)
#define PL_CP_SERIALNUM_FLAG (1<<2)


#define PRDDUCTCLASSLEN 64
#define MANUFACUREROUILEN 64
#define SERIALNUMLEN 128
#if !defined(TCSUPPORT_PRODUCTIONLINE_DATE)
#define RESERVELEN (PROLINE_CWMPPARA_RA_SIZE-PRDDUCTCLASSLEN-MANUFACUREROUILEN-SERIALNUMLEN-2*sizeof(int))
#endif

typedef struct _proline_Para{
	int flag;
	char productclass[PRDDUCTCLASSLEN];
	char manufacturerOUI[MANUFACUREROUILEN];
	char serialnum[SERIALNUMLEN];
	char reserve[RESERVELEN];
	int magic;
} proline_Para;
#endif


typedef enum ifaceType
{
		ifra0=0,
		ifra1,
		ifra2,
		ifra3,
		ifnas0,
		ifnas1,
		ifnas2,
		ifnas3,
		ifnas4,
		ifnas5,
		ifnas6,
		ifnas7,
#if defined(TCSUPPORT_WAN_PTM)
        ifnas8,
        ifnas9,
#endif
#if defined(TCSUPPORT_WAN_ETHER)
        ifnas10,
#endif
		ifeth0,
}ifaceType;
/*
#define PROC_TSARM_PKTS_CLEAR_LOCATION "/proc/tc3162/tsarm_pktsclear"
#define PROC_ETHER_PKTS_CLEAR_LOCATION "/proc/tc3162/eth_pktsclear"
*/
//yzwang_20091208
#define AUTOPVC_STATE_STOP 0
#define AUTOPVC_STATE_PROBE_PVC 1
#define AUTOPVC_STATE_PROBE_VC_POOL 2
#define AUTOPVC_STATE_FINISH 3



/*function prototype*/
#ifdef TCSUPPORT_MODEL_NAME_CUSKIT
typedef struct {
	long addr;
	char* val;
} cuskitInfo_map_t;

void
GetCuskitInfoFromBootloader(long addr, char *cuskit_info);

int
sysInfo_boot(mxml_node_t *top);
#endif

int
sysInfo_write(mxml_node_t *top,mxml_node_t *parant);

int
sysInfo_init(void);

int sysInfo_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
/*pork 20090309 added over*/

int
system_init(void);
int
system_init_boot(mxml_node_t *top);
int
system_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int
system_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
diagnostic_init(void);
int
diagnostic_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int
diagnostic_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
deviceInfo_init(void);
int
deviceInfo_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int
deviceInfo_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
dynDisp_init(void);
int
dynDisp_boot(mxml_node_t *node);
int
dynDisp_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);

int
webCurSet_init(void);
int
webCurSet_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
#if 0
int
sysRoute_read(mxml_node_t *top, char *name);

int
systemLog_read(mxml_node_t *top, char *name);
#endif


int
info_init(void);

int
info_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);


#ifndef PURE_BRIDGE
// frank modify 20110520
int
etherInfo_read(mxml_node_t *node, char flag);
#else
int
usbInfo_read(mxml_node_t *node);
#endif
int
adslInfo_read(mxml_node_t *top,mxml_node_t *node);
#if defined(CWMP) || defined(TCSUPPORT_SNMP_ATMADSL)
int
adslStatsShow_read(mxml_node_t *top,mxml_node_t *node);
int
adslStatsLast_read(mxml_node_t *top,mxml_node_t *node);
int
adslStatsTotal_read(mxml_node_t *top,mxml_node_t *node);
int
adslStatsCurdaySt_read(mxml_node_t *top,mxml_node_t *node);
int
adslStatsQutHourSt_read(mxml_node_t *top,mxml_node_t *node);
#endif
#if defined(TCSUPPORT_SNMP_ATMADSL)
int
adslStatsPrevdaySt_read(mxml_node_t *top, mxml_node_t *node);
int
adslStatsIntervalSt_read(mxml_node_t *top, mxml_node_t *node);
#endif
int
wlanInfo_read(mxml_node_t *node);

int
autoPVCInfo_read(mxml_node_t *node);//yzwang_20091208
int
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
checkPpp(int cur_pvc, int cur_serv);
#else
checkPpp(int cur_pvc);
#endif
//added by xyzhu_nj_20091209
int
ifInfo_read(mxml_node_t *node, ifaceType type);
int devDefInf_read(mxml_node_t *node);
int
wifiMacTab_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
#if defined (CWMP) && defined(TR111)
int
dhcpClientLimit_init(void);
int
dhcpClient_init(void);
int
dhcpClient_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
unsigned long
get_dhcpClient_count_vlaue(char *str, char *keyword, int length, int base);
int
get_dhcpClient_Info_vlaue(char *str, char *keyword, int length, char *val);
#endif
//#ifdef CWMP
#if defined(CWMP) || defined(RTCONFIG_TR069)
//add by brian for dsl diagnostic
int
cwmp_dsl_diagnostic_init(void);
int
cwmp_dsl_diagnostic_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
void
splitValue(char *strbuf,char*attr,mxml_node_t *curNode);
//add by brian for atm f5 loopback diagnostic
int
cwmp_f5loopback_diagnostic_init(void);
int
cwmp_f5loopback_diagnostic_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int
IPInterface_init(void);

#endif
#ifdef TCSUPPORT_WLAN
#if defined(TCSUPPORT_WLAN_MAXSTANUM_GUI)
#if (defined(RT3390) && defined(VENDOR_FEATURE3_SUPPORT)) || (defined(RT5392) && defined(BB_SOC)) || (defined(MT7601E) && defined(BB_SOC)) || (defined(MT7592) && defined(BB_SOC))
#define MAX_STA_NUM "7"
#else
#define MAX_STA_NUM "31"
#endif
#endif
//added by xyzhu_nj_20091209
int
wifiMacTab_init(void);

int
getIpFromArp(char *macAttr, char *ip);

#if defined(RT3390) || defined(RT3090) || defined(RT5392) || defined(MT7601E)|| defined(MT7592)
#define MAX_LEN_OF_MAC_TABLE 32
#define MAC_ADDR_LENGTH		6
#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN		6
#endif
#define MAX_NUMBER_OF_MAC	64

#define MODE_CCK		0
#define MODE_OFDM		1
#define MODE_HTMIX		2
#define MODE_HTGREENFIELD	3
#if defined(MT7592) || defined(MT7612E) || defined(MT7610E)
#define MODE_VHT		4	//Andy Chiu, 2015/08/25
#endif

//Andy Chiu, 2015/08/25
#if defined(MT7592) || defined(MT7612E) || defined(MT7610E)
#define BAND_WIDTH_20		0
#define BAND_WIDTH_40		1
#define BAND_WIDTH_80		2
#define BAND_WIDTH_BOTH	3
#define BAND_WIDTH_10		4	/* 802.11j has 10MHz. This definition is for internal usage. doesn't fill in the IE or other field. */
#endif

#define BW_20			0
#define BW_40			1
#define BW_BOTH			2
#define BW_10			3


// MIMO Tx parameter, ShortGI, MCS, STBC, etc.  these are fields in TXWI. Don't change this definition!!!
//Remember to sync with WiFi_driver/include/oid.h if WiFi driver is updated.
//Andy Chiu, 2015/06/30. collect these.
#if defined(TCSUPPORT_WLAN_RT6856)
typedef union  _MACHTTRANSMIT_SETTING {
	struct  {
	unsigned short	BW:1;	//channel bandwidth 20MHz or 40 MHz
	unsigned short	MCS:7;	// MCS
	unsigned short	MODE:2;	// Use definition MODE_xxx.
	unsigned short	rsv:3;
	unsigned short	STBC:2;	//SPACE
	unsigned short	ShortGI:1;
	} field;
	unsigned short	word;
 } MACHTTRANSMIT_SETTING, *PMACHTTRANSMIT_SETTING;

typedef struct _RT_802_11_MAC_ENTRY {
	UCHAR ApIdx;
	UCHAR Addr[ETHER_ADDR_LEN];
	UCHAR Aid;
	UCHAR Psm;	/* 0:PWR_ACTIVE, 1:PWR_SAVE */
	UCHAR MimoPs;	/* 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled */
	CHAR AvgRssi0;
	CHAR AvgRssi1;
	CHAR AvgRssi2;
	UINT32 ConnectedTime;
	MACHTTRANSMIT_SETTING TxRate;
	MACHTTRANSMIT_SETTING MaxRate;
} RT_802_11_MAC_ENTRY, *PRT_802_11_MAC_ENTRY;
#elif defined(MT7592)
typedef union _MACHTTRANSMIT_SETTING {
	//Andy Chiu, 2015/08/25. update date structure.
	struct {
		USHORT MODE:3;	/* Use definition MODE_xxx. */
		USHORT iTxBF:1;
		USHORT eTxBF:1;
		USHORT STBC:1;	/* only support in HT/VHT mode with MCS0~7 */
		USHORT ShortGI:1;
		USHORT BW:2;	/* channel bandwidth 20MHz/40/80 MHz */
		USHORT ldpc:1;
		USHORT MCS:6;	/* MCS */
	} field;
	USHORT word;
} MACHTTRANSMIT_SETTING, *PMACHTTRANSMIT_SETTING;

typedef struct _RT_802_11_MAC_ENTRY {
	UCHAR ApIdx;
	UCHAR Addr[MAC_ADDR_LENGTH];
	UCHAR Aid;
	UCHAR Psm;		/* 0:PWR_ACTIVE, 1:PWR_SAVE */
	UCHAR MimoPs;		/* 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled */
	CHAR AvgRssi0;
	CHAR AvgRssi1;
	CHAR AvgRssi2;
	UINT32 ConnectedTime;
	MACHTTRANSMIT_SETTING TxRate;
	UINT32 LastRxRate;
/*sync with MT7603E_DPA_LinuxAP_4.0.1.0_rev2.P1_20160707*/
//#ifdef RTMP_RBUS_SUPPORT
	SHORT StreamSnr[3];				/* BF SNR from RXWI. Units=0.25 dB. 22 dB offset removed */
	SHORT SoundingRespSnr[3];			/* SNR from Sounding Response. Units=0.25 dB. 22 dB offset removed */
/*	SHORT TxPER;	*/					/* TX PER over the last second. Percent */
/*	SHORT reserved;*/
//#endif /* RTMP_RBUS_SUPPORT */
} RT_802_11_MAC_ENTRY, *PRT_802_11_MAC_ENTRY;
#elif defined(RT5392) || defined(RT3090) || defined(TCSUPPORT_DUAL_WLAN_RT5592_RT3593)
typedef union _MACHTTRANSMIT_SETTING {
	struct {
		USHORT	MODE:2;	// Use definition MODE_xxx.
		USHORT	rsv:3;
		USHORT	STBC:2;	//SPACE
		USHORT	ShortGI:1;
		USHORT	BW:1;	//channel bandwidth 20MHz or 40 MHz
		USHORT	MCS:7;	// MCS
	} field;
	USHORT word;
} MACHTTRANSMIT_SETTING, *PMACHTTRANSMIT_SETTING;

typedef struct _RT_802_11_MAC_ENTRY {
	UCHAR ApIdx;
	UCHAR Addr[MAC_ADDR_LENGTH];
	UCHAR Aid;
	UCHAR Psm;		/* 0:PWR_ACTIVE, 1:PWR_SAVE */
	UCHAR MimoPs;		/* 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled */
	CHAR AvgRssi0;
	CHAR AvgRssi1;
	CHAR AvgRssi2;
	UINT32 ConnectedTime;
	MACHTTRANSMIT_SETTING TxRate;
//#ifdef RTMP_RBUS_SUPPORT
	UINT32		LastRxRate;
	SHORT		StreamSnr[3];				/* BF SNR from RXWI. Units=0.25 dB. 22 dB offset removed */
	SHORT		SoundingRespSnr[3];			/* SNR from Sounding Response. Units=0.25 dB. 22 dB offset removed */
/*	SHORT		TxPER;	*/					/* TX PER over the last second. Percent */
/*	SHORT		reserved;*/
//#endif /* RTMP_RBUS_SUPPORT */
} RT_802_11_MAC_ENTRY, *PRT_802_11_MAC_ENTRY;
#endif

typedef struct _RT_802_11_MAC_TABLE {
	ULONG Num;
	RT_802_11_MAC_ENTRY Entry[MAX_NUMBER_OF_MAC];
} RT_802_11_MAC_TABLE, *PRT_802_11_MAC_TABLE;

//Andy Chiu, 2015/06/30. Add 5G mac table
#if defined(MT7612E) || defined(MT7610E)
typedef union _MACHTTRANSMIT_SETTING_5G{
	//Andy Chiu, 2015/08/25. update date structure.
	struct {
		USHORT MODE:3;	/* Use definition MODE_xxx. */
		USHORT iTxBF:1;
		USHORT eTxBF:1;
		USHORT STBC:1;	/* only support in HT/VHT mode with MCS0~7 */
		USHORT ShortGI:1;
		USHORT BW:2;	/* channel bandwidth 20MHz/40/80 MHz */
		USHORT ldpc:1;
		USHORT MCS:6;	/* MCS */
	} field;
	USHORT word;
} MACHTTRANSMIT_SETTING_5G, *PMACHTTRANSMIT_SETTING_5G;

typedef struct _RT_802_11_MAC_ENTRY_5G {
	UCHAR ApIdx;
	UCHAR Addr[MAC_ADDR_LENGTH];
	UCHAR Aid;
	UCHAR Psm;		/* 0:PWR_ACTIVE, 1:PWR_SAVE */
	UCHAR MimoPs;		/* 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled */
	CHAR AvgRssi0;
	CHAR AvgRssi1;
	CHAR AvgRssi2;
	UINT32 ConnectedTime;
	MACHTTRANSMIT_SETTING TxRate;
	UINT32 LastRxRate;

//sync with MT7620
	int StreamSnr[3];				/* BF SNR from RXWI. Units=0.25 dB. 22 dB offset removed */
	int SoundingRespSnr[3];			/* SNR from Sounding Response. Units=0.25 dB. 22 dB offset removed */
/*	SHORT TxPER;	*/					/* TX PER over the last second. Percent */
/*	SHORT reserved;*/
} RT_802_11_MAC_ENTRY_5G, *PRT_802_11_MAC_ENTRY_5G;

typedef struct _RT_802_11_MAC_TABLE_5G {
	ULONG Num;
	RT_802_11_MAC_ENTRY_5G Entry[MAX_NUMBER_OF_MAC];
} RT_802_11_MAC_TABLE_5G, *PRT_802_11_MAC_TABLE_5G;
#elif defined(RT5592)
typedef union _MACHTTRANSMIT_SETTING_5G{
	struct {
		USHORT	MODE:2;	// Use definition MODE_xxx.
		USHORT	rsv:3;
		USHORT	STBC:2;	//SPACE
		USHORT	ShortGI:1;
		USHORT	BW:1;	//channel bandwidth 20MHz or 40 MHz
		USHORT	MCS:7;	// MCS
	} field;
	USHORT word;
} MACHTTRANSMIT_SETTING_5G, *PMACHTTRANSMIT_SETTING_5G;

typedef struct _RT_802_11_MAC_ENTRY_5G {
	UCHAR ApIdx;
	UCHAR Addr[MAC_ADDR_LENGTH];
	UCHAR Aid;
	UCHAR Psm;		/* 0:PWR_ACTIVE, 1:PWR_SAVE */
	UCHAR MimoPs;		/* 0:MMPS_STATIC, 1:MMPS_DYNAMIC, 3:MMPS_Enabled */
	CHAR AvgRssi0;
	CHAR AvgRssi1;
	CHAR AvgRssi2;
	UINT32 ConnectedTime;
	MACHTTRANSMIT_SETTING TxRate;
	UINT32 LastRxRate;

//sync with MT7620
	SHORT StreamSnr[3];				/* BF SNR from RXWI. Units=0.25 dB. 22 dB offset removed */
	SHORT SoundingRespSnr[3];			/* SNR from Sounding Response. Units=0.25 dB. 22 dB offset removed */
/*	SHORT TxPER;	*/					/* TX PER over the last second. Percent */
/*	SHORT reserved;*/
} RT_802_11_MAC_ENTRY_5G, *PRT_802_11_MAC_ENTRY_5G;

typedef struct _RT_802_11_MAC_TABLE_5G {
	ULONG Num;
	RT_802_11_MAC_ENTRY_5G Entry[MAX_NUMBER_OF_MAC];
} RT_802_11_MAC_TABLE_5G, *PRT_802_11_MAC_TABLE_5G;
#endif

typedef struct mac_entry_s {
	unsigned char Addr[MAC_ADDR_LENGTH];
}mac_entry_t;

typedef struct mac_table_s {
	unsigned long Num;
	mac_entry_t macEntry[MAX_LEN_OF_MAC_TABLE];
}mac_table_t;

extern int _get_sta_list(int band, RT_802_11_MAC_TABLE *sta_list);
extern int DisConnectSta(int band, RT_802_11_MAC_TABLE *sta_list);


typedef struct _SITE_SURVEY_2G
{
	char channel[4];
#if defined(TCSUPPORT_WLAN_RT6856)
	char len[4];
#endif
	unsigned char ssid[33];
	char bssid[20];
	char authmode[15];	//security part1
	char encryption[8];	//security part2 and need to shift data
	char signal[9];
	char wmode[8];
	char extch[7];
	char nt[3];
	char wps[4];
	char dpid[4];
	char newline;
} SITE_SURVEY_2G;


typedef struct _SITE_SURVEY_5G
{
	char channel[4];
#if defined(TCSUPPORT_WLAN_RT6856)
	char len[4];
#endif
	unsigned char ssid[33];
	char bssid[20];
	char authmode[15];	//security part1
	char encryption[8];	//security part2 and need to shift data
#if defined(MT7612E) || defined(MT7610E)
	char signal[10];
	char wmode[9];
	char extch[7];
	char nt[3];
	char wps[4];
	char dpid[5];
#else
	char signal[9];
	char wmode[8];
	char extch[7];
	char nt[3];
	char wps[4];
	char dpid[4];
#endif
	char newline;
} SITE_SURVEY_5G;

typedef struct _SITE_SURVEY
{
	char channel[4];
//	unsigned char channel;
//	unsigned char centralchannel;
//	unsigned char unused;
	unsigned char ssid[33];
	char bssid[18];
	char encryption[9];
	char authmode[16];
	char signal[9];
	char wmode[8];
//	char bsstype[3];
//	char centralchannel[3];
} SITE_SURVEY;

typedef struct _SITE_SURVEY_ARRAY
{
	SITE_SURVEY SiteSurvey[64];
} SSA;

#define SITE_SURVEY_APS_MAX	(16*1024)

void
getMacEntryByIndex( mac_table_t *pMacTable, int index );
#ifdef BBUTOWBU
#ifdef WSC_AP_SUPPORT
#define GNU_PACKED __attribute__ ((packed))
typedef struct GNU_PACKED _WSC_CONFIGURED_VALUE {
	unsigned short WscConfigured; // 1 un-configured; 2 configured
	unsigned char	WscSsid[32 + 1];
	unsigned short WscAuthMode;	// mandatory, 0x01: open, 0x02: wpa-psk, 0x04: shared, 0x08:wpa, 0x10: wpa2, 0x20: wpa2-psk
	unsigned short WscEncrypType;	// 0x01: none, 0x02: wep, 0x04: tkip, 0x08: aes
	unsigned char	DefaultKeyIdx;
	unsigned char	WscWPAKey[64 + 1];
} WSC_CONFIGURED_VALUE;
//added by fredli
#define MAX_NUMBER_OF_ACL				64
typedef struct _RT_802_11_ACL_ENTRY {
    unsigned char   Addr[MAC_ADDR_LENGTH];
    unsigned short  Rsv;
} RT_802_11_ACL_ENTRY, *PRT_802_11_ACL_ENTRY;

typedef struct GNU_PACKED _RT_802_11_ACL {
    unsigned long   Policy;             // 0-disable, 1-positive list, 2-negative list
    unsigned long   Num;
    RT_802_11_ACL_ENTRY Entry[MAX_NUMBER_OF_ACL];
} RT_802_11_ACL, *PRT_802_11_ACL;
#endif
#endif
#endif
#endif

typedef struct _Nand_info
{
	unsigned int id;          //deviceid+menuid
	unsigned int ext_id; 
	unsigned int nand_version;
	char devciename[14];
}Nand_info_t;

#ifdef TCSUPPORT_PRODUCTIONLINE
int tcgetProLinePara(void*buf,int flag);
void GetEtherAddr(unsigned char *mac_addr);
int
proLineInfo_read(mxml_node_t *top,mxml_node_t *node);
#endif
#endif

#if defined(TCSUPPORT_WEB_CFG_NODE)
int
webUsingNode_init(void);
int
webUsingNode_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int
webUsingNode_boot(mxml_node_t *top);
#endif
int
webTempNode_init(void);
int
GUITemp_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int
defaultWan_init(void);
int
defaultWan_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);

#ifdef TCSUPPORT_WPA_SUPPLICANT
int
IPOEDot1xInfo_init(void);
int
IPOEDot1xInfo_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);

#endif
int
webCustom_init(void);
int
webCustom_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);

#ifdef TCSUPPORT_IPV6_PRIVACYADDRS
char*
getWanIpInfo(int type, char* device, char* buf);
#endif
#ifdef TCSUPPORT_SYSLOG
#ifdef TCSUPPORT_SYSLOG_ENHANCE

#define	SYSLOG_START_STRING		"StartOfTCSysLog"	/* the string in flash to indentify the start of log file */
#define SYSLOG_END_STRING		"EndOfTCSysLog"		/* The string in flash to identify the end of log file */

#define DELETE_LOG_NUM			10
#ifdef TCSUPPORT_NAND_FLASH
/* the log count that save to flash */
#define gMaxLogCnt 			2048//500//100
#define MAX_LOGFILE_SIZE		131072
#else
/* the log count that save to flash */
#define gMaxLogCnt 			1024//500//100
#define MAX_LOGFILE_SIZE		65536//51200//10225 			/* 10240 - strlen(EndOfTCSysLog\r\n) */
#endif
#define MAXLINE         		1024	/* maximum line length */

#define	 LOGACCESS		"LogAccess"
#define	 FROMFLASH		"FromFlash"
#define	 TOFILE			"ToFile"
#define	 TOFLASH		"ToFlash"
#define	 CLEARLOG		"ClearLog" //Ren

int
logAccess_init(void);
int
logAccess_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int
logAccess_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
read_logMsg_from_flash(char *file_name);
int
update_logFile(mxml_node_t *top);
int
write_logMsg_to_flash();
int
check_logMsg_level(char *logMsg);
unsigned long
get_logfile_size(char *fileName);

enum severity_level{
	LEVEL_EMER = 0,
	LEVEL_ALERT,
	LEVEL_CRITICAL,
	LEVEL_ERROR,
	LEVEL_WARN,
	LEVEL_NOTICE,
	LEVEL_INFO,
	LEVEL_DEBUG,
	LEVEL_NUM,
};

#endif
#endif

