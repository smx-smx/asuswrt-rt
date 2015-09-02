/////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////

#include  "deviceinfo.h"
#include  "event.h"
#include  "soappost.h"

#include  "tools.h"
#include  "descfile.h"
#include "../lib/libtcapi.h"

#define TITLE               "# externalport:protocol:internalport:internalclient:enabled:description:leaseduration\n"
int PortMapInit();
int FindPortMapList(char *proto,int ExtPort);
int DeletePortMapList(char *proto,int ExtPort);
int AddPortMapList(char *RmtHostIp,int ExtPort,char *Proto,int IntPort,char * IntClientIp,int Enabled,char * PortMapDes,int LeaseDura);


//add by liuchong to read the port map config file;

int PortMapRead(FILE *fp,char *ExtPort,char *Protocol,char *IntPort,char *IntClient,char *Enabled,char *Desc,char *LeaseDur);

int SavePortMapList();

struct mPortMapList {
        char    RemoteHostIp[20];
        int     ExternalPort;
        char    Protocol[10];
        int     InternalPort;
        char    InternalClientIp[20];
        int     Enabled;
        char    PortMapDescription[64];
        int     LeaseDuration;
        struct  mPortMapList * Previous;
        struct  mPortMapList * Next;
} *PortMapListPtr;



static unsigned long gConnectionStartTime = 0;
#define   soapstring      "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body>\n"

extern struct device_info * deviceinfo[10];
extern char   LAN_IFNAME[16];
extern char   WAN_IFNAME[16];

extern char   nat[16];

extern char *EventUrl[];
extern int DevCount;
extern struct Subscrible_Event * SubEvent;

int   PortMapNumOfEntries = 0;

char *GateServiceType[] = {"urn:schemas-microsoft-com:service:OSInfo:1"
                          ,"urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1"
                          ,"urn:schemas-upnp-org:service:WANIPConnection:1"};


char * GateSoapUrl[] = {"upnp/control/OSInfo1",
			"upnp/control/WANCommonIFC1",
			"upnp/control/WANIPConn1"};


extern int SubscribleEventListFind(char *ip,char *subcmd);

void UpnpCreatePropertySet(char *packet,struct Thread_Event * event);
int GetIpaddressFromSoap(char *ip,char *body);
int UpnpNotifyAllExt(char *eventcmd,char *item,char *value);
void UpnpNotifyThread(struct Thread_Event * event);


void PortMapDelete(char *proto,int extport,char *wanip);
int addPacketFilter(char * Proto, char *SrcIP, char *DestIP,int DestPort,int Enabled, char *Desc);
int delPortForward(char * Proto, char *ExtIP, int ExtPort, char* IntIP, int IntPort);
int delPacketFilter(char * Proto, char *SrcIP, char *DestIP, int DestPort);
int addPortForward(char * Proto, char *ExtIP, int ExtPort, char *IntIP,int IntPort, int Enabled, char *Desc);
int delNatAllPortMap();


int   liu_testflag = 0;




int getProtoNum(char * proto)
{
	if (strcmp(proto,"TCP")==0)
                return 6;
        else if (strcmp(proto,"UDP")==0)
                return 17;
	else return 0;
}



//////////////////////////////////////////////////////////////////////////////////////
// int PostCallBack(int sockfd,char *firstline,char *body)
//
// in:   int sockfd
//       char *firstline    ;  first line of the received buffer 
//                             such as : POST ****** HTTP/1.1
// 	 char *body         ;  left of the received buffer
//
//////////////////////////////////////////////////////////////////////////////////////


int PostCallBack(int sockfd,char * firstline,char *body)
{
	char  subcmd[64];
	char  eventcmd[64];
	char  httpver[16];
	int i;
	int flag;
	char  servicetype[64];
	char  soapcmd[64];

	memset(soapcmd,0,sizeof(soapcmd));
	memset(servicetype,0,sizeof(servicetype));
	memset(subcmd,0,sizeof(subcmd));
	memset(eventcmd,0,sizeof(eventcmd));
	memset(httpver,0,sizeof(httpver));
	
	if(PharseFirstLine((char *)subcmd,(char *)httpver,firstline) == -1)
		return -1;
	flag = 0;
	for(i = 0;i<DevCount;i++)
	{

		if(!strcmp(GateSoapUrl[i],(char *)subcmd))
		{
			strcpy((char *)servicetype,GateServiceType[i]);
			strcpy((char *)eventcmd,EventUrl[i]); //get the accordding sid to use when receving a addportmapping action
			flag = 1;
			break;  	
		}
		else
			continue; 
	}	

	if(flag != 1)
		return -1;



	if(GetSoapCommand((char *)soapcmd,body) == -1)
		return -1;
/*
	if(GetIpaddressFromSoap((char *)ClientIp,body) == -1)
		return -1;
*/
	SendSoapResponse(sockfd,(char *)eventcmd,(char *)servicetype,(char *)soapcmd,(char *)httpver,body);
	close(sockfd);	
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////
//	int GetSoapCommand(char *cmd,char *body)
//in:   char *body ;   the received packet
//out   char *cmd  ;   the soap action 
//
/////////////////////////////////////////////////////////////////////////////////////
int GetSoapCommand(char *cmd,char *body)
{
	char *   flag = "#";
	char *temp;
	int i = 0;
	if((body =strstr(body,flag))== NULL)
		return -1;
	body ++;
	temp = body;

	while((*temp!='"')&&(*temp != '\r')&&(*temp != '\n')&(*temp != ' '))
	{
		cmd[i] = *temp;
		i++;
		temp++;
	};
	cmd[i] = '\0';

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//    int GetFirstXmlItemValue(char *value,char *body)
//   
//out:    char * value ;    get the item value
//in:     char * item;	    the item we want to look for
//        char * body;      the received packet
//return:    0  	    success
//	    -1  	    error
//////////////////////////////////////////////////////////////////////////////////////////

int GetFirstXmlItemValue(char *value,char *item, char *body)
{
	char *  temp_end;
	char * temp_start;
	char * temp;

	char   item_end[64];
	sprintf((char *)item_end,"</%s>",item);
	if((temp_end = strstr(body,item))==NULL)
		return -1;
	//find the item and next go to the position of the value;
	if((temp_end = strstr(temp_end,">")) == NULL) //the char in front of the item
		return -1;
	temp_end ++;
	temp_start = temp_end;
	if((temp_end = strstr(temp_end,(char *)item_end)) == NULL)
		return -1;
	temp = value;

	while(temp_start!=temp_end)
	{
		*temp = *temp_start;
		temp++;
		temp_start++;
	};
	*temp = '\0';
	return 0;

}

int GetIpaddressFromSoap(char *ip,char *body)
{
	char * ipstart;
	char * temp_ip;

	temp_ip = ip;
	ipstart = strstr(body,"Host:");
	if(ipstart == NULL)
	{
		printf("error in getting ip addres start\n");
		return -1;
	}
	body = ipstart + strlen("Host:");
	while(*body == ' ')
		body ++;
	ipstart = body; //at the start of the ip address
	while(*ipstart != ':')
	{
		*temp_ip = *ipstart;
		temp_ip ++;
		ipstart ++;
	};
	*temp_ip = '\0';
	return 0;
}



//////////////////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////////////////
int UpnpNotifyAllExt(char *eventcmd,char *item,char *value)
{
	struct Subscrible_Event * currentptr;
	struct Subscrible_Event * nextptr;
	struct Thread_Event     * threadevent;

	int i = 0;

	threadevent = (struct Thread_Event *)malloc(sizeof(struct Thread_Event));
	if(threadevent == NULL)
		return -1;
	

	currentptr = SubEvent;
	nextptr = currentptr->next;
	strcpy(threadevent->notifyitem,item);
	strcpy(threadevent->value,value);
	while(currentptr != nextptr)
	{

			
		if(!strcmp(nextptr->Subcmd,eventcmd))
		{

			strcpy(threadevent->ServerIp,nextptr->ServerIp);
			strcpy(threadevent->sid,nextptr->sid);
			nextptr->seq ++;
			threadevent->seq = nextptr->seq;
			UpnpNotifyThread(threadevent);
		}


/*
	
		printf("the node %d ip:%s\n",i,nextptr->ServerIp);
		printf("the node %d cmd:%s\n",i,nextptr->Subcmd);
		printf("the node %d sid:%s\n",i,nextptr->sid);
		printf("the node %ld seq:%d\n",i,nextptr->seq);
*/

		nextptr = nextptr->next;
		i++;


	};
	free(threadevent);
	return 0;	
	
}


/////////////////////////////////////////////////////////////////////////////////////
//
///
/////////////////////////////////////////////////////////////////////////////////////

void  UpnpNotifyThread(struct Thread_Event * event)
{
	
	char notifypacket[1024];
	struct  sockaddr_in  seraddr;
	int     sockfd;

	memset(notifypacket,0,sizeof(notifypacket));
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		printf("error creating socket");
	}

	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(5000);
	seraddr.sin_addr.s_addr = inet_addr(event->ServerIp);
	bzero(&(seraddr.sin_zero),8);

	if(connect(sockfd,(struct sockaddr *)&seraddr,sizeof(struct sockaddr)) == -1)
	{
		fprintf(stderr,"can not connecting client control point ");
	}
	else
	{
	UpnpCreatePropertySet((char *)notifypacket,event);

//	free(event);//free the memory allocated in the thread entry function

	if(send(sockfd,(char *)notifypacket,strlen(notifypacket)+1,0) < 0)
	{
		printf("can not send the packet\n");
	}
	if(recv(sockfd,(char *)notifypacket,1023,0)<0)
	{
	}
	
	close(sockfd);
	}
}

/*
//20070215 rewrite UpnpNotifyThread function 

void  UpnpNotifyThread(struct Thread_Event * event)
{
	
	int status;
	pid_t pid;
  pid = fork();
	if(pid == 0)
	{
		char notifypacket[1024];
		struct  sockaddr_in  seraddr;
		int     sockfd;
		memset(notifypacket,0,sizeof(notifypacket));

		memset(&seraddr, 0, sizeof(seraddr));//20071214 here
		seraddr.sin_family = AF_INET;
		seraddr.sin_port = htons(5000);
		seraddr.sin_addr.s_addr = inet_addr(event->ServerIp);
		bzero(&(seraddr.sin_zero),8);
	
		sockfd = socket(AF_INET,SOCK_STREAM,0);
	
		if(sockfd<0)
		{
			fprintf(stderr,"Debug: error creating socket .\r\n");
		}
		
		if(connect(sockfd,(struct sockaddr *)&seraddr,sizeof(struct sockaddr)) == -1)
		{
			fprintf(stderr,"Debug:can not connecting client control point .\r\n");
		}

		UpnpCreatePropertySet((char *)notifypacket,event);
		
		if(send(sockfd,(char *)notifypacket,strlen(notifypacket)+1,0) < 0)
		{
			fprintf(stderr,"Debug: can not send the packet .\r\n");
		}

		if(recv(sockfd,(char *)notifypacket,1023,0)<0)
		{
			fprintf(stderr,"Debug : recv pkt failure.\r\n");
		}
		close(sockfd);	
		 _exit(0);
	}

	if(waitpid(pid, &status, 0) == -1)
	{
		fprintf(stderr, "Debug : child process is death\r\n");
	}
	else
	{
		fprintf(stderr,"Debug : receive the exit signal from child process. \r\n");
	}
	
}*/
	
void UpnpCreatePropertySet(char *packet,struct Thread_Event * event)
{
	char temp[64];

	char propertyset[512];
	int i;
	memset(temp,0,sizeof(temp));
	memset(propertyset,0,sizeof(propertyset));
	
	sprintf(propertyset,"<e:propertyset xmlns:e=\"urn:schemas-upnp-org:event-1-0\">\n<e:property>\n<%s>%s</%s>\n</e:property>\n</e:propertyset>\n",event->notifyitem,event->value,event->notifyitem);
	i = 0;
	
	strcpy(packet,"NOTIFY /notify ");
        strcat(packet,"HTTP/1.1\r\n");
        sprintf((char *)temp,"HOST: %s:%d\r\n",event->ServerIp,5000);
        strcat(packet,(char *)temp);
        strcat(packet,"CONTENT-TYPE: text/xml\r\n");
        sprintf((char *)temp,"CONTENT-LENGTH: %ld\r\n",strlen((char *)propertyset)+1);
        strcat(packet,(char *)temp);

        strcat(packet,"NT: upnp:event\r\n");
        strcat(packet,"NTS: upnp:propchange\r\n");

        sprintf((char *)temp,"SID: uuid:%s\r\n",(char *)event->sid);
        strcat(packet,(char *)temp);
//	event->seq = event->seq + 1;
        sprintf((char *)temp,"SEQ: %ld\r\n\r\n",event->seq);
        strcat(packet,(char *)temp);
        strcat(packet,(char *)propertyset);
}








//////////////////////////////////////////////////////////////////////////////////////
//	void SendSoapReponse(int sockfd,char *servicetype,char *cmd)
//in:	int sockfd    ;
//      char * servicetype ;
//      char * cmd	;
//
/////////////////////////////////////////////////////////////////////////////////////
extern char autoconf[16]; //lee 1-4

void SendSoapResponse(int sockfd,char *eventcmd,char *servicetype,char *cmd,char *httpversion,char *body)
{
	char RepSoapPacket[2048];
	char RepContents[2048];
	char linerep[384];
	struct  utsname sys_info;
	time_t  currenttm;

	char   PortMapNums[32];
	char   wanip[32];
	char  value_ip[64];
	char  value_intport[64];
	char  value_extport[64];
	char  value_proto[64];
	char  value_desc[64];
	char  index[32];
	char  enabled[16];
	char  leaseduration[32];
	int  notifyflag = 0;
	int loop = 0;
	int succeed = -1;
//gcyin
	char downRate[32];
	char upRate[32];
	unsigned long	NewLayer1UpstreamMaxBitRate = 0;
	unsigned long	NewLayer1DownstreamMaxBitRate = 0;
	unsigned long	timeTemp = 0;
	unsigned long NewUptime = 0;
	
	
	memset(PortMapNums,0,sizeof(PortMapNums));
	memset(RepSoapPacket,0,sizeof(RepSoapPacket));
	memset(RepContents,0,sizeof(RepContents));
	memset(linerep,0,sizeof(linerep));
	memset(wanip,0,sizeof(wanip));
	memset(value_ip,0,sizeof(value_ip));
	memset(value_intport,0,sizeof(value_intport));
	memset(value_extport,0,sizeof(value_extport));
	memset(value_proto,0,sizeof(value_proto));
	memset(value_desc,0,sizeof(value_desc));
	
	strcpy((char *)RepContents,soapstring);
	sprintf((char *)linerep,"<u:%sResponse xmlns:u=\"%s\">\n",cmd,servicetype);
	strcat((char *)RepContents,(char *)linerep);

//gcyin 
	memset(downRate, 0, sizeof(downRate));
	memset(upRate, 0, sizeof(upRate));
	tcapi_get("Info_Adsl", "DataRateUp", upRate);
	NewLayer1UpstreamMaxBitRate = strtoul(upRate, NULL, 0)*1000;
	tcapi_get("Info_Adsl", "DataRateDown", downRate);
	NewLayer1DownstreamMaxBitRate = strtoul(downRate, NULL, 0)*1000;	
	
	if(!strcmp(servicetype,GateServiceType[0]))
	{
	}
	else if(!strcmp(servicetype,GateServiceType[1]))
	{
		if(!strcmp(cmd,"GetTotalBytesSent"))
		{
			succeed = 1;
			sprintf((char *)linerep,"<NewTotalBytesSent>%ld</NewTotalBytesSent>\n",GetTotalCommunicate(BYTESSENT));		
		}
		else if(!strcmp(cmd,"GetCommonLinkProperties"))
		{
			succeed = 1;
			sprintf((char *)linerep,"<NewWanAccessType>Ethernet</NewWanAccessType>\n<NewLayer1UpstreamMaxBitRate>%ld</NewLayer1UpstreamMaxBitRate>\n<NewLayer1DownstreamMaxBitRate>%ld</NewLayer1DownstreamMaxBitRate>\n<NewPhysicalLinkStatus>%s</NewPhysicalLinkStatus>\n",NewLayer1UpstreamMaxBitRate,NewLayer1DownstreamMaxBitRate,getNetIfState(WAN_IFNAME));
		}
		else if(!strcmp(cmd,"GetTotalBytesReceived"))
		{
			succeed = 1;
			sprintf((char *)linerep,"<NewTotalBytesReceived>%ld</NewTotalBytesReceived>\n",GetTotalCommunicate(BYTESRECV));
		}
		else if(!strcmp(cmd,"GetTotalPacketsSent"))
		{
			succeed =1;
			sprintf((char *)linerep,"<NewTotalPacketsSent>%ld</NewTotalPacketsSent>\n",GetTotalCommunicate(PACKETSSENT));
		}
		else if(!strcmp(cmd,"GetTotalPacketsReceived"))
		{
			succeed =1;
			sprintf((char *)linerep,"<NewTotalPacketsReceived>%ld</NewTotalPacketsReceived>\n",GetTotalCommunicate(PACKETSRECV));
		}
		else if(!strcmp(cmd,"X_GetICSStatistics"))
		{
			//time(&currenttm);
			succeed = 1;
			timeTemp = (unsigned long)time(NULL);
			if((NewUptime = (timeTemp - gConnectionStartTime)) > 1000000) { 
				gConnectionStartTime = timeTemp;
				NewUptime = time(NULL) - gConnectionStartTime;
			}
			sprintf((char *)linerep,"<TotalBytesSent>%ld</TotalBytesSent>\n<TotalBytesReceived>%ld</TotalBytesReceived>\n<TotalPacketsSent>%ld</TotalPacketsSent>\n<TotalPacketsReceived>%ld</TotalPacketsReceived>\n<Layer1DownstreamMaxBitRate>%ld</Layer1DownstreamMaxBitRate>\n<Uptime>%ld</Uptime>\n",GetTotalCommunicate(BYTESSENT),GetTotalCommunicate(BYTESRECV),GetTotalCommunicate(PACKETSSENT),GetTotalCommunicate(PACKETSRECV),NewLayer1DownstreamMaxBitRate,(unsigned long)NewUptime);
		}
	}
	else if(!strcmp(servicetype,GateServiceType[2]))
	{
	
		if(!strcmp(cmd,"GetConnectionTypeInfo"))
		{
			succeed = 1;

			strcpy((char *)linerep,"<NewConnectionType>IP_Routed</NewConnectionType>\n<NewPossibleConnectionTypes>IP_Routed</NewPossibleConnectionTypes>\n");

		}
		else if(!strcmp(cmd,"GetNATRSIPStatus"))
		{
			succeed = 1;
			
			sprintf((char *)linerep,"<NewRSIPAvailable>1</NewRSIPAvailable>\n<NewNATEnabled>%s</NewNATEnabled>\n","1");	
		}
		else if(!strcmp(cmd,"GetExternalIPAddress"))
		{
			succeed = 1;
			getIpAddrFromIfname(WAN_IFNAME,(char *)wanip);

			sprintf((char *)linerep,"<NewExternalIPAddress>%s</NewExternalIPAddress>\n",(char *)wanip); //here need modified

		}
		else if(!strcmp(cmd,"AddPortMapping"))
		{
			
			if(autoconf[0] == '0') //lee 1-4
			{
					succeed = -1;
		  }
			else
			{
			if((GetFirstXmlItemValue((char *)value_extport,"NewExternalPort",body)!=-1)
				&&(GetFirstXmlItemValue((char *)value_proto,"NewProtocol",body)!=-1)
				&&(GetFirstXmlItemValue((char *)value_intport,"NewInternalPort",body)!= -1)
				&&(GetFirstXmlItemValue((char *)value_ip,"NewInternalClient",body)!=-1)
				&&(GetFirstXmlItemValue((char *)value_desc,"NewPortMappingDescription",body)!=-1))
			{
				if(getProtoNum((char *)value_proto) == 0)
				{
					succeed = -1;//here add error packet to reponse
				}
				else
				{
					//here add the function to check the ip address
					
				}
				if(GetFirstXmlItemValue((char *)enabled,"NewEnabled",body) ==-1)
				        succeed = -1;
				if(GetFirstXmlItemValue((char *)leaseduration,"NewLeaseDuration",body)==-1)
					succeed = -1;
				//add by liuchong to process port map list 
				if(FindPortMapList(value_proto,atoi(value_extport)))
					DeletePortMapList(value_proto,atoi(value_extport));
			 	AddPortMapList("0",atoi(value_extport),value_proto,atoi(value_intport),value_ip,atoi(enabled),value_desc,atoi(leaseduration));					

/*
				if(SavePortMapList()== -1)
					printf("can not save portmap list!\n");
*/

			  	if(succeed == -1)
				{
					//here should add the function to add port map to the system use the route -add command
					getIpAddrFromIfname(WAN_IFNAME,wanip);
				//	addPacketFilter(value_proto,"0",wanip,atoi(value_extport),1,value_desc);
					fprintf(stderr,"SendSoapResponse:addPortForward\n");
					addPortForward(value_proto,wanip,atoi(value_extport),value_ip,atoi(value_intport),atoi(enabled),value_desc);
				
					//here should set the notify flag to notify client
					notifyflag =1;
					PortMapNumOfEntries ++;
					succeed = 1;
					strcpy((char *)linerep,"");


				}


			}
	}


		}
		else if(!strcmp(cmd,"DeletePortMapping"))
		{
			//here add it in future	
			if((GetFirstXmlItemValue((char *)value_extport,"NewExternalPort",body)!=-1)
				&&(GetFirstXmlItemValue((char *)value_proto,"NewProtocol",body)!=-1))
			{
				if(getProtoNum((char *)value_proto) == 0)
                                {
                                        succeed = -1;//here add error packet to reponse
                                }
                                else
                                {
                                        //here add the function to check the ip address

                                }

				if(succeed == -1)
				{
					
					//here add the function to add the portmap by the syscall
					getIpAddrFromIfname(WAN_IFNAME,wanip);
					PortMapDelete((char *)value_proto,atoi(value_extport),wanip);	

					DeletePortMapList(value_proto,atoi(value_extport));

/*
					if(SavePortMapList() ==-1)
						printf("can not save the port map config\n");
*/


					if(1)
					{
						notifyflag = 1;
						PortMapNumOfEntries--;
						succeed = 1;
				
					}
					
					strcpy((char *)linerep,"");

				}

			}

		}
		else if(!strcmp(cmd,"RequestConnection"))
		{
			succeed = 1;
			strcpy((char *)linerep,"<RequestConnection>Connected</RequestConnection>\n");
		}
		else if(!strcmp(cmd,"GetGenericPortMappingEntry"))
		{
			if(GetFirstXmlItemValue(index,"NewPortMappingIndex",body) ==-1)
				return ; //here should add error reponse packet
			if(atoi(index) == 0)
			{
				while(strcmp(PortMapListPtr->PortMapDescription,"Head"))
				{
					loop ++;
					if(loop >64)
						break;
					//printf("the Port desc is:%s\n",PortMapListPtr->PortMapDescription);
					PortMapListPtr = PortMapListPtr->Next;
				};
			}
			PortMapListPtr = PortMapListPtr->Next;
			if(!strcmp(PortMapListPtr->PortMapDescription,"Head"))
					succeed = -1;
			else
			{
			//	printf("***The response ext port is:%d\n",PortMapListPtr->ExternalPort);
				succeed = 1;
				sprintf((char *)linerep,"<NewRemoteHost></NewRemoteHost><NewExternalPort>%d</NewExternalPort><NewProtocol>%s</NewProtocol><NewInternalPort>%d</NewInternalPort><NewInternalClient>%s</NewInternalClient><NewEnabled>%d</NewEnabled><NewPortMappingDescription>%s</NewPortMappingDescription><NewLeaseDuration>%d</NewLeaseDuration>",PortMapListPtr->ExternalPort,PortMapListPtr->Protocol,PortMapListPtr->InternalPort,PortMapListPtr->InternalClientIp,PortMapListPtr->Enabled,PortMapListPtr->PortMapDescription,PortMapListPtr->LeaseDuration);
			}
			
		
		}else if(!strcmp(cmd,"GetSpecificPortMappingEntry"))
		{
			if(GetFirstXmlItemValue((char *)value_extport,"NewExternalPort",body)==-1)
				return;
			if(GetFirstXmlItemValue((char *)value_proto,"NewProtocol",body)==-1)
				return;

			if(1)
			{
				if(FindPortMapList(value_proto,atoi(value_extport)))
				{
					succeed = 1;
					sprintf((char *)linerep,"<NewInternalPort>%d</NewInternalPort><NewInternalClient>%s</NewInternalClient><NewEnabled>%d</NewEnabled><NewPortMappingDescription>%s</NewPortMappingDescription><NewLeaseDuration>%d</NewLeaseDuration>",PortMapListPtr->InternalPort,PortMapListPtr->InternalClientIp,PortMapListPtr->Enabled,PortMapListPtr->PortMapDescription,PortMapListPtr->LeaseDuration);
				}
	
			}

		}



	}
	else
	{
	}

	if(succeed !=1)
	{
		memset(RepSoapPacket,0,2048);
		SoapErrorResponse(RepSoapPacket);
	}
	else
	{
		strcat((char*)RepContents,(char *)linerep);
		sprintf((char *)linerep,"</u:%sResponse>\n</s:Body></s:Envelope>",cmd);
		strcat((char *)RepContents,(char *)linerep);

		memset(&sys_info,0x00,sizeof(sys_info));

		uname(&sys_info);

// contruct the response soap packet

		sprintf(linerep,"%s 200 OK\r\n",httpversion);
		strcpy((char *)RepSoapPacket,(char *)linerep);
		sprintf(linerep,"CONTENT-LENGTH:%ld\r\n",strlen((char *)RepContents)+1);
		strcat((char *)RepSoapPacket,(char *)linerep);
		strcat((char *)RepSoapPacket,"CONTENT-TYPE:text/xml\r\n");
		currentTmToHttpDate(0,linerep);
		strcat((char *)RepSoapPacket,(char *)linerep); //date;
		strcat((char *)RepSoapPacket,"EXT:\r\n");
		sprintf((char *)linerep,"SERVER:%s/%s UPnP/1.0 myigd/1.0\r\n\r\n",sys_info.sysname,sys_info.release);

		strcat((char *)RepSoapPacket,(char *)linerep);

		strcat((char *)RepSoapPacket,(char *)RepContents);
	}
	
	send(sockfd,(char *)RepSoapPacket,strlen((char *)RepSoapPacket)+1,0);
	close(sockfd);

	if(notifyflag == 1)
	{
		sprintf(PortMapNums,"%d",PortMapNumOfEntries);
		UpnpNotifyAllExt(eventcmd,"PortMappingNumberOfEntries",(char *)PortMapNums);
	}

}




int AddPortMapList(char * RmtHostIp,int ExtPort,char *Proto,int IntPort,char *IntClientIp,int Enabled,char *PortMapDes,int LeaseDura)
{
	struct mPortMapList * ListTemp;
	struct mPortMapList * Temp;

	ListTemp =(struct mPortMapList *)malloc(sizeof(struct mPortMapList));

	memset(ListTemp,0,sizeof(struct mPortMapList));

	strcpy(ListTemp->RemoteHostIp,RmtHostIp);

	ListTemp->ExternalPort = ExtPort;
	strcpy(ListTemp->Protocol,Proto);
	ListTemp->InternalPort = IntPort;
	strcpy(ListTemp->InternalClientIp,IntClientIp);
	ListTemp->Enabled = Enabled;
	strcpy(ListTemp->PortMapDescription,PortMapDes);
	ListTemp->LeaseDuration = LeaseDura;
	ListTemp->Next = NULL;
	ListTemp->Previous = NULL;

	Temp = PortMapListPtr->Next;

	PortMapListPtr->Next = ListTemp;
	ListTemp->Previous = PortMapListPtr;
	ListTemp->Next = Temp;
	Temp->Previous = ListTemp;

	return 0;	
}

int PortMapInit()
{


	char ExtPort[10];
	char Protocol[10];
	char IntPort[10];
	char IntClient[20];
	char Enabled[4];
	char Desc[64];
	char LeaseDur[10];

	char buff[100];
	char wanip[32];

	FILE * fp;
	int p;



	PortMapListPtr =(struct mPortMapList *)malloc(sizeof(struct mPortMapList));
	strcpy(PortMapListPtr->PortMapDescription,"Head");
	PortMapListPtr->Next = PortMapListPtr;
	PortMapListPtr->Previous = PortMapListPtr;
//	delNatAllPortMap();
	// get wan ip address



	getIpAddrFromIfname(WAN_IFNAME,wanip);

	if((fp = fopen(PORTMAPFILENAME,"r")))
	{
		if(fgets(buff,99,fp)==NULL)
			return -1;
		while(!feof(fp))
		{
			p = PortMapRead(fp,(char *)ExtPort,(char *)Protocol,(char *)IntPort,(char *)IntClient,(char *)Enabled,(char *)Desc,(char *)LeaseDur);
			if(p == 1)
			{
				PortMapNumOfEntries ++;

				AddPortMapList("0.0.0.0",atoi((char *)ExtPort),(char *)Protocol,atoi((char *)IntPort),(char *)IntClient,atoi((char *)Enabled),(char *)Desc,atoi((char *)LeaseDur));


				if(atoi(Enabled) == 1)
				{
					fprintf(stderr,"PortMapInit:addPortForward\n");
					addPortForward((char *)Protocol,wanip,atoi(ExtPort),IntClient,atoi(IntPort),1,Desc);
				}

			}
			else
				continue;
		}
	}
	else
	{
		exit(0);
	}
	fclose(fp);

	return 0;
}

int DeletePortMapList(char *proto,int ExtPort)
{
	int result;
	struct mPortMapList *DeleteTemp;
	struct mPortMapList *Next;
	struct mPortMapList *Previous;

	result = FindPortMapList(proto,ExtPort);
	if(result == 1)
	{
		DeleteTemp = PortMapListPtr;
		Next = PortMapListPtr->Next;
		Previous = PortMapListPtr->Previous;
		PortMapListPtr = PortMapListPtr->Next;
		
		Previous->Next = Next;
		Next->Previous = Previous;
		free(DeleteTemp);
		return 1;
	}
	else
		return -1;
	
	
}


int FindPortMapList(char * proto,int ExtPort)
{
	struct mPortMapList *PortMapStart;
	PortMapStart = PortMapListPtr;

	while(1)
	{
		if((PortMapListPtr->ExternalPort == ExtPort)&&!(strcmp(PortMapListPtr->Protocol,proto)))
		{
			return 1;
		}
		PortMapListPtr = PortMapListPtr->Next;

		if(PortMapListPtr->Next == PortMapStart)
		{
			if((PortMapListPtr->ExternalPort == ExtPort)&&(!strcmp(PortMapListPtr->Protocol,proto)))
				return 1;
			return 0;
		}
	};
}

int PortMapRead(FILE *fp,char *ExtPort,char *Protocol,char *IntPort,char *IntClient,char *Enabled,char *Desc,char *LeaseDur)
{
	char buff[100];
	char *next;
	char *out[7];
	int i;
	int exit_flag;

	for(i=0;i<7;i++)
		if((out[i]=(char *)malloc(32))==NULL)
			exit_flag = 1;

	if(fgets(buff,99,fp))
	{
		i = 0;
		if((next = strchr(buff,'\n')))
			*next = '\0';
		if(!strcmp(buff,"\0"))
			exit_flag = 1;

		while((next = strchr((char *)buff,':')))
		{
			*next = '\0';
			strcpy(out[i],(char *)buff);
			strcpy((char *)buff,next+1);
			i ++;
		};
		strcpy(out[i],(char *)buff);

	}
	else
		exit_flag = 1;

if(exit_flag !=1)
{	
	strcpy(ExtPort,out[0]);
	strcpy(Protocol,out[1]);
	strcpy(IntPort,out[2]);
	strcpy(IntClient,out[3]);
	strcpy(Enabled,out[4]);
	strcpy(Desc,out[5]);
	strcpy(LeaseDur,out[6]);

}
	for(i=0;i<7;i++)
		free(out[i]);
if(exit_flag !=1)
	return 1;
else
	return -1;
}

/*
int PortMapWrite(FILE *fp,int ExtPort,char *Protocol,int IntPort,char *IntClient,int Enabled,char *Desc,int LeaseDur)
{
	char buff[100];
	memset(buff,0,sizeof(buff));
	sprintf(buff,"%d:%s:%d:%s:%d:%s:%d\n",ExtPort,Protocol,IntPort,IntClient,Enabled,Desc,LeaseDur);
	if(!fputs(buff,fp))
		return -1;


}

*/

int SavePortMapList()
{
	int loop = 0;
	FILE * fp;
	char buff[100];
	while(strcmp(PortMapListPtr->PortMapDescription,"Head"))
	{
		PortMapListPtr = PortMapListPtr->Next;
		if(loop >64)
			break;
		loop ++;
	}

	if(!(fp = fopen(PORTMAPFILENAME,"w")))
		return -1;
	
	if(!fputs(TITLE,fp))
		return -1;
	
	memset(buff,0,sizeof(buff));
	PortMapListPtr = PortMapListPtr->Next;
	
	while(strcmp(PortMapListPtr->PortMapDescription,"Head"))
	{
		sprintf(buff,"%d:%s:%d:%s:%d:%s:%d\n",
			PortMapListPtr->ExternalPort,PortMapListPtr->Protocol,PortMapListPtr->InternalPort,
			PortMapListPtr->InternalClientIp,PortMapListPtr->Enabled,PortMapListPtr->PortMapDescription,
			PortMapListPtr->LeaseDuration);
		if(!fputs(buff,fp))
			return -1;	
		PortMapListPtr = PortMapListPtr->Next;
	}
	fclose(fp);
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void PortMapDelete(char *proto,int extport,char *wanip)
{
	int result;

//	pthread_mutex_lock(&sublist_mutex);	

	result = FindPortMapList(proto,extport);

	if(result == 1)
	{
		if(PortMapListPtr->Enabled == 1)
		{
	//		delPacketFilter(proto,PortMapListPtr->RemoteHostIp,wanip,extport);
			delPortForward(proto,wanip,extport,PortMapListPtr->InternalClientIp,PortMapListPtr->InternalPort);
		}
	}
//	pthread_mutex_unlock(&sublist_mutex);

}


/////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
int addPacketFilter(char * Proto, char *SrcIP, char *DestIP,int DestPort,int Enabled, char *Desc)
{

	static char *IpAny = "0/0";
//	char prt[4];
	int ret=0;

	if(!strcmp(SrcIP,"0"))
		SrcIP = IpAny;
//	sprintf(command, "/sbin/ipchains -I upnp -j ACCEPT -p %s -s %s -d %s %d",
//	Proto, SrcIP, DestIP, DestPort);
//	printf("add portmap packet filter\n");
//	system(command);
	ret = 1;
	return (1);
}


int addPortForward(char * Proto, char *ExtIP, int ExtPort, char *IntIP,int IntPort, int Enabled, char *Desc)
{
	char command[255];
	char prt[4];

	int ret=0;

	if (!strcmp(Proto,"TCP")||!strcmp(Proto,"tcp"))
		strcpy (prt, "tcp");
	else
		strcpy (prt, "udp");
fprintf(stderr,"%s %s %d %s %d %d %s\n",Proto,ExtIP,ExtPort,IntIP,IntPort,Enabled,Desc);
	if(Enabled == 1)
	{
/*		sprintf(command,"/sbin/aupnpiptables.sh %s %s %d %s %d", prt, ExtIP, ExtPort, IntIP, IntPort);
*///lee 1-4 UPNP_PRE
		sprintf(command,"iptables -t nat -I UPNP_PRE -p %s -d %s --dport %d -j DNAT --to %s:%d",prt,ExtIP,ExtPort,IntIP,IntPort);
		
	}
	else
	{
	/*	sprintf(command,"/sbin/dupnpiptables.sh %s %s %d %s %d", prt, ExtIP, ExtPort, IntIP, IntPort);
*/
		sprintf(command,"iptables -t nat -D UPNP_PRE -p %s -d %s --dport %d -j DNAT --to %s:%d",prt,ExtIP,ExtPort,IntIP,IntPort);

	}

/*
	pid = vfork();

	if(pid == 0)
		execl("/bin/sh","sh","-c",command,(char *)0);
*/
	system(command);

	ret = 1;
	return (ret);
}

int delNatAllPortMap()
{

	char command[255];
	sprintf(command,"iptables -t nat -F UPNP_PRE");  //lee 1-4
	system(command);
	return 0;

}


int delPacketFilter(char * Proto, char *SrcIP, char *DestIP, int DestPort)
{

	static char *IpAny = "0/0";
//	char prt[4];
	int ret=0;
			
	if(!strcmp(SrcIP,"0"))
		SrcIP = IpAny;

/*	if (Proto==6)
		strcpy(prt, "tcp");
	else
		strcpy(prt, "udp");
*/

//	sprintf(command,"/sbin/ipchains -D upnp -j ACCEPT -p %s -s %s -d %s %d"
//	, Proto, SrcIP, DestIP, DestPort);
	
//	system(command);

	ret = 1;
	return(ret);
}

int delPortForward(char * Proto, char *ExtIP, int ExtPort, char* IntIP, int IntPort)
{

	char command[255];
	char prt[4];

	int ret=0;
	
	if (!strcmp(Proto,"TCP")||!strcmp(Proto,"tcp"))
		strcpy (prt, "tcp");
	else
		strcpy (prt, "udp");

/*
	sprintf(command, "/sbin/dupnpiptables.sh %s %s %d %s %d", prt, ExtIP, ExtPort, IntIP, IntPort);
*/
	sprintf(command,"iptables -t nat -D UPNP_PRE -p %s -d %s --dport %d  -j DNAT --to %s:%d",prt,ExtIP,ExtPort,IntIP,IntPort);

/*
	pid = vfork();
	if(pid == 0)
		execl("/bin/sh","sh","-c",command,(char *)0);
*/
	system(command);

	ret = 1;

	return (ret);
}

int SoapErrorResponse(char * errstr)
{
	char line[1024];
	char temp[64];
	struct  utsname  sys_info;
	
	memset(temp,0,64);
	memset(line,0,1024);
	memset(&sys_info,0x00,sizeof(sys_info));
	uname(&sys_info);
	
	strcat(line,"<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">");
	strcat(line,"<s:Body><s:Fault><faultcode>s:Client</faultcode><faultstring>UPnPError</faultstring><detail>");
	strcat(line,"<UPnPError xmlns=\"urn:schemas-upnp-org:control-1-0\">");
	strcat(line,"<errorCode>401</errorCode><errorDescription>Invalid Action</errorDescription></UPnPError></detail></s:Fault></s:Body></s:Envelope>\r\n");

	strcpy(errstr,"HTTP/1.1 500 Internal Server Error\r\n");
	sprintf(temp,"CONTENT-LENGTH: %ld\r\n",strlen(line));
	strcat(errstr,temp);
	strcat(errstr,"CONTENT-TYPE:text/xml\r\n");
	strcat(errstr,"EXT:\r\n");
	sprintf(temp,"SERVER:%s/%s UPnP/1.0 myigd/1.0\r\n",sys_info.sysname,sys_info.release);
	strcat(errstr,temp);
	strcat(errstr,line);
	return 0;
	
}

