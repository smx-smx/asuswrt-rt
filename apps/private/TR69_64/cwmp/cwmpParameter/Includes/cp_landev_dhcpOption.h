#ifndef CP_LANDEV_DHCPOPTION_H_
#define CP_LANDEV_DHCPOPTION_H_

#include "cp_core.h"

#define DHCPD_NODENAME "Dhcpd"
#define DHCPD_OPT60_NODENAME "Dhcpd_Opt60"//for delay commit
#define DHCPD_OPT60 "Dhcpd_Option60"
#define DHCPD_OPT240 "Dhcpd_Option240"

#define VENDORCLASSID_MAX		64
#define CLIENTID_MAX			65
#define USERCLASSID_MAX		65
#define CHADDR_MAX				6
#define CHADDRMASK_MAX		6
#define DOMAINNAME_MAX		39

#define VENDORCLASSIDMODE_EXACT (0x01<<0)
#define VENDORCLASSIDMODE_PREFIX (0x01<<1)
#define VENDORCLASSIDMODE_SUFFIX (0x01<<2)
#define VENDORCLASSIDMODE_SUBSTRING (0x01<<3)
#define USEALLOCATEDWAN_NORMAL (0x01<<0)
#define USEALLOCATEDWAN_PASSTHROUGH (0x01<<1)

#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getdhcpOption_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setdhcpOption_EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getdhcpOption_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setdhcpOption_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getdhcpOption_TagValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setdhcpOption_TagValue(void* value, struct DeviceNode*  deviceNode);
int8 getdhcpOption_TagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setdhcpOption_TagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getdhcpOption_ValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setdhcpOption_ValueValue(void* value, struct DeviceNode*  deviceNode);
int8 getdhcpOption_ValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setdhcpOption_ValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getdhcpConSer_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setdhcpConSer_EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getdhcpConSer_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setdhcpConSer_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getPoolOrderValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setPoolOrderValue(void* value, struct DeviceNode*  deviceNode);
int8 getPoolOrderAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setPoolOrderAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getSourceInterfaceValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setSourceInterfaceValue(void* value, struct DeviceNode*  deviceNode);
int8 getSourceInterfaceAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setSourceInterfaceAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getVendorClassIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setVendorClassIDValue(void* value, struct DeviceNode*  deviceNode);
int8 getVendorClassIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVendorClassIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getVendorClassIDExcludeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setVendorClassIDExcludeValue(void* value, struct DeviceNode*  deviceNode);
int8 getVendorClassIDExcludeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVendorClassIDExcludeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getVendorClassIDModeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setVendorClassIDModeValue(void* value, struct DeviceNode*  deviceNode);
int8 getVendorClassIDModeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setVendorClassIDModeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getClientIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setClientIDValue(void* value, struct DeviceNode*  deviceNode);
int8 getClientIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setClientIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getClientIDExcludeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setClientIDExcludeValue(void* value, struct DeviceNode*  deviceNode);
int8 getClientIDExcludeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setClientIDExcludeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getUserClassIDValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUserClassIDValue(void* value, struct DeviceNode*  deviceNode);
int8 getUserClassIDAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUserClassIDAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getUserClassIDExcludeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUserClassIDExcludeValue(void* value, struct DeviceNode*  deviceNode);
int8 getUserClassIDExcludeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUserClassIDExcludeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getChaddrValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setChaddrValue(void* value, struct DeviceNode*  deviceNode);
int8 getChaddrAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setChaddrAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getChaddrMaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setChaddrMaskValue(void* value, struct DeviceNode*  deviceNode);
int8 getChaddrMaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setChaddrMaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getChaddrExcludeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setChaddrExcludeValue(void* value, struct DeviceNode*  deviceNode);
int8 getChaddrExcludeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setChaddrExcludeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getLocallyServedValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setLocallyServedValue(void* value, struct DeviceNode*  deviceNode);
int8 getLocallyServedAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setLocallyServedAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getMinAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMinAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getMinAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMinAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getMaxAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setMaxAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getMaxAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setMaxAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getReservedAddressesValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setReservedAddressesValue(void* value, struct DeviceNode*  deviceNode);
int8 getReservedAddressesAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setReservedAddressesAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getdhcpConSer_MaskValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setdhcpConSer_MaskValue(void* value, struct DeviceNode*  deviceNode);
int8 getdhcpConSer_MaskAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setdhcpConSer_MaskAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getdhcpConSer_DNSValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setdhcpConSer_DNSValue(void* value, struct DeviceNode*  deviceNode);
int8 getdhcpConSer_DNSAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setdhcpConSer_DNSAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getClientIDExcludeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setClientIDExcludeValue(void* value, struct DeviceNode*  deviceNode);
int8 getClientIDExcludeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setClientIDExcludeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getDomainNameValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDomainNameValue(void* value, struct DeviceNode*  deviceNode);
int8 getDomainNameAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDomainNameAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getIPRoutersValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setIPRoutersValue(void* value, struct DeviceNode*  deviceNode);
int8 getIPRoutersAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setIPRoutersAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getDHCPLeaseTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPLeaseTimeValue(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPLeaseTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPLeaseTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getUseAllocatedWANValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setUseAllocatedWANValue(void* value, struct DeviceNode*  deviceNode);
int8 getUseAllocatedWANAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setUseAllocatedWANAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
		
int8 getAssociatedConnectionValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setAssociatedConnectionValue(void* value, struct DeviceNode*  deviceNode);
int8 getAssociatedConnectionAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setAssociatedConnectionAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getDHCPServerIPAddressValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setDHCPServerIPAddressValue(void* value, struct DeviceNode*  deviceNode);
int8 getDHCPServerIPAddressAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPServerIPAddressAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getDHCPStaticAddressNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getDHCPStaticAddressNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setDHCPStaticAddressNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getdhcpConSer_OptionNumValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getdhcpConSer_OptionNumAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setdhcpConSer_OptionNumAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

/*
      This Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.DHCPOption.{i}.
	Parent Node: InternetGatewayDevice.LANDevice.{i}.LANHostConfigManagement.DHCPConditionalServingPool.{i}.DHCPOption.
	Parent Node Table Location: cp_landev_dhcpConSer_nodetable.c
*/
extern CwmpParameterStruct DHCPConSerOptionIndex[];

int	cwmpInitDHCPConSerOptionSubTree( DeviceNodePtr deviceNode );

int8 getconSerOption_EnableValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setconSerOption_EnableValue(void* value, struct DeviceNode*  deviceNode);
int8 getconSerOption_EnableAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setconSerOption_EnableAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getconSerOption_TagValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setconSerOption_TagValue(void* value, struct DeviceNode*  deviceNode);
int8 getconSerOption_TagAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setconSerOption_TagAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
int8 getconSerOption_ValueValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setconSerOption_ValueValue(void* value, struct DeviceNode*  deviceNode);
int8 getconSerOption_ValueAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setconSerOption_ValueAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);
	
void base64Encode(uint8 *src, uint8 *dest, int srcLen, int *destLen);
void decodeBase64(char *Data);
int isValidMacAddr(char* mac);
int isValidMacAddrMask(char* mask);
int  isValidIPAddr(char *ipstr);
int BeLANIP(char* RouteGateway);
#endif	
#endif

