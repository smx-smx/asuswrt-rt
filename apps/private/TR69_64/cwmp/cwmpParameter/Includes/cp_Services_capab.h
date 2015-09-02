#ifndef CP_SERVICES_CAPAB_H_
#define CP_SERVICES_CAPAB_H_

#include "cp_core.h"

#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getMaxProfileCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMaxProfileCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxProfileCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMaxLineCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMaxLineCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxLineCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFaxT38Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getFaxT38Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFaxT38Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#endif

