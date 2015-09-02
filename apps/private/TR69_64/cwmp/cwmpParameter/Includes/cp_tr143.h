#ifndef _CP_TR143_H
#define _CP_TR143_H

#include "cp_core.h"

/*******************  InternetGatewayDevice.Capabilities. start ************************/
int8 getDiagDldTsportsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDiagDldTsportsValue(void* value, struct DeviceNode*  deviceNode);
int8 getDiagDldTsportsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDiagDldTsportsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDiagUpTsportsValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDiagUpTsportsValue(void* value, struct DeviceNode*  deviceNode);
int8 getDiagUpTsportsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDiagUpTsportsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*******************  InternetGatewayDevice.DownloadDiagnostics. start **********************/
int8 resetCwmpDownloadDiagnostics(void);
char *convertSysTimeToString(struct timeval tv, char *timeStr, int len);

int8 getDldDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDldDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getDldDiagnosticsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDldDiagnosticsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDldInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDldInterfaceValue(void* value, struct DeviceNode*  deviceNode);
int8 getDldInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDldInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDldDownloadURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDldDownloadURLValue(void* value, struct DeviceNode*  deviceNode);
int8 getDldDownloadURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDldDownloadURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDldDSCPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDldDSCPValue(void* value, struct DeviceNode*  deviceNode);
int8 getDldDSCPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDldDSCPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDldEthernetPriorityValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDldEthernetPriorityValue(void* value, struct DeviceNode*  deviceNode);
int8 getDldEthernetPriorityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDldEthernetPriorityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDldROMTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDldROMTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDldROMTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDldBOMTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDldBOMTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDldBOMTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDldEOMTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDldEOMTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDldEOMTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDldTestBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDldTestBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDldTestBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDldTotalBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDldTotalBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDldTotalBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDldTCPOpenRequestTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDldTCPOpenRequestTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDldTCPOpenRequestTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDldTCPOpenResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDldTCPOpenResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDldTCPOpenResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*******************  InternetGatewayDevice.UploadDiagnostics. start **********************/
int8 resetCwmpUploadDiagnostics(void);

int8 getUldDiagnosticsStateValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUldDiagnosticsStateValue(void* value, struct DeviceNode*  deviceNode);
int8 getUldDiagnosticsAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUldDiagnosticsAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUldInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUldInterfaceValue(void* value, struct DeviceNode*  deviceNode);
int8 getUldInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUldInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUldUploadURLValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUldUploadURLValue(void* value, struct DeviceNode*  deviceNode);
int8 getUldUploadURLAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUldUploadURLAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUldDSCPValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUldDSCPValue(void* value, struct DeviceNode*  deviceNode);
int8 getUldDSCPAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUldDSCPAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUldEthernetPriorityValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUldEthernetPriorityValue(void* value, struct DeviceNode*  deviceNode);
int8 getUldEthernetPriorityAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUldEthernetPriorityAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUldTestFileLengthValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUldTestFileLengthValue(void* value, struct DeviceNode*  deviceNode);
int8 getUldTestFileLengthAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUldTestFileLengthAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUldROMTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUldROMTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUldROMTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUldBOMTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUldBOMTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUldBOMTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUldEOMTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUldEOMTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUldEOMTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUldTotalBytesSentValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUldTotalBytesSentAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUldTotalBytesSentAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUldTCPOpenRequestTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUldTCPOpenRequestTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUldTCPOpenRequestTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUldTCPOpenResponseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUldTCPOpenResponseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUldTCPOpenResponseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*******************  InternetGatewayDevice.UDPEchoConfig. start **********************/
#define TR143UDPEcho    "Cwmp_tr143UDPEcho"

/*
the attribute of this node is :
Enable			  		{Yes,No}
Interface		  		{""}
SourceIPAddress   		{"192.168.1.10"}
UDPPort           		{"7"}
EchoPlusEnabled   		{Yes,No}
EchoPlusSupported 		{Yes}
*/

int8 getUDPEchoEnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUDPEchoEnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getUDPEchoEnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUDPEchoEnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUDPEchoInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUDPEchoInterfaceValue(void* value, struct DeviceNode*  deviceNode);
int8 getUDPEchoInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUDPEchoInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUDPEchoSourceIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUDPEchoSourceIPAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getUDPEchoSourceIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUDPEchoSourceIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUDPEchoUDPPortValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUDPEchoUDPPortValue(void* value, struct DeviceNode*  deviceNode);
int8 getUDPEchoUDPPortAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUDPEchoUDPPortAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUDPEchoEchoPlusEnabledValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUDPEchoEchoPlusEnabledValue(void* value, struct DeviceNode*  deviceNode);
int8 getUDPEchoEchoPlusEnabledAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUDPEchoEchoPlusEnabledAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUDPEchoPlusSupportedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUDPEchoPlusSupportedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUDPEchoPlusSupportedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUDPEchoPacketsReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUDPEchoPacketsReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUDPEchoPacketsReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUDPEchoPacketsRespondedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUDPEchoPacketsRespondedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUDPEchoPacketsRespondedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUDPEchoBytesReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUDPEchoBytesReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUDPEchoBytesReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUDPEchoBytesRespondedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getUDPEchoBytesRespondedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUDPEchoBytesRespondedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTimeFirstPacketReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTimeFirstPacketReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimeFirstPacketReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTimeLastPacketReceivedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTimeLastPacketReceivedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimeLastPacketReceivedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

