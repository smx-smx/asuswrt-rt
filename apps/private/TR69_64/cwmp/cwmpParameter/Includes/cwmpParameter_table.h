
/*
** $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/private/TR69_64/cwmp/cwmpParameter/Includes/cwmpParameter_table.h#4 $
*/
/************************************************************************
 *
 *	Copyright (C) 2003 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/

#ifndef _PARAMETER_C_
#define _PARAMETER_C_


#define CWMP_BOOLEAN   		 		(BooleanType)	
#define CWMP_INTEGER     		 	(IntType)	
#define CWMP_UNSIGNEDINT   		 	(UnsignedIntType)	
#define CWMP_STRING     			(StringType)	
#define CWMP_OBJECT   		        (ObjectType)	
#define CWMP_DATE_TIME    	        (DateTimeType)	
#define CWMP_BASE64     			(Base64Type)	

#define TRUE 1
#define FALSE 0

#ifdef WITHVOIP
#define VOIP 1
#else
#define VOIP 0
#endif

//add by brian for ip ping diagnostic
#define IPPINGDIAGNOSTICFILE "/tmp/cwmp/ippingdiagnostic_stat"
#ifdef TCSUPPORT_TRACEROUTE
#define NO_QMARKS 0
#define TRACEROUTEDIAGNOSTICFILE "/tmp/cwmp/traceroutediagnostic_stat"
#define MAX_TRACEROUTBUF_LEN 32768 //for read traceroute dignostic result
#endif
#define IMCOMPLETEFLAG "0 packets received"
#define MAX_PINGBUF_LEN 2048 //for read ip ping dignostic result
//add by brian for dsl diagnostic
#define CWMPDSLDIAGNOSTIC "CDSLDiagnostic_Common"
//add by renyu for dsl interface config
#define INFOADSL_NODE "Info_Adsl"
#define INFOSHOW_NODE "Info_Showtime"
#define INFOLAST_NODE "Info_LastShow"
#define INFOTOTAL_NODE "Info_TotalShow"
#define INFOCURDAYST_NODE "Info_CurdayStShow"
#define INFOQUTHOURST_NODE "Info_QutHourStShow"
#define DHCPLEASE_NODE "DhcpLease_Entry%d"
#define INFONAS_NODE "Info_nas%d"
#define PVC_NUMBER 8
#define ReadOnly  						0x00
#define Writable 						0x01
#define SEEK_SET 						0
#define QUOTATIONMARKSLEN				140    /* the length use for QuotationMarksExchange function */
#define DNS_INFO_PATH	"/etc/resolv.conf"

#define MAX_STRING_LENGTH 				256
#define MAX_VIRSERV_RULE 				10
#define MAX_LEN_OF_SUPPORTED_RATES 		12
//#define VOIP  							1 //add by renyu_20091225
#define PKT_QOS_RULE_MAX	16

#define CWMP_PURE_BRIDGE_TYPE_IP_BRIDGED (1 << 2)

#define MAX_TR69_PARAM

/*wan Encap definition. dynamic/static/pppoe/pppoa/pure bridge mode*/
#define WAN_ENCAP_DYN			"0"
#define WAN_ENCAP_STATIC		"1"
#define WAN_ENCAP_PPP			"2"
#define WAN_ENCAP_BRIDGE		"3"

#define SEC_PER_DAY 86400 /*60*60*24  for ppp link-up time*/
#define SEC_PER_HOUR 3600 /*60*60         shnwind 2008.4.14*/
#define SEC_PER_MIN 60
#define MAX_WAN_NODE 			8
#if defined(TCSUPPORT_CWMP_VDSL)	
#define MAX_PTM_NODE 			2
#endif
#define MAX_VCPOOL_NUM 			16
#define MAX_PM_RULE				10
#define MAXSSIDNUM				4

#define MAX_FORWARD_NUM 			16
#define MAX_ROUTE_NUM 			16

/*Add for VendorConfigFile TR069 Nodes*/
#define MAX_VENDORCFGFILE_NUM	2


#define 	CFG_YES 			1
#define		CFG_NO				0
#define		NONECHILD			0
#define 	NOBITSET			0
#define		NODELAY				0
#define 	DELAY				1

#define MAX_DHCP_STATIC_LEASE   8
typedef struct cfgNode_s
{
	char 	 	*nodeName;
	char 	 	*node_id;
	uint32   	bitMap;
	uint8    	bitNum;
	uint8    	settingFlag;
	int     	(*commitFunc)(struct cfgNode_s* data);
}cfgNode_t,*cfgNode_ptr;


#define  	ATTRFLAG0 			0
#define 	ATTRFLAG1			1
typedef struct attr_s
{
	char *attrName;
	void *attrPtr;
	int attrLen;
	uint32 magic;
	int fIndex;
	uint32 fBit;
}attr_t, *attr_ptr;


/***************************************************************************************************/
							/*TR069 attribute struct flag.define here.*/
/***************************************************************************************************/
#define	CWMP_ATTR_FLAG						(1 << 0)
#define DSLLINK_ATTR_FLAG					(1 << 1)
#define WLAN_ATTR_FLAG						(1 << 2)
#define IPCON_ATTR_FLAG						(1 << 3)
//add by brian for ip ping diagnostic
#define IPPINGDIAGNOSTIC_ATTR_FLAG			(1 << 4)
//add by brian for dsl diagnostic
#define DSLDIAGNOSTIC_ATTR_FLAG				(1 << 5)
//add by brian for atm f5 loopback diagnostic
#define ATMDIAGNOSTIC_ATTR_FLAG				(1 << 6)
#ifdef TR111
#define TR111_ATTR_FLAG						(1 << 7)
#endif
#ifdef VOIP
#define TR104_ATTR_FLAG						(1 << 8)
#endif
#define	DEVICEINFO_ATTR_FLAG				(1 << 9)
#define TIME_ATTR_FLAG						(1 << 10)
//add by renyu
#define DSLINTERCFG_ATTR_FLAG				(1 << 11)
#define DSLCOMINTERCFG_ATTR_FLAG			(1 << 12)
#define DSLCONNMGMT_ATTR_FLAG				(1 << 13)
#define DSLCONNMGMTSVR_ATTR_FLAG			(1 << 14)
#define DEVCONFIG_ATTR_FLAG					(1 << 15)

//add by brian for layer3forwarding
#define LAYER3FORWARDING_ATTR_FLAG			(1 << 16)
#define SHOWTIME_ATTR_FLAG					(1 << 17)
#define TOTAL_ATTR_FLAG						(1 << 18)
#define LASTSHOWTIME_ATTR_FLAG				(1 << 19)
#define CURRENTDAY_ATTR_FLAG				(1 << 20)
#define QUARTERHOUR_ATTR_FLAG				(1 << 21)
#define LANCONFIGSECURITY_ATTR_FLAG			(1 << 22)

//add by liujilin
#define LANDEVICE_ATTR_FLAG					(1 << 23)	

#define IPINTERFACE_ATTR_FLAG				(1 << 24)
#define ETHERINTERCFG_ATTR_FLAG				(1 << 25)
#define HOST_ATTR_FLAG						(1 << 26)

//add by yzwang 
#define QOS_ATTR_FLAG						(1 << 27)
#define USERITF_ATTR_FLAG					(1 << 28)
#ifdef TR143
#define	TR143_ATTR_FLAG						(1 << 29)
#endif

#ifdef DHCP_PROFILE
#define DHCPOPTION_ATTR_FLAG					(1 << 30)	
#define DHCPCONSER_ATTR_FLAG					(1 << 31)	
#endif

#ifdef TCSUPPORT_TRACEROUTE 
#define TRACEROUTEDIAGNOSTIC_ATTR_FLAG    (1 << 2)
#endif
#if defined(TCSUPPORT_CWMP_VDSL)
#define PTMLINK_ATTR_FLAG    (1 << 4)
#endif
/***************************************************************************************************/
						/*TR069 attribute struct flag.define here.*/
/***************************************************************************************************/

#define MAX_ATTR_LEN 						512
#define MAX_ATTR_OFFSET 					9

/***************************************************************************************************/
						/*TR069 attribute struct definition*/
/***************************************************************************************************/
/*the attribute of TR069 Parameter: InternetGatewayDevice.DeviceInfo.*/
#define DEVICEINF_MAGIC_ATTR	0x11221122
typedef struct deviceInfoAttr
{
	uint8  aDeviceSummary;
	uint8  aLANDeviceNumberOfEntries;
	uint8  aWANDeviceNumberOfEntries;
	
	uint8  aManufacturer;
	uint8  aManufacturerOUI;
	uint8  aModelName;
	uint8  aDescription;
	
	uint8  aProductClass;
	uint8  aSerialNumber;
	uint8  aHardwareVersion;
	uint8  aSoftwareVersion;
	
	uint8  aSpecVersion;
	uint8  aProvisioningcode;
	uint8  aUpTime;
	uint8  aDeviceLog;
	
	/*Add for VendorConfigFile TR069 Nodes*/
	uint8 aVendorCfgFileNumOfEntries;
	uint8 aVendorCfgFileName[MAX_VENDORCFGFILE_NUM];
	uint8 aVendorCfgFileVersion[MAX_VENDORCFGFILE_NUM];
	uint8 aVendorCfgFileDate[MAX_VENDORCFGFILE_NUM];
	uint8 aVendorCfgFileDescrip[MAX_VENDORCFGFILE_NUM];

	uint8 aModemFirmwareVersion;
	uint8 Reserved[3];

	uint32  magic;  //This variable should be at the end of the structure.
	
}deviceInfoAttr;

/*the attribute of TR069 Parameter: InternetGatewayDevice.ManagementServer.*/
#define CWMP_MAGIC_ATTR 	0xAABBCCDD 
typedef struct cwmpAttr
{
	uint8  aEnableCWMP;
	uint8  aURL;
	uint8  aUsername;	
	uint8  aPassword;	
	uint8  aPeriodicInformEnable;
	uint8  aPeriodicInformInterval;	
	uint8  aPeriodicInformTime;	
	uint8  aConnectionRequestURL;
	uint8  aConnectionRequestUsername;
	uint8  aConnectionRequestPassword;
	uint8  aUpgradesManaged;
	uint8  aParameterKey;
	uint8 Reserved[8];
	uint32  magic;  //This variable should be at the end of the structure. 
}cwmpAttr;

/*InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANDSLLinkConfig.*/
#define DSLLink_MAGIC_ATTR    0x11223344
typedef struct dslLinkAttr
{
	uint8  aWANDSLLinkCfg1Enable[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1LinkStatus[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1LinkType[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1AutoConfig[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1ModType[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1DestAddr[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1ATMEncap[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1FCSPreserved[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1ATMQoS[MAX_WAN_NODE];
	uint8  aATMPeakCellRate[MAX_WAN_NODE];
	uint8  aATMMaximumBurstSize[MAX_WAN_NODE];
	uint8  aATMSustainableCellRates[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1ATMTransmittedBlocks[MAX_WAN_NODE];
	uint8  aWANDSLLinkCfg1ATMReceivedBlocks[MAX_WAN_NODE];

	uint8  aWANDSLLinkCfg1VCSearchList;
	uint8  aWANDSLLinkCfg1AAL5CRCErrors;
	uint8  aWANDSLLinkCfg1ATMCRCErrors;
	uint8  aWANDSLLinkCfg1ATMHECErrors;		
	uint8  aX_TTNET_IGMPProxyEnable;	
	/*	there are 116 bytes used,
	 **  if you add other attribute,please update the stats.
	  */
	  
	uint8 Reserved[79];
	
	uint32  magic;  //This variable should be at the end of the structure. 
}dslLinkAttr;
#if defined(TCSUPPORT_CWMP_VDSL)	
/*InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANPTMLinkConfig.*/
#define PTMLink_MAGIC_ATTR    0x11223355
typedef struct ptmLinkAttr
{
	uint8  aWANPTMLinkCfgEnable[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgLinkStatus[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgMACAddress[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgBytesSent[MAX_PTM_NODE];

	uint8  aWANPTMLinkCfgBytesReceived[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgFramesSent[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgFramesReceived[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgOOSNearEnd[MAX_PTM_NODE];

	uint8  aWANPTMLinkCfgOOSFarEnd[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgErrorsSend[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgErrorsReceived[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgUnicastPacketsSent[MAX_PTM_NODE];

	uint8  aWANPTMLinkCfgUnicastPacketsReceived[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgDiscardPacketsSent[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgDiscardPacketsReceived[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgMulticastPacketsSent[MAX_PTM_NODE];

	uint8  aWANPTMLinkCfgMulticastPacketsReceived[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgBroadcastPacketsSent[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgBroadcastPacketsReceived[MAX_PTM_NODE];
	uint8  aWANPTMLinkCfgUnknownProtoPacketsReceived[MAX_PTM_NODE];
	/*	there are 40 bytes used,
	 **  if you add other attribute,please update the stats.
	  */	  
	uint8 Reserved[20];
	
	uint32  magic;  //This variable should be at the end of the structure. 
}ptmLinkAttr;
#endif
/*InternetGatewayDevice.LanDevice.1.WLANConfiguration.{i}.*/
#define WLAN_MAGIC_ATTR 	0x22335566
typedef struct wlanAttr
{
	uint8  aWLanEnable[4];	
 	uint8  aWLanMaxBitRate[4];	
 	uint8  aWLanChannel[4];
	uint8  aAutoChannelEnable[4];
 	uint8  aWLanSSID[4];

 	uint8  aWLanBeaconType[4];
 	uint8  aWLanMACAddressControlEnabled[4];
 	uint8  aWLanWEPKeyIndex[4];	
  	uint8  aWLanKeyPassphrase[4];
	
  	uint8  aWLanBasicEncryptionModes[4];
  	uint8  aWLanBasicAuthenticationMode[4];
  	uint8  aWLanWPAEncryptionModes[4];
  	uint8  aWLanWPAAuthenticationMode[4];
	
  	uint8  aWLanBasicDataTransmitRates[4];
  	uint8  aWLanOperationalDataTransmitRates[4];
  	uint8  aWLanRadioEnabled[4];
  	uint8  aWLanAutoRateFallBackEnabled[4];
		
	uint8  aWEPKey[4];	
	uint8  aWLanPreSharedKey[10];
	uint8  aWLanPreKeyPassphrase[10];
	
	uint8  aWLanStatus[4];
	uint8  aWLanBSSID[4];
	uint8  aWLanStandard[4];
	uint8  aWLanWEPEncryptionLevel[4];

	uint8  aWLanIEEE11iEncryptionModes[4];
	uint8  aWLanIEEE11iAuthenticationMode[4];
	
	uint8  aWLanPossibleChannels[4];
	uint8  aWLanPossibleDataTransmitRates[4];
	uint8  aWLanHideSSIDEnabled[4];	
	uint8  aWLanTotalBytesSent[4];
	uint8  aWLanTotalBytesReceived[4];
	
	uint8  aWLanTotalPacketsSent[4];
	uint8  aWLanTotalPacketsReceived[4];

	uint8  aWLanTotalAssociations[4];

	uint8  aWLanAssociatedDevMACAddr[4];
	uint8  aWLanAssociatedDevIPAddr[4];
	uint8  aWLanAssociatedDevAuthState[4];

	/*wps attribute start*/
	uint8  aWPSEnable;
	uint8  aWPSDeviceName;
	uint8  aWPSDevicePassword;
	uint8  aWPSUUID;
	
	uint8  aWPSVersion;
	uint8  aWPSConfigMethodsSupported;
	uint8  aWPSConfigMethodsEnabled;
	uint8  aWPSSetupLockedState;
	
	uint8  aWPSSetupLock;
	uint8  aWPSConfigurationState;
	uint8  aWPSLastConfigurationError;
	uint8  aWPSRegistrarNumberOfEntries;
	
	uint8  aWPSRegistrarEstablished;
	uint8  aWPSRegistrarEnable[1];
	uint8  aWPSRegistrarUUID[1];
	uint8  aWPSRegistrarDeviceName[1];
	uint8 aWLanTxPower;
	uint8 aWLanTxPreamble;	
	/*wps attribute end*/
	
	/*	there are 158 bytes used,

	 **	if you add other attributes, please update the stats.
	  */
#if defined(TCSUPPORT_WLAN_8021X)
	uint8  aRadiusServer[4][2];
	uint8  aRadiusPort[4][2];
	uint8  aRadiusKey[4][2];
#endif
	uint8 Reserved[26];
	
	uint32 magic;
}wlanAttr;

/*InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}. WANIPConnection.{i}.*/
/*InternetGatewayDevice.WANDevice.{i}.WANConnectionDevice.{i}. WANPPPConnection.{i}.*/
#define IPCON_MAGIC_ATTR		0xAACCDDBB
typedef struct ipConAttr
{
	uint8  aWANIPCnt1_1Enable[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1Type[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1Name[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1DefGateway[MAX_WAN_NODE];

	uint8  aWANIPCnt1_1SubMask[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1ExtIPAddr[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1AddrType[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1CntTrigger[MAX_WAN_NODE];

	uint8  aWANIPCnt1_1RProRx[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1NATEnable[MAX_WAN_NODE];

	uint8  aWANIPCnt1_1CntStatus[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1PsbCntTypes[MAX_WAN_NODE];
	
	uint8  aWANIPCnt1_1Uptime[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1LastCntError[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1RSIPAvailable[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1DNSEnabled[MAX_WAN_NODE];	
	
	uint8  aWANIPCnt1_1DNSOverAllow[MAX_WAN_NODE];	
	uint8  aWANIPCnt1_1DNSServers[MAX_WAN_NODE];
	
	uint8  aWANIPCnt1_1MaxMTUSize[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1X_TTNET_BytesSent[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1X_TTNET_BytesReceived[MAX_WAN_NODE];
	
	uint8  aWANIPCnt1_1MACAddress[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1TransportType[MAX_WAN_NODE];
	
	uint8  aWANIPConnectionNumOfEntries[MAX_WAN_NODE];
	uint8  aWANPPPConnectionNumOfEntries[MAX_WAN_NODE];
	
	uint8  aWANIPCnt1_1EthBytesSent[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1EthBytesRcv[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1EthPktsSent[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1EthPktRcv[MAX_WAN_NODE];

	uint8  aPortMappingEntryNum[MAX_WAN_NODE];  

	uint8  aIPPMapping1Enable[MAX_WAN_NODE][MAX_PM_RULE];
	uint8  aIPPMapping1Descrip[MAX_WAN_NODE][MAX_PM_RULE];
	uint8  aIPPMapping1LeaseDur[MAX_WAN_NODE][MAX_PM_RULE];
	uint8  aIP1RemoteHost[MAX_WAN_NODE][MAX_PM_RULE];

	uint8  aIP1ExternalPort[MAX_WAN_NODE][MAX_PM_RULE];
	uint8  aIP1InternalPort[MAX_WAN_NODE][MAX_PM_RULE];
	uint8  aIPPMapping1Protocol[MAX_WAN_NODE][MAX_PM_RULE];
	uint8  aIP1InternalClient[MAX_WAN_NODE][MAX_PM_RULE];
	uint8  aExternalPortEndRange[MAX_WAN_NODE][MAX_PM_RULE]; 

	uint8  aWANPPPCnt1_1PPPoESvcName[MAX_WAN_NODE];
	uint8  aWANPPPCnt1_1Username[MAX_WAN_NODE];
	uint8  aWANPPPCnt1_1Password[MAX_WAN_NODE];
	uint8  AuthenType[MAX_WAN_NODE];
	uint8  aWANPPPCnt1_1PPPoEACName[MAX_WAN_NODE];
	
	uint8  aWANIPCnt1_1IdleDisconnectTime[MAX_WAN_NODE];
	
	uint8  aWANIPCnt1_1EthErrorsSent[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1EthErrorsRcv[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1EthDiscardPktsSent[MAX_WAN_NODE];
	uint8  aWANIPCnt1_1EthDiscardPktRcv[MAX_WAN_NODE];
	
	/*	there are 320 bytes used.
	 **	if add other attributes,please update the stats.
	  */
	//uint8  Reserved[40];
		
	uint32 magic;
}ipConAttr;

//add by brian for ip ping diagnostic
#define IPPINGDIAGNOSTIC_MAGIC_ATTR 0x44332211 
typedef struct ipPingDiagnosticAttr
{
	uint8 aIpPingDiagnosticsState;
	uint8 aIpPingInterface;
	uint8 aIpPingHost;
	uint8 aIpPingNumberOfRepetitions;
	uint8 aIpPingTimeout;
	uint8 aIpPingDataBlockSize;
	uint8 aIpPingDSCP;
	uint8 aIpPingSuccessCount;
	uint8 aIpPingFilureCount;
	uint8 aIpPingAverageResponseTime;
	uint8 aIpPingMinimumResponseTime;
	uint8 aIpPingMaxumumResponseTime;
	uint8 reserved[16];	
	uint32  magic;
}ipPingDiagnosticAttr;


#ifdef TCSUPPORT_TRACEROUTE
#define TRACEROUTEDIAGNOSTIC_MAGIC_ATTR 0x33442211
typedef struct traceRouteDiagnosticAttr
{
	uint8 aTraceRouteDiagnosticsState;
	uint8 aTraceRouteInterface;
	uint8 aTraceRouteHost;
	uint8 aTraceRouteNumberOfTries;
	uint8 aTraceRouteTimeout;
	uint8 aTraceRouteDataBlockSize;
	uint8 aTraceRouteDSCP;
	uint8 aTraceRouteMaxHopCount;
	uint8 aTraceRouteResponseTime;
	uint8 aTraceRouteRouteHopsNumberOfEntries;
	uint8 aTraceRouteHopHost;
	uint8 aTraceRouteHopHostAddress;
	uint8 aTraceRouteHopErrorCode;
	uint8 aTraceRouteHopRTTimes;	
	uint8 reserved[14];	
	uint32  magic;
}traceRouteDiagnosticAttr;

#endif
//add by brian for dsl diagnostic
#define DSLDIAGNOSTIC_MAGIC_ATTR 0x44331122 
typedef struct dslDiagnosticAttr
{
	uint8 aWANDSLDiagnostics_LoopDiagnosticsState;
	uint8 aWANDSLDiagnostics_ACTPSDds;
	uint8 aWANDSLDiagnostics_ACTPSDus;
	uint8 aWANDSLDiagnostics_ACTATPds;
	uint8 aWANDSLDiagnostics_ACTATPus;
	uint8 aWANDSLDiagnostics_HLINSCds;
	uint8 aWANDSLDiagnostics_HLINpsds;
	uint8 aWANDSLDiagnostics_QLNpsds;
	uint8 aWANDSLDiagnostics_SNRpsds;
	uint8 aWANDSLDiagnostics_BITSpsds;
	uint8 aWANDSLDiagnostics_GAINSpsds;
	uint8 reserved[53];	
	uint32  magic;
}dslDiagnosticAttr;

#ifdef TR111
//add by renyu for tr111
#define TR111_MAGIC_ATTR 0x11223344
typedef struct tr111Atttr
{
	uint8 amDevNotificationLimit;

	uint8 Reserved[3];
	
	uint32  magic;  
}tr111Attr;
#endif
#ifdef VOIP
//add by renyu for tr104
#define TR104_MAGIC_ATTR 0x11223344
typedef struct tr104Atttr
{
	uint8 aVoiceProfileNmberOfEntries;
	uint8 aMaxProfileCount;
	uint8 aMaxLineCount;
	uint8 aFaxT38;
	
	uint8 aLocalPortMin;
	uint8 aSIPProxyServer;
	uint8 aSIPProxyServerPort;
	uint8 aSIPProxyServerTransport;
	
	uint8 aSIPRegistrarServer;
	uint8 aSIPRegistrarServerPort;
	uint8 aSIPRegistrarServerTransport;
	uint8 aOutboundProxy;
	
	uint8 aOutboundProxyPort;
	uint8 aLineEnable;
	uint8 aLineSIPAuthUserName;	
	uint8 aLineSIPAuthPassword;
	
	uint8 aProfileFaxT38;
	uint8 aEchoCancellationEnable;
	uint8 aCodec;	
	uint8 aPacketizationPeriod;	
	
	uint8 aPriority;

	//332 node in TR104 still not impliment
	uint8 Reserved[332+3];
	
	uint32  magic;  
}tr104Attr;
#endif

//add by brian for atm f5 loopback diagnostic
#define ATMDIAGNOSTIC_MAGIC_ATTR 0x44112233
typedef struct atmDiagnosticAttr
{
	uint8 aWANATMF5Loopback_DiagnosticsState[MAX_WAN_NODE];
	uint8 aWANATMF5Loopback_NumberOfRepetitions[MAX_WAN_NODE];
	uint8 aWANATMF5Loopback_Timeout[MAX_WAN_NODE];
	uint8 aWANATMF5Loopback_SuccessCount[MAX_WAN_NODE];
	uint8 aWANATMF5Loopback_FailureCount[MAX_WAN_NODE];
	uint8 aWANATMF5Loopback_AverageResponseTime[MAX_WAN_NODE];
	uint8 aWANATMF5Loopback_MinimumResponseTime[MAX_WAN_NODE];
	uint8 aWANATMF5Loopback_MaximumResponseTime[MAX_WAN_NODE];	
	uint32  magic;
}atmDiagnosticAttr;

//add by yzwang_20100108 for timezone
#define TIME_MAGIC_ATTR	0x12121212
typedef struct timeAttr
{
	uint8  aTimeNTPServer1;
	uint8  aTimeCurrentLocalTime;
	uint8  aTimeLocalTimeZone;

	uint8  Reserved[13];
	uint32  magic;  
}timeAttr;

//add by renyu for dsl interface config
#define DSLINTERCFG_MAGIC_ATTR 0x44331122 
typedef struct dslInterCfgAttr
{
	uint8 aStatus;
	uint8 aModulationType;
	uint8 aLineEncoding;
	uint8 aDataPath;
	
	uint8 aUpstreamCurrRate;
	uint8 aDownstreamCurrRate;
	uint8 aUpstreamMaxRate;
	uint8 aDownstreamMaxRate;
	
	uint8 aUpstreamNoiseMargin;
	uint8 aDownstreamNoiseMargin;
	uint8 aUpstreamAttenuation;
	uint8 aDownstreamAttenuation;
	
	uint8 aUpstreamPower;
	uint8 aDownstreamPower;
	uint8 aATURVendor;
	uint8 aATURCountry;
	
	uint8 aATUCVendor;
	uint8 aATUCCountry;
	uint8 aShowtimeStart;
	uint8 aTotalStart;

	uint8 aInterleaveDepth;	
	uint8 aATURANSIStd;
	uint8 aATURANSIRev;
	uint8 aATUCANSIStd;
	
	uint8 aATUCANSIRev;
	uint8 aX_TTNET_DSLUpTime;
	uint8 aX_TTNET_NumOfClientConnected;
	uint8 aEnable;
	
	uint8 aLinkEncapsulationSupported;
	uint8 aLinkEncapsulationUsed;
	uint8 aLinkEncapsulationRequested;
	uint8 StandardsSupported;
	
	uint8 aStandardUsed;
	uint8 aWanConnectionNumOfEntries;
	uint8 aDSLIfaceCfgLineNumber;
#if defined(TCSUPPORT_CWMP_VDSL)
	uint8 aAllowedProfiles;
	uint8 aCurrentProfiles;
	uint8 aUPBOKLE;	
	uint8 aTRELLISds;

	uint8 aTRELLISus;
	uint8 aACTSNRMODEds;
	uint8 aACTSNRMODEus;
	uint8 aACTUALCE;

	uint8 aSNRMpbds;
	uint8 aSNRMpbus;
	uint8 reserved[11];	
#else
	uint8 reserved[21];
#endif	
	uint32  magic;
}dslInterCfgAttr;

//add by renyu for dsl common interface config
#define DSLCOMINTERCFG_MAGIC_ATTR 0x44331122 
typedef struct dslComInterCfgAttr
{
	uint8 aEnabledForInternet;
	uint8 aWANAccessType;
	uint8 aLayer1UpstreamMaxBitRate;
	uint8 aLayer1DownstreamMaxBitRate;
	
	uint8 aPhysicalLinkStatus;
	uint8 aTotalBytesSent;
	uint8 aTotalBytesReceived;
	uint8 aTotalPacketsSent;
	
	uint8 aTotalPacketsReceived;
	
	uint8 reserved[3];	
	uint32  magic;
}dslComInterCfgAttr;

//add by renyu for dsl connection management
#define DSLCONNMGMT_MAGIC_ATTR 0x44331122 
typedef struct dslConnMgmtAttr
{
	uint8 aConnSerNumbeOfEntries;
	
	uint8 reserved[3];	
	uint32  magic;
}dslConnMgmtAttr;

//add by renyu for dsl connection management service
#define DSLCONNMGMTSVR_MAGIC_ATTR 0x44331122 
typedef struct dslConnMgmtSvrAttr
{
	uint8 aWANConnectionDevice;
	uint8 aWANConnectionService;
	uint8 aDestinationAddress;
	uint8 aLinkType;
	
	uint8 aConnectionType;
	
	uint8 reserved[3];	
	uint32  magic;
}dslConnMgmtSvrAttr;

//add by yzwang_20100118 for deviceConfig
#define DEVCONFIG_MAGIC_ATTR	0x12121213
typedef struct devConfigAttr
{
	uint8  aPersistentData;
	uint8  aConfigFile;

	uint8  Reserved[2];
	uint32 magic;  
}devConfigAttr;

#define LAYER3FORWARDING_MAGIC_ATTR 0x44332112
typedef struct layer3ForwardingAttr
{
	uint8  aForwarding1Enable[MAX_FORWARD_NUM];
	uint8  aForwarding1Status[MAX_FORWARD_NUM];	
	uint8  aForwarding1Type[MAX_FORWARD_NUM];
	uint8  aForwarding1DestIPAddress[MAX_FORWARD_NUM];
	
	uint8  aForwarding1SubnetMask[MAX_FORWARD_NUM];
	uint8  aForwarding1SourceIPAddress[MAX_FORWARD_NUM];
	uint8  aForwarding1SourceSubnetMask[MAX_FORWARD_NUM];
	uint8  aForwarding1GatewayIPAddress[MAX_FORWARD_NUM];
	
	uint8  aForwarding1Interface[MAX_FORWARD_NUM];
	uint8  aForwarding1ForwardingMetric[MAX_FORWARD_NUM];

	uint8  aForwardNumberOfEntries;
	uint8  aDefaultConnectionService;
	uint8  Reserved[2];
	uint32 magic;
}layer3ForwardingAttr;
//add by renyu for stats.Showtime
#define SHOWTIME_MAGIC_ATTR 0x44331122 
typedef struct showtimeAttr
{
	uint8 aReceiveBlock;
	uint8 aTransmitBlock;
	uint8 aCellDelin;
	uint8 aLinkRetrain;
	
	uint8 aInitErrors;
	uint8 aInitTimeouts;
	uint8 aLossOfFraming;
	uint8 aErroredSecs;
	
	uint8 aSeverelyErroredSecs;	
	uint8 aFecErrors;
	uint8 aATUCFECErrors;
	uint8 aHECErrors;
	
	uint8 aCRCErrors;	
	uint8 aATUCCRCErrors;
	uint8 aLOF;
	
	uint8 reserved[1];	
	uint32  magic;
}showtimeAttr;

//add by renyu for stats.Showtime
#define TOTAL_MAGIC_ATTR 0x44331122 
typedef struct totalAttr
{
	uint8 aReceiveBlock;
	uint8 aTransmitBlock;
	uint8 aCellDelin;
	uint8 aLinkRetrain;
	
	uint8 aInitErrors;
	uint8 aInitTimeouts;
	uint8 aLossOfFraming;
	uint8 aErroredSecs;
	
	uint8 aSeverelyErroredSecs;	
	uint8 aFecErrors;
	uint8 aATUCFECErrors;
	uint8 aHECErrors;
	
	uint8 aCRCErrors;	
	uint8 aATUCCRCErrors;
	
	uint8 reserved[2];	
	uint32  magic;
}totalAttr;

//add by renyu for stats.LastShowtime
#define LASTSHOWTIME_MAGIC_ATTR 0x44331122 
typedef struct lastShowtimeAttr
{
	uint8 aReceiveBlock;
	uint8 aTransmitBlock;
	uint8 aCellDelin;
	uint8 aLinkRetrain;
	
	uint8 aInitErrors;
	uint8 aInitTimeouts;
	uint8 aLossOfFraming;
	uint8 aErroredSecs;
	
	uint8 aSeverelyErroredSecs;	
	uint8 aFecErrors;
	uint8 aATUCFECErrors;
	uint8 aHECErrors;

	uint8 aCRCErrors;	
	uint8 aATUCCRCErrors;
	
	uint8 reserved[2];	
	uint32  magic;
}lastShowtimeAttr;

//add by renyu for stats.CurrentDay
#define CURRENTDAY_MAGIC_ATTR 0x44331122 
typedef struct currentDayAttr
{
	uint8 aReceiveBlock;
	uint8 aTransmitBlock;
	uint8 aCellDelin;
	uint8 aLinkRetrain;
	
	uint8 aInitErrors;
	uint8 aInitTimeouts;
	uint8 aLossOfFraming;
	uint8 aErroredSecs;
	
	uint8 aSeverelyErroredSecs;	
	uint8 aFecErrors;
	uint8 aATUCFECErrors;
	uint8 aHECErrors;
	
	uint8 aCRCErrors;	
	uint8 aATUCCRCErrors;
	
	uint8 reserved[2];	
	uint32  magic;
}currentDayAttr;

//add by renyu for stats.QuarterHour
#define QUARTERHOUR_MAGIC_ATTR 0x44331122 
typedef struct quarterHourAttr
{
	uint8 aReceiveBlock;
	uint8 aTransmitBlock;
	uint8 aCellDelin;
	uint8 aLinkRetrain;
	
	uint8 aInitErrors;
	uint8 aInitTimeouts;
	uint8 aLossOfFraming;
	uint8 aErroredSecs;
	
	uint8 aSeverelyErroredSecs;	
	uint8 aFecErrors;
	uint8 aATUCFECErrors;
	uint8 aHECErrors;
	
	uint8 aCRCErrors;	
	uint8 aATUCCRCErrors;
	
	uint8 reserved[2];	
	uint32  magic;
}quarterHourAttr;
#define LANCONFIGSECURITY_MAGIC_ATTR 0x44332114
typedef struct lanconfigsecurityAttr
{
	uint8  aConfigPassword;	
	uint8  aX_TC_Username;		
	uint8  reserved[2];
	uint32 magic;
}lanconfigsecurityAttr;

#define LANDEVICE_MAGIC_ATTR	0x13121213
typedef struct lanDeviceAttr
{
	uint8  aLanEthIfNumOfEntries;
	uint8  aLanUSBIfNumOfEntries;
	uint8  aLanWLANConfigNumOfEntries;
#ifdef DHCP_PROFILE	
	uint8  aLanDHCPOptionNumOfEntries;
	uint8  aDHCPConditionalPoolNumOfEntries;
#endif	
	uint8  aDHCPServerCfg1;
	
	uint8  aDHCPServerEnable1;
	uint8  aDHCPRelay1;
	uint8  aMinAddress1;
	uint8  aMaxAddress1;
	
	uint8  aReservedAddr1;
	uint8  aSubnetMask1;
	uint8  aDNSServers1;
	uint8  aDomainName1;
	
	uint8  aIPRouters1;
	uint8  aDHCPLeaseTime1;	
	uint8  aIPInterfaceNum;	
	uint8  aX_TTNET_IGMPSnoopEnable;

#ifdef DHCP_PROFILE	
	uint8  Reserved[2];
#endif

	uint32 magic;  
}lanDeviceAttr;

#define IPINTERFACE_MAGIC_ATTR	0x13121213
typedef struct ipInterfaceAttr
{
	uint8  aEnable;
	uint8  aIPInterfaceIPAddress;
	uint8  aIPInterfaceSubnetMask;	
	uint8  aIPInterfaceAddressingType;
	
	uint32 magic;  
}ipInterfaceAttr;

#define ETHERINTERCFG_MAGIC_ATTR	0x13121213
typedef struct etherInterCfgAttr
{
	uint8  aStatus;
	uint8  aMACAddress;
	
	uint8  aBytesSent;
	uint8  aBytesReceived;
	uint8  aPacketsSent;
	uint8  aPacketsReceived;
#if defined(TCSUPPORT_LAYER2BRIDGING)
	uint8 aBridgeEnable;
	uint8 aBridgeName;
	uint8 aBridgeVLANID;
	uint8 aBridgeKey;

	uint8 aAvailableInterfaceKey;
	uint8 aAvailableInterfaceType;
	uint8 aAvailableInterfaceReference;
	uint8 aFilterBridgeReference;

	uint8 aFilterInterface;
	uint8 aFilterExclusivityOrder;
	uint8 aMarkingBridgeReference;
	uint8 aMarkingInterface;
	
	uint8 aMarkingVLANIDUntag;
	uint8  Reserved[1];
#else
	uint8  Reserved[2];	
#endif
	uint8  aUnicastPacketsSent;
	uint8  aUnicastPacketsReceived;
	uint8  aDiscardPacketsSent;
	uint8  aDiscardPacketsReceived;
	uint8  aMulticastPacketsSent;
	uint8  aMulticastPacketsReceived;

	uint32 magic;  
}etherInterCfgAttr;

//add by renyu for Hosts.Host.{i}.
#define HOST_MAGIC_ATTR 0x44331122 
typedef struct hostAttr
{
	uint8 aHostNumber;
	uint8 aIPAddress;
	uint8 aAddressSource;
	uint8 aLeaseTimeRemaining;

	uint8 aMACAddress;	
	uint8 aHostName;
	uint8 aInterfaceType;
	uint8 aActive;
		
	uint32  magic;
}hostAttr;

//add by yzwang_20100125 for QoS
#define QOS_MAGIC_ATTR	0x12121214
typedef struct qosAttr
{
	uint8  aQoSQueueInterface[4];
	uint8  aQoSDropAlgorithm[4];
	uint8  aQoSSchedulerAlgorithm[4];
	uint8  aQoSQueueEnable[4];

	uint8  aQoSProtocol[PKT_QOS_RULE_MAX];
	uint8  aQoSDSCPMark[PKT_QOS_RULE_MAX];
	uint8  aQoSForwardingPolicy[PKT_QOS_RULE_MAX];
	uint8  aQoSClassQueue[PKT_QOS_RULE_MAX];
	uint8  aQoSClassPolicer[PKT_QOS_RULE_MAX];
	uint8  aQoSClassificationEnable[PKT_QOS_RULE_MAX];
	uint8  aQosDestIP[PKT_QOS_RULE_MAX];
	uint8  aQosDestMask[PKT_QOS_RULE_MAX];
	uint8  aQosSourceIP[PKT_QOS_RULE_MAX];
	uint8  aQosSourceMask[PKT_QOS_RULE_MAX];
	uint8  aQosDestPort[PKT_QOS_RULE_MAX];
	uint8  aQoSSourcePort[PKT_QOS_RULE_MAX];
	uint8  aQoSSourceMACAddress[PKT_QOS_RULE_MAX];
	uint8  aQoSDestMACAddress[PKT_QOS_RULE_MAX];
	uint8  aQoSPolicerEnable[PKT_QOS_RULE_MAX];	
	uint8  aX_TTNET_QoSBandwidth;

	uint8 Reserved[295];//reserved for 10+64 parameters
	uint32  magic;  
}qosAttr;

//add by yzwang_20100204 for UserInterface
#define USERITF_MAGIC_ATTR 	0x12121215
typedef struct userInterfaceAttr
{
	uint8  aWarrantyDate;

	uint8 Reserved[31];//reserved for 23 parameters
	uint32  magic;  
}userInterfaceAttr;

#ifdef TR143
#define TR143_MAGIC_ATTR	0x11223344
typedef struct tr143Attr
{
	uint8	aDiagDldTsports;
	uint8	aDiagUpTsports;
	
	uint8 	aDldDiagnostics;
	uint8 	aDldInterface;
	uint8	aDldDownloadURL;
	uint8 	aDldDSCP;
	uint8 	aDldEthernetPriority;
	uint8 	aDldROMTime;
	uint8 	aDldBOMTime;
	uint8 	aDldEOMTime;
	uint8 	aDldTestBytesReceived;
	uint8 	aDldTotalBytesReceived;
	uint8 	aDldTCPOpenRequestTime;
	uint8 	aDldTCPOpenResponseTime;
	
	uint8 	aUldDiagnostics;
	uint8 	aUldInterface;
	uint8 	aUldUploadURL;
	uint8 	aUldDSCP;
	uint8 	aUldEthernetPriority;
	uint8 	aUldTestFileLength;
	uint8 	aUldROMTime;
	uint8 	aUldBOMTime;
	uint8 	aUldEOMTime;
	uint8 	aUldTotalBytesSent;
	uint8 	aUldTCPOpenRequestTime;
	uint8 	aUldTCPOpenResponseTime;

	uint8 	aUDPEchoEnable;
	uint8 	aUDPEchoInterface;
	uint8 	aEchoSourceIPAddress;
	uint8 	aUDPEchoUDPPort;
	uint8 	aUDPEchoPlusEnabled;
	uint8 	aUDPEchoPlusSupported;
	uint8 	aUDPEchoPacketsReceived;
	uint8 	aUDPEchoPacketsResponded;
	uint8 	aUDPEchoBytesReceived;
	uint8 	aUDPEchoBytesResponded;
	uint8 	aTimeFirstPacketReceived;
	uint8 	aTimeLastPacketReceived;

	uint32  magic;
}tr143Attr;
#endif

#ifdef DHCP_PROFILE
#define DHCPOPTION_MAGIC_ATTR 0x11223344
typedef struct dhcpOptionAttr
{
	uint8 aEnable;
	uint8 aTag;
	uint8 aValue;

	uint8 Reserved[1];	
	uint32  magic;  
}dhcpOptionAttr;

#define DHCPCONSER_MAGIC_ATTR 0x11223344
typedef struct dhcpConSerAttr
{
	uint8 aEnable;
	uint8 aPoolOrder;
	uint8 aSourceInterface;
	uint8 aVendorClassID;

	uint8 aVendorClassIDExclude;
	uint8 aVendorClassIDMode;
	uint8 aClientID;
	uint8 aClientIDExclude;

	uint8 aUserClassID;
	uint8 aUserClassIDExclude;
	uint8 aChaddr;
	uint8 aChaddrMask;

	uint8 aChaddrExclude;
	uint8 aLocallyServed;
	uint8 aMinAddress;
	uint8 aMaxAddress;

	uint8 aReservedAddresses;
	uint8 aSubnetMask;
	uint8 aDNSServers;
	uint8 aDomainName;

	uint8 aIPRouters;
	uint8 aDHCPLeaseTime;
	uint8 aUseAllocatedWAN;
	uint8 aAssociatedConnection;

	uint8 aDHCPServerIPAddress;
	uint8 aDHCPStaticAddressNum;
	uint8 aOptionNum;

	uint8 aconSerOption_Enable;
	uint8 aconSerOption_Tag;
	uint8 aconSerOption_Value;

	uint8 Reserved[2];	
	uint32  magic;  
}dhcpConSerAttr;
#endif



/*attribute struct definition.*/
extern 	deviceInfoAttr				tcDeviceInfoAttr;
extern 	cwmpAttr 					tcCwmpAttr;
extern	dslLinkAttr 				tcDslLinkAttr;
#if defined(TCSUPPORT_CWMP_VDSL)	
extern	ptmLinkAttr 				tcPtmLinkAttr;
#endif
extern 	wlanAttr					tcWlanAttr;
extern 	ipConAttr 					tcIpConAttr;
//add by brian for ip ping diagnostic
extern 	ipPingDiagnosticAttr 		tcIpPingDiagnosticAttrAttr;
#ifdef TCSUPPORT_TRACEROUTE
extern     traceRouteDiagnosticAttr 	tcTraceRouteDiagnosticAttrAttr;
#endif
//add by brian for dsl diagnostic
extern 	dslDiagnosticAttr 			tcdslDiagnosticAttr;
//add by brian for atm f5 loopback diagnostic
extern 	atmDiagnosticAttr 			tcatmDiagnosticAttr;
#ifdef TR111
extern 	tr111Attr 					tcTR111Attr;
#endif
#ifdef VOIP
extern 	tr104Attr 					tcTR104Attr;
#endif
extern 	timeAttr 					tcTimeAttr;
//add by renyu
extern 	dslInterCfgAttr 			tcdslInterCfgAttr;
extern 	dslComInterCfgAttr			tcdslComInterCfgAttr;
extern 	dslConnMgmtAttr				tcdslConnMgmtAttr;
extern 	dslConnMgmtSvrAttr			tcdslConnMgmtSvrAttr;
extern 	devConfigAttr 				tcDevConfigAttr;
//add by brian for layer3forwarding
extern layer3ForwardingAttr 		tclayer3ForwardingAttr;
extern lanconfigsecurityAttr 		tclanconfigsecurityAttr;
extern 	showtimeAttr				tcshowtimeAttr;
extern 	totalAttr					tctotalAttr;
extern 	lastShowtimeAttr			tclastShowtimeAttr;
extern 	currentDayAttr				tccurrentDayAttr;
extern 	quarterHourAttr				tcquarterHourAttr;

extern  lanDeviceAttr          		tcLanDeviceAttr;
extern  ipInterfaceAttr          	tcipInterfaceAttr;
extern  etherInterCfgAttr          	tcetherInterCfgAttr;
extern	hostAttr					tchostAttr;

extern	qosAttr			        	tcQoSAttr;
extern	userInterfaceAttr			tcUserInterfaceAttr;
#ifdef TR143
extern tr143Attr 					tcTr143Attr;
#endif

#ifdef DHCP_PROFILE
extern  dhcpOptionAttr          	tcdhcpOptionAttr;
extern  dhcpConSerAttr          	tcdhcpConSerAttr;
#endif


/***************************************************************************************************/
						/*TR069 attribute struct definition*/
/***************************************************************************************************/

void SetCfgNodeFlag( char *cfgNode, int nodeIndex );
cfgNode_ptr SearchCfgNode( char *cfgNodeName );
void ClearCfgNodeFlag( cfgNode_ptr pCfgNode );
int delayCommitCfgNode( char *cfgNode );
int tr069CommitCfgNode(cfgNode_ptr pCfgNode);
extern int HandleParametersSetting( cfgNode_ptr pCfgNode, int type, int delayFlag );

void InitAttrs(void);
void ReadAttr(attr_ptr tcAttr_ptr);
void ResetAttr(attr_ptr tcAttr_ptr);
void SaveAttr(attr_ptr tcAttr_ptr);

int cwmpInitParameterTree();
void cwmpFreeParameterTree(DeviceNodePtr rootPtr);
int8 getTheValue(char** value, ParameterType* type, DeviceNode*  deviceNode, char *valueString);

int8 GetTheAttributes(uint8 attribute,uint8* notification, AccessEntry** acl, DeviceNodePtr deviceNode);
int8 SetTheAttributes(uint8* attribute,
					  uint8 notificationChange, 
					  uint8 notification, 
					  uint8 accessListChange, 
					  AccessEntry* acl, 
					  DeviceNodePtr deviceNode);

int QuotationMarksExchange(char * SrcPtr, int SrcLength , char* DestPtr , int DestLength);
int RpHex2dec(char c);
uint8 Get_ACS_PVC_Number(void);
int8 cwmpGetVpiVciStr(char* pvcStr,  char* vpiStr, char* vciStr);
int CheckVPIVCI(int pvcIndex);
int8 cwmpGetVpiVciStrFromList(char* pvcStr,  char* vpiStr, char* vciStr, int *len);
uint8 GetVcPoolProbeAction(void);
int InternalClientIpCheck(char *st);
int SubnetLegalCheck(char *mask);
int getWANLinkIndex(DeviceNode*  deviceNode);
int getWANConnectionIndex(DeviceNode*  deviceNode);
int getWANNodeIndexForPortMapping(DeviceNode*  deviceNode);
int getPortMappingIndex(DeviceNode*  deviceNode);

int updateConfigFile( cfgNode_ptr pCfgNode );

int RouteCommitCfgNode( cfgNode_ptr pCfgNode );
int queueManagementCommit(cfgNode_ptr pCfgNode);

#ifdef TR143
int tr143UDPEchoCommitFunc(cfgNode_ptr pCfgNode);
#endif

int  _checkIp(char *ipstr);
BOOLEAN ValidateChecksum(UINT PIN);
#ifdef TR143
int8 setDldUldDiagStateType(char * value);
char * getDldUldDiagStateType(void);
#endif

typedef struct RouteEntry_s
{
	char DST_IP[16];
	char GateWay[16];
	char Sub_Mask[16];
	char Device[16];
}RouteEntry_t;
int SearchRouteByDesIP(char *desIp, RouteEntry_t *re);
int checkCPEIP(char *ipaddr);
int checkPortValid( uint32 portIn );

#ifdef DHCP_PROFILE
int8 DHCPProfileCheckIP(cfgNode_ptr pCfgNode);
#endif

int8 ADSLPARASETTING(cfgNode_ptr pCfgNode);
#endif

