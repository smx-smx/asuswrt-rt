#ifndef CP_MANAGERMENT_H_
#define CP_MANAGERMENT_H_

#include "cp_core.h"

extern CwmpParameterStruct ManagementServer[];

#ifdef TR111
extern CwmpParameterStruct ManageableDevice[];
extern CwmpParameterStruct ManageableDeviceIndex[];
#endif

#define CIRCLE_EXTERNAL 8
#define CIRCLE_INTERNAL 32
int8 getEnableCWMPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEnableCWMPValue(void* value, struct DeviceNode*  deviceNode);
int8 getEnableCWMPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEnableCWMPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setURLValue(void* value, struct DeviceNode*  deviceNode);
int8 getURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUsernameValue(void* value, struct DeviceNode*  deviceNode);
int8 getUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8   setPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8   setPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPeriodicInformEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPeriodicInformEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getPeriodicInformEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPeriodicInformEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPeriodicInformIntervalValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPeriodicInformIntervalValue(void* value, struct DeviceNode*  deviceNode);
int8 getPeriodicInformIntervalAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPeriodicInformIntervalAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode); 

int8 getPeriodicInformTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPeriodicInformTimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getPeriodicInformTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPeriodicInformTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getParameterKeyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getParameterKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setParameterKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getConnectionRequestURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getConnectionRequestURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConnectionRequestURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getConnectionRequestUsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setConnectionRequestUsernameValue(void* value, struct DeviceNode*  deviceNode);
int8 getConnectionRequestUsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConnectionRequestUsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getConnectionRequestPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setConnectionRequestPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getConnectionRequestPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConnectionRequestPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUpgradesManagedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUpgradesManagedValue(void* value, struct DeviceNode*  deviceNode);
int8 getUpgradesManagedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUpgradesManagedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#ifdef TR111
int cwmpInitManageableDeviceSubTree(DeviceNodePtr deviceNode);
int8 getmDevNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getmDevNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setmDevNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getmDevNotificationLimitValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setmDevNotificationLimitValue(void* value, struct DeviceNode*  deviceNode);
int8 getmDevNotificationLimitAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setmDevNotificationLimitAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
int8 getmDevOUIValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getmDevOUIAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 getmDevSNValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getmDevSNAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 getmDevClassValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getmDevClassAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 getmDevHostValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getmDevHostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 RebuildmDevObject();
#endif

#endif
