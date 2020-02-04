/*

	Tomato Firmware
	Copyright (C) 2006-2009 Jonathan Zarate

*/

#include "rc.h"
#include <sys/time.h>

#if 1	//#ifdef DEBUG_RCTEST
// used for various testing
static int rctest_main(int argc, char *argv[])
{
	int on = 0;

	if (argc < 3) {
		_dprintf("test what?\n");
	}
	else {
		on = atoi(argv[2]);
		_dprintf("%s %d\n", argv[1], on);

		if(0)	;
#ifdef RTCONFIG_USB
		else if (strcmp(argv[1], "nasapps") == 0) {
			if(on == 1)
				restart_nas_services(0, 1);
			else if(on == 2) {	//restart
				restart_nas_services(1, 1);
			}
			else
				restart_nas_services(1, 0);
		}
		else if (strcmp(argv[1], "ftpsamba") == 0) {
			if(on == 1) {
				create_passwd();
				start_samba();
				start_ftpd();
			}
			else if(on == 2) {	//restart
				stop_samba();
				stop_ftpd();
				sleep(1);
				create_passwd();	//Andy Chiu, 2015/06/05. Add for samba account modification.
				start_samba();
				start_ftpd();
			}
			else {
				stop_samba();
				stop_ftpd();
			}
		}
		else if (strcmp(argv[1], "ftp") == 0) {
			if(on == 1)
				start_ftpd();
			else if(on == 2) {	//restart
				stop_ftpd();
				sleep(1);
				start_ftpd();
			}
			else
				stop_ftpd();
		}
		else if (strcmp(argv[1], "samba") == 0) {
			if(on == 1) {
				create_passwd();
				start_samba();
			}
			else if(on == 2) {	//restart
				stop_samba();
				sleep(1);
				create_passwd();	//Andy Chiu, 2015/06/05. Add for samba account modification.
				start_samba();
			}
			else
				stop_samba();
		}
#ifdef RTCONFIG_WEBDAV
		else if (strcmp(argv[1], "webdav") == 0) {
			if(on == 1)
				start_webdav();				
			else if(on == 2) {	//restart
				stop_webdav();
				sleep(1);
				start_webdav();
			}
			else
				stop_webdav();
			tcapi_commit("Firewall");
			tcapi_commit("Upnpd");
		}
#endif
#ifdef RTCONFIG_CLOUDSYNC
		else if (strcmp(argv[1], "cloudsync") == 0) {
			if(on == 1)
				start_cloudsync();
			else if(on == 2) {	//restart
				stop_cloudsync();
				sleep(1);
				start_cloudsync();
			}
			else
				stop_cloudsync();
		}
#endif
#ifdef TCSUPPORT_SWAP_FILE
		else if (strcmp(argv[1], "swap") == 0) {
			char buf[256] = {0};
			char path[PATH_MAX] = {0};
			if(on == 1) {
				if(tcapi_match("USB_Entry", "usb_path1", "storage")) {
					memset(buf, sizeof(buf), 0);
					memset(path, sizeof(path), 0);
					tcapi_get("USB_Entry", "usb_path1_fs_path0", buf);
					sprintf(path, "/tmp/mnt/%s", buf);
					start_swapfile(path);
				}
				if(tcapi_match("USB_Entry", "usb_path2", "storage")) {
					memset(buf, sizeof(buf), 0);
					memset(path, sizeof(path), 0);
					tcapi_get("USB_Entry", "usb_path2_fs_path0", buf);
					sprintf(path, "/tmp/mnt/%s", buf);
					start_swapfile(path);
				}
			}
			else {
				if(tcapi_match("USB_Entry", "usb_path1", "storage")) {
					memset(buf, sizeof(buf), 0);
					memset(path, sizeof(path), 0);
					tcapi_get("USB_Entry", "usb_path1_fs_path0", buf);
					sprintf(path, "/tmp/mnt/%s", buf);
					stop_swapfile(path);
				}
				if(tcapi_match("USB_Entry", "usb_path2", "storage")) {
					memset(buf, sizeof(buf), 0);
					memset(path, sizeof(path), 0);
					tcapi_get("USB_Entry", "usb_path2_fs_path0", buf);
					sprintf(path, "/tmp/mnt/%s", buf);
					stop_swapfile(path);
				}
			}
		}
#endif
#endif
		else if (strcmp(argv[1], "net_and_phy") == 0) {
			if(on == 1) {
				;
#ifdef RTCONFIG_MEDIA_SERVER
				start_dms();
				start_mt_daapd();
#endif
			}
			else if(on == 2) {	//restart
				tcapi_commit("Lan");
				tcapi_commit("Dhcpd");
				tcapi_commit("Wan");
#ifdef RTCONFIG_USB
				restart_nas_services(1, 1);
#ifdef RTCONFIG_MEDIA_SERVER
				force_stop_dms();
				stop_mt_daapd();
				start_dms();
				start_mt_daapd();
#endif
#endif
#if defined(RTCONFIG_PPTPD) || defined(RTCONFIG_ACCEL_PPTPD)
				if(tcapi_match("PPTP_Entry", "pptpd_enable", "1")) {
					stop_pptpd();
					start_pptpd();
				}
#endif
			}
			else {
				;
#ifdef RTCONFIG_USB
#ifdef RTCONFIG_MEDIA_SERVER
				force_stop_dms();
				stop_mt_daapd();
#endif
#endif
#if defined(RTCONFIG_PPTPD) || defined(RTCONFIG_ACCEL_PPTPD)
				if(tcapi_match("PPTP_Entry", "pptpd_enable", "1"))
					stop_pptpd();
#endif
			}
		}
#ifdef RTCONFIG_SHP
		else if (strcmp(argv[1], "lfp") == 0) {
			restart_lfp();
		}
#endif
		else if (strcmp(argv[1], "service") == 0) {
			stop_wanduck();
#ifdef RTCONFIG_USB
			stop_usb();
#endif
			stop_services();
		}
		else if (strcmp(argv[1], "chk_ping") == 0) {
			
			do_dns_detect();
		}
		else {
			printf("what?\n");
		}
	}
	return 0;
}
#endif


static int hotplug_main(int argc, char *argv[])
{
	if (argc >= 2) {
		if (strcmp(argv[1], "net") == 0) {
			hotplug_net();
		}
#ifdef RTCONFIG_USB
		// !!TB - USB Support
		else if (strcmp(argv[1], "usb") == 0) {
			hotplug_usb();
		}
#ifdef LINUX26
		else if (strcmp(argv[1], "block") == 0) {
			hotplug_usb();
		}
#endif
#endif
	}
	return 0;
}

typedef struct {
	const char *name;
	int (*main)(int argc, char *argv[]);
} applets_t;

static const applets_t applets[] = {
	{ "rc_init",			init_main			},
	{ "rc_call",			rctest_main			},
	{ "ip-up",			ipup_main			},
	{ "ip-down",			ipdown_main			},
	{ "auth-fail",			authfail_main			},
#ifdef RTCONFIG_VPNC
	{ "vpnc-ip-up",			vpnc_ipup_main				},
	{ "vpnc-ip-down",		vpnc_ipdown_main				},
	{ "vpnc-ip-pre-up",		vpnc_ippreup_main				},
	{ "vpnc-auth-fail",		vpnc_authfail_main				},
#ifdef TCSUPPORT_IPV6
	{ "vpnc-ipv6-up",		vpnc_ip6up_main				},
	{ "vpnc-ipv6-down",		vpnc_ip6down_main				},
#endif
#endif
#ifdef RTCONFIG_OPENVPN
	{ "ovpn-up",			ovpn_up_main				},
	{ "ovpn-down",			ovpn_down_main			},
	{ "ovpn-route-up",		ovpn_route_up_main				},
#endif
	{ "hotplug",			hotplug_main			},
	{ "ddns_updated", 		ddns_updated_main		},
	{ "udhcpc_wan",			udhcpc_wan			},
	// { "udhcpc_lan",			udhcpc_lan			},
	{ "udhcpc_expires",		udhcpc_expires			},
	{ "udhcpc_bound",		udhcpc_bound			},
	// { "zcip",			zcip_wan			},
	{ "wanduck",                    wanduck_main                    },
	{ "tcpcheck",                   tcpcheck_main                   },
#ifdef RTCONFIG_USB
	{ "ejusb",			ejusb_main			},
#endif
#ifdef ASUS_DISK_UTILITY
	{ "disk_monitor",		diskmon_main			},
#endif
	{ "service",			service_main			},
#ifdef RTCONFIG_LETSENCRYPT
	{ "le_acme",				le_acme_main			},
#endif
	{NULL, NULL}
};

int main(int argc, char **argv)
{
	char *base;
	int f;

	/*
		Make sure std* are valid since several functions attempt to close these
		handles. If nvram_*() runs first, nvram=0, nvram gets closed. - zzz
	*/
	if ((f = open("/dev/null", O_RDWR)) < 3) {
		dup(f);
		dup(f);
	}
	else {
		close(f);
	}

	base = strrchr(argv[0], '/');
	base = base ? base + 1 : argv[0];

#if defined(DEBUG_NOISY)
	if (nvram_match("debug_logrc", "1")) {
		int i;

		cprintf("[rc %d] ", getpid());
		for (i = 0; i < argc; ++i) {
			cprintf("%s ", argv[i]);
		}
		cprintf("\n");

	}
#endif

#if defined(DEBUG_NOISY)
	// if (nvram_match("debug_ovrc", "1")) {
	{
		char tmp[256];
		char *a[32];

		realpath(argv[0], tmp);
		if ((strncmp(tmp, "/tmp/", 5) != 0) && (argc < 32)) {
			sprintf(tmp, "%s%s", "/tmp/", base);
			if (f_exists(tmp)) {
				cprintf("[rc] override: %s\n", tmp);
				memcpy(a, argv, argc * sizeof(a[0]));
				a[argc] = 0;
				a[0] = tmp;
				execvp(tmp, a);
				exit(0);
			}
		}
	}
#endif

	const applets_t *a;
	for (a = applets; a->name; ++a) {
		if (strcmp(base, a->name) == 0) {
			openlog(base, LOG_PID, LOG_USER);
			return a->main(argc, argv);
		}
	}


	if(!strcmp(base, "restart_wireless")){
		printf("restart wireless...\n");
		// restart_wireless();
		return 0;
	}
	else if(!strcmp(base, "wan_up")){
		if(argc == 2) {
			wan_up(argv[1]);
		}
		else {
			printf("Usage: wan_up <Wan Interface>\n");
		}
		return 0;
	}
#ifdef RTCONFIG_USB
	else if(!strcmp(base, "get_apps_name")){
		if(argc != 2){
			printf("Usage: get_apps_name [File name]\n");
			return 0;
		}

		return get_apps_name(argv[1]);
	}
	else if(!strcmp(base, "asus_sd")){
		if(argc != 3){
			printf("Usage: asus_sd [device_name] [action]\n");
			return 0;
		}

		return asus_sd(argv[1], argv[2]);
	}
	else if(!strcmp(base, "asus_lp")){
		if(argc != 3){
			printf("Usage: asus_lp [device_name] [action]\n");
			return 0;
		}

		return asus_lp(argv[1], argv[2]);
	}
	else if(!strcmp(base, "asus_sg")){
		if(argc != 3){
			printf("Usage: asus_sg [device_name] [action]\n");
			return 0;
		}

		return asus_sg(argv[1], argv[2]);
	}
	else if(!strcmp(base, "asus_sr")){
		if(argc != 3){
			printf("Usage: asus_sr [device_name] [action]\n");
			return 0;
		}

		return asus_sr(argv[1], argv[2]);
	}
	else if(!strcmp(base, "asus_tty")){
		if(argc != 3){
			printf("Usage: asus_tty [device_name] [action]\n");
			return 0;
		}

		return asus_tty(argv[1], argv[2]);
	}
	else if(!strcmp(base, "asus_usbbcm")){
		if(argc != 3){
			printf("Usage: asus_usbbcm [device_name] [action]\n");
			return 0;
		}

		return asus_usbbcm(argv[1], argv[2]);
	}
	else if(!strcmp(base, "asus_usb_interface")){
		if(argc != 3){
			printf("Usage: asus_usb_interface [device_name] [action]\n");
			return 0;
		}

		return asus_usb_interface(argv[1], argv[2]);
	}
	else if (!strcmp(base, "usb_notify")) {
#if defined(RTCONFIG_APP_PREINSTALLED) || defined(RTCONFIG_APP_NETINSTALLED)
		usb_notify();
#endif

		return 0;
	}
#endif
	else if(!strcmp(base, "run_app_script")){
		if(argc != 3){
			printf("Usage: run_app_script [Package name | allpkg] [APP action]\n");
			return 0;
		}

		if(!strcmp(argv[1], "allpkg"))
			return run_app_script(NULL, argv[2]);
		else
			return run_app_script(argv[1], argv[2]);
	}
#ifdef RTCONFIG_VPNC
	else if(!strcmp(base, "run_vpnc")) {
		stop_vpnc();
		start_vpnc();
		return 0;
	}
#endif
#ifdef RTCONFIG_TR069
	else if(!strcmp(base, "run_tr069")) {
		stop_tr();
		start_tr();
		return 0;
	}
#endif

#ifdef RTCONFIG_USB_MODEM
	else if(!strcmp(base, "write_3g_ppp_conf")){
		write_3g_ppp_conf();
		return 0;
	}
	else if(!strcmp(base, "start_wan_if")){
		if(argc == 2)
		{
			start_wan_if(atoi(argv[1]));
		}
		else
		{
			printf("Usage: start_wan_if <Wan_PVC number>\n");
		}
		return 0;
	}
	else if(!strcmp(base, "stop_wan_if")){
		if(argc == 2)
		{
			stop_wan_if(atoi(argv[1]));
		}
		else
		{
			printf("Usage: stop_wan_if <Wan_PVC number>\n");
		}
		return 0;
	}
#endif /* RTCONFIG_USB_MODEM */
	else if(!strcmp(base, "rcset")){
		if( argc != 4 )
		{
			printf("rc set: Error, should be 4 but only %d parameters!\n", argc);
			return -1;
		}
		else
		{
			int ret = -1;
			//cprintf("rcset:%s, %s, %s\n", argv[1], argv[2], argv[3]);
			ret = tcapi_set(argv[1], argv[2], argv[3]);
			return ret; //return 0 on success!
		}
	}
	else if(!strcmp(base, "rcget")){
		if( argc != 3 )
		{
			printf("rc set: Error, should be 3 but only %d parameters!\n", argc);
			return -1;
		}
		else
		{
			int ret = -1;
			char valuebuf[600] = {0};
			//cprintf("rcget:%s, %s\n", argv[1], argv[2]);
			ret = tcapi_get(argv[1], argv[2], valuebuf);
			printf("%s", valuebuf);
			return ret; //return 0 on success!
		}
	}
#ifdef RTCONFIG_OPENVPN
	else if (!strcmp(base, "add_ovpn_fw_rules")) {
		run_ovpn_fw_scripts();
	}
#endif

	printf("Unknown applet: %s\n", base);
	return 0;
}
