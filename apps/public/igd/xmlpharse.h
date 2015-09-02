#ifndef  _XMLPHARSE
#define  _XMLPHARSE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <netdb.h>
#include <unistd.h>
#include "deviceinfo.h"
#include <signal.h>
#include <sys/time.h>


#define DEBUG  1


#define  UPNP_INVALID_DEVICE -1;

int  DevCount = 0;


void pharseigdconf(char *igdconfbuff,char * target,char *value);

int pharsexml(FILE*fp,struct device_info *deviceinfo);
char * pharsestr(char *xmlstr,char *target,char *utarget);

int DeviceAdvertisement(int flag,char * DevType, int RootDev,char * Udn, char *Server, char * Location);
void CreateServiceRequestPacket(int Notf,int flag,char *RqstBuf,char * NtSt,char *Usn,char *Server,char * Location);
int NewRequestHandler(struct sockaddr_in * DestAddr, int NumPacket, char ** RqPacket);
void currentTmToHttpDate(int flag ,char *out);

int ServiceAdvertisement(int flag, char * Udn, char * ServType,char *Server,char * Location);
int SendDeviceAdvertisement(char * devtype,char *sertype,char *udn,char * location,int exp);
void tScheduleAdvertiseTimeout(int timeout);
void handler_timer();
extern  int MiniServerInit(int listenfd);

#endif
