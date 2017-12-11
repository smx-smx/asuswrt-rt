 /*
 * Copyright 2016, ASUSTeK Inc.
 * All Rights Reserved.
 *
 * THIS SOFTWARE IS OFFERED "AS IS", AND ASUS GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 */

#ifndef __nt_actMail_h__
#define __nt_actMail_h__


#include <libnt.h>
#include <nt_eInfo.h>
#include <nt_actMail_common.h>

#define MyDBG(fmt,args...) \
	if(isFileExist(NOTIFY_ACTION_MAIL_DEBUG) > 0) { \
		Debug2Console("[ACTMAIL][%s:(%d)]"fmt, __FUNCTION__, __LINE__, ##args); \
	}
#define ErrorMsg(fmt,args...) \
	Debug2Console("[ACTMAIL][%s:(%d)]"fmt, __FUNCTION__, __LINE__, ##args);

/* Define a shuffle function. e.g. DECL_SHUFFLE(double). */
#define DECL_SHUFFLE(type)				\
void shuffle_##type(type *list, size_t len) {		\
	int j;						\
	type tmp;					\
	while(len) {					\
		j = irand(len);				\
		if (j != len - 1) {			\
			tmp = list[j];			\
			list[j] = list[len - 1];	\
			list[len - 1] = tmp;		\
		}					\
		len--;					\
	}						\
}							\


/* SEND MAIL INFO STRUCTURE
---------------------------------*/
typedef struct __mail_info__t_
{
	char   modelName[16];
	char   subject[64];
	char   toMail[256];
	char   msg[100];
	time_t tstamp;
	int    MsendId;
	int    event;

}MAIL_INFO_T;

/* NOTIFY MAIL CONTENT INFO STRUCTURE
---------------------------------*/
struct MAIL_CONTENT_T
{
	char lang[4];
	int  event;
	MAIL_INFO_T *MailContentFunc;
};

/* NOTIFY MAIL CONTENT CALLBACK FUNCTION DECLARE
NAMING RULE: [EventName]_[Language]_FUNC
---------------------------------
   ### Reservation ###
---------------------------------*/
/* RESERVATION MAIL EVENT */
MAIL_INFO_T *RESERVATION_MAIL_CONFIRM_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *RESERVATION_MAIL_CONFIRM_TW_FUNC(MAIL_INFO_T *mInfo);
/* ------------------------------
   ### System ###
---------------------------------*/
/* WAN EVENT */
MAIL_INFO_T *SYS_WAN_DISCONN_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *SYS_WAN_BLOCK_EN_FUNC(MAIL_INFO_T *mInfo);
/* PASSWORD EVENT */
MAIL_INFO_T *SYS_PASSWORD_SAME_WITH_LOGIN_WIFI_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *SYS_PASSWORD_WIFI_WEAK_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *SYS_PASSWORD_LOGIN_STRENGTH_CHECK_EN_FUNC(MAIL_INFO_T *mInfo);
/* GUEST NETWORK EVENT */
MAIL_INFO_T *SYS_GUESTWIFI_ONE_ENABLE_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *SYS_GUESTWIFI_MORE_ENABLE_EN_FUNC(MAIL_INFO_T *mInfo);
/* RSSI EVENT */
MAIL_INFO_T *SYS_RSSI_LOW_SIGNAL_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *SYS_RSSI_LOW_SIGNAL_AGAIN_EN_FUNC(MAIL_INFO_T *mInfo);
/* DUALWAN EVENT */
MAIL_INFO_T *SYS_DUALWAN_FAILOVER_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *SYS_DUALWAN_FAILBACK_EN_FUNC(MAIL_INFO_T *mInfo);
/* SYS DETECT EVENT */
MAIL_INFO_T *SYS_SCAN_DLNA_PLAYER_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *SYS_DETECT_ASUS_SSID_UNENCRYPT_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *SYS_ECO_MODE_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *SYS_GAME_MODE_EN_FUNC(MAIL_INFO_T *mInfo);
/* FIRMWARE EVENT */
MAIL_INFO_T *SYS_FW_NWE_VERSION_AVAILABLE_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *SYS_NEW_SIGNATURE_UPDATED_EN_FUNC(MAIL_INFO_T *mInfo);
/* ------------------------------
   ### Administration ###
---------------------------------*/
/* LOGIN EVENT */
MAIL_INFO_T *ADMIN_LOGIN_FAIL_LAN_WEB_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *ADMIN_LOGIN_FAIL_SSH_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *ADMIN_LOGIN_FAIL_TELNET_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *ADMIN_LOGIN_FAIL_SSID_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *ADMIN_LOGIN_FAIL_AICLOUD_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *ADMIN_LOGIN_DEVICE_DOUBLE_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *ADMIN_LOGIN_ACCOUNT_DOBLE_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *ADMIN_LOGIN_FAIL_VPNSERVER_EN_FUNC(MAIL_INFO_T *mInfo);
/* ------------------------------
   ### Security ###
---------------------------------*/
/* PROTECTION EVENT */
MAIL_INFO_T *PROTECTION_INTO_MONITORMODE_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *PROTECTION_VULNERABILITY_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *PROTECTION_CC_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *PROTECTION_DOS_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *PROTECTION_SAMBA_GUEST_ENABLE_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *PROTECTION_FTP_GUEST_ENABLE_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *PROTECTION_FIREWALL_DISABLE_EN_FUNC(MAIL_INFO_T *mInfo);
/* ------------------------------
   ### Parental Contorl ###
---------------------------------*/
/* PARENTAL CONTROL EVENT */
MAIL_INFO_T *PARENTALCTL_BLOCK_EN_FUNC(MAIL_INFO_T *mInfo);
/* ------------------------------
   ### Traffic Management ###
---------------------------------*/
/* TRAFFIC METER EVENT */
MAIL_INFO_T *TRAFFICMETER_ALERT_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *TRAFFICMETER_BW_LIMITER_EN_FUNC(MAIL_INFO_T *mInfo);
/* ------------------------------
   ### USB Function ###
---------------------------------*/
/* USB EVENT */
MAIL_INFO_T *USB_DM_TASK_FINISHED_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *USB_DISK_SCAN_FAIL_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *USB_DISK_EJECTED_FAIL_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *USB_DISK_PARTITION_FULL_EN_FUNC(MAIL_INFO_T *mInfo);
MAIL_INFO_T *USB_DISK_FULL_EN_FUNC(MAIL_INFO_T *mInfo);


/* NOTIFY MAIL CONTENT CALLBACK FUNCTION TABLE
NAMING RULE: [EventName]_[Language]_FUNC
---------------------------------*/
struct MAIL_CONTENT_T CONTENT_TABLE[] =
{
	/* ------------------------------
	   ### Reservation ###
	---------------------------------*/
	/* RESERVATION MAIL EVENT */
	{"EN", RESERVATION_MAIL_CONFIRM_EVENT,          (MAIL_INFO_T *) RESERVATION_MAIL_CONFIRM_EN_FUNC},
//	{"TW", RESERVATION_MAIL_CONFIRM_EVENT,          (MAIL_INFO_T *) RESERVATION_MAIL_CONFIRM_TW_FUNC},
	/* ------------------------------
	   ### System ###
	---------------------------------*/
	/* WAN EVENT */
	{"EN", SYS_WAN_DISCONN_EVENT,                   (MAIL_INFO_T *) SYS_WAN_DISCONN_EN_FUNC},
	{"EN", SYS_WAN_BLOCK_EVENT,                     (MAIL_INFO_T *) SYS_WAN_BLOCK_EN_FUNC},
	/* PASSWORD EVENT */
	{"EN", SYS_PASSWORD_SAME_WITH_LOGIN_WIFI_EVENT, (MAIL_INFO_T *) SYS_PASSWORD_SAME_WITH_LOGIN_WIFI_EN_FUNC},
	{"EN", SYS_PASSWORD_WIFI_WEAK_EVENT,            (MAIL_INFO_T *) SYS_PASSWORD_WIFI_WEAK_EN_FUNC},
	{"EN", SYS_PASSWORD_LOGIN_STRENGTH_CHECK_EVENT, (MAIL_INFO_T *) SYS_PASSWORD_LOGIN_STRENGTH_CHECK_EN_FUNC},
	/* GUEST NETWORK EVENT */
	{"EN", SYS_GUESTWIFI_ONE_ENABLE_EVENT,          (MAIL_INFO_T *) SYS_GUESTWIFI_ONE_ENABLE_EN_FUNC},
	{"EN", SYS_GUESTWIFI_MORE_ENABLE_EVENT,         (MAIL_INFO_T *) SYS_GUESTWIFI_MORE_ENABLE_EN_FUNC},
	/* RSSI EVENT */
	{"EN", SYS_RSSI_LOW_SIGNAL_EVENT,               (MAIL_INFO_T *) SYS_RSSI_LOW_SIGNAL_EN_FUNC},
	{"EN", SYS_RSSI_LOW_SIGNAL_AGAIN_EVENT,         (MAIL_INFO_T *) SYS_RSSI_LOW_SIGNAL_AGAIN_EN_FUNC},
	/* DUALWAN EVENT */
	{"EN", SYS_DUALWAN_FAILOVER_EVENT,              (MAIL_INFO_T *) SYS_DUALWAN_FAILOVER_EN_FUNC},
	{"EN", SYS_DUALWAN_FAILBACK_EVENT,              (MAIL_INFO_T *) SYS_DUALWAN_FAILBACK_EN_FUNC},
	/* SYS DETECT EVENT */
	{"EN", SYS_SCAN_DLNA_PLAYER_EVENT,              (MAIL_INFO_T *) SYS_SCAN_DLNA_PLAYER_EN_FUNC},
	{"EN", SYS_DETECT_ASUS_SSID_UNENCRYPT_EVENT,    (MAIL_INFO_T *) SYS_DETECT_ASUS_SSID_UNENCRYPT_EN_FUNC},
	{"EN", SYS_ECO_MODE_EVENT,                      (MAIL_INFO_T *) SYS_ECO_MODE_EN_FUNC},
	{"EN", SYS_GAME_MODE_EVENT,                     (MAIL_INFO_T *) SYS_GAME_MODE_EN_FUNC},
	/* FIRMWARE EVENT */
	{"EN", SYS_FW_NWE_VERSION_AVAILABLE_EVENT,      (MAIL_INFO_T *) SYS_FW_NWE_VERSION_AVAILABLE_EN_FUNC},
	{"EN", SYS_NEW_SIGNATURE_UPDATED_EVENT,         (MAIL_INFO_T *) SYS_NEW_SIGNATURE_UPDATED_EN_FUNC},
	/* ------------------------------
	   ### Administration ###
	---------------------------------*/
	/* LOGIN EVENT */
	{"EN", ADMIN_LOGIN_FAIL_LAN_WEB_EVENT,          (MAIL_INFO_T *) ADMIN_LOGIN_FAIL_LAN_WEB_EN_FUNC},
	{"EN", ADMIN_LOGIN_FAIL_SSH_EVENT,              (MAIL_INFO_T *) ADMIN_LOGIN_FAIL_SSH_EN_FUNC},
	{"EN", ADMIN_LOGIN_FAIL_TELNET_EVENT,           (MAIL_INFO_T *) ADMIN_LOGIN_FAIL_TELNET_EN_FUNC},
	{"EN", ADMIN_LOGIN_FAIL_SSID_EVENT,             (MAIL_INFO_T *) ADMIN_LOGIN_FAIL_SSID_EN_FUNC},
	{"EN", ADMIN_LOGIN_FAIL_AICLOUD_EVENT,          (MAIL_INFO_T *) ADMIN_LOGIN_FAIL_AICLOUD_EN_FUNC},
	{"EN", ADMIN_LOGIN_DEVICE_DOUBLE_EVENT,         (MAIL_INFO_T *) ADMIN_LOGIN_DEVICE_DOUBLE_EN_FUNC},
	{"EN", ADMIN_LOGIN_ACCOUNT_DOBLE_EVENT,         (MAIL_INFO_T *) ADMIN_LOGIN_ACCOUNT_DOBLE_EN_FUNC},
	{"EN", ADMIN_LOGIN_FAIL_VPNSERVER_EVENT,        (MAIL_INFO_T *) ADMIN_LOGIN_FAIL_VPNSERVER_EN_FUNC},
	/* ------------------------------
	   ### Security ###
	---------------------------------*/
	/* PROTECTION EVENT */
	{"EN", PROTECTION_INTO_MONITORMODE_EVENT,       (MAIL_INFO_T *) PROTECTION_INTO_MONITORMODE_EN_FUNC},
	{"EN", PROTECTION_VULNERABILITY_EVENT,          (MAIL_INFO_T *) PROTECTION_VULNERABILITY_EN_FUNC},
	{"EN", PROTECTION_CC_EVENT,                     (MAIL_INFO_T *) PROTECTION_CC_EN_FUNC},
	{"EN", PROTECTION_DOS_EVENT,                    (MAIL_INFO_T *) PROTECTION_DOS_EN_FUNC},
	{"EN", PROTECTION_SAMBA_GUEST_ENABLE_EVENT,     (MAIL_INFO_T *) PROTECTION_SAMBA_GUEST_ENABLE_EN_FUNC},
	{"EN", PROTECTION_FTP_GUEST_ENABLE_EVENT,       (MAIL_INFO_T *) PROTECTION_FTP_GUEST_ENABLE_EN_FUNC},
	{"EN", PROTECTION_FIREWALL_DISABLE_EVENT,       (MAIL_INFO_T *) PROTECTION_FIREWALL_DISABLE_EN_FUNC},
	/* ------------------------------
	   ### Parental Contorl ###
	---------------------------------*/
	/* PARENTAL CONTROL EVENT */
	{"EN", PARENTALCTL_BLOCK_EVENT,                 (MAIL_INFO_T *) PARENTALCTL_BLOCK_EN_FUNC},
	/* ------------------------------
	   ### Traffic Management ###
	---------------------------------*/
	/* TRAFFIC METER EVENT */
	{"EN", TRAFFICMETER_ALERT_EVENT,                (MAIL_INFO_T *) TRAFFICMETER_ALERT_EN_FUNC},
	{"EN", TRAFFICMETER_BW_LIMITER_EVENT,           (MAIL_INFO_T *) TRAFFICMETER_BW_LIMITER_EN_FUNC},
	/* ------------------------------
	   ### USB Function ###
	---------------------------------*/
	/* USB EVENT */
	{"EN", USB_DM_TASK_FINISHED_EVENT,              (MAIL_INFO_T *) USB_DM_TASK_FINISHED_EN_FUNC},
	{"EN", USB_DISK_SCAN_FAIL_EVENT,                (MAIL_INFO_T *) USB_DISK_SCAN_FAIL_EN_FUNC},
	{"EN", USB_DISK_EJECTED_FAIL_EVENT,             (MAIL_INFO_T *) USB_DISK_EJECTED_FAIL_EN_FUNC},
	{"EN", USB_DISK_PARTITION_FULL_EVENT,           (MAIL_INFO_T *) USB_DISK_PARTITION_FULL_EN_FUNC},
	{"EN", USB_DISK_FULL_EVENT,                     (MAIL_INFO_T *) USB_DISK_FULL_EN_FUNC},
	/* The End */
	{"",0,NULL}
};


/* mailutility.cc */
static void  erase_symbol(char *old, char *sym);
static void  get_hostname_from_NMP(char *mac, char *hostname);
static void  print_mail_list(mail_s *head);
static void  free_mail_list(mail_s *head);
static void  extract_data(const char *path, FILE *new_f);
static void  SET_TRAFFICMETER_INFO(const char *name, int unit);
static char *GET_TRAFFICMETER_INFO(const char *name, int unit);



#endif

