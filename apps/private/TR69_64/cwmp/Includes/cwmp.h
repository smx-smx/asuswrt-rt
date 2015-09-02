
/*
** $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/private/TR69_64/cwmp/Includes/cwmp.h#2 $
*/
/************************************************************************
 *
 *	Copyright (C) 2005 Trendchip Technologies, Corp.
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
#ifndef __CWMP_H__
#define __CWMP_H__
//#include "AsEngine.h"
//#include "WcProtos.h"
//#include "Global.h"
//#include "wcClient.h"

//#include "Global_res.h"



/************************************************************************
*                        			MACRO							
************************************************************************
*/

#define CwmpInformPeriodOndDay "86400"     //sec
#define CwmpURLMaxLen 256
#define CwmpWebClientBufferSize 1600 //512->1024->1600 ,hqfeng,2006.03.02

#define CWMP_REDUCE_CODE  0  //Added for reduce code, hqfeng, 2006.08.07

#define CWMP_MAGIC_NUMBER 6964

#define CWMP_FIRMWARE 1
#define CWMP_WEB_CONTENT 2
#define CWMP_CONFIGURATION_FILE 3



#define CWMP_MAC2NAME_DISABLE			0
#define CWMP_MAC2NAME_ENABLE			1
#define CWMP_LOAD_DEFAULT_FLAG_CLEAR	0
#define CWMP_LOAD_DEFAULT_FLAG_SET		1

/*add by sam 20080707 begin*/
#define TR69_OUI_LEN 3
/*add by sam 20080707 end*/

/*add by brian====begin*/
//pthread_mutex_t value_change_lock;

#define SYSROUTE_CMD "/sbin/route -n > /tmp/cwmproute"
#define SYSROUTE_PATH "/tmp/cwmproute"
/*add by brian====end*/

#define NASNAME "nas"
#define PPPNAME "ppp"
#define PVCFLAG "UG"
	
//add by bria for atm f5 loopback diagnostic
#define ATM_F5LOOPBACK_DIAGNOSTIC_PATH "/proc/tc3162/atm_f5_loopback_diagnostic"
#define ATM_F5LOOPBACK_DIAGNOSTIC_RESET_PATH "/proc/tc3162/atm_f5_loopback_reset_diagnostic"

//modify by brian
//#if defined(CSE_TANGO) ||defined (SZTELE_WORKSSYS)||defined(MAX_TR69_PARAM)
typedef enum
{
	eDiagnosticsStateNone,
	eDiagnosticsStateComplete,
	eDiagnosticsStateRequested,
	eDiagnosticsStateError_internal,
	eDiagnosticsStateError_other
} F5DiagnosticsState;
typedef struct atmF5LoopbackDiagnostis_s{
	F5DiagnosticsState DiagnosticsState;
	uint32 NumberOfRepetitions;
	uint32 Timeout;
	uint32 SuccessCount;
	uint32 FailureCount;
	uint32 AverageResponseTime;
	uint32 MinimumResponseTime;
	uint32 MaximumResponseTime;
} atmF5LoopbackDiagnostis_t;
//#endif
/*
	TransferComplete (0x01)
	InternalError = 9002 (0x01<<1)
	DownloadFailure = 9010 (0x01<<2)
	FileTransferAuthenFailure = 9012 (0x01<<3)
	UnsupportedProtocol = 9013 (0x01<<4)
*/
#define EVENT_NoTransferComplete (0x00)
#define EVENT_TransferComplete (0x01)
#define TransferComplete_Error9002  (0x01<<1)
#define TransferComplete_Error9010 (0x01<<2)
#define TransferComplete_Error9012  (0x01<<3)
#define TransferComplete_Error9013 (0x01<<4)
#define TransferComplete_Error9011 (0x01<<5)
#define ErrorCodeMask (0x3e)      //ErrorCodeMask (0x1e)

int nmInitCi(void);
void cwmpInit(void);
char * getCwmpUrl(void);
//char* cwmpGetURL(void);
int setCwmpUrl(char *value);
char * getCwmpPath(void);
void  setCwmpPath(char *value);//added by hqfeng,2006.06.06
int  getCwmpPort(void);
void  setCwmpPort(int value);//added by hqfeng,2006.06.06
char * getCwmpManufacture(void);
char * getCwmpOUI(void);
char * getCwmpProductClass(void);
char * getCwmpSerialNum(void);
char * getHardwareVer(void);
char * getUpDateTime(void);
char * getDefaultWANExternalIPAddr(void);
char * getConRequestURL(void);
//void cwmpResetRequestBuffer(wcRequestPtr);
char * getCwmpUserName(void);
char * getCwmpPassword(void);
uint8 cwmpGetAuthenFlag(void);
void cwmpSetAuthenFlag(uint8 value);
void cwmpResetAuthenFlag(void);


#ifndef CODE_SHRINK  //qingpu.20080110
uint8 cwmpGetSerialNumFlag(void);
#endif
int cwmpGetInformTimerID(void);
void cwmpSetInformTimerID(int value);
uint32 GetInformTaskID(void);
void cwmpBootInform(void);
//added by xyzhu_20091209
#ifdef TCSUPPORT_WLAN
void wifiMacTabChanged(void);
void wifiMacTabTimerStart(void);
#endif
void cwmpPeriodInform(void);
int cwmpBuildInformPkt(void);
int cwmpInformTaskStart(void);

void *cwmpInformTask(void);

#ifndef THREAD_REDUCE
int cwmpValueChangeTaskStart(void);
//modify by brian
void *cwmpValueChangeTask(void);
uint32 GetValueChangeTaskID(void);
#else
int cwmpValueChangeRoutine(void);
#endif

boolean getCwmpInitFlag(void);
void setCwmpInitFlag(boolean value);

void  setInformPeriod(uint32 value);
uint32  getInformPeriod(void);

void  setInformPeriodEnable(boolean value);
boolean  getInformPeriodEnable(void);

#if defined(TCSUPPORT_RANDOM_INFORM)
boolean getRandomInformFlag(void);
#endif

uint8 getIP_BridgedTypeFlag(void);
void setIP_BridgedTypeFlag(uint8 value);

#if !defined(TCSUPPORT_CT) 
int cwmpSetCwmpUrl(char * URL,int flag);
#endif
//int cwmpSetCwmpUrl(char * URL);

#ifdef CWMP_REDIRECT
int cwmpSetRedirectCwmpUrl(char *URL);
char* GetCwmpRedirectURL(void);
uint8 GetCwmpRedirectCnt(void);
void cwmpCalculateReirectCnt(void);
void ClearCwmpRedirectCnt(void);
void SetCwmpRedirectFlag(uint8 flag);
uint8 GetCwmpRedirectFlag(void);
uint8 GetCwmpRedirectHttpFlag(void);
Unsigned32 GetCwmpRedirectHostAddress(void);
Unsigned16 GetCwmpRedirectPort(void);
char* GetCwmpRedirectPath(void);
char* GetCwmpRedirectHostName(void);
uint8 GetCwmpRedirectConnReqFlag(void);
void SetCwmpRedirectConnReqFlag(uint8 value);
#endif /*CWMP_REDIRECT*/
uint8 cwmpCheckValidURL(char* URL);

void setCPEPath(char* value);
char* getCPEPath(void);

int setCPEPort(uint32 value);
uint32 getCPEPort(void);

char* getCPEUserName(void);
void setCPEUserName(char* value);

#ifdef CT_COM_DEVICEREG
char* getDeviceregUserName(void);
char* getDeviceregUserPasswd(void);
#endif

char* getCPEPassword(void);
void setCPEPassword(char* value);

uint8 GetRetryCount(void);

#ifndef CODE_SHRINK  //qingpu.20080110
void SetRetryCount(uint8 value);

uint8 getWaitingReceiveFlag(void);
void setWaitingReceiveFlag(uint8 value);

uint8 getCwmpLanTestFlag(void);
void setCwmpLanTestFlag(uint8 value);
#endif

//void cwmpResetSession(wcHttpSessionPtr  theSessionPtr);

#ifndef CODE_SHRINK  //qingpu.20080110
uint8 cwmpGetSessionCompleteFlag(void);
void cwmpSetSessionCompleteFlag(uint8 value);
#endif

//modify by brian
int cwmpPerformWANADSLDiagnostics(void);
int cwmpPerformF5LoopbackDiagnostics(void);

uint8 cwmpGetF5LoopbackDiagnosticsResetFlag(void);
void cwmpSetF5LoopbackDiagnosticsResetFlag(uint8 value);

uint8 cwmpGetF5LoopbackDiagnosticsWannodeindex(void);
void cwmpSetF5LoopbackDiagnosticsWannodeindex(uint8 value);

uint8 cwmpGetF5LoopbackDiagnosingFlag(void);
void cwmpSetF5LoopbackDiagnosingFlag(uint8 value);

uint8 cwmpGetF5LoopbackDiagnosticsWannodeindex(void);
void cwmpSetF5LoopbackDiagnosticsWannodeindex(uint8 value);

void cwmpSetF5LoopbackDiagnosticsNumOfRep(uint32 NumOfRepetions, uint8 wannode);
void cwmpSetF5LoopbackDiagnosticsTimeout(uint32 Timeout, uint8 wannode);

#ifdef TR143
int cwmpPerformDownloadDiagnostics(void);
uint8 cwmpInitDiagnosticsStruct(void);
#endif
int  getUnLockRealmsUserFlag(void);
void setUnLockRealmsUserFlag(int value);

uint8 getHttpsFlag(void);
void setHttpsFlag(uint8 value);
//uint8 getSessionClearFlag(void);
//void setSessionClearFlag(uint8 value);
uint8 getSendHostPortNumFlag(void);
void setSendHostPortNumFlag(uint8 value);

char* getParameterKey(void);
void cwmpResetSessionRetryWaitInterval(void);
void cwmpSetSessionRetryFlag(int value);
void cwmpDoSessionRetry(void);
void cwmpReInitHostTableTaskStart();

#ifndef CODE_SHRINK  //qingpu.20080110
int getGetRPCMethodsFlag(void);
int setGetRPCMethodsFlag(int value);

uint8 getInitEmptyPktFlag(void);
void setInitEmptyPktFlag(uint8 value);
#endif

void cwmpSetNotifyFlag(uint8 value);
uint8 cwmpGetNotifyFlag(void);

void cwmpSetSpSaveFlag(uint8 value);
uint8 cwmpGetSpSaveFlag(void);

char* getProvisionCode(void);
int setProvisionCode(char * value);

uint8 getCwmpSwitch(void);
void setCwmpSwitch(uint8 value);
uint8 SetDownloadErrType(uint8 flag);
uint8 GetDownloadErrType(void);

uint8 SetUploadErrType(uint8 flag);
uint8 GetUploadErrType(void);

uint8 SetFileTransferCompleteFlag(uint8 flag);
uint8 GetFileTransferCompleteFlag(void);
uint8 SetFileTransferStartTime(uint32 start);
uint32 GetFileTransferStartTime(void);
uint8 SetFileTransferCompleteTime(uint32 end);
uint32 GetFileTransferCompleteTime(void);
#if defined(TCSUPPORT_CWMP_PRECISE_TIME)
uint8 SetFileTransferStartTime_improve();
uint8 SetFileTransferCompleteTime_improve();
char * getCurrentTime(void);
#endif
uint8 SetCommandKey(char* value, int index);
char* GetCommandKey(int index);

uint8 getWANIndex(void);  //qingpu,20070920
char * getWANExternalName(uint8 index);
//void  getWANExternalIP(int node, char* externalIP);
void  getWANSubnetMask(int node, char* subnet);
void  getWANExternalGateway(int node, char* externalGateway);
//void  getLANIP(char* LANIPStr);

void cwmpCheckIPValid(void);

//For Session Retry Policy
int  cwmpSessionRetryWaitIntervalCounter(void);
void cwmpSessionResetRetryWaitInterval(void);
int cwmpSessionGetRetryWaitInterval(void);
int cwmpSessionGetRetryFlag(void);
void cwmpSessionSetRetryFlag(int value);
int cwmpGetSessionRetryCount(void);
void cwmpCalculateSessionRetryCount(int value);
void cwmpSessionRetryTask(void);
void cwmpSetInformCompleteFlag(int value);
int cwmpGetInformCompleteFlag(void);

void cwmpResetInformFlag(void);

void cwmpSetCurACSInfo(uint32 theHostAddress, uint16 thePort, char* thePath);
void cwmpGetCurACSInfo(uint32 *theHostAddressP, uint16 *thePortP, char *thePath);

boolean  getHoldRequestFlag(void);
void  setHoldRequestFlag(boolean  value);

#if 	CWMP_USING_MBUF_FOR_TXBUF == 0 //qingpu.20080110
boolean  getPreAllocTxBufFlag(void);
void  setPreAllocTxBufFlag(boolean  value);
#endif

char* cwmp_strdup(char * str);

#if 	CWMP_USING_MBUF_FOR_TXBUF == 0
int cwmpInitTxBuf(void);
#endif
int cwmpConfigInit(void);

uint8 cwmpGetDownloadFileType(void);
char* cwmpGetDownloadTargetFileName(void);
uint8 cwmpSupportDownloadFile(char* downloadFileName);

char*  getCapitalStr(char* str);


void cwmpConfigSSLCA(void);

char* getCustomerCA(void);
void cwmpSignalTimerInform(uint32 time);
void cwmpSignalScheduleInform(unsigned int delaySeconds);
void cwmpSignalInform(void);


#ifdef WLAN_RT2561_SUPPORT	
uint32 getCwmpWLANTaskId(void);
#endif

uint32 getCwmpReInitHostTableTaskId(void); //qingpu.20071129

char* getDeviceSummary(void);
void cwmpWLANTask(void);

#if 	CWMP_USING_MBUF_FOR_TXBUF == 0
uint32 getTxBufLenFromMMCT(void);
#endif

/*added by hqfeng, 2006.09.14*/
int  getGetFromBootbaseFlag(void);
void  setGetFromBootbaseFlag(int  value);
char* getManufacturer(void);
int  setManufacturer(char *value);
char* getModelName(void);
int  setModelName(char *value);
char* getDescription(void);
int  setDescription(char *value);
char* getProductClass(void);
int  setProductClass(char *value);

char * getCwmpModelName(void);

uint8 getIPPingCompleteFlag(void);
void setIPPingCompleteFlag(uint8 value);
uint8 getDownloadRomFileFlag(void);
void setDownloadRomFileFlag(uint8 value);

#ifdef CT_COM_DEVICEREG
//uint8 getInitialDeviceRegisterFlag(void);
int getDevregInformFlag(void);
void setDevregInformFlag(int flag);
#endif

#ifdef TR143
//uint8 getDiagnosticsCompleteFlag(void);
void setDiagnosticsCompleteFlag(uint8 value);
uint8 cwmpInitDiagnosticsStruct(void);
#define MAXBUFLEN 		100
#define UDPHEADERLEN 	20
#define PACKNUM			10

uint8 *put32( uint8	*cp, uint32	x);
int getServerIPFromInterface( char *serverIP );
int checkUDPEchoEnable( void );
int checkUDPEchoPlusEnable( void );
int getUDPEchoPort( void );
char *getUDPEchoSourceIP( void );
void clearUDPEchoStats( void );
void UDPEchoStart( void );
void closeUDPSocket( void );
void UDPEchoRecvSendProc( void );

#endif

uint8 getConnRequestAuthFlag(void);
void setConnRequestAuthFlag(uint8 value);

uint8 getNoArrayTypeFlag(void);
void setNoArrayTypeFlag(uint8 value);
boolean getFaultDetailFlag(void);
void setFaultDetailFlag(boolean value);

uint8 getFactoryResetFlag(void);
void setFactoryResetFlag(uint8 value);

char* cwmpGetFileNameFromDownloadURL(void);

uint8 getCwmpUpgradeManaged(void);

void setCwmpConnectionIndex(uint8 index);
uint8 getCwmpConnectionIndex(void);

int getFirmwareVersionFlag(void);
void setFirmwareVersionFlag(uint8 value);

char* getCustomerHardwareVer(void);
int  setCustomerHardwareVer(char *value);

//added for ScheduleInform RPC, hqfeng, 2007.01.08
uint8 getScheduleInformFlag(void);
void setScheduleInformFlag(uint8 value);

void cwmpPeriodicInformTime(void);
int setDefaultPeriodicInformTime(void);
boolean checkPeriodicInformTime(void);
//Boolean checkPeriodicInformTime(void);
char* getPeriodicInformTime(void);
void setPeriodicInformTime(char *strPITValue);
boolean isPITUnknownValue(char *strTime);
//Boolean isPITUnknownValue(char *strTime);
uint8 Get_ACS_PVC_Number(void);

//#ifdef RANDOM_INFORM
#if defined(TCSUPPORT_RANDOM_INFORM)
void cwmpStartInform(void);
#endif

uint8 getMotiveRSTFlag(void);
void setACSAuthSuccess(void);

#ifdef USE_SSL
uint8 get_ssl_check_mem_flag(void);
void set_ssl_check_mem_flag(uint8 value);
#endif

//add by brian
char* getCwmpHostName(void);
void setCwmpHostName(char* value);

void setcwmpAcsUrl(char* value);
char* getcwmpAcsUrl(void);
void setcwmpPrdInformTime(char *value);
char* getcwmpPrdInformTime(void);

int cwmpDealWithFaultCode(int errorCode);
int isInRouteTable(char *ip,char *dev);
#endif // __CWMP_H__
