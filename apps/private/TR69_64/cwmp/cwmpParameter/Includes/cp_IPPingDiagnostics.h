#ifndef CP_IPPINGDIAGNOSTICS_H_
#define CP_IPPINGDIAGNOSTICS_H_

#include "cp_core.h"

#define MAX_DOMAIN_LENGTH 256
#if !defined(TCSUPPORT_CWMP_CS_BSNL)
extern CwmpParameterStruct IPPingDiagnostics[];
#endif
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
#if !defined(TCSUPPORT_CWMP_CS_BSNL)
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
#endif
void getCWMPPingResult(void);
void *cwmpPingTask(void);
int cwmpPingTaskStart(void);
void doCwmpPing(void);
#ifdef THREAD_REDUCE
static void resetCwmpPing(void);
#endif
#endif

