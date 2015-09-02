#ifndef _RESTORE_ROM_T_H
#define _RESTORE_ROM_T_H

typedef unsigned long		uint32;
typedef unsigned short 		uint16;
typedef unsigned char		uint8;
typedef unsigned long int 		ip4a;
typedef signed short		 	sint15;
/* Extract a short from a long */
#define hiword_o2u(x)       ((uint16)((x) >> 16))
#define loword_o2u(x)       ((uint16)(x))

/* Extract a byte from a short */
#define hibyte_o2u(x)       ((uint8)((x) >> 8))
#define lobyte_o2u(x)       ((uint8)(x))


#define SEPCHAR_SPACE " \t\0"
#define SEPCHAR_SYMBOL '@'
#define BACKUP_ROM_T_READ_FILE  "/tmp/rom_tInfoRead"
#define SYSINFO_FLAG "SysInfo"
#define ENTRY_FLAG "Entry"
#define ROMFILE_FLAG "RestoreLinos"
#define WAN_O2U "Wan"
#define COMMON_O2U "Common"
#define ACTIVE_O2U "Active"
#define Entry0_O2U "Entry0"
#define AuthMode_O2U "AuthMode"
#define CHANNEL_O2U "Channel"
#define LAN_O2U "Lan"
#define ENTRY_O2U "Entry"
#define ENTRY0_O2U "Entry0"
#define NETMASK_O2U "netmask"
#define IP_O2U "IP"
#define DHCPD_O2U "Dhcpd"
#define LANALIAS_O2U "LanAlias"
#define LANRIPD_O2U "LanRipd"
#define DPROXY_O2U "Dproxy"
#define DHCPRELAY_O2U "DhcpRelay"
#define WLAN_O2U "WLan"
#define BssidNum 1

#define PPP_USER_NAME_MAXLEN 64
#define PPP_PASSWORD_MAXLEN 64

#define		FLD_STR_30					(255+1)
#define		FLD_STR_31					(255+2)
#define		FLD_STR_8					(255+3)
#define		FLD_FILTER_APPLY			(255+4)
#define		FLD_BOOLEAN				(255+5)
#define		FLD_IP						(255+6)
#define		FLD_INT_2_TO_5				(255+7)
#define		FLD_INT_1_TO_2				(255+8)
#define		FLD_STR_9					(255+9)
#define		FLD_STR_15					(255+10)
#define		FLD_INT_LEASE_TIME			(255+11)
#define		FLD_STR_32					(255+12)
#define		FLD_STR_64					(255+13)
#define		FLD_INT_1500_TO_2347		(255+14)
#define		FLD_INT_256_TO_2346		(255+15)
#define		FLD_INT_1_TO_4				(255+16)
#define		FLD_STR_WEP_KEY			(255+17)

#define		FLD_NOT_CARE				0
#define		FLD_INT_2_ONLY				1
#define		FLD_INT_BELOW_2			2
#define		FLD_INT_BELOW_3			3
#define		FLD_INT_BELOW_4			4
#define		FLD2_INT_BELOW_4			5
#define		FLD_INT_BELOW_14			14
#define		FLD_INT_BELOW_15			15
#define		FLD_INT_BELOW_32			32
#define		FLD_INT_BELOW_254			254
#define		FLD_INT_BELOW_255			255

#define		FLD_INT_BELOW_65535		65535
#define		FLD_INT_32_TO_65535		(65535-1)

#define M3_2_DHCP_TYPE				30200001 	//"DHCP <0(None) | 1(Server) | 2(Relay)>", INT, 1},
#define M3_2_CLIENT_IP_START			30200002 	//"Client IP Pool Starting Address", IP, 4},
#define M3_2_CLIENT_IP_POOL			30200003 	//"Size of Client IP Pool", INT, 1},
#define M3_2_PRIMARY_DNS			30200004 	//"Primary DNS Server", IP, 4},
#define M3_2_SECONDARY_DNS			30200005 	//"Secondary DNS Server", IP, 4},
#define M3_2_REMOTE_DHCP_SRV		30200006 	//"Remote DHCP Server", IP, 4},
#define M3_2_DHCP_SERVER_LEASE_TIME	30200007	// dhcp server lease time
#define M3_2_IP_ADDR					30200008	//"IP Address", IP, 4},
#define M3_2_IP_SUBNET_MASK			30200009 	//"IP Subnet Mask", INT, 1},
#define M3_2_RIP_DIRECTION			30200010 	//"RIP Direction <0(None) | 1(Both) | 2(In Only) | 3(Out Only)>", SUBI, 1},
#define M3_2_RIP_VERSION				30200011 	//"Version <0(Rip-1) | 1(Rip-2B) |2(Rip-2M)>", SUBI, 1},
#define M3_2_1_ALIAS1				30201001 	//"IP Alias 1 <0(No) | 1(Yes)>", SUBI, 1},
#define M3_2_1_ALIAS1_IP_ADDR		30201002 	//"IP Address", IP, 4},
#define M3_2_1_ALIAS1_SUBNET_MASK	30201003 	//"IP Subnet Mask", INT, 1},

	/*	Menu 3.5 Wireless Lan Setup	*/
#define		M3_5_ESSID							30500001 	//"ESSID", STR, 33, NOT_USED, NOT_USED},
#define		M3_5_HIDE_ESSID					30500002	//"Hide ESSID <0(No) | 1(Yes)>", UINT_SPT, 2, NOT_USED, NOT_USED},
#define		M3_5_CHANNEL_ID					30500003 	//"Channel ID <1|2|3|4|5|6|7|8|9|10|11|12|13|14>", UINT_SPT, 1, NOT_USED, NOT_USED},
#define		M3_5_RTS_THRESHOLD				30500004 	//"RTS Threshold <0 ~ 2432>", INT_SPT, 2, NOT_USED, NOT_USED},
#define		M3_5_FRAG_THRESHOLD				30500005 	//"FRAG. Threshold<256 ~ 2432>", INT_SPT, 2, NOT_USED, NOT_USED},
#define		M3_5_WEP							30500006 	//"WEP <0(DISABLE) | 1(64-bit WEP) | 2(128-bet WEP)>", INT_SPT, 1, NOT_USED, NOT_USED},
#define		M3_5_WEP_DEFAULT_KEY			30500007 	//"Default Key <1|2|3|4>", INT_SPT, 1, NOT_USED, NOT_USED},
#define		M3_5_WEP_KEY1						30500008 	//"WEP Key1", STR, 13, NOT_USED, NOT_USED},
#define		M3_5_WEP_KEY2						30500009 	//"WEP Key2", STR, 13, NOT_USED, NOT_USED},
#define		M3_5_WEP_KEY3						30500010 	//"WEP Key3", STR, 13, NOT_USED, NOT_USED},
#define		M3_5_WEP_KEY4						30500011 	//"WEP Key4", STR, 13, NOT_USED, NOT_USED},
#define		M3_5_WLAN_ACTIVE					30500012

	/*	Menu 3.5.1 WLan MAC Address Filter */
#define		M3_5_1_ACTIVE						30501001 	//"Mac Filter Active <0(No) | 1(Yes)>", UINT_SPT, 1, NOT_USED, NOT_USED},
#define		M3_5_1_FILTER_ACTION				30501002	//"Filter Action <0(Allow) | 1(Deny)>", UINT_SPT, 1, NOT_USED, NOT_USED},
#define		M3_5_1_ADDR_1						30501003 	//"Address  1", UINT_SPT, 6, NOT_USED, NOT_USED},
#define		M3_5_1_ADDR_2						30501004 	//"Address  2", UINT_SPT, 6, NOT_USED, NOT_USED},
#define		M3_5_1_ADDR_3						30501005 	//"Address  3", UINT_SPT, 6, NOT_USED, NOT_USED},
#define		M3_5_1_ADDR_4						30501006 	//"Address  4", UINT_SPT, 6, NOT_USED, NOT_USED},
#define		M3_5_1_ADDR_5						30501007 	//"Address  5", UINT_SPT, 6, NOT_USED, NOT_USED},
#define		M3_5_1_ADDR_6						30501008 	//"Address  6", UINT_SPT, 6, NOT_USED, NOT_USED},
#define		M3_5_1_ADDR_7						30501009 	//"Address  7", UINT_SPT, 6, NOT_USED, NOT_USED},
#define		M3_5_1_ADDR_8						30501010 	//"Address  8", UINT_SPT, 6, NOT_USED, NOT_USED},

#define M4_ISP				40000001 	//"ISP <0(No) | 1(Yes)>", SUBI, 2, 7, 1},
#define M4_CONFIG			40000000 	//"Configured <0(No) | 1(Yes)>", SUBI, 2, 0, 1},
#define M4_ACTIVE			40000002 	//"Active <0(No) | 1(Yes)>", SUBI, 2, 1, 1},
#define M4_ENCAPSULATION	40000004 	//"Encapsulation <2(PPPOE) | 3(RFC 1483)| 4(PPP)| 5(ENET ENCAP)>", UINT, 2, NOT_USED, NOT_USED},
#define M4_ISP_NAME		40000003 	//"ISP's Name", STR, 10, NOT_USED, NOT_USED},
#define M4_MULTIPLEX		40000005 	//"Multiplexing <1(LLC-based) | 2(VC-based)", UINT, 2, NOT_USED, NOT_USED},
#define M4_VPI				40000006 	//"VPI #", UINT, 2, NOT_USED, NOT_USED},
#define M4_VCI				40000007 	//"VCI #", UINT, 2, NOT_USED, NOT_USED},
#define M4_SERVICE_NAME	40000008 	//"Service Name", STR, 32, NOT_USED, NOT_USED},
#define M4_LOGIN_NAME		40000009 	//"My Login", STR, 32, NOT_USED, NOT_USED},
#define M4_PASSWORD		40000010 	//"My Password", STR, 32, NOT_USED, NOT_USED},
#define M4_SUA				40000011 	//"Single User Account <0(No) | 1(Yes)>", SUBI, 2, 6, 1},
#define M4_IP_ADDR_ASSIGN		40000012 	//"IP Address Assignment<0(Static)|1(Dynamic)>", SUBI, 2, 0, 1},
#define M4_IP_ADDR			40000013 	//"IP Address", IP, 4, NOT_USED, NOT_USED},
#define M4_ENET_ENCAP_GATEWAY		40000014 	//"ENET ENCAP Gateway", IP, 4, NOT_USED, NOT_USED},
#define M4_REMOTE_IP_MASK		40000015
#define M4_IN_PRO_FILTER1		40000016		/* isp node input protocol filter apply */ 	
#define M4_IN_PRO_FILTER2		40000017	
#define M4_IN_PRO_FILTER3		40000018 	
#define M4_IN_PRO_FILTER4		40000019 	
#define M4_OUT_PRO_FILTER1		40000020 	/* isp node output protocol filter apply */
#define M4_OUT_PRO_FILTER2		40000021 	
#define M4_OUT_PRO_FILTER3		40000022 	
#define M4_OUT_PRO_FILTER4		40000023
#define M4_IDLE_TIMEOUT	40000024		/* idle timeout */
#define M4_ROUTE_IP			40000025
#define M4_BCP_BRIDGE		40000026
#define M4_QOS				40000027
#define M4_PCR				40000028
#define M4_SCR				40000029
#define M4_MBS				40000030
#define M4_RIP_DIRECTION	40000031
#define M4_RIP_VERSION		40000032
#define M4_NAILEDUP_CONNECTION 40000033
#define M4_MULTICAST			40000034


#define		M23_ROOTPASSWORD					230000000	/* "System Password", STR, 32, NOT_USED, NOT_USED}, */
#if 0
#define END_OF_TABLE					999999999
#endif
extern int restore_rom_t(mxml_node_t *tree);
#if 0
typedef enum {		/*	date type for field value  */
STR, IP, INT_SPT, UINT_SPT, INT_HEX, INT_INDEX, INT_HR2MIN,
PASSCODE, PWD, MAC, SUBI, STR_HEX, DATE, TIME, END
} fdtype;

typedef struct control_table_s {
	uint32	id;
	fdtype	data_type;
	uint8	data_length;
} control_table_t;
#endif

#endif



















