#ifndef CP_USRITF_H_
#define CP_USRITF_H_

#include "cp_core.h"


#if !defined(TCSUPPORT_CWMP_CS_BSNL)
extern CwmpParameterStruct UserInterface[];

int8 getWarrantyDateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWarrantyDateValue(void* value, struct DeviceNode*  deviceNode);
int8 getWarrantyDateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWarrantyDateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif
#endif
