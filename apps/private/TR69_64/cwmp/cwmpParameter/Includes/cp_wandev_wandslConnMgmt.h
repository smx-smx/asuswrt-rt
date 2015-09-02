#ifndef CP_WANDEV_WANDSLCONNMGMT_H_
#define CP_WANDEV_WANDSLCONNMGMT_H_

#include "cp_core.h"

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLConnectionManagement.
	Parent Node Table Location: cp_wandev_wandslConnMgmt_nodetable.c
*/
#if !defined(TCSUPPORT_CWMP_CS_BSNL)
extern CwmpParameterStruct ConnectionServiceIndex[];

/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANDSLConnectionManagement.ConnectionService.
	Parent Node Table Location: cp_wandev_wandslConnMgmt_nodetable.c
*/
extern CwmpParameterStruct ConnectionService[];

/*----------------------dsl connection management function begin--------------------------------------------*/
int8 getConnSerNumbeOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getConnSerNumbeOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConnSerNumbeOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*--------------------dsl connection management function end-----------------------------------------------*/

int	cwmpInitConnectionServiceSubTree( DeviceNodePtr deviceNode );

/*--------------------dsl connection management connection service function begin-----------------------------*/
int8 getWANConnectionDeviceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANConnectionDeviceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANConnectionDeviceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANConnectionServiceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANConnectionServiceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANConnectionServiceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDestinationAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDestinationAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDestinationAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLinkTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLinkTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLinkTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getConnectionTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getConnectionTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConnectionTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif
/*--------------------dsl connection management connection service function end-----------------------------*/

#endif

