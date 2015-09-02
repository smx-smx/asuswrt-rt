#ifndef CP_DEVICEINFO_D_H_
#define CP_DEVICEINFO_D_H_

#include "cp_core.h"

extern int DeviceInfo_init(void);
int8 getManufacturerOUI_TestDynamicValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getManufacturerOUI_TestDynamicAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 getSerialNumber_TestDynamicValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSerialNumber_TestDynamicAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);

extern CwmpParameterStruct DeviceInfo_D[];
#endif
