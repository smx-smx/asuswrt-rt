#ifndef CP_TRACEROUTEDIAGNOSTICS_H_
#define CP_TRACEROUTEDIAGNOSTICS_H_

#include "cp_core.h"

#define MAX_DOMAIN_LENGTH 256
extern CwmpParameterStruct TraceRouteDiagnostics[];
int	cwmpInitRouteHops1SubTree( DeviceNodePtr deviceNode );


//add by xyyou for traceroute diagnostic

typedef struct traceroute_s{
	ip4a   	target;
	uint32  NumberofTries;
	uint32 Timeout;
	uint32 DataBlockSize;
	uint32 MaxHopCount;
	uint32 ResponseTime;
	uint16 RouteHopsNumberOfEntries;
} traceroute_t;

typedef struct tracerouteHophost_s{
	char HopHost[257];
	char HopHostAddress[16];
	uint32 HopErrorCode;
	char HopRTTime[33];
	
} tracerouteHophost_t;

int8 getTraceRouteDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTraceRouteDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getTraceRouteDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTraceRouteDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTraceRouteInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTraceRouteInterfaceValue(void* value, struct DeviceNode*  deviceNode);
int8 getTraceRouteInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTraceRouteInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTraceRouteHostValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTraceRouteHostValue(void* value, struct DeviceNode*  deviceNode);
int8 getTraceRouteHostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTraceRouteHostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getNumberOfTriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setNumberOfTriesValue(void* value, struct DeviceNode*  deviceNode);
int8 getNumberOfTriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setNumberOfTriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTraceRouteTimeoutValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTraceRouteTimeoutValue(void* value, struct DeviceNode*  deviceNode);
int8 getTraceRouteTimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTraceRouteTimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTraceRouteDataBlockSizeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTraceRouteDataBlockSizeValue(void* value, struct DeviceNode*  deviceNode);
int8 getTraceRouteDataBlockSizeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTraceRouteDataBlockSizeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTraceRouteDSCPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTraceRouteDSCPValue(void* value, struct DeviceNode*  deviceNode);
int8 getTraceRouteDSCPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTraceRouteDSCPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMaxHopCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMaxHopCountValue(void* value, struct DeviceNode*  deviceNode);
int8 getMaxHopCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxHopCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHopHostValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHopHostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHopHostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHopHostAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHopHostAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHopHostAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getHopErrorCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHopErrorCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHopErrorCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getHopRTTimesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHopRTTimesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHopRTTimesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

	
int8 getRouteHopsNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getRouteHopsNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setRouteHopsNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int getCWMPTraceRouteResult(void);
void *cwmpTraceRouteTask(void);
int cwmpTraceRouteTaskStart(void);
void doCwmpTraceRoute(void);
int RebuildRouteHopsObject(void);

#ifdef THREAD_REDUCE
static void resetCwmpTraceRoute(void);
#endif
#endif

