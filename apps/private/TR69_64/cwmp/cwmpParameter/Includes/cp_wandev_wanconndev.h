#ifndef CP_WANDEV_WANCONNDEV_H_
#define CP_WANDEV_WANCONNDEV_H_

#include "cp_core.h"
#include "cp_wandev.h"
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANDSLLinkConfig.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
extern CwmpParameterStruct WANDSLLinkConfig[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANATMF5LoopbackDiagnostics.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
extern CwmpParameterStruct WANATMF5LoopbackDiagnostics[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANIPConnection[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.PPPConnection.1.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANPPPConnection[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct WANDeviceIndex[];

#if defined(TCSUPPORT_CWMP_VDSL)
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANPTMLinkConfig.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
extern CwmpParameterStruct WANPTMLinkConfig[];
#endif

extern spSysParam_t spSysParam;
extern uint16 ACSaddingPVCFlag;

int8 DeviceNodeDelPVCObject(uint32 num, DeviceNode*  deviceNode);
int8 cwmpAddConnectionObject(uint32* num, DeviceNode*  deviceNode);
int8 cwmpDeleteConnectionObject(uint32 num, DeviceNode*  deviceNode);

int8 getWANIPCntNumOfEntries1_1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCntNumOfEntries1_1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCntNumOfEntries1_1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPPPCntNumOfEntries1_1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPPPCntNumOfEntries1_1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCntNumOfEntries1_1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif


