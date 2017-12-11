/************************************************************************
 *
 *	Copyright (C) 2009 Trendchip Technologies, Corp.
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
#ifndef _ETHERCMD_H
#define _ETHERCMD_H

#include "libmii.h"
#include "../../../version/tcversion.h"


//#define ETHCMD_VERSION          "1.0.0"

#define GSW_BASE     		0x0
#define GSW_ARL_BASE     	(GSW_BASE + 0x0000)
#define GSW_BMU_BASE     	(GSW_BASE + 0x1000)
#define GSW_PORT_BASE     	(GSW_BASE + 0x2000)
#define GSW_MAC_BASE     	(GSW_BASE + 0x3000)
#define GSW_MIB_BASE     	(GSW_BASE + 0x4000)
#define GSW_CFG_BASE     	(GSW_BASE + 0x7000)

#define GSW_PCR(n)		(GSW_PORT_BASE + (n)*0x100 + 0x04)
#define GSW_MFC     		(GSW_ARL_BASE + 0x10)
#define GSW_UPW			(GSW_ARL_BASE + 0x44)
#define GSW_PEM(n)		(GSW_ARL_BASE + (n)*0x4 + 0x48)

#define GSW_MMSCR0_Q(n)		(GSW_BMU_BASE + (n)*0x8)
#define GSW_MMSCR1_Q(n)		(GSW_BMU_BASE + (n)*0x8 + 0x04)

#define GSW_PMCR(n)     	(GSW_MAC_BASE + (n)*0x100)
#define GSW_PMSR(n)     	(GSW_MAC_BASE + (n)*0x100 + 0x08)
#define GSW_PINT_EN(n)     	(GSW_MAC_BASE + (n)*0x100 + 0x10)
#define GSW_SMACCR0     	(GSW_MAC_BASE + 0xe4)
#define GSW_SMACCR1     	(GSW_MAC_BASE + 0xe8)
#define GSW_CKGCR		(GSW_MAC_BASE + 0xf0)

#define GSW_ESR(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x00)
#define GSW_INTS(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x04)
#define GSW_TGPC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x10)
#define GSW_TBOC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x14)
#define GSW_TGOC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x18)
#define GSW_TEPC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x1C)
#define GSW_RGPC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x20)
#define GSW_RBOC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x24)
#define GSW_RGOC(n)    		(GSW_MIB_BASE + (n)*0x100 + 0x28)
#define GSW_REPC1(n)   		(GSW_MIB_BASE + (n)*0x100 + 0x2C)
#define GSW_REPC2(n)   		(GSW_MIB_BASE + (n)*0x100 + 0x30)
#define GSW_MIBCNTEN  		(GSW_MIB_BASE + 0x800)
#define GSW_AECNT1  		(GSW_MIB_BASE + 0x804)
#define GSW_AECNT2  		(GSW_MIB_BASE + 0x808)

#define GSW_CFG_PPSC     	(GSW_CFG_BASE + 0x0)
#define GSW_CFG_PIAC     	(GSW_CFG_BASE + 0x4)
#define GSW_CFG_GPC     	(GSW_CFG_BASE + 0x14)

#define VPORT_FILE     "/var/vport_result" 

#define ETHCMD_ENABLE	"enable"
#define ETHCMD_DISABLE	"disable"
#define ETHCMD_PORT	"port"
#define ETHCMD_QUERY	"query"

#define ETHCMD_MEDIA_TYPE	"media_type"
#define ETHCMD_RESET		"reset"
#define ETHCMD_VLANPT		"vlanpt"
#define ETHCMD_LANCHIP		"lanchip"
#define ETHCMD_STATUS		"status"

#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
#define HELP_MEDIA_TYPE	"%s [option] [port 0-%d][swap]\tget/set media type\n" \
							"  [option]: auto - auto select\n" \
							"            100FD - 100Mb, Full Duplex\n" \
							"            100HD - 100Mb, Half Duplex\n" \
							"            10FD  - 10Mb,  Full Duplex\n" \
							"            10HD  - 10Mb,  Half Duplex\n" \
							" {port 0-%d}: required if <interface> is Ethernet Switch"
#define HELP_MEDIA_TYPE_INFO	"[option] [port num] [swap]\tget/set media type\n" \
							"  [option]: auto - auto select\n" \
							"            100FD - 100Mb, Full Duplex\n" \
							"            100HD - 100Mb, Half Duplex\n" \
							"            10FD  - 10Mb,  Full Duplex\n" \
							"            10HD  - 10Mb,  Half Duplex\n" \
							" {port num}: required if <interface> is Ethernet Switch"							
#else
#define HELP_MEDIA_TYPE	"[option] [port 0-3]\tget/set media type\n" \
							"  [option]: auto - auto select\n" \
							"            100FD - 100Mb, Full Duplex\n" \
							"            100HD - 100Mb, Half Duplex\n" \
							"            10FD  - 10Mb,  Full Duplex\n" \
							"            10HD  - 10Mb,  Half Duplex\n" \
							" {port 0-3}: required if <interface> is Ethernet Switch"
#endif
#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
#define HELP_RESET	"%s {port 0-%d} [swap]\tsoft reset transceiver\n" \
					"  {port 0-%d}: required if <interface> is Ethernet Switch"
#define HELP_RESET_INFO	"{port num} [swap]\tsoft reset transceiver\n" \
					"  {port num} : required if <interface> is Ethernet Switch"
#else					
#define HELP_RESET	"%s {port 0-%d}\tsoft reset transceiver\n" \
					"  {port num}: required if <interface> is Ethernet Switch\n"
#endif					
#define HELP_VLANPT	"enable|disable|query VLAN port of LanSwitch "

#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
#define HELP_LANCHIP 	"%s {enable|disable}\tenable/disable all port of lan chip\n" \
						"  [port 0-%d] [swap]\t\t\tenable/disable a port of lan chip\n"
#define HELP_LANCHIP_INFO 	"{enable|disable}\tenable/disable all port of lan chip\n" \
						"  [port num] [swap]\t\t\tenable/disable a port of lan chip\n"	
#else
#define HELP_LANCHIP 	"{enable|disable}\tenable/disable all port of lan chip\n" \
						"  [port 0-3]\t\t\tenable/disable a port of lan chip\n"					
#endif						
#define HELP_STATUS 	"get the Link status of ports\n"
#define HELP_IGMPSNOOP 	"enable|disable|query igmpsnoop of LanSwitch "

#define HELP_VLAN_ACTIVE	"vlan active <0|1>"
#define HELP_VLAN_PVID		"vlan pvid <port> <pvid>"

#if defined(EXTEND_SETVID_PARAM) || defined(SQA_VERIFY)
#define HELP_VLAN_VID		"vlan vid <index> <active:0|1> <vid> <portMap> <egtagPortMap>\n" \
							"	 <ivl_en> <fid> <stag>\n"
#else
#define HELP_VLAN_VID		"vlan vid <index> <active:0|1> <vid> <portMap> <tagPortMap>\n"
#endif //SQA_VERIFY

#ifdef ETHCMD_EXTENSION
#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
#define HELP_PORTBINDING	"portbinding <index> <add:1|delete:0> <interface:e0~e%d, p0~p7,w0~w3, u0>"
#define HELP_PORTBINDING_INFO	"portbinding <index> <add:1|delete:0> <interface:ex, px,wx, ux>"
#else
#define HELP_PORTBINDING	"portbinding <index> <add:1|delete:0> <interface:e0~e3, p0~p7,w0~w3, u0>"
#endif
#endif

//#if defined(SQA_VERIFY)

#define MT7530_UPW_REG_UPDATE 1	//20121011 YM_Chan request for MT7530 E2 h/w register spec modification

#define HELP_QOS_TYPE		"qos type <sp:1|wrr:0>\n"
#ifdef MT7530_UPW_REG_UPDATE	//20121011 YM_Chan request for MT7530 E2 h/w register spec modification
#define HELP_QOS_BASE		"qos base <port-based:0|tag-based:1|dscp-based:2|acl-based:3|arl-based:4|stag-based:5>\n"
#else
#define HELP_QOS_BASE		"qos base <port-based:0|tag-based:1|dscp-based:2|acl-based:3|arl-based:4>\n"
#endif
#define HELP_QOS_PRIO_QMAP	"qos prio-qmap <prio:0~7> <queue:0~7>\n"
#define HELP_QOS_PRIO_TAGMAP	"qos prio-tagmap <prio:0~7> <tag:0~7>\n"
#define HELP_QOS_PRIO_DSCPMAP	"qos prio-dscpmap <prio:0~7> <dscp:0~63>\n"
//#define HELP_QOS_VPRI_QMAP	"qos vprio-qmap <prio:0~7> <queue:0~7>\n"
#define HELP_QOS_PORT_PRIO	"qos port-prio <port> <prio:0~7>\n"
#define HELP_QOS_PORT_WEIGHT	"qos port-weight <port:0~7> <q0> <q1> <q2> <q3> <q4> <q5> <q6> <q7>\n" \
							 " <qn>: the weight of queue n, range: 1~16\n"
#define HELP_QOS_DSCP_PRIO	"qos dscp-prio <dscp:0~63> <prio:0~7> : for ingress\n"

#define HELP_ARL_L2LEN_CHK	"arl l2len-chk <active:0|1>\n"

#define HELP_ARL_AGING		"arl aging <active:0|1> <time:1~65536>\n"

#define HELP_ARL_MAC_TBL_ADD	"arl mactbl-add <MacAddr> <DestPortMap>\n"\
	                        " ** optional : <leaky_en:0|1> <eg_tag:0~7> <usr_pri:0~7> <sa_mir_en:0|1> <sa_port_fw:0~7>\n"

#define HELP_ARL_DIP_TBL_ADD	"arl diptbl-add <DIP> <DestPortMap> <leaky_en:0|1> <eg_tag:0~7> <usr_pri:0~7> <status:0~3>\n"

#define HELP_ARL_SIP_TBL_ADD	"arl siptbl-add <DIP> <SIP> <DestPortMap> <status:0~3>\n"

#define HELP_ACL_SETPORTEN	"acl enable <port> <port_enable:0|1>\n"
#define HELP_ACL_SETMULTIHIT	"acl multi-hit <acl_multi:0|1>\n"
#define HELP_ACL_ACL_TBL_ADD	"arl acltbl-add <tbl_idx:0~63> <vawd1> <vawd2>\n"
#define HELP_ACL_MASK_TBL_ADD	"arl masktbl-add <tbl_idx:0~31> <vawd1> <vawd2>\n"
#define HELP_ACL_RULE_TBL_ADD	"arl ruletbl-add <tbl_idx:0~31> <vawd1> <vawd2>\n"
#define HELP_ACL_RATE_TBL_ADD	"arl ratetbl-add <tbl_idx:0~31> <vawd1> <vawd2>\n"


#define HELP_VLAN_PORT_MODE	"vlan port-mode <port> <mode:0~3>\n" \
							"<mode>: 0: port matrix mode\n" \
							"        1: fallback mode\n" \
							"        2: check mode\n" \
							"        3: security mode\n"\

#define HELP_VLAN_PORT_ATTR	"vlan port-attr <port> <attr:0~3>\n" \
							"<attr>: 0: user port\n" \
							"        1: statck port\n" \
							"        2: translation port\n" \
							"        3: transparent port\n"

#define HELP_VLAN_EGRESS_TAG_PVC	"vlan eg-tag-pvc <port> <eg_tag:0~7>\n" \
							 "<eg_tag>: 0: disable\n" \
							 "          1: consistent\n" \
							 "          2: reserved\n" \
							 "          3: reserved\n" \
							 "          4: untagged\n" \
							 "          5: swap\n" \
							 "          6: tagged\n" \
							 "          7: stack\n"

#define HELP_VLAN_EGRESS_TAG_PCR	"vlan eg-tag-pcr <port> <eg_tag:0~3>\n" \
							 "<eg_tag>: 0: untagged\n" \
							 "          1: swap\n" \
							 "          2: tagged\n" \
							 "          3: stack\n"

#define HELP_VLAN_ACC_FRM	"vlan acc-frm <port> <acceptable_frame_type:0~3>\n" \
							"<type>: 0: admit all frames\n" \
							"        1: admit only vlan-taged frames\n" \
							"        2: admit only untagged or priority-tagged frames\n" \
							"        3: reserved\n"


#define HELP_SWITCH_RESET	"switch software reset\n"
#define HELP_RATE_CONTROLL	"ratectl <ingress:1|exgress:0> <port: 0-6> <rate(kbps): 1-1000000)>\n"
#define HELP_MACCTL_COLLMT	"macctl col-cnt <max collision count limit:0|1~15>\n"
#define HELP_MACCTL_FC		"macctl fc <enable:0|1>\n"

#define HELP_MIRROR_EN		"portmirror enable <mirror_en:0|1> <mirror_port: 0-6>\n"
#define HELP_MIRROR_PORTBASED	"portmirror port-based <port> <port_tx_mir:0|1> <port_rx_mir:0|1> <acl_mir:0|1> <vlan_mis:0|1> <igmp_mir:0|1>\n"

#define HELP_PHY_AN_EN		"phyctl an <port> <auto_negotiation_en:0|1>\n"
#define HELP_PHY_FC_EN		"phyctl fc <port> <full_duplex_pause_capable:0|1>\n"

#define HELP_STP		"stp <port> <fid> <state>\n" \
						 "<state>: 0: Disable/Discarding\n" \
						 "         1: Blocking/Listening/Discarding\n" \
						 "         2: Learning\n" \
						 "         3: Forwarding\n"

//#endif //SQA_VERIFY

#define ERR_MEDIAY_TYPE 0xFFFF
#define ENABLE_LAN_MASK 0xF7FF

#define MAX_VID_VALUE	4095
#define MAX_VLAN_RULE	16
#define NONE_IF	-1
#define ETHCMD_ERR	-1
#define MAX_PVCS	8

#define LAN_IF	"eth0"
#define ETHUP_NAMESIZE 16
#define ETHUP_WAN "nas0"
#define ETHUP_MAXPRORITY 7
#define ETHUP_MINPRORITY 0
#define ETHUP_CMDSIZE 128
#define ETHUP_ADDWANCMDNUM 5
#ifdef TCSUPPORT_TC2031
#define ETHUP_WAIT_PHY_READY	5
#else
#define ETHUP_WAIT_PHY_READY	3
#endif
#define PHY_COMPANYID 0x03a2
#define MAX_PHY_NUM 32

#if 0
#define EBT_FWD_FLUSH 		"ebtables -F FORWARD"
#define EBT_FWD_ACCEPT		"ebtables -P FORWARD ACCEPT"
#define EBT_MODULES_INSTALL	"insmod /lib/modules/%s/kernel/net/bridge/netfilter/%s"
#define EBT_MODULES_REMOVE	"rmmod ebtable_filter.ko;rmmod ebtables.ko"
#define EBT_FWD_DROP_RULE 	"ebtables -A FORWARD -i eth0.%d -o eth0.%d -j DROP"
#endif

#define LAN_PORT_ITF_PREFIX	"elan."

#define VLAN_GROUP_ARRAY_LEN 4096
#define START_VLAN_VID	0x1

struct linkType{
	char* name;
	u16	val; 
};

struct portMap{
	char* ifname;
	char* real_name;
};

enum vport_t{
	enable=0,
	disable,
	query
};

#define REG_MFC_ADDR			(0x0010)
#define REG_MFC_MIRROR_PORT_OFFT	(0)
#define REG_MFC_MIRROR_PORT_LENG	(3)
#define REG_MFC_MIRROR_PORT_RELMASK	(0x00000007)
#define REG_MFC_MIRROR_PORT_MASK	(REG_MFC_MIRROR_PORT_RELMASK << REG_MFC_MIRROR_PORT_OFFT)
#define REG_MFC_MIRROR_EN_OFFT		(3)
#define REG_MFC_MIRROR_EN_LENG		(1)
#define REG_MFC_MIRROR_EN_RELMASK	(0x00000001)
#define REG_MFC_MIRROR_EN_MASK		(REG_MFC_MIRROR_EN_RELMASK << REG_MFC_MIRROR_EN_OFFT)

#define REG_ATA1_ADDR			(0x0074)
#define REG_ATA2_ADDR			(0x0078)

#define REG_ATWD_ADDR			(0x007C)
#define REG_ATWD_STATUS_OFFT		(2)
#define REG_ATWD_STATUS_LENG		(2)
#define REG_ATWD_STATUS_RELMASK		(0x00000003)
#define REG_ATWD_STATUS_MASK		(REG_ATWD_STATUS_RELMASK << REG_ATWD_STATUS_OFFT)
#define REG_ATWD_PORT_OFFT		(4)
#define REG_ATWD_PORT_LENG		(8)
#define REG_ATWD_PORT_RELMASK		(0x000000FF)
#define REG_ATWD_PORT_MASK		(REG_ATWD_PORT_RELMASK << REG_ATWD_PORT_OFFT)
#define REG_ATWD_LEAKY_EN_OFFT		(12)
#define REG_ATWD_LEAKY_EN_LENG		(1)
#define REG_ATWD_LEAKY_EN_RELMASK	(0x00000001)
#define REG_ATWD_LEAKY_EN_MASK		(REG_ATWD_LEAKY_EN_RELMASK << REG_ATWD_LEAKY_EN_OFFT)
#define REG_ATWD_EG_TAG_OFFT		(13)
#define REG_ATWD_EG_TAG_LENG		(3)
#define REG_ATWD_EG_TAG_RELMASK		(0x00000007)
#define REG_ATWD_EG_TAG_MASK		(REG_ATWD_EG_TAG_RELMASK << REG_ATWD_EG_TAG_OFFT)
#define REG_ATWD_USR_PRI_OFFT		(16)
#define REG_ATWD_USR_PRI_LENG		(3)
#define REG_ATWD_USR_PRI_RELMASK	(0x00000007)
#define REG_ATWD_USR_PRI_MASK		(REG_ATWD_USR_PRI_RELMASK << REG_ATWD_USR_PRI_OFFT)
#define REG_ATWD_SA_MIR_EN_OFFT		(19)
#define REG_ATWD_SA_MIR_EN_LENG		(1)
#define REG_ATWD_SA_MIR_EN_RELMASK	(0x00000001)
#define REG_ATWD_SA_MIR_EN_MASK		(REG_ATWD_SA_MIR_EN_RELMASK << REG_ATWD_SA_MIR_EN_OFFT)
#define REG_ATWD_SA_PORT_FW_OFFT	(20)
#define REG_ATWD_SA_PORT_FW_LENG	(3)
#define REG_ATWD_SA_PORT_FW_RELMASK	(0x00000007)
#define REG_ATWD_SA_PORT_FW_MASK	(REG_ATWD_SA_PORT_FW_RELMASK << REG_ATWD_SA_PORT_FW_OFFT)

#define REG_ATC_ADDR			(0x0080)
#define REG_ATC_AC_CMD_OFFT		(0)
#define REG_ATC_AC_CMD_LENG		(3)
#define REG_ATC_AC_CMD_RELMASK		(0x00000007)
#define REG_ATC_AC_CMD_MASK		(REG_ATC_AC_CMD_RELMASK << REG_ATC_AC_CMD_OFFT)
#define REG_ATC_AC_SAT_OFFT		(4)
#define REG_ATC_AC_SAT_LENG		(2)
#define REG_ATC_AC_SAT_RELMASK		(0x00000003)
#define REG_ATC_AC_SAT_MASK		(REG_ATC_AC_SAT_RELMASK << REG_ATC_AC_SAT_OFFT)
#define REG_ATC_AC_MAT_OFFT		(8)
#define REG_ATC_AC_MAT_LENG		(4)
#define REG_ATC_AC_MAT_RELMASK		(0x0000000F)
#define REG_ATC_AC_MAT_MASK		(REG_ATC_AC_MAT_RELMASK << REG_ATC_AC_MAT_OFFT)
#define REG_AT_SRCH_HIT_OFFT		(13)
#define REG_AT_SRCH_HIT_RELMASK		(0x00000001)
#define REG_AT_SRCH_HIT_MASK		(REG_AT_SRCH_HIT_RELMASK << REG_AT_SRCH_HIT_OFFT)
#define REG_AT_SRCH_END_OFFT		(14)
#define REG_AT_SRCH_END_RELMASK		(0x00000001)
#define REG_AT_SRCH_END_MASK		(REG_AT_SRCH_END_RELMASK << REG_AT_SRCH_END_OFFT)
#define REG_ATC_BUSY_OFFT		(15)
#define REG_ATC_BUSY_LENG		(1)
#define REG_ATC_BUSY_RELMASK		(0x00000001)
#define REG_ATC_BUSY_MASK		(REG_ATC_BUSY_RELMASK << REG_ATC_BUSY_OFFT)
#define REG_AT_ADDR_OFFT		(16)
#define REG_AT_ADDR_LENG		(12)
#define REG_AT_ADDR_RELMASK		(0x00000FFF)
#define REG_AT_ADDR_MASK		(REG_AT_ADDR_RELMASK << REG_AT_ADDR_OFFT)

#define REG_TSRA1_ADDR			(0x0084)
#define REG_TSRA2_ADDR			(0x0088)
#define REG_ATRD_ADDR			(0x008C)

#define REG_VTCR_ADDR			(0x0090)
#define REG_VTCR_VID_OFFT		(0)
#define REG_VTCR_VID_LENG		(12)
#define REG_VTCR_VID_RELMASK		(0x00000FFF)
#define REG_VTCR_VID_MASK		(REG_VTCR_VID_RELMASK << REG_VTCR_VID_OFFT)
#define REG_VTCR_FUNC_OFFT		(12)
#define REG_VTCR_FUNC_LENG		(4)
#define REG_VTCR_FUNC_RELMASK		(0x0000000F)
#define REG_VTCR_FUNC_MASK		(REG_VTCR_FUNC_RELMASK << REG_VTCR_FUNC_OFFT)
#define REG_VTCR_IDX_INVLD_OFFT		(16)
#define REG_VTCR_IDX_INVLD_RELMASK	(0x00000001)
#define REG_VTCR_IDX_INVLD_MASK		(REG_VTCR_IDX_INVLD_RELMASK << REG_VTCR_IDX_INVLD_OFFT)
#define REG_VTCR_BUSY_OFFT		(31)
#define REG_VTCR_BUSY_RELMASK		(0x00000001)
#define REG_VTCR_BUSY_MASK		(REG_VTCR_BUSY_RELMASK << REG_VTCR_BUSY_OFFT)

#define REG_VAWD1_ADDR			(0x0094)
#define REG_VAWD2_ADDR			(0x0098)

#define REG_GFCCR0_ADDR			(0x1FE0)
#define REG_FC_EN_OFFT			(31)
#define REG_FC_EN_RELMASK		(0x00000001)
#define REG_FC_EN_MASK			(REG_FC_EN_RELMASK << REG_FC_EN_OFFT)

#define REG_SSC_P0_ADDR			(0x2000)

#define REG_PCR_P0_ADDR			(0x2004)
#define REG_PCR_VLAN_MIS_OFFT		(2)
#define REG_PCR_VLAN_MIS_LENG		(1)
#define REG_PCR_VLAN_MIS_RELMASK	(0x00000001)
#define REG_PCR_VLAN_MIS_MASK		(REG_PCR_VLAN_MIS_RELMASK << REG_PCR_VLAN_MIS_OFFT)
#define REG_PCR_ACL_MIR_OFFT		(7)
#define REG_PCR_ACL_MIR_LENG		(1)
#define REG_PCR_ACL_MIR_RELMASK		(0x00000001)
#define REG_PCR_ACL_MIR_MASK		(REG_PCR_ACL_MIR_RELMASK << REG_PCR_ACL_MIR_OFFT)
#define REG_PORT_RX_MIR_OFFT		(8)
#define REG_PORT_RX_MIR_LENG		(1)
#define REG_PORT_RX_MIR_RELMASK		(0x00000001)
#define REG_PORT_RX_MIR_MASK		(REG_PORT_RX_MIR_RELMASK << REG_PORT_RX_MIR_OFFT)
#define REG_PORT_TX_MIR_OFFT		(9)
#define REG_PORT_TX_MIR_LENG		(1)
#define REG_PORT_TX_MIR_RELMASK		(0x00000001)
#define REG_PORT_TX_MIR_MASK		(REG_PORT_TX_MIR_RELMASK << REG_PORT_TX_MIR_OFFT)
#define REG_PORT_ACL_EN_OFFT		(10)
#define REG_PORT_ACL_EN_LENG		(1)
#define REG_PORT_ACL_EN_RELMASK		(0x00000001)
#define REG_PORT_ACL_EN_MASK		(REG_PORT_ACL_EN_RELMASK << REG_PORT_ACL_EN_OFFT)
#define REG_PCR_EG_TAG_OFFT		(28)
#define REG_PCR_EG_TAG_LENG		(2)
#define REG_PCR_EG_TAG_RELMASK		(0x00000003)
#define REG_PCR_EG_TAG_MASK		(REG_PCR_EG_TAG_RELMASK << REG_PCR_EG_TAG_OFFT)

#define REG_PIC_P0_ADDR			(0x2008)
#define REG_PIC_IGMP_MIR_OFFT		(19)
#define REG_PIC_IGMP_MIR_LENG		(1)
#define REG_PIC_IGMP_MIR_RELMASK	(0x00000001)
#define REG_PIC_IGMP_MIR_MASK		(REG_PIC_IGMP_MIR_RELMASK << REG_PIC_IGMP_MIR_OFFT)

#define REG_PSC_P0_ADDR			(0x200C)

#define REG_PVC_P0_ADDR			(0x2010)
#define REG_PVC_ACC_FRM_OFFT		(0)
#define REG_PVC_ACC_FRM_LENG		(2)
#define REG_PVC_ACC_FRM_RELMASK		(0x00000003)
#define REG_PVC_ACC_FRM_MASK		(REG_PVC_ACC_FRM_RELMASK << REG_PVC_ACC_FRM_OFFT)
#define REG_PVC_EG_TAG_OFFT		(8)
#define REG_PVC_EG_TAG_LENG		(3)
#define REG_PVC_EG_TAG_RELMASK		(0x00000007)
#define REG_PVC_EG_TAG_MASK		(REG_PVC_EG_TAG_RELMASK << REG_PVC_EG_TAG_OFFT)

#define REG_PPBV1_P0_ADDR		(0x2014)
#define REG_PPBV2_P0_ADDR		(0x2018)
#define REG_BSR_P0_ADDR			(0x201C)
#define REG_STAG01_P0_ADDR		(0x2020)
#define REG_STAG23_P0_ADDR		(0x2024)
#define REG_STAG45_P0_ADDR		(0x2028)
#define REG_STAG67_P0_ADDR		(0x202C)

#define REG_CMACCR_ADDR			(0x30E0)
#define REG_MTCC_LMT_OFFT		(9)
#define REG_MTCC_LMT_LENG		(4)
#define REG_MTCC_LMT_RELMASK		(0x0000000F)
#define REG_MTCC_LMT_MASK		(REG_MTCC_LMT_RELMASK << REG_MTCC_LMT_OFFT)

#endif
