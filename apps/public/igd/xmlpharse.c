/****************************************************************************
*	this file read the device description xml file ,and pharsed it
*	
*	written by liuchong 2003.3.19
*****************************************************************************/
#include "xmlpharse.h"
#include "tools.h"
#include "descfile.h"

#define  IGD_CONFIG_FILE    "/etc/igd/igd.conf"


char   lanip[32];
char   LAN_IFNAME[16];
char   WAN_IFNAME[16];

char   nat[16];
char   autoconf[16]; //lee 1-4

extern  int PortMapInit();

struct device_info * deviceinfo[10];

int main(int argc,char * argv[])
{
	FILE * fp;
	FILE * cfp;
	int i;
	char  file[64];
	char  url[64];
	char  igded[16];
	char  igdconf[64];
	char  multiroute[128];

	cfp = fopen("/var/log/upnpd.pid", "wb");
	if(cfp)
       	{
		fprintf(cfp, "%d", getpid());
		fclose(cfp);
       	}

	memset(multiroute,0,sizeof(multiroute));
	memset(nat,0,sizeof(nat));
	memset(LAN_IFNAME,0,sizeof(LAN_IFNAME));
	memset(WAN_IFNAME,0,sizeof(WAN_IFNAME));
	memset(igded,0,sizeof(igded));
	memset(igdconf,0,sizeof(igdconf));	

	if(!(cfp=fopen(IGD_CONFIG_FILE,"r")))
	{
		printf("config file not found\n");
		exit(0);
	}

	fread(igdconf,sizeof(char),64,cfp);
	fclose(cfp);

	pharseigdconf(igdconf,"igd=",igded);


	if(!strcmp(igded,"disabled"))
		exit(0);
	pharseigdconf(igdconf,"lanif=",LAN_IFNAME);
	pharseigdconf(igdconf,"wanif=",WAN_IFNAME);

	pharseigdconf(igdconf,"nat=",nat);
  pharseigdconf(igdconf,"autoconf=",autoconf); //lee 1-4
  if(autoconf[0] == '0')
  {
  		delNatAllPortMap();
  }


//	strcpy(LAN_IFNAME,argv[1]);
//	strcpy(WAN_IFNAME,argv[2]);

// here add the multicast route support
	//sprintf(multiroute,"/sbin/route add -net 224.0.0.0 netmask 240.0.0.0 dev %s",LAN_IFNAME);
	sprintf(multiroute,"/sbin/route add -net 239.0.0.0 netmask 255.0.0.0 dev %s",LAN_IFNAME);
	
	system(multiroute);
	
	getIpAddrFromIfname(LAN_IFNAME,lanip);
	sprintf(url,"http://%s:%d/%s",lanip,SOAP_PORT,XML_DEVICEDESC_FILE);
	
	sprintf(file,"%s%s",XML_FILE_PATH,XML_DEVICEDESC_FILE);
	if(!(fp=fopen(file,"r")))
	{
		printf("can't open file\n");
		exit(0);
	}

	for(i = 0;i<10;i++)
	{
		deviceinfo[i] =(struct device_info *)malloc(sizeof(struct device_info));
		if(pharsexml(fp,deviceinfo[i])!=1)
			break;
#ifdef DEBUG
/*	
		printf("--------------------%d-------------------\n",i);
		printf("device type is:%s\n",(char *)deviceinfo[i]->devicetype);
		printf("device udn is:%s\n",(char *)deviceinfo[i]->udn);
		printf("service type is:%s\n",(char *)deviceinfo[i]->servicetype);
		printf("service id is:%s\n",(char *)deviceinfo[i]->serviceId);
*/
#endif
	}

	DevCount = i;
	fclose(fp);
	PortMapInit();

	for(i = 0;i<3;i++)
	{
	//	send the bye bye packet first
		DeviceAdvertisement(0,deviceinfo[i]->devicetype ,i==0,deviceinfo[i]->udn,"LINUX",url);
		ServiceAdvertisement(0,deviceinfo[i]->udn,deviceinfo[i]->servicetype,"LINUX",url);
	}


	for(i=0;i<3;i++)
	{
		DeviceAdvertisement(1,deviceinfo[i]->devicetype,i==0,deviceinfo[i]->udn,"LINUX",url);
		ServiceAdvertisement(1,deviceinfo[i]->udn,deviceinfo[i]->servicetype,"LINUX",url);
	}

	tScheduleAdvertiseTimeout(1800);

	MiniServerInit(SOAP_PORT);


	return 1;
	
}


void pharseigdconf(char * igdconfbuff,char * target,char *value)
{
	char * temp;

	temp = value;

	if((igdconfbuff = strstr(igdconfbuff,target)) != NULL)
	{
		
		igdconfbuff +=strlen(target);

		while((*igdconfbuff!=' ')&&(*igdconfbuff!='\r')&&(*igdconfbuff!='\n'))
		{
			*temp = *igdconfbuff;
			temp++;
			igdconfbuff++;
		};
		*temp = '\0';
		
	}
	else
		value[0]='\0';
	
}



//////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void tScheduleAdvertiseTimeout(int timeout)
{
	sigset_t   timer_sig;
	struct   itimerval   timer_value;
	timer_value.it_value.tv_sec = timeout;
	timer_value.it_value.tv_usec = 0;
	timer_value.it_interval = timer_value.it_value;

	setitimer(ITIMER_REAL,&timer_value,NULL);
	
	sigemptyset(&timer_sig);
	sigaddset(&timer_sig,SIGALRM);
	signal(SIGALRM,&handler_timer);
	
}

void handler_timer()
{
	int i;
	char url[64];

        sprintf(url,"http://%s:%d/%s",lanip,SOAP_PORT,XML_DEVICEDESC_FILE);
	
	for(i = 0;i<3;i++)
        {
        //      send the bye bye packet first
                DeviceAdvertisement(0,deviceinfo[i]->devicetype ,i==0,deviceinfo[i]->udn,"LINUX",url);
                ServiceAdvertisement(0,deviceinfo[i]->udn,deviceinfo[i]->servicetype,"LINUX",url);
        }

        for(i=0;i<3;i++)
        {
                DeviceAdvertisement(1,deviceinfo[i]->devicetype,i==0,deviceinfo[i]->udn,"LINUX",url);
                ServiceAdvertisement(1,deviceinfo[i]->udn,deviceinfo[i]->servicetype,"LINUX",url);
        }


}


int pharsexml(FILE *fp,struct device_info *deviceinfo)
{
	char xmlstr[128];
	int  flag;
	char * temp;

	flag = 1;

	while(!feof(fp))
	{
		if(!fgets(xmlstr,126,fp))
			return UPNP_INVALID_DEVICE;
		if(flag == 1)
		{
			temp = pharsestr((char *)xmlstr,"<deviceType>","</deviceType>");
			if(temp!=NULL)
			{
				strcpy((char *)deviceinfo->devicetype,temp);
				flag = 2;
			//	break;
			}
		}
		else if(flag == 2)
		{
			temp = pharsestr((char *)xmlstr,"<UDN>","</UDN>");
			if(temp!=NULL)
			{
				strcpy((char *)deviceinfo->udn,temp);
				flag = 3;
			//	break;
			}
		}
		else if(flag == 3)
		{
			temp = pharsestr((char *)xmlstr,"<serviceType>","</serviceType>");
			if(temp!=NULL)
			{
				strcpy((char *)deviceinfo->servicetype,temp);
				flag = 4;
			}
		}
		else if(flag == 4)
		{
			temp = pharsestr((char *)xmlstr,"<serviceId>","</serviceId>");
			if(temp!=NULL)
			{
				strcpy((char *)deviceinfo->serviceId,temp);
				
				return 1;
			}
		}
	};
	
	if(flag != 4)
		return 0;
	return 1;

}

//Find the target string ,and return its value;

char *pharsestr(char *xmlstr,char *target,char *utarget)
{

	char * top;
	char * bottom;

	if((top = strstr(xmlstr,target))==NULL)
		return NULL;

	top = top + strlen(target);
	if((bottom = strstr(xmlstr,utarget))!=NULL)
	{
		*bottom = '\0';
	
		if(DEBUG)
//			printf("liu the device type is:%s\n",top);
		return top;

	}
	return NULL;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function     : int SendDeviceAdvertisement()
//
//
//
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//not used

int SendDeviceAdvertisement(char * devtype,char *sertype,char *udn,char * location,int exp)
{


//		DeviceAdvertisement(devtype,i==0,udn,"LINUX",location,exp);
//		ServiceAdvertisement(udn,sertype,"LINUX",location,exp);
			
		
		
	return 1;
	
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // Function    : int ServiceAdvertisement( char * Udn, char * ServType,char *Server,char * Location,int  Duration)
 // Description : This function creates the advertisement packet based on the input parameter, and send it to the
 //               multicast channel.
 // Parameters  : Server : Os.
 //               Udn : Device UDN
 //               ServType : Service Type.
 //               Location : Loaction of Device description document.
 //               Duration : Life time of this device.
 // Return value: 1 if successfull.
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 int ServiceAdvertisement(int flag, char * Udn, char * ServType,char *Server,char * Location)
  {
       char Mil_Nt[180], Mil_Usn[180];
       char * szReq[1];
       struct sockaddr_in DestAddr;
       int RetVal;


       fflush(stdout);


       szReq[0] = (char*)malloc(2048);
       if (szReq[0] == NULL) return -104 ;

       DestAddr.sin_family = AF_INET;
       DestAddr.sin_addr.s_addr = inet_addr("239.255.255.250");
       DestAddr.sin_port = htons(1900);


       sprintf(Mil_Nt,"%s",ServType);
       sprintf(Mil_Usn,"%s::%s",Udn,ServType);
       CreateServiceRequestPacket(1,flag,szReq[0],Mil_Nt,Mil_Usn,Server,Location);
       RetVal = NewRequestHandler(&DestAddr,1, szReq);


       free(szReq[0]);
       return RetVal;
  }














//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // Function    : int DeviceAdvertisement(int RootDev,char * Udn, char * DevType,char *Server,char * Location,int  Duration)
 // Description : This function creates the device advertisement request based on the input parameter, and send it to the
 //               multicast channel.
 // Parameters  : RootDev : 1 means root device 0 means embedded device.
 //               Udm : Device UDN
 //               DevType : Device Type.
 //               Location : Loaction of Device description document.
 //               Duration : Life time of this device.
 // Return value: 1 if successfull.
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int DeviceAdvertisement(int flag, char * DevType, int RootDev,char * Udn, char *Server, char * Location)
{

    struct sockaddr_in DestAddr;
    char *szReq[3], Mil_Nt[180], Mil_Usn[180];
    int RetVal;


    DestAddr.sin_family = AF_INET;
    DestAddr.sin_addr.s_addr = inet_addr("239.255.255.250");
    DestAddr.sin_port = htons(1900);
    if(RootDev) //If deviceis a root device , here we need to send 3 advertisement or reply
    {
       szReq[0] = (char*)malloc(2048);
       szReq[1] = (char*)malloc(2048);
       szReq[2] = (char*)malloc(2048);

       if (szReq[0] == NULL || szReq[1] == NULL || szReq[2] == NULL) return -1;

       strcpy(Mil_Nt,"upnp:rootdevice");
       sprintf(Mil_Usn,"%s::upnp:rootdevice",Udn);
       CreateServiceRequestPacket(1,flag,szReq[0],Mil_Nt,Mil_Usn,Server,Location);

       sprintf(Mil_Nt,"%s",Udn);
       sprintf(Mil_Usn,"%s",Udn);
       CreateServiceRequestPacket(1,flag,szReq[1],Mil_Nt,Mil_Usn,Server,Location);


       sprintf(Mil_Nt,"%s",DevType);
       sprintf(Mil_Usn,"%s::%s",Udn,DevType);
       CreateServiceRequestPacket(1,flag,szReq[2],Mil_Nt,Mil_Usn,Server,Location);

       RetVal = NewRequestHandler(&DestAddr,3, szReq) ;

       free(szReq[0]);
       free(szReq[1]);
       free(szReq[2]);


    }
    else //If device is not a root device then it is a sub-device., here we need to send 2 advertisement or reply
    {

       szReq[0] = (char*)malloc(2048);
       szReq[1] = (char*)malloc(2048);
       if (szReq[0] == NULL || szReq[1] == NULL ) return -104 ;

       sprintf(Mil_Nt,"%s",Udn);
       sprintf(Mil_Usn,"%s",Udn);
       CreateServiceRequestPacket(1,flag,szReq[0],Mil_Nt,Mil_Usn,Server,Location);


       sprintf(Mil_Nt,"%s",DevType);
       sprintf(Mil_Usn,"%s::%s",Udn,DevType);
       CreateServiceRequestPacket(1,flag,szReq[1],Mil_Nt,Mil_Usn,Server,Location);

       RetVal = NewRequestHandler(&DestAddr,2, szReq);
       free(szReq[0]);
       free(szReq[1]);


     }

    return RetVal;


}


 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // Function    :  void CreateServiceRequestPacket(int Notf,char *RqstBuf,char * NtSt,char *Usn,char *Server,char * S,char * Location,int  Duration)
 // Description : This function creates a HTTP request packet.  Depending on the input parameter it either creates a service advertisement
 //               request or service shutdown request etc.
 // Parameters  : Notf : Specify the type of notification, either advertisement or shutdown etc.
 //               RqstBuf  : Output buffer filled with HTTP statement.
 //               RqstId   : Same ID as send with the request HTTP statement.
 //               ServType : Service Type or category
 //               ServId   :Service unique name or ID
 //               Location : Location URL.
 //               Duration : Service duration in sec.
 // Return value: None
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 void CreateServiceRequestPacket(int Notf,int flag,char *RqstBuf,char * NtSt,char *Usn,char *Server,char * Location)
 {
   char TempBuf[300];
   int  Port = 1900;
   char Date[40];
   struct utsname sys_info;
  
   //Note  Notf=0 means service shutdown, Notf=1 means service advertisement, Notf =2 means reply
   currentTmToHttpDate(0,Date);

   memset(&sys_info,0x00,sizeof(sys_info));
   uname(&sys_info);

   if(Notf ==2)
   {
     strcpy(RqstBuf,"HTTP/1.1 200 OK\r\n");
     sprintf(TempBuf,"CACHE-CONTROL: max-age=%d\r\n",1800);
     strcat(RqstBuf,TempBuf);
     strcat(RqstBuf,Date);
     strcat(RqstBuf,"EXT:\r\n");
     sprintf(TempBuf,"LOCATION: %s\r\n",Location);
     strcat(RqstBuf,TempBuf);
     sprintf(TempBuf,"SERVER: %s/%s UPnP/1.0 myigd/1.0\r\n",
             sys_info.sysname, sys_info.release);
     strcat(RqstBuf,TempBuf);
     sprintf(TempBuf,"ST: %s\r\n",NtSt);
     strcat(RqstBuf,TempBuf);


   }
   else if(Notf ==1)
   {
      strcpy(RqstBuf,"NOTIFY * HTTP/1.1\r\n");
      strcpy(TempBuf,"HOST: ");
      strcat(TempBuf,SSDP_IP);
      sprintf(TempBuf,"%s:%d\r\n",TempBuf,Port);
      strcat(RqstBuf,TempBuf);
      sprintf(TempBuf,"CACHE-CONTROL: max-age=%d\r\n",1800);
      strcat(RqstBuf,TempBuf);
      sprintf(TempBuf,"LOCATION: %s\r\n",Location);
      strcat(RqstBuf,TempBuf);
      sprintf(TempBuf,"NT: %s\r\n",NtSt);
      strcat(RqstBuf,TempBuf);
      if(flag == 1)
     	 strcat(RqstBuf,"NTS: ssdp:alive\r\n");
      else if(flag == 0)
	 strcat(RqstBuf,"NTS: ssdp:byebye\r\n");

      sprintf(TempBuf,"SERVER: %s/%s UPnP/1.0 myigd/1.0\r\n",
             sys_info.sysname, sys_info.release);
      strcat(RqstBuf,TempBuf);
   }
   else if(Notf ==0)
   {
      strcpy(RqstBuf,"NOTIFY * HTTP/1.1\r\n");
      strcpy(TempBuf,"HOST: ");
      strcat(TempBuf,SSDP_IP);
      
      sprintf(TempBuf,"%s:%d\r\n",TempBuf,Port);
      strcat(RqstBuf,TempBuf);

      // Following two header is added to interop with Windows Millenium but this is not
      // a part of UPNP spec 1.0 
      sprintf(TempBuf,"CACHE-CONTROL: max-age=%d\r\n",1800);
      strcat(RqstBuf,TempBuf);
      sprintf(TempBuf,"LOCATION: %s\r\n",Location);
      strcat(RqstBuf,TempBuf);


      sprintf(TempBuf,"NT: %s\r\n",NtSt);
      strcat(RqstBuf,TempBuf);
      strcat(RqstBuf,"NTS: ssdp:byebye\r\n");  

   }

   sprintf(TempBuf,"USN: %s\r\n",Usn);
   strcat(RqstBuf,TempBuf);
   strcat(RqstBuf,"\r\n\r\n");

 }


 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // Function    : int NewRequestHandler(struct sockaddr_in * DestAddr, int NumPacket, char ** RqPacket)
 // Description : This function works as a request handler which passes the HTTP request string to multicast channel then
 //               wait for the response, once it received, it is passed back to callback function.
 // Parameters  : RqPacket : Request packet in HTTP format.
 //               DestAddr : Ip address, to send the reply.
 //               NumPacket: Number of packet to be sent.
 // Return value: 1 if successfull.
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 int NewRequestHandler(struct sockaddr_in * DestAddr, int NumPacket, char ** RqPacket)
 {
      int ReplySock, socklen=sizeof( struct sockaddr_in),RetVal,TryIdx=0;
      struct timeval tmout;
      fd_set WrSet;
      int NumCopy,Index;
/*
      Event * Evt = (Event *) malloc(sizeof(Event));
      if ( Evt == NULL)
      {
         return -104;
      }
      else Evt->ErrCode =  0;
*/

      ReplySock = socket(AF_INET, SOCK_DGRAM, 0);

      RetVal = fcntl(ReplySock,F_GETFL,0);
      fcntl(ReplySock,F_SETFL,RetVal|O_NONBLOCK);
      if( ReplySock == -1 || RetVal == -1)
      {
        // SendErrorEvent(-200);
  //      free(Evt);
         return -205;

      }
      for(Index=0;Index< NumPacket;Index++)
      {

          NumCopy =0;
          TryIdx =0;

          while(TryIdx < 3 && NumCopy <2)
          {

             FD_ZERO(&WrSet);
             FD_SET(ReplySock,&WrSet);

             tmout.tv_sec = 1;
             tmout.tv_usec = 1;

             sendto(ReplySock,*(RqPacket+Index),strlen(*(RqPacket+Index)),0,(struct sockaddr *)DestAddr, socklen);
		
             if ( select(ReplySock+1,NULL,&WrSet,NULL,NULL) == -1)
             {
                //if ( errno == EBADF)
		              
                //else if ( errno ==  EINTR)
		              
               // else if ( errno ==  EINVAL )
		              
               // else if ( errno ==   ENOMEM )
		              
          //      SendErrorEvent(-200);
                break;
             }
             else if(FD_ISSET(ReplySock,&WrSet))
             {
               	 ++NumCopy;
             }
             else TryIdx++;
         }
       }

    //   free(Evt);
       close(ReplySock);
       return 0;
 }













//*************************************************************************
//* Name: currentTmToHttpDate
//*
//* Description:  Returns the current date/time in the fixed length format 
//*               of RFC 822, updated by RFC 1123 (as described in HTTP 1.1
//*               http://www.w3.org/Protocols/rfc2616/rfc2616.htm 
//*               format is: 'DATE: Mon, 30 Apr 1979 08:49:37 GMT\r\n\0'
//*               ('\r\n\0' is added for convenience)
//*
//* In:           char * out (space for output , must be at least 38 
//*               characters)
//*
//* Out:          the current date is put in out
//*
//* Return Codes: None
//* Error Codes:  None       
//*************************************************************************

void currentTmToHttpDate(int flag ,char *out)
{
  char *month=NULL;
  char *day=NULL;
  time_t current_time;
  struct tm * current_tm;
  time(&current_time);
  current_tm=gmtime(&current_time);

  switch (current_tm->tm_mon)
    {
    case 0: month="Jan";break;
    case 1: month="Feb";break;
    case 2: month="Mar";break;
    case 3: month="Apr";break;
    case 4: month="May";break;
    case 5: month="Jun";break;
    case 6: month="Jul";break;
    case 7: month="Aug";break;
    case 8: month="Sep";break;
    case 9: month="Oct";break;
    case 10: month="Nov";break;
    case 11: month="Dec";break;
    }
  switch (current_tm->tm_wday)
    {
    case 0:day="Sun";break;
    case 1:day="Mon";break;
    case 2:day="Tue";break;
    case 3:day="Wed";break;
    case 4:day="Thu";break;
    case 5:day="Fri";break;
    case 6:day="Sat";break;
    }
  if(flag ==0)
	  strcpy(out,"DATE: ");
  else if(flag == 1)
	 strcpy(out,"LAST-MODIFIED: ");
  strcat(out,day);
  strcat(out,", ");
  sprintf(&out[strlen(out)],"%02d ",current_tm->tm_mday);
  strcat(out,month);
  sprintf(&out[strlen(out)]," %04d %02d:%02d:%02d GMT\r\n",
	  (current_tm->tm_year+1900), 
	  current_tm->tm_hour,current_tm->tm_min,
	  current_tm->tm_sec);
  
}








