
/*
** $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/private/TR69_64/cwmp/Includes/cwmpParameter.h#1 $
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


#define CWMP_BOOLEAN   		 (BooleanType)	
#define CWMP_INTEGER     		 (IntType)	
#define CWMP_UNSIGNEDINT   		 (UnsignedIntType)	
#define CWMP_STRING     			 (StringType)	
#define CWMP_OBJECT   		       (ObjectType)	
#define CWMP_DATE_TIME    	       (DateTimeType)	
#define CWMP_BASE64     			 (Base64Type)	

#define TRUE 1
#define FALSE 0

//add by brian for ip ping diagnostic
#define IPPINGDIAGNOSTICFILE "/tmp/cwmp/ippingdiagnostic_stat"
#define IMCOMPLETEFLAG "0 packets received"
#define MAX_PINGBUF_LEN 2048 //for read ip ping dignostic result
//add by brian for dsl diagnostic
#define CWMPDSLDIAGNOSTIC "CDSLDiagnostic_Common"

#define ReadOnly  0x00
#define Writable 0x01
#define SEEK_SET 0
#define QUOTATIONMARKSLEN		140    /* the length use for QuotationMarksExchange function */
#define DNS_INFO_PATH	"/etc/resolv.conf"

#define MAX_STRING_LENGTH 256
#define MAX_VIRSERV_RULE 10
#define MAX_LEN_OF_SUPPORTED_RATES 12
#define VOIP  		1 //add by renyu_20091225

#define CWMP_PURE_BRIDGE_TYPE_IP_BRIDGED (1 << 2)
/*-----------------------------TR069 cfg node flag--------------------------------*/
#define CFG_NODE_WAN						(1 << 0)
#define CFG_NODE_WLAN						(1 << 1)
#define CFG_NODE_CWMP						(1 << 2)
#define CFG_NODE_AUTOPVC					(1 << 3)
#define CFG_NODE_VIRSERVER					(1 << 4)
#define CFG_NODE_LAN						(1 << 5)
#define CFG_NODE_DPROXY					(1 << 6)
#ifdef TR111
#define CFG_NODE_LIMITTIME					(1 << 7)
#endif
#ifdef VOIP
#define CFG_NODE_VOIPBASIC					(1 << 8)
#define CFG_NODE_VOIPMEDIA				(1 << 9)
#define CFG_NODE_VOIPSUPPORTEDCODEC		(1 << 10)
#endif

/*wan Encap definition. dynamic/static/pppoe/pppoa/pure bridge mode*/
#define WAN_ENCAP_DYN			"0"
#define WAN_ENCAP_STATIC		"1"
#define WAN_ENCAP_PPP			"2"
#define WAN_ENCAP_BRIDGE		"3"

#define SEC_PER_DAY 86400 /*60*60*24  for ppp link-up time*/
#define SEC_PER_HOUR 3600 /*60*60         shnwind 2008.4.14*/
#define SEC_PER_MIN 60




//add by brian for ip ping diagnostic
typedef struct ping_s
{
	ip4a target;	/*Starting target Ip address*/
	uint32 interval;/*Inter-ping interval(s)*/
	uint32 sent;	/*Total number of sent*/
	uint32 responses;/*Total number of responses*/
	uint16 len;		/*Length of data portion of ping*/
	uint32 maxrtt;	/*Maximun rtt*/
	uint32 minrtt;	/*Minimun rtt*/
	uint32 avrrtt;	/*Average rtt*/	
}ping_t;

typedef struct cfgNode_s
{
	char *nodeName;
	int fIndex;
	uint32 fBit;
}cfgNode_t,*cfgNode_ptr;

#define 	CFGNODEFLAG0         0
#define 	CFGNODEFLAG1		1
typedef struct attr_s
{
	char *attrName;
	void *attrPtr;
	int attrLen;
	uint32 magic;
	int fIndex;
	uint32 fBit;
}attr_t, *attr_ptr;

#define  	ATTRFLAG0 			0
#define 	ATTRFLAG1			1


#define MAX_WAN_NODE 				8
#define MAX_VCPOOL_NUM 			16
#define MAX_PM_RULE				10

/*TR069 attribute struct flag.*/
#define	CWMP_ATTR_FLAG					(1 << 0)
#define DSLLINK_ATTR_FLAG					(1 << 1)
#define WLAN_ATTR_FLAG						(1 << 2)
#define IPCON_ATTR_FLAG						(1 << 3)
//add by brian for ip ping diagnostic
#define IPPINGDIAGNOSTIC_ATTR_FLAG			(1 << 4)
//add by brian for dsl diagnostic
#define DSLDIAGNOSTIC_ATTR_FLAG			(1 << 5)
//add by brian for atm f5 loopback diagnostic
#define ATMDIAGNOSTIC_ATTR_FLAG			(1 << 6)
#ifdef TR111
#define TR111_ATTR_FLAG						(1 << 7)
#endif
#ifdef VOIP
#define TR104_ATTR_FLAG						(1 << 8)
#endif

#define MAX_ATTR_LEN 						512
#define MAX_ATTR_OFFSET 					9

/*TR069 attribute struct definition*/

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
	
	/*  there are 12 bytes used,
	 ** if you add other attribute,please update the stats.
	  */
	  
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
	
	/*	there are 116 bytes used,
	 **  if you add other attribute,please update the stats.
	  */
	  
	uint8 Reserved[80];
	
	uint32  magic;  //This variable should be at the end of the structure. 
}dslLinkAttr;
/*InternetGatewayDevice.LanDevice.1.WLANConfiguration.{i}.*/
#define WLAN_MAGIC_ATTR 	0x22335566
typedef struct wlanAttr
{
	uint8  aWLanEnable[4];	
 	uint8  aWLanMaxBitRate[4];	
 	uint8  aWLanChannel[4];
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

	uint8  aWLanPossibleChannels[4];
	uint8  aWLanPossibleDataTransmitRates[4];
	uint8  aWLanTotalBytesSent[4];
	uint8  aWLanTotalBytesReceived[4];
	
	uint8  aWLanTotalPacketsSent[4];
	uint8  aWLanTotalPacketsReceived[4];

	uint8  aWLanTotalAssociations[4];

	uint8  aWLanAssociatedDevMACAddr[4];
	uint8  aWLanAssociatedDevIPAddr[4];
	uint8  aWLanAssociatedDevAuthState[4];

	/*	there are 132 bytes used,
	 **	if you add other attributes, please update the stats.
	  */
	uint8 Reserved[60];
	
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
	
	/*	there are 320 bytes used.
	 **	if add other attributes,please update the stats.
	  */
	  
	uint8 Reserved[64];
		
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

void InitAttrs(void);
void ReadAttr(attr_ptr tcAttr_ptr);
void ResetAttr(attr_ptr tcAttr_ptr);
void SaveAttr(attr_ptr tcAttr_ptr);

int cwmpInitParameterTree();
void cwmpFreeParameterTree(DeviceNodePtr rootPtr);

int	cwmpInitManagementServerSubTree( DeviceNodePtr deviceNode );
int	cwmpInitLANDeviceSubTree( DeviceNodePtr deviceNode );
int	cwmpInitWANDeviceSubTree( DeviceNodePtr deviceNode );
int cwmpInitWANConnectionDeviceTable( DeviceNodePtr deviceNode );
int cwmpInitAssociatedDeviceTable( DeviceNodePtr deviceNode);
int cwmpInitWEPKeyTable( DeviceNodePtr deviceNode );
int cwmpInitPreSharedKey( DeviceNodePtr deviceNode );
void cwmpFreeDeviceTable(DeviceNodePtr deviceNode);

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
int getWANLinkIndex(DeviceNode*  deviceNode);
int getWANConnectionIndex(DeviceNode*  deviceNode);
int getWANNodeIndexForPortMapping(DeviceNode*  deviceNode);
int getPortMappingIndex(DeviceNode*  deviceNode);

/************************************* WLan relation function **************************************/
int	cwmpInitWLANConfiguration1SubTree( DeviceNodePtr deviceNode );
int	cwmpInitWLANTable( DeviceNodePtr deviceNode);

int8 cwmpAddConnectionObject(uint32* num, DeviceNode*  deviceNode);
int8 cwmpDeleteConnectionObject(uint32 num, DeviceNode*  deviceNode);

int8 getWLANCfgEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgBSSIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgBSSIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgBSSIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgMaxBitRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgMaxBitRateValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgMaxBitRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgMaxBitRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgChannelValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgChannelValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgChannelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgChannelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgSSIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgSSIDValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgSSIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgSSIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgBeaconTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgBeaconTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgBeaconTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgBeaconTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgMACAddressControlEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgMACAddressControlEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgMACAddressControlEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgMACAddressControlEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgStandardValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgStandardAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgStandardAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgWEPKeyIndexValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgWEPKeyIndexValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgWEPKeyIndexAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgWEPKeyIndexAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getWLANCfgWEPEncryptionLevelValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgWEPEncryptionLevelAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgWEPEncryptionLevelAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgBasicEncryptionModesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgBasicEncryptionModesValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgBasicEncryptionModesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgBasicEncryptionModesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgBasicAuthenticationModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgBasicAuthenticationModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgBasicAuthenticationModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgBasicAuthenticationModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgWPAEncryptionModesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgWPAEncryptionModesValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgWPAEncryptionModesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgWPAEncryptionModesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgWPAAuthenticationModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgWPAAuthenticationModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgWPAAuthenticationModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgWPAAuthenticationModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgPossibleChannelsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgPossibleChannelsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgPossibleChannelsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgBasicDataTransmitRatesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgBasicDataTransmitRatesValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgBasicDataTransmitRatesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgBasicDataTransmitRatesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgOperationalDataTransmitRatesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgOperationalDataTransmitRatesValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgOperationalDataTransmitRatesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgOperationalDataTransmitRatesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgPossibleDataTransmitRatesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgPossibleDataTransmitRatesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgPossibleDataTransmitRatesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgRadioEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgRadioEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgRadioEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgRadioEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgAutoRateFallBackEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWLANCfgAutoRateFallBackEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWLANCfgAutoRateFallBackEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgAutoRateFallBackEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgTotalBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgTotalBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgTotalBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgTotalBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgTotalBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgTotalBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgTotalPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgTotalPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgTotalPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgTotalPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgTotalPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgTotalPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWLANCfgTotalAssociationsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWLANCfgTotalAssociationsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWLANCfgTotalAssociationsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

//#ifndef CD_WEBPAGE
int8 getAssociatedDeviceMACAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAssociatedDeviceMACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAssociatedDeviceMACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAssociatedDeviceIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAssociatedDeviceIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAssociatedDeviceIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAssociatedDeviceAuthenticationStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAssociatedDeviceAuthenticationStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAssociatedDeviceAuthenticationStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
//#endif

int8 getWEPKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWEPKeyValue(void* value, struct DeviceNode*  deviceNode);
int8 getWEPKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWEPKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPreSharedKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPreSharedKeyValue(void* value, struct DeviceNode*  deviceNode);
int8 getPreSharedKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPreSharedKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
/*--------------------------------------------WLan relation function end-----------------------------------------*/
/*********************************************** Wan relation function *******************************************/
int8 DeviceNodeAddPVCObject(uint32 *num, DeviceNode*  deviceNode);
int8 DeviceNodeDelPVCObject(uint32 num, DeviceNode*  deviceNode);

int8 getWANIPCntNumOfEntries1_1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCntNumOfEntries1_1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCntNumOfEntries1_1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPPPCntNumOfEntries1_1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPPPCntNumOfEntries1_1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCntNumOfEntries1_1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int  cwmpInitWANDSLLinkConfigTable( DeviceNodePtr deviceNode );

int8 getWANDSLLinkCfg1EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1LinkStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1LinkStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1LinkStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1LinkTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1LinkTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1LinkTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1LinkTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1AutoConfigValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1AutoConfigAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1AutoConfigAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ModulationTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ModulationTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ModulationTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1DestinationAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1DestinationAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1DestinationAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1DestinationAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMEncapsulationValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1ATMEncapsulationValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMEncapsulationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMEncapsulationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1FCSPreservedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1FCSPreservedValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1FCSPreservedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1FCSPreservedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getWANDSLLinkCfg1VCSearchListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1VCSearchListValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1VCSearchListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1VCSearchListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMTransmittedBlocksValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMTransmittedBlocksAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMTransmittedBlocksAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMReceivedBlocksValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMReceivedBlocksAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMReceivedBlocksAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMQoSValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1ATMQoSValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMQoSAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMQoSAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMPeakCellRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1ATMPeakCellRateValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMPeakCellRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMPeakCellRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMMaximumBurstSizeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1ATMMaximumBurstSizeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMMaximumBurstSizeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMMaximumBurstSizeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMSustainableCellRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1ATMSustainableCellRateValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMSustainableCellRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMSustainableCellRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1AAL5CRCErrorsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1AAL5CRCErrorsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1AAL5CRCErrorsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMCRCErrorsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMCRCErrorsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMCRCErrorsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMHECErrorsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMHECErrorsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMHECErrorsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int  cwmpInitWANIPConnectionTable( DeviceNodePtr deviceNode );

int8 getWANCnt1_1EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1StatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1StatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1StatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1PsbTypesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1PsbTypesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1PsbTypesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1TypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1TypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1TypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1TypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1NameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1NameValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1NameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1NameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1UptimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1UptimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1UptimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1LErrorValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1LErrorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1LErrorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANIPCnt1_1RSIPAvailableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1RSIPAvailableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1RSIPAvailableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1NATEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1NATEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1NATEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1NATEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANIPCnt1_1AddrTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANIPCnt1_1AddrTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1AddrTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1AddrTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1ExtIPAddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1ExtIPAddrValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1ExtIPAddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1ExtIPAddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANIPCnt1_1SubMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANIPCnt1_1SubMaskValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1SubMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1SubMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANIPCnt1_1DefGatewayValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANIPCnt1_1DefGatewayValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1DefGatewayAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1DefGatewayAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1DNSEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1DNSEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1DNSEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1DNSEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1DNSOvAllowedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1DNSOvAllowedValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1DNSOvAllowedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1DNSOvAllowedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1DNSServersValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1DNSServersValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1DNSServersAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1DNSServersAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1MACAddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1MACAddrValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1MACAddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1MACAddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1CntTriggerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1CntTriggerValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1CntTriggerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1CntTriggerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1RProRxValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1RProRxValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1RProRxAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1RProRxAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1PMapNumOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1PMapNumOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1PMapNumOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPPMapping1EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPMapping1EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPMapping1EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPMapping1EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPPMapping1LeaseDurValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPMapping1LeaseDurValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPMapping1LeaseDurAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPMapping1LeaseDurAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIP1RemoteHostValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIP1RemoteHostValue(void* value, struct DeviceNode*  deviceNode);
int8 getIP1RemoteHostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIP1RemoteHostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIP1ExternalPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIP1ExternalPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getIP1ExternalPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIP1ExternalPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPExternalPortEndRangeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPExternalPortEndRangeValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPExternalPortEndRangeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPExternalPortEndRangeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIP1InternalPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIP1InternalPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getIP1InternalPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIP1InternalPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPPMapping1ProtocolValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPMapping1ProtocolValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPMapping1ProtocolAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPMapping1ProtocolAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIP1InternalClientValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIP1InternalClientValue(void* value, struct DeviceNode*  deviceNode);
int8 getIP1InternalClientAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIP1InternalClientAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPPMapping1DescripValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPMapping1DescripValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPMapping1DescripAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPMapping1DescripAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int	cwmpInitWANEtherStats( DeviceNodePtr deviceNode );

int8 getEthernetBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthernetBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthernetPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthernetPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int  cwmpInitWANPPPConnectionTable( DeviceNodePtr deviceNode );

int8 getWANPPPCnt1_1UsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1UsernameValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1UsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1UsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPPPCnt1_1PasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1PasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1PasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1PasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPPPCnt1_1TransTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1TransTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1TransTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

//#ifdef TC_TR069
int8 getWANPPPCnt1_1AuthenTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1AuthenTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1AuthenTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1AuthenTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
//#endif

int8 getWANPPPCnt1_1PPPoEACNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1PPPoEACNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1PPPoEACNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1PPPoEACNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPPPCnt1_1PPPoESvcNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1PPPoESvcNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1PPPoESvcNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1PPPoESvcNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 DeviceNodeAddPMappingObject(uint32* num, DeviceNode*  deviceNode);

int	cwmpInitWANGetPortMappingFromFlash( DeviceNodePtr deviceNode );

int8 DeviceNodeDelPMappingObject(uint32 num, DeviceNode*  deviceNode);


/*-----------------------------------------------Wan relation function end ------------------------------------*/
/*******************************************cwmp relation function start *******************************/
int8 getEnableCWMPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEnableCWMPValue(void* value, struct DeviceNode*  deviceNode);
int8 getEnableCWMPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEnableCWMPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setURLValue(void* value, struct DeviceNode*  deviceNode);
int8 getURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUsernameValue(void* value, struct DeviceNode*  deviceNode);
int8 getUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8   setPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8   setPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPeriodicInformEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPeriodicInformEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getPeriodicInformEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPeriodicInformEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPeriodicInformIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPeriodicInformIntervalValue(void* value, struct DeviceNode*  deviceNode);
int8 getPeriodicInformIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPeriodicInformIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPeriodicInformTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPeriodicInformTimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getPeriodicInformTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPeriodicInformTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getParameterKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getParameterKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setParameterKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getConnectionRequestURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getConnectionRequestURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConnectionRequestURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getConnectionRequestUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setConnectionRequestUsernameValue(void* value, struct DeviceNode*  deviceNode);
int8 getConnectionRequestUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConnectionRequestUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getConnectionRequestPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setConnectionRequestPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getConnectionRequestPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConnectionRequestPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUpgradesManagedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUpgradesManagedValue(void* value, struct DeviceNode*  deviceNode);
int8 getUpgradesManagedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUpgradesManagedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*-----------------------------------------------cwmp relation function end -----------------------------------*/

//add by brian for ip ping diagnostic
/***************************************ip ping diagnostic relation function begin***************************/
int8 getDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPPingInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPingInterfaceValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPingInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPingInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPPingHostValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPingHostValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPingHostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPingHostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getNumberOfRepetitionsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setNumberOfRepetitionsValue(void* value, struct DeviceNode*  deviceNode);
int8 getNumberOfRepetitionsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setNumberOfRepetitionsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPPingTimeoutValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPingTimeoutValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPingTimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPingTimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDataBlockSizeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDataBlockSizeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDataBlockSizeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDataBlockSizeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDSCPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDSCPValue(void* value, struct DeviceNode*  deviceNode);
int8 getDSCPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDSCPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSuccessCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSuccessCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSuccessCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFailureCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFailureCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFailureCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAverageResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAverageResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAverageResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMinimumResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMinimumResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMinimumResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMaximumResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMaximumResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaximumResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*--------------------ip ping diagnostic relation function end-------------------------*/

//add by brian for dsl diagnostic
/*----------------------dsl diagnostic relation function begin----------------------*/
int8 getWANDSLDiagnositcs_LoopDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLDiagnositcs_LoopDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_LoopDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_LoopDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_ACTPSDdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_ACTPSDdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_ACTPSDdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_ACTPSDusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_ACTPSDusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_ACTPSDusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_ACTATPdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_ACTATPdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_ACTATPdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_ACTATPusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_ACTATPusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_ACTATPusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLINSCdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLINSCdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLINSCdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLINpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLINpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLINpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_QLNpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_QLNpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_QLNpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_SNRpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_SNRpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_SNRpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_BITSpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_BITSpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_BITSpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_GAINSpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_GAINSpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_GAINSpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
/*--------------------dsl diagnostic relation function end-----------------------------*/

//add by brian for atm f5 loopback diagnostic
 /*------------------atm f5 loopback diagnostic relation function begin------------------*/ 
int8 getWANATMF5LoopbackDiagnostics_DiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANATMF5LoopbackDiagnostics_DiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_DiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_DiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANATMF5LoopbackDiagnostics_NumberOfRepetitionsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANATMF5LoopbackDiagnostics_NumberOfRepetitionsValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_NumberOfRepetitionsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_NumberOfRepetitionsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANATMF5LoopbackDiagnostics_TimeoutValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANATMF5LoopbackDiagnostics_TimeoutValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_TimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_TimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANATMF5LoopbackDiagnostics_SuccessCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_SuccessCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_SuccessCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANATMF5LoopbackDiagnostics_FailureCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_FailureCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_FailureCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANATMF5LoopbackDiagnostics_AverageResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_AverageResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_AverageResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANATMF5LoopbackDiagnostics_MinimumResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_MinimumResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_MinimumResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANATMF5LoopbackDiagnostics_MaximumResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_MaximumResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_MaximumResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
 /*-----------------atm f5 loopback diagnostic relation function end--------------------*/
#ifdef TR111
int cwmpInitManageableDeviceSubTree(DeviceNodePtr deviceNode);
int8 getmDevNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getmDevNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setmDevNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getmDevNotificationLimitValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setmDevNotificationLimitValue(void* value, struct DeviceNode*  deviceNode);
int8 getmDevNotificationLimitAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setmDevNotificationLimitAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getmDevOUIValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getmDevOUIAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 getmDevSNValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getmDevSNAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 getmDevClassValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getmDevClassAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 RebuildmDevObject();
#endif
#ifdef VOIP
int  cwmpInitServicesSubTree( DeviceNodePtr deviceNode );
int  cwmpInitVoiceServiceSubTree( DeviceNodePtr deviceNode );
int  cwmpInitCapabilitiesSubTree( DeviceNodePtr deviceNode );
int	cwmpInitVoiceProfileSubTree( DeviceNodePtr deviceNode );
int	cwmpInitVoiceProfileRTPSubTree( DeviceNodePtr deviceNode );
int	cwmpInitVoiceProfileSIPSubTree( DeviceNodePtr deviceNode );
int	cwmpInitLineSubTree( DeviceNodePtr deviceNode );
int	cwmpInitLineSIPSubTree( DeviceNodePtr deviceNode );
int	cwmpInitLineVoiceProcessingSubTree( DeviceNodePtr deviceNode );
int	cwmpInitLineCodecSubTree( DeviceNodePtr deviceNode );
int	cwmpInitLineCodecListSubTree( DeviceNodePtr deviceNode );
int8 getVoiceProfileNmberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getVoiceProfileNmberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVoiceProfileNmberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getMaxProfileCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMaxProfileCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxProfileCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getMaxLineCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMaxLineCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxLineCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getFaxT38Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFaxT38Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFaxT38Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getLocalPortMinValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLocalPortMinValue(void* value, struct DeviceNode*  deviceNode);
int8 getLocalPortMinAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLocalPortMinAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getSIPProxyServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPProxyServerValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPProxyServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPProxyServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getSIPProxyServerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPProxyServerPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPProxyServerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPProxyServerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getSIPProxyServerTransportValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPProxyServerTransportValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPProxyServerTransportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPProxyServerTransportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getSIPRegistrarServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPRegistrarServerValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPRegistrarServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPRegistrarServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getSIPRegistrarServerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPRegistrarServerPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPRegistrarServerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPRegistrarServerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getSIPRegistrarServerTransportValue(char** value, ParameterType* type, DeviceNode*  deviceNode);	
int8 setSIPRegistrarServerTransportValue(void* value, struct DeviceNode*  deviceNode);	
int8 getSIPRegistrarServerTransportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPRegistrarServerTransportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getOutboundProxyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setOutboundProxyValue(void* value, struct DeviceNode*  deviceNode);
int8 getOutboundProxyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setOutboundProxyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getOutboundProxyPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setOutboundProxyPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getOutboundProxyPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setOutboundProxyPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getProfileFaxT38Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setProfileFaxT38Value(void* value, struct DeviceNode*  deviceNode);
int8 getProfileFaxT38Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setProfileFaxT38Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getLineEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLineEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getLineEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getLineSIPAuthUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLineSIPAuthUserNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getLineSIPAuthUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineSIPAuthUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getLineSIPAuthPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLineSIPAuthPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getLineSIPAuthPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineSIPAuthPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getEchoCancellationEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEchoCancellationEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getEchoCancellationEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEchoCancellationEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getCodecValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCodecAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCodecAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getPacketizationPeriodValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPacketizationPeriodValue(void* value, struct DeviceNode*  deviceNode);
int8 getPacketizationPeriodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPacketizationPeriodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getPriorityValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPriorityValue(void* value, struct DeviceNode*  deviceNode);
int8 getPriorityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPriorityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
//add by brian for dsl diagnostic
int8 NT_ADSL_def_Diagnostics_poll(char *attrName,char *p_diagnostic_set_buf);

#endif

