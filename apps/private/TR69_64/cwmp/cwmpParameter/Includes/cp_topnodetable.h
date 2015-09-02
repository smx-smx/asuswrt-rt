#ifndef CP_TOPNODETABLE_H_
#define CP_TOPNODETABLE_H_

#include "cp_core.h"

int8 getDeviceSummaryValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDeviceSummaryAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceSummaryAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLanDeviceNumberOfEntriesValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 getLanDeviceNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLanDeviceNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getWanDeviceNumberOfEntries(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 getWanDeviceNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWanDeviceNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

extern CwmpParameterStruct InternetGatewayDevice[];
extern CwmpParameterStruct TopIGDNode[];
/*
	Parent Node: InternetGatewayDevice.DeviceInfo.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct DeviceInfo[];

/*
	Parent Node: InternetGatewayDevice.ManagementServer.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct ManagementServer[];

/*
	Parent Node: InternetGatewayDevice.IPPingDiagnostics.
	Parent Node Table Location: cp_topnodetable.c
*/
#if !defined(TCSUPPORT_CWMP_CS_BSNL)
extern CwmpParameterStruct IPPingDiagnostics[];
#endif

#ifdef TCSUPPORT_TRACEROUTE
/*
	Parent Node: InternetGatewayDevice.TraceRouteDiagnostics.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct TraceRouteDiagnostics[];
#endif

/*
	This Node:    InternetGatewayDevice.LANDevice.{i}
	Parent Node: InternetGatewayDevice.LANDevice.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct LanDeviceIndex[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct WANDeviceIndex[];

/*
	Parent Node: InternetGatewayDevice.Services.
	Parent Node Table Location: cp_topnodetable.c
*/
#if !defined(TCSUPPORT_CWMP_CS_BSNL)
extern CwmpParameterStruct Services[];

/*
	Parent Node: InternetGatewayDevice.Time.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct Time[];

/*
	Parent Node: InternetGatewayDevice.DeviceConfig.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct DeviceConfig[];
#endif
/*
	Parent Node: InternetGatewayDevice.Layer3Forwarding.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct Layer3Forwarding[];

/*
	Parent Node: InternetGatewayDevice.LanConfigSecurity.
	Parent Node Table Location: cp_topnodetable.c
*/
extern CwmpParameterStruct LanConfigSecurity[];

/*
	Parent Node: InternetGatewayDevice.QueueManagement.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct QueueManagement[];

/*
	Parent Node: InternetGatewayDevice.UserInterface.
	Parent Node Table Location: cp_topnodetable.c 
*/
#if !defined(TCSUPPORT_CWMP_CS_BSNL)
extern CwmpParameterStruct UserInterface[];
#endif
#ifdef TR143
/*
	Parent Node: InternetGatewayDevice.Capabilities.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct tr143_Capabilities[];

/*
	Parent Node: InternetGatewayDevice.DownloadDiagnostics.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct DownloadDiagnostics[];

/*
	Parent Node: InternetGatewayDevice.UploadDiagnostics.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct UploadDiagnostics[];
/*
	Parent Node: InternetGatewayDevice.UDPEchoConfig.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct UDPEchoConfig[];

#endif
#if !defined(TCSUPPORT_CWMP_CS_BSNL)
#if defined(TCSUPPORT_LAYER2BRIDGING)
extern CwmpParameterStruct Layer2Bridging[];
#endif
#endif
/*
	Parent Node: InternetGatewayDevice.X_TTNET_Service.
	Parent Node Table Location: cp_topnodetable.c 
*/
#endif
