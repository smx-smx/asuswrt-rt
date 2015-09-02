#ifndef   _MINISERVER
#define   _MINISERVER

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
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <asm/errno.h>
#include <sys/stat.h>
#include <math.h>
#include <sys/ioctl.h>
#include <net/if.h>
#define DEBUG 1


extern  void currentTmToHttpDate(int flag,char *out);
extern  int PostCallBack(int sockfd,char *firstline,char *body);


int ReadAllPacket(int sockfd,char * vptr,int n);
int CheckInvalidMsearch(char * urn,char * m_ssdp,char * uuid);
int CreateMsearchRespondPacket(char * packet,char * urn,char * uuid);
int MsearchCallBack(int sockfd,struct sockaddr_in *clientaddr,char *firstline,char *body);
int PharseMsearchString(char * body,char *urn,char *m_ssdp);

void  StartUdpCommunication(int sockfd);
//void  StartUdpCommunication(void * args);

void RunMiniServer(int listenfd,int udplistenfd);
int MiniServerInit(int listenfd);

int ProcessHttpCmd(int sockfd,char *cmd,char * firstline,char * body);
int GetCallBack(int sockfd,char *firstline,char * body);
int PharseFirstLine(char *result,char *httpversion,char * firstline);


void GetCmd(char *cmd,char * firstline,char *buffer,char *body,int count);
void HandleRequest(int sockfd);

int SubscribleCallBack(int sockfd,char *firstline,char *body);
void CreateSerialId(char *id);
//krammer change for bug 857
#ifdef TRENDCHIP
void SendSubscribleNotify(int port,char *subcmd,char *serverIp,char *sid,char *httpversion, char *call_back_path);
int CreateNotifyPacket(char *packet,char *cmd,char *id,char *hostip,int port,char *httpversion, char *call_back_path);
#else
void SendSubscribleNotify(int port,char *subcmd,char *serverIp,char *sid,char *httpversion);
int CreateNotifyPacket(char *packet,char *cmd,char *id,char *hostip,int port,char *httpversion);
#endif

int CreatePropertySet(char *cmd,char *propset);
void PropertySet(char *propset,char *item,char *value);
void SendSubscribleHttpOk(int sockfd,char *sid,char *httpversion);

int UnsubscribleCallBack(int sockfd,char *firstline,char *body);



#endif






