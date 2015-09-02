#ifndef CP_TIME_H_
#define CP_TIME_H_

#include "cp_core.h"

extern CwmpParameterStruct Time[];

#define  NTP_TIME        	      "0"             /* NTP Server Automatically*/
#define  COMPUTER_TIME        "1"             /* PC's Clock*/
#define  MANUAL_TIME            "2"            /* Manually*/


#if !defined(TCSUPPORT_CWMP_CS_BSNL)
int8 getTimeNTPServer1Value(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTimeNTPServer1Value(void* value, struct DeviceNode*  deviceNode);
int8 getTimeNTPServer1Attribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimeNTPServer1Attribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTimeCurrentLocalTimeValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 getTimeCurrentLocalTimeAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimeCurrentLocalTimeAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

int8 getTimeLocalTimeZoneValue(char** value, ParameterType* type, DeviceNode*  deviceNode);
int8 setTimeLocalTimeZoneValue(void* value, struct DeviceNode*  deviceNode);
int8 getTimeLocalTimeZoneAttribute(uint8* notification, AccessEntry** acl, DeviceNode* deviceNode);
int8 setTimeLocalTimeZoneAttribute(uint8 notificationChange, uint8 notification, uint8 accessListChange, AccessEntry* acl, struct DeviceNode*  deviceNode);

#endif

#endif
