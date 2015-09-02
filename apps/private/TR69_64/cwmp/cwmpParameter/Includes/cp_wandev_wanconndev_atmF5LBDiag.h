#ifndef CP_WANDEV_WANCONNDEV_ATMF5LBDIAG_H_
#define CP_WANDEV_WANCONNDEV_ATMF5LBDIAG_H_
#include "cp_core.h"


#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getWANATMF5LoopbackDiagnostics_DiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANATMF5LoopbackDiagnostics_DiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_DiagnosticsStateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_DiagnosticsStateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANATMF5LoopbackDiagnostics_NumberOfRepetitionsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANATMF5LoopbackDiagnostics_NumberOfRepetitionsValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_NumberOfRepetitionsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_NumberOfRepetitionsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANATMF5LoopbackDiagnostics_TimeoutValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANATMF5LoopbackDiagnostics_TimeoutValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_TimeoutAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_TimeoutAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANATMF5LoopbackDiagnostics_SuccessCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_SuccessCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_SuccessCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANATMF5LoopbackDiagnostics_FailureCountValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_FailureCountAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_FailureCountAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANATMF5LoopbackDiagnostics_AverageResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_AverageResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_AverageResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANATMF5LoopbackDiagnostics_MinimumResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_MinimumResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_MinimumResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANATMF5LoopbackDiagnostics_MaximumResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANATMF5LoopbackDiagnostics_MaximumResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANATMF5LoopbackDiagnostics_MaximumResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

#endif

