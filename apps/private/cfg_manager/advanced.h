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
#ifndef _ADVANCED_H
#define _ADVANCED_H
#include <linux/version.h>

#define KERNEL_2_6_36 	(LINUX_VERSION_CODE > KERNEL_VERSION(2,6,31))

#define MISC "Misc"
#define FIREWALL  "Firewall"
#if 0  /*Orginal ci-command mode */
#define FIREWALL_ENTRY 	"Firewall_Entry"
#endif
#define FIREWALL_PATH "/etc/firewall.conf"

#define ROUTE	"Route"
#define ROUTE_NUM "Route_num"
#define ROUTE_COUNT_PATH 	"/etc/route.count"
#define ROUTE_PATH 	"/etc/route.sh"
/*#define ROUTE_ENTRY 	"Route_%d_Entry"*/
#define ROUTE_ATTR_ID		"ID"
#define ROUTE_ATTR_DST_IP	"DST_IP"
#define ROUTE_ATTR_SUB_MASK	"Sub_mask"
#define ROUTE_ATTR_DEVICE	"Device"
#define ROUTE_ATTR_GW	"Gateway"
#define ROUTE_CMD_DEV	"dev"
#define ROUTE_CFG_IP_INDEX	3
#define ROUTE_CFG_SUB_MASK_INDEX	5
#define ROUTE_CFG_DEV_INDEX	6
#define ROUTE_CFG_GW_INDEX	7
#define ROUTE_CFG_METRIC_NAME_INDEX	8
#define ROUTE_CFG_METRIC_INDEX	9
/*shnwind modify 2008.4.17*/
#define ROUTE_CMD_NO_METRIC_DEV "/sbin/route add -net %s netmask %s dev %s\n"
#define ROUTE_CMD_NO_METRIC_GW "/sbin/route add -net %s netmask %s gw %s\n"
#define ROUTE_CMD_WITH_METRIC_DEV "/sbin/route add -net %s netmask %s dev %s metric %s\n"
#define ROUTE_CMD_WITH_METRIC_GW "/sbin/route add -net %s netmask %s gw %s metric %s\n"
#define ROUTE_IP_INDEX	0
#define ROUTE_MASK_INDEX	1
#define ROUTE_IF_TYPE_INDEX	2
#define ROUTE_GW_INDEX	2
#define ROUTE_METRIC_INDEX	3
#define MAX_STATIC_ROUTE_NUM 16 //add by brian for change route mechanism
#ifdef TCSUPPORT_VPN
#define VPN					"VPN"
#define VPN_PSK_FILE		"/etc/vpn_psk.txt"
#define VPN_SP_FILE			"/etc/vpn_setkey.conf"
#define VPN_SA_FILE			"/etc/vpn_racoon.conf"
#define VPN_SETKEY_BIN		"/usr/bin/setkey"
#define VPN_RACOON_BIN		"/usr/bin/racoon"
#define VPN_ADD_RULE_FILE	"/etc/vpn_addIpTableRules"
#define VPN_DEL_RULE_FILE	"/etc/vpn_delIpTableRules"
#define VPN_DEL_RULE_FILE2	"/etc/vpn_delIpTableRules2"
#endif

#ifdef TCSUPPORT_LAYER3FORWARD_ENHANCEMENT
#define BASIC_ROUTE_TABLE_INDEX 150
#endif

#if defined(TCSUPPORT_XT_CONNLIMIT) || defined(TCSUPPORT_MULTI_NAT)
#define NAT    "Nat"
#endif
#define NAT_PATH	"/etc/nat_pvc%d/ipmode"
#define DMZ_PATH "/etc/nat_pvc%d/dmz"
#define VIRSERV_PATH "/etc/nat_pvc%d/vserver%d"
#define IPADDRMAP_SERVERIP_PATH "/etc/nat_pvc%d/server_ip"

//#define QOS_DROP  1  /*Rodney_20091115*/

/*QoS*/
#if 0  /*Rodney_20090724*/
#define QOS_SH	"/etc/qos_rule.sh"
#define QOS_MODULE_PATH "/lib/modules/%s/kernel/net/bridge/netfilter/"
#define QOS_MODULE_NUM  6
#define QOS "QoS"
#define QOS_COMMON "Common"
#define QOS_BROUTE  "ebtables -t broute"
#endif

#ifdef QOS_DROP  /*Rodney_20091115*/
#define QOS_DROP_PATH   "/lib/modules/%s/kernel/net/sched/"
#define QOS_DROP_TAIL   1
#define QOS_DROP_RED    2
#define QOS_DROP_WRED   4

#define QOS_PRIO_IPP    1
#define QOS_PRIO_DSCP   2
#define QOS_PRIO_QUEUE  4

#define QOS_PPP_INTERFACE 0
#define QOS_NAS_INTERFACE 1

#define DROP_LIMIT  20000
#define DROP_MIN    5000
#define DROP_MAX    10000
#define DROP_AVPKT  5000
#define DROP_BURST  1
#define DROP_PRO   1

#define ENABLE      1
#define DISABLE     0
#endif
#define QOS_SH					"/etc/qos_rule.sh"
#if KERNEL_2_6_36
#define QOS_MODULE_PATH 		"/lib/modules/2.6.36/kernel/net/bridge/netfilter/"
#define QOS_TC_MODULE_PATH		"/lib/modules/2.6.36/kernel/net/sched/"
#define QOS_EBT_MODULE_PATH 	"/lib/modules/2.6.36/kernel/net/bridge/netfilter/"
#else
#define QOS_MODULE_PATH 		"/lib/modules/2.6.22.15/kernel/net/bridge/netfilter/"
#define QOS_TC_MODULE_PATH		"/lib/modules/2.6.22.15/kernel/net/sched/"
#define QOS_EBT_MODULE_PATH 	"/lib/modules/2.6.22.15/kernel/net/bridge/netfilter/"
#endif
#define QOS_MODULE_NUM  		8
#define QOS_TC_MODULE_NUM		4
#ifdef IPV6
#define QOS_EBT_MODULE_NUM		8
#else
#define QOS_EBT_MODULE_NUM		6
#endif
#define QOS 					"QoS"
#define QOS_COMMON 				"Common"
#define QOS_BROUTE  "ebtables -t broute"

#define ON						1
#define OFF						0
#define MAX_CMD_LENGTH			256
#define CMD_BUF_LENGTH			64
#define	INSMODE					1
#define RMMODE					0
#define ADD						1
#define DEL						0
/* frank modify for hwnat qos bug */						
#if defined(TCSUPPORT_HWNAT)
#define MAX_TYPE_RULE_NUM			15
#else
#define MAX_TYPE_RULE_NUM			16
#endif
/* end frank modify for hwnat qos bug */						
#define MAX_APP_RULE_NUM			4
#define MAX_TEM_RULE_NUM			8
#define MAX_VLAN_RULE_NUM			8
#define PQ_QUEUE_NUM				4
#define	WRR_QUEUE_NUM				4
#define	CAR_QUEUE_NUM				6
#if defined(TCSUPPORT_PORT_TRIGGER)
#define MAX_PORTTRIGGERING_NODE                           8
#endif
//#define	QOS_DEBUG

#if 0  /*Orginal ci-command mode */
#define NAT	"Nat"

#define NAT_IPMODE	"IPMODE"
#define NAT_SUBPVC_ENTRY	"%s_PVC%d_Entry"
#define NAT_SUBRULE_ENTRY	"%s_PVC%d_%d_Entry"
/*#define NAT_RULE_INDEX	0*/
#define DMZ_ACTIVE_INDEX	0
/*#define IPADDRMAP_RULE_INDEX 0*/
#define ADSL	"Adsl"
#endif

#define PM	"PushMail"
#define PM_PAR_NUM	8
#define PM_LETTER_PATH 	"/etc/email/letter"

#define DMZ	"Dmz"
#define DMZ_SH	"/usr/script/dmz.sh"
#define VIRSERV	"VirServer"
#define VIRSERV_SH	"/usr/script/vserver.sh"
#define IPADDRMAP	"IpAddrMap"
#define IPADDRMAP_SERVER "Server"
#define IPADDRMAP_SH	"/usr/script/ipaddr_mapping.sh"
#define ADSL_ATTR_TRANSMODE	"TransMode"
#define ADSL_ATTR_MODE	"MODULATIONTYPE"
#define ADSL_ATTR_TYPE	"ANNEXTYPEA"
#define ADSL_ATTR_SRA	"dslx_sra"
#define ADSL_ATTR_BITSWAP	"dslx_bitswap"
#define ADSL_ATTR_SNRMOFFSET	"dslx_snrm_offset"
#define ADSL_ATTR_GINP	"dslx_ginp"
#define ADSL_ATTR_RXAGC	"adsl_rx_agc"
#define ADSL_OPEN_CMD "wan adsl opencmd"
#define ADSL_SETANNEX_CMD "wan ghs set annex"
#define ADSL_ATTR_DLA	"dslx_dla_enable"

#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_RT63365) || defined(TCSUPPORT_CPU_RT63368)
#define ADSL_ATTR_ESNP	"dslx_adsl_esnp" //Enhanced Sudden Noise Protection for ADSL
#endif

#ifdef TCSUPPORT_WAN_PTM
#define VDSL_ATTR_TARGETSNRM	"vdsl_snrm_offset"
#define VDSL_ATTR_TXGAINOFF	"vdsl_tx_gain_off" //tx power cut off
#define VDSL_ATTR_RXAGC	"vdsl_rx_agc" //rx agc gain
#define VDSL_ATTR_UPBO	"vdsl_upbo" //upstream power back off
#define VDSL_ATTR_ESNP	"dslx_vdsl_esnp" //Enhanced Sudden Noise Protection for VDSL
#define VDSL_ATTR_BITSWAP	"dslx_vdsl_bitswap"
#define VDSL_ATTR_GVECTOR "dslx_vdsl_vectoring"
#define VDSL_ATTR_NONSTD_GVECTOR	"dslx_vdsl_nonstd_vectoring"
#endif /* TCSUPPORT_WAN_PTM */

#ifdef TCSUPPORT_WAN_PTM
#define VDSL_PROFILE_30A_MULTI 0
#define VDSL_PROFILE_17A_MULTI 1
#define VDSL_PROFILE_12A_MULTI 2
#define VDSL_PROFILE_8A_MULTI 3
#endif /* TCSUPPORT_WAN_PTM */

#define DSLX_TESTLAB "dslx_testlab"

#define MISC_ATTR_HWNAT "hwnat_enable"
#define MISC_ATTR_TELNET "telnetd_enable"

#define ADSL_NAME "Adsl"
#define ADSL_ACTIVE "Active"
#define SAR_OPEN_CMD "wan adsl open"
#define SAR_CLOSE_CMD "wan adsl close"
#define SAR_RESET_CMD "wan adsl reset"

#define ADSL_PATH "/etc/adsl.conf"
#define ADSL_SH	"/etc/adsl.sh"
#define ADSL_BOOT_SH	"/etc/adsl_boot.sh"
#define ADSL_ENTRY 	"Adsl_Entry"
#define ADSL_NAME_INDEX 0
#define ADSL_CMD_INDEX 1

#define PARSE_PORT_SCARE_NUM 3
#define COMMA_SYMBOL ","
#define SPACE_SYMBOL " "
#define COLON_SYMBOL ':'

#if 0  /*Orginal ci-command mode */
int
firewall_read(mxml_node_t *node);
#endif

#define ALGSW_NODE_NAME	"ALGSwitch"
#define SYSLOG_DEFAULT_PATH 		"/tmp/var/log/messages"
#ifdef TCSUPPORT_SYSLOG
#ifdef TCSUPPORT_SYSLOG_ENHANCE
//#define	SYSLOG_DEBUG
#define SYSLOG_VALUE_LEN	8
#define SYSLOG_CONF_PATH	"/var/log/log.conf"
#define SYSLOG_NODE_NAME	"Syslog"
#define	SYSLOG_NODE_ENTRY	"Entry"
#endif
#endif


#if defined(TCSUPPORT_PORT_TRIGGER)
#define TOTAL_PORT_TRIGGER_PROTO	2
#define CMD_MAX_SIZE				300
#define TCP_UDP				 		"TCP/UDP"
#define PORT_TRIGGER_SH_PATH 		"/var/tmp/porttrigger.sh"
#define PREROUTING_RULE_CMD_FORMAT "iptables -t nat -A PREROUTING_RULE -i br0 -p %s --dport %s:%s -j TRIGGER --trigger-type out --trigger-proto %s --trigger-match %s-%s --trigger-relate %s-%s\n"
#define PREROUTING_WAN_CMD_FORMAT "iptables -t nat -A PREROUTING_WAN -p %s --dport %s:%s -j TRIGGER --trigger-type dnat\n"
#define FORWARD_WAN_CMD_FORMAT "iptables -t filter -A FORWARD_WAN -p %s --dport %s:%s -j TRIGGER --trigger-type in\n" 
#endif

int
firewall_boot(mxml_node_t *top);
int
firewall_init(void);
int
firewall_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int
firewall_write(mxml_node_t *top, mxml_node_t *parant);
int
firewall_verify(mxml_node_t *node);
int
firewall_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
firewall_check_filter(mxml_node_t *top);
void
firewall_scan_port_and_add_rule();

//Ren.B
int
misc_boot(mxml_node_t *top);
int
misc_init(void);
int
misc_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int
misc_write(mxml_node_t *top, mxml_node_t *parant);
int
misc_verify(mxml_node_t *node);
int
misc_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
//Ren.E

int
trafficMonitor_boot(mxml_node_t *top);
int
trafficMonitor_init(void);
int
trafficMonitor_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int
trafficMonitor_write(mxml_node_t *top, mxml_node_t *parant);
int
trafficMonitor_verify(mxml_node_t *node);
int
trafficMonitor_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
PushMail_boot(mxml_node_t *top);
int
PushMail_init(void);
int
PushMail_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int
PushMail_write(mxml_node_t *top, mxml_node_t *parant);
int
PushMail_verify(mxml_node_t *node);
int
PushMail_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#ifdef TCSUPPORT_DSL_LINE_DIAGNOSTIC
int
dslDiag_boot(mxml_node_t *top);
int
dslDiag_init(void);
int
dslDiag_write(mxml_node_t *top, mxml_node_t *parant);
int
dslDiag_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#ifdef CWMP
#define 	CWMP_ACTIVE				"Active"
#define		CWMP_CONREQ_PORT		"conReqPort"
#define		BUFLEN					10
#define		CMDBUF					128
#define  	PORT_LEN				8
#define		FIREWALL_RULE			0
#define		SPI_RULE				1
void 
cwmp_conReq_rule(mxml_node_t *top, int type);
#endif

int
route_init(void);
int
route_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);

int
route_write(mxml_node_t *top, mxml_node_t *parant);
int
route_verify(mxml_node_t *node);
int
route_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
route_boot(mxml_node_t *top);
/*krammer add for bug 1427*/

#ifdef TCSUPPORT_VPN
int
vpn_init(void);
int
vpn_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int
vpn_write(mxml_node_t *top, mxml_node_t *parant);
int
vpn_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif
void
update_route_sh(mxml_node_t *top, mxml_node_t *parant);

#if 0  /*Orginal ci-command mode */
int
nat_read(mxml_node_t *node);
#endif

int
nat_init(void);
int
nat_write(mxml_node_t *top, mxml_node_t *parant);
int
nat_verify(mxml_node_t *node);
int
nat_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#if defined(TCSUPPORT_PORT_TRIGGER)
int
portTriggering_init(void);
int
portTriggering_boot(mxml_node_t *top);
int
portTriggering_read(mxml_node_t *node, char nodeName[][MAX_NODE_NAME], char *attr);
int
portTriggering_write(mxml_node_t *top, mxml_node_t *parant);
int
portTriggering_verify(mxml_node_t *node);
int
portTriggering_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif
int
algswitch_init(void);
int
algswitch_boot(mxml_node_t *top);
int
algswitch_write(mxml_node_t *top, mxml_node_t *parant);
int
algswitch_verify(mxml_node_t *node);
int
algswitch_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);

#if 0  /*Orginal ci-command mode */
int
dmz_read(mxml_node_t *node);
#endif

int
dmz_init(void);
int
dmz_read(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr);
int
dmz_write(mxml_node_t *top, mxml_node_t *parant);
int
dmz_verify(mxml_node_t *node);
int
dmz_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
dmz_boot(mxml_node_t *top);

#if 0  /*Orginal ci-command mode */
int
virServer_read(mxml_node_t *node);
#endif

int
virServ_init(void);
int
virServer_write(mxml_node_t *top, mxml_node_t *parant);
int
virServer_verify(mxml_node_t *node);
int
virServer_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
virServ_boot(mxml_node_t *top);

#if 0  /*Orginal ci-command mode */
int
ipAddrMap_read(mxml_node_t *node);
#endif

#if 0  /*Orginal ci-command mode */
int
adsl_read(mxml_node_t *node);
#endif

int
adsl_boot(mxml_node_t *top);
int
adsl_init(void);
int
adsl_write(mxml_node_t *top, mxml_node_t *parant);
int
adsl_verify(mxml_node_t *node);
int
adsl_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
/****************shnwind add****************/
void create_route_exe_sh(mxml_node_t *top, int action);
void create_route_sh(mxml_node_t *top, mxml_node_t *parant);
int change_device_name(mxml_node_t *top, char *dev, char *ret);
//int rebuild_route(mxml_node_t *top);
#define FIREWALL_TMP_PATH "/etc/firewall.conf.tmp"
#define ROUTE_EXE_SH "/etc/route_exe.sh"
#define ROUTE_BOOT_SH	"/etc/route.sh"
#define ROUTE_TMP_PATH	"/tmp/route"
#define ROUTE_ADD 1
#define ROUTE_DEL 0
#ifdef TCSUPPORT_LAYER3FORWARD_ENHANCEMENT
#define ROUTE_NONE 2 //Add or Del depend on the value of attribute "Active"
#endif
/************shnwind add end***********/
#endif

#ifdef TCSUPPORT_QOS
#ifdef TCSUPPORT_QOS_QUEUE_MODIFY
int setQueueMask(char *qm, char sw_pri[][32], int qNum);
int qosDiscplineRule( mxml_node_t *top );
int qosTypeRule( mxml_node_t *top, int index );
#endif
#endif

#if 1  /*Rodney_20090724*/
int qos_init(void);
int qos_boot(mxml_node_t *top);
int qos_write(mxml_node_t *top, mxml_node_t *parant);
int qos_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int qos_paramcheck(mxml_node_t *top, char nodeName[][MAX_NODE_NAME], char qos_attr_name[][32], int index);
void qos_remarking(FILE *fp, int index, char *jumpchain);
void qos_IPv4rule(FILE *fp, int index, char *jumpchain);
void qos_8021Qrule(FILE *fp, int index, char *jumpchain);
int qos_checkRangeValue(unsigned int val1, unsigned int val2, unsigned int min, unsigned int max);
void qos_moduleInsert();
void qos_moduleRemove();

#else
/**********20080703 racing add************/
#if 0  /*Orginal ci-command mode */
int
qos_read(mxml_node_t *node);
#endif
int
qos_write(mxml_node_t *top, mxml_node_t *parant);
int
qos_verify(mxml_node_t *node);
int
qos_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#ifdef QOS_DROP  /*Rodney_20091115*/
void qos_drop_reset(mxml_node_t *top);
void qos_drop_set(int mode, int drop_mode_id, int prio_mode_id);
void qos_drop_set_rule(int interface, int pvc);
void qos_drop_unset_rule();
#endif

#if (defined(TCSUPPORT_WEBMON) || defined(TCSUPPORT_ACCESSLOG)) && defined(TCSUPPORT_USBHOST)
int backupLog_init(void);
#endif
#ifdef TCSUPPORT_WEBMON
int webHistory_init(void);
int webHistory_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif
#ifdef TCSUPPORT_ACCESSLOG
int accessLog_init(void);
int accessLog_boot(mxml_node_t *top);
int accessLog_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

/**********20080703 racing add************/
/*autopvc*/
#define MAX_VCPOOL_NUM 16
#define AUTOPVC "AutoPVC"
#define TMP_MAC_PATH "/tmp/mac"

typedef struct pvc_pool_s{
	int vpi;
	int vci;
	int encap;
	int state;
}pvc_pool_t;

typedef struct vc_pool_s{
	int pvc;
	int vpi;
	int vci;
}vc_pool_t;

#define AUTOPVC_STOP 0
#define AUTOPVC_START 1
#define AUTOPVC_SUCCESS 2
#define AUTOPVC_SUCCESS_AND_REPLACE 3

#define ENCAP_LLC 0
#define ENCAP_VC 1

#define PROBE_OAM 1
#define PROBE_PPPOE (1<<1)
#define PROBE_ARP (1<<2)
#define PROBE_DHCP (1<<3)
#define PROBE_PPPOA (1<<4)	//javi

#define AUTOPVC_SET_CMD "sys autopvc set"
#define AUTOPVC_ENCAP_CMD "sys autopvc encap"
#define AUTOPVC_SERVICE_CMD "sys autopvc service"

int
autopvc_init(void);
int
autopvc_write(mxml_node_t *top, mxml_node_t *parant);
int
autopvc_verify(mxml_node_t *node);
int
autopvc_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int check_vpi_vci(int vpi, int vci);
int get_current_pvc_setting(mxml_node_t *top);
int get_autopvc_setting(mxml_node_t *top);
int check_adsl_stat(void);
void
reset_all_autopvc_data();


/*autopvc*/
//#if defined(TCSUPPORT_C1_SYSLOG)
#if defined(TCSUPPORT_SYSLOG)
int
syslog_init(void);
int
syslog_write(mxml_node_t *top, mxml_node_t *parant);
int
syslog_verify(mxml_node_t *node);
int
syslog_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
syslog_boot(mxml_node_t *top);
#endif

#ifdef TCSUPPORT_PORTBIND

/*portbind*/
//#define  PORTBIND_DEBUG
#define  MAX_PORTBIND_NUM		16
#define	 CMD_BUF_LEN			128
#define  BUF_LEN				32
#define  PORTBIND				"PortBind"
#define	 PORTBIND_COMMON		"Common"
#define ADD_RULE_FOR_DHCP				1
#define	DEL_RULE_FOR_DHCP			0
#define ADD_RULE_FOR_DHCP_SH	"/tmp/etc/add_rule_for_dhcp.sh"
#define DEL_RULE_FOR_DHCP_SH	"/tmp/etc/del_rule_for_dhcp.sh"

struct if_map
{
	char *webIfName;
	char *ifName;
};

int
portbind_init(void);
int
portbind_write(mxml_node_t *top, mxml_node_t *parant);
int
portbind_verify(mxml_node_t *node);
int
portbind_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
portbind_boot(mxml_node_t *top);
void 
set_portbind_rule_for_dhcp(int bridgeModePVC[], int pvc_num, int iface, int action, int ebtabCreate); 
int
syn_portbind_summary(mxml_node_t *node);
int 
exec_portbind_cmd(char *cmd);
#endif

#ifdef TCSUPPORT_DMS
#define DMSCFG_PATH "/etc/dms_basic.cfg"
#define DMSDEVICECFG_PATH "/etc/dms_device.cfg"
#define DMS_START_PATH "/usr/script/dms_start.sh"
#define DMS_NODE "DMS"
#define DMSCFG_NODE "Basic"
#define DMSCFG_IPADDR_NODE "IPAddress"
#define DMSDEVICECFG_NODE "Device"

int
dms_boot(mxml_node_t *top);
int
dms_init(void);
int
dms_write(mxml_node_t *top, mxml_node_t *parant);
int
dms_verify(mxml_node_t *node);
int
dms_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#if defined(TCSUPPORT_AUTO_GENERATE)
int
autogenerate_init(void);
int
autogenerate_write(mxml_node_t *top, mxml_node_t *parant);
int
autogenerate_verify(mxml_node_t *node);
int
autogenerate_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
autogenerate_boot(mxml_node_t *top);
#endif

#ifdef TCSUPPORT_VLAN_TAG
#if defined(TCSUPPORT_VTAG_ALLDEV)
//#define	VLANTAG_DEBUG

#define VTAG_SWITCH_PROC		"/proc/tc3162/vlan_tag_sw"
#define	VTAG_ETH_PORTS_PROC		"/proc/tc3162/eth_ports"
#define	VTAG_WLAN_PORTS_PROC	"/proc/tc3162/wlan_ports"
#define	VTAG_USB_PORTS_PROC		"/proc/tc3162/usb_ports"
#define	VTAG_WAN_PORTS_PROC		"/proc/tc3162/wan_ports"
#if defined(TCSUPPORT_WAN_ETHER)
#define VTAG_WAN_CMD_FMT        "echo %d %d %d %d %d %d %d %d %d %d %d > %s"
#elif defined(TCSUPPORT_WAN_PTM)
#define VTAG_WAN_CMD_FMT        "echo %d %d %d %d %d %d %d %d %d %d > %s"
#else
#define VTAG_WAN_CMD_FMT        "echo %d %d %d %d %d %d %d %d > %s"
#endif

/* port type */
#define		ETH_PORTS_TYPE		0
#define		WLAN_PORTS_TYPE		1
#define		USB_PORTS_TYPE		2
#define		WAN_PORTS_TYPE		4

#define		VLAN_TAG_ENABLE			(1<<12)
#define		VLAN_TAG_SWITCH			(1<<13)

int 
get_pvid_from_node(mxml_node_t *top, int *ports, int ports_num, int type);
int
vlantag_init(void);
int
vlantag_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int
vlantag_boot(mxml_node_t *top);
#endif
#endif

#ifdef TCSUPPORT_SIPROXD
#ifndef PURE_BRIDGE
/*Alg*/
#define SIPROXD_DEFAULT_PORT          5060
#define SIPROXD_RTPPORT_LOW           7070
#define SIPROXD_RTPPORT_HIGH          7079
#define SIPROXD_CONF_PATH             "/etc/alg/siproxd.conf"
#define SIPROXD_PIDFILE               "/etc/alg/sippid"
#define SIPROXD_DIR                   "/etc/alg"
#define SIPROXD_SIP_LISTEN_PORT          9876

typedef enum
{
    SIPROXD_IPTABLE_ADD = 0,       
    SIPROXD_IPTABLE_REMOVE = 1    
}SIPROXD_IPTABLES_COMMAND;         

int alg_init(void);
int alg_verify(mxml_node_t *node);
int alg_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int alg_boot(mxml_node_t *top);
int siproxd_write_conf(mxml_node_t *top, int *sipPort);
int siproxd_modify_iptables(int sipPort, int cmdType);
void siproxd_stop(void);
void siproxd_start(mxml_node_t *top);
#endif /*ndef PURE_BRIDGE*/
#endif

typedef enum
{
	TRANS_Mode_ADSL = 0,
	TRANS_Mode_VDSL = 1,
	TRANS_Mode_ETHWAN = 2
}TRANS_Mode;
