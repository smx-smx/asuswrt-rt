#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <linux/sockios.h>
#include <shared.h>
#include <bcmnvram.h>
#include <typedefs.h>

#include <rc.h>

#ifdef RTCONFIG_USB_MODEM
#include <usb_info.h>
#endif

void hotplug_net(void)
{
	char *lan_ifname = nvram_safe_get("lan_ifname");
	char *interface, *action;
	bool psta_if, dyn_if, add_event, remove_event;
	int unit;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
#ifdef RTCONFIG_USB_MODEM
	char device_path[128], usb_path[PATH_MAX], usb_node[32], port_path[8];
	char nvram_name[32];
	char word[PATH_MAX];
	char modem_type[8];
#endif

	if (!(interface = getenv("INTERFACE")) ||
	    !(action = getenv("ACTION")))
		return;

	_dprintf("hotplug net INTERFACE=%s ACTION=%s\n", interface, action);

#ifdef LINUX26
	add_event = !strcmp(action, "add");
#else
	add_event = !strcmp(action, "register");
#endif

#ifdef LINUX26
	remove_event = !strcmp(action, "remove");
#else
	remove_event = !strcmp(action, "unregister");
#endif

#ifdef RTCONFIG_BCMWL6
	psta_if = wl_wlif_is_psta(interface);
#else
	psta_if = 0;
#endif

#if ASUSWRT
	dyn_if = !strncmp(interface, "wds", 3) || psta_if;
#else
	dyn_if = 0;
#endif

	if (!dyn_if && !remove_event)
		goto NEITHER_WDS_OR_PSTA;

	if (dyn_if && add_event) {
#ifdef RTCONFIG_RALINK
		if (nvram_match("sw_mode", "2"))
			return;

		if (strncmp(interface, WDSIF_5G, strlen(WDSIF_5G)) == 0 && isdigit(interface[strlen(WDSIF_5G)]))
		{
			if (nvram_match("wl1_mode_x", "0")) return;
		}
		else
		{
			if (nvram_match("wl0_mode_x", "0")) return;
		}
#endif

		/* Bring up the interface and add to the bridge */
		ifconfig(interface, IFUP, NULL, NULL);

#ifdef RTCONFIG_EMF
		if (nvram_match("emf_enable", "1")) {
			eval("emf", "add", "iface", lan_ifname, interface);
			emf_mfdb_update(lan_ifname, interface, TRUE);
			emf_uffp_update(lan_ifname, interface, TRUE);
			emf_rtport_update(lan_ifname, interface, TRUE);
		}
#endif
#ifdef CONFIG_BCMWL5
		/* Indicate interface create event to eapd */
		if (psta_if) {
			_dprintf("hotplug_net(): send dif event to %s\n", interface);
			wl_send_dif_event(interface, 0);
			return;
		}
#endif
		if (!strncmp(lan_ifname, "br", 2)) {
			eval("brctl", "addif", lan_ifname, interface);
#ifdef CONFIG_BCMWL5
			/* Inform driver to send up new WDS link event */
			if (wl_iovar_setint(interface, "wds_enable", 1)) {
				_dprintf("%s set wds_enable failed\n", interface);
				return;
			}
#endif
		}

		return;
	}

#ifdef CONFIG_BCMWL5
	if (remove_event) {
		/* Indicate interface delete event to eapd */
		wl_send_dif_event(interface, 1);

#ifdef RTCONFIG_EMF
		if (nvram_match("emf_enable", "1"))
			eval("emf", "del", "iface", lan_ifname, interface);
#endif /* RTCONFIG_EMF */
	}
#endif

NEITHER_WDS_OR_PSTA:
	/* PPP interface removed */
	if (strncmp(interface, "ppp", 3) == 0 && remove_event) {
#if 0
		while ((unit = ppp_ifunit(interface)) >= 0) {
			snprintf(prefix, sizeof(prefix), "wan%d_", unit);
			nvram_set(strcat_r(prefix, "pppoe_ifname", tmp), "");
		}
#endif
	}
#ifdef RTCONFIG_USB_MODEM
	// Android phone, RNDIS interface, NCM, qmi_wwan.
	else if(!strncmp(interface, "usb", 3)) {
		if ((unit = get_usbif_dualwan_unit()) < 0) {
			usb_dbg("(%s): in the current dual wan mode, didn't support the USB modem.\n", interface);
			return;
		}

		snprintf(prefix, sizeof(prefix), "wan%d_", unit);

		if(!strcmp(action, "add")) {
			unsigned int vid, pid;
			char buf[32];
			int i = 0;

			logmessage("hotplug", "add net %s.", interface);
			_dprintf("hotplug net: add net %s.\n", interface);

			snprintf(device_path, 128, "%s/%s/device", SYS_NET, interface);

			memset(usb_path, 0, PATH_MAX);
			if(realpath(device_path, usb_path) == NULL){
				_dprintf("hotplug net(%s): skip 1. device_path %s.\n", interface, device_path);
				return;
			}

			if(get_usb_node_by_string(usb_path, usb_node, 32) == NULL){
				_dprintf("hotplug net(%s): skip 2. usb_path %s.\n", interface, usb_path);
				return;
			}

			if(get_path_by_node(usb_node, port_path, 8) == NULL){
				_dprintf("hotplug net(%s): skip 3. usb_node %s.\n", interface, usb_node);
				return;
			}

			snprintf(buf, 32, "%s", nvram_safe_get("usb_modem_act_path"));
			if(strcmp(buf, "") && strcmp(buf, usb_node)){
				_dprintf("hotplug net(%s): skip 4. port_path %s.\n", interface, port_path);
				return;
			}

			if(!strcmp(buf, ""))
				nvram_set("usb_modem_act_path", usb_node); // needed by find_modem_type.sh.

			while(!strcmp(nvram_safe_get("usb_modem_act_type"), "") && i++ < 3){
				_dprintf("hotplug net(%s): wait for the modem driver at %d second...\n", interface, i);
#if 0
				eval("find_modem_type.sh");
#else
				system("find_modem_type.sh");
#endif
				sleep(1);
			}

			snprintf(modem_type, 8, "%s", nvram_safe_get("usb_modem_act_type"));
			_dprintf("hotplug net: usb_modem_act_type=%s.\n", modem_type);
			if(!strcmp(modem_type, "mbim")){
				_dprintf("hotplug net(%s): skip the MBIM interface.\n", interface);
				return;
			}

			vid = get_usb_vid(usb_node);
			pid = get_usb_pid(usb_node);
			logmessage("hotplug", "Got net %s, vid 0x%x, pid 0x%x.", interface, vid, pid);
			_dprintf("hotplug net: Got net %s, vid 0x%x, pid 0x%x.\n", interface, vid, pid);

#ifdef RT4GAC55U
			if(vid == 0x05c6 && pid == 0x9026 && !strcmp(port_path, "2") && nvram_get_int("usb_gobi") != 1)
				return;
#endif

			if(!strcmp(interface, "usb0")){
				_dprintf("hotplug net(%s): let usb0 wait for usb1.\n", interface);
				sleep(1);
			}

			snprintf(nvram_name, 32, "usb_path%s_act", port_path);
			snprintf(word, PATH_MAX, "%s", nvram_safe_get(nvram_name));
			_dprintf("hotplug net(%s): %s %s.\n", interface, nvram_name, word);

			//if(!strcmp(word, "usb1") && strcmp(interface, "usb1")){
			if(!strcmp(word, "usb1")){
				// If there are 2 usbX, use QMI:usb1 to connect.
				logmessage("hotplug", "skip to set net %s.", interface);
				_dprintf("hotplug net: skip to set net %s.\n", interface);
				return;
			}
			else{
				logmessage("hotplug", "set net %s.", interface);
				_dprintf("hotplug net: set net %s.\n", interface);
				nvram_set(nvram_name, interface);
				snprintf(buf, 32, "%u", vid);
				nvram_set("usb_modem_act_vid", buf);
				snprintf(buf, 32, "%u", pid);
				nvram_set("usb_modem_act_pid", buf);
				nvram_set("usb_modem_act_dev", interface);
				nvram_set(strcat_r(prefix, "ifname", tmp), interface);
			}

			// won't wait at the busy time of every start_wan when booting.
			if(!strcmp(nvram_safe_get("success_start_service"), "1")){
				// wait for Andorid phones.
				_dprintf("hotplug net INTERFACE=%s ACTION=%s: wait 2 seconds...\n", interface, action);
				sleep(2);
			}
			else{
				// wait that the modem nvrams are ready during booting.
				// e.q. Huawei E398.
				int i = 0;
				snprintf(nvram_name, 32, "usb_path%s", port_path);
				while(strcmp(nvram_safe_get(nvram_name), "modem") && i++ < 3){
					_dprintf("%s: waiting %d second for the modem nvrams...", __FUNCTION__, i);
					sleep(1);
				}
			}

			if(nvram_get_int("usb_modem_act_reset") != 0){
				logmessage("hotplug", "Modem had been reset and woken up net %s.", interface);
				_dprintf("hotplug net(%s) had been reset and woken up.\n", interface);
				nvram_set("usb_modem_act_reset", "2");
				return;
			}

#ifdef RTCONFIG_DUALWAN
			// avoid the busy time of every start_wan when booting.
			if(!strcmp(nvram_safe_get("success_start_service"), "0")
					&& strcmp(modem_type, "rndis") // rndis modem can't get IP when booting.
					&& strcmp(modem_type, "qmi") // qmi modem often be blocked when booting.
					&& (unit == WAN_UNIT_FIRST || nvram_match("wans_mode", "lb"))
					){
				_dprintf("%s: start_wan_if(%d)!\n", __FUNCTION__, unit);
#if 0
				start_wan_if(unit);
#else
				tcapi_set("USBModem_Entry", "usb_modem_act", "start");
				tcapi_commit("USBModem_Entry");
#endif
			}
#endif
		}
		else{
			logmessage("hotplug", "remove net %s.", interface);
			_dprintf("hotplug net: remove net %s.\n", interface);

			snprintf(usb_node, 32, "%s", nvram_safe_get("usb_modem_act_path"));
			if(strlen(usb_node) <= 0)
				return;

			if(get_path_by_node(usb_node, port_path, 8) == NULL)
				return;

			snprintf(nvram_name, 32, "usb_path%s_act", port_path);

			if(!strcmp(nvram_safe_get(nvram_name), interface)) {
				tcapi_set("USBModem_Entry", "usb_modem_act", "stop");
				tcapi_commit("USBModem_Entry");

				nvram_unset(nvram_name);
				clean_modem_state(1);
			}

			if(strlen(port_path) <= 0)
				return;

			nvram_set(strcat_r(prefix, "ifname", tmp), "");
#if 0
			char dhcp_pid_file[1024];

			snprintf(dhcp_pid_file, 1024, "/var/run/udhcpc-%s.pid", interface);

			kill_pidfile_s(dhcp_pid_file, SIGUSR2);
			kill_pidfile_s(dhcp_pid_file, SIGTERM);
#endif
		}
	}
	// Beceem dongle, ASIX USB to RJ45 converter, ECM.
	else if(!strncmp(interface, "eth", 3)) {
		// for all models, ethernet's physical interface.
		if(!strncmp(interface, "eth0", 4))
			return;

		if ((unit = get_usbif_dualwan_unit()) < 0) {
			usb_dbg("(%s): in the current dual wan mode, didn't support the USB modem.\n", interface);
			return;
		}

		snprintf(prefix, sizeof(prefix), "wan%d_", unit);

		if(!strcmp(action, "add")) {
			nvram_set(strcat_r(prefix, "ifname", tmp), interface);

			_dprintf("hotplug net INTERFACE=%s ACTION=%s: wait 2 seconds...\n", interface, action);
			sleep(2);

			snprintf(device_path, 128, "%s/%s/device", SYS_NET, interface);

			// Huawei E353.
			if(check_if_dir_exist(device_path)) {
				memset(usb_path, 0, PATH_MAX);
				if(realpath(device_path, usb_path) == NULL)
					return;

				if(get_usb_node_by_string(usb_path, usb_node, 32) == NULL)
					return;

				if(get_path_by_node(usb_node, port_path, 8) == NULL)
					return;

				snprintf(nvram_name, 32, "usb_path%s_act", port_path);

				nvram_set(nvram_name, interface);
				nvram_set("usb_modem_act_path", usb_node);
				nvram_set("usb_modem_act_dev", interface);
			}
			// Beceem dongle.
			else{
				// do nothing.
			}

#ifdef RTCONFIG_DUALWAN
			// avoid the busy time of every start_wan when booting.
			if(!strcmp(nvram_safe_get("success_start_service"), "0")
					&& (unit == WAN_UNIT_FIRST || nvram_match("wans_mode", "lb"))
					){
				_dprintf("%s: start_wan_if(%d)!\n", __FUNCTION__, unit);
#if 0
				start_wan_if(unit);
#else
				tcapi_set("USBModem_Entry", "usb_modem_act", "start");
				tcapi_commit("USBModem_Entry");
#endif
			}
#endif
		}
		else{
			nvram_set(strcat_r(prefix, "ifname", tmp), "");

			//stop_wan_if(unit);

			snprintf(usb_node, 32, "%s", nvram_safe_get("usb_modem_act_path"));
			if(strlen(usb_node) <= 0)
				return;

			if(get_path_by_node(usb_node, port_path, 8) == NULL)
				return;

			snprintf(nvram_name, 32, "usb_path%s_act", port_path);

			if(!strcmp(nvram_safe_get(nvram_name), interface)) {
				tcapi_set("USBModem_Entry", "usb_modem_act", "stop");
				tcapi_commit("USBModem_Entry");

				nvram_unset(nvram_name);
				clean_modem_state(1);
			}

			char dhcp_pid_file[1024];

			snprintf(dhcp_pid_file, 1024, "/var/run/udhcpc-%s.pid", interface);

			kill_pidfile_s(dhcp_pid_file, SIGUSR2);
			kill_pidfile_s(dhcp_pid_file, SIGTERM);

#ifdef RTCONFIG_USB_BECEEM
			if(strlen(port_path) <= 0)
				system("asus_usbbcm usbbcm remove");
#endif
		}
	}
#ifdef RTCONFIG_USB_BECEEM
	// WiMAX: madwimax, gctwimax.
	else if(!strncmp(interface, "wimax", 5)){
		if(nvram_get_int("sw_mode") != SW_MODE_ROUTER)
			return;

		if((unit = get_usbif_dualwan_unit()) < 0){
			usb_dbg("(%s): in the current dual wan mode, didn't support the USB modem.\n", interface);
			return;
		}

		snprintf(prefix, sizeof(prefix), "wan%d_", unit);

		if(!strcmp(action, "add")){
			snprintf(usb_node, 32, "%s", nvram_safe_get("usb_modem_act_path"));
			if(strlen(usb_node) <= 0)
				return;
			if(get_path_by_node(usb_node, port_path, 8) == NULL)
				return;

			nvram_set(strcat_r(prefix, "ifname", tmp), interface);

			snprintf(nvram_name, 32, "usb_path%s_act", port_path);
			nvram_set(nvram_name, interface);

			_dprintf("hotplug net INTERFACE=%s ACTION=%s: wait 2 seconds...\n", interface, action);
			sleep(2);

#ifdef RTCONFIG_DUALWAN
			// avoid the busy time of every start_wan when booting.
			if(!strcmp(nvram_safe_get("success_start_service"), "0")
					&& (unit == WAN_UNIT_FIRST || nvram_match("wans_mode", "lb"))
					){
				_dprintf("%s: start_wan_if(%d)!\n", __FUNCTION__, unit);
#if 0
				start_wan_if(unit);
#else
				tcapi_set("USBModem_Entry", "usb_modem_act", "start");
				tcapi_commit("USBModem_Entry");
#endif
			}
#endif
		}
		else // remove: do nothing.
			;
	}
#endif
#endif
}

