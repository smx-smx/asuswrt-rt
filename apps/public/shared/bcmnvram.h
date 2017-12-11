#ifndef	_BCMNVRAM_H
#define	_BCMNVRAM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include <libtcapi.h>
#include <tcapi.h>

#define DUALWAN_DATA	"Dualwan_Entry"
#define WANDUCK_DATA	"Wanduck_Common"
#define SYSINFO		"SysInfo_Entry"
#define USB_DATA	"USB_Entry"
#define USBMODEM_DATA	"USBModem_Entry"
#define APPS_DATA	"Apps_Entry"
#define SAMBA_DATA	"Samba_Entry"
#define WEBDAV_DATA	"AiCloud_Entry"
#define CLOUD_DATA	"AiCloud_Entry"
#define WEBCURSET_DATA	"WebCurSet_Entry"

static inline char *tcapi_x_get(char *flag){
	static char str[MAXLEN_TCAPI_MSG];
	int ret = 0;

	memset(str, 0, MAXLEN_TCAPI_MSG);

	if(!strncmp(flag, "rc_support", 10))
		ret = tcapi_get(SYSINFO, flag, str);
	if(!strncmp(flag, "productid", strlen("productid")))
		ret = tcapi_get(SYSINFO, "ProductName", str);
#ifdef RTCONFIG_ODMPID
	else if(!strncmp(flag, "odmpid", strlen("odmpid")))
		ret = tcapi_get(SYSINFO, "odmpid", str);
#endif
	else if(!strcmp(flag, "x_Setting"))
		ret = tcapi_get(SYSINFO, flag, str);
	else if(!strncmp(flag, "nat_", 4))
		ret = tcapi_get(SYSINFO, flag, str);
	else if(!strncmp(flag, "wans", 4))
		ret = tcapi_get(DUALWAN_DATA, flag, str);
	else if(!strncmp(flag, "wandog", 6))
		ret = tcapi_get(DUALWAN_DATA, flag, str);
	else if(!strncmp(flag, "wan", 3))
		ret = tcapi_get(WANDUCK_DATA, flag, str);
	else if(!strncmp(flag, "link_wan", 8))
		ret = tcapi_get(WANDUCK_DATA, flag, str);
	else if(!strcmp(flag, "link_internet"))
		ret = tcapi_get(WANDUCK_DATA, flag, str);
	else if(!strcmp(flag, "web_redirect"))
		ret = tcapi_get(WANDUCK_DATA, flag, str);
	else if(!strcmp(flag, "asus_mfg"))
		ret = tcapi_get(SYSINFO, flag, str);
	else if(!strcmp(flag, "success_start_service"))
		ret = tcapi_get(WEBCURSET_DATA, flag, str);
#ifdef RTCONFIG_USB
#ifdef RTCONFIG_USB_MODEM
	else if(!strncmp(flag, "modem_", 6))
		ret = tcapi_get(USBMODEM_DATA, flag, str);
	else if(!strncmp(flag, "usb_modem_", 10))
		ret = tcapi_get(USBMODEM_DATA, flag, str);
	else if(!strcmp(flag, "Dev3G"))
		ret = tcapi_get(USBMODEM_DATA, flag, str);
	else if(!strcmp(flag, "d3g"))
		ret = tcapi_get(USBMODEM_DATA, flag, str);
	else if(!strncmp(flag, "g3state_", 8))
		ret = tcapi_get(USBMODEM_DATA, flag, str);
	else if(!strncmp(flag, "g3err_", 6))
		ret = tcapi_get(USBMODEM_DATA, flag, str);
	else if(!strncmp(flag, "stop_modem", 10))
		ret = tcapi_get(USBMODEM_DATA, flag, str);
	else if(!strncmp(flag, "stop_conn_3g", 12))
		ret = tcapi_get(USBMODEM_DATA, flag, str);
	else if(!strcmp(flag, "test_k3770"))
		ret = tcapi_get(USBMODEM_DATA, flag, str);
#ifdef RTCONFIG_USB_BECEEM
	else if(!strcmp(flag, "beceem_switch"))
		ret = tcapi_get(USBMODEM_DATA, flag, str);
#endif
	else
#endif /* RTCONFIG_USB_MODEM */
	if(!strncmp(flag, "usb_", 4))
		ret = tcapi_get(USB_DATA, flag, str);
	else if(strstr(flag, "hci_ports"))
		ret = tcapi_get(USB_DATA, flag, str);
	else if(!strcmp(flag, "stop_xhcimode"))
		ret = tcapi_get(USB_DATA, flag, str);
	else if(!strncmp(flag, "stop_sd", 7))
		ret = tcapi_get(USB_DATA, flag, str);
	else if(!strncmp(flag, "stop_lp", 7))
		ret = tcapi_get(USB_DATA, flag, str);
	else if(!strncmp(flag, "stop_sg", 7))
		ret = tcapi_get(USB_DATA, flag, str);
	else if(!strncmp(flag, "stop_cd", 7))
		ret = tcapi_get(USB_DATA, flag, str);
	else if(!strncmp(flag, "stop_ui", 7))
		ret = tcapi_get(USB_DATA, flag, str);
	else if(!strncmp(flag, "u2ec_", 5))
		ret = tcapi_get(USB_DATA, flag, str);
	else if(!strncmp(flag, "apps_", 5))
		ret = tcapi_get(APPS_DATA, flag, str);
	else if(!strncmp(flag, "st_", 3))
		ret = tcapi_get(SAMBA_DATA, flag, str);
	else if(!strncmp(flag, "webdav_", 7))
		ret = tcapi_get(WEBDAV_DATA, flag, str);
	else if(!strncmp(flag, "ss_support", 10))
		ret = tcapi_get(CLOUD_DATA, flag, str);
#endif /* RTCONFIG_USB */

	if(ret == 0 && str[0] != 0)
		return str;

	if(ret != 0)
		return NULL;
	else
		return "";
}

#define nvram_get(x) tcapi_x_get(x)

static inline char *nvram_safe_get(char *flag){
	char *ptr;

	if((ptr = nvram_get(flag)) != NULL)
		return ptr;
	else
		return "";
}

#define nvram_get_int(x) atoi(nvram_safe_get(x))
#define nvram_match(x, y) !strcmp(nvram_safe_get(x), y)
#define nvram_invmatch(x, y) strcmp(nvram_safe_get(x), y)

static inline int tcapi_x_set(char *flag, char *value){
	int ret = 0;

	if(!strncmp(flag, "rc_support", 10))
		ret = tcapi_set(SYSINFO, flag, value);
	if(!strncmp(flag, "productid", strlen("productid")))
		ret = tcapi_set(SYSINFO, "ProductName", value);
#ifdef RTCONFIG_ODMPID
	else if(!strncmp(flag, "odmpid", strlen("odmpid")))
		ret = tcapi_set(SYSINFO, "odmpid", value);
#endif
	else if(!strcmp(flag, "x_Setting"))
		ret = tcapi_set(SYSINFO, flag, value);
	else if(!strncmp(flag, "nat_", 4))
		ret = tcapi_set(SYSINFO, flag, value);
	else if(!strncmp(flag, "wans", 4))
		ret = tcapi_set(DUALWAN_DATA, flag, value);
	else if(!strncmp(flag, "wandog", 6))
		ret = tcapi_set(DUALWAN_DATA, flag, value);
	else if(!strncmp(flag, "wan", 3))
		ret = tcapi_set(WANDUCK_DATA, flag, value);
	else if(!strncmp(flag, "link_wan", 8))
		ret = tcapi_set(WANDUCK_DATA, flag, value);
	else if(!strcmp(flag, "link_internet"))
		ret = tcapi_set(WANDUCK_DATA, flag, value);
	else if(!strcmp(flag, "web_redirect"))
		ret = tcapi_set(WANDUCK_DATA, flag, value);
#ifdef RTCONFIG_USB
#ifdef RTCONFIG_USB_MODEM
	else if(!strncmp(flag, "modem_", 6))
		ret = tcapi_set(USBMODEM_DATA, flag, value);
	else if(!strncmp(flag, "usb_modem_", 10))
		ret = tcapi_set(USBMODEM_DATA, flag, value);
	else if(!strcmp(flag, "Dev3G"))
		ret = tcapi_set(USBMODEM_DATA, flag, value);
	else if(!strcmp(flag, "d3g"))
		ret = tcapi_set(USBMODEM_DATA, flag, value);
	else if(!strncmp(flag, "g3state_", 8))
		ret = tcapi_set(USBMODEM_DATA, flag, value);
	else if(!strncmp(flag, "g3err_", 6))
		ret = tcapi_set(USBMODEM_DATA, flag, value);
	else if(!strncmp(flag, "stop_modem", 10))
		ret = tcapi_set(USBMODEM_DATA, flag, value);
	else if(!strncmp(flag, "stop_conn_3g", 12))
		ret = tcapi_set(USBMODEM_DATA, flag, value);
	else if(!strcmp(flag, "test_k3770"))
		ret = tcapi_set(USBMODEM_DATA, flag, value);
#ifdef RTCONFIG_USB_BECEEM
	else if(!strcmp(flag, "beceem_switch"))
		ret = tcapi_set(USBMODEM_DATA, flag, value);
#endif
	else
#endif /* RTCONFIG_USB_MODEM */
	if(!strncmp(flag, "usb_", 4))
		ret = tcapi_set(USB_DATA, flag, value);
	else if(strstr(flag, "hci_ports"))
		ret = tcapi_set(USB_DATA, flag, value);
	else if(!strcmp(flag, "stop_xhcimode"))
		ret = tcapi_set(USB_DATA, flag, value);
	else if(!strncmp(flag, "stop_sd", 7))
		ret = tcapi_set(USB_DATA, flag, value);
	else if(!strncmp(flag, "stop_lp", 7))
		ret = tcapi_set(USB_DATA, flag, value);
	else if(!strncmp(flag, "stop_sg", 7))
		ret = tcapi_set(USB_DATA, flag, value);
	else if(!strncmp(flag, "stop_cd", 7))
		ret = tcapi_set(USB_DATA, flag, value);
	else if(!strncmp(flag, "stop_ui", 7))
		ret = tcapi_set(USB_DATA, flag, value);
	else if(!strncmp(flag, "u2ec_", 5))
		ret = tcapi_set(USB_DATA, flag, value);
	else if(!strncmp(flag, "apps_", 5))
		ret = tcapi_set(APPS_DATA, flag, value);
	else if(!strncmp(flag, "st_", 3))
		ret = tcapi_set(SAMBA_DATA, flag, value);
	else if(!strncmp(flag, "webdav_", 7))
		ret = tcapi_set(WEBDAV_DATA, flag, value);
	else if(!strncmp(flag, "ss_support", 10))
		ret = tcapi_set(CLOUD_DATA, flag, value);
#endif

	return ret;
}

#define nvram_set(x, y) tcapi_x_set(x, y)

static inline int nvram_set_int(char *key, int value){
	char nvramstr[128];

	snprintf(nvramstr, 128, "%d", value);
	return nvram_set(key, nvramstr);
}

#define nvram_unset(x) tcapi_x_set(x, "")

#define nvram_commit tcapi_save

#endif	//_BCMNVRAM_H
