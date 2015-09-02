
/*
** $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/private/TR69_64/cwmp/Includes/cwmpRpc.h#2 $
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
/*
** $Log: cwmpRpc.h,v $
** Revision 1.10  2011/08/19 13:22:11  janey
** [Bugfix]:#11369
** un open code compile option issue
**
** Revision 1.9  2011/08/05 01:50:04  xyzhu_nj
** #11006: ct com merge to main trunk.(phase 3)
** 1. Add TCSUPPORT_COMPILE tag for ct com compile option;
** 2. Delete files in filesystem that CT_COM not used;
** 3. Merge the lastes bugfix from ct com branch to main trunk.(TLB7.1.2.20-51 -- 57);
** 4. update mic.
**
** Revision 1.8  2011/07/29 10:23:48  xyzhu_nj
** #11006: Merge China telecom branch to Main trunk.(phase 2)
**
** Revision 1.7  2011/06/17 07:32:24  yzwang_nj
** [Feature]#10610
** Support VendorConfigFile and  static Host TR069 nodes
**
** Revision 1.6  2011/05/13 14:51:14  xyyou
** 10264
**
** Revision 1.5  2011/04/08 05:54:52  xmdai_nj
** #9784:Power off during firmware upload, CPE can't send event code of "7 TRANSFER COMPLETE" and "M Upload" after reboot.
**
** Revision 1.4  2011/03/17 13:42:42  xmdai_nj
** #9463:tr69 download/upload starttime complete should be current CPE time
**
** Revision 1.3  2010/12/29 12:23:14  xflu_sz
** merge code from branch_sz to main trunk
** bug: #2132&7710,
**
** Revision 1.2  2010/12/22 04:06:22  rclv_nj
** bug 8131
**
** Revision 1.1.1.1  2010/09/30 21:00:11  josephxu
** apps/public, private
**
** Revision 1.1.1.1  2010/04/09 09:42:44  feiyan
** New TC Linux Make Flow Trunk
**
** Revision 1.4  2010/03/22 02:38:25  xyzhu_nj
** [bug#4725]: Add TR143 parameters: IGD.Capabilities / IGD.DownloadDiagnostics. / IGD.UploadDiagnostics.
** [bug#4837[: Add TR143 parameters:
** IGD.UDPEchoConfig.
**
** Revision 1.3  2010/02/01 10:29:34  xyzhu_nj
** [#bug4703]: Modify commit or delay commit mechanism on tr69 app.
**
** Revision 1.2  2009/12/30 06:49:15  xhshi
** [#4363]
** Implement  tr069 engine in linux platform and remove some debug messages
**
** Revision 1.1  2009/12/24 12:21:28  feiyan
** [Feature] BugID:4407
** 1.Implement TR069 and TR064 Engine under Linux
**  2.Implement TR069 RPC Methods(Reboot, Factory Reset, Download, Upload)
**
** Revision 1.15  2009/07/16 08:01:43  taoliu
** #2404
** Implement UDPEcho plus of TR143.
**
** Revision 1.14  2009/06/19 12:48:10  taoliu
** #2404
** TR143 HTTP download/upload diagnostics
**
** Revision 1.13  2009/04/23 07:52:11  jlshi
** Merge from TC2 Branch for bug #2440
**
** Revision 1.12  2009/04/16 07:06:32  taoliu
** #2302,#2303
** TR069 can't add/delete pure bridge pvc
**
** Revision 1.11  2009/04/03 10:30:43  hzbian
** Open Source
**
** Revision 1.10  2009/03/20 01:13:49  xhshi
** [ENHANCEMENT]
** #1821,#1241
** merge from bharti branch and add other new layer2bridging parameters
**
** Revision 1.9  2009/02/12 11:33:21  taoliu
** fixed some compile warnings
**
** Revision 1.8  2008/11/26 08:38:33  hzbian
** [BUGFIX]#1077 #1082
** Interface is not correct, but diagnostics is still performed.
** "8 DIAGNOSTICS COMPLETE" is not taken out in periodic inform
** after diagnostics  is finished successfully.
**
** Revision 1.7  2008/11/04 12:21:37  jlshi
** Compile error with tr064 but without tr069.
** Compile C9 version with TC2_TRUE compile option,compile error.
**
** Revision 1.6  2008/08/07 10:26:20  qingpu
** Enhancement:Support TR069 302 Redirect function.(Merge from Branch)
**
** Revision 1.5  2008/01/10 10:46:18  qingpu
** Enhancement:close some TR69 ci-commands for code size.
**
** Revision 1.4  2007/07/27 07:37:31  maojianli
** Merge 2_11_3_0 back to main trunk
**
** Revision 1.3  2007/02/28 03:43:54  xzwang
** Enhancement: Realize Upload RPC.
** Enhancement: Realize ScheduleInform RPC.
**
** Revision 1.2  2007/02/08 02:28:34  hqfeng
** -Add cwmpIsNumeric()
**
** Revision 1.1.1.1  2006/12/29 08:25:55  josephxu
** LiNOS transfer from Hsinchu to Suzhou (20061229)
**
** Revision 1.4  2006/12/28 09:39:59  yen
** Merge newest tr069 code from Huawei trunk to Taiwan trunk to fix some bugs.
**
** Revision 1.3  2006/12/05 12:05:49  yen
** - Merge TR-069 code after 10/1/2006 into Taiwan Trunk.
**
** Revision 1.2  2006/11/16 04:50:13  yen
** - Fix IOTissue: Connection request fails while test with SupportSoft ACS because SupportSoft
**                does not support authentication.
**
** Revision 1.1  2006/09/30 08:43:44  yen
** Merge TR-069 and TR-064 code into trunk.
**
** Revision 1.31  2006/09/07 02:41:12  xzwang
** - Reject to set some specific parameters like DeviceInfo.Uptime to be active notification while the device node's flag is 0.
** - Alway delete old node from active and passive notification list and then add it if the new attribute is not off.
**
** Revision 1.30  2006/09/01 05:32:02  xzwang
** Resolve the bug that get paramter value/name on entire object will cause
** modem reboot by the following means:
**  - Use os_malloc/os_free to replace malloc/free.
**  - Write a function os_str_realloc to replace realloc.
**  - Write a function cwmp_strdup to replace strdup.
**  - If malloc fails, Free response structure and document before build fault structure.
**  - Pre-allocate Tx buffer during cwmp initializaion.
**
** Revision 1.29  2006/08/28 06:38:27  hqfeng
** - Modify holdRequest[4] to holdRequest[8] in SoapHeadInfo.
**
** Revision 1.28  2006/08/25 05:32:15  xzwang
** - Add one structure "ACSInfo" in rpc for record the currenct information of ACS' URL.
**
** Revision 1.27  2006/08/22 09:52:36  xzwang
** add function "cwmpResetAttribute" and "cwmpClearNotifyList"
**
** Revision 1.26  2006/08/15 08:12:17  xzwang
** Enhance retry mechanism.
**
** Revision 1.25  2006/08/14 08:30:57  haijian
** Extend downloadstruct to support all arguments of download RPC.
**
** Revision 1.24  2006/08/14 06:14:07  hqfeng
** - Add Session Retry Policy
** - Reduce code size.
**
** Revision 1.23  2006/08/11 07:28:55  xzwang
**  Realize value change inform mechanism for forced active notification parameters.
**
** Revision 1.22  2006/08/03 06:39:49  xzwang
** - Modify value change inform mechanism to resolve the problem that CPE can not send
**   value change inform to ACS if we add/delete object after set attributes.
**
** Revision 1.21  2006/07/31 07:39:08  xzwang
** - Add fault parameter in TransferComplete RPC.
**
** Revision 1.20  2006/07/31 04:19:26  xzwang
** - Extract fault information of ACS fault code correctly.
**
** Revision 1.19  2006/07/21 14:21:17  hqfeng
** - Merge the code passed the test in UNH.
**
** Revision 1.18  2006/07/12 01:27:53  hqfeng
** - Modify INSTANCENUMBER form 8 to 16.
** - Add some function for GetRPCMethods (CPE->ACS)
**
** Revision 1.17  2006/07/11 07:28:31  xzwang
** - Set attribute for all parameters under partial object.
**
** Revision 1.16  2006/07/07 03:24:27  xzwang
** - set download commandKey array number to be 33.
** - use pointer to replace array for rxBuf.
**
** Revision 1.15  2006/07/06 08:29:09  hqfeng
**  - Add FaultStringLength
**
** Revision 1.14  2006/07/05 07:17:11  hqfeng
**  - Remove the mark of InvalidRequestUsername in FaultCode Struct.
**
** Revision 1.13  2006/07/02 08:10:45  xzwang
** correct typedef errors for parameter interface function.
**
** Revision 1.12  2006/07/02 05:24:57  xzwang
** modify return type from "int" to be "int8" for typedef parameter interface function.
**
** Revision 1.11  2006/06/29 07:41:04  hqfeng
**  - Recover the getCwmpRebootFlag().
**
** Revision 1.10  2006/06/28 06:06:37  hqfeng
** - Add  InvalidRequestUsername to FaultCode struct.
**
** Revision 1.9  2006/06/28 03:17:37  hqfeng
** - Modify the getCwmpRebootFlag().
**
** Revision 1.8  2006/06/25 09:20:36  xzwang
** Modify CWMP_RX_BUF_SIZE from 2048 to 3072.
**
** Revision 1.7  2006/06/24 13:55:00  xzwang
** User pointer replace array in parameter structure to reduce memory occupation.
**
** Revision 1.6  2006/06/23 11:12:42  ylwang
** Add Port Mapping (Another Method of Dynamic Allocation Memory) .
**
** Revision 1.5  2006/06/22 03:02:55  xzwang
** add function "cwmpGetFaultParaList()"
**
** Revision 1.4  2006/06/22 01:25:28  hqfeng
** - Add ACSFaultCode define.
**
** Revision 1.3  2006/06/21 09:51:29  penny
** merge from 3612 Trunk
**
** Revision 1.2  2006/06/20 04:36:22  maojianli
** dhcp host dynamic
**
** Revision 1.1.1.1  2006/06/19 12:25:42  hqfeng
** - Created 3162L/L2 trunk for TR-069(Merge to new trunk).
**
** Revision 1.19  2006/06/15 10:40:56  xzwang
** modify function name from "cwmpCommandParse_new" to "cwmpParseSoapPaket".
**
** Revision 1.18  2006/06/14 06:06:19  xzwang
** - Realize value change inform function.
**
** Revision 1.17  2006/06/09 08:49:25  hqfeng
** - Modify getCwmpRebootFlag();
**
** Revision 1.16  2006/06/09 01:21:34  hqfeng
** - Add kAscii_Colon and kAscii_Slash.
**
** Revision 1.15  2006/06/08 13:08:45  xzwang
** - remove some debug messages.
** - add download RPC function.
** - close connection while receive one empty packet.
** - realize basic authentication for download from file server.
** - realize two active connections.
**
** Revision 1.14  2006/06/03 07:32:09  hqfeng
** - Modify for Fault Code Machine.
**
** Revision 1.13  2006/05/24 05:58:27  hqfeng
**  - Add getCwmpRebootFlag().
**
** Revision 1.12  2006/05/09 02:49:09  hqfeng
** - Add a flag member in DeviceNode struct.
**
** Revision 1.11  2006/04/29 01:36:43  hqfeng
**  - Modify for AddObject/DeleteObject.
**
** Revision 1.10  2006/04/25 10:01:56  xzwang
** delete "prev" member in device stack.
**
** Revision 1.9  2006/04/17 03:15:05  hqfeng
** - Add the define:
**    #define PARAMETER_NAME_LEN 40
**    #define PARAMETER_NUM 10
**
**
*/

#ifndef __CWMPRPC_H__
#define __CWMPRPC_H__

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
/*
#include "switch.h"
#include "global.h"
#include "os.h"
#include "qlm.h"
#include "qcm.h"
#include "qrm.h"
#include "mbuf.h"
#include "cbuf.h"
#include "filter.h"
#include "spt.h"
#include "sys_pwc.h"
#include "sys_isr.h"
#include "sys_proc.h"
#include "raserror.h"
#include "cmdparse.h"
#include "Global.h"
*/
#include "cwmpXmlparser.h"
#include "mbuf.h"
//#include "cwmpXml.h"

#define CWMP_TX_BUF_SIZE 2048
#define CWMP_RX_BUF_SIZE 5120

#define  kAscii_Null  0x00
#define  kAscii_Dot  0x2E
#define  kAscii_Space  0x20
#define kAscii_Colon        0x3A
#define kAscii_Slash        0x2F

#define PARAMETER_NAME_LEN 40
#define PARAMETER_NUM 16

#define INSTANCENUMBER 16	//8->16,hqfeng,2006.07.11

#define MAC_CHAN_ID		0  //add by hqfeng, 2006.04.30

#define FaultStringLength		400  //add by hqfeng, 2006.07.06

/*The following defines are for theSettingFlag, hqfeng.*/
#define CWMP_SET_WLAN			(1 << 0) //The operation is setting the values of the parameters about WLAN.
#define CWMP_DOWNLOAD_TCROM		(1 << 1) 
#define CWMP_FACTORY_RESET			(1 << 2) 
#define CWMP_SET_LAYER2BRIDGING (1 << 3)


/*The following defines are for theIPPingFlag, hqfeng*/
#define CWMP_IPPING_ACTIVE			(1 << 0)
#define CWMP_IPPING_INPROGRESS		(1 << 1) 
#define CWMP_IPPING_TERMINATED		(1 << 2) //used to check the ping test is terminated unusally or not.

/*The following defines are for theTraceRouteFlag, xyyou*/
#ifdef TCSUPPORT_TRACEROUTE
#define CWMP_TRACEROUTE_ACTIVE			(1 << 0)
#define CWMP_TRACEROUTE_INPROGRESS		(1 << 1) 
#define CWMP_TRACEROUTE_TERMINATED		(1 << 2) //used to check the trace route test is terminated unusally or not.
#endif

/*The following defines are for theInformFlag, hqfeng*/
#define INFORM_BOOTSTRAP				(1 << 0)
#define INFORM_BOOT					(1 << 1)
#define INFORM_PERIODIC				(1 << 2)
#define INFORM_SCHEDULED				(1 << 3)
#define INFORM_VALUECHANGE			(1 << 4)
#define INFORM_KICKED					(1 << 5)
#define INFORM_CONNECTIONREQUEST	(1 << 6)
#define INFORM_TRANSFERCOMPLETE		(1 << 7)
#define INFORM_DIAGNOSTICSCOMPLETE	(1 << 8)
#define INFORM_REQUESTDOWNLOAD		(1 << 9)
#define INFORM_MREBOOT				(1 << 10)
#define INFORM_MDOWNLOAD				(1 << 11)

/* The following defines are for the flag in tcCwmpAttr_t, hqfeng*/
#define CWMP_CONN_REQUEST_AUTH				(1 << 0)
#define CWMP_FIRMWARE_VERSION				(1 << 1)
#define CWMP_PURE_BRIDGE_TYPE_IP_BRIDGED	(1 << 2)  //0:PPPoE_Bridged(default; 1:IP_Bridged, add by liutao@20090403

#if 0
#define CWMP_NO_ARRAY_TYPE_FLAG				(1 << 2)
#endif

/*The following macro is for theLoopBackFlag*/
#define CWMP_LOOPBACK_ACTIVE			(1 << 0)

/*The following macro is for theDSLLOOPFlag*/
#define CWMP_DSLLOOP_ACTIVE			(1 << 0)


/*define for using mbuf to store txBuf message*/
#define CWMP_USING_MBUF_FOR_TXBUF  1

#if CWMP_USING_MBUF_FOR_TXBUF
#define CWMP_FREE_PARA_XML_NODE_IMMEDIATELY 1
#endif

#ifdef TR143
/*The following defines are for download/upload diagnostics, taoliu*/
#define CWMP_DIAGNOSTICS_ACTIVE			(1 << 0)		//diagnostics start flag
#define CWMP_DIAGNOSTICS_INPROGRESS		(1 << 1) 		//diagnostics is in progress
#define CWMP_DIAGNOSTICS_TERMINATED		(1 << 2) 		//diagnostics should be terminate at once
#endif

/* download error types*/
typedef enum {
	NULL_TYPE,
	FILE_ILLEGAL,
	CONNECTION_ERR,
	FILE_UNAUTHEN,	
	DOWNLOAD_FAIL,
} downloadErrType;

/* upload error types*/
typedef enum {
	NULL_UPLOAD_TYPE,	
	UPLOAD_FAIL,
} uploadErrType;

typedef enum {
	BOOTSTRAP,
	BOOT,
	PERIODIC,
	SCHEDULED,
	VALUECHANGE,
	KICKED,
	CONNECTIONREQUEST,
	TRANSFERCOMPLETE,
	DIAGNOSTICCOMPLETE,
	REQUESTDOWNLOAD,
	M_Reboot,
	M_ScheduleInform,
	M_Download,
	M_Upload,
	M,
	X
} InformEventIndex;

/**************************Data structure for frame mechanism******************************/
//typedef unsigned char		uint8;		/* 8-bit unsigned integer        */

typedef enum{
	NoType,
	ObjectType,
	StringType,
	IntType,
	UnsignedIntType,
	BooleanType,
	DateTimeType,
	Base64Type,
	ParameterValueStructType,
	ParameterInfoStructType,
	SetParameterAttributesStructType,
	ParameterAttributeStructType,
	DeviceIdStructType,
	EventStructType,
	FaultDetailType
} ParameterType, *ParameterTypePtr; 

typedef enum{
	Read,
	Write,
	Add, //add by hqfeng, 2006.04.25
	Delete, //add by hqfeng, 2006.04.25
	Reboot  //add by hqfeng, 2006.04.28
} AccessType;

typedef enum{
	ExceptError = -1,
	MethodNotSupport = 9000,
	RequestDeny = 9001,
	InternalError = 9002,
	InvalidArgument = 9003,
	ResourceExceed = 9004,
	InvalidParameterName = 9005,
	InvalidParameterType = 9006,
	InvalidParameterValue = 9007,
	SetReadOnlyParameter = 9008,
	RejectRequestNotification = 9009,
	DownloadFailure = 9010,
	UploadFailure = 9011,
	FileTransferAuthenFailure = 9012,
	UnsupportedProtocol = 9013,
	InvalidRequestUsername = 9800,
	NoGetValueFunction = 9801, //added by hqfeng,2007.01.05
	NoGetAttributeFunction = 9802 //added by hqfeng,2007.01.05
} FaultCode;

typedef enum{
	ACS_Error = -2,
	ACS_MethodNotSupport = 8000,
	ACS_RequestDeny = 8001,
	ACS_InternalError = 8002,
	ACS_InvalidArgument = 8003,
	ACS_ResourceExceed = 8004,
	ACS_RetryRequest = 8005
} ACSFaultCode;

typedef enum{
	None = 0,
	Passive = 1,	
	Active = 2
} NotifyType;

typedef enum{
	Download = 0,
	Upload = 1
} FileTransferType;

typedef struct{
	int faultCode;
	char* faultString;
} FaultCode_String_Map;
	
typedef struct StringStruct{
	char*    value;
	struct StringStruct*  next;
} StringStruct, *StringStructPtr;

typedef struct IntStruct{
	int     value;
	struct IntStruct* next;
} IntStruct, *IntStructPtr;

typedef struct UnsignedIntStruct{
	unsigned int   value;
	struct UnsignedIntStruct* next;
}UnsignedIntStruct, *UnsignedIntStructPtr;

typedef struct BooleanStruct{
	unsigned short     value;
	struct BooleanStruct* next;
} BooleanStruct, *BooleanStructPtr;

typedef struct DateTimeStruct{
	char     value[64];
	struct DateTimeStruct* next;
} DateTimeStruct, *DateTimeStructPtr;

typedef struct Base64Struct{
	char     value[64];
	struct Base64Struct* next;
} Base64Struct, *Base64StructPtr;

#if 0
typedef struct MethodListStruct{
	char*     value;
	struct MethodListStruct* next;
} MethodListStruct, *MethodListStructPtr;
#endif

typedef struct AccessEntry{
	char   entryName[64];
	struct AccessEntry* next;
} AccessEntry,*AccessEntryPtr;

typedef struct ParameterAttributeStruct{
	char     *name;
	uint8    notification;
	AccessEntryPtr accessList;
	struct ParameterAttributeStruct* next;
} ParameterAttributeStruct, *ParameterAttributeStructPtr;

typedef struct ParameterAttributesSetStruct{
	char     *name;
	uint8  notificationChange;
	uint8    notification;
	uint8  accessListChange;
	AccessEntryPtr accessList;
	struct ParameterAttributesSetStruct* next;
} ParameterAttributesSetStruct,*ParameterAttributesSetStructPtr;

typedef struct ParameterInfoStruct{
	char*     name; 
	unsigned short  writable;
	struct ParameterInfoStruct* next;
} ParameterInfoStruct, *ParameterInfoStructPtr;

typedef struct{
	ParameterType type;
	char* value;
} ValueInfo, *ValueInfoPtr;

typedef struct ParameterValueStruct{
	char         *name; 
	ValueInfoPtr   valueInfoPtr;
	struct ParameterValueStruct* next;
} ParameterValueStruct, *ParameterValueStructPtr;

typedef struct RPCArgument{
	char    *name;
	ParameterType type;
	int            number;
	uint8	beArray;
	void*     value;
	void* parameterList;
	struct RPCArgument* next;
} RPCArgument, *RPCArgumentPtr;


typedef struct{
	char rpcName[64];
	RPCArgumentPtr argumentPtr;
} SoapBodyInfo, *SoapBodyInfoPtr;


typedef struct{
	char  id[64]; // yen2006719
	char   holdRequest[8];    // 4->8, by hqfeng,2006.08.14
	char   noMoreRequest[4];
} SoapHeadInfo, *SoapHeadInfoPtr;

typedef struct{
	SoapHeadInfoPtr  headStructPtr;
	SoapBodyInfoPtr  bodyStructPtr;
} SoapInfo, *SoapInfoPtr;

typedef struct DeviceNode{
	char   name[PARAMETER_NAME_LEN]; /* Name of the node*/
	uint8   flag; // 0: Can't set attribute to active type; 1: No limitation; 2: Can't set attribute to active or passive type. hqfeng,2006.09.30 
	ParameterType   type;  /* Type of the parameter*/
	uint8   accessType; /* Access control list for paramter. ReadOnly = 0, Writeable = 1 */
	
	struct DeviceNode*  firstChild;
	struct DeviceNode*   parentNode;
	struct DeviceNode*   prevSibling;
	struct DeviceNode*   nextSibling;
	int8   (*getValueInfo)(char** value, ParameterType* type, struct DeviceNode*  self);
	int8   (*setValueInfo)(void*, struct DeviceNode*  self);
	int8   (*getAttribute)(uint8* notification, AccessEntry** acl, struct DeviceNode* self);
	int8   (*setAttribute)(uint8, uint8, uint8, AccessEntry*, struct DeviceNode*  self);
//	int8   (*addObject)(uint32* num);
//	int8   (*deleteObject)(uint32 num);
	int8   (*addObject)(uint32* num, struct DeviceNode*  self);
	int8   (*deleteObject)(uint32 num, struct DeviceNode*  self);
} DeviceNode, *DeviceNodePtr; 

typedef struct RPCFault{
	char* rpcErrorType;
	char* parameterName;
	int faultCode;
	char* faultString;
	struct RPCFault* next;
} RPCFault, *RPCFaultPtr;

typedef struct FaultDetail{
	int faultCode;
	char* faultString;
	RPCFaultPtr rpcFaultList;
} FaultDetail, *FaultDetailPtr;

typedef struct FaultStruct{
	char faultcode[24];
	char faultstring[48];
	FaultDetailPtr detail;
} FaultStruct, *FaultStructPtr;

typedef struct DeviceIdStruct{
	char manufacture[65];
	char OUI[7];
	char productClass[65];
	char serialNumber[65];
} DeviceIdStruct, * DeviceIdStructPtr;

typedef struct  EventStruct{
	char eventCode[65];
	char commandKey[33];
	struct EventStruct* next;
} EventStruct, *EventStructPtr;

typedef struct InformStruct{
	DeviceIdStruct  deviceId;
	EventStructPtr	  eventList;
	unsigned int     		  maxEnvelopes;
	char 		  currentTime[64];
	unsigned int			  retryCount;
	ParameterValueStructPtr parameterList;
} InformStruct, *InformStructPtr;

typedef struct FileTransferStruct{
	FileTransferType transferType;
	char * CmdKey;
	uint16 sCmdKey; // size of cmdkey
	char *FileType;
	uint16 sFileType;
	char *URL;
	uint16 sURL;
	char *Username;
	uint16 sUsername;
	char *Password;
	uint16 sPassword;
	unsigned int FileSize;
	char * TargetFileName;
	uint16 sTargetFileName;
	unsigned int DelaySeconds;
	char * SuccessURL;
	uint16 sSuccessURL;
	char * FailureURL;
	uint16 sFailureURL;
	FaultDetailPtr faultStructPtr;//added for dealing with Download error,hqfeng,2006.10.26
} FileTransferStruct, *FileTransferStructPtr;

typedef struct ACSInfo{
	uint32		theHostAddress;
	uint16		thePort;
	char			thePath[256];
} ACSInfo;

#ifdef CWMP_REDIRECT
typedef struct RedirectACSInfo{
	char cwmpRedirectURL[256];
	char cwmpRedirectPath[256];
	uint16 cwmpRedirectPort;
	uint8 cwmpRedirectHttpFlag;
	uint8 cwmpRedirectCnt;
	uint8 cwmpRedirectFlag;
	uint32 cwmpRedirectHostAddress;
	char cwmpRedirectHostName[64];
} RedirectACSInfo;
#endif

typedef struct flagStruct{
	uint8  theSettingFlag;//Add for dealing with some special setting operations, hqfeng,2006.10.12
	uint8  theIPPingFlag;//Add for IP  Ping, hqfeng, 2006.10.23	
	#ifdef TCSUPPORT_TRACEROUTE
	uint8  theTraceRouteFlag;//Add for trace route, xyyou,20110427
	#endif
	uint16  theInformFlag;//Add for buiding inform, hqfeng, 2006.10.23
	uint8  theLoopBackFlag;
	uint8  theDSLLoopFlag;	
#ifdef TR143
	uint8 theDownloadDiagFlag; //add for download diagnostics, taoliu,20090508
	uint8 theUploadDiagFlag; //add for upload diagnostics, taoliu,20090508
#endif
} flagStruct, *flagStructPtr;
//added by xyzhu_nj_091130
void attrCfgNode(int type);
#define READ 				0
#define SAVE				1
#define	COMMIT		   		2

#define ATTRFLAG0			0
#define ATTRFLAG1			1

#define MAX_CFG_NODE_NUM 32
typedef struct attrStruct{
	/*
	 This two flags can support 64 attribute structs.
	 If we want to support more attribute structs.
	 we should add other flags.
	 */
	uint32 theSettingFlag0;
	uint32 theSettingFlag1;
}attrStruct, *attrStructPtr;
//#define MAX_ATTRS_NUM 32

#ifdef TR143
typedef struct TR143DiagnosticsInfo{
	char transferURL[256];
#if 0
	char ifaceChannName[10];
#else
	char interfaceName[10];
#endif
	uint32 transferIPAddress;
	uint32 lengToSend;
#if 0
	time_t sTime; //the date time at which the TCP socket open (SYN) was sent for the HTTP connection
	uint32 sysTimeTCPOpenRequest; //the system time at which the TCP socket open (SYN) was sent for the HTTP connection
	uint32 sysTimeTCPOpenResponse; //the system time at which the TCP ACK to the socket opening the HTTP connection was received
	uint32 sysTimeROM;//the system time at which the client sends the GET command
	uint32 sysTimeBOM;//the system time at which the first data packet is received
	uint32 sysTimeEOM;//the system time at which the last data packet is received
#else
	struct timeval sysTimeTCPOpenRequest;
	struct timeval sysTimeTCPOpenResponse;
	struct timeval sysTimeROM;
	struct timeval sysTimeBOM;
	struct timeval sysTimeEOM;
#endif
	uint8 startCountReceiveFlag;
	uint8 RTAddFlag;
	uint32 testBytesReceived;
	uint32  BOMInOctet;
	uint32  EOMInOctet;
	uint32  BOMOutOctet;
	uint32  EOMOutOctet;
	char diagGateway[32];
} TR143DiagnosticsInfo;

typedef struct TR143UDPEchoInfo{
	uint32 packetReceived;
	uint32 packetResponsed;
	uint32 bytesReceived;
	uint32 bytesResponsed;
	uint32 respReplyFailCnt;  //failed count of send valid response packet
	//time_t sTimeFirstRcv; //date time of  first received packet
	//uint32 sysTimeFirstRcv;	//system time of first received packet
	//uint32 sysTimeRecentRcv;  //system time of most recent received packet
	struct timeval sysTimeFirstRcv;
	struct timeval sysTimeRecentRcv;
} TR143UDPEchoInfo;
#endif

typedef struct{
	char *rxBuf/*[ CWMP_RX_BUF_SIZE]*/;
#if CWMP_USING_MBUF_FOR_TXBUF
	mbuf_t	*txBuf;
#else
	char* txBuf;
#endif
	IXML_Document* requestDocPtr;
	IXML_Document* responseDocPtr;
	IXML_Node* requestHeadNodePtr;
	IXML_Node* responseHeadNodePtr;
	IXML_Node* requestBodyNodePtr;
	IXML_Node* responseBodyNodePtr;
	SoapInfoPtr   requestStructPtr;
	SoapInfoPtr   responseStructPtr;
	DeviceNodePtr IGDDeviceRootPtr;
	InformStruct  informStruct;
	FaultStructPtr soapFaultPtr;
        FileTransferStructPtr fileTransferStruct;
	uint8 responseFlag;
	ACSInfo acsCurInfo;
#ifdef CWMP_REDIRECT
	RedirectACSInfo redirectacsCurInfo;
#endif
#ifdef TR143
	TR143DiagnosticsInfo loadDiagInfo;
	TR143UDPEchoInfo UDPEchoInfo;
#endif
#if 1 // to be compatable with old version.
	IXML_Node *rpcNode;
	IXML_Node *rpcNodeHead;
	int  rpcHeadID;
#endif
	flagStruct  flags;  //we will use this struct to replace all the gloable flag in TR-069,hqfeng,2006.10.12
	attrStruct attrFlags;
} cwmp_rpc_t;

typedef int (*RPCMethodFun)(SoapBodyInfoPtr, SoapBodyInfoPtr);
typedef struct{
   char* name;
   RPCMethodFun  RPC_Method;
} cwmp_rpc_methodmap;	

typedef struct DeviceNodeStack{
	DeviceNodePtr deviceNode;
	struct DeviceNodeStack* next;
} DeviceNodeStack, *DeviceNodeStackPtr;

typedef struct NotifyDeviceNodeInfo{
#if 0  //xzwang_060802
	DeviceNodePtr deviceNode;
#endif
	char*  deviceNodeName;  //added for judge whether the deviceNode pointer is changed. xzwang_060802
	char*  acsSetValue;
	struct NotifyDeviceNodeInfo* next;
	struct NotifyDeviceNodeInfo* prev;
} NotifyDeviceNodeInfo, *NotifyDeviceNodeInfoPtr;

typedef int8 (*GetValueInfoFun)(char** value, ParameterType* type, struct DeviceNode*  self);
typedef int8 (*SetValueInfoFun)(void*, struct DeviceNode*  self);
typedef int8 (*GetAttributeFun)(uint8* notification, AccessEntry** acl, struct DeviceNode* self);
typedef int8 (*SetAttributeFun)(uint8, uint8, uint8, AccessEntry*, struct DeviceNode*  self);
typedef int8 (*AddObjectFun)(uint32* num,struct DeviceNode*  self);
typedef int8 (*DeleteObjectFun)(uint32 num,struct DeviceNode*  self);

/****************************** Functions for Frame Mechanism *********************************/
//Inform mechanism
int8 cwmpRpcInform(cwmp_rpc_t*);
int8 cwmpBuildInformStruct(SoapInfoPtr InformStructPtr);
int8 cwmpBuildInformHeadStruct(SoapHeadInfoPtr  informHeadStructPtr);
int8 cwmpGetInformHeadId(char* headId);
int8 cwmpBuildInformBodyStruct(SoapBodyInfoPtr informBodyStructPtr);
int8 cwmpInitArgument(char* name, int number,  ParameterType type,  uint8 beArray,  RPCArgumentPtr* rpcArgPtrP);
int8 cwmpGetDeviceID(DeviceIdStructPtr *idPtrP);
int8 cwmpSetDeviceID(char* manufacture, char* oui, char* productClass, char* serialNumber);
int8 cwmpGetInformEventParaList(EventStructPtr *eventPtrP,  int* numP);
int8 cwmpAddInformEvent(char* eventCode, char* commandKey);
int8 cwmpGetMaxEnvelopes(void** valuePtrP);
int8 cwmpSetMaxEnvelopes(unsigned int  maxEnvelope);
int8	cwmpGetCurrentTime(void** valuePtrP);
int8	cwmpSetCurrentTime(char* curTime);
int8	cwmpGetRetryCount(void** valuePtrP);
int8	cwmpSetRetryCount(unsigned int  retryCount);
int8	cwmpGetInformParameterList(ParameterValueStructPtr* paraValuePtrP,  int* numP);
int8	cwmpAddInformParameter(char* name, char* value, ParameterType type);
#if CWMP_REDUCE_CODE
int8 cwmpClearInformEvent(void);
#endif

//extract information.
int cwmpExtractTreeInfoToStruct(IXML_Document* requestDocPtr, SoapInfoPtr  requestStructPtr);
int cwmpExtractBodyInfoToStruct(IXML_Node* requestBodyNode, SoapBodyInfoPtr  requestBodyStructPtr);
int cwmpExtractHeadInfoToStruct(IXML_Node* requestHeadNode, SoapHeadInfoPtr  requestHeadStructPtr);
int cwmpBuildDefautReqArg( SoapBodyInfoPtr  requestBodyStructPtr);
int cwmpBuildRequestArgument(RPCArgumentPtr requestArgumentPtr , IXML_Node* curBodyNode);
int cwmpExtractArgInfoToFaultDetailStruct(IXML_Node* curBodyNode, FaultDetailPtr* ArgP);
int cwmpBeArrayArgument(IXML_Node* argNode, ParameterType *argTypeP);
int cwmpBeArrayType(char* nodeName);
int cwmpExtractBodyInfoToValueStruct(IXML_Node* curBodyNode, ParameterValueStructPtr* tempParaP);
int cwmpExtractBodyInfoToAttriStruct(IXML_Node* curBodyNode, ParameterAttributesSetStructPtr* tempParaP);
int cwmpBuildAccessList(IN IXML_Node* bodyNode, OUT AccessEntry* accessList);
int cwmpGetArgumentType(char* attributeValue, ParameterTypePtr argumentTypePtr);

//build response struct.
int cwmpBuildResponseStruct(SoapInfoPtr  requestStructPtr, SoapInfoPtr  responseStructPtr);
int cwmpBuildResponseHeadStruct(SoapHeadInfoPtr  requestHeadStructPtr, SoapHeadInfoPtr  responseHeadStructPtr);
int cwmpBuildResponseBodyStruct(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int cwmpGetRPCMethodsResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int cwmpSetParameterValuesResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int cwmpGetParameterValuesResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int cwmpGetParameterNamesResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int cwmpSetParameterAttributesResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int cmwpGetParameterAttributesResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int cwmpRpcInformResponseResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int cwmpProcessGetRPCMethodsResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
//int8 cwmpProcessRPCResponse (RPCArgumentPtr  requestArgumentPtr, uint8 accessType, RPCArgumentPtr responseArgumentPtr);
int cwmpProcessRPCResponse (RPCArgumentPtr  requestArgumentPtr, uint8 accessType, RPCArgumentPtr responseArgumentPtr, int* status );
int cwmpGetParameterList(RPCArgumentPtr requestArgumentPtr, ParameterType resArgType, RPCArgumentPtr  responseArgumentPtr);
int cwmpSetParameterList(RPCArgumentPtr requestArgumentPtr, ParameterType reqArgType);
//int8 cwmpAddObject(RPCArgumentPtr requestArgumentPtr, ParameterType reqArgType);
int cwmpAddObject(RPCArgumentPtr requestArgumentPtr, uint32 *instanceNumber);
int cwmpResetAttribute(char *deviceName);
int cwmpAddObjectResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int cwmpDeleteObject(RPCArgumentPtr requestArgumentPtr);
int cwmpDeleteObjectResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int cwmpRebootResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int cwmpReboot(RPCArgumentPtr requestArgumentPtr);
int cwmpFactoryResetResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int cwmpScheduleInformResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);

int cwmpConfigArgumentStruct(char* name, ParameterType paraType, int number, uint8 beArray, RPCArgumentPtr argumentPtr);
int cwmpGetAllDeviceNodeInfo(char* requestParameterName, ParameterType resArgType, void** tempResParaPtr, int* paraNumberPtr);
int cwmpSetDeviceNodeInfo(void* requestParameter,ParameterType reqArgType);
int cwmpSetDeviceNodeValue(ValueInfoPtr valueInfoPtr, DeviceNodePtr deviceNode);
int8 cwmpGetBooleanValue(char* boolStr);
int8 cwmpIsNumeric(const char *valueStr);//added by hqfeng,2007.02.06
int cwmpSetDeviceNodeAttribute(ParameterAttributesSetStructPtr reqParameter, DeviceNodePtr deviceNode);
int cwmpSearchDeviceNodeByName(char* requestParameterName, DeviceNodePtr deviceRoot, DeviceNodePtr* curDeviceNode);
int cwmpGetSingleDeviceNodeInfo(DeviceNodePtr deviceNode, ParameterType resArgType, void** responseParameter);
int cwmpSetNoMoreRequest(char* noMoreRequest);
int cwmpGetRPCResponseFun(char* rpcName, RPCMethodFun* RPCMethodResponsePtr);
int cwmpPushDeviceStack(DeviceNodePtr deviceNode);
int cwmpPopDeviceStack(DeviceNodePtr* deviceNode);
int cwmpResetDeviceStack(void);//added by hqfeng,2007.01.05
int cwmpDivideDeviceNodeName( char **parameterName, char *name);
int cwmpGetDeviceNodeName(DeviceNodePtr deviceNode, char* deviceName);
uint8 cwmpClearNextLevelFlag();
uint8 cwmpSetNextLevelFlag();
uint8 cwmpGetNextLevelFlag();
uint8 cwmpSetRpcResponseFlag(cwmp_rpc_t *rpc);
uint8 cwmpGetRpcResponseFlag(cwmp_rpc_t *rpc);
uint8 cwmpResetRpcResponseFlag(cwmp_rpc_t *rpc);

//build response tree.
int cwmpBuildResponseTree(SoapInfoPtr responseStructPtr, IXML_Document* responseDoc);
int8 cwmpBuildEnvelopeNode(IXML_Document* responseDoc);
int8 cwmpBuildResponseHeadNode(SoapHeadInfoPtr responseHeadStructPtr, IXML_Node* responseHeadNode) ;
int cwmpBuildResponseBodyNode(SoapBodyInfoPtr responseBodyStructPtr, IXML_Node* responseBodyNode) ;
int cwmpBuildFaultBody(FaultStructPtr soapFaultPtr, IXML_Node* responseBodyNode);
int cwmpCreatArgumentValueNode(void* argValue, ParameterType argType, IXML_Node** argValueNode, IXML_Node** argAttrNode);
int8 cwmpSetBodyParaNode(void* parameterList, ParameterType argType, IXML_Node* bodyParaNode, void** newParaListP);
int8 cwmpCreateBasicParameterNode(void* parameter, char* typeString, IXML_Node* paraNode, IXML_Node** oldParaNode);
int8 cwmpCreateParaValueNode(ParameterValueStructPtr curPara, IXML_Node* curNode, IXML_Node** oldCurNodeP);
int8 cwmpCreateParaInfoNode(ParameterInfoStructPtr curPara, IXML_Node* curNode, IXML_Node** oldCurNodeP);
int8 cwmpCreateParaAttributeNode(ParameterAttributeStructPtr curPara, IXML_Node* curNode, IXML_Node** oldCurNodeP);
int8 cwmpCreateEventNode(EventStructPtr curPara, IXML_Node* curNode, IXML_Node** oldCurNodeP);
int8 cwmpCreatXMLNode(char* nodeName, char* nodeValue, IXML_NODE_TYPE nodeType, IXML_Node** envNode);

#if CWMP_USING_MBUF_FOR_TXBUF
//build txBuf.
int cwmpFrameTreeToBuf(IXML_Document* responseDoc, mbuf_t **txBuf);
#else
int cwmpFrameTreeToBuf(IXML_Document* responseDoc, char **txBuf);
#endif

//add by brian for removing warning
int8 cwmpBuildFileTransferResponse(SoapBodyInfoPtr BodyStructPtr, int status);

//release soap doc and struct.
int8 cwmpReleaseSoapDoc(IXML_Document* soapDocPtr);
int8 cwmpReleaseSoapDoc_new(IXML_Document* soapDocPtr);
int8 cwmpReleaseSoapStruct(SoapInfoPtr soapStructPtr);
int8 cwmpReleaseSoapHeadStruct(SoapHeadInfoPtr  *headStructPtrP);
int8 cwmpReleaseSoapBodyStruct(SoapBodyInfoPtr *bodyStructPtrP);
int8 cwmpFreeArgment(RPCArgument **argument);
int8 cwmpFreeParameterList(void* parameterList, ParameterType argType);
int8 cwmpFreeParameter(void* curPara, ParameterType argType, void** nextPara);
int8 cwmpReleaseFaultStruct(FaultStructPtr soapFaultPtr);

//Value change function.
void cwmpAddNotifyNodeList(NotifyDeviceNodeInfoPtr notifyNodeP, NotifyType notify);
void cwmpDeleteNotifyNodeList(DeviceNode* deviceNode, NotifyType notify);
int cwmpFreeNotifyNode(NotifyDeviceNodeInfoPtr *NotifyNodeP, NotifyDeviceNodeInfoPtr *NotifyListHeadP, NotifyDeviceNodeInfoPtr *NotifyListTailP);
void cwmpModifyNotifyNodeList(DeviceNode* deviceNode, NotifyType notify);
int8 cwmpCreateNotifyNode(DeviceNodePtr deviceNode, NotifyDeviceNodeInfoPtr* notifyNodeP);
int8 cwmpInitNotifyNodeList(void);
int8 cwmpClearNotifyNodeList( NotifyType notify);
int8 cwmpMultipulateNotifyListByNewAttr(DeviceNodePtr deviceNode, uint8 newNotification);
int8 cwmpMultipulateNotifyListByNewValue(DeviceNodePtr deviceNode);
//modify by brian for removing warning
int/*8*/ cwmpPushValueChangeStack(DeviceNodePtr deviceNode);
int8 cwmpPopValueChangeStack(DeviceNodePtr* deviceNode);
int8 cwmpSearchChangedValue(NotifyType notify);
int8 cwmpBeForcedNotifyNode(DeviceNodePtr deviceNode);
int8 cwmpBuildValueChangeParameters(void);
int cwmpCreateDeviceNodeName(DeviceNodePtr deviceNode, char** deviceNodeNameP);
/***************************************** End *************************************************/



/***************************** Using  Mbuf For Storing TxBuf Message ********************************/
mbuf_t *cwmpGetNextMbuf(mbuf_t* curMbuf);
void  cwmpAddMbufList(mbuf_t *mbuf);
void cwmpResetMbufList(void);
int32 cwmpCountMbufListDataLength(void);
int8 cwmpCopyStringToMbuf(char* Msg);
int8 cwmpCopyMbufToString(char* sBuf, uint32 sBufSize);
mbuf_t *cwmpGetMbufListHead(void);
uint8 cwmpGetAllocMbufFlag(void);
void cwmpSetAllocMbufFlag(uint8 value);
void cwmpSetFirstMbufFlag(uint8 value);
void cwmpSetParaMbufList(void);
void cwmpLinkParaMbufList(void);
//void cwmpDumpMbufData(void);
/***************************************** End *************************************************/



// Initiaiization
int cwmpRpcInit(cwmp_rpc_t*);


// CPE Methods, ACS can call these methods

int/*8*/ cwmpDownloadResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int/*8*/ cwmpUploadResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int8 cwmpPerformFileTransfer(void);
int/*8*/ cwmpGetFileTransferArgument(RPCArgumentPtr arg);
//void cwmpFreeFileTransferStruct(FileTransferStructPtr dsp);
int/*8*/ cwmpTransferCompleteResponseResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
int cwmpBuildTransferCompleteBodyStruct(SoapBodyInfoPtr BodyStructPtr);
int	cwmpGetFaultDetail(FaultDetailPtr *faultDetailPtrP);
int/*8*/ cwmpFaultCodeResponse(SoapBodyInfoPtr  requestBodyStructPtr, SoapBodyInfoPtr  responseBodyStructPtr);
static int dealACSFaultCode(int acsFaultCode, cwmp_rpc_t **rpcPtr);

//CPE gets ACS's RPC Methods,hqfeng,2006.07.11
int cwmpBuildGetRPCMethodsBodyStruct(SoapBodyInfoPtr BodyStructPtr);

#if 0
int cwmpGetParameterNames(cwmp_rpc_t* rpc);
int cwmpRpcGetParameterNameResponse(cwmp_rpc_t*);
int cwmpGetParameterAttributes(cwmp_rpc_t* rpc);
int cwmpSetParameterAttributes(cwmp_rpc_t*);
int cwmpSetParameterAttributesResponse(cwmp_rpc_t*);
int cmwpGetParameterAttributesResponse(cwmp_rpc_t*);
int cwmpAddObject(cwmp_rpc_t*);
int cwmpAddObjectResponse(cwmp_rpc_t*);
int cwmpDeleteObject(cwmp_rpc_t*);
int cwmpDeleteObjectResponse(cwmp_rpc_t*);
int cwmpDownload(cwmp_rpc_t*);
int cwmpDownloadResponse(cwmp_rpc_t*);
int cwmpReboot(cwmp_rpc_t*);
int cwmpRebootResponse(cwmp_rpc_t*);
int cwmpGetRpcMethods(cwmp_rpc_t* rpc );

#endif
// Server Methods

	
// General
int cwmpRpcDispatch(cwmp_rpc_t *);
#if CWMP_REDUCE_CODE
void cwmpRpcNodeTreeRecursive(IXML_Node *nodeptr);
#endif
void	cwmpParseSoapPacket(void);
#ifndef CODE_SHRINK  //qingpu.20080110
int8 cwmpGetFaultParaList(void);
#endif

void cwmpItoa( int num, char* str, int type);
uint8 getCwmpRebootFlag(uint8* rebootFlag);

//Build fault code struct
int cwmpCreateDetailFaultStruct(int faultCode, char* faultString);
int cwmpAddSetParameterValuesFaultList(char* parameterName, int faultCode, char* faultString);
//int8 cwmpBuildFaultStruct(int errorCode);
int8 cwmpInitFaultStruct(cwmp_rpc_t *rpc);
#if defined(TCSUPPORT_CWMP_PRECISE_TIME)
char * TimeFormatConvert_improve(uint32 time);
#endif
char * TimeFormatConvert(uint32 time);

void printLogsysfile(char* logsysfile);
void traceRoute(cwmp_rpc_t* rpc);
void sendInform();
#endif 
