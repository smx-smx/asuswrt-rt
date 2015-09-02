#ifndef CP_WANDEV_WANCONNDEV_IP_PPP_CONN_H_
#define CP_WANDEV_WANCONNDEV_IP_PPP_CONN_H_

#include "cp_core.h"

extern spSysParam_t spSysParam;
extern uint16 ACSaddingPVCFlag;
#define MAX_VIRSERV_RULE 10


/*
	Parent Node: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.PortMapping.
	Parent Node Table Location: cp_wandev_wanconndev_subtree_nodetable.c
*/
extern CwmpParameterStruct PortMappingIndex[];

/*rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.*/
int8 getWANCnt1_1EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1StatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1StatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1StatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1PsbTypesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1PsbTypesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1PsbTypesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1TypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1TypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1TypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1TypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1NameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1NameValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1NameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1NameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1UptimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1UptimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1UptimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1LErrorValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1LErrorAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1LErrorAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANIPCnt1_1RSIPAvailableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1RSIPAvailableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1RSIPAvailableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1NATEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1NATEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1NATEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1NATEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANIPCnt1_1AddrTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANIPCnt1_1AddrTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1AddrTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1AddrTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1ExtIPAddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1ExtIPAddrValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1ExtIPAddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1ExtIPAddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANIPCnt1_1SubMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANIPCnt1_1SubMaskValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1SubMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1SubMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANIPCnt1_1DefGatewayValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANIPCnt1_1DefGatewayValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1DefGatewayAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1DefGatewayAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1DNSEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1DNSEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1DNSEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1DNSEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1DNSOvAllowedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1DNSOvAllowedValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1DNSOvAllowedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1DNSOvAllowedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1DNSServersValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1DNSServersValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1DNSServersAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1DNSServersAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


int8 getWANCnt1_1MACAddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1MACAddrValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1MACAddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1MACAddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1CntTriggerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1CntTriggerValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1CntTriggerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1CntTriggerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1RProRxValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANCnt1_1RProRxValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANIPCnt1_1RProRxAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1RProRxAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANCnt1_1PMapNumOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANIPCnt1_1PMapNumOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANIPCnt1_1PMapNumOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.PortMapping.{i}.*/
int	cwmpInitWANGetPortMappingFromFlash( DeviceNodePtr deviceNode );
int8 DeviceNodeAddPMappingObject(uint32* num, DeviceNode*  deviceNode);
int8 DeviceNodeDelPMappingObject(uint32 num, DeviceNode*  deviceNode);

int8 getIPPMapping1EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPMapping1EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPMapping1EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPMapping1EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPPMapping1LeaseDurValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPMapping1LeaseDurValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPMapping1LeaseDurAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPMapping1LeaseDurAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIP1RemoteHostValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIP1RemoteHostValue(void* value, struct DeviceNode*  deviceNode);
int8 getIP1RemoteHostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIP1RemoteHostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIP1ExternalPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIP1ExternalPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getIP1ExternalPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIP1ExternalPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getIPExternalPortEndRangeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPExternalPortEndRangeValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPExternalPortEndRangeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPExternalPortEndRangeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

int8 getIP1InternalPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIP1InternalPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getIP1InternalPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIP1InternalPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPPMapping1ProtocolValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPMapping1ProtocolValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPMapping1ProtocolAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPMapping1ProtocolAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIP1InternalClientValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIP1InternalClientValue(void* value, struct DeviceNode*  deviceNode);
int8 getIP1InternalClientAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIP1InternalClientAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPPMapping1DescripValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPPMapping1DescripValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPPMapping1DescripAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPPMapping1DescripAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.IPConnection.1.Stats.*/
int8 getEthernetBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthernetBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthernetPacketsSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetPacketsSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetPacketsSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEthernetPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getEthernetPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEthernetPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
/*rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.PPPConnection.1.*/
int8 getWANPPPCnt1_1UsernameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1UsernameValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1UsernameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1UsernameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPPPCnt1_1PasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1PasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1PasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1PasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPPPCnt1_1TransTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1TransTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1TransTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

//#ifdef TC_TR069
int8 getWANPPPCnt1_1AuthenTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1AuthenTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1AuthenTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1AuthenTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
//#endif


int8 getWANPPPCnt1_1PPPoEACNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1PPPoEACNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1PPPoEACNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1PPPoEACNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANPPPCnt1_1PPPoESvcNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANPPPCnt1_1PPPoESvcNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANPPPCnt1_1PPPoESvcNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANPPPCnt1_1PPPoESvcNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif
