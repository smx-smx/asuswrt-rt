 /*
 * Copyright 2015, ASUSTeK Inc.
 * All Rights Reserved.
 *
 * THIS SOFTWARE IS OFFERED "AS IS", AND ASUS GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 */

#ifndef __nt_common_h__
#define __nt_common_h__


/* SOCKET SERVER DEFINE SETTING 
---------------------------------*/
#define NOTIFY_CENTER_SOCKET_PATH               "/etc/notify_center_socket"
#define NOTIFY_MAIL_SERVICE_SOCKET_PATH         "/etc/notify_mail_socket"

#define MAX_NOTIFY_SOCKET_CLIENT    5

#define NOTIFY_CENTER_PID_PATH                  "/tmp/Notification_Center.pid"
#define NOTIFY_CENTER_MONITOR_PID_PATH          "/tmp/Notification_Center_Monitor.pid"
#define NOTIFY_ACTION_MAIL_PID_PATH             "/tmp/Notification_Center_ActMail.pid"

#define NOTIFY_CENTER_LOG_FILE                  "/tmp/NTC.log"
#define NOTIFY_CENTER_MONITOR_LOG_FILE          "/tmp/NTM.log"
#define NOTIFY_ACTION_MAIL_LOG_FILE             "/tmp/NAM.log"

#define NOTIFY_CENTER_DEBUG                     "/tmp/NTC_DEBUG"
#define NOTIFY_CENTER_MONITOR_DEBUG             "/tmp/NTM_DEBUG"
#define NOTIFY_ACTION_MAIL_DEBUG                "/tmp/NAM_DEBUG"

#define NOTIFY_CENTER_TEMP_DIR                  "/tmp/nc"
#define EVENTID_SCRIPT_DEFINE_PATH              "/tmp/nc/event.conf"

/* NOTIFICATION DATABASE DEFINE SETTING 
----------------------------------------*/
#define TYPE_SHIFT              16
#define NOTIFY_DB_QLEN          200
#define NOTIFY_DB_DEBUG         "/tmp/NTD_DEBUG"

#ifdef ASUSWRT_SDK /* ASUSWRT SDK */

#include <rtconfig.h>

#if (defined(RTCONFIG_JFFS2) || defined(RTCONFIG_BRCM_NAND_JFFS2) || defined(RTCONFIG_UBIFS))
#define NOTIFY_DB_FOLDER        "/jffs/nc/"
#else
#define NOTIFY_DB_FOLDER        "/tmp/nc/"
#endif

#else /* DSL_ASUSWRT_SDK */

#if defined(TCSUPPORT_ADD_JFFS) || defined(TCSUPPORT_SQUASHFS_ADD_YAFFS)
#define NOTIFY_DB_FOLDER        "/jffs/nc/"
#else
#define NOTIFY_DB_FOLDER        "/tmp/nc/"
#endif

#endif

/* ACTION SERVICE EVENT DEFINE 
---------------------------------*/
#define ACTION_NOTIFY_WEBUI     0x01
#define ACTION_NOTIFY_EMAIL     0x02
#define ACTION_NOTIFY_APP       0x04
#define ACTION_NOTIFY_WEEKLY    0x08


#define MAX_NOTIFY_EVENT_NUM    128

/* NOTIFY CLIENT EVENT DEFINE 
---------------------------------*/
/* RESERVATION EVENT */
#define RESERVATION_MAIL_REPORT_EVENT              0xF000           /* Used in email daemon status report */
#define RESERVATION_MAIL_CONFIRM_EVENT             0xF001
/* ------------------------------
    ### System ###
---------------------------------*/
/* WAN EVENT */
#define SYS_WAN_DISCONN_EVENT                      0x10001
#define SYS_WAN_BLOCK_EVENT                        0x10002
/* PASSWORD EVENT */
#define SYS_PASSWORD_SAME_WITH_LOGIN_WIFI_EVENT    0x10003
#define SYS_PASSWORD_WIFI_WEAK_EVENT               0x10004
#define SYS_PASSWORD_LOGIN_STRENGTH_CHECK_EVENT    0x10005
/* GUEST NETWORK EVENT */
#define SYS_GUESTWIFI_ONE_ENABLE_EVENT             0x10006
#define SYS_GUESTWIFI_MORE_ENABLE_EVENT            0x10007
/* RSSI EVENT */
#define SYS_RSSI_LOW_SIGNAL_EVENT                  0x10008
#define SYS_RSSI_LOW_SIGNAL_AGAIN_EVENT            0x10009
/* DUALWAN EVENT */
#define SYS_DUALWAN_FAILOVER_EVENT                 0x1000A
#define SYS_DUALWAN_FAILBACK_EVENT                 0x1000B
/* SYS DETECT EVENT */
#define SYS_SCAN_DLNA_PLAYER_EVENT                 0x1000C
#define SYS_DETECT_ASUS_SSID_UNENCRYPT_EVENT       0x1000D
#define SYS_ECO_MODE_EVENT                         0x1000E
#define SYS_GAME_MODE_EVENT                        0x1000F
/* FIRMWARE EVENT */
#define SYS_FW_NWE_VERSION_AVAILABLE_EVENT         0x10010
#define SYS_NEW_SIGNATURE_UPDATED_EVENT            0x10011
/* ------------------------------
    ### Administration ###
---------------------------------*/
/* LOGIN EVENT */
#define ADMIN_LOGIN_FAIL_LAN_WEB_EVENT             0x20001
#define ADMIN_LOGIN_FAIL_WAN_WEB_EVENT             0x20002
#define ADMIN_LOGIN_FAIL_SSH_EVENT                 0x20003
#define ADMIN_LOGIN_FAIL_TELNET_EVENT              0x20004
#define ADMIN_LOGIN_FAIL_SSID_EVENT                0x20005
#define ADMIN_LOGIN_FAIL_AICLOUD_EVENT             0x20006
#define ADMIN_LOGIN_DEVICE_DOUBLE_EVENT            0x20007
#define ADMIN_LOGIN_ACCOUNT_DOBLE_EVENT            0x20008
#define ADMIN_LOGIN_FAIL_VPNSERVER_EVENT           0x20009
/* ------------------------------
    ### Security ###
---------------------------------*/
/* PROTECTION EVENT */
#define PROTECTION_INTO_MONITORMODE_EVENT          0x30001
#define PROTECTION_VULNERABILITY_EVENT             0x30002
#define PROTECTION_CC_EVENT                        0x30003
#define PROTECTION_DOS_EVENT                       0x30004
#define PROTECTION_SAMBA_GUEST_ENABLE_EVENT        0x30005
#define PROTECTION_FTP_GUEST_ENABLE_EVENT          0x30006
#define PROTECTION_FIREWALL_DISABLE_EVENT          0x30007
/* ------------------------------
    ### Parental Contorl ###
---------------------------------*/
/* PARENTAL CONTROL EVENT */
#define PARENTALCTL_BLOCK_EVENT                    0x40001
/* ------------------------------
    ### Traffic Management ###
---------------------------------*/
/* TRAFFIC METER EVENT */
#define TRAFFICMETER_CUTOFF_EVENT                  0x50001
#define TRAFFICMETER_ALERT_EVENT                   0x50002
#define TRAFFICMETER_BW_LIMITER_EVENT              0x50003
/* ------------------------------
    ### USB Function ###
---------------------------------*/
/* USB EVENT */
#define USB_DM_TASK_FINISHED_EVENT                 0x60001
#define USB_DISK_SCAN_FAIL_EVENT                   0x60002
#define USB_DISK_EJECTED_FAIL_EVENT                0x60003
#define USB_DISK_PARTITION_FULL_EVENT              0x60004
#define USB_DISK_FULL_EVENT                        0x60005

/* ------------------------------
    ### Hint Item ###
---------------------------------*/
/* HINT EVENT */
#define HINT_USB_CHECK_EVENT                       0x10000001
#define HINT_USB_FIRSTIME_CHECK_EVENT              0x10000002
#define HINT_SAMBA_INLAN_EVENT                     0x10000003
#define HINT_OSX_INLAN_EVENT                       0x10000004
#define HINT_DUALBAND_WITHOUT_5G_EVENT             0x10000005
#define HINT_USB3_WITHOUT_ENHANCE_EVENT            0x10000006
#define HINT_XBOX_PS_EVENT                         0x10000007
#define HINT_UPNP_RENDERER_EVENT                   0x10000008
#define HINT_DETECT_GAME_SERVICE_EVENT             0x10000009
#define HINT_SUGGEST_ENABLE_DFS_CHANNEL_EVENT      0x1000000A

/* SEND MAIL STATUS
---------------------------------*/
typedef enum {
	MAIL_WAIT = 0,
	MAIL_SENDING,
	MAIL_SUCCESS,
	MAIL_FAILED,
	MAIL_FATAL_ERROR                /* E-Mail Config error */
}MAIL_STATUS_T;

/* ACTION MAIL EVENT STRUCTURE
---------------------------------*/
typedef struct __act_mail_event__t_
{
	int             MsendId;        /* Random ID to record send mail index */
	int             Mretry;         /* Record retry times when send mail fail */
	time_t          MsendTime;      /* Record send time */
	MAIL_STATUS_T   MsendStatus;    /* Record send mail status */
}ACTMAIL_EVENT_T;

/* NOTIFY CLIENT EVENT STRUCTURE
---------------------------------*/
typedef struct __notify_event__t_
{
	int             event;          /* Refer NOTIFY CLIENT EVENT DEFINE */
	char            msg[100];       /* Info */

/*  ### NT_CENTER HANDLER Info ### */
	time_t          tstamp;         /* Receive time */
	ACTMAIL_EVENT_T mail_t;         /* Action Mail Data Info*/

}NOTIFY_EVENT_T;

/* NOTIFY DATABASE STRUCTURE */
typedef struct __notify_database__t_
{
	time_t          tstamp;         /* Receive time */
	int             event;          /* Refer NOTIFY CLIENT EVENT DEFINE */
	int             flag;           /* Refer ACTION SERVICE EVENT DEFINE */
	int             status;         /* Status for read or not */
	char            msg[100];       /* Info */

}NOTIFY_DATABASE_T;

#endif

