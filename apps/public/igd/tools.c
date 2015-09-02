#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include "tools.h"

extern  char  WAN_IFNAME[16];

int getsockfd()
{
	int sockfd = -1;
	if(sockfd == -1)
	{
		if((sockfd = socket(AF_INET,SOCK_RAW,IPPROTO_RAW)) == -1)
		{
			printf("can not create raw socket\n");
			return -1;
		}
	}
	return sockfd;
}

int getIpAddrFromIfname(char * ifname,char * ipaddr)
{
	struct ifreq  ifr;
	struct sockaddr_in * saddr;
	int fd;

	fd = getsockfd();
	if(fd >=0)
	{
		strcpy(ifr.ifr_name,ifname);
		ifr.ifr_addr.sa_family = AF_INET;
		if(ioctl(fd,SIOCGIFADDR,&ifr) == 0)
		{
			
			saddr = (struct sockaddr_in *)&ifr.ifr_addr;
			strcpy(ipaddr,inet_ntoa(saddr->sin_addr));
			close(fd);
			return 0;
		}
		else
		{
			strcpy(ipaddr,"0.0.0.0");
			close(fd);
			return -1;
		}
	}
	close(fd);
	return -1;
}

char * getNetIfState(char * ifname)
{
	struct ifreq   ifr;
	int    fd;

	fd = getsockfd();
	if(fd >= 0)
	{
		if (strlen(ifname)<sizeof(ifr.ifr_name))
		{
			strcpy(ifr.ifr_name,ifname);
		}
		ifr.ifr_addr.sa_family = AF_INET;
		if(ioctl(fd,SIOCGIFFLAGS,&ifr)<0)
		{
			close(fd);
			return NULL;
		}
		if(ifr.ifr_flags&IFF_UP)
		{
			close(fd);
			return "Up";
		}
	}
	close(fd);
	return NULL;
}

unsigned long GetTotalCommunicate(int flag)
{
	FILE * fp;
	char  dev[15];
	char  format[128];
	
	unsigned long pkt = 0,total = 0;
	memset(format,0,128);
	switch(flag)
	{
		case PACKETSSENT:
			strcpy(format,"%[^:]:%*u %*u %*u %*u %*u %*u %*u %*u %*u %lu %*u %*u %*u %*u %*u %*u\n");
			break;
		case PACKETSRECV:
			strcpy(format,"%[^:]:%*u %lu %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u\n");
			break;
		case BYTESSENT:
			strcpy(format,"%[^:]:%*u %*u %*u %*u %*u %*u %*u %*u %lu %*u %*u %*u %*u %*u %*u %*u\n");
			break;
		case BYTESRECV:
			strcpy(format,"%[^:]:%lu %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u\n");
	}
	fp = fopen("/proc/net/dev","r");
	if(fp != NULL)
	{
		while(getc(fp) != '\n');
		while(getc(fp) != '\n');

		while(!feof(fp))
		{
			fscanf(fp,format,dev,&pkt);
			if(strcmp(dev,WAN_IFNAME))
				total+=pkt;
		}
		fclose(fp);
	}
	else
		total = 1;
	return total;
}











