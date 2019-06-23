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
#ifndef _CFG_MANAGER_H
#define _CFG_MANAGER_H

#ifdef WITHVOIP
#define VOIP        1 /*use WITHVOIP define to decide using voip function or not. shnwind 20100203*/
#else
#define VOIP        0
#endif

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>

#include "config.h"
#include "mxml.h"
#include "common.h"
#include "tcapi.h"
#include "interface.h"
#include "advanced.h"
#include "access.h"
#include "maintenance.h"
#include "web.h"
#include "tcapi_proc.h"
#include "apps.h"
#include "defaults.h"
#include "vpn.h"
#include "bwdpi.h"

#if VOIP
#include "voip.h"
#endif

#include "../lib/libtcapi.h"
#if 1
#include "../../public/mtd/tc_partition.h"
#endif

#define CFGMGR_VER	1

#define GUEST_NETWORK_MARK	8 /* guest mark only 8 ~ 13 (6 guestnetwork) */

#define MAX_TCAPI_TIME	10

#define VSLN66U_WAN_NUM	4

#define TCSUPPORT_GETNODE_ENHANCE	1

#define MAX_TCAPI_CLIENT	5

#define ROMFILE_TAG	"ROMFILE"
#define ROMFILE_PATH 	"/tmp/var/romfile.cfg"
#define DEF_ROMFILE_PATH 	"/userfs/romfile.cfg"
#ifdef TCSUPPORT_ISP_CUSTOMIZE_ROMFILE
#define DEV_ZERO_PATH	"/dev/zero"
#endif

//Andy Chiu, 2015/03/09
#define CFG_RESTORE_DIR	"/tmp/var/cfgrestore"
#define WEBTYPE_ROMFILE_PATH	"/tmp/var/cfgrestore/%s_romfile.cfg"
#define CC_RESTORE_PATH	"/tmp/var/cfgrestore/restore_cc"

#define UPDATE_ROMFILE_CMD	"/userfs/bin/mtd write %s romfile"
#if 1 //def TCSUPPORT_BACKUPROMFILE
#define BR_FILE_NAME "/tmp/tc_backupromfile"
#define DR_FILE_NAME "/tmp/tc_defaultromfile"
#ifdef TCSUPPORT_ISP_CUSTOMIZE_ROMFILE
#define USR_FILE_NAME "/tmp/tc_userromfile"
#endif
#endif

/* running romfile path used by customer*/
#define RR_FILE_NAME	"/tmp/customer_runningromfile"

/* default romfile path used by customer*/
#define CUSTOMER_DEF_ROMFILE_PATH 	"/tmp/customer_defaultromfile"

#define UPLOAD_ROMFILE_PATH	"/var/tmp/up_romfile.cfg"
#define UPLOAD_COMPRESSROMFILE_PATH	"/var/tmp/up_romfile.cfg.gz"
#define RUNNING_ROMFILE_PATH 	"/dev/mtdblock1"
#define BK_WEBINFO_PATH	"/tmp/web.xml"
#define TMP_CONF	"/tmp/tmp.conf"
#define TMP_PPP_PATH "/tmp/checkppp"

#ifndef TCSUPPORT_QOS
#define QOS_REMARKING  1  /*Rodney_20090724*/
#endif

#if 1
#define	RUN_ROMFILE_FLAG	0
#define	BK_ROMFILE_FLAG	1
#if 1
#define	DEF_ROMFILE_FLAG	2
#endif
#define	DEFFILESYSTEM_ROMFILE_FLAG	3
#endif
#ifdef TCSUPPORT_ISP_CUSTOMIZE_ROMFILE
#define USER_ROMFILE_FLAG 4
#endif

#define OP_NUM		3
#define OP_READ		0
#define OP_WRITE	1
#define OP_VERIFY	2

#define NO_QMARKS 0
#define QMARKS 1
#define NO_ATTRIBUTE 2

#define AUTOPVC_RESULT_IDLE 0
#define AUTOPVC_RESULT_SUCCESS 1
#define AUTOPVC_RESULT_FAIL 2

#define DEFAULT_MTU 1500 //Ren

#ifndef PURE_BRIDGE
#if defined(TCSUPPORT_WAN_ETHER_LAN)
	#define PVC_NUM 13
#elif defined(TCSUPPORT_USB_3G_DONGLE)
	#define PVC_NUM 12
#elif defined(TCSUPPORT_WAN_ETHER)
	#define PVC_NUM 11
#elif defined(TCSUPPORT_WAN_PTM)
    #define PVC_NUM 10
#else
    #define PVC_NUM 8
#endif
#else
	#define PVC_NUM 4
#endif
#define MAX_VIRSERV_RULE 10
/*krammer : change the value from 16 to 64*/
#define MAX_ROUTE_NUM 64
/*krammer : change the value from 16 to 64*/
#define MAX_BSSID_NUM 4
#ifdef TCSUPPORT_VPN
#define MAX_VPN_NUM 8
#endif
#if !defined(TCSUPPORT_C1_NEW_GUI) 
#define LAN_NUM             1
#endif
#define MAX_ARG_NUM	3
#define DEF_IP	"0.0.0.0"
/**********20080825 racing add************/
#if 1
#define MAX_QOS_RULE 16
#else
#define MAX_QOS_RULE 36
#endif
#if !defined(TCSUPPORT_ACCOUNT_ACL) 
/**********20080825 racing add************/
#define MAX_ACL_RULE 16
/**********20080926 racing add************/
#endif

#if !defined(TCSUPPORT_MAX_IPMACFILTER_NUM_48) 
#define MAX_IPMACFILTER_RULE 16
#endif

/**********20081021 krammer add************/
//#define MAX_URL_FILTER_RULE 16
/*Only Support 8 rule for l7-filter in kernel. shnwind 20110417*/
#define MAX_URL_FILTER_RULE 8 

#define SUB_NODE_NAME	"Entry"
#define SUB_PVC_NAME	"PVC%d"
#define NAS_IF	"nas%d"
#define PPP_IF	"ppp%d"
#define Inet_Addr  "inet addr:"
#define HWADDR   "HWaddr"
#define NAS_GATEWAY "gateway="
#define NAS_GATEWAYFILE "/etc/nas%d_gateway.conf"

#define MLDPROXY "MLDproxy"
#define VERSION_IPV4 4
#define VERSION_IPV6 6
#define KILL_ECMH "killall -9 ecmh"
#define ECMH_PID "/var/run/ecmh.pid"
#define ECMH_PROCESS "/userfs/bin/ecmh -i %s -p /var/run/ecmh.pid"
#define ATTR_SIZE 32

#define TM_NODE "TrafficMonitor"
#define TM_Entry "Entry"

#define FIREWALL "Firewall"

#define QOS_NODE "QoS"
#define QOS_COMMON "Common"
#define QOS_ENTRY0 "Entry0"

#define VPNC_NODE "VPNC"

#define MAXSIZE	160
#define QM_LEN	2

#define FLASH_NOT_SUPPORT -4
#define WRONG_ROMFILE -3
#define SAME	-2
#define FAIL -1
#define SUCCESS 0
#define ROMFILE_ERR_MSG "The Romfile format is wrong!!!"
#define USE_DEF_ROMFILE_MSG "Romfile format is wrong, we use default romfile to replace current setting romfile!!\n"
#if 1
#define ROMFILE_CHECK_ERR_MSG "Romfile check is wrong!!!\n"
#endif
#define DEF_ROMFILE_ERR_MSG "Default romfile format is wrong.Please to check again!!!\n"
#define OPEN_FAIL_MSG "Open fail: %s %s\n"
#define RENAME_FAIL_MSG "Rename fail: %s\n"
#define VERIFY_ERR_MSG "verify_romfile func: None %s node\n"
#define CREATE_ROMFILE_ERR_MSG "Fail to create the romfile!!!\n"

#define UPGRADE_ROMFILE_CMD	"/userfs/bin/mtd %s write /tmp/var/romfile.cfg romfile"
#define UPGRADE_COMPRESSROMFILE_CMD	"/userfs/bin/mtd %s write /tmp/var/romfile.cfg.gz romfile"

#define ATM_VC_NUM			8
#define MERGE_FROM_TABLE 	1
#define MERGE_FROM_USER		0
#define MAX_SERVICE_NUM 	8
#define TOTAL_WAN_NUM   ((PVC_NUM-ATM_VC_NUM)*MAX_SERVICE_NUM + ATM_VC_NUM)
#define WAN_PVC_EXT 		"wan_pvc_ext"
#define DEV_PVC_EXT 		"dev_pvc_ext"
#define DIAG_PVC_EXT		"diag_pvc_ext"
#define NAT_PVC_EXT			"nat_pvc_ext"
#define VOIP_INTERFACE		"voip_interface"
#define	FILTER_INTERFACE	"filter_interface"
#define NAS_EXT_GATEWAYFILE "/etc/nas%d_%d_gateway.conf"
#define NAS_EXT_IF			"nas%d_%d"
#define PPP_EXT_IF  		"ppp%d%d"
#define WAN_EXT_PATH		"/etc/isp%d_%d.conf"
#define NATEXT_PATH			"/etc/nat_pvc%d_%d/ipmode"
#define DMZEXT_PATH 		"/etc/nat_pvc%d_%d/dmz"
#define VIRSERVEXT_PATH 	"/etc/nat_pvc%d_%d/vserver%d"

#define SINGLE_SKU_PATH_2G "/etc/Wireless/RT2860AP/SingleSKU.dat"
#define SINGLE_SKU_PATH_5G "/etc/Wireless/RT2860AP_AC/SingleSKU.dat"
#define SINGLE_SKU_PATH_2G_CE "/etc/Wireless/RT2860AP/SingleSKU_CE.dat"
#define SINGLE_SKU_PATH_2G_AU "/etc/Wireless/RT2860AP/SingleSKU_AU.dat"
#define SINGLE_SKU_PATH_2G_SG "/etc/Wireless/RT2860AP/SingleSKU_SG.dat"
#define SINGLE_SKU_PATH_5G_CE "/etc/Wireless/RT2860AP_AC/SingleSKU_CE.dat"
#define SINGLE_SKU_PATH_5G_AU "/etc/Wireless/RT2860AP_AC/SingleSKU_AU.dat"
#define SINGLE_SKU_PATH_5G_SG "/etc/Wireless/RT2860AP_AC/SingleSKU_SG.dat"

#define SHIFT_INDEX 64//32
#define ATTR_INDEX	0
#define VALUE_INDEX	1

//filter bit map
#define WAN_VECTOR (1<<0)
#define FIREWALL_VECTOR (1<<1)
#define ACL_VECTOR (1<<2)
#define IPFILTER_VECTOR (1<<3)
#define APP_VECTOR (1<<4)
#define URL_VECTOR (1<<5)
#define ALGSW_VECTOR (1<<6)
#if !defined(TCSUPPORT_TIME_OF_DAY) 
#if defined(RA_PARENTALCONTROL) && defined(TCSUPPORT_PARENTAL_URLBLOCK)
	#define L7_BIT_MASK (APP_VECTOR|URL_VECTOR|PARENTAL_URL_VECTOR)
#endif
#endif
#ifdef TCSUPPORT_DHCP_PORT_FLT
#define	PORT_FLT_VECTOR		(1<<9)
#endif
#if defined(RA_PARENTALCONTROL) && defined(TCSUPPORT_PARENTAL_URLBLOCK)
#define	PARENTAL_URL_VECTOR		(1<<10)
#endif

//added by xyzhu_nj_20091204
#define TCAPI_READALL_CMD 7

/*Add for VendorConfigFile TR069 Nodes*/
#define MAX_VendorCfgFile_NUM	2
//flag for NOTDEFAULTROMFILEAREA 
#define NOTDEFAULTROMFILEAREA 0
#define WITHDEFAULTROMFILEAREA 1

#if 0
typedef struct
cfg_op_s{
	int (*cfg_read)(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr);
	int (*cfg_write)(mxml_node_t *top,mxml_node_t *node);
	int (*cfg_verify)(mxml_node_t *node);
	int (*cfg_execute)(mxml_node_t *top, char name[][MAX_NODE_NAME]);
}cfg_op_t;
#endif

typedef struct
cfg_node_s{
	char *name;
	/*
	char name[MAX_NODE_NAME];
	*/
/*
    15   14  13 12  11  10   9   8   7    6   5    4   3   2    1   0       (bit order)
+----+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
| ADv |                Type          |              MAX			|
+----+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/
#define SINGLE_TYPE	(1 << 8)
#define PVC_TYPE	(1 << 9)
#define ENTRY_TYPE	(1 << 10)
#define WEB_TYPE	(1 << 11)
#define UPDATE_TYPE	(1 << 12)
#define ADV_FLAG	(1 << 15)
	unsigned short int   type;
	char **adv_table;
	char **merge_table;
	int (*cfg_read)(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr);
	int (*cfg_write)(mxml_node_t *top,mxml_node_t *node);
	int (*cfg_verify)(mxml_node_t *node);
	int (*cfg_execute)(mxml_node_t *top, char name[][MAX_NODE_NAME]);
	int (*cfg_boot)(mxml_node_t *top);
}cfg_node_t;

typedef int (*node_init)(void);

int
register_cfg_node(cfg_node_t *newNode);

mxml_node_t*
parser_romfile(const char* pathname);

mxml_node_t *
load_def_romfile(char *file);

int
create_romfile(mxml_node_t *xml);

int
isNode(const char *name);

int
verify_romfile( mxml_node_t *tree);


const char*
whitespace_cb(mxml_node_t *node, int where);

void
print_mxml_tree( mxml_node_t *tree);

void
tcapi_readAll_req(tcapi_msg_t *msg);

#if 0  /*Orginal ci-command mode */
int
node_read(mxml_node_t * top, char * node_name, char * file, char * node_attr, int type);
#endif
int
node_write(mxml_node_t *node, char *file, int type);

int
updateString(char* file, const char* src, char *dst);

int
write2file(char *buf, char *pathname);

cfg_node_t *
getAllCfgTable(void);

#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
char *getPVCGateway(int pvcindex, int serv);
#else
char *getPVCGateway(int pvcindex);
#endif

int isWanUp();

#ifdef IPV6
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
char *getPVCGatewayIP6(int pvcindex, int serv);
#else
char *getPVCGatewayIP6(int pvcindex);
#endif
char *getIP6PreDelegation(char *isp, char *dev, char *buf);
#endif


//#ifdef TCSUPPORT_DNSEACHPVC
char* getDNSInfo(char* type, char* isp, char* dev, char* buf, int ipversion);
//#endif

#if 1
void loadRomfile(int types);
void loadBackupRomfile(void);
#if 1
void loadDefaultRomfile(void);
#endif

#if 1
mxml_node_t*check_romfile(char* pathname);
#endif

mxml_node_t*check_romfile(char* pathname);
mxml_node_t*tc_parse_romfile(int check_flag);

int
get_customer_default_romfile(void);
int
get_customer_backup_romfile(void);
int
update_customer_backup_romfile(char *file);
#endif

int
get_customer_running_romfile(void);
int
update_customer_running_romfile(char *file);
int
get_customer_def_romfile(void);

/*********shnwind add*/
#define UPNPD_PATH 	"/etc/igd/igd.conf"
struct PRE_SYS_STATE{
#ifndef PURE_BRIDGE
    	char nat_raw_state;/*system use raw table NOTRACK module */
//    	char filter_m_state;	/*system load ipfilter module */
    	unsigned int filter_m_state;	/*system load ipfilter module */
#endif
    	char wlan_Bssid_num;/*WLan bssid_num */
	char wlan_chann;//add by xyyou to support AutoChann
#if VOIP
	char voip_service;	
#endif	
#ifdef TCSUPPORT_WLAN_WDS
	int WdsEnable;
	char WdsEncrypType[5];
#if !defined(TCSUPPORT_C1_NEW_GUI) 
	char WdsKey[65];
#endif
	char Wds_MAC[MAX_WDS_ENTRY][18];
	char WepKeyStr[4][27];
#endif
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520)
	char cc_is_run;
#endif
	//unsigned int  ebtable_status;
//	char l7filter_state;/*system load l7filter module*/
}pre_sys_state;
/*******shnwind add end****/
#ifdef TCSUPPORT_GETNODE_ENHANCE
#define DYN_DISP_ENTRY_NODE	"dynDisp_Entry"
#define WIFIMACTAB_NODE	"wifiMacTab"
#define NODE_CACHE_NUM 5
#ifdef TCSUPPORT_CPU_TC3162
#define CACHE_TIME_OUT 1 	//sec
#else
#define CACHE_TIME_OUT 3 	//sec, 0.2->3
#endif
typedef struct node_cache_s{
	char nodeName[MAXLEN_NODE_NAME];
	struct timespec tpstart;
}node_cache_t;

int
CacheNodeInit(void);
int
isCacheTimeout(struct timespec tpCur, struct timespec tpstart);

int
insertCacheNode(int timeoutIndex, char *nodeName , struct timespec tpCur);

int
modifyCacheNodeById(int i, char *nodeName ,struct timespec tpCur);
#endif

#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
typedef struct extension_table_s{
	char *ori_nodename;
	char *ext_nodename;
	char start_num;
	char end_num;
	char *merge_attr;
}extension_table_t;
#endif

void do_parse_romfile(mxml_node_t **tree_parse);
void set_modulestatus();
int strcmp_Diagnostic(char* nodeName);
void write_backup_romfile(char* path);

void lanaccess_wl( int freq);

extern char *get_nas_ifname_of_WAN(int wan_pvc_index, char *resultValue); //Ren: get wan interface name, ex: nas0(ATM), nas8(PTM), nas9(PTM1), nas10(ETHERNET WAN) etc.
void setup_rtsp_conntrack(void); //Ren

typedef struct log_record_s{
	unsigned long counter;
	char *str;
	struct log_record_s* next;
}log_record_t;

typedef struct email_auth_data_s{
	char mailServer[64];
	char email[32];
	char acct[32];
	char pwd[32];
}email_auth_data_t;

#define WIFI_LOG_FILE "/tmp/asusfbsvcs/wifilog.txt"
#define FREE_LOG_FILE "/tmp/asusfbsvcs/free.txt"
#define TOP_LOG_FILE "/tmp/asusfbsvcs/top.txt"
#define PSINFO_LOG_FILE "/tmp/asusfbsvcs/psInfo.txt"
#define IPTABLES_LOG_FILE "/tmp/asusfbsvcs/iptables_save.txt"
#define SYSLOG_LOG_FILE "/tmp/asusfbsvcs/currLogFile.txt"
#define DMESG_LOG_FILE "/tmp/asusfbsvcs/dmesg.txt"
#define SYNC_LOG_FILE "/tmp/asusfbsvcs/sync_log.txt"
#define SNR_LOG_FILE "/tmp/asusfbsvcs/snr_crc_log.txt"
#define MODEM_LOG_FILE "/tmp/asusfbsvcs/modemlog.txt"
#define ENC_ROMFILE_FILE "/tmp/asusfbsvcs/romfile_feedback.cfg"
#ifdef RTCONFIG_SYSSTATE
#define CPUUSAGE_LOG_FILE "/tmp/asusfbsvcs/cpuusage_log.txt"
#define RAMUSAGE_LOG_FILE "/tmp/asusfbsvcs/ramusage_log.txt"
#define CPUTEMP_LOG_FILE "/tmp/asusfbsvcs/cputemp_log.txt"
#endif

#define CREATE_STRIP_PSINFO_SCRIPT "/usr/script/Create_Strip_psInfo.sh"
#define DO_STRIP_PSINFO "/tmp/StripsInfo.sh"

#define LOG_CRC_SNR_PERIOD 30
#define LOG_MAX_ENTRY 6000

#define WRITE_RUNNING_ROMFILE	0x0001
#define WRITE_BACKUP_ROMFILE	0x0002
#define WRITE_DEFAULT_ROMFILE	0x0004
int write_cur_romfile_in_flash(mxml_node_t *node, const int write_type);
int remove_dup_node_in_romfile(mxml_node_t *top);

#ifdef TCSUPPORT_WEBMON
extern int backup_webmon(void);
#endif

#endif
