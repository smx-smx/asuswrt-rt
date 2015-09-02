#ifndef CP_LAYER3FORWARDING_H_
#define CP_LAYER3FORWARDING_H_
#include "cp_core.h"

#define  ROUTE_NODE "Route"
#define WEB_SET_ROUTE_NODE "WebCurSet_Entry"
#define ROUTE_SUB_NODE "Entry"
#define STATIC_ROUTE_NUM 16
#define PVC_NUMBER 8

typedef struct _Route_Info_List
{
	char flag[5];
	char destip[16];
	char submask[16];
#ifdef TCSUPPORT_LAYER3FORWARD_ENHANCEMENT
	char srcip[16];
	char src_submask[16];
#endif
	char gateway[16];
	char interface[5];	/*pvc0~pvc8(nas or ppp)*/
	char metric[5];		/*1~15*/
	int entryindex;		/*route entry index :1~16*/
	struct _Route_Info_List *next;	/*next point of route entry list*/
}Route_Info_List;


extern CwmpParameterStruct ForwardingIndex[];
int OperateRoute(int index);
void FreeAllRouteMemory();

int	cwmpInitForwardingTableFromFlash(DeviceNodePtr deviceNode);

int8 getDefaultConnectionServiceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDefaultConnectionServiceValue(void* value, struct DeviceNode*  deviceNode);
int8 getDefaultConnectionServiceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDefaultConnectionServiceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getForwardNumberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getForwardNumberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setForwardNumberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 addForwardingObject(uint32* num,  DeviceNode*  deviceNode);
int8 deleteForwardingObject(uint32 num, DeviceNode*  deviceNode);

int8 getForwarding1EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setForwarding1EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getForwarding1EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setForwarding1EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getForwarding1StatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getForwarding1StatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8   setForwarding1StatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getForwarding1TypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setForwarding1TypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getForwarding1TypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setForwarding1TypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getForwarding1DestIPAddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setForwarding1DestIPAddrValue(void* value, struct DeviceNode*  deviceNode);
int8 getForwarding1DestIPAddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setForwarding1DestIPAddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getForwarding1DestSubMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setForwarding1DestSubMaskValue(void* value, struct DeviceNode*  deviceNode);
int8 getForwarding1DestSubMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setForwarding1DestSubMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getForwarding1SrcIPAddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setForwarding1SrcIPAddrValue(void* value, struct DeviceNode*  deviceNode);
int8 getForwarding1SrcIPAddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setForwarding1SrcIPAddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getForwarding1SrcSubMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setForwarding1SrcSubMaskValue(void* value, struct DeviceNode*  deviceNode);
int8 getForwarding1SrcSubMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setForwarding1SrcSubMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getForwarding1GtwIPAddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setForwarding1GtwIPAddrValue(void* value, struct DeviceNode*  deviceNode);
int8 getForwarding1GtwIPAddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setForwarding1GtwIPAddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getForwarding1IfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setForwarding1IfaceValue(void* value, struct DeviceNode*  deviceNode);
int8 getForwarding1IfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setForwarding1IfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getForwarding1FwMetricValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setForwarding1FwMetricValue(void* value, struct DeviceNode*  deviceNode);
int8 getForwarding1FwMetricAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setForwarding1FwMetricAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif
