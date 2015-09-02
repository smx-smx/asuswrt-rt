#ifndef CP_LANCONFIGSECURITY_H_
#define CP_LANCONFIGSECURITY_H_
#include "cp_core.h"

#define WEB_PSWD_LEN			32
#define USER_NAME_LEN			32
int8 getConfigPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setConfigPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getConfigPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConfigPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getX_TC_UsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setX_TC_UsernameValue(void* value, struct DeviceNode*  deviceNode);
int8 getX_TC_UsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setX_TC_UsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#endif
