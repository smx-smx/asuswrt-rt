#ifndef CP_SERVICES_H_
#define CP_SERVICES_H_

#include "cp_core.h"

#if !defined(TCSUPPORT_CWMP_CS_BSNL)
#ifdef VOIP
#define VOICESERVICESNUM		1
#define VOICEPROFILENUM		1
#endif

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceService
	Parent Node Table Location: cp_Services_nodetable.c
*/
extern CwmpParameterStruct Capabilities[];

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}
	Parent Node Table Location: cp_Services_nodetable.c
*/
extern CwmpParameterStruct VoiceProfile[];

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.
	Parent Node Table Location: cp_Services_nodetable.c
*/
extern CwmpParameterStruct VoiceServiceIndex[];

/*
	Parent Node: InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.
	Parent Node Table Location: cp_Services_nodetable.c
*/
extern CwmpParameterStruct VoiceProfileIndex[];

int8 getVoiceProfileNmberOfEntriesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getVoiceProfileNmberOfEntriesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVoiceProfileNmberOfEntriesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int	cwmpInitVoiceServiceSubTree( DeviceNodePtr deviceNode );
int	cwmpInitVoiceProfileSubTree( DeviceNodePtr deviceNode );

#endif
#endif
