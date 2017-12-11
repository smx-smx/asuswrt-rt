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
#ifndef _ACCESS_H
#define _ACCESS_H

#include "cfg_manager.h"

#define SNMPD "Snmpd"
#define SNMPD_PATH 	"/etc/snmp/snmpd.conf"
#define SNMPD_STAT_PATH 	"/etc/snmp/snmpd_stat.conf"
#define SNMPD_TMP_PATH 	"/usr/etc/snmp/snmpd.conf.tmp"
#define SNMPD_PID_PATH	"/var/log/snmpd.pid"
#define SNMPD_CMD	"/userfs/bin/snmpd -Ln -c %s -p %s"
/*#define SNMPD_ENTRY 	"Snmpd_Entry"*/
#define SNMPD_ACTIVE  "ACTIVE=\"Yes\""
#define ACTIVE		"Yes"
#define DEACTIVE		"No"
#define SNMPD_STAT_INDEX  0
#define SNMPD_ATTR_INDEX  0
#define SNMPD_VALUE_INDEX  1
#define SNMP_ARRAY_SIZE	64
#define TRAP_COMMUNITY	"public"

#if defined(TCSUPPORT_SNMP_FULL)
#define		SNMP_NODE		"Snmpd_Entry"
#define 	SNMP_COLDTRAP		"coldTrapFlag"
#define 	TRAP_FLAG		"1"
#if !defined(TCSUPPORT_START_TRAP)

#define COLD_TRAP_VALUE	"0"
#define TRAPIP_DEFAULT	"trapip"
#endif
#endif
#if defined(TCSUPPORT_SNMP_V3)
#define SNMPV3_AUTH_FILE "/etc/net-snmp/snmpd.conf"
#define SNMPV3_ACCESS_FILE "/etc/net-snmp/snmp/snmpd.conf"
#endif


#if defined(TCSUPPORT_IPV6_SNMP)
#define LOCAL_INTERFACE "br0"
#define AGENT_ADDRESS_INFO "udp:161,udp6:161"
#endif


#define UPNPD	"Upnpd"
#define UPNPD_ACTIVE	"enable"
#if 0  /*Orginal ci-command mode */
#define UPNPD_ENTRY 	"Upnpd_Entry"
#endif
#ifdef TCSUPPORT_UPNP
#define UPNPD_CMD 	"/userfs/bin/miniupnpd -P %s -f %s &"
#else
#define UPNPD_CMD 	"/userfs/bin/upnpd &"
#endif
#define UPNPD_CONF 	"/var/tmp/miniupnpd.conf"
#define UPNPD_PATH 	"/etc/igd/igd.conf"
#define UPNPD_PID_PATH "/var/log/upnpd.pid"
#define DDNS_CONF		"Ddns"
#define DDNS_STATUS	"Vram"
#define DDNS_INFO	"Vram_Entry"
#define DDNS_SETTING	"Ddns_Entry"
#define DDNS_CONF_PATH "/etc/ddns.conf"
#define DDNS_PID_PATH "/var/run/ez-ipupdate.pid"
#define DDNS_SH	"/etc/script/ddns.sh"
#define DDNS_RUN_SH	"/etc/script/ddns_run.sh %s &"
/**********20080825 racing add************/
#define ACL_SH	"/tmp/etc/acl.sh"
#define ACL_STOP_SH "/usr/script/acl_stop.sh"
#define ACL "ACL"
#define ACL_COMMON "Common"
/**********20080825 racing add************/

/**********20080926 racing add************/
#define IPMACFILTER_SH	"/tmp/etc/ipmacfilter.sh"
#define IPMACFILTER_STOP_SH "/usr/script/ipmacfilter_stop.sh"
#define IPMACFILTER "IpMacFilter"
#define IPMACFILTER_COMMON "Common"
/**********20080926 racing add************/

/**********20081021 krammer add************/
#define MAX_APP_RULE 4
#define MAX_URL_RULE 16
#define MAX_KEYWORD_RULE 16
#define APPFILTER "AppFilter"
#define URLFILTER "UrlFilter"
#define KEYWORDFILTER "KeywordFilter"
#define APP_FILTER_SH	"/tmp/etc/AppFilter.sh"
#define URL_FILTER_SH	"/tmp/etc/UrlFilter.sh"
#define URL_PATTER_PATH "/etc/l7-protocols/Url.pat"
#define APP_STOP_SH	"/usr/script/AppFilterStop.sh "
#define URL_STOP_SH	"/usr/script/UrlFilterStop.sh "
#define STRING_COMMON 	"Common"
#define STRING_ENTRY 	"Entry"
#define HTTP "http://"
#define MAX_URL_INPUT_LENGTH 48
#define SPCHNUMBER 8
/**********20081021 krammer add************/
/**********20091221 jlliu add**************/
#ifdef USE_SSL
#define SSL_CA "SslCA"
#define MAX_CA_NUM 4
#define MAX_CA_SIZE 4096
#define FRAG_SIZE 512
#define MAX_ROMFILE_SIZE (64*1024)
/*Length of path should be less than 32 */
#define CA_PATH "/etc/CustomerCA.pem"
/*Length of path should be less than 32 */ 
#define TMP_CA_PATH "/var/tmp/CA.pem" 
#define UPLOAD_CA_PATH "/var/tmp/ca"
#define TMP_ROMFILE_PATH "/var/romfile.cfg"
#define CA_NODE_NAME "Entry"
#define CA_UPGRADE_FLAG "UpgradeFlag"
#endif /* USE_SSL */
/**********20091221 jlliu add**************/

#ifdef RA_PARENTALCONTROL
#define PARENTAL "Parental"
#define PARENTAL_NODE "Entry"
#if defined(TCSUPPORT_PARENTAL_URLBLOCK)
#define ATTR_BLOCKTYPE "BlockType"
#define ATTR_MAC "MAC"
#define ATTR_URL "URL"
#define PARENTAL_URL_PATTER_PATH "/etc/l7-protocols/Parental_Url.pat"
#define NEW_PARENTAL_URL_CHAIN "iptables -t filter -N parental_url_chain"
#define INSERT_PARENTAL_URL_CHAIN_TO_FOWARD "iptables -t filter -I FORWARD -j parental_url_chain"
#define DELETE_PARENTAL_URL_CHAIN_FROM_FOWARD "iptables -t filter -D FORWARD -j parental_url_chain"
#define FLUSH_PARENTAL_URL_CHAIN "iptables -t filter -F parental_url_chain"
#define ZERO_PARENTAL_URL_CHAIN "iptables -t filter -Z parental_url_chain"
#define DELETE_PARENTAL_URL_CHAIN "iptables -t filter -X parental_url_chain"
#endif

#define PROC_ARP "/proc/net/arp"	//copied from asp_handler.c and added by Javier

#endif/*RA_PARENTALCONTROL*/
#ifdef CWMP
#define CWMP_NODE				"Cwmp"
#define	CWMP_PORT_NUM				"7547"
#define	OLD_CWMP_CONREQ_PORT	"oldConReqPort"
#define CWMP_UP_PATH		"/tmp/cwmp_is_up"
#endif

#define FIREWALL_NODE "Firewall"  //Ren
#define FIREWALL_ENTRY "Entry"  //Ren

#if  defined(TCSUPPORT_SAMBA)
#define MAX_SAMBA_RULE 8
#define SAMBA_ARRAY_SIZE	64
#define SAMBA_COLUMN_SIZE	4
#define SAMBA_NODE				"Samba"
#define SAMBA_ENTRY			"Entry"
//#define SAMBA_PATH				"/etc/samba/smb.conf"
#define SAMBA_PATH				"/etc/smb.conf"
#define SAMBA_NMBD_CMD		"/userfs/bin/nmbd -D\n"
#define SAMBA_SMBD_CMD		"/userfs/bin/smbd -D\n"
#define SAMBA_PASSWD_CMD		"/userfs/bin/smbpasswd -a"
#define SAMBA_CMD				"/usr/script/samba.sh"
#define SAMBA_ADD_DIR_CMD		"/usr/script/samba_add_dir.sh"
#define SAMBA_NMBD_PID_PATH	"/var/run/nmbd.pid"
#define SAMBA_SMBD_PID_PATH	"/var/run/smbd.pid"
#define SAMBA_KILL_SMBD_CMD	"killall smbd"
#define SAMBA_PASSWD			"Passwd"
#define FTPD_CONF_PATH		"/etc/vsftpd.conf"
#endif 

//Andy Chiu add for wol, 2014/10/13
//#ifdef RTCONFIG_WOL
#define WOL_ATTR_NUM			4
#define WOL_NODE		"Wol"
#define WOL_ENTRY		"Entry"
#define WOL_CMD_PATH	"/var/tmp/wol.sh"
#define WOL_LIST		"List_"
//#endif

//Andy Chiu, 2014/10/28, add for client list
#define RTPRIV_IOCTL_GET_MAC_TABLE 						(SIOCIWFIRSTPRIV + 0x0F)
#define CLIENT_LIST_NODE	"ClientList"
#define CLIENT_LIST_SIZE	255
#define USERDEF_CL_NODE	"UserDefCL"
#define USERDEF_CL_SIZE		255
#define ASUSDEV_LST_NODE	"AsusDevLst"
#define ASUSDEV_LST_SIZE	255

//Carlos Hua, 2016/03/28
#ifdef RTCONFIG_NOTIFICATION_CENTER
#define NC_NODE                 "NTcenter"
#define NC_ENTRY                "Entry"
#endif

typedef struct _UserDefClientDev
{
	char name[36];
	char type[16];
	char mac[20];
}UserDefClientDev;

#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
#define 		FILTER_INTERFACE	"filter_interface"
#endif
#define 		CWMP_RULE				0
void 
update_preSerivce_rule(mxml_node_t *top, int type);

#if 0  /*Orginal ci-command mode */
int
snmpd_read(mxml_node_t *node);
#endif

int
snmpd_boot(mxml_node_t *top);
int
snmpd_init(void);
int
snmpd_write(mxml_node_t *top, mxml_node_t *parant);
int
snmpd_verify(mxml_node_t *node);
int
snmpd_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if 0  /*Orginal ci-command mode */
int
upnpd_read(mxml_node_t *node);
#endif

//Andy Chiu, 2015/01/21.
int start_miniupnpd(int force);
int stop_miniupnpd();

int
upnpd_boot(mxml_node_t *top);
int
upnpd_init(void);
int
upnpd_write(mxml_node_t *top, mxml_node_t *parant);
int
upnpd_verify(mxml_node_t *node);
int
upnpd_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);


#if defined(TCSUPPORT_SAMBA)
int 
samba_boot(mxml_node_t *top);
int
samba_init(void);
int
samba_write(mxml_node_t *top, mxml_node_t *parant);
int
samba_verify(mxml_node_t *node);
int
samba_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif 

#ifdef CWMP
int
cwmp_boot(mxml_node_t *top);
int
cwmp_init(void);
int
cwmp_write(mxml_node_t *top, mxml_node_t *parant);
int
cwmp_verify(mxml_node_t *node);
int
cwmp_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif
#if 0  /*Orginal ci-command mode */
int
ddns_read(mxml_node_t *node);
#endif

int
ddns_init(void);
int
ddns_write(mxml_node_t *top, mxml_node_t *parant);
int
ddns_verify(mxml_node_t *node);
int
ddns_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

/**shnwind add*****************/
int
write_upnp_conf(mxml_node_t *top);
#endif

#if !defined(TCSUPPORT_ACCOUNT_ACL) 
/**********20080825 racing add************/
#if 0  /*Orginal ci-command mode */
int
acl_read(mxml_node_t *node);
#endif

int
acl_boot(mxml_node_t *top);
int
acl_init(void);
int
acl_write(mxml_node_t *top, mxml_node_t *parant);
int
acl_verify(mxml_node_t *node);
int
acl_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

int
acl_check_filter(mxml_node_t *top);
/**********20080825 racing add************/

/**********20080926 racing add************/
#if 0  /*Orginal ci-command mode */
int
ipfilter_read(mxml_node_t *node);
#endif

int
ipfilter_boot(mxml_node_t *top);
int
ipfilter_init(void);
int
ipfilter_write(mxml_node_t *top, mxml_node_t *parant);
int
ipfilter_verify(mxml_node_t *node);
int
ipfilter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
ipfilter_check_filter(mxml_node_t *top);
int
ipfilter_check_duplicate(mxml_node_t *top,mxml_node_t *parant);

/**********20080926 racing add************/

/**********20081021 krammer add************/

int
app_filter_boot(mxml_node_t *top);
int
app_filter_init(void);
int
app_filter_write(mxml_node_t *top, mxml_node_t *parant);
int
app_filter_verify(mxml_node_t *node);
int
app_filter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
app_check_filter(mxml_node_t *top);
/**********20081021 krammer add************/

/**********20081021 krammer add************/
int
url_filter_boot(mxml_node_t *top);
int
url_filter_init(void);
int
url_filter_write(mxml_node_t *top, mxml_node_t *parant);
int
url_filter_verify(mxml_node_t *node);
int
url_filter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
url_check_filter(mxml_node_t *top);
int
urlfilter_check_duplicate(mxml_node_t *top,mxml_node_t *parant);

/****** keywork filter *********/
int
keyword_filter_boot(mxml_node_t *top);
int
keyword_filter_init(void);
int
keyword_filter_write(mxml_node_t *top, mxml_node_t *parant);
int
keyword_filter_verify(mxml_node_t *node);
int
keyword_filter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
keyword_check_filter(mxml_node_t *top);
int
keyword_filter_check_duplicate(mxml_node_t *top,mxml_node_t *parant);

/**********20081021 krammer add************/
/**********20091221 jlliu add**************/
#ifdef USE_SSL
int
sslca_boot(mxml_node_t *top);
int
sslca_init(void);
int
sslca_write(mxml_node_t *top, mxml_node_t *parant);
int
sslca_verify(mxml_node_t *node);
int
sslca_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif /* USE_SSL */
/**********20091221 jlliu add**************/
#ifdef CWMP
/**********20091202 xyzhu add**************/
int
tr069Attr_init(void);
/**********20091202 xyzhu add**************/
#endif


#ifdef RA_PARENTALCONTROL
int
parental_init(void);
int
parental_boot(mxml_node_t *top);
int
parental_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#if defined(TCSUPPORT_PARENTAL_URLBLOCK)
int
parental_url_check_filter(mxml_node_t *top);
#endif
#endif/*RA_PARENTALCONTROL*/
#ifdef TCSUPPORT_FILTER_DSCP
#define		DNOT_CARE_DSCP		"64"
#endif
//#ifdef CWMP
//void start_cwmp(void);
//#endif

//Andy Chiu add for wol, 2014/10/13
//#ifdef RTCONFIG_WOL
int wol_boot(mxml_node_t *top);
int wol_write(mxml_node_t *top, mxml_node_t *parant);
int wol_verify(mxml_node_t *node);
int wol_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int wol_init(void);
//#endif

//Andy Chiu, 2014/10/28. add for client list
int client_list_boot(mxml_node_t *top);
int client_list_verify(mxml_node_t *node);
int client_list_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int client_list_init(void);
int userdef_cl_init(void);
int asusdev_lst_init(void);

#ifdef RTCONFIG_TR069
int tr_init(void);
int tr_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

//Carlos Hua, 2016/03/28
#ifdef RTCONFIG_NOTIFICATION_CENTER
int NTcenter_init(void);
int NTcenter_boot(mxml_node_t *top);
int NTcenter_write(mxml_node_t *top, mxml_node_t *parant);
int NTcenter_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif
