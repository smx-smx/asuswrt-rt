
#ifndef __nt_eInfo_h__
#define __nt_eInfo_h__

/* NOTIFY CENTER EVENT MAPPING INFO STRUCTURE
---------------------------------*/
struct eInfo {
	char    *eName;
	int      value;
	int      eType;
};
enum {
	TYPE_OF_RSV=0,
	TYPE_OF_IMPORTANT,
	TYPE_OF_NOTICE,
	TYPE_OF_TOTAL
};

struct eInfo mapInfo[] =
{
	/* RESERVATION EVENT */
	{"RESERVATION_MAIL_REPORT_EVENT"            ,RESERVATION_MAIL_REPORT_EVENT              ,TYPE_OF_RSV},
	{"RESERVATION_MAIL_CONFIRM_EVENT"           ,RESERVATION_MAIL_CONFIRM_EVENT             ,TYPE_OF_RSV},
	/* ------------------------------
	   ### System ###
	---------------------------------*/
	/* WAN EVENT */
	{"SYS_WAN_DISCONN_EVENT"                   ,SYS_WAN_DISCONN_EVENT                       ,TYPE_OF_NOTICE},
	{"SYS_WAN_BLOCK_EVENT"                     ,SYS_WAN_BLOCK_EVENT                         ,TYPE_OF_NOTICE},
	/* PASSWORD EVENT */
	{"SYS_PASSWORD_SAME_WITH_LOGIN_WIFI_EVENT" ,SYS_PASSWORD_SAME_WITH_LOGIN_WIFI_EVENT     ,TYPE_OF_IMPORTANT},
	{"SYS_PASSWORD_WIFI_WEAK_EVENT"            ,SYS_PASSWORD_WIFI_WEAK_EVENT                ,TYPE_OF_IMPORTANT},
	{"SYS_PASSWORD_LOGIN_STRENGTH_CHECK_EVENT" ,SYS_PASSWORD_LOGIN_STRENGTH_CHECK_EVENT     ,TYPE_OF_IMPORTANT},
	/* GUEST NETWORK EVENT */
	{"SYS_GUESTWIFI_ONE_ENABLE_EVENT"          ,SYS_GUESTWIFI_ONE_ENABLE_EVENT              ,TYPE_OF_NOTICE},
	{"SYS_GUESTWIFI_MORE_ENABLE_EVENT"         ,SYS_GUESTWIFI_MORE_ENABLE_EVENT             ,TYPE_OF_NOTICE},
	/* RSSI EVENT */
	{"SYS_RSSI_LOW_SIGNAL_EVENT"               ,SYS_RSSI_LOW_SIGNAL_EVENT                   ,TYPE_OF_NOTICE},
	{"SYS_RSSI_LOW_SIGNAL_AGAIN_EVENT"         ,SYS_RSSI_LOW_SIGNAL_AGAIN_EVENT             ,TYPE_OF_NOTICE},
	/* DUALWAN EVENT */
	{"SYS_DUALWAN_FAILOVER_EVENT"              ,SYS_DUALWAN_FAILOVER_EVENT                  ,TYPE_OF_NOTICE},
	{"SYS_DUALWAN_FAILBACK_EVENT"              ,SYS_DUALWAN_FAILBACK_EVENT                  ,TYPE_OF_NOTICE},
	/* SYS DETECT EVENT */
	{"SYS_SCAN_DLNA_PLAYER_EVENT"              ,SYS_SCAN_DLNA_PLAYER_EVENT                  ,TYPE_OF_NOTICE},
	{"SYS_DETECT_ASUS_SSID_UNENCRYPT_EVENT"    ,SYS_DETECT_ASUS_SSID_UNENCRYPT_EVENT        ,TYPE_OF_NOTICE},
	{"SYS_ECO_MODE_EVENT"                      ,SYS_ECO_MODE_EVENT                          ,TYPE_OF_NOTICE},
	{"SYS_GAME_MODE_EVENT"                     ,SYS_GAME_MODE_EVENT                         ,TYPE_OF_NOTICE},
	/* FIRMWARE EVENT */
	{"SYS_FW_NWE_VERSION_AVAILABLE_EVENT"      ,SYS_FW_NWE_VERSION_AVAILABLE_EVENT          ,TYPE_OF_IMPORTANT},
	{"SYS_NEW_SIGNATURE_UPDATED_EVENT"         ,SYS_NEW_SIGNATURE_UPDATED_EVENT             ,TYPE_OF_NOTICE},
	/* ------------------------------
	   ### Administration ###
	---------------------------------*/
	/* LOGIN EVENT */
	{"ADMIN_LOGIN_FAIL_LAN_WEB_EVENT"          ,ADMIN_LOGIN_FAIL_LAN_WEB_EVENT              ,TYPE_OF_NOTICE},
	{"ADMIN_LOGIN_FAIL_WAN_WEB_EVENT"          ,ADMIN_LOGIN_FAIL_WAN_WEB_EVENT              ,TYPE_OF_NOTICE},
	{"ADMIN_LOGIN_FAIL_SSH_EVENT"              ,ADMIN_LOGIN_FAIL_SSH_EVENT                  ,TYPE_OF_NOTICE},
	{"ADMIN_LOGIN_FAIL_TELNET_EVENT"           ,ADMIN_LOGIN_FAIL_TELNET_EVENT               ,TYPE_OF_NOTICE},
	{"ADMIN_LOGIN_FAIL_SSID_EVENT"             ,ADMIN_LOGIN_FAIL_SSID_EVENT                 ,TYPE_OF_NOTICE},
	{"ADMIN_LOGIN_FAIL_AICLOUD_EVENT"          ,ADMIN_LOGIN_FAIL_AICLOUD_EVENT              ,TYPE_OF_NOTICE},
	{"ADMIN_LOGIN_DEVICE_DOUBLE_EVENT"         ,ADMIN_LOGIN_DEVICE_DOUBLE_EVENT             ,TYPE_OF_NOTICE},
	{"ADMIN_LOGIN_ACCOUNT_DOBLE_EVENT"         ,ADMIN_LOGIN_ACCOUNT_DOBLE_EVENT             ,TYPE_OF_NOTICE},
	{"ADMIN_LOGIN_FAIL_VPNSERVER_EVENT"        ,ADMIN_LOGIN_FAIL_VPNSERVER_EVENT            ,TYPE_OF_NOTICE},
	/* ------------------------------
	   ### Security ###
	---------------------------------*/
	/* PROTECTION EVENT */
	{"PROTECTION_INTO_MONITORMODE_EVENT"       ,PROTECTION_INTO_MONITORMODE_EVENT           ,TYPE_OF_NOTICE},
	{"PROTECTION_VULNERABILITY_EVENT"          ,PROTECTION_VULNERABILITY_EVENT              ,TYPE_OF_NOTICE},
	{"PROTECTION_CC_EVENT"                     ,PROTECTION_CC_EVENT                         ,TYPE_OF_NOTICE},
	{"PROTECTION_DOS_EVENT"                    ,PROTECTION_DOS_EVENT                        ,TYPE_OF_NOTICE},
	{"PROTECTION_SAMBA_GUEST_ENABLE_EVENT"     ,PROTECTION_SAMBA_GUEST_ENABLE_EVENT         ,TYPE_OF_IMPORTANT},
	{"PROTECTION_FTP_GUEST_ENABLE_EVENT"       ,PROTECTION_FTP_GUEST_ENABLE_EVENT           ,TYPE_OF_IMPORTANT},
	{"PROTECTION_FIREWALL_DISABLE_EVENT"       ,PROTECTION_FIREWALL_DISABLE_EVENT           ,TYPE_OF_IMPORTANT},
	/* ------------------------------
	   ### Parental Contorl ###
	---------------------------------*/
	/* PARENTAL CONTROL EVENT */
	{"PARENTALCTL_BLOCK_EVENT"                 ,PARENTALCTL_BLOCK_EVENT                     ,TYPE_OF_NOTICE},
	/* ------------------------------
	   ### Traffic Management ###
	---------------------------------*/
	/* TRAFFIC METER EVENT */
	{"TRAFFICMETER_CUTOFF_EVENT"               ,TRAFFICMETER_CUTOFF_EVENT                   ,TYPE_OF_NOTICE},
	{"TRAFFICMETER_ALERT_EVENT"                ,TRAFFICMETER_ALERT_EVENT                    ,TYPE_OF_NOTICE},
	{"TRAFFICMETER_BW_LIMITER_EVENT"           ,TRAFFICMETER_BW_LIMITER_EVENT               ,TYPE_OF_NOTICE},
	/* ------------------------------
	   ### USB Function ###
	---------------------------------*/
	/* USB EVENT */
	{"USB_DM_TASK_FINISHED_EVENT"               ,USB_DM_TASK_FINISHED_EVENT                 ,TYPE_OF_NOTICE},
	{"USB_DISK_SCAN_FAIL_EVENT"                 ,USB_DISK_SCAN_FAIL_EVENT                   ,TYPE_OF_NOTICE},
	{"USB_DISK_EJECTED_FAIL_EVENT"              ,USB_DISK_EJECTED_FAIL_EVENT                ,TYPE_OF_NOTICE},
	{"USB_DISK_PARTITION_FULL_EVENT"            ,USB_DISK_PARTITION_FULL_EVENT              ,TYPE_OF_NOTICE},
	{"USB_DISK_FULL_EVENT"                      ,USB_DISK_FULL_EVENT                        ,TYPE_OF_NOTICE},
	/* ------------------------------
	   ### Hint Item ###
	---------------------------------*/
	/* HINT EVENT */
	{"HINT_USB_CHECK_EVENT"                     ,HINT_USB_CHECK_EVENT                       ,TYPE_OF_NOTICE},
	{"HINT_USB_FIRSTIME_CHECK_EVENT"            ,HINT_USB_FIRSTIME_CHECK_EVENT              ,TYPE_OF_NOTICE},
	{"HINT_SAMBA_INLAN_EVENT"                   ,HINT_SAMBA_INLAN_EVENT                     ,TYPE_OF_NOTICE},
	{"HINT_OSX_INLAN_EVENT"                     ,HINT_OSX_INLAN_EVENT                       ,TYPE_OF_NOTICE},
	{"HINT_DUALBAND_WITHOUT_5G_EVENT"           ,HINT_DUALBAND_WITHOUT_5G_EVENT             ,TYPE_OF_NOTICE},
	{"HINT_USB3_WITHOUT_ENHANCE_EVENT"          ,HINT_USB3_WITHOUT_ENHANCE_EVENT            ,TYPE_OF_NOTICE},
	{"HINT_XBOX_PS_EVENT"                       ,HINT_XBOX_PS_EVENT                         ,TYPE_OF_NOTICE},
	{"HINT_UPNP_RENDERER_EVENT"                 ,HINT_UPNP_RENDERER_EVENT                   ,TYPE_OF_NOTICE},
	{"HINT_DETECT_GAME_SERVICE_EVENT"           ,HINT_DETECT_GAME_SERVICE_EVENT             ,TYPE_OF_NOTICE},
	{"HINT_SUGGEST_ENABLE_DFS_CHANNEL_EVENT"    ,HINT_SUGGEST_ENABLE_DFS_CHANNEL_EVENT      ,TYPE_OF_NOTICE},
	/* The End */
	{0,0,0}
};

#endif
