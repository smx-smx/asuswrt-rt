#ifndef CP_SERVICES_VOICEPROF_H_
#define CP_SERVICES_VOICEPROF_H_

#include "cp_core.h"

#define LINENUM					1

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.Line.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
#if !defined(TCSUPPORT_CWMP_CS_BSNL)
extern CwmpParameterStruct LineIndex[];

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.Line.{i}.
	Parent Node Table Location: cp_Services_voiceProf_nodetable.c
*/
extern CwmpParameterStruct Line[];

int8 getLocalPortMinValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLocalPortMinValue(void* value, struct DeviceNode*  deviceNode);
int8 getLocalPortMinAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLocalPortMinAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPProxyServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPProxyServerValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPProxyServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPProxyServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPProxyServerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPProxyServerPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPProxyServerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPProxyServerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPProxyServerTransportValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPProxyServerTransportValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPProxyServerTransportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPProxyServerTransportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPRegistrarServerValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPRegistrarServerValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPRegistrarServerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPRegistrarServerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPRegistrarServerPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSIPRegistrarServerPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getSIPRegistrarServerPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPRegistrarServerPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSIPRegistrarServerTransportValue(char** value, ParameterType* type, DeviceNode*  deviceNode);	
int8 setSIPRegistrarServerTransportValue(void* value, struct DeviceNode*  deviceNode);	
int8 getSIPRegistrarServerTransportAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSIPRegistrarServerTransportAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getOutboundProxyValue(char** value, ParameterType* type, DeviceNode*  deviceNode);		
int8 setOutboundProxyValue(void* value, struct DeviceNode*  deviceNode);
int8 getOutboundProxyAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setOutboundProxyAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getOutboundProxyPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setOutboundProxyPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getOutboundProxyPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setOutboundProxyPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getProfileFaxT38Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setProfileFaxT38Value(void* value, struct DeviceNode*  deviceNode);
int8 getProfileFaxT38Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setProfileFaxT38Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int	cwmpInitLineSubTree( DeviceNodePtr deviceNode );

#endif
#endif

