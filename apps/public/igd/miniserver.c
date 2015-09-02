/////////////////////////////////////////////////////////////////////////////////////////////////////
// This file inlcudes some function to process the http request and reonse.
// 
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////


#include "miniserver.h"
#include "test.h"
#include "event.h"
#include "tools.h"
#include "deviceinfo.h"
#include  "descfile.h"
#include "libtcapi.h"

#define  EVENT_OSINFO            0
#define  EVENT_WANCOMMONIFC      1
#define  EVENT_WANIPCON          2

extern  struct device_info * deviceinfo[10];
extern  int    DevCount;
extern  char   lanip[32];
extern  int   PortMapNumOfEntries;
extern  char  LAN_IFNAME[16];
extern  char  WAN_IFNAME[16];

struct Subscrible_Event * SubEvent;

char *    EventUrl[] = {"upnp/event/OSInfo1",
		      "upnp/event/WANCommonIFC1",
		      "upnp/event/WANIPConn1"};


char     serialchar[] = {'0','1','2','3','4','5','6','7','8','9',
		       'a','b','c','d','e','f'};

int    test_id = 0;

int miniserverrunning = 0;
extern int errno;

void  handle_subscrible(void *args);

int SubscribleEventListAdd(char *ip,char *sid,char *Subcmd);
int SubscribleEventListFind(char *ip,char *subcmd);
int SubscribleEventListDelete(char *ip,char *subcmd);

int ReadNextPostPacket(int sockfd,char *body);
int SubscribleEventListInit(void)
{
	SubEvent = (struct Subscrible_Event *)malloc(sizeof(struct Subscrible_Event));
	if(SubEvent == NULL)
		return -1;
	strcpy((char *)SubEvent->desc,"Head");
	SubEvent->ServerPort = 5000;
	SubEvent->ServerIp[0] = '\0';
	SubEvent->Subcmd[0] ='\0';
	strcpy(SubEvent->http_ver,"HTTP/1.1");
	SubEvent->sid[0] = '\0';
	SubEvent->seq = 0;
	SubEvent->next = SubEvent;
	SubEvent->previous = SubEvent;
	return 0;
}


int SubscribleEventListAdd(char *ip,char *sid,char *Subcmd)
{
	struct Subscrible_Event *ListTemp;
	struct Subscrible_Event *Temp;
	
	if(SubscribleEventListFind(ip,Subcmd) == 1)
	{
		strcpy((char *)SubEvent->sid,sid);
		SubEvent->seq = 0;
		return 0;
	}

	ListTemp = (struct Subscrible_Event *)malloc(sizeof(struct Subscrible_Event));
	memset(ListTemp,0,sizeof(struct Subscrible_Event));
	ListTemp->ServerPort = 5000;
	strcpy((char *)ListTemp->ServerIp,ip);
	strcpy((char *)ListTemp->Subcmd,Subcmd);
	strcpy((char *)ListTemp->sid,sid);
	strcpy((char *)ListTemp->http_ver,"HTTP/1.1");
	ListTemp->seq = 0;
	ListTemp->desc[0] = '\0';
	ListTemp->next = NULL;
	ListTemp->previous = NULL;
	Temp = SubEvent->next;
	SubEvent->next = ListTemp;
	ListTemp->previous = SubEvent;
	ListTemp->next = Temp;
	Temp->previous = ListTemp;
	return 0;
		
	
}

int SubscribleEventListFind(char *ip,char *subcmd)
{
	struct Subscrible_Event * start;
	start = SubEvent;
	while(1)
	{
		if(!strcmp(SubEvent->ServerIp,ip)&&!strcmp(SubEvent->Subcmd,subcmd))
		{
			return 1;
		}
		SubEvent = SubEvent->next;
		if(SubEvent->next ==start)
		{
			if(!strcmp(SubEvent->ServerIp,ip)&&!strcmp(SubEvent->Subcmd,subcmd))
				return 1;
			return 0;
		}
	};
}

int SubscribleEventListDelete(char *ip,char *subcmd)
{
	int result;
	struct Subscrible_Event * deletetemp;
	struct Subscrible_Event * next;
	struct Subscrible_Event * previous;

	result = SubscribleEventListFind(ip,subcmd);
	if(result == 1)
	{
		deletetemp = SubEvent;
		next = SubEvent->next;
		previous = SubEvent->previous;
		SubEvent = SubEvent->next;

		previous->next = next;
		next->previous = previous;
		free(deletetemp);
		return 1;
	}
	else
		return -1;

}



int MiniServerInit(int listenport)
{
	int listenfd;
	int val,SsdpSock;
	int connfd;
	
        time_t   current_time;
        struct   tm * current_tm;
        		
	int maxfd;
	int result;
	u_char 	 Ttl=4;
    
	fd_set   servfd;
	struct   sockaddr_in tcpServerAddr;
	struct   ip_mreq SsdpMcastAddr;
    	struct   sockaddr_in SelfAddr;
	struct   sockaddr_in ClientAddr;
	
	int      clientlen;
	
	int      On = 1;
	
        time(&current_time);
        current_tm = gmtime(&current_time);

	srandom(current_tm->tm_hour*current_tm->tm_min*current_tm->tm_sec);

	SubscribleEventListInit();
//UDP server start..........
        SsdpSock = socket(AF_INET, SOCK_DGRAM, 0);
        val = fcntl(SsdpSock,F_GETFL,0);
        fcntl(SsdpSock,F_SETFL,val|O_NONBLOCK);

    	if ( SsdpSock == -1 || val == -1)
    	{
       		return -1;
    	}
    	
    	if ( setsockopt(SsdpSock, SOL_SOCKET, SO_REUSEADDR, &On, sizeof(On)) == -1)
    		return -1;

    	bzero(&SsdpMcastAddr, sizeof(struct ip_mreq ));
    	SsdpMcastAddr.imr_interface.s_addr = htonl(INADDR_ANY);
    	SsdpMcastAddr.imr_multiaddr.s_addr = inet_addr(SSDP_IP);


    	bzero((char *)&SelfAddr, sizeof(struct sockaddr_in));

    	SelfAddr.sin_family = AF_INET;
    	SelfAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    	SelfAddr.sin_port = htons(1900);

    	if (bind( SsdpSock, (struct sockaddr *) &SelfAddr, sizeof(SelfAddr)) != 0)
    	{
       		return 	-1;
        }
    	setsockopt(SsdpSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &SsdpMcastAddr, sizeof(struct ip_mreq));
    	setsockopt(SsdpSock, IPPROTO_IP, IP_MULTICAST_TTL, &Ttl, sizeof(Ttl));



// TCP Server start.........
	bzero(&tcpServerAddr,sizeof(tcpServerAddr));
	tcpServerAddr.sin_family = AF_INET;
	tcpServerAddr.sin_port = htons(SOAP_PORT);
	tcpServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd == -1)
	{
		printf("error create listen socket \n");
		return -1;
	}


	if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&On,sizeof(int))==-1)
	{
		printf("set socketopt error\n");
		return -1;
	}



	if(bind(listenfd,(struct sockaddr *)&tcpServerAddr,sizeof(tcpServerAddr)) == -1)
	{
		printf("bind listenfd error\n");
		close(listenfd);
		return -1;
	}
	if(listen(listenfd,10) == -1)
	{
		printf("listen error \n");
		close(listenfd);
		return -1;
	}

	FD_ZERO(&servfd);
	maxfd = SsdpSock;
	if(SsdpSock<listenfd)
		maxfd = listenfd;
	while(1)
	{
		FD_SET(listenfd,&servfd);
		FD_SET(SsdpSock,&servfd);
		result = select(maxfd+1,&servfd,NULL,NULL,NULL);

		//Ren.B
		if(result == -1 )
		{
			if(errno == EINTR)
			{
				continue;
			}

			printf("[upnpd][%s][%d]select(), result=%d\n", __FUNCTION__, __LINE__, result );
			return -1;
		}
		//Ren.E

		if(FD_ISSET(listenfd,&servfd))
		{
			clientlen = sizeof(ClientAddr);
			connfd = accept(listenfd,(struct sockaddr *)&ClientAddr,&clientlen);

			HandleRequest(connfd);
			close(connfd);
		}
	 	if(FD_ISSET(SsdpSock,&servfd))
		{
			StartUdpCommunication(SsdpSock);
		}
	
	}

	return 0;
}


void StartUdpCommunication(int sockfd)
{
	
	int   len,addrlen;
	struct  sockaddr_in clientaddr;
 	char  body[1024];
	char  buffer[1024];
	char  firstline[64];
	char  cmd[64];

	memset(body,0,1024);
	memset(firstline,0,64);
	memset(cmd,0,64);
	memset(buffer,0,1024);


	len = 0;
	addrlen = sizeof(clientaddr);
	len = recvfrom(sockfd,buffer,1023,0,(struct sockaddr *)&clientaddr,&addrlen);
	if(len < 0)
	{
		return;
	}
	GetCmd(cmd,firstline,buffer,body,len);

	if(!strcmp(cmd,"M-SEARCH"))
	{

		MsearchCallBack(sockfd,&clientaddr,firstline,body);
	}
	else
		printf("can not equal M-SEARCH");

}

void HandleRequest(int sockfd)
{
	int totalsize;

	int retcode;
	char firstline[64];

//	fd_set readset;
//	struct timeval timeout;
	char buffer[2048];
	char nextbuff[2048];

	char cmd [64];
	char body[2048];


//	timeout.tv_sec = 30;
//	timeout.tv_usec = 0;
//	FD_ZERO(&readset);
//	FD_SET(sockfd,&readset);
	memset(firstline,0,64);
	memset(buffer,0,2048);
	memset(body,0,2048);
	memset(nextbuff,0,2048);	

/*
	while(1)
	{
		retcode = select(sockfd + 1,&readset,NULL,NULL,&timeout);
		if(retcode == 0)
			if(DEBUG)
				printf("select read timeout\n");
		if(retcode == -1)
		{
			if(errno == EINTR)
				continue;
		}
		else
			break;
		
		
	};//end while (1)

*/

	totalsize = 0;
	totalsize = recv(sockfd,buffer,2047,0);

	//get the cmd from client,such as GET POST,NOTIFY etc.
	GetCmd(cmd,firstline,buffer,body,totalsize);

	if(DEBUG)
	{
	}

	//process the cmd and reponse it
	retcode = ProcessHttpCmd(sockfd,cmd,firstline,body);
	
//	return 0;

}
/////////////////////////////////////////////////////////////
// process the request cmd and reponse it
// in:  int sockfd  ; socket
// in:  char * cmd  ; HTTP command such as "GET"
// in: char * firstline ; first line of http request
// in: char * body;  the buffer of the http request except the first line
//
/////////////////////////////////////////////////////////////
int ProcessHttpCmd(int sockfd,char *cmd,char *firstline,char *body)
{

	char c;
	c = cmd[0];
	switch(c)
	{
		case 'G':
			GetCallBack(sockfd,firstline,body);
			break;
		case 'P':
			ReadNextPostPacket(sockfd,body);
			PostCallBack(sockfd,firstline,body);
			break;
		case 'M':
			//	MpostCallBack(sockfd,firstline,body);
			break;
		case 'S':
			SubscribleCallBack(sockfd,firstline,body);
			break;
		case 'U':
			UnsubscribleCallBack(sockfd,firstline,body);
			break;
		case 'N':
		//	NotifyCallBack(sockfd,firstline,body);
			break;
	}
	return 0;


}

int ReadAllPacket(int sockfd,char * vptr,int n)
{
	int nleft;
	int nread;
	char * ptr;

	ptr = vptr;
	nleft = n;
	while(nleft > 0)
	{
		if((nread = recv(sockfd,ptr,nleft,0)) <0 )
		{
			if(errno == EINTR)
				nread = 0;
			else
				return -1;
		}
		else if(nread == 0)
			break;
		nleft -= nread;
		ptr   += nread;
	};
	return (n - nleft);
}



int  ReadNextPostPacket(int sockfd,char *body)
{
	int  bsize = 0;
	char buffer[2048];
	char * xmlflag = "http://schemas.xmlsoap.org/soap/envelope/";

	memset(buffer,0,2048);

	if(strstr(body,xmlflag)==NULL)
	{
		bsize = recv(sockfd,buffer,2047,0);
		if(bsize > 0)
		{
			strcat(body,buffer);
		}
	}
	return 0;
	
}


////////////////////////////////////////////////////////////////////////
////
////
////
////
////////////////////////////////////////////////////////////////////////
int MsearchCallBack(int sockfd,struct sockaddr_in *clientaddr,char * firstline,char * body)
{
	char  urn[64];
	char  m_ssdp[16];
	char  packet[1024];
	int   result;
	char  uuid[64];
	
//	int   test_sock;
		
	int len;


	int   addrlen = sizeof(struct sockaddr_in);

	
	memset(urn,0,64);
	memset(m_ssdp,0,16);
	memset(packet,0,1024);
	memset(uuid,0,64);
	if(PharseMsearchString(body,urn,m_ssdp) == -1)
		return -1;

	result = CheckInvalidMsearch(urn,m_ssdp,uuid);
	if(result !=0)
	{
		//add the error code respond
		return 0;
	}
	//here respond	
	if(CreateMsearchRespondPacket(packet,urn,uuid)==-1)
		return -1;
/*
	test_sock = socket(AF_INET,SOCK_DGRAM,0);
*/	
	if((len = sendto(sockfd,packet,strlen(packet),0,(struct sockaddr *)clientaddr,addrlen)) == -1)
	{
		return -1;
	}
	if((len = sendto(sockfd,packet,strlen(packet),0,(struct sockaddr *)clientaddr,addrlen)) == -1)
	{
		return -1;
	}


	return 0;
	//send the packet
	
}

int  CreateMsearchRespondPacket(char * packet,char * urn,char * uuid)
{
	char   tempstr[128];
	char   ipaddr[24];
	char   date[40];
	
	struct  utsname   sys_info;
	

	memset(&sys_info,0x00,sizeof(sys_info));
	uname(&sys_info);
	
	memset(tempstr,0,128);
	memset(ipaddr,0,24);
	memset(date,0,40);
	currentTmToHttpDate(0,date);

	if(getIpAddrFromIfname(LAN_IFNAME,ipaddr) == -1)
		return -1;

	strcpy(packet,"HTTP/1.1 200 OK\r\n");
	strcat(packet,"CACHE-CONTROL: max-age=1800\r\n");
	strcat(packet,date);
	strcat(packet,"EXT:\r\n");
	sprintf(tempstr,"LOCATION: http://%s:%d/%s\r\n",ipaddr,SOAP_PORT,XML_DEVICEDESC_FILE);
	strcat(packet,tempstr);
	sprintf(tempstr,"SERVER: %s/%s UPnP/1.0 myigd/1.0\r\n",sys_info.sysname,sys_info.release);
	strcat(packet,tempstr);
	sprintf(tempstr,"ST: %s\r\n",urn);
	strcat(packet,tempstr);
	sprintf(tempstr,"USN: %s::%s\r\n\r\n",uuid,urn);

	strcat(packet,tempstr);
	return 0;

}
int CheckInvalidMsearch(char * urn,char * m_ssdp,char * uuid)
{
	int flag;
	int i;

	if(strcmp(m_ssdp,"ssdp:discover"))
	{
		//here add error 412 respond
	}
	flag = -1;	
	for(i=0;i<DevCount;i++)
	{
		if((!strcmp(deviceinfo[i]->udn,urn))||(!strcmp(deviceinfo[i]->devicetype,urn))||(!strcmp(deviceinfo[i]->servicetype,urn)))
		{
			strcpy(uuid,deviceinfo[i]->udn);
			flag = 0;
			break;
		}
	}

	if(!strcmp(urn,"upnp:rootdevice"))
	{
		strcpy(uuid,deviceinfo[0]->udn);
		flag = 0;
	}

	if(flag == -1)
	{
		//addr error respond
		return -1;
	}
			

	return 0;
}

int  PharseMsearchString(char * body,char *urn,char *m_ssdp)
{	
	char * temp_s;
	char * temp_e;
	//get the st string

	if((body = strstr(body,"239.255.255.250")) == NULL)
		return -1;

	if((temp_s = strstr(body,"ST:")) == NULL)
		return -1;
	temp_s = temp_s + strlen("ST:");
	while(*temp_s == ' ')
		temp_s++;
	if((temp_e = strstr(temp_s,"\r\n")) == NULL)
		return -1;
	strncpy(urn,temp_s,(temp_e-temp_s));
	
	if((temp_s = strstr(body,"ssdp:")) == NULL)
		return -1;
	if((temp_e = strstr(temp_s,"\"")) == NULL)
		return -1;
	strncpy(m_ssdp,temp_s,temp_e-temp_s);

	return 0;

}


int UnsubscribleCallBack(int sockfd,char *firstline,char *body)
{
	send(sockfd,"HTTP/1.1 200 OK\r\n",strlen("HTTP/1.1 200 OK\r\n"),0);
	close(sockfd);
	return 0;
}



/////////////////////////////////////////////////////////////////////////////
//  SubscribleCallBack(),process the subscrible request and reponse it
//  in:   sockfd, 
//
//
//
//
//////////////////////////////////////////////////////////////////////////////
int SubscribleCallBack(int sockfd,char * firstline,char *body)
{
	char subcmd[64]; //store the subscrible command;
	char serverIp[24];
	char sid[64];
	char http_ver[16];

	//krammer add for bug 857
	#ifdef TRENDCHIP
	char call_back_path[64]={0};/*call back path buffer*/
	#endif


	
	int  serverport;

	char * temp;
	

	
	if(PharseFirstLine((char *)subcmd,(char *)http_ver,firstline) == -1)
		return -1;
	temp = strstr(body,"Callback: <http://");
	if(temp == NULL)
		return -1;
	temp = temp + strlen("Callback: <http://"); //point to the ip address

	body = temp;
	body = strstr(body,":");
	* body = '\0';

	strcpy(serverIp,temp); //get the notify server ip address

	body ++;
	temp =body;
	body =  strstr(body,"/");
	*body = '\0';
	
	serverport = atoi(temp); // get the notify server port;

	#ifdef TRENDCHIP
	body ++;
	temp =body;
	/*the format is : call/back/path>*/
	if(body){
		body =  strstr(body,">");
		if(body){
			*body = '\0';
		}
	}

	if(temp){
		strcpy(call_back_path, temp);
	}
	#endif

	//create the serial id
	CreateSerialId(sid);
	if(DEBUG)
	{
	//	printf("-------------------------------------------------\n");
	//	printf("the ip:%s\n",(char *)serverIp);
	//	printf("the port:%d\n",serverport);
	//	printf("the cmd :%s\n",(char *)subcmd);
	//	printf("the sid :%s\n",(char *)sid);
	}

	SubscribleEventListAdd((char *)serverIp,(char *)sid,(char *)subcmd);
	// send http ok reponse packet
	SendSubscribleHttpOk(sockfd,sid,(char *)http_ver);
	
	close(sockfd);

	#ifdef TRENDCHIP
	SendSubscribleNotify(serverport,subcmd,serverIp,sid,(char *)http_ver,call_back_path);
	#else
	SendSubscribleNotify(serverport,subcmd,serverIp,sid,(char *)http_ver);
	#endif
	

	return 0;
}

void handle_subscrible(void *args)
{	
	#ifdef TRENDCHIP
	char call_back_path[64]={0};	
	#endif
	
	struct Subscrible_Event * event = (struct Subscrible_Event *)args;
	#ifdef TRENDCHIP
	SendSubscribleNotify(event->ServerPort,event->Subcmd,event->ServerIp,event->sid,event->http_ver, call_back_path);
	#else
	SendSubscribleNotify(event->ServerPort,event->Subcmd,event->ServerIp,event->sid,event->http_ver);
	#endif
	
		
}











/////////////////////////////////////////////////////////////////////
//	CreateSerialId()
//
//
/////////////////////////////////////////////////////////////////////
void CreateSerialId(char *id)
{
	char * temp;
	int i,j,k;

	temp = id;

	for(k = 0;k<5;k++)
	{
		if(k == 0)
		{
			for(j=0;j<8;j++)
			{
				i = random();
				i = fmod(i,16);
				*temp = serialchar[i];
				temp++;
			}
		}
		else if( k == 4)
		{
			for(j=0;j<12;j++)
			{
				i = random();
				i = fmod(i,16);
				*temp = serialchar[i];
				temp++;
			}
		}
		else 
		{
			for(j=0;j<4;j++)
			{
				i = random();
				i = fmod(i,16);
				*temp = serialchar[i];
				temp++;
			}		
		}
		*temp = '-';
		temp++;
	}	
	temp--;
        *temp = '\0';

}


////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////
#ifdef TRENDCHIP
/*krammer change for bug 857*/
/*____________________________________________________________________*
**	function name: SendSubscribleNotify
**
**	description: 
*     sed a notify for subscribe 
*                                           
**	parameters:
*     port:the port for notify packet
*     subcmd:
*     serverIp:the ip for notify packet
*     sid:
*	httpversion:the httpversion for notify packet
*	call_back_path: the call back path for notify packet
*
**	global:
*     none
**	return: 
*     void
**	call:CreateNotifyPacket
**	revision:
*     1.krammer 
* _____________________________________________________________________*/

void SendSubscribleNotify(int port,char * subcmd,char *serverIp,char *sid,char *httpversion, char *call_back_path)
#else
void SendSubscribleNotify(int port,char * subcmd,char *serverIp,char *sid,char *httpversion)
#endif
{
	struct sockaddr_in  seraddr;
	int sockfd;

	char  notifpacket[2048];
	memset(notifpacket,0,sizeof(notifpacket));
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		if(DEBUG)
			printf("create socket error\n");
		return;
	}
	
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(port);
	seraddr.sin_addr.s_addr = inet_addr(serverIp);
	bzero(&(seraddr.sin_zero),8);
	
	if(connect(sockfd,(struct sockaddr *)&seraddr,sizeof(struct sockaddr)) == -1)
	{
		if(DEBUG)
			printf("can not connect client\n");
		close(sockfd);
		return ;
	}

	#ifdef TRENDCHIP
	//create the  notify packet 
	if(CreateNotifyPacket((char *)notifpacket,subcmd,sid,serverIp,port,httpversion, call_back_path)==-1)
		return ;
	#else
	//create the  notify packet 
	if(CreateNotifyPacket((char *)notifpacket,subcmd,sid,serverIp,port,httpversion)==-1)
		return ;
	#endif
	


	if(send(sockfd,(char *)notifpacket,strlen((char *)notifpacket)+1,0)<0)
	{	if(DEBUG)
			printf("can not send notify packet\n");
		close(sockfd);
		return ;
	}

//here must wait to receiving the client http ok reponse
	if(recv(sockfd,notifpacket,1024,0)<0)
	{
	//here add the error information of the received reponsed packet	
	}
	
	close(sockfd);


}
///////////////////////////////////////////////////////////////////////////////////
// CreateNotifyPacket()
//	contruct notify packet to send to client 
// out:   char * packet;   notify packet to be sent;
// in:    char * cmd;	subscrible command to identify the service
// in:    char * id;           SID: uuid:.........................
//
//////////////////////////////////////////////////////////////////////////////////
#ifdef TRENDCHIP
/*krammer change for bug 857*/
/*____________________________________________________________________*
**	function name: CreateNotifyPacket
**
**	description: 
*     create a notify packet 
*                                           
**	parameters:
*     packet:packet buffer
*     cmd:
*     id:
*     hostip:the ip for notify packet
*	port:the port for notify packet
*	httpversion: the httpversion for notify packet
*	call_back_path:the call back path for notify packet
*
**	global:
*     none
**	return: 
*     0
**	call:CreatePropertySet
**	revision:
*     1.krammer 
* _____________________________________________________________________*/

int CreateNotifyPacket(char *packet,char *cmd,char *id,char *hostip,int port,char *httpversion, char *call_back_path)
#else
int CreateNotifyPacket(char *packet,char *cmd,char *id,char *hostip,int port,char *httpversion)
#endif
{
	char temp[64];
	
	char propertyset[2048];
	memset(temp,0,sizeof(temp));
	memset(propertyset,0,sizeof(propertyset));
	CreatePropertySet(cmd,(char *)propertyset);

	#ifdef TRENDCHIP
	if(strlen(call_back_path)==0){
		strcpy(packet,"NOTIFY /notify ");
	}
	else{
		strcpy(packet, "NOTIFY /");
		strcat(packet, call_back_path);
		strcat(packet, " ");
	}
	#else
	strcpy(packet,"NOTIFY /notify ");
	#endif
	
	strcat(packet,httpversion);
	strcat(packet,"\r\n");

	sprintf((char *)temp,"HOST: %s:%d\r\n",hostip,port);
	strcat(packet,(char *)temp);
	strcat(packet,"CONTENT-TYPE: text/xml\r\n");
	sprintf((char *)temp,"CONTENT-LENGTH: %ld\r\n",strlen((char *)propertyset)+1);
	strcat(packet,(char *)temp);

	strcat(packet,"NT: upnp:event\r\n");
	strcat(packet,"NTS: upnp:propchange\r\n");
	sprintf((char *)temp,"SID: uuid:%s\r\n",id);
	strcat(packet,(char *)temp);
	sprintf((char *)temp,"SEQ: %d\r\n\r\n",0);
	strcat(packet,(char *)temp);

	strcat(packet,(char *)propertyset);
	return 0;	
}


///////////////////////////////////////////////////////////////////////////////////
//  CreatePropertySet(char *cmd,char *propset)
//  in: char *cmd  ;     event cmd id
//  out: char * propset;   the property set string
//
///////////////////////////////////////////////////////////////////////////////////

int CreatePropertySet(char *cmd,char *propset)
{
	char wanip[32];
	char portmaps[32];
	char tmp[32];

	strcpy(propset,"<e:propertyset xmlns:e=\"urn:schemas-upnp-org:event-1-0\">\n");

	if(!strcmp(cmd,EventUrl[EVENT_OSINFO]))
	{
		PropertySet(propset,"OSMajorVersion","5");
		PropertySet(propset,"OSMinorVersion","1");
		PropertySet(propset,"OSBuildNumber","2600");
#if !defined(TCSUPPORT_CD_NEW_GUI) 
		tcapi_get("SysInfo_Entry", "ProductName", tmp);
		PropertySet(propset,"OSMachineName",tmp);
#endif
	}
	else if(!strcmp(cmd,EventUrl[EVENT_WANCOMMONIFC]))
	{
		PropertySet(propset,"PhysicalLinkStatus","Up");
	}
	else if(!strcmp(cmd,EventUrl[EVENT_WANIPCON]))
	{
		getIpAddrFromIfname(WAN_IFNAME,wanip);
		PropertySet(propset,"PossibleConnectionTypes","IP_Routed");
		PropertySet(propset,"ConnectionStatus","Connected");
		PropertySet(propset,"X_Name","IGD-1.0");
		PropertySet(propset,"ExternalIPAddress",wanip);
		sprintf(portmaps,"%d",PortMapNumOfEntries);
		PropertySet(propset,"PortMappingNumberOfEntries",(char *)portmaps);
	}

	strcat(propset,"</e:propertyset>\n");

	return 0;
}

void PropertySet(char * propset,char *item,char *value)
{
	char temp[256];
	sprintf((char *)temp,"<e:property>\n<%s>%s</%s>\n</e:property>\n",item,value,item);
	strcat(propset,(char *)temp);
}





/////////////////////////////////////////////////////////////////////////////////
// 
//
//
/////////////////////////////////////////////////////////////////////////////////

void SendSubscribleHttpOk(int sockfd,char *sid,char *httpversion)
{
	char httpok[256];
	char date[40];
	struct utsname  sys_info;
	char  temp[64];

	currentTmToHttpDate(0,date);

	memset(&sys_info,0x00,sizeof(sys_info));
	uname(&sys_info);	

	strcpy((char *)httpok,httpversion);
	strcat((char *)httpok," 200 OK\r\n");
	strcat((char *)httpok,date);
	sprintf(temp,"SERVER: %s/%s UPnP/1.0 myigd/1.0\r\n",sys_info.sysname,sys_info.release);
	strcat((char *)httpok,temp);
	strcat((char *)httpok,"SID: uuid:");
	strcat((char *)httpok,sid);
	strcat((char *)httpok,"\r\n");

	strcat((char *)httpok,"TIMEOUT: Second-1800\r\n\r\n");

	send(sockfd,(char *)httpok,strlen(httpok),0);
//	close(sockfd);	
	
}







///////////////////////////////////////////////////////////////////////////////
//	GetCallBack() process the get command and reponse it
//args:    in:   int sockfd;
//         in:   char * cmd;   http command request;
//         in:   char * firstline;  first line of the http request
//         in:   char * body;
//
///////////////////////////////////////////////////////////////////////////////

int GetCallBack(int sockfd,char * firstline,char * body)
{
	char  filename[64];
	char   rephead[256];
	char   filecontents[256];
	char   httpversion[16];
	char   temp[64];
	char   time_date[40];
	int    filesize;
	
	char   url[64];
	char   endfile[128];
		
	FILE * fp;	

	struct utsname sys_info;
	struct stat  *filestate;

	filestate = (struct stat *)malloc(sizeof(struct stat));

	memset(filename,0,64);
	memset(&sys_info,0x00,sizeof(sys_info));
	uname(&sys_info);

	if((PharseFirstLine((char *)filename,(char *)httpversion,firstline))== -1)
		return -1; //add the http error reponse packet;
	sprintf(temp,"%s%s",XML_FILE_PATH,filename);
	if(stat((char *)temp,filestate) != 0)
	{
		if(DEBUG)
		{
			printf("can not get the file size\n");
		}
	}

	currentTmToHttpDate(0,time_date);

	//next: open file and construct response message
	//construct the reponse message head
	strcpy((char *)rephead,(char *)httpversion);
	strcat((char *)rephead," 200 OK\r\n");

	strcat((char *)rephead,"CONTENT-TYPE: text/xml\r\n");

	if(!strcmp(filename,XML_DEVICEDESC_FILE))
	{
		sprintf(url,urlbase,lanip,SOAP_PORT);
		sprintf(endfile,endxml,lanip,80);
		filesize = filestate->st_size + strlen(xmlheader) + strlen(url) + strlen(endfile);
		sprintf((char *)temp,"CONTENT-LENGTH: %d\r\n",filesize);
	}
	else
		sprintf((char *)temp,"CONTENT-LENGTH: %ld\r\n",(long)filestate->st_size);

	strcat((char *)rephead,(char *)temp);
//	currentTmToHttpDate(1,time_modif);

//	strcat((char *)rephead,"LAST-MODIFIED: Tue, 01 Apr 2003 01:33:51 GMT\r\n");
	strcat((char *)rephead,time_date);

	sprintf(temp,"SERVER: %s/%s UPnP/1.0 myigd/1.0\r\n",sys_info.sysname,sys_info.release);
	strcat((char *)rephead,temp);
	strcat((char *)rephead,"CONNECTION: close\r\n\r\n"); //end of the repons packet head;

	//free(&filestate);
	free(filestate);//20070216 here
	//read the file and send it;
	sprintf(temp,"%s%s",XML_FILE_PATH,filename);
	if(!(fp =fopen((char *)temp,"r")))
	{
		if(DEBUG)
			printf("can not open the file\n");
		return -1;
	}

	send(sockfd,rephead,strlen(rephead),0); // send the head of the reponse packet 

	
	if(!strcmp(filename,XML_DEVICEDESC_FILE))
	{
		send(sockfd,xmlheader,strlen(xmlheader),0);
		send(sockfd,url,strlen(url),0);
	}

	while(!feof(fp))
	{
		if(!fgets(filecontents,254,fp))
			break;
		send(sockfd,filecontents,strlen(filecontents),0);

	};//end feof while

	if(!strcmp(filename,XML_DEVICEDESC_FILE))
		send(sockfd,endfile,strlen(endfile),0);


	fclose(fp);
	close(sockfd);
	
	return 0;		

}




////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////
int PharseFirstLine(char * result,char *httpversion,char * firstline)
{
//	char pharseresult[64];
	char * temp;

	firstline = strstr(firstline," "); //find the first blank of the firstline


	if(firstline == NULL)
	{
		if(DEBUG)
			printf("can not find xml file\n");
		return -1;
	}
	firstline ++; //skip the blank char 
	firstline ++; //skip the '/' char 
	temp = firstline;
	firstline = strstr(firstline," "); //find the second blank of the firstline


	if(firstline == NULL)
		return -1;	

	*firstline = '\0';

	strcpy((char *)result,temp);

	firstline ++;
	
	strcpy(httpversion,firstline);

	return 0;
		
}





//////////////////////////////////////////////////////////////
//get command from buffer ,and the left buffer copy to body
//in:    char *buffe
//out:   char *cmd
//out:   char * body
//
//////////////////////////////////////////////////////////////

void GetCmd(char *cmd ,char *firstline,char *buffer,char *body,int count)
{

	char * temp;
	int len;
	char c;
	char * interval = "\r\n";
	
	

	//get the first line 
	

	temp = buffer;

	if((buffer = strstr(buffer,interval))== NULL)
	{
		cmd[0] ='\0'; 
		return;
	}
	*buffer = '\0';
	strcpy((char *)firstline ,temp);

	buffer = buffer + strlen(interval);
//copy the left buffer contents to the body buffer
	len = count - strlen(firstline)-strlen(interval); //count the bytes number of the left buffer

	memcpy((char *)body,buffer,len);
	
	//next to get cmd from the firstline buffer

	c = firstline[0];
	switch(c)
	{
		case 'G':
			strcpy((char *)cmd,"GET");
			break;
		case 'P':
			strcpy((char *)cmd,"POST");
			break;
		case 'M':
			if(firstline[2] == 'P')
				strcpy((char *)cmd,"M-POST");
			else if(firstline[2] == 'S')
				strcpy((char *)cmd,"M-SEARCH");
			break;
		case 'S':
			strcpy((char *)cmd,"SUBSCRIBE");
			break;
		case 'U':
			strcpy((char *)cmd,"UNSUBSCRIBE");
			break;
		case 'N':
			strcpy((char *)cmd,"NOTIFY");
			break;
		default:
			cmd[0] = '\0';
	
	}	



}












