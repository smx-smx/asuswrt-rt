#ifndef CP_X_TTNET_Servie_H_
#define CP_X_TTNET_Servie_H_

#include "cp_core.h"

#define MAX_IPMACFILTER_RULE 16

#if !defined(TCSUPPORT_CWMP_CS_BSNL)
extern CwmpParameterStruct X_TTNET_Service[];
extern CwmpParameterStruct X_TTNET_Filter[];
extern CwmpParameterStruct MACFilterDevice[];
extern CwmpParameterStruct MACFilterDeviceIndex[];

int8 getX_TTNET_UPnPEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setX_TTNET_UPnPEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getX_TTNET_UPnPEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_TTNET_UPnPEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getX_TTNET_RemoteAccessEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setX_TTNET_RemoteAccessEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getX_TTNET_RemoteAccessEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_TTNET_RemoteAccessEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int isHexNumber(char c);
int isValidMacAddress(char* mac);
int8 DeviceNodeAddMACFilterObject(uint32* num, DeviceNode*  deviceNode);
int8 DeviceNodeDelMACFilterObject(uint32 num, DeviceNode*  deviceNode);
int cwmpInitMACFilterSubTree(DeviceNodePtr deviceNode);
int8 getEnabledValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 setEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getMACAddressValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 setMACAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getMACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

#endif
#endif

