#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

#include <netinet/in.h>
#include <netinet/ip.h>
//#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h> 
#include <sys/types.h> 
#include <sys/time.h>
#include <sys/times.h>

#include "trendchip_load.h"
#include "trendchip-def.h"
#include "gb-constants.h"
#include <errno.h>


static char pvc_name[8];
static int  pvc_num = 0 ;

int sock;
struct sockaddr_in it;
char host[32];
unsigned long lHostIp;
unsigned short seq;

#define ICMP_ECHO 8 /* icmp echo requir */  
#define ICMP_ECHOREPLY 0 /* icmp echo reply */  
#define ICMP_HEADSIZE 8 /* icmp packet header size */  
#define IP_HEADSIZE 20 /* ip packet header size */  
#define u_char unsigned char
#define u_short unsigned short
#define u_long unsigned long

typedef struct tagIpHead /* icmp packet header */  
{  
u_char ip_verlen; /* ip version and ip header lenth*/  
u_char ip_tos; /* ip type of service */  
u_short ip_len; /* ip packet lenghth */  
u_short ip_id; /* ip packet identification */  
u_short ip_fragoff; /* ip packet fragment and offset */  
u_char ip_ttl; /* ip packet time to live */  
u_char ip_proto; /* ip packet protocol type */  
u_short ip_chksum; /* ip packet header checksum */  
u_long ip_src_addr; /* ip source ip adress */  
u_long ip_dst_addr; /* ip destination ip adress */  
} IPHEAD;  


typedef struct tagIcmpHead /* icmp header */  
{  
u_char icmp_type; /* icmp service type */  
/* 8 echo require, 0 echo reply */  
u_char icmp_code; /* icmp header code */  
u_short icmp_chksum; /* icmp header chksum */  
u_short icmp_id; /* icmp packet identification */  
u_short icmp_seq; /* icmp packet sequent */  
u_char icmp_data[1]; /* icmp data, use as pointer */  
} ICMPHEAD;  

void test_ethernet_lan_connection()
{
	int  ret;		
	int  fd ;
	char *buf = NULL;
	
	fd = open("/proc/tc3162/eth_link_st",O_RDONLY);
	if (fd == -1)
		goto err;
	buf = (char *)ASP_MALLOC(128);
	if (NULL == buf)
		goto err; 
	ret = read(fd,buf,127);
	if (ret<=0)
		goto err;
	*(buf+ret)=0;
	
	if (strstr(buf,"Down")== NULL)
		ASP_OUTPUT("Pass");
	else
	 	ASP_OUTPUT("Fail");
	 	
	close(fd);	
	ASP_FREE(buf);
	return;	 	 	

err:
	if (fd != -1)
	close(fd);
	if (buf != NULL)	
	ASP_FREE(buf);
	
	ASP_OUTPUT("Fail");
	return;	 
}
/*remove this unuse function because I use check_line_state() to replace function of it. shnwind 2008.4.17*/  
# if 0
void test_adsl_sync()
{
	int  ret;		
	int  fd ;
	char *buf = NULL;
	
	fd = open("/proc/tc3162/adsl_stats",O_RDONLY);
	if (fd == -1)
		goto err;
	buf = (char *)ASP_MALLOC(128);
	if (NULL == buf)
		goto err; 
	ret = read(fd,buf,127);
	if (ret<=0)
		goto err;
	*(buf+ret)=0;
	
	if (strstr(buf,"up") == NULL) 
		ASP_OUTPUT("Fail");
	else
	 	ASP_OUTPUT("Pass");

	close(fd);	
	ASP_FREE(buf);
	
	return;	 
	 	

err:
	if (fd != -1)
	close(fd);
	if (buf != NULL)	
	ASP_FREE(buf);
	
	ASP_OUTPUT("Fail");	
	return;	 
}
#endif

void test_atm_oam_segment()
{
	char arg[32];
	int fd = -1;
	int ret;	
	if (!strcmp(web_api->wan[pvc_num]->status,val_def->wan_activated))
	{
		fd = open("/proc/tc3162/oam_ping", O_WRONLY);
		if (fd == -1)
			goto err;	
		memset(arg,0,32);						
		sprintf(arg,"%ld %ld 1 0 2",web_api->wan[pvc_num]->vpi,web_api->wan[pvc_num]->vci );
		write(fd,arg,31);
		close(fd);	
		sleep(1);
		
		fd = open("/proc/tc3162/oam_ping",O_RDONLY);
		if (fd == -1)
			goto err;
		memset(arg,0,32);
		ret = read(fd,arg,31);
		if (ret <= 0)
			goto err;	
		if (strstr(arg,"1") == NULL)
			ASP_OUTPUT("Fail");
		else
		 	ASP_OUTPUT("Pass");	
		
		close(fd);	
		return;	 	
	}	
err:
	if (fd != -1)
		close(fd);
	ASP_OUTPUT("Fail");	
	return ;	

}


void test_atm_oam_endtoend()
{
	char arg[32];
	int fd = -1;
	int ret;	
	if (!strcmp(web_api->wan[pvc_num]->status,val_def->wan_activated))
	{
		fd = open("/proc/tc3162/oam_ping", O_WRONLY);
		if (fd == -1)
			goto err;	
		memset(arg,0,32);						
		sprintf(arg,"%ld %ld 1 1 2",web_api->wan[pvc_num]->vpi,web_api->wan[pvc_num]->vci );
		write(fd,arg,31);
		close(fd);	
		sleep(1);
		
		fd = open("/proc/tc3162/oam_ping",O_RDONLY);
		if (fd == -1)
			goto err;
		memset(arg,0,32);
		ret = read(fd,arg,31);
		if (ret <= 0)
			goto err;	
		if (strstr(arg,"1") == NULL)
			ASP_OUTPUT("Fail");
		else
		 	ASP_OUTPUT("Pass");	
		
		close(fd);	
		return;	 
	}			
err:
	if (fd != -1)
		close(fd);
	ASP_OUTPUT("Fail");	
	return ;	

}


static int in_cksum(unsigned short *buf, int sz)
{
	int nleft = sz;
	int sum = 0;
	unsigned short *w = buf;
	unsigned short ans = 0;

	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		*(unsigned char *) (&ans) = *(unsigned char *) w;
		sum += ans;
	}

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	ans = ~sum;
	return (ans);
}




static long time_now() 
{  
	struct timeval now;  
	long lPassed;  

	gettimeofday(&now, 0);  
	lPassed = now.tv_sec * 1000000 + now.tv_usec;  
	
	return lPassed;  
} 

static void noresp()
{
#ifdef ASP_DEBUG
fprintf(stderr,"no response\n");	
#endif	
	return ;
}


static int do_ping()
{
	int size;

	ICMPHEAD *pkt,*pIcmpHead;
	IPHEAD* pIpHead; 

	char pktSend[200],pktRecv[200];
	struct sockaddr_in from;
	int fromlen = sizeof(from);
	
	long lSendTime;
	int delta;
	unsigned char ttl;
	int iPacketSize,iIpHeadLen;

	struct timeval timeout;
	fd_set rset;
	
	seq = 0;
	
	pkt = (struct ICMPHEAD *) pktSend;
	memset(pkt, 0, sizeof(pktSend));
	pkt->icmp_type = ICMP_ECHO;  
	pkt->icmp_code = 0;  
	pkt->icmp_id = seq;  
	pkt->icmp_seq = seq; 
	
	*((long *)pkt->icmp_data) = time_now();   

	iPacketSize = ICMP_HEADSIZE + 4;
	pkt->icmp_chksum = in_cksum((unsigned short *) pkt, iPacketSize);

	lSendTime = time_now();  
	size = sendto(sock,pktSend,iPacketSize, 0,
			   (struct sockaddr *) &it, sizeof(struct sockaddr_in));
	if (size <0)
	{
#ifdef ASP_DEBUG
fprintf(stderr,"Fail 1\n");	
#endif	
		return -1;
  	}
	signal(SIGALRM, noresp);
	alarm(2);

	timeout.tv_sec = 0;
	timeout.tv_usec = 500000;
	FD_ZERO(&rset);
	FD_SET(sock, &rset);
	if(!select(sock + 1, &rset, NULL, NULL, &timeout))
	{
		fprintf(stderr, "Select timeout\n");
		return -1;
	}
	
	size = recvfrom(sock, pktRecv, sizeof(pktRecv), 0,
				(struct sockaddr *) &from, &fromlen);
	if (size == -1)
	{
#ifdef ASP_DEBUG
fprintf(stderr,"Fail 2\n");	
#endif	
		return -1;
	}
	
	delta = (int)((time_now() - lSendTime)/1000);  
	pIpHead = (IPHEAD *)pktRecv;  
	iIpHeadLen = (int)((pIpHead->ip_verlen & 0x0f) << 2);  
	if (size < iIpHeadLen + ICMP_HEADSIZE)
	{
#ifdef ASP_DEBUG
fprintf(stderr,"Fail 3\n");	
#endif		
		return -1;
	}
	ttl = pIpHead->ip_ttl;
	pIcmpHead = (ICMPHEAD *)(pktRecv + iIpHeadLen);  

	if (pIcmpHead->icmp_type != ICMP_ECHOREPLY) 
	{
		//not icmp echo reply packet
#ifdef ASP_DEBUG
fprintf(stderr,"Fail 4\n");	
#endif		
		return -1;
	}
	if (pIcmpHead->icmp_id != seq || pIcmpHead->icmp_seq != seq) 
	{
		//not proper icmp sequent number
#ifdef ASP_DEBUG
fprintf(stderr,"Fail 5\n");	
#endif		
		return -1;
	}

	return 0;	
}

void adsl_ping (char *value)
{
	char *host;
	struct hostent *h;
	char dst_host[32];	
	int val;
	
	//check ip address or host name valid	
//	host = "www.yahoo.com";
  	host = value;
        /* host maybe blank and if blank it will take much time here. shnwind 2008.4.17*/
        if(!strlen(host)){ 
          ASP_OUTPUT("Fail");
          return ;
        }
	sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	val = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, val | O_NONBLOCK);
	memset(&it,0,sizeof(it));						
	it.sin_family = AF_INET;
	
	if ((lHostIp = inet_addr(host)) != INADDR_NONE)
	{
		it.sin_addr.s_addr = lHostIp;  
		strcpy( dst_host, host ); 

	}
	else if ((h = gethostbyname(host)) != NULL )
	{
	
		it.sin_addr = *(struct in_addr *) h->h_addr;
		sprintf( dst_host, "%s", h->h_name);  	
	
	}
	else
	{
		 ASP_OUTPUT("Fail");
		 return;
	} 
#ifdef ASP_DEBUG
fprintf(stderr,"dst_host: %s\n",dst_host);
#endif 

	if (do_ping() == 0)
		 ASP_OUTPUT("Pass");
	else
		 ASP_OUTPUT("Fail");		
	
	return;	 
							
}







void ping_dns_server()
{	
	read_diag_dns(pvc_num);
	adsl_ping(web_api->main_diagnostics->dns_server);		
	return;
}

void ping_yahoo()
{
	adsl_ping("www.yahoo.com");
	return;
}




int TcWebApiDiagnostic_set(int id,char *value)
{
	char * ret = NULL;
	switch (id)
	{
		case GB_C_DIAGNOSTIC_VC:	
			ret = ASP_FORM(value);
			if (ret != NULL)
			{
				memset(pvc_name,0,8);						
				strncpy(pvc_name,ret,strlen(ret));
				pvc_num = asp_atoi(pvc_name + strlen(val_def->diag_vc));
			}
			break;				
	}
	
	return 1;
}	

char *TcWebApiDiagnostic_get(int id)
{
	int i;
	char buf[32];
	switch (id)
	{
		case GB_C_DIAGNOSTIC_VC:				
			for(i = 0 ; i<8 ; i++)
			{
				if(i == pvc_num)
					sprintf(buf,"<OPTION SELECTED >%s%d",val_def->diag_vc,i);
				else
					sprintf(buf,"<OPTION >%s%d",val_def->diag_vc,i);
				ASP_OUTPUT(buf);
			}
			break;	
	
		case GB_C_DIAGNOSTIC_TEST_LAN_CONTNET:
			test_ethernet_lan_connection();		
		break;	
		case GB_C_DIAGNOSTIC_TEST_ADSL_SYNCH:
                  /* at this time, check the ADSL link state is correct. use check_line_state() replace wan_pvc_connect(pvc_num). shnwind 2008.4.17*/
	          if(check_line_state()){
                    ASP_OUTPUT("Pass");
                  }else{
                     ASP_OUTPUT("Fail");
                  }	     
		break;			
		case GB_C_DIAGNOSTIC_TEST_SEG_PING:
			if(!check_line_state()){
				ASP_OUTPUT("Skipped");
			}else{
				test_atm_oam_segment();		
                        }		
		break;	
		case GB_C_DIAGNOSTIC_TEST_END_PING:
			if(!check_line_state()){
				ASP_OUTPUT("Skipped");
			}else{
				test_atm_oam_endtoend();		
                        }		
		break;	
		case GB_C_DIAGNOSTIC_TEST_PING_DNS:
			if(!check_line_state()){
				ASP_OUTPUT("Skipped");
			}else{
				ping_dns_server();	
                        }	
		break;	
		case GB_C_DIAGNOSTIC_TEST_PING_YAHOO:
			if(!check_line_state()){
				ASP_OUTPUT("Skipped");
			}else{
				ping_yahoo();	
                        }	
		break;	
		case GB_C_DIAGNOSTIC_TEST:
			ASP_OUTPUT("Fail");		
		break;			
							
	}		
	return ASP_OK;
}
/*______________________________________________________________________________
**function name:check_line_state
**
**description: 
*     this function is use for checking ADSL is up or down
**parameters: 
*     none
**global:
*     none
**return: 
*     1:up
*     0:down
**call:
*     none
**revision:
*     1.shnwind 20080417
**____________________________________________________________________________*/
int 
check_line_state(){
  char buf[64],*tmp,line_state[4];;
  FILE *pFile;
  
  pFile = fopen("/proc/tc3162/adsl_stats", "r");
	if(pFile){
		while(fgets(buf, 64, pFile)){
			if((tmp=strstr(buf,"ADSL link status:"))){
				 strcpy(line_state,tmp+strlen("ADSL link status:"));
      }
		}
		fclose(pFile);
	}
	if(strstr(line_state,"up")){
    return 1;
  }else{
    return 0;
  }
}

