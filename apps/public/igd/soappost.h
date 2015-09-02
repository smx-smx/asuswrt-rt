#ifndef   _SOAPPOST
#define   _SOAPPOST



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
#define DEBUG 1

int SoapErrorResponse(char * errstr);
int PostCallBack(int sockfd,char *firstline,char *body);
int GetSoapCommand(char *cmd,char *body);
void SendSoapResponse(int sockfd,char *eventcmd,char *servicetype,char *cmd,char *httpversion,char *body);

int GetFirstXmlItemValue(char *value,char *item,char *body);

extern int PharseFirstLine(char *result,char *httpversion,char *firstline);
extern void currentTmToHttpDate(int flag,char *out); 


#endif
