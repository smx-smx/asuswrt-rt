#ifndef _CP_DEVICEINFO_H
#define _CP_DEVICEINFO_H

#include "cp_core.h"

#define MAX_TR69_PARAM

extern CwmpParameterStruct VendorConfigFileIndex[];

int8 getManufacturerValue(char** value, ParameterType* type,  DeviceNode*  deviceNode);
int8 getManufacturerAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setManufacturerAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getManufacturerOUIValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getManufacturerOUIAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setManufacturerOUIAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getModelNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getModelNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setModelNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getDescriptionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDescriptionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDescriptionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getProductClassValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getProductClassAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setProductClassAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);
#endif

int8 getSerialNumberValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSerialNumberAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSerialNumberAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getHardwareVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getHardwareVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setHardwareVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getSoftwareVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSoftwareVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSoftwareVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getSpecVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getSpecVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSpecVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getProvisioningCodeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setProvisioningCodeValue(void* value, struct DeviceNode*  deviceNode);
int8 getProvisioningCodeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setProvisioningCodeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUpTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUpTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUpTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDeviceLogValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDeviceLogAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDeviceLogAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

/*Add for VendorConfigFile TR069 Nodes*/
#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getVendorCfgFileNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getVendorCfgFileNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVendorCfgFileNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int  cwmpInitVendorCfgFileSubTree( DeviceNodePtr deviceNode );
int8 getVendorCfgFileNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getVendorCfgFileNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVendorCfgFileNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getVendorCfgFileVersionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getVendorCfgFileVersionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVendorCfgFileVersionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getVendorCfgFileDateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getVendorCfgFileDateAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVendorCfgFileDateAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);

int8 getVendorCfgFileDescripValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getVendorCfgFileDescripAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVendorCfgFileDescripAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, DeviceNode* deviceNode);
#endif
#endif

