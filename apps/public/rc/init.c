#include "rc.h"
#include <sys/types.h>
#include <linux/fs.h>
#include "shutils.h"
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

int init_nvram(void)
{
	int model = get_model();

	tcapi_set(SYSINFO, "rc_support", "");

	/* Paul modify 2013/6/5 */
	switch (model) {
	case MODEL_DSLN16U:
		f_write_string("/proc/sys/vm/min_free_kbytes", "4096", 0, 0);
		nvram_set("ehci_ports", "1-1 1-2");
		nvram_set("ohci_ports", "2-1 2-2");
		add_rc_support("2.4G update usbX2 modem rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid no5gmssid appbase pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN55UC1:
	case MODEL_DSLN55UD1:
		f_write_string("/proc/sys/vm/min_free_kbytes", "4096", 0, 0);
		nvram_set("ehci_ports", "1-1 1-2");
		nvram_set("ohci_ports", "2-1 2-2");
		add_rc_support("2.4G 5G update usbX2 modem rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid appbase pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN66U:
		f_write_string("/proc/sys/vm/min_free_kbytes", "4096", 0, 0);
		nvram_set("ehci_ports", "1-2 1-1");
		nvram_set("ohci_ports", "2-2 2-1");
		add_rc_support("2.4G 5G update usbX2 modem rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid appnet pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN10C1:
	case MODEL_DSLN10PC1:
	case MODEL_DSLN12EC1:
		add_rc_support("2.4G update rawifi dsl wifi_hw_sw ipv6 PARENTAL2 mssid no5gmssid pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN12UC1:
	case MODEL_DSLN14U:
		f_write_string("/proc/sys/vm/min_free_kbytes", "4096", 0, 0);
		nvram_set("ehci_ports", "1-1");
		nvram_set("ohci_ports", "2-1");
		add_rc_support("2.4G update usbX1 modem rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid no5gmssid appnet pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLAC56U:
		f_write_string("/proc/sys/vm/min_free_kbytes", "14336", 0, 0);
		nvram_set("ehci_ports", "1-1 1-2");
		nvram_set("ohci_ports", "2-1 2-2");
		add_rc_support("2.4G 5G 11AC update usbX2 rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid appbase pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN17U:
		f_write_string("/proc/sys/vm/min_free_kbytes", "7168", 0, 0);
		nvram_set("ehci_ports", "1-1 1-2");
		nvram_set("ohci_ports", "2-1 2-2");
		add_rc_support("2.4G update usbX2 rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid appbase pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLAC52U:
		f_write_string("/proc/sys/vm/min_free_kbytes", "14336", 0, 0);
		nvram_set("ehci_ports", "1-1");
		nvram_set("ohci_ports", "2-1");
		add_rc_support("2.4G 5G 11AC update usbX1 rawifi dsl wifi_hw_sw ipv6 PARENTAL2 printer mssid appbase pwrctrl iptv wds HTTPS");
		break;
	case MODEL_DSLN16:
		f_write_string("/proc/sys/vm/min_free_kbytes", "4096", 0, 0);
		add_rc_support("2.4G update rawifi dsl wifi_hw_sw ipv6 PARENTAL2 mssid pwrctrl iptv wds HTTPS");
		break;

	}

	add_rc_support("switchctrl"); //LAN -> Switch Control
	add_rc_support("feedback");
	add_rc_support("telnet");
	add_rc_support("WPS_reset");

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
#ifdef MTK_CRYPTO_DRIVER
	add_rc_support("hwcrypto");
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

	return 0;
}

static void sysinit(void)
{
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
