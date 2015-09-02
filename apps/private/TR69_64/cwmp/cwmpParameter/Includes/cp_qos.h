#ifndef CP_QOS_H_
#define CP_QOS_H_

#include "cp_core.h"

#define QOS_NODENAME 	"QoS"
#define QOS_COMMON 	"QoS_Common"
#define QOS_ENTRY 		"QoS_Entry%d"


#if !defined(TCSUPPORT_CWMP_CS_BSNL)
extern CwmpParameterStruct QueueManagement[];

/*Parameters for Queue*/
extern CwmpParameterStruct Queue[];
extern CwmpParameterStruct QueueIndex[];

/*Parameters for Classification*/
extern CwmpParameterStruct Classification[];
extern CwmpParameterStruct ClassificationIndex[];

/*Parameters for Policer*/
extern CwmpParameterStruct Policer[];
extern CwmpParameterStruct PolicerIndex[];


/*Funcs for Queue*/
int	cwmpInitQoSQueueSubTree( DeviceNodePtr deviceNode );

#ifdef TCSUPPORT_QOS_QUEUE_MODIFY
/* add support add/delete Queue object */
int8 addQoSQueueObject(uint32* num, DeviceNode*  deviceNode);
int8 deleteQoSQueueObject(uint32 num, DeviceNode*  deviceNode);
#endif
#endif


#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getQoSQueueInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getQoSQueueInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQoSQueueInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQoSDropAlgorithmValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getQoSDropAlgorithmAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQoSDropAlgorithmAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQoSSchedulerAlgorithmValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getQoSSchedulerAlgorithmAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQoSSchedulerAlgorithmAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQoSQueueEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
#ifdef TCSUPPORT_QOS_QUEUE_MODIFY
int8 setQoSQueueEnableValue(void* value, struct DeviceNode*  deviceNode);
#endif
int8 getQoSQueueEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQoSQueueEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*Funcs  for Classification*/
int	cwmpInitQoSClassificationSubTree( DeviceNodePtr deviceNode );

int8 addClassificationObject(uint32* num, DeviceNode*  deviceNode);
int8 deleteQoSRuleObject(uint32 num, DeviceNode*  deviceNode);

int8 getQoSProtocolValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQoSProtocolValue(void* value, struct DeviceNode*  deviceNode);
int8 getQoSProtocolAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQoSProtocolAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQoSDSCPMarkValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQoSDSCPMarkValue(void* value, struct DeviceNode*  deviceNode);
int8 getQoSDSCPMarkAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQoSDSCPMarkAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQoSForwardingPolicyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQoSForwardingPolicyValue(void* value, struct DeviceNode*  deviceNode);
int8 getQoSForwardingPolicyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQoSForwardingPolicyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQoSClassQueueValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQoSClassQueueValue(void* value, struct DeviceNode*  deviceNode);
int8 getQoSClassQueueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQoSClassQueueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQoSClassPolicerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQoSClassPolicerValue(void* value, struct DeviceNode*  deviceNode);
int8 getQoSClassPolicerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQoSClassPolicerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQoSClassificationEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQoSClassificationEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getQoSClassificationEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQoSClassificationEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQosDestIPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQosDestIPValue(void* value, struct DeviceNode*  deviceNode);
int8 getQosDestIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQosDestIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQosDestMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQosDestMaskValue(void* value, struct DeviceNode*  deviceNode);
int8 getQosDestMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQosDestMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQosSourceIPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQosSourceIPValue(void* value, struct DeviceNode*  deviceNode);
int8 getQosSourceIPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQosSourceIPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQosSourceMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQosSourceMaskValue(void* value, struct DeviceNode*  deviceNode);
int8 getQosSourceMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQosSourceMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQosDestPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQosDestPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getQosDestPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQosDestPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQosSourcePortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQosSourcePortValue(void* value, struct DeviceNode*  deviceNode);
int8 getQosSourcePortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQosSourcePortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQosSourceMACAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQosSourceMACAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getQosSourceMACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQosSourceMACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getQosDestMACAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQosDestMACAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getQosDestMACAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQosDestMACAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);


/*Funcs  for Policer*/
int	cwmpInitQoSPolicerSubTree( DeviceNodePtr deviceNode );

int8 getQoSPolicerEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setQoSPolicerEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getQoSPolicerEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setQoSPolicerEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif
#endif
