#ifndef CP_WANDEV_H_
#define CP_WANDEV_H_

#include "cp_core.h"

#if defined(TCSUPPORT_CWMP_VDSL)
#define TOTAL_REMOTE_ATM_DEVICE_NODE 	8
#define TOTAL_REMOTE_PTM_DEVICE_NODE 	2
#define TOTAL_REMOTE_DEVICE_NODE 	(TOTAL_REMOTE_ATM_DEVICE_NODE+TOTAL_REMOTE_PTM_DEVICE_NODE)
#else
#define TOTAL_REMOTE_DEVICE_NODE 	8
#endif
extern spSysParam_t spSysParam;

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLDiagnostics.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANDSLDiagnostics[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct WANDeviceIndex[];

/*Acs is adding pvc object*/
extern CwmpParameterStruct WANConnectionDeviceIndex_1[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANConnectionDeviceIndex_2[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANDSLInterfaceConfig[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLCommonInterfaceConfig.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANCommonInterfaceConfig[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLConnectionManagement.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANDSLConnectionManagement[];
#if 0
/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANConnectionDeviceIndex_2_IP[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.
	Parent Node Table Location: cp_wandev_nodetable.c
*/
extern CwmpParameterStruct WANConnectionDeviceIndex_2_PPP[];
#endif

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
extern CwmpParameterStruct WANConnectionDevice_2[];
#if defined(TCSUPPORT_CWMP_VDSL)
extern CwmpParameterStruct WANConnectionDevice_2_PTM[];
/*Acs is adding pvc object for PTM*/
extern CwmpParameterStruct WANConnectionDeviceIndex_1_PTM[];
#endif

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
extern CwmpParameterStruct WANConnectionDevice_2_IP[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.
	Parent Node Table Location: cp_wandev_wanconndev_nodetable.c
*/
extern CwmpParameterStruct WANConnectionDevice_2_PPP[];


int8 DeviceNodeAddPVCObject(uint32 *num, DeviceNode*  deviceNode);
int  cwmpInitWANConnectionDeviceTable( DeviceNodePtr deviceNode );
#endif

