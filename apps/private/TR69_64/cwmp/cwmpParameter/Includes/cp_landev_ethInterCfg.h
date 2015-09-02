#ifndef CP_LANDEV_ETHINTERCFG_H_
#define CP_LANDEV_ETHINTERCFG_H_

#include "cp_core.h"

#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getEthInterCfg_StatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthInterCfg_StatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthInterCfg_StatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getEthInterCfg_MACAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthInterCfg_MACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthInterCfg_MACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif


#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getStats_BytesStValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getStats_BytesStAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setStats_BytesStAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getStats_BytesRvdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getStats_BytesRvdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setStats_BytesRvdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getStats_PacketsStValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getStats_PacketsStAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setStats_PacketsStAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getStats_PacketsRvdValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getStats_PacketsRvdAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setStats_PacketsRvdAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif


#endif

