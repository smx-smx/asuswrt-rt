#ifndef CP_DEVCONFIG_H_
#define CP_DEVCONFIG_H_

#include "cp_core.h"

/*Define character(s) to  replace '<' and '>'  */
#define LEFT_REPLACE	"@@"
#define RIGHT_REPLACE	"@@"
#define BUF_APPEND	512

/*This two flags are used to identify left or right bracket when the LEFT_REPLACE and RIGHT_REPLACE are identical*/
#define LEFT_FLAG	0
#define RIGHT_FLAG	1

extern CwmpParameterStruct DeviceConfig[];


#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getPersistentDataValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPersistentDataValue(void* value, struct DeviceNode*  deviceNode);
int8 getPersistentDataAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPersistentDataAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getConfigFileValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setConfigFileValue(void* value, struct DeviceNode*  deviceNode);
int8 getConfigFileAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setConfigFileAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif
#endif
