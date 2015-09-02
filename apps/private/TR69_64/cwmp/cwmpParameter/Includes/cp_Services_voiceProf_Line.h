#ifndef CP_SERVICES_VOICEPROF_LINE_H_
#define CP_SERVICES_VOICEPROF_LINE_H_

#include "cp_core.h"

#if !defined(TCSUPPORT_CWMP_CS_BSNL)
#ifdef VOIP
#define CODECLIST_NUMBER 8
#define CODECENTRY_NUMBER 4
#endif

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.VoiceProfile.Line.{i}.Codec.List.
	Parent Node Table Location: cp_Services_voiceProf_Line_nodetable.c
*/
extern CwmpParameterStruct ListIndex[];

int	cwmpInitLineCodecListSubTree( DeviceNodePtr deviceNode );

int8 getLineEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLineEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getLineEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineSIPAuthUserNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLineSIPAuthUserNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getLineSIPAuthUserNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineSIPAuthUserNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getLineSIPAuthPasswordValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLineSIPAuthPasswordValue(void* value, struct DeviceNode*  deviceNode);
int8 getLineSIPAuthPasswordAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLineSIPAuthPasswordAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getEchoCancellationEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setEchoCancellationEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getEchoCancellationEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setEchoCancellationEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getCodecValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getCodecAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setCodecAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPacketizationPeriodValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPacketizationPeriodValue(void* value, struct DeviceNode*  deviceNode);
int8 getPacketizationPeriodAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPacketizationPeriodAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getPriorityValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPriorityValue(void* value, struct DeviceNode*  deviceNode);
int8 getPriorityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPriorityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

#endif



