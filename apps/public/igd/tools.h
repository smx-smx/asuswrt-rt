#ifndef  _TOOLS
#define  _TOOLS

//#define   WAN_IFNAME    "ppp0"
//#define   LAN_IFNAME    "eth0"
#define   XML_DEVICEDESC_FILE   "gatedesc.xml"
#define   SSDP_IP                "239.255.255.250"
#define   SOAP_PORT              5555

#define   PACKETSSENT         0
#define   PACKETSRECV         1
#define   BYTESSENT           2
#define   BYTESRECV           3


int getsockfd();
int getIpAddrFromIfname(char * ifname,char * ipaddr);

char * getNetIfState(char * ifname);
unsigned long GetTotalCommunicate(int flag);

#endif
