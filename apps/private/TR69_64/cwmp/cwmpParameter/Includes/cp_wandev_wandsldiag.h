#ifndef CP_WANDEV_WANDSLDIAG_H_
#define CP_WANDEV_WANDSLDIAG_H_

#include "cp_core.h"

/*----------------------dsl diagnostic relation function begin----------------------*/
#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getWANDSLDiagnositcs_LoopDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLDiagnositcs_LoopDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_LoopDiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_LoopDiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_ACTPSDdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_ACTPSDdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_ACTPSDdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_ACTPSDusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_ACTPSDusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_ACTPSDusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_ACTATPdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_ACTATPdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_ACTATPdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_ACTATPusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_ACTATPusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_ACTATPusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLINSCdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLINSCdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLINSCdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_HLINpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_HLINpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_HLINpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_QLNpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_QLNpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_QLNpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_SNRpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_SNRpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_SNRpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_BITSpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_BITSpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_BITSpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLDiagnositcs_GAINSpsdsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLDiagnositcs_GAINSpsdsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLDiagnositcs_GAINSpsdsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
/*--------------------dsl diagnostic relation function end-----------------------------*/

//add by brian for dsl diagnostic
//static void resetCWMPPingResult(void);
//static void doCwmpIPPingError(void);
int8 NT_ADSL_def_Diagnostics_poll(char *attrName,char *p_diagnostic_set_buf);

#endif
#endif

