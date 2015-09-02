#ifndef CP_WANDEV_WANCONNDEV_PTMLINKCFG_H_
#define CP_WANDEV_WANCONNDEV_PTMLINKCFG_H_
#include "cp_core.h"

/*rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANPTMLinkConfig.*/
int8 getWANPTMLinkCfgEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPTMLinkCfgEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDPTMLinkCfgEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgLinkStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgLinkStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgLinkStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgMACAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgMACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgMACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgFramesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgFramesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgFramesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgFramesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgFramesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgFramesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgOOSNearEndValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgOOSNearEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgOOSNearEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgOOSFarEndValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgOOSFarEndAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgOOSFarEndAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgErrorsSendValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgErrorsSendAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgErrorsSendAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgErrorsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgErrorsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgErrorsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgUnicastPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgUnicastPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgUnicastPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgUnicastPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgUnicastPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgUnicastPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgDiscardPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgDiscardPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgDiscardPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgDiscardPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgDiscardPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgDiscardPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgMulticastPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgMulticastPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgMulticastPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgMulticastPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgMulticastPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgMulticastPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgBroadcastPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgBroadcastPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgBroadcastPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgBroadcastPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgBroadcastPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgBroadcastPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPTMLinkCfgUnknownProtoPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPTMLinkCfgUnknownProtoPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPTMLinkCfgUnknownProtoPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
