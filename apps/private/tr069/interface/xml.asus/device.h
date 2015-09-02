/*
 * Copyright(c) 2013, ASUSTeK Inc.
 * All rights reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTeK Inc.;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

#ifndef __DEVICE_H
#define __DEVICE_H

#include <tr_lib.h>
#include "sysdeps.h"
#ifdef ASUSWRT
#include <rtconfig.h>
#endif

struct handler {
	char *name;
	union {
		struct {
			int (*get)(node_t, char *, char **);
			int (*set)(node_t, char *, char *);
		};
		struct {
			int (*add)(node_t, char *, int);
			int (*del)(node_t, char *, int);
		};
	};
};

struct handler *get_handler(char *name);

#ifdef TR232
struct bulkdata_profile {
	char node_path[128];
	//unsigned int enable;
	//char alias[64];
	//char protocol[16];
	//char encoding_type[16];
	unsigned int reporting_interval;
	char time_reference[32];
	char file_transfer_URL[128];
	char file_transfer_username[64];
	char file_transfer_password[64];
	char control_file_format[128];
	int count;
};

void update_profile_sched(void);
static int set_bulkdata_enable(node_t node, char *arg, char *value);
static int get_bulkdata_status(node_t node, char *arg, char **value);
static int set_bulkdata_profile_enable(node_t node, char *arg, char *value);
static int add_bulkdata_profile(node_t node, char *arg, int nin);
static int del_bulkdata_profile(node_t node, char *arg, int nin);
static int add_profile_parameter(node_t node, char *arg, int nin);
static int del_profile_parameter(node_t node, char *arg, int nin);
#endif

int dev_init(char *arg);
int dev_notify_queued(void);
int dev_notify(char *services, char *arg, int queue);
void dev_reboot(void);

int save_old_xml( char *arg );
int update_new_xml();

int dev_firmware_upgrade(char *path);

int dev_settings_commit(void);
int dev_settings_reset(void);
int dev_settings_save(char *path);
int dev_settings_restore(char *path);

void record_vendor_config_info(char *url);
int delete_all_instance(char *del_path);

int forwarding_notification();
int update_forwarding();
int update_dhcp_static_addr();
int dhcpstatic_notification();

void prepare_wan_info(void);
char *get_eth_ifname_para(char *path, char *ifname);
char *get_diag_ifname_para(char *path, char *ifname);

int add_manageable_device(char *oui, char *serial, char *class);
int del_manageable_device(char *oui, char *serial, char *class);
int update_manageable_device();

#ifdef RTCONFIG_DSL
char *get_xdsl_prefix(char *path, char *prefix);
char *find_xdsl_unsed_index(char *path, char *prefix);
#endif

#ifdef TR098
#ifdef ASUSWRT
#define IGD_WANDEVICE_WAN_INST  1
#define IGD_WANDEVICE_LAN_INST  2
#define IGD_WANDEVICE_USB_INST  3
#else	/* DSL_ASUSWRT */
#define IGD_WANDEVICE_DSL_INST  1
#define IGD_WANDEVICE_WAN_INST  2
#define IGD_WANDEVICE_USB_INST  3
#define IGD_WANDEVICE_LAN_INST  4
#endif
char *eth_wanip_prefix_by_path(char *path, char *prefix);
int get_wd_inst_by_wan_unit(int wan_unit);
int get_inst_num_by_path(char *path);
int get_wan_unit_by_path(char *path);
char *eth_wlannum_prefix_by_path(char *path, char *prefix);
int update_wlan_device();
void update_device_summary();
void update_deviceInfo_description();
int update_wan_device();
int update_port_mapping();
char *convert_vif_to_sif(char *iface, char *buf);
int portmapping_notification();
int getWANDevInstNum(char *name);
int getWANConDevInstNum(char *name);
void add_portmapping_entry(char *full_path, int nin, char *path);
void del_portmapping_entry(char *full_path, int nin, char *path);
void reset_portmapping_count(char *path);
#ifdef RTCONFIG_XDSL
int check_dsl_wan_device_by_path(char *path);
int find_xdsl_wan_device(int wd_inst, char *index_name);
void xdsl_compare_portmapping_instance(int unit, int wd_inst, char *desc, 
	char *ext_port, char *in_client, char *in_port, char *proto);
char *xdsl_pick_portmapping_instance(int unit, int wd_inst, char *path);
void xdsl_compare_portmapping_nvram(int unit, int wd_inst, char *ext_port, 
	char *in_port, char *proto, char *in_client, char *desc, int index);
void xdsl_reset_portmapping_count(int unit, int wd_inst);
void add_del_xdsl_portmapping(int unit, int wd_inst, char *full_path, int nin, int flag);
void set_xdsl_portmapping(int unit, int wd_inst, int nin, char *field, char *value);
#endif
#endif

#ifdef TR181
#ifdef RTCONFIG_XDSL
#ifdef ASUSWRT
#ifdef RTCONFIG_DUALWAN
#define DEVICE_ETH_WANLAN_INST  1
#define DEVICE_ETH_LAN_INST     2
#ifdef RTCONFIG_USB
#define DEVICE_USB_INST 		3
#endif	/* RTCONFIG_USB */
#define DEVICE_DSL_INST         4
#else	/* #ifdef RTCONFIG_DUALWAN */
#define DEVICE_ETH_LAN_INST     1
#ifdef RTCONFIG_USB
#define DEVICE_USB_INST         2
#endif	/* #ifdef RTCONFIG_USB */
#define DEVICE_DSL_INST         3
#endif	/* RTCONFIG_DUALWAN */

#else	/* #ifdef ASUSWRT */
#define DEVICE_ETH_WAN_INST 	1
#define DEVICE_ETH_LAN_INST     2
#ifdef RTCONFIG_USB
#define DEVICE_USB_INST 		3
#endif	/* RTCONFIG_USB */
#define DEVICE_DSL_INST         4
#endif

#else	/* #ifdef RTCONFIG_XDSL */
#define DEVICE_ETH_WAN_INST     1
#ifdef RTCONFIG_DUALWAN
#define DEVICE_ETH_WANLAN_INST  2
#define DEVICE_ETH_LAN_INST     3
#ifdef RTCONFIG_USB
#define DEVICE_USB_INST         4
#endif	/* RTCONFIG_USB */
#else
#define DEVICE_ETH_LAN_INST     2
#ifdef RTCONFIG_USB
#define DEVICE_USB_INST         3
#endif	/* RTCONFIG_USB */
#endif	/* RTCONFIG_DUALWAN */
#endif	/* RTCONFIG_XDSL */

#define IP_FLAG               0
#define PPP_FLAG              1
#define START_INDEX_2G          1
#define START_INDEX_5G          5
#define NUM_MMSSID              3
int check_path_vaild(char *path);
int ethernet_unit_by_path(char *path);
int update_processor_info();
int update_wifi_device();
int update_wan_lan_if();
int update_wifi_radio_ssid_ap();
int update_bridging();
int update_ipv6rd();
int update_host();
char *dhcpv4_client_by_path(char *path, char *prefix);
int update_dhcpv4_client();
char *wifinum_prefix_by_path(char *path, char *prefix);
char *ethernet_prefix_by_path(char *path, char *prefix);
int portmapping_notification();
int update_port_mapping();
int add_host_client(char *ipaddr, char *hostname);
int del_host_client(char *ipaddr);
void update_gateway_info();
int search_ip_ppp_lowerlayer(char *path, int flag);
int search_bridging_lowerlayer(char *path);
#ifdef RTCONFIG_XDSL
//void update_xtm_link_num(char *path, char *field_name);
int find_xtm_link(int xdsl_id, char *index_name);
#endif
#endif

#ifdef RTCONFIG_SFEXPRESS
enum
{
	OVPNC_INDEX,
	OVPNC_DESCRIPTION,
	OVPNC_USERNAME,
	OVPNC_PASSWORD
};
int openvpn_client_file(const char *type, char *path);
int update_ovpnc();
int get_ovpnc_index_value(node_t node);
#endif	//RTCONFIG_SFEXPRESS

void reasign_instance_name(node_t node, int del_nin, int count_inc);
int get_wan_prefix_ifunit(node_t node);

#ifdef DSL_ASUSWRT
#define WANDUCK_NODE	"Wanduck_Common"
#define TR069_NODE		"TR069_Entry"
#define SYSINFO_NODE	"SysInfo_Entry"
#define DUALWAN_NODE	"Dualwan_Entry"
#define DHCPD_NODE		"Dhcpd"
#define STATIC_DHCP_NODE		"Dhcpd_Entry"
#define LAN_NODE		"Lan_Entry0"
#define DHCPD_COMMON_NODE		"Dhcpd_Common"
#define TIMEZONE_NODE	"Timezone_Entry"
#define WLAN_NODE		"WLan_Entry0"
#define WLAN_COMMON_NODE		"WLan_Common"
#define ACL_NODE		"ACL_Entry0"
#define WAN_COMMON_NODE "Wan_Common"
#define INFO_ADSL_NODE	"Info_Adsl"
#define LAN_DHCP_NODE	"Lan_Dhcp"
#define DPROXY_NODE		"Dproxy_Entry"
#define ROUTE_NODE		"Route_Entry0"
#define VIRSERVER_NODE	"VirServer_PVC0_Entry0"
#define USBMODEM_NODE	"USBModem_Entry"
#define FIREWALL_NODE	"Firewall_Entry"
#define HTTPS_NODE		"Https_Entry"
#define ACCOUNT_NODE	"Account_Entry0"
#define UPNPD_NODE		"Upnpd_Entry"
#define IPV6RD_NODE		"ipv6rd_Entry"
#define CWMPDSLDIAGNOSTIC	"CDSLDiagnostic_Common"

/* Encap */
#define PPPOE_LLC	"PPPoE LLC"
#define PPPOE_VCMUX	"PPPoE VC-Mux"
#define PPPOA_LLC	"PPPoA LLC"
#define PPPOA_VCMUX	"PPPoA VC-Mux"
#define ROUTED_IP_LLC	"1483 Routed IP LLC(IPoA)"
#define ROUTED_IP_VCMUX	"1483 Routed IP VC-Mux"
#define BRIDGED_IP_LLC	"1483 Bridged IP LLC"
#define BRIDGED_IP_VCMUX	"1483 Bridged IP VC-Mux"
#define BRIDGED_ONLY_LLC	"1483 Bridged Only LLC"
#define BRIDGED_ONLY_VCMUX	"1483 Bridged Only VC-Mux"

#define MAX_STATIC_NUM	32
#define NUM_MSSID_SUPPORT	3

#define WAN_XTM_PREFIX	"Wan_PVC"

#ifdef TCSUPPORT_WAN_ATM
#define WAN_ATM_INDEX 	0
#endif

#ifdef TCSUPPORT_WAN_PTM
#define WAN_PTM_INDEX	8
#define WAN_PTM_EXT_PREFIX	"WanExt_PVC8e"
#endif

#ifdef TCSUPPORT_WAN_ETHER
#define WAN_ETHER_INDEX	10
#endif

#ifdef RTCONFIG_USB_MODEM
#define WAN_USB_INDEX	11
#endif

char *get_wifi_ifname(char *prefix, char *ifname);
char* convert_prefix_to_pvc(char *prefix, char *pvc);
#endif

#endif /*__DEVICE_H*/
