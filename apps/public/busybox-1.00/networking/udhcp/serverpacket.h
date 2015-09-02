#ifndef _SERVERPACKET_H
#define _SERVERPACKET_H

#include "packet.h"

#if defined (CWMP) && defined(TR111)
#include "dhcpd.h"
//#include <fcntl.h>
#include <unistd.h>
#include "../../../lib/libtcapi.h"
#endif

int sendOffer(struct dhcpMessage *oldpacket);
int sendNAK(struct dhcpMessage *oldpacket);
int sendACK(struct dhcpMessage *oldpacket, uint32_t yiaddr);
int send_inform(struct dhcpMessage *oldpacket);

#if defined (CWMP) && defined(TR111)
/* where to write the devices info file */
#define DEVICE_INFO_FILE         "/etc/devices.conf"
#define DEVICE_HOSTINDEX_INFO_FILE         "/etc/device_hostindex.conf"
//#define MAX_BUF_SIZE 2048
#define ATTRIBUTE_ACTIVE 0x20

int check_vi(struct dhcpMessage *packet);
void parseDev(struct dhcpMessage *packet, mDev_t *dev);
void insertDevice(struct dhcpMessage *packet);
#if 0
int findDevice(uint8_t clientID[]);
#else
//int findDevice(mDev_t* pdev);
int findDevice(struct dhcpMessage *packet,mDev_t* pdev);
#endif
void deleteDevice(struct dhcpMessage *packet);
void deleteDeviceByMac(uint8_t *macAddr);
unsigned long getDeviceNum(void);
void writeDevicesFile(void);
void writeHostIndexFile(void);
void cwmpShowmDev(void);
void dhcpRefresh(void);
#endif
#endif
