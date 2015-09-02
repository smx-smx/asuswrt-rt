#ifndef CP_LANDEV_IPINTERFACE_H_
#define CP_LANDEV_IPINTERFACE_H_

#include "cp_core.h"

int8 getIPInterface_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPInterface_EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPInterface_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPInterface_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getIPInterfaceIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPInterfaceIPAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPInterfaceIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPInterfaceIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getIPInterfaceSubnetMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPInterfaceSubnetMaskValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPInterfaceSubnetMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPInterfaceSubnetMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getIPInterfaceAddressingTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPInterfaceAddressingTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPInterfaceAddressingTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPInterfaceAddressingTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

uint8 cnt_mask_bits(ip4a	subnet_mask);
#endif

