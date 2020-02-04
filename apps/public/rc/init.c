#include "rc.h"
#include <sys/types.h>
#include <linux/fs.h>
#include "shutils.h"
#include <libutils.h>
#ifdef RTCONFIG_USB
#include "usb_info.h"
#endif
#include "tcutils.h"

#define SYSINFO		"SysInfo_Entry"
#define APPS_DATA	"Apps_Entry"

#ifdef RTCONFIG_USB_MODEM
void clean_modem_state(int flag){
	if(flag){
		nvram_unset("usb_modem_act_path");
		nvram_unset("usb_modem_act_type");
		nvram_unset("usb_modem_act_dev");
	}

	// modem.
	nvram_unset("usb_modem_act_int");
	nvram_unset("usb_modem_act_bulk");
	nvram_unset("usb_modem_act_vid");
	nvram_unset("usb_modem_act_pid");
	nvram_unset("usb_modem_act_sim");
	nvram_unset("usb_modem_act_signal");
	nvram_unset("usb_modem_act_operation");
	nvram_unset("usb_modem_act_imsi");
	nvram_unset("usb_modem_act_imei");
	nvram_unset("usb_modem_act_iccid");
	nvram_unset("usb_modem_act_tx");
	nvram_unset("usb_modem_act_rx");
	nvram_unset("usb_modem_act_hwver");
	nvram_unset("usb_modem_act_band");
	nvram_unset("usb_modem_act_scanning");
	nvram_unset("usb_modem_act_auth");
	nvram_unset("usb_modem_act_auth_pin");
	nvram_unset("usb_modem_act_auth_puk");

	// auto APN
	nvram_unset("usb_modem_auto_running");
	nvram_unset("usb_modem_auto_country");
	nvram_unset("usb_modem_auto_isp");
	nvram_unset("usb_modem_auto_apn");
	nvram_unset("usb_modem_auto_dialnum");
	nvram_unset("usb_modem_auto_user");
	nvram_unset("usb_modem_auto_pass");

	// modem state.
	nvram_unset("g3state_pin");
	nvram_unset("g3state_z");
	nvram_unset("g3state_q0");
	nvram_unset("g3state_cd");
	nvram_unset("g3state_class");
	nvram_unset("g3state_mode");
	nvram_unset("g3state_apn");
	nvram_unset("g3state_dial");
	nvram_unset("g3state_conn");

	// modem error.
	nvram_unset("g3err_pin");
	nvram_unset("g3err_apn");
	nvram_unset("g3err_conn");
	nvram_unset("g3err_imsi");
}
#endif

void clean_wanduck_state(int unit)
{
	char prefix[] = "wanXXXXXXXXXX_";
	char attr[128] = {0};

	snprintf(prefix, sizeof(prefix), "wan%d", unit);
	cprintf("[%s]Begin...prefix=[%s]\n", __FUNCTION__, prefix);

	snprintf(attr, sizeof(attr), "%s_dhcpenable_x", prefix);
	nvram_unset(attr);
	snprintf(attr, sizeof(attr), "%s_dnsenable_x", prefix);
	nvram_unset(attr);
	snprintf(attr, sizeof(attr), "%s_netmask", prefix);
	nvram_unset(attr);
	snprintf(attr, sizeof(attr), "%s_ipaddr", prefix);
	nvram_unset(attr);
	snprintf(attr, sizeof(attr), "%s_lease", prefix);
	nvram_unset(attr);
	snprintf(attr, sizeof(attr), "%s_ipaddr", prefix);
	nvram_unset(attr);
	snprintf(attr, sizeof(attr), "%s_expires", prefix);
	nvram_unset(attr);
	snprintf(attr, sizeof(attr), "%s_gateway", prefix);
	nvram_unset(attr);
	snprintf(attr, sizeof(attr), "%s_dns", prefix);
	nvram_unset(attr);
	snprintf(attr, sizeof(attr), "%s_pppoe_ifname", prefix);
	nvram_unset(attr);

	cprintf("[%s]End...\n", __FUNCTION__);
}

static int SupportCountrySelect()
{
	char ProductName[32];
	char TerritoryCode[8];

	bzero(ProductName, sizeof(ProductName));
	bzero(TerritoryCode, sizeof(TerritoryCode));

	tcapi_get("SysInfo_Entry", "ProductName", ProductName);
	tcapi_get("SysInfo_Entry", "TerritoryCode", TerritoryCode);

	return check_SupportCountrySelect(ProductName, TerritoryCode);
}

int init_nvram(void)
{
	int model = get_model();
#ifdef RTCONFIG_OPENVPN
	char node[MAXLEN_NODE_NAME] = {0};
	int i;
#endif

	tcapi_set(SYSINFO, "rc_support", "");

	/* Paul modify 2013/6/5 */
	switch (model) {
	case MODEL_DSLN16U:
		nvram_set("ehci_ports", "1-1 1-2");
		nvram_set("ohci_ports", "2-1 2-2");
		add_rc_support("2.4G update usbX2 modem rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid no5gmssid appbase pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN55UC1:
	case MODEL_DSLN55UD1:
		nvram_set("ehci_ports", "1-1 1-2");
		nvram_set("ohci_ports", "2-1 2-2");
		add_rc_support("2.4G 5G update usbX2 modem rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid appbase pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN66U:
		nvram_set("ehci_ports", "1-2 1-1");
		nvram_set("ohci_ports", "2-2 2-1");
		add_rc_support("2.4G 5G update usbX2 modem rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid appnet pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN10C1:
	case MODEL_DSLN10PC1:
	case MODEL_DSLN12EC1:
	case MODEL_DSLN10D1:
		add_rc_support("2.4G rawifi dsl wifi_hw_sw ipv6 PARENTAL2 mssid no5gmssid pwrctrl iptv wds");
		break;
	case MODEL_DSLN12UC1:
	case MODEL_DSLN12UD1:
	case MODEL_DSLN14U:
	case MODEL_DSLN14UB1:
		nvram_set("ehci_ports", "1-1");
		nvram_set("ohci_ports", "2-1");
		add_rc_support("2.4G update usbX1 modem rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid no5gmssid appnet pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLAC56U:
		nvram_set("ehci_ports", "1-1 1-2");
		nvram_set("ohci_ports", "2-1 2-2");
		add_rc_support("2.4G 5G 11AC update usbX2 rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid appbase pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN17U:
		nvram_set("ehci_ports", "1-1 1-2");
		nvram_set("ohci_ports", "2-1 2-2");
		add_rc_support("2.4G update usbX2 rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid appbase pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLAC52U:
	case MODEL_DSLAC55U:
		nvram_set("ehci_ports", "1-1");
		nvram_set("ohci_ports", "2-1");
		add_rc_support("2.4G 5G 11AC update usbX1 rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid appbase pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN16:
		add_rc_support("2.4G update rawifi dsl wifi_hw_sw ipv6 PARENTAL2 mssid pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLAC51:
		add_rc_support("2.4G 5G 11AC update rawifi dsl wifi_hw_sw ipv6 PARENTAL2 mssid pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN16P:
		nvram_set("ehci_ports", "1-1");
		nvram_set("ohci_ports", "2-1");
		add_rc_support("2.4G update usbX1 modem rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid appnet pwrctrl iptv wds HTTPS");
		break;
	}

	add_rc_support("switchctrl"); //LAN -> Switch Control
	add_rc_support("feedback");
	add_rc_support("telnet");
	add_rc_support("WPS_reset");
#ifdef TCSUPPORT_SSH
	// Carlos Add SSH 2015/07/23
	add_rc_support("ssh");
#endif

#ifdef RTCONFIG_USER_LOW_RSSI
	add_rc_support("user_low_rssi");
#endif

#ifdef RTCONFIG_DUALWAN
	add_rc_support("dualwan");
#endif
#if defined(RTCONFIG_PPTPD) || defined(RTCONFIG_ACCEL_PPTPD)
	add_rc_support("pptpd");
#endif
#ifdef RTCONFIG_OPENVPN
	add_rc_support("openvpnd");
#endif
#if defined(RTCONFIG_VPNC)
	add_rc_support("vpnc");
#endif
#ifdef TCSUPPORT_VPN
	add_rc_support("tcipsec");
#endif
#ifdef RTCONFIG_SPECTRUM
	add_rc_support("spectrum");
#endif /* RTCONFIG_SPECTRUM */

#ifdef ASUSSUPPORT_EMAIL
	add_rc_support("email");
#endif

#ifdef CWMP
	add_rc_support("cwmp");
#endif

#ifdef TCSUPPORT_USB_3G_DONGLE
	add_rc_support("modem");
#endif

#ifdef RTCONFIG_TR069
	add_rc_support("tr069");
#endif

//Carlos add cloudsync config trigger GUI display support items.
//2015/09/17
#ifdef RTCONFIG_CLOUDSYNC
	tcapi_set(CLOUD_DATA, "ss_support", "");
#ifdef RTCONFIG_WEBSTORAGE
	add_ss_support("asuswebstorage");
#endif
#ifdef RTCONFIG_WEBDAV
	add_ss_support("webdav");
#endif
#ifdef RTCONFIG_DROPBOX_CLIENT
	add_ss_support("dropbox");
#endif
#ifdef RTCONFIG_FTP_CLIENT
	add_ss_support("ftp");
#endif
#ifdef RTCONFIG_SAMBA_CLIENT
	add_ss_support("samba");
#endif
#ifdef RTCONFIG_USB_CLIENT
	add_ss_support("usb");
#endif
#endif
#ifdef TCSUPPORT_DSL_LINE_DIAGNOSTIC
	add_rc_support("dsl_diag");
#endif
#ifdef TCSUPPORT_WEBMON
	add_rc_support("ipt_webmon");
#endif
#ifdef TCSUPPORT_ACCESSLOG
	add_rc_support("access_log");
#endif
#ifdef MT7530_SUPPORT //Currently only MT7530 (DSL-AC56U/DSL-AC52U/DSL-AC55U/DSL-N17U) can support jumbo frame
	add_rc_support("jumbo_frame");
#endif
#ifdef RTCONFIG_BWDPI
	add_rc_support("bwdpi");
#endif

	if (SupportCountrySelect())
		add_rc_support("loclist");

	tcapi_set(APPS_DATA, "apps_ipkg_server", "http://nw-dlcdnet.asus.com/asusware/mipsbig/stable");
	tcapi_set(APPS_DATA, "apps_dev", "");
	tcapi_set(APPS_DATA, "apps_mounted_path", "");

#ifdef RTCONFIG_DUALWAN
	#if defined(TCSUPPORT_WAN_ATM) || defined(TCSUPPORT_WAN_PTM)
		set_wanscap_support("dsl");
	#endif /* defined(TCSUPPORT_WAN_ATM) || defined(TCSUPPORT_WAN_PTM) */
	#ifdef TCSUPPORT_WAN_ETHER
		add_wanscap_support("wan");
	#endif /* TCSUPPORT_WAN_ETHER */
	#ifdef TCSUPPORT_WAN_ETHER_LAN
		add_wanscap_support("lan");
	#endif
	#ifdef TCSUPPORT_USB_3G_DONGLE
		add_wanscap_support("usb");
	#endif /* TCSUPPORT_USB_3G_DONGLE */
#endif /* RTCONFIG_DUALWAN */

#ifdef RTCONFIG_OPENVPN
	//clear state, could be moved to temp node
	for (i = 1; i <= OVPN_SERVER_MAX; i++) {
		sprintf(node, "OpenVPN_Entry%d", OVPN_SERVER_BASE + i);
		tcapi_set(node, "state", "0");
		tcapi_set(node, "errno", "0");
	}
	for (i = 1; i <= OVPN_CLIENT_MAX; i++) {
		sprintf(node, "OpenVPN_Entry%d", OVPN_CLIENT_BASE + i);
		tcapi_set(node, "state", "0");
		tcapi_set(node, "errno", "0");
	}
	tcapi_set("OpenVPN_Common", "vpn_upload_state", "");
#endif
#ifdef RTCONFIG_LETSENCRYPT
	tcapi_set("Vram_Entry", "le_state", "0");
#endif
	tcapi_save();	//Andy Chiu, 2015/07/14
	return 0;
}

static void tweak_kernel_platdep(void)
{
	int model = get_model();
	int min_free_kbytes = 0;
	int skbmgr_driver_max_skb = 0;	//sk buffer for whole system
	int skbmgr_limit = 0;		// sk buffer for DSL/Switch driver
	int skbmgr_4k_limit = 0;	// sk buffer for WiFi driver limit
	char strtmp[16] = {0};

	switch (model) {
	case MODEL_DSLN16U:	//128M 2.4G WiFi, USB
		min_free_kbytes = 4096;
		skbmgr_driver_max_skb = 6144;
		skbmgr_limit = 4096;	
		skbmgr_4k_limit = 1024;
		break;
	case MODEL_DSLN55UC1:	//128M 2.4G  + 5G WiFi, USB
	case MODEL_DSLN55UD1:
		min_free_kbytes = 4096;
		skbmgr_driver_max_skb = 6144;
		skbmgr_limit = 4096;	
		skbmgr_4k_limit = 1536;
		break;
	case MODEL_DSLN66U:	//64M USB, 64M on  2.4G  + 5G WiFi
		min_free_kbytes = 4096;
		skbmgr_driver_max_skb = 3072;
		skbmgr_limit = 2048;	
		skbmgr_4k_limit = 256;
		break;
	case MODEL_DSLN10C1:	//32M 2.4G WiFi, No USB
	case MODEL_DSLN10PC1:
	case MODEL_DSLN12EC1:
	case MODEL_DSLN10D1:
		skbmgr_driver_max_skb = 2340;
		skbmgr_limit = 1536;	
		skbmgr_4k_limit = 512;
		break;
	case MODEL_DSLN12UC1:	//64M 2.4G WiFi, USB
	case MODEL_DSLN12UD1:
	case MODEL_DSLN14U:
	case MODEL_DSLN14UB1:
		min_free_kbytes = 4096;
		skbmgr_driver_max_skb = 4096;
		skbmgr_limit = 3072;	
		skbmgr_4k_limit = 512;
		break;
	case MODEL_DSLAC56U:	//256M 2.4G  + 5G AC WiFi, USB
		min_free_kbytes = 14336;
		skbmgr_driver_max_skb = 10240;
		skbmgr_limit = 6144;	
		skbmgr_4k_limit = 2048;
		break;
	case MODEL_DSLN17U:	//128M 2.4G, USB
		min_free_kbytes = 7168;
		skbmgr_driver_max_skb = 6144;
		skbmgr_limit = 4096;	
		skbmgr_4k_limit = 1024;
		break;
	case MODEL_DSLAC52U:	//128M 2.4G  + 5G AC WiFi, USB
	case MODEL_DSLAC55U:
		min_free_kbytes = 7168;
		skbmgr_driver_max_skb = 7168;
		skbmgr_limit = 4096;	
		skbmgr_4k_limit = 2048;
		break;
	case MODEL_DSLN16:		//64M 2.4G WiFi, No USB
	case MODEL_DSLAC51:
	case MODEL_DSLN16P:		//64M 2.4G WiFi, USB
		skbmgr_driver_max_skb = 6144;
		skbmgr_limit = 4096;	
		skbmgr_4k_limit = 1024;
		break;
	}

	if (min_free_kbytes > 0)
	{
		memset(strtmp, 0, sizeof(strtmp));
		snprintf(strtmp, sizeof(strtmp), "%d", min_free_kbytes);
		f_write_string("/proc/sys/vm/min_free_kbytes", strtmp, 0, 0);
	}

	if (skbmgr_driver_max_skb > 0)
	{
		memset(strtmp, 0, sizeof(strtmp));
		snprintf(strtmp, sizeof(strtmp), "%d", skbmgr_driver_max_skb);
		f_write_string("/proc/net/skbmgr_driver_max_skb", strtmp, 0, 0);
	}

	if (skbmgr_limit > 0)
	{
		memset(strtmp, 0, sizeof(strtmp));
		snprintf(strtmp, sizeof(strtmp), "%d", skbmgr_limit);
		f_write_string("/proc/net/skbmgr_limit", strtmp, 0, 0);
	}

	if (skbmgr_4k_limit > 0)
	{
		memset(strtmp, 0, sizeof(strtmp));
		snprintf(strtmp, sizeof(strtmp), "%d", skbmgr_4k_limit);
		f_write_string("/proc/net/skbmgr_4k_limit", strtmp, 0, 0);
	}
}

static void tweak_kernel(void)
{
	f_write_string("/proc/sys/net/ipv4/tcp_fin_timeout", "40", 0, 0);
	f_write_string("/proc/sys/net/ipv4/tcp_keepalive_intvl", "30", 0, 0);
	f_write_string("/proc/sys/net/ipv4/tcp_keepalive_probes", "5", 0, 0);
	f_write_string("/proc/sys/net/ipv4/tcp_keepalive_time", "1800", 0, 0);
	f_write_string("/proc/sys/net/ipv4/tcp_retries2", "5", 0, 0);
	f_write_string("/proc/sys/net/ipv4/tcp_syn_retries", "3", 0, 0);
	f_write_string("/proc/sys/net/ipv4/tcp_synack_retries", "3", 0, 0);
	f_write_string("/proc/sys/net/ipv4/tcp_tw_reuse", "1", 0, 0);
	f_write_string("/proc/sys/net/ipv4/icmp_ignore_bogus_error_responses", "1", 0, 0);
	f_write_string("/proc/sys/net/ipv4/icmp_echo_ignore_broadcasts", "1", 0, 0);
	f_write_string("/proc/sys/net/ipv4/tcp_rfc1337", "1", 0, 0);
	f_write_string("/proc/sys/net/ipv4/tcp_syncookies", "1", 0, 0);

	tweak_kernel_platdep();
}

static void sysinit(void)
{
#ifdef RTCONFIG_USB
	mkdir(NOTIFY_DIR, 0755);
	mkdir(NOTIFY_DIR"/"NOTIFY_TYPE_USB, 0755);
#endif
#ifdef TCSUPPORT_USB_PRINTER_SERVER
	// LPRng support
	mkdir("/var/state", 0777);
	mkdir("/var/state/parport", 0777);
	mkdir("/var/state/parport/svr_statue", 0777);
#endif
#ifdef TCSUPPORT_USBHOST
	eval("hotplug2", "--coldplug");
	start_hotplug2();
#endif
	setup_passwd();
	tweak_kernel();
	init_nvram();  // for system indepent part after getting model	

#ifdef RTCONFIG_SHP
	// if(nvram_get_int("qos_enable") || nvram_get_int("macfilter_enable_x") || nvram_get_int("lfp_disable_force")) {
	if(tcapi_match("QoS_Entry0", "qos_enable", "1")) {
		tcapi_set("GUITemp_Entry1", "lfp_disable", "1");
	}
	else {
		tcapi_set("GUITemp_Entry1", "lfp_disable", "0");
	}

	if(tcapi_get_int("GUITemp_Entry1", "lfp_disable")==0) {
		restart_lfp();
	}
#endif

	setup_conntrack();
}

int init_main(int argc, char *argv[])
{

	sysinit();
	
#ifdef TCSUPPORT_USBHOST
	int fd = -1;
	fd = file_lock("usb");	// hold off automount processing
	start_usb();
	file_unlock(fd);	// allow to process usb hotplug events
#endif

	start_services();

#ifdef TCSUPPORT_USB_PRINTER_SERVER
	start_usblpsrv();
#endif

	//update success_start_service in the end
	tcapi_set("WebCurSet_Entry", "success_start_service", "1");

	return 0;
}
