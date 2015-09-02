#ifndef CP_WANDEV_WANCONNDEV_DSLLINKCFG_H_
#define CP_WANDEV_WANCONNDEV_DSLLINKCFG_H_
#include "cp_core.h"

extern uint16 ACSaddingPVCFlag;

#if !defined(TCSUPPORT_CWMP_CS_BSNL)
/*rpc of tr069 parameters: InternetGatewayDevice.WANDevice.1.WANConnectionDevice.{i}.WANDSLLinkConfig.*/
int8 getWANDSLLinkCfg1EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1LinkStatusValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1LinkStatusAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1LinkStatusAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1LinkTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1LinkTypeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1LinkTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1LinkTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1AutoConfigValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1AutoConfigAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1AutoConfigAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ModulationTypeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ModulationTypeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ModulationTypeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1DestinationAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1DestinationAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1DestinationAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1DestinationAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMEncapsulationValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1ATMEncapsulationValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMEncapsulationAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMEncapsulationAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1FCSPreservedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1FCSPreservedValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1FCSPreservedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1FCSPreservedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1VCSearchListValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1VCSearchListValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1VCSearchListAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1VCSearchListAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMTransmittedBlocksValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMTransmittedBlocksAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMTransmittedBlocksAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMReceivedBlocksValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMReceivedBlocksAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMReceivedBlocksAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMQoSValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1ATMQoSValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMQoSAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMQoSAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMPeakCellRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1ATMPeakCellRateValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMPeakCellRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMPeakCellRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMMaximumBurstSizeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1ATMMaximumBurstSizeValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMMaximumBurstSizeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMMaximumBurstSizeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMSustainableCellRateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setWANDSLLinkCfg1ATMSustainableCellRateValue(void* value, struct DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMSustainableCellRateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMSustainableCellRateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1AAL5CRCErrorsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1AAL5CRCErrorsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1AAL5CRCErrorsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMCRCErrorsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMCRCErrorsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMCRCErrorsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getWANDSLLinkCfg1ATMHECErrorsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getWANDSLLinkCfg1ATMHECErrorsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setWANDSLLinkCfg1ATMHECErrorsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
#endif

#endif
