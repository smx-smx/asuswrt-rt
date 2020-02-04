/*
 * Router rc control script
 *
 * Copyright 2005, Broadcom Corporation
 * All Rights Reserved.
 *
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: rc.h,v 1.39 2005/03/29 02:00:06 honor Exp $
 */

#ifndef __RC_H__
#define __RC_H__

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h> // !!TB
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <net/if.h>

#include <bcmnvram.h>
#include "libtcapi.h"
#include "tcapi.h"
#include "tcutils.h"
#include <utils.h>
#include <shutils.h>
#include <shared.h>
#include <linux/version.h>

#ifdef RTCONFIG_OPENVPN
#include "openvpn_config.h"
#include "openvpn_control.h"
#endif

#define USBCORE_MOD	"usbcore"
#if defined (RTCONFIG_USB_XHCI)
#define USB30_MOD	"xhci-hcd"
#endif
#define USB20_MOD	"ehci-hcd"
#define USBSTORAGE_MOD	"usb-storage"
#define SCSI_MOD	"scsi_mod"
#define SD_MOD		"sd_mod"
#define SG_MOD		"sg"
#ifdef LINUX26
#define USBOHCI_MOD	"ohci-hcd"
#define USBUHCI_MOD	"uhci-hcd"
#define USBPRINTER_MOD	"usblp"
#define SCSI_WAIT_MOD	"scsi_wait_scan"
#define USBFS		"usbfs"
#else
#define USBOHCI_MOD	"usb-ohci"
#define USBUHCI_MOD	"usb-uhci"
#define USBPRINTER_MOD	"printer"
#define USBFS		"usbdevfs"
#endif

#ifdef RTCONFIG_IPV6
extern char wan6face[];
#endif

#define LOGNAME get_productid()
#define is_ap_mode() (nvram_match("sw_mode", "3"))
#ifdef RTCONFIG_USB_MODEM
#define is_phyconnected() (nvram_match("link_wan", "1") || nvram_match("link_wan1", "1"))
#else
#define is_phyconnected() (nvram_match("link_wan","1"))
#endif

#define NAT_RULES	"/tmp/nat_rules"
#ifdef RTCONFIG_OLD_PARENTALCTRL
int nvram_set_by_seq(char *name, unsigned int seq, char *value);
char * nvram_get_by_seq(char *name, unsigned int seq);
int parental_ctrl(void);
#endif	/* RTCONFIG_OLD_PARENTALCTRL */

//	#define DEBUG_IPTFILE
//	#define DEBUG_RCTEST
//	#define DEBUG_NOISY

#ifdef DEBUG_NOISY
#define TRACE_PT(fmt, args...)		\
do {					\
	char psn[16];			\
	pid_t pid;			\
	pid = getpid();			\
	psname(pid, psn, sizeof(psn));	\
	_dprintf("[%d %s:%s +%ld] " fmt,\
		pid, psn, __func__, uptime(), ##args);	\
} while(0)
#else
#define TRACE_PT(args...) do { } while(0)
#endif

#define MAX_NO_BRIDGE 	2
#define MAX_NO_MSSID	4
#define PROC_SCSI_ROOT	"/proc/scsi"
#define USB_STORAGE	"usb-storage"

#ifndef MAX_WAIT_FILE
#define MAX_WAIT_FILE 5
#endif
#define PPP_DIR "/etc/ppp/peers"
#define PPP_CONF_FOR_3G "/etc/ppp/peers/3g"
#define PPP_IF	"ppp%d"

#ifdef RTCONFIG_USB_BECEEM
#define BECEEM_DIR "/tmp/Beceem_firmware"
#define WIMAX_CONF "/tmp/wimax.conf"
#define WIMAX_LOG "/tmp/wimax.log"
#endif

#ifndef FALSE
#define FALSE   0
#endif
#ifndef TRUE
#define TRUE    1
#endif

#define NOTIFY_DIR "/tmp/notify"
#define NOTIFY_TYPE_USB "usb"

#define BOOT		0
#define REDIAL		1
#define CONNECTING	2

#define PPPOE0		0
#define PPPOE1		1

#define GOT_IP			0x01
#define RELEASE_IP		0x02
#define	GET_IP_ERROR		0x03
#define RELEASE_WAN_CONTROL	0x04
#define USB_DATA_ACCESS		0x05	//For WRTSL54GS
#define USB_CONNECT		0x06	//For WRTSL54GS
#define USB_DISCONNECT		0x07	//For WRTSL54GS

#define SERIAL_NUMBER_LENGTH	12	//ATE need

#define SET_LED(val)	do { } while(0)

typedef enum { IPT_TABLE_NAT, IPT_TABLE_FILTER, IPT_TABLE_MANGLE } ipt_table_t;

#define IFUP (IFF_UP | IFF_RUNNING | IFF_BROADCAST | IFF_MULTICAST)

#define	IPT_V4	0x01
#define	IPT_V6	0x02
#define	IPT_ANY_AF	(IPT_V4 | IPT_V6)
#define	IPT_AF_IS_EMPTY(f)	((f & IPT_ANY_AF) == 0)

// init.c
extern int init_main(int argc, char *argv[]);
extern int reboothalt_main(int argc, char *argv[]);
extern int console_main(int argc, char *argv[]);
extern void clean_modem_state(int flag);
extern void clean_wanduck_state(int unit);

// interface.c
extern int _ifconfig(const char *name, int flags, const char *addr, const char *netmask, const char *dstaddr);
#define ifconfig(name, flags, addr, netmask) _ifconfig(name, flags, addr, netmask, NULL)
extern int route_add(char *name, int metric, char *dst, char *gateway, char *genmask);
extern int route_del(char *name, int metric, char *dst, char *gateway, char *genmask);
extern int start_vlan(void);
extern int stop_vlan(void);
extern int config_vlan(void);
extern void config_loopback(void);
#ifdef RTCONFIG_IPV6
extern int ipv6_mapaddr4(struct in6_addr *addr6, int ip6len, struct in_addr *addr4, int ip4mask);
#endif

// wan.c
extern int wan_prefix(char *ifname, char *prefix);
extern int wan_ifunit(char *wan_ifname);
extern int wanx_ifunit(char *wan_ifname);
extern int preset_wan_routes(char *wan_ifname);
extern int found_default_route(int wan_unit);
extern int find_default_route_ifname(char *ifname, size_t size);
extern int autodet_main(int argc, char *argv[]);
extern int add_multi_routes(void);
extern int do_dns_detect();
#ifdef RTCONFIG_USB_MODEM
void start_wan_if(int unit);
void stop_wan_if(int unit);
#endif

// lan.c
extern void hotplug_net(void);

// firewall.c
extern int start_firewall(int wanunit, int lanunit);
extern void enable_ip_forward(void);

// ppp.c
extern int ipup_main(int argc, char **argv);
extern int ipdown_main(int argc, char **argv);
#ifdef RTCONFIG_IPV6
extern int ip6up_main(int argc, char **argv);
extern int ip6down_main(int argc, char **argv);
#endif
extern int authfail_main(int argc, char **argv);
extern int ppp_ifunit(char *ifname);
extern int ppp_linkunit(char *linkname);
extern void USB_restart_dnsmasq(void);

// pppd.c
extern int start_pppd(int unit);
extern void stop_pppd(int unit);
extern int start_demand_ppp(int unit, int wait);
extern int start_pppoe_relay(char *wan_if);
extern void stop_pppoe_relay(void);

// network.c
extern void set_host_domain_name(void);
extern void start_lan(void);
extern void stop_lan(void);
extern void do_static_routes(int add);
extern void start_wl(void);
extern void wan_up(char *wan_ifname);
#ifdef RTCONFIG_IPV6
extern void wan6_up(const char *wan_ifname);
#endif
extern void wan_down(char *wan_ifname);
extern void update_wan_state(char *prefix, int state, int reason);
#ifdef OVERWRITE_DNS
extern int update_resolvconf();
#endif

// rtstate.c
extern void add_rc_support(char *feature);

// udhcpc.c
extern int udhcpc_wan(int argc, char **argv);
extern int udhcpc_lan(int argc, char **argv);
extern int udhcpc_expires(int argc, char **argv);
extern int udhcpc_bound(int argc, char **argv);
extern int start_udhcpc(char *wan_ifname, int unit, pid_t *ppid);
extern int zcip_wan(int argc, char **argv);
extern int start_zcip(char *wan_ifname);

#ifdef RTCONFIG_IPV6
extern int dhcp6c_state_main(int argc, char **argv);
extern void start_dhcp6c(void);
extern void stop_dhcp6c(void);
extern int ipv6aide_main(int argc, char *argv[]);
#endif

// auth.c
extern int start_auth(int unit, int wan_up);
extern int stop_auth(int unit, int wan_down);
extern int restart_auth(int unit);
#ifdef RTCONFIG_EAPOL
extern int wpacli_main(int argc, char *argv[]);
#endif

// mtd.c
extern int mtd_erase(const char *mtdname);
extern int mtd_unlock(const char *mtdname);
extern int mtd_write_main(int argc, char *argv[]);
extern int mtd_unlock_erase_main(int argc, char *argv[]);

// watchdog.c
extern int watchdog_main(int argc, char *argv[]);

// wpsfix.c
#ifdef RTCONFIG_RALINK
extern int wpsfix_main(int argc, char *argv[]);
#endif

// usbled.c
extern int usbled_main(int argc, char *argv[]);
#ifdef RTCONFIG_FANCTRL
// phy_tempsense.c
extern int phy_tempsense_main(int argc, char *argv[]);
#endif
#ifdef RTCONFIG_BCMWL6
#ifdef RTCONFIG_PROXYSTA
// psta_monitor.c
extern int psta_monitor_main(int argc, char *argv[]);
#endif
#endif
extern int radio_main(int argc, char *argv[]);

// ntp.c
extern int ntp_main(int argc, char *argv[]);

// btnsetup.c
extern int ots_main(int argc, char *argv[]);

// common.c
extern void usage_exit(const char *cmd, const char *help) __attribute__ ((noreturn));
#define modprobe(mod, args...) ({ char *argv[] = { "/sbin/modprobe", mod, ## args, NULL }; _eval(argv, NULL, 0, NULL); })
extern int insertmod(char *mod_full_path);
extern int removemod(char *mod_name);
extern int modprobe_r(const char *mod);
#define xstart(args...)	_xstart(args, NULL)
extern int _xstart(const char *cmd, ...);
extern void run_nvscript(const char *nv, const char *arg1, int wtime);
extern void run_userfile (char *folder, char *extension, const char *arg1, int wtime);
extern void setup_conntrack(void);
extern void inc_mac(char *mac, int plus);
extern void set_mac(const char *ifname, const char *nvname, int plus);
extern const char *default_wanif(void);
//	extern const char *default_wlif(void);
extern void simple_unlock(const char *name);
extern void simple_lock(const char *name);
extern void killall_tk(const char *name);
extern void kill_pidfile_tk(const char *pidfile);
extern long fappend(FILE *out, const char *fname);
extern long fappend_file(const char *path, const char *fname);
extern void logmessage(char *logheader, char *fmt, ...);
extern char *trim_r(char *str);
extern void restart_lfp(void);
extern int get_meminfo_item(const char *name);
extern void setup_timezone(void);
extern int is_valid_hostname(const char *name);
extern void setup_ct_timeout(int connflag);
extern void setup_udp_timeout(int connflag);
extern void setup_ftp_conntrack(int port);
extern void setup_pt_conntrack(void);
extern int pppstatus(void);
extern void time_zone_x_mapping(void);

// ssh.c
extern void start_sshd(void);
extern void stop_sshd(void);

// usb.c
#ifdef RTCONFIG_USB
FILE* fopen_or_warn(const char *path, const char *mode);
extern void hotplug_usb(void);
extern void start_usb(void);
extern void remove_usb_module(void);
extern void stop_usb(void);
extern void start_lpd();
extern void stop_lpd();
extern void start_u2ec();
extern void stop_u2ec();
extern int ejusb_main(int argc, char *argv[]);
extern void webdav_account_default(void);
extern void remove_storage_main(int shutdn);
extern int start_usbled(void);
extern int stop_usbled(void);
extern void restart_nas_services(int stop, int start);
extern void stop_nas_services(int force);
#endif
#ifdef RTCONFIG_CROND
extern void start_cron(void);
extern void stop_cron(void);
#endif
extern void start_webdav(void);
#ifdef RTCONFIG_USB_PRINTER
extern void start_usblpsrv(void);
#endif
#ifdef RTCONFIG_SAMBASRV
extern void create_custom_passwd(void);
extern void stop_samba(void);
extern void start_samba(void);
#endif
#ifdef RTCONFIG_WEBDAV
extern void stop_webdav(void);
extern void stop_all_webdav(void);
#else
static inline void stop_all_webdav(void) { }
#endif
#ifdef RTCONFIG_FTP
extern void stop_ftpd(void);
extern void start_ftpd(void);
#endif
#ifdef RTCONFIG_CLOUDSYNC
extern void stop_cloudsync(void);
extern void start_cloudsync(void);
#endif
#if defined(RTCONFIG_APP_PREINSTALLED) || defined(RTCONFIG_APP_NETINSTALLED)
extern int stop_app(void);
extern int start_app(void);
extern void usb_notify();
#endif

#ifdef ASUS_DISK_UTILITY
extern void start_diskmon(void);
extern void stop_diskmon(void);
extern int diskmon_main(int argc, char *argv[]);
extern void remove_pool_error(const char *device, const char *flag);
#endif
extern int diskremove_main(int argc, char *argv[]);

#ifdef RTCONFIG_CIFS
extern void start_cifs(void);
extern void stop_cifs(void);
extern int mount_cifs_main(int argc, char *argv[]);
#else
static inline void start_cifs(void) {};
static inline void stop_cifs(void) {};
#endif
#ifdef TCSUPPORT_SWAP_FILE
extern void start_swapfile(char *mountpoint);
extern void stop_swapfile(char *mountpoint);
#endif

// linkmonitor.c
extern int linkmonitor_main(int argc, char *argv[]);

// vpn.c
extern void start_pptpd(void);
extern void stop_pptpd(void);

// vpnc.c
#ifdef RTCONFIG_VPNC
extern int start_vpnc(void);
extern void stop_vpnc(void);
extern int vpnc_ipup_main(int argc, char **argv);
extern int vpnc_ipdown_main(int argc, char **argv);
extern int vpnc_ippreup_main(int argc, char **argv);
extern int vpnc_authfail_main(int argc, char **argv);
extern void update_vpnc_state(int state, int reason);
#ifdef TCSUPPORT_IPV6
extern int vpnc_ip6up_main(int argc, char **argv);
extern int vpnc_ip6down_main(int argc, char **argv);
#endif
#endif

// ovpn.c
extern int ovpn_up_main(int argc, char **argv);
extern int ovpn_down_main(int argc, char **argv);
extern int ovpn_route_up_main(int argc, char **argv);

// tr069.c
#ifdef RTCONFIG_TR069
extern int start_tr(void);
extern void stop_tr(void);
#endif

// wanduck.c
extern int wanduck_main(int argc, char *argv[]);

// tcpcheck.c
extern int setupsocket(int sock);
extern void wakeme(int sig);
extern void goodconnect(int i);
extern void failedconnect(int i);
extern void subtime(struct timeval *a, struct timeval *b, struct timeval *res);
extern void setupset(fd_set *theset, int *numfds);
extern void waitforconnects();
extern int tcpcheck_main(int argc, char *argv[]);

// usb_devices.c
#ifdef RTCONFIG_USB
extern int asus_sd(const char *device_name, const char *action);
extern int asus_lp(const char *device_name, const char *action);
extern int asus_sr(const char *device_name, const char *action);
extern int asus_tty(const char *device_name, const char *action);
extern int asus_usb_interface(const char *device_name, const char *action);
extern int asus_sg(const char *device_name, const char *action);
extern int asus_usbbcm(const char *device_name, const char *action);
#endif
#ifdef RTCONFIG_USB_MODEM
extern int is_create_file_dongle(const unsigned int vid, const unsigned int pid);
#ifdef RTCONFIG_USB_BECEEM
extern int is_beceem_dongle(const int mode, const unsigned int vid, const unsigned int pid);
extern int is_samsung_dongle(const int mode, const unsigned int vid, const unsigned int pid);
extern int is_gct_dongle(const int mode, const unsigned int vid, const unsigned int pid);
extern int write_beceem_conf(const char *eth_node);
extern int write_gct_conf(void);
#endif
extern int is_android_phone(const int mode, const unsigned int vid, const unsigned int pid);
extern int is_storage_cd(const int mode, const unsigned int vid, const unsigned int pid);
extern int write_3g_conf(FILE *fp, int dno, int aut, const unsigned int vid, const unsigned int pid);
extern int init_3g_param(const char *port_path, const unsigned int vid, const unsigned int pid);
extern int write_3g_ppp_conf(void);
#endif

//services.c
extern void write_static_leases(char *file);
extern void start_dnsmasq(void);
extern void stop_dnsmasq(void);
extern int ddns_updated_main(int argc, char *argv[]);
#ifdef RTCONFIG_IPV6
extern void start_ipv6_tunnel(void);
extern void stop_ipv6_tunnel(void);
extern void start_radvd(void);
extern void stop_radvd(void);
extern void start_dhcp6s(void);
extern void stop_dhcp6s(void);
extern void start_ipv6(void);
extern void stop_ipv6(void);
#endif

extern void start_nat_rules(void);
extern void stop_nat_rules(void);
extern void stop_syslogd(void);
extern void stop_klogd(void);
extern int start_syslogd(void);
extern int start_klogd(void);
extern int start_logger(void);
extern void handle_notifications(char* rc_service);
extern int stop_watchdog(void);
extern int start_watchdog(void);
extern int get_apps_name(const char *string);
extern int run_app_script(const char *pkg_name, const char *pkg_action);
extern int start_telnetd(void);
extern void stop_telnetd(void);
extern int run_telnetd(void);
extern void start_hotplug2(void);
extern void stop_services(void);
extern void stop_logger(void);
extern void setup_passwd(void);
extern void create_passwd(void);
extern int start_services(void);
extern void check_services(void);
extern int no_need_to_start_wps(void);
extern int start_wanduck(void);
extern void stop_wanduck(void);
extern void stop_ntpc(void);
extern int start_ntpc(void);
extern void stop_networkmap(void);
extern int start_networkmap(void);
extern int stop_wps(void);
extern int start_wps(void);
extern void stop_upnp(void);
extern void start_upnp(void);
extern void stop_ddns(void);
extern int start_ddns(void);
extern void refresh_ntpc(void);
extern void start_hotplug2(void);
extern void stop_hotplug2(void);
extern int stop_lltd(void);
extern void stop_rstats(void);
extern void stop_autodet(void);
extern void start_autodet(void);
extern void start_httpd(void);
extern int wl_wpsPincheck(char *pin_string);
extern int start_wps_pbc(int unit);
extern int service_main(int argc, char *argv[]);

#ifdef BTN_SETUP
enum BTNSETUP_STATE
{
	BTNSETUP_NONE=0,
	BTNSETUP_DETECT,
	BTNSETUP_START,
	BTNSETUP_DATAEXCHANGE,
	BTNSETUP_DATAEXCHANGE_FINISH,
	BTNSETUP_DATAEXCHANGE_EXTEND,
	BTNSETUP_FINISH
};
#endif

#ifdef RTCONFIG_LETSENCRYPT
// letsencrypt.c
extern int start_letsencrypt(void);
extern int stop_letsencrypt(void);
extern int le_acme_main(int argc, char **argv);
extern int copy_le_certificate(char *dst_cert, char *dst_key);
extern int is_correct_le_certificate(char *cert_path);
#define HTTPD_CERT	"/etc/cert.pem"
#define HTTPD_KEY	"/etc/key.pem"
#endif

#endif	/* __RC_H__ */
