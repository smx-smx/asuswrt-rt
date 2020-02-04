#ifndef __ASP_HANDLER_H__
#define __ASP_HANDLER_H__

#define MAX_NO_MSSID	4

#ifndef _TCAPI_TUPLE
#define _TCAPI_TUPLE

struct tcapi_tuple{
	char* node;
	char* attribute;
	char* value;
	struct tcapi_tuple *next;
};


struct tcapi_tuple wlan_system_router_defaults[] = {
#if 1
//for ASUS WRT.B
	//General
	{"WLan_Entry", "wl_wep_type", "0"},
	{"WLan_Entry", "wl_phrase", ""},
	//Bridge
	{"WLan_WDS", "wl_ap_mode", "0"},
	{"WLan_WDS", "wl_wdsapply", "0"},
	{"WLan_WDS", "wl_wdslist", ""},
	//Wireless MAC Filter(ACL)
	{"ACL_Entry", "wl_macmode", "disabled"},
	{"ACL_Entry", "wl_maclist", ""},
	//Wireless - WPS
	{"WLan_Common", "wps_enable","1"},
	//Wireless - Professional
	{"WLan_Entry", "radio_on","1"},
	{"WLan_Entry", "radio_flag","1"},
	{"WLan_Entry", "wl_schedule_radio","1"},
	{"WLan_Entry", "wl_radio_date","1111111"},
	{"WLan_Entry", "wl_radio_time","00002359"},
	{ "WLan_Common", "wl_wme_no_ack", "off"},		/* WME No-Acknowledgment mode */
	{"WLan_Entry", "wl_sched0", ""},
	{"WLan_Entry", "wl_sched1", ""},
	{"WLan_Entry", "wl_user_rssi", "0"},
	{NULL, NULL, NULL }
//for ASUS WRT.E
#else
//for RT-65168.B
	{"WLan_Common", "APOn", "0"},
	{"Info_WLan","CurrentChannel", ""},
	{"Info_WLan","maxStaNumSupported", ""},
	{"WLan_Entry","MaxStaNum", "0"},
	{"WebCurSet_Entry", "wlan_id", "0"},
	{"GUI_Entry0", "ssidOneOnly", ""},
	{"WLan_Entry", "WPSConfMode", "0"},   //SSID Settings -> Use WPS
	{"Info_WLan", "isDot1XSupported", ""},
	{"Info_WLan", "isbharti", ""},
	{"WebCustom_Entry", "isWlanEncrypt", ""},
	{"Info_WLan","isAuthenTypeSupported", ""},
	{"WebCustom_Entry","isWPSSupported", ""},
	{"WLan_Entry", "WPSConfStatus", ""},
	{"WLan_Entry","WPSMode", "0"},
	//WPS Settings->AP self PIN code
	{"Info_WLan", "wlanSelfPinCode", ""},
	{"Info_WLan","WPSGenPinCode", "0"},
	//WPS Settings->enrollee PIN code
	{"WLan_Entry","enrolleePinCode", ""},
	{"Info_WLan","wlanWPStimerRunning", "0"},
	//WPS Settings->WPS progress
	{"Info_WLan", "wlanWPSStatus", ""},
	{"Info_WLan","WPSOOBActive", "0"},
	//Backup RADIUS
	{"Info_WLan","isDot1XEnhanceSupported", ""},
	{"WLan_Entry","BAK_RADIUS_Server", ""},
	{"WLan_Entry","BAK_RADIUS_Port", ""},
	{"WLan_Entry","BAK_RADIUS_Key", ""},
	{"WLan_Common","session_timeout_interval", ""},	//RADIUS Re-auth Interval
	//WEP AuthType
	{"WLan_Entry","WEPAuthType", ""},
	//WDS
	{"Info_WLan","isWDSSupported", ""},
	{"WLan_WDS", "Wds_MAC0", ""},
	{"WLan_WDS", "Wds_MAC1", ""},
	{"WLan_WDS", "Wds_MAC2", ""},
	{"WLan_WDS", "Wds_MAC3", ""},
	//Wireless MAC Address Filter
	{"WLan_Entry","WLan_MAC0", ""},
	{"WLan_Entry","WLan_MAC1", ""},
	{"WLan_Entry","WLan_MAC2", ""},
	{"WLan_Entry","WLan_MAC3", ""},
	{"WLan_Entry","WLan_MAC4", ""},
	{"WLan_Entry","WLan_MAC5", ""},
	{"WLan_Entry","WLan_MAC6", ""},
	{"WLan_Entry","WLan_MAC7", ""},
	{NULL, NULL, NULL }
//for RT-65168.E
#endif
};

#if defined(TCSUPPORT_WLAN_RT6856)

// stub for wlconf, etc.
struct tcapi_tuple wlan_configuration_router_defaults[] = {
//	{"WLan_Common", "wl_unit", "0"},
	{"WLan_Common", "CountryRegion", "0"},
	{"WLan_Common", "CountryRegionABand", "3"},
	{"WLan_Common", "CountryCode", "TW"},
	{"WLan_Common", "BssidNum", "1"},
	{"WLan_Entry", "ssid", "RT6856_2.4G_iNIC_jaja"},	//for match the element name in wlan_MBSSID_system_defaults
	{"WLan_Common", "WirelessMode", "1"},
	{"WLan_Entry", "TxRate", "0"},
	{"WLan_Entry", "LedOff", "0"},
	{"WLan_Common", "Channel", "1"},
	{"WLan_Common", "BasicRate", "15"},
	{"WLan_Common", "BeaconPeriod", "100"},
	{"WLan_Common", "DtimPeriod", "1"},
	{"WLan_Common", "TxPower", "100"},
	{"WLan_Entry", "DisableOLBC", "0"},
	{"WLan_Common", "BGProtection", "0"},
	{"WLan_Entry", "TxAntenna", ""}, /* Paul modify 2013/3/11, TxAntenna & RxAntenna default should be empty, otherwise will cause WiFi connectivity issue. */
	{"WLan_Entry", "RxAntenna", ""},
	{"WLan_Common", "TxPreamble", "0"},
	{"WLan_Common", "RTSThreshold", "2347"},
	{"WLan_Common", "FragThreshold", "2346"},
	{"WLan_Common", "TxBurst", "1"},
	{"WLan_Common", "EDCCA_AP_STA_TH", "255"},
	{"WLan_Common", "EDCCA_AP_AP_TH", "255"},
	{"WLan_Common", "EDCCA_FALSE_CCA_TH", "3000"},
	{"WLan_Common", "EDCCA_ED_TH", "90"},
	{"WLan_Common", "EDCCA_BLOCK_CHECK_TH", "2"},
	{"WLan_Common", "EDCCA_AP_RSSI_TH", "-80"},
	{"WLan_Common", "PktAggregate", "0"},
	{"WLan_Common", "TurboRate", "0"},
	{"WLan_Entry", "WmmCapable", "0"},
	{"WLan_Common", "APSDCapable", "0"},
	{"WLan_Common", "DLSCapable", "0"},
	{"WLan_Common", "APAifsn", "3;7;1;1"},
	{"WLan_Common", "APCwmin", "4;4;3;2"},
	{"WLan_Common", "APCwmax", "6;10;4;3"},
	{"WLan_Common", "APTxop", "0;0;94;47"},
	{"WLan_Common", "APACM", "0;0;0;0"},
	{"WLan_Common", "BSSAifsn", "3;7;2;2"},
	{"WLan_Common", "BSSCwmin", "4;4;3;2"},
	{"WLan_Common", "BSSCwmax", "10;10;4;3"},
	{"WLan_Common", "BSSTxop", "0;0;94;47"},
	{"WLan_Common", "BSSACM", "0;0;0;0"},
	{"WLan_Common", "AckPolicy", "0;0;0;0"},
	//write NoForwarding and NoForwardingBTNBSSID based on wl_ap_isolate 
	//{"WLan_Entry", "NoForwarding", "0"},
	//{"WLan_Common", "NoForwardingBTNBSSID", "0"},
	{"WLan_Common", "wl_ap_isolate", "0"},
	{"WLan_Entry", "HideSSID", "0"},
	{"WLan_Common", "ShortSlot", "1"},
	{"WLan_Common", "AutoChannelSelect", "0"},
	{"WLan_Common", "AutoChannelSkipList", ""},
	{"WLan_Common", "HT_BSSCoexistence", "1"},
	{"WLan_Entry", "IEEE8021X", "0"},
	{"WLan_Common", "WirelessEvent", "0"},
	{"WLan_Entry", "PreAuth", "0"},
	{"WLan_Entry", "auth_mode_x", "OPEN"},  //Authentication Type		//for match the element name in wlan_MBSSID_system_defaults
	{"WLan_Entry", "crypto", "NONE"},	//for match the element name in wlan_MBSSID_system_defaults
	{"WLan_Entry", "wep_x", "0"},
	{"WLan_Entry", "RekeyInterval", "0"},
	{"WLan_Common", "RekeyMethod", "DISABLE"},
	{"WLan_Common", "PMKCachePeriod", "10"},
	{"WLan_Entry", "wpa_psk", ""},	//for match the element name in wlan_MBSSID_system_defaults
	{"WLan_Entry", "key", "1"},	//DefaultKeyID		//for match the element name in wlan_MBSSID_system_defaults
	{"WLan_Entry", "key1", ""},
	{"WLan_Entry", "key2", ""},
	{"WLan_Entry", "key3", ""},
	{"WLan_Entry", "key4", ""},
	{"WLan_Entry", "phrase_x", ""},
	/* Key1Type and Key1Str1 ~ Key4Type and Key4Str4 will be generated by write_MBSSID_profile() */
	{"WLan_Entry", "HSCounter", "0"},
	{"WLan_Entry", "AccessPolicy", "0"}, 
	/* AccessPolicy0 and AccessControlList0 ~ AccessPolicy3 and AccessControlList3 will be generated by write_wlan_profile() */
	{"WLan_WDS", "WdsEnable", "0"},
	/*
	{"WLan_WDS", "WdsEncrypType", "NONE"},
	{"WLan_WDS", "WdsList", ""},
	{"WLan_WDS", "WdsKey", ""},
	{"WLan_WDS", "Wds0Key", ""},
	{"WLan_WDS", "WdsDefaultKeyID", "1"},
	*/
	{"WLan_Entry", "bw_enabled", "0"}, /* Bandwidth Limiter */
	{"WLan_Entry", "bw_dl", ""},       /* Bandwidth Limiter */
	{"WLan_Entry", "bw_ul", ""},       /* Bandwidth Limiter */
	{"WLan_Entry", "RADIUS_Server", ""},
	{"WLan_Entry", "RADIUS_Port", "1812"},
//	{"WLan_Entry", "RADIUS_Key", "ralink"},	//parameter name in the Web GUI.
	{"WLan_Entry", "RADIUS_Key1", ""},
//	{"WLan_Entry", "RADIUS_Key2", ""},
//	{"WLan_Entry", "RADIUS_Key3", ""},
//	{"WLan_Entry", "RADIUS_Key4", ""},
	{"WLan_Entry", "own_ip_addr", "192.168.177.123"},	//get the value from "tcapi get Lan_Entry IP"	
	{"WLan_Common", "HT_RDG", "1"},
	{"WLan_Common", "HT_EXTCHA", "0"},
	{"WLan_Common", "HT_LinkAdapt", "0"},
	{"WLan_Common", "HT_OpMode", "0"},
	{"WLan_Common", "HT_MpduDensity", "5"},
	{"WLan_Common", "HT_BW", "1"},
	{"WLan_Common", "HT_AutoBA", "1"},
	{"WLan_Common", "HT_AMSDU", "0"},
	{"WLan_Common", "HT_BAWinSize", "64"},
	{"WLan_Common", "HT_GI", "1"},
	{"WLan_Entry", "HT_MCS", "33"},
	{"WLan_Entry", "ExtEEPROM", "0"},
	{"WLan_Entry", "Bss2040Coexist", "0"},
	{"WLan_Common", "HighPower", "1"},	//enhance RX throughput, Javier.20120816
	{"WLan_Common", "MacAddress", "00:11:22:33:44:00"},
	//The following parameters are added by referring to RT-N65U.
	{"WLan_Common", "CarrierDetect", "0"},
	{"WLan_Common", "GreenAP", "0"},
	{"WLan_Common", "HT_HTC", "1"},
	{"WLan_Common", "HT_BADecline", "0"},
	{"WLan_Common", "HT_STBC", "1"},
	{"WLan_Common", "HT_PROTECT", "1"},
	{"WLan_Common", "HT_DisallowTKIP", "1"},
	{"WLan_Common", "WiFiTest", "0"},
	{"WLan_Common", "TGnWifiTest", "0"},
	{"WLan_Common", "IgmpSnEnable", "0"},
	//{"WLan_Common", "McastPhyMode", "3"},	//handled by wl_mrate_x
	//{"WLan_Common", "McastMcs", "7"},	//handled by wl_mrate_x
	{"WLan_Common", "wl_mrate_x", "0"},
	{"WLan_Common", "location_code", ""},
	{"WLan_Entry", "WscVendorPinCode", "24705561"},
	{"WLan_Entry", "WscManufacturer", "ASUSTeK Computer Inc."},
	{"WLan_Entry", "WscModelName", "WPS Router"},
	{"WLan_Entry", "WscDeviceName", "ASUS WPS Router"},
	{"WLan_Entry", "WscModelNumber", "DSL-N66U"},
	{"WLan_Entry", "WscSerialNumber", "00000000"},
	{"WLan_Entry", "WscV2Support", "1"},
	{"WLan_Entry", "RadioOn", "1"},
//	{"WLan_Common", "IgmpAdd", "01:00:5e:7f:ff:fa"},
	{"WLan_Common", "HT_TxStream", "3"},
	{"WLan_Common", "HT_RxStream", "3"},
	{"WLan_Common", "PhyRateLimit", "0"},
	{"WLan_Entry", "DBG_LEVEL", "0"},
	//The following parameters are dedicated for 5G.
	{"WLan_Entry", "RDRegion", "CE"},
	{"WLan_Common", "IEEE80211H", "1"},
	{"WLan_Entry", "CSPeriod", "5"},
	{"WLan_Entry", "StationKeepAlive", "0"},
	{"WLan_Entry", "DfsLowerLimit", "0"},
	{"WLan_Entry", "DfsUpperLimit", "0"},
	{"WLan_Entry", "DfsIndoor", "0"},
	{"WLan_Entry", "DFSParamFromConfig", "0"},
	{"WLan_Entry", "FCCParamCh0", ""},
	{"WLan_Entry", "FCCParamCh1", ""},
	{"WLan_Entry", "FCCParamCh2", ""},
	{"WLan_Entry", "FCCParamCh3", ""},
	{"WLan_Entry", "CEParamCh0", ""},
	{"WLan_Entry", "CEParamCh1", ""},
	{"WLan_Entry", "CEParamCh2", ""},
	{"WLan_Entry", "CEParamCh3", ""},
	{"WLan_Entry", "JAPParamCh0", ""},
	{"WLan_Entry", "JAPParamCh1", ""},
	{"WLan_Entry", "JAPParamCh2", ""},
	{"WLan_Entry", "JAPParamCh3", ""},
	{"WLan_Entry", "JAPW53ParamCh0", ""},
	{"WLan_Entry", "JAPW53ParamCh1", ""},
	{"WLan_Entry", "JAPW53ParamCh2", ""},
	{"WLan_Entry", "JAPW53ParamCh3", ""},
	{"WLan_Entry", "FixDfsLimit", "0"},
	{"WLan_Entry", "LongPulseRadarTh", "0"},
	{"WLan_Entry", "AvgRssiReq", "0"},
	{"WLan_Entry", "DFS_R66", "0"},
	{"WLan_Entry", "BlockCh", ""},
#if 0		
	{ "wl_subunit", 	"-1"	},
	{ "wl_vifnames", 	""	},	/* Virtual Interface Names */
	/* Wireless parameters */
	{ "wl_version", EPI_VERSION_STR, 0 },	/* OS revision */

	{ "wl_ifname", "" },			/* Interface name */
	{ "wl_hwaddr", "" },			/* MAC address */
	{ "wl_phytype", "b" },			/* Current wireless band ("a" (5 GHz),
						 * "b" (2.4 GHz), or "g" (2.4 GHz))
						 */
	{ "wl_corerev", "" },			/* Current core revision */
	{ "wl_phytypes", "" },			/* List of supported wireless bands (e.g. "ga") */
	{ "wl_radioids", "" },			/* List of radio IDs */
	{ "wl_ssid", "ASUS" },			/* Service set ID (network name) */
	{ "wl1_ssid", "ASUS_5G" },
	{ "wl_bss_enabled", "1"},		/* Service set ID (network name) */
						/* See "default_get" below. */
//	{ "wl_country_code", ""},		/* Country Code (default obtained from driver) */
	{ "wl_radio", "1"},			/* Enable (1) or disable (0) radio */
	{ "wl_closed", "0"},			/* Closed (hidden) network */
	{ "wl_ap_isolate", "0"},		/* AP isolate mode */
#ifndef RTCONFIG_RALINK
	{ "wl_wmf_bss_enable", "0"},		/* WMF Enable/Disable */
	{ "wl_mcast_regen_bss_enable", "1"},	/* MCAST REGEN Enable/Disable */
	{ "wl_rxchain_pwrsave_enable", "1"},	/* Rxchain powersave enable */
	{ "wl_rxchain_pwrsave_quiet_time", "1800"},	/* Quiet time for power save */
	{ "wl_rxchain_pwrsave_pps", "10"},	/* Packets per second threshold for power save */
	{ "wl_rxchain_pwrsave_stas_assoc_check", "0"},	/* STAs associated before powersave */
	{ "wl_radio_pwrsave_enable", "0"},	/* Radio powersave enable */
	{ "wl_radio_pwrsave_quiet_time", "1800"},	/* Quiet time for power save */
	{ "wl_radio_pwrsave_pps", "10"},	/* Packets per second threshold for power save */
	{ "wl_radio_pwrsave_level", "0"},	/* Radio power save level */
	{ "wl_radio_pwrsave_stas_assoc_check", "0"},	/* STAs associated before powersave */
#endif
	{ "wl_mode", "ap"},			/* AP mode (ap|sta|wds) */
	{ "wl_lazywds", "0"},			/* Enable "lazy" WDS mode (0|1) */
	{ "wl_wds", ""},			/* xx:xx:xx:xx:xx:xx ... */
	{ "wl_wds_timeout", "1"},		/* WDS link detection interval defualt 1 sec */
	{ "wl_wep", "disabled"},		/* WEP data encryption (enabled|disabled) */
	{ "wl_auth", "0"},			/* Shared key authentication optional (0) or
						 * required (1)
						 */
	{ "wl_key", "1"},			/* Current WEP key */
	{ "wl_key1", ""},			/* 5/13 char ASCII or 10/26 char hex */
	{ "wl_key2", ""},			/* 5/13 char ASCII or 10/26 char hex */
	{ "wl_key3", ""},			/* 5/13 char ASCII or 10/26 char hex */
	{ "wl_key4", ""},			/* 5/13 char ASCII or 10/26 char hex */
	{ "wl_maclist", ""},			/* xx:xx:xx:xx:xx:xx ... */
	{ "wl_macmode", "disabled"},		/* "allow" only, "deny" only, or "disabled"
						 * (allow all)
						 */
	{ "wl_channel", "0"},			/* Channel number */
#ifdef MEDIA_REVIEW
#ifndef RTCONFIG_RALINK
	{ "wl1_channel", "48"},
#endif
#endif
#ifndef RTCONFIG_RALINK
	{ "wl_reg_mode", "off"},		/* Regulatory: 802.11H(h)/802.11D(d)/off(off) */
	{ "wl_dfs_preism", "60"},		/* 802.11H pre network CAC time */
	{ "wl_dfs_postism", "60"},		/* 802.11H In Service Monitoring CAC time */
	/* Radar thrs params format: version thresh0_20 thresh1_20 thresh0_40 thresh1_40 */
	{ "wl_radarthrs", "0 0x6a8 0x6c8 0x6ac 0x6c7"},
	{ "wl_rate", "0"},			/* Rate (bps, 0 for auto) */
#if 0
#if 0
	{ "wl_mrate", "0"},			/* Mcast Rate (bps, 0 for auto) */
#else
	{ "wl_mrate", "12000000", 0 },		/* default setting of SDK 5.110.27.0 */
#endif
#else
//	{ "wl_mrate_x", "7", 0 },		/* 12 Mbps */
	{ "wl_mrate_x", "0", 0 },
#endif
#else
	{ "wl_mrate_x", "0", 0 },		/* ralink auto rate */
#endif
#ifndef RTCONFIG_RALINK
	{ "wl_rateset", "12"},			/* "default" or "all" or "12" */
#else
	{ "wl_rateset", "default"},
#endif
	{ "wl_frag", "2346"},			/* Fragmentation threshold */
	{ "wl_rts", "2347"},			/* RTS threshold */
#ifndef RTCONFIG_RALINK
	{ "wl_dtim", "3"},			/* DTIM period */
#else
	{ "wl_dtim", "1"},
#endif
	{ "wl_bcn", "100"},			/* Beacon interval */
	{ "wl_bcn_rotate", "1"},		/* Beacon rotation */
	{ "wl_plcphdr", "long"},		/* 802.11b PLCP preamble type */
	{ "wl_gmode", XSTR(GMODE_AUTO)},	/* 54g mode */
	{ "wl_gmode_protection", "auto"},	/* 802.11g RTS/CTS protection (off|auto) */
	{ "wl_frameburst", "on"},		/* BRCM Frambursting mode (off|on) */
	{ "wl_wme", "on"},			/* WME mode (off|on|auto) */
#ifndef RTCONFIG_RALINK
	{ "wl_wme_bss_disable", "0"},		/* WME BSS disable advertising (off|on) */
	{ "wl_antdiv", "-1"},			/* Antenna Diversity (-1|0|1|3) */
//	{ "wl_interfmode", "2"},		/* Interference mitigation mode (2/3/4) */
	{ "wl_infra", "1"},			/* Network Type (BSS/IBSS) */
	{ "wl_nbw_cap", "0"},			/* BW Cap; def 20inB and 40inA */
#endif
	{ "wl_nctrlsb", "lower"},		/* N-CTRL SB */
	{ "wl_nband", "2"},
	{ "wl0_nband", "2"},
	{ "wl1_nband", "1"},
#ifndef RTCONFIG_RALINK
	{ "wl_nmcsidx", "-1"},			/* MCS Index for N - rate */
	{ "wl_nmode", "-1"},			/* N-mode */
	{ "wl_rifs_advert", "auto"},		/* RIFS mode advertisement */
	{ "wl_nreqd", "0"},			/* Require 802.11n support */
#endif
	{ "wl_nmode_protection", "auto"},	/* 802.11n Protection */
#ifndef RTCONFIG_RALINK
	{ "wl_mimo_preamble", "auto"},		/* Mimo PrEamble */
#else
	{ "wl_mimo_preamble", "mm"},
#endif
#ifndef RTCONFIG_RALINK
	{ "wl_vlan_prio_mode", "off"},		/* VLAN Priority support */
	{ "wl_leddc", "0x640000"},		/* 100% duty cycle for LED on router */
#endif
	{ "wl_rxstreams", "0"},			/* 802.11n Rx Streams, 0 is invalid, WLCONF will
						 * change it to a radio appropriate default
						 */
	{ "wl_txstreams", "0"},			/* 802.11n Tx Streams 0, 0 is invalid, WLCONF will
						 * change it to a radio appropriate default
						 */
	{ "wl_stbc_tx", "auto"},		/* Default STBC TX setting */
	{ "wl_stbc_rx", "1", 0 },		/* Default STBC RX setting */
	{ "wl_ampdu", "auto"},			/* Default AMPDU setting */
	/* Default AMPDU retry limit per-tid setting */
	{ "wl_ampdu_rtylimit_tid", "7 7 7 7 7 7 7 7"},
	/* Default AMPDU regular rate retry limit per-tid setting */
	{ "wl_ampdu_rr_rtylimit_tid", "3 3 3 3 3 3 3 3"},
	{ "wl_amsdu", "auto"},
	{ "wl_obss_coex", "1"},

	/* WPA parameters */
	{ "wl_auth_mode", "none"},		/* Network authentication mode (radius|none) */
	{ "wl_wpa_psk", ""},			/* WPA pre-shared key */
	{ "wl_wpa_gtk_rekey", "3600"},		/* GTK rotation interval */
	{ "wl_radius_ipaddr", ""},		/* RADIUS server IP address */
	{ "wl_radius_key", "" },		/* RADIUS shared secret */
	{ "wl_radius_port", "1812"},		/* RADIUS server UDP port */
	{ "wl_crypto", "tkip+aes"},		/* WPA data encryption */
	{ "wl_net_reauth", "36000"},		/* Network Re-auth/PMK caching duration */
	{ "wl_akm", ""},			/* WPA akm list */

#ifdef __CONFIG_WFI__
	{ "wl_wfi_enable", "0"},		/* 0: disable, 1: enable WifiInvite */
	{ "wl_wfi_pinmode", "0"},		/* 0: auto pin, 1: manual pin */
#endif /* __CONFIG_WFI__ */
#ifdef __CONFIG_WAPI_IAS__
	/* WAPI parameters */
	{ "wl_wai_cert_name", ""},		/* AP certificate name */
	{ "wl_wai_cert_index", "1"},		/* AP certificate index. 1:X.509, 2:GBW */
	{ "wl_wai_cert_status", "0"},		/* AP certificate status */
	{ "wl_wai_as_ip", ""},			/* ASU server IP address */
	{ "wl_wai_as_port", "3810"},		/* ASU server UDP port */
#endif /* __CONFIG_WAPI_IAS__ */
	/* WME parameters (cwmin cwmax aifsn txop_b txop_ag adm_control oldest_first) */
	/* EDCA parameters for STA */
	{ "wl_wme_sta_be", "15 1023 3 0 0 off off"},	/* WME STA AC_BE parameters */
	{ "wl_wme_sta_bk", "15 1023 7 0 0 off off"},	/* WME STA AC_BK parameters */
	{ "wl_wme_sta_vi", "7 15 2 6016 3008 off off"},	/* WME STA AC_VI parameters */
	{ "wl_wme_sta_vo", "3 7 2 3264 1504 off off"},	/* WME STA AC_VO parameters */

	/* EDCA parameters for AP */
	{ "wl_wme_ap_be", "15 63 3 0 0 off off"},	/* WME AP AC_BE parameters */
	{ "wl_wme_ap_bk", "15 1023 7 0 0 off off"},	/* WME AP AC_BK parameters */
	{ "wl_wme_ap_vi", "7 15 1 6016 3008 off off"},	/* WME AP AC_VI parameters */
	{ "wl_wme_ap_vo", "3 7 1 3264 1504 off off"},	/* WME AP AC_VO parameters */

	{ "wl_wme_no_ack", "off"},		/* WME No-Acknowledgment mode */
	{ "wl_wme_apsd", "on"},			/* WME APSD mode */

	/* Per AC Tx parameters */
	{ "wl_wme_txp_be", "7 3 4 2 0"},	/* WME AC_BE Tx parameters */
	{ "wl_wme_txp_bk", "7 3 4 2 0"},	/* WME AC_BK Tx parameters */
	{ "wl_wme_txp_vi", "7 3 4 2 0"},	/* WME AC_VI Tx parameters */
	{ "wl_wme_txp_vo", "7 3 4 2 0"},	/* WME AC_VO Tx parameters */

	{ "wl_maxassoc", "128"},		/* Max associations driver could support */
	{ "wl_bss_maxassoc", "128"},		/* Max associations driver could support */

	{ "wl_sta_retry_time", "5"},		/* Seconds between association attempts */
#ifdef BCMDBG
	{ "wl_nas_dbg", "0"},			/* Enable/Disable NAS Debugging messages */
#endif

	// ASUS used only?	
	{ "wl_nmode_x", 		"0"	},	/* 0/1/2, auto/nonly,bgmixed*/
	{ "wl_bw", 			"0"	},
	{ "wl0_bw", "0"},
	{ "wl1_bw", "1"},
	{ "wl_auth_mode_x", 		"open"	},
// open/shared/psk/wpa/radius
	{ "wl_wpa_mode", 		"0"	},
// 0/1/2/3/4/5 psk+psk2/psk/psk2/wpa/wpa2/wpa+wpa2
	{ "wl_wep_x", 			"0"	},
// WEP data encryption 0, 1, 2 : disabled/5/13
	{ "wl_radio_date_x", "1111111", 0 	},
	{ "wl_radio_time_x", "00002359", 0 	},
	{ "wl_infra",			"1"	},	// Network Type (BSS/IBSS)
	{ "wl_phrase_x",		""	},	// Passphrase	// Add
	{ "wl_lanaccess", 		"off"   },
	{ "wl_expire", 			"0"     },
#ifdef RTCONFIG_RALINK
	{ "wl_TxPower",			"100" 	},
#else
//	{ "wl_TxPower", 		"0"	},
	{ "wl_TxPower",			"80"	},
#endif

#ifdef RTCONFIG_WIRELESSREPEATER
	{ "wlc_list",			""},
	{ "wlc_band",			""},
	{ "wlc_ssid", 			""},
	{ "wlc_wep",			""},
	{ "wlc_key",			""},
	{ "wlc_wep_key",		""},
	{ "wlc_auth_mode", 		""},
	{ "wlc_crypto", 		""},
	{ "wlc_wpa_psk",		""},
	{ "wlc_nbw_cap", 		""},
	{ "wlc_ure_ssid",		""},
#endif
	// WPA parameters
#ifdef RTCONFIG_RALINK
	{ "wl_PktAggregate", "1" },	// UI configurable
 	{ "wl_HT_OpMode", "0" }, 	// UI configurable
	{ "wl_DLSCapable", "0" },	// UI configurable
	{ "wl_GreenAP", "0" },		// UI configurable
	{ "wl_key_type", "0", 0 },
	{ "wl_HT_AutoBA", "1" },
	{ "wl_HT_HTC", "1"},
	{ "wl_HT_RDG", "1"},
	{ "wl_HT_LinkAdapt", "0"},
	{ "wl_HT_MpduDensity", "5" },
	{ "wl_HT_AMSDU", "0"},
        { "wl_HT_GI", "1"},
	{ "wl_HT_BAWinSize", "64" },
	{ "wl_HT_MCS", "33"},
	{ "wl_HT_BADecline", "0" },
//	{ "wl_HT_TxStream", "2" },
//	{ "wl_HT_RxStream", "3" },
//        { "wl0_HT_TxStream", "2" }, // move to init_nvram for model dep.
//        { "wl0_HT_RxStream", "2" }, // move to init_nvram for model dep.
//        { "wl1_HT_TxStream", "2" }, // move to init_nvram for model dep.
//        { "wl1_HT_RxStream", "3" }, // move to init_nvram for model dep.
	{ "wl_HT_STBC", "1" },
	{ "wl_IgmpSnEnable", "1" },
	{ "wl_McastPhyMode", "0" },
	{ "wl_McastMcs", "0" },
	// the following for ralink 5g only
	{ "wl_IEEE80211H", "0" },
	{ "wl_CSPeriod", "10" },
	{ "wl_RDRegion", "FCC" },
	{ "wl_CarrierDetect", "0" },
	{ "wl_ChannelGeography", "2" },
	{ "wl_txbf", "1" },
	{ "wl_txbf_en", "0" },
#endif

// WPS 
//	#if defined (W7_LOGO) || defined (WIFI_LOGO)
	{ "wps_enable", "1"},
//	#else
//	{ "wps_enable", "0"},					// win7 logo
//	#endif
#ifdef RTCONFIG_RALINK
	{ "wps_mode", "1"},					/* AP WSC PIN method */
	{ "wl_wsc_config_state", "0"},				/* config state unconfiged */
#endif
	{ "wps_band", "0"},					/* "0": 2.4G, "1": 5G */

// Wireless WDS Mode
	{ "wl_mode",			"ap"		},	// AP mode (ap|sta|wds)
	{ "wl_lazywds",			"1"		},	// Enable "lazy" WDS mode (0|1)
	{ "wl_wds",			""		},	// xx:xx:xx:xx:xx:xx ...
	{ "wl_wds_timeout",		"1"		},	// WDS link detection interval defualt 1 sec*/

	{ "wl_mode_x", "0"},   					// 0/1/2(ap/wds/hybrid)
	{ "wl_wdsapply_x", "0"},
	{ "wl_wdslist", ""}, 					// xxxxxxxxxxxx ...


// Wireless Mac Filter
	{ "wl_maclist",			""		},	// xx:xx:xx:xx:xx:xx ...
	{ "wl_macmode",			"disabled"	},
	{ "wl_maclist_x", ""},					// xxxxxxxxxxxx ... xxxxxxxxxxx

// Radius Server
	{ "wl_radius_ipaddr",		""		},	// RADIUS server IP address
	{ "wl_radius_key",		""		},	// RADIUS shared secret
	{ "wl_radius_port",		"1812"		},	// RADIUS server UDP port
#endif

	{NULL, NULL, NULL }
};

#else /* Paul add 2012/9/13 for DSL-N15U */

//TCSUPPORT_WLAN_RT3092
// stub for wlconf, etc.
struct tcapi_tuple wlan_configuration_router_defaults[] = {
	{"WLan_Common", "CountryRegion", "5"},
	{"WLan_Common", "CountryRegionABand", "7"},
	{"WLan_Common", "CountryCode", "TW"},
	{"WLan_Common", "BssidNum", "1"},
	//{"WLan_Common", "TempComp", "2"},	//This parameter will cause Guest Network fail. Javier
	{"WLan_Entry", "ssid", "RT3092_2.4G"},	//for match the element name in wlan_MBSSID_system_defaults
	{"WLan_Common", "WirelessMode", "9"},
	{"WLan_Entry", "TxRate", "0"},
	{"WLan_Common", "Channel", "11"},
	{"WLan_Common", "BasicRate", "15"},
	{"WLan_Common", "BeaconPeriod", "100"},
	{"WLan_Common", "DtimPeriod", "1"},
	{"WLan_Common", "TxPower", "100"},
	{"WLan_Entry", "DisableOLBC", "0"},
	{"WLan_Common", "BGProtection", "0"},
	{"WLan_Common", "MaxStaNum", "0"},
	{"WLan_Common", "TxPreamble", "1"},
	{"WLan_Common", "RTSThreshold", "2347"},
	{"WLan_Common", "FragThreshold", "2346"},
	{"WLan_Common", "TxBurst", "1"},
#if defined(MT7612E) || defined(MT7610E) || defined(DSL_N55U_D1) || defined (DSL_N10_C1)  || defined (DSL_N10P_C1) || defined (DSL_N12E_C1) || defined (DSL_N10_D1) || defined (DSL_N14U) || defined (DSL_N14U_B1)
	{"WLan_Common", "ED_MODE", "2"},
#endif
#if defined(MT7612E) || defined(MT7610E) || defined(MT7592)
	{"WLan_Common", "EfuseBufferMode", "1"},
#endif
	{"WLan_Common", "EDCCA_AP_STA_TH", "255"},
	{"WLan_Common", "EDCCA_AP_AP_TH", "255"},
	{"WLan_Common", "EDCCA_FALSE_CCA_TH", "3000"},
	{"WLan_Common", "EDCCA_ED_TH", "90"},
	{"WLan_Common", "EDCCA_BLOCK_CHECK_TH", "2"},
	{"WLan_Common", "EDCCA_AP_RSSI_TH", "-80"},
	{"WLan_Common", "PktAggregate", "0"},
	{"WLan_Common", "TurboRate", "0"},
	{"WLan_Entry", "WmmCapable", "0"},
	{"WLan_Common", "APSDCapable", "0"},
	{"WLan_Common", "DLSCapable", "0"},	
	{"WLan_Common", "APAifsn", "3;7;1;1"},
	{"WLan_Common", "APCwmin", "4;4;3;2"},
	{"WLan_Common", "APCwmax", "6;10;4;3"},
	{"WLan_Common", "APTxop", "0;0;94;47"},
	{"WLan_Common", "APACM", "0;0;0;0"},
	{"WLan_Common", "BSSAifsn", "3;7;2;2"},
	{"WLan_Common", "BSSCwmin", "4;4;3;2"},
	{"WLan_Common", "BSSCwmax", "10;10;4;3"},
	{"WLan_Common", "BSSTxop", "0;0;94;47"},
	{"WLan_Common", "BSSACM", "0;0;0;0"},
	{"WLan_Common", "AckPolicy", "0;0;0;0"},
	//write NoForwarding and NoForwardingBTNBSSID based on wl_ap_isolate 
	//{"WLan_Entry", "NoForwarding", "0"},
	//{"WLan_Common", "NoForwardingBTNBSSID", "0"},
	{"WLan_Common", "wl_ap_isolate", "0"},
	{"WLan_Entry", "HideSSID", "0"},
	{"WLan_Common", "ShortSlot", "1"},
	{"WLan_Common", "AutoChannelSelect", "0"},
	{"WLan_Common", "AutoChannelSkipList", ""},
	{"WLan_Common", "HT_BSSCoexistence", "1"},
	{"WLan_Entry", "IEEE8021X", "0"},
	{"WLan_Common", "WirelessEvent", "0"},
	{"WLan_Common", "IdsEnable", "0"},
	{"WLan_Common", "AuthFloodThreshold", "32"},
	{"WLan_Common", "AssocReqFloodThreshold", "32"},
	{"WLan_Common", "ReassocReqFloodThreshold", "32"},			
	{"WLan_Common", "ProbeReqFloodThreshold", "32"},
	{"WLan_Common", "DisassocFloodThreshold", "32"},
	{"WLan_Common", "DeauthFloodThreshold", "32"},
	{"WLan_Common", "EapReqFooldThreshold", "32"},		
	{"WLan_Entry", "PreAuth", "0"},
	{"WLan_Entry", "auth_mode_x", "OPEN"},  //Authentication Type	//for match the element name in wlan_MBSSID_system_defaults
	{"WLan_Entry", "crypto", "NONE"},	//for match the element name in wlan_MBSSID_system_defaults
	{"WLan_Entry", "RekeyInterval", "0"},
	{"WLan_Common", "RekeyMethod", "DISABLE"},
	{"WLan_Common", "PMKCachePeriod", "10"},
	{"WLan_Entry", "wpa_psk", ""},	//for match the element name in wlan_MBSSID_system_defaults
	{"WLan_Entry", "key", "1"},	//DefaultKeyID		//for match the element name in wlan_MBSSID_system_defaults
	{"WLan_Entry", "key1", ""},
	{"WLan_Entry", "key2", ""},
	{"WLan_Entry", "key3", ""},
	{"WLan_Entry", "key4", ""},
	{"WLan_Entry", "phrase_x", ""},
	{"WLan_Entry", "wep_x", "0"},
	/* Key1Type and Key1Str1 ~ Key4Type and Key4Str4 will be generated by write_MBSSID_profile() */
	{"WLan_Entry", "HSCounter", "0"},
	{"WLan_Entry", "AccessPolicy", "0"},
	/* AccessPolicy0 and AccessControlList0 ~ AccessPolicy3 and AccessControlList3 will be generated by write_wlan_profile() */
	{"WLan_WDS", "WdsEnable", "0"},
	/*
	{"WLan_WDS", "WdsEncrypType", "NONE"},
	{"WLan_WDS", "WdsList", ""},
	{"WLan_WDS", "WdsKey", ""},
	*/
	{"WLan_Entry", "bw_enabled", "0"}, /* Bandwidth Limiter */
	{"WLan_Entry", "bw_dl", ""},       /* Bandwidth Limiter */
	{"WLan_Entry", "bw_ul", ""},       /* Bandwidth Limiter */
	{"WLan_Entry", "RADIUS_Server", ""},
	{"WLan_Entry", "RADIUS_Port", "1812"},
	{"WLan_Entry", "RADIUS_Key1", ""},	//parameter name in the Web GUI.
	{"WLan_Entry", "own_ip_addr", "192.168.177.123"},	//get the value from "tcapi get Lan_Entry IP"	
	{"WLan_Common", "HT_HTC", "0"},
	{"WLan_Common", "HT_RDG", "0"},
	{"WLan_Common", "HT_EXTCHA", "0"},
	{"WLan_Common", "HT_LinkAdapt", "0"},
	{"WLan_Common", "HT_OpMode", "0"},
	{"WLan_Common", "HT_MpduDensity", "5"},
	{"WLan_Common", "HT_BW", "1"},
	{"WLan_Common", "HT_AutoBA", "1"},
	{"WLan_Common", "HT_AMSDU", "0"},
	{"WLan_Common", "HT_BAWinSize", "64"},
	{"WLan_Common", "HT_GI", "1"},
	{"WLan_Common", "HT_BADecline", "0"},
	{"WLan_Common", "HT_STBC", "1"},
	{"WLan_Entry", "HT_MCS", "33"},
	{"WLan_Entry", "MeshId", "MESH"},
	{"WLan_Entry", "MeshAutoLink", "1"},
	{"WLan_Entry", "MeshAuthMode", "OPEN"},
	{"WLan_Entry", "MeshEncrypType", "NONE"},
	{"WLan_Entry", "MeshWPAKEY", ""},
	{"WLan_Entry", "MeshDefaultkey", "33"},
	{"WLan_Entry", "MeshWEPKEY", "1"},
	{"WLan_Entry", "WscVendorPinCode", "24705561"},
	{"WLan_Entry", "WscManufacturer", ""},
	{"WLan_Entry", "WscModelName", ""},
	{"WLan_Entry", "WscDeviceName", ""},
	{"WLan_Entry", "WscModelNumber", ""},
	{"WLan_Entry", "WscSerialNumber", ""},
	{"WLan_Entry", "RadioOn", "1"},
	{"WLan_Common", "IgmpSnEnable", "0"},
	//{"WLan_Common", "McastPhyMode", "3"},	//handled by wl_mrate_x
	//{"WLan_Common", "McastMcs", "7"},	//handled by wl_mrate_x
	{"WLan_Common", "wl_mrate_x", "0"},
	{"WLan_Common", "location_code", ""},
	{"WLan_Common", "MacAddress", "00:11:22:33:44:00"},
	{"WLan_Common", "CarrierDetect", "0"},
	{"WLan_Common", "GreenAP", "0"},
	{"WLan_Entry", "TxAntenna", ""}, /* Paul modify 2013/3/11, TxAntenna & RxAntenna default should be empty, otherwise will cause WiFi connectivity issue. */
	{"WLan_Entry", "RxAntenna", ""},
	{"WLan_Common", "HT_TxStream", "2"},
	{"WLan_Common", "HT_RxStream", "2"},
	{"WLan_Common", "PhyRateLimit", "0"},
	{"WLan_Entry", "DBG_LEVEL", "0"},
	//The following parameters are dedicated for 5G.
	{"WLan_Entry", "RDRegion", "CE"},
	{"WLan_Common", "IEEE80211H", "1"},
	{"WLan_Entry", "CSPeriod", "5"},
	{"WLan_Entry", "StationKeepAlive", "0"},
	{"WLan_Entry", "DfsLowerLimit", "0"},
	{"WLan_Entry", "DfsUpperLimit", "0"},
	{"WLan_Entry", "DfsIndoor", "0"},
	{"WLan_Entry", "DFSParamFromConfig", "0"},
	{"WLan_Entry", "FCCParamCh0", ""},
	{"WLan_Entry", "FCCParamCh1", ""},
	{"WLan_Entry", "FCCParamCh2", ""},
	{"WLan_Entry", "FCCParamCh3", ""},
	{"WLan_Entry", "CEParamCh0", ""},
	{"WLan_Entry", "CEParamCh1", ""},
	{"WLan_Entry", "CEParamCh2", ""},
	{"WLan_Entry", "CEParamCh3", ""},
	{"WLan_Entry", "JAPParamCh0", ""},
	{"WLan_Entry", "JAPParamCh1", ""},
	{"WLan_Entry", "JAPParamCh2", ""},
	{"WLan_Entry", "JAPParamCh3", ""},
	{"WLan_Entry", "JAPW53ParamCh0", ""},
	{"WLan_Entry", "JAPW53ParamCh1", ""},
	{"WLan_Entry", "JAPW53ParamCh2", ""},
	{"WLan_Entry", "JAPW53ParamCh3", ""},
	{"WLan_Entry", "FixDfsLimit", "0"},
	{"WLan_Entry", "LongPulseRadarTh", "0"},
	{"WLan_Entry", "AvgRssiReq", "0"},
	{"WLan_Entry", "DFS_R66", "0"},
	{"WLan_Entry", "BlockCh", ""},
	{"WLan_Entry", "VHT_BW", "1"},
	{"WLan_Entry", "VHT_SGI", "1"},
	{"WLan_Entry", "VHT_LDPC", "1"},
	{NULL, NULL, NULL }
};
#endif

/*Multiple BSSID*/
//Not change this order arbitrarily because the funation wl_get_2G_guestnetwork() based on it to match javi script
struct tcapi_tuple wlan_MBSSID_system_defaults[] = {
	{"WLan_Entry", "bss_enabled", "0"},
	{"WLan_Entry", "ssid", "ASUS"},
	{"WLan_Entry", "auth_mode_x", "OPEN"},	//AuthMode
	{"WLan_Entry", "crypto", "NONE"},	//EncrypType
	{"WLan_Entry", "wpa_psk", ""},	//WPAPSK
	{"WLan_Entry", "wep_x", "0"},	//WEP Encryption	//5
	{"WLan_Entry", "key", "1"},  //DefaultKeyID
	{"WLan_Entry", "key1", ""},
	{"WLan_Entry", "key2", ""},
	{"WLan_Entry", "key3", ""},
	{"WLan_Entry", "key4", ""},	//10
	{"WLan_Entry", "expire", "0"},
	{"WLan_Entry", "lanaccess", "off"},
	{"WLan_Entry", "expire_tmp", "0"},
	{"WLan_Entry", "wl_macmode", "disabled"},
	{"WLan_Entry", "bss_enabled", "0"},
	{"WLan_Entry", "wl_maclist", ""},
	{"WLan_Entry", "phrase_x", ""},
	{"WLan_Entry", "bw_enabled", "0"},
	{"WLan_Entry", "bw_dl", ""},
	{"WLan_Entry", "bw_ul", ""},
	{"WLan_Entry", "url_enabled", "0"},
	{"WLan_Entry", "url_mode", "0"},
	{"WLan_Entry", "url_rulelist", ""},
	{NULL, NULL, NULL }
};
/*Multiple BSSID*/

#endif

#endif
