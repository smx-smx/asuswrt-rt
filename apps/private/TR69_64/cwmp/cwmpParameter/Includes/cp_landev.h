#ifndef CP_LANDEV_H_
#define CP_LANDEV_H_

#include "cp_core.h"

/*
	This Node:    InternetGatewayDevice.LANDevice.{i}
	Parent Node: InternetGatewayDevice.LANDevice.
	Parent Node Table Location: cp_topnodetable.c 
*/
extern CwmpParameterStruct LanDeviceIndex[];

#if 0
/*
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
extern CwmpParameterStruct WLANConfiguration[];
#endif

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.1.WPS
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.1.
	Parent Node Table Location: cp_landevice_nodetable.c
*/

#ifdef WSC_AP_SUPPORT
extern CwmpParameterStruct WPSTable[];

int cwmpInitWLANWPS(DeviceNodePtr deviceNode);
#endif

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.{i}
	Parent Node: InternetGatewayDevice.LANDevice.{i}.WLanConfiguration.
	Parent Node Table Location: cp_landevice_nodetable.c
*/

extern CwmpParameterStruct WLANConfigurationIndex[];

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.IPInterface.{i}
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landev_nodetable.c
*/

extern CwmpParameterStruct IPInterfaceIndex[];

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceConfig.{i}
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceConfig.
	Parent Node Table Location: cp_landev_nodetable.c
*/

extern CwmpParameterStruct EthInterCfgIndex[];
/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.Hosts.
	Parent Node: InternetGatewayDevice.LANDevice.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/

extern CwmpParameterStruct Hosts[];

int	cwmpInitWLANConfiguration1SubTree( DeviceNodePtr deviceNode );

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceNumberOfEntries
	Parent Node: InternetGatewayDevice.LANDevice.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getLANEthIfNumOfEntries1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLANEthIfNumOfEntries1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLANEthIfNumOfEntries1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANUSBInterfaceNumberOfEntries
	Parent Node: InternetGatewayDevice.LANDevice.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getLANUSBIfNumOfEntries1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLANUSBIfNumOfEntries1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLANUSBIfNumOfEntries1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANWLANConfigurationNumberOfEntries
	Parent Node: InternetGatewayDevice.LANDevice.{i}.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getLANWLANConfigNumOfEntries1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLANWLANConfigNumOfEntries1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLANWLANConfigNumOfEntries1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPServerConfigurable
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getDHCPServerCfg1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPServerCfg1Value(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPServerCfg1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPServerCfg1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPServerEnable
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getDHCPServerEnable1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPServerEnable1Value(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPServerEnable1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPServerEnable1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPRelay
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getDHCPRelay1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPRelay1Value(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPRelay1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPRelay1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.MinAddress
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getMinAddress1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMinAddress1Value(void* value, struct DeviceNode*  deviceNode);
int8 getMinAddress1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMinAddress1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.MaxAddress
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getMaxAddress1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMaxAddress1Value(void* value, struct DeviceNode*  deviceNode);
int8 getMaxAddress1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxAddress1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.ReservedAddresses
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getReservedAddr1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setReservedAddr1Value(void* value, struct DeviceNode*  deviceNode);
int8 getReservedAddr1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setReservedAddr1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.SubnetMask
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getSubnetMask1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSubnetMask1Value(void* value, struct DeviceNode*  deviceNode);
int8 getSubnetMask1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSubnetMask1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DNSServers
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getDNSServers1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDNSServers1Value(void* value, struct DeviceNode*  deviceNode);
int8 getDNSServers1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDNSServers1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DomainName
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
	Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getDomainName1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDomainName1Value(void* value, struct DeviceNode*  deviceNode);
int8 getDomainName1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDomainName1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.IPRouters
        Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
        Parent Node Table Location: cp_landevice_nodetable.c
*/
int8 getIPRouters1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPRouters1Value(void* value, struct DeviceNode*  deviceNode);
int8 getIPRouters1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPRouters1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPLeaseTime
        Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
        Parent Node Table Location: cp_landevice_nodetable.c
*/
#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getDHCPLeaseTime1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPLeaseTime1Value(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPLeaseTime1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPLeaseTime1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif
/*
      This Node:     InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.X_TTNET_IGMPSnoopEnable
        Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.
        Parent Node Table Location: cp_landevice_nodetable.c
*/

int8 getIPInterfaceNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getIPInterfaceNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPInterfaceNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int	cwmpInitLANIPInterfaceSubTree( DeviceNodePtr deviceNode );
int	cwmpInitLANEthInterCfgSubTree( DeviceNodePtr deviceNode );

#ifdef DHCP_PROFILE
/*
      This Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPOption.{i}.
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPOption.
	Parent Node Table Location: cp_landev_nodetable.c 
*/
extern CwmpParameterStruct DHCPOptionIndex[];

/*
      This Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.
	Parent Node Table Location: cp_landev_nodetable.c
*/
extern CwmpParameterStruct DHCPConSerPoolIndex[];

#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getDHCPOptionNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDHCPOptionNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPOptionNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDHCPConditionalPoolNumOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDHCPConditionalPoolNumOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPConditionalPoolNumOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int cwmpInitDHCPOptionSubTree( DeviceNodePtr deviceNode );
int cwmpInitDHCPConSerPoolSubTree( DeviceNodePtr deviceNode );

#endif
#endif
#endif
