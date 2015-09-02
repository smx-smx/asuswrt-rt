#ifndef CP_LANDEV_HOSTS_H_
#define CP_LANDEV_HOSTS_H_

#include "cp_core.h"

#define BLANK_MAC "00:00:00:00:00:00"

#define LANHOST_NODE	"LanHost"
#define LANHOST_ENTRY	"LanHost_Entry%d"
#define LEASENUM_ATTR	"LeaseNum"

/*
      This Node: InternetGatewayDevice.LANDevice.{i}.Hosts.{i}
	Parent Node: InternetGatewayDevice.LANDevice.{i}.Hosts.
	Parent Node Table Location: cp_landev_hosts_nodetable.c
*/
extern CwmpParameterStruct HostIndex[];

int8 getHostNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHostNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHostNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8   RebuildHostTable();
int	cwmpInitHostSubTree( DeviceNodePtr deviceNode );
	
int8 getIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getAddressSourceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getAddressSourceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAddressSourceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getLeaseTimeRemainingValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getLeaseTimeRemainingAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLeaseTimeRemainingAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getMACAddress_hostValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getMACAddress_hostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMACAddress_hostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getHostNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHostNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHostNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getInterfaceTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getInterfaceTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInterfaceTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getActive_hostValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getActive_hostAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setActive_hostAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

