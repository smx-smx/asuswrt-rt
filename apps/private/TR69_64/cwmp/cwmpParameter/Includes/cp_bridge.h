#ifndef CP_BRIDGE_H_
#define CP_BRIDGE_H_

#include "cp_core.h"

#if defined(TCSUPPORT_LAYER2BRIDGING)
#define MAX_BRIDGE_NUM      16
#define MAX_FILTER_NUM       32
#define MAX_MARKING_NUM  32
#define MAX_INTEFACE_NUM  32
#define SP_VLAN_MAX_PORTS 32
#define BRIDGEIDLEFLAG         1
#define FILTERIDLEFLAG          2
#define MARKINGIDLEFLAG      3

#define WANINTERFACE_PVC_0			"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1"
#define WANINTERFACE_PVC_1			"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.2"
#define WANINTERFACE_PVC_2			"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.3"
#define WANINTERFACE_PVC_3			"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.4"
#define WANINTERFACE_PVC_4			"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.5"
#define WANINTERFACE_PVC_5			"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.6"
#define WANINTERFACE_PVC_6			"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.7"
#define WANINTERFACE_PVC_7			"InternetGatewayDevice.WANDevice.1.WANConnectionDevice.8"
#define LANINTERFACE_ETHERNET_1		"InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.1"
#define LANINTERFACE_ETHERNET_2		"InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.2"
#define LANINTERFACE_ETHERNET_3		"InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.3"
#define LANINTERFACE_ETHERNET_4		"InternetGatewayDevice.LANDevice.1.LANEthernetInterfaceConfig.4"
#define LANINTERFACE_USB				"InternetGatewayDevice.LANDevice.1.LANUSBInterfaceConfig.1"
#define LANINTERFACE_WLAN_0			"InternetGatewayDevice.LANDevice.1.WLANConfiguration.1"
#define LANINTERFACE_WLAN_1			"InternetGatewayDevice.LANDevice.1.WLANConfiguration.2"
#define LANINTERFACE_WLAN_2			"InternetGatewayDevice.LANDevice.1.WLANConfiguration.3"
#define LANINTERFACE_WLAN_3			"InternetGatewayDevice.LANDevice.1.WLANConfiguration.4"


#if !defined(TCSUPPORT_CWMP_CS_BSNL)
typedef struct
{
	uint32 bitmap;
	char *name;
	int16 value;
	char *interfacename;
}portInterfaceConvert_t;

typedef struct bridgeInfo
{
	uint8 bridgeID;
	char name[64];
	UINT vlanID;
}bridgeInfo;

typedef struct filterInfo
{
	uint8 filterID;
	int groupID;
	uint8 portID;
	UINT order;
}filterInfo;
typedef struct markingInfo
{
	uint8 MarkingID;
	int groupID;
	uint8 portID;
	boolean IDtag;
}markingInfo;

extern CwmpParameterStruct  BridgeIndex[];
extern CwmpParameterStruct AvailableInterfaceIndex[];
extern CwmpParameterStruct  FilterIndex[];
extern CwmpParameterStruct MarkingIndex[];
extern CwmpParameterStruct Bridge[];
extern CwmpParameterStruct AvailableInterface[];
extern CwmpParameterStruct Filter[];
extern CwmpParameterStruct Marking[];

int VlanGetPortValue(char *portstr,int flags);
char* VlanGetinterfaceName(uint32 portid);
int VlanGetinterfaceInfo(uint32 portid,int flags);
int StrConvertBit(char *portname);
 uint32 cwmpReturnInstance(uint8 flag);
 
int cwmpBridgeInfoInit(DeviceNode *deviceNode);
int cwmpFilterInfoInit(DeviceNode *deviceNode);
int cwmpMarkingInfoInit(DeviceNode *deviceNode);
void resetMarkingInfo(uint32 num);
void resetFilterInfo(uint32 num);

int8 addBridgeObject(uint32 *num, DeviceNode* deviceNode);
int8 delBridgeObject(uint32 num, DeviceNode*  deviceNode);
int cwmpInitBridgeSubTree(DeviceNodePtr deviceNode);

int cwmpInitAvailableInterfaceSubTree(DeviceNodePtr deviceNode);

int8 addFilterObject(uint32 *num, DeviceNode* deviceNode);
int8 delFilterObject(uint32 num, DeviceNode*  deviceNode);
int cwmpInitFilterTableFromFlash(DeviceNodePtr deviceNode);

 int cwmpInitMarkingTableFromFlash(DeviceNodePtr deviceNode);
int8 addMarkingObject(uint32 *num, DeviceNode* deviceNode);
int8 delMarkingObject(uint32 num, DeviceNode*  deviceNode);

int8 getBridgeEnableValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 setBridgeEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getBridgeEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setBridgeEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFilterBridgeReferenceValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 setFilterBridgeReferenceValue(void* value, struct DeviceNode*  deviceNode);
int8 getFilterBridgeReferenceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFilterBridgeReferenceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getFilterInterfaceValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 setFilterInterfaceValue(void* value, struct DeviceNode*  deviceNode);
int8 getFilterInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setFilterInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getBridgeNameValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 setBridgeNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getBridgeNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setBridgeNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getBridgeKeyValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 getBridgeKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setBridgeKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getVLANIDValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 setVLANIDValue(void* value, struct DeviceNode*  deviceNode);
int8 getVLANIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVLANIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAvailableInterfaceKeyValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 getAvailableInterfaceKeyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAvailableInterfaceKeyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getAvailableInterfaceTypeValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 getAvailableInterfaceTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAvailableInterfaceTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getInterfaceReferenceValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 getInterfaceReferenceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setInterfaceReferenceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getExclusivityOrderValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 setExclusivityOrderValue(void* value, struct DeviceNode*  deviceNode);
int8 getExclusivityOrderAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setExclusivityOrderAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMarkingBridgeReferenceValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 setMarkingBridgeReferenceValue(void* value, struct DeviceNode*  deviceNode);
int8 getMarkingBridgeReferenceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMarkingBridgeReferenceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMarkingInterfaceValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 setMarkingInterfaceValue(void* value, struct DeviceNode*  deviceNode);
int8 getMarkingInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMarkingInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getVLANIDUntagValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 setVLANIDUntagValue(void* value, struct DeviceNode*  deviceNode);
int8 getVLANIDUntagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVLANIDUntagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif
#endif
#endif

