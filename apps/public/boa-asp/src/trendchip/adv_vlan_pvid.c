#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "trendchip_load.h"
#include "trendchip-def.h"
#include "gb-constants.h"

static void	write_file()
{
	int fd;
	char *tmp;
	tmp=(char *)ASP_MALLOC(256);
	
	fprintf(stderr,"34534534");
	
	fd=open("/etc/vlan_pvid.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	sprintf(tmp,"ATMVC0=%s\nATMVC1=%s\nATMVC2=%s\nATMVC3=%s\nATMVC4=%s\nATMVC5=%s\nATMVC6=%s\nATMVC7=%s\nETHPORT0=%s\nETHPORT1=%s\nETHPORT2=%s\nETHPORT3=%s\nUSBPORT=%s\n" \
			,web_api->adv_vlan_pvid->atmvc0,web_api->adv_vlan_pvid->atmvc1,web_api->adv_vlan_pvid->atmvc2 \
			,web_api->adv_vlan_pvid->atmvc3,web_api->adv_vlan_pvid->atmvc4,web_api->adv_vlan_pvid->atmvc5 \
			,web_api->adv_vlan_pvid->atmvc6,web_api->adv_vlan_pvid->atmvc7,web_api->adv_vlan_pvid->ethport0 \
			,web_api->adv_vlan_pvid->ethport1,web_api->adv_vlan_pvid->ethport2,web_api->adv_vlan_pvid->ethport1 \
			,web_api->adv_vlan_pvid->usbport);
	write(fd,tmp,strlen(tmp));
	
	
	close(fd);
	ASP_FREE(tmp);	
}

static void gonext()
{

	
		ASP_OUTPUT("<html><meta http-equiv='refresh' content=0;URL='/cgi-bin/adv_vlan_group.asp'></html>");	
	
}

char *TcWebApiAdv_vlan_pvid_get(int id)
{
	char tmp[4];
	switch(id)
	{
		case GB_C_VLAN_ATMPVID0:
			sprintf(tmp,"\"%s\"",web_api->adv_vlan_pvid->atmvc0);
			ASP_OUTPUT(tmp);
			break;	
		case GB_C_VLAN_ATMPVID1:
			sprintf(tmp,"\"%s\"",web_api->adv_vlan_pvid->atmvc1);
			ASP_OUTPUT(tmp);
			break;	
		case GB_C_VLAN_ATMPVID2:
			sprintf(tmp,"\"%s\"",web_api->adv_vlan_pvid->atmvc2);
			ASP_OUTPUT(tmp);
			break;	
		case GB_C_VLAN_ATMPVID3:
			sprintf(tmp,"\"%s\"",web_api->adv_vlan_pvid->atmvc3);
			ASP_OUTPUT(tmp);
			break;	
		case GB_C_VLAN_ATMPVID4:
			sprintf(tmp,"\"%s\"",web_api->adv_vlan_pvid->atmvc4);
			ASP_OUTPUT(tmp);
			break;	
		case GB_C_VLAN_ATMPVID5:
			sprintf(tmp,"\"%s\"",web_api->adv_vlan_pvid->atmvc5);
			ASP_OUTPUT(tmp);
			break;	
		case GB_C_VLAN_ATMPVID6:
			sprintf(tmp,"\"%s\"",web_api->adv_vlan_pvid->atmvc6);
			ASP_OUTPUT(tmp);
			break;	
		case GB_C_VLAN_ATMPVID7:
			sprintf(tmp,"\"%s\"",web_api->adv_vlan_pvid->atmvc7);
			ASP_OUTPUT(tmp);
			break;	
			
		case GB_C_VLAN_ETHERNETPVID1:
			sprintf(tmp,"\"%s\"",web_api->adv_vlan_pvid->ethport0);
			ASP_OUTPUT(tmp);
			break;	
		case GB_C_VLAN_ETHERNETPVID2:
			sprintf(tmp,"\"%s\"",web_api->adv_vlan_pvid->ethport1);
			ASP_OUTPUT(tmp);
			break;				
		case GB_C_VLAN_ETHERNETPVID3:
			sprintf(tmp,"\"%s\"",web_api->adv_vlan_pvid->ethport2);
			ASP_OUTPUT(tmp);
			break;	
		case GB_C_VLAN_ETHERNETPVID4:
			sprintf(tmp,"\"%s\"",web_api->adv_vlan_pvid->ethport3);
			ASP_OUTPUT(tmp);
			break;				
			
		case GB_C_VLAN_USBPVID4:
			sprintf(tmp,"\"%s\"",web_api->adv_vlan_pvid->usbport);
			ASP_OUTPUT(tmp);
			break;		
	}
	return ASP_OK;
}

int TcWebApiAdv_vlan_pvid_set(int id,char *value)
{

	char *ret;
	ret=ASP_FORM(value);
	if(ret==NULL)
		return -1;
	switch(id)
	{
		case GB_C_VLAN_ATMPVID0:
			strncpy(web_api->adv_vlan_pvid->atmvc0,ret,2);
			break;	
		case GB_C_VLAN_ATMPVID1:
			strncpy(web_api->adv_vlan_pvid->atmvc1,ret,2);
			break;	
		case GB_C_VLAN_ATMPVID2:
			strncpy(web_api->adv_vlan_pvid->atmvc2,ret,2);
			break;	
		case GB_C_VLAN_ATMPVID3:
			strncpy(web_api->adv_vlan_pvid->atmvc3,ret,2);
			break;	
		case GB_C_VLAN_ATMPVID4:
			strncpy(web_api->adv_vlan_pvid->atmvc4,ret,2);
			break;	
		case GB_C_VLAN_ATMPVID5:
			strncpy(web_api->adv_vlan_pvid->atmvc5,ret,2);
			break;	
		case GB_C_VLAN_ATMPVID6:
			strncpy(web_api->adv_vlan_pvid->atmvc6,ret,2);
			break;	
		case GB_C_VLAN_ATMPVID7:
			strncpy(web_api->adv_vlan_pvid->atmvc7,ret,2);
			break;	
			
		case GB_C_VLAN_ETHERNETPVID1:
			strncpy(web_api->adv_vlan_pvid->ethport0,ret,2);

			break;	
		case GB_C_VLAN_ETHERNETPVID2:
			strncpy(web_api->adv_vlan_pvid->ethport1,ret,2);
			break;				
		case GB_C_VLAN_ETHERNETPVID3:
			strncpy(web_api->adv_vlan_pvid->ethport2,ret,2);
			break;	
		case GB_C_VLAN_ETHERNETPVID4:
			strncpy(web_api->adv_vlan_pvid->ethport3,ret,2);
			break;				
			
		case GB_C_VLAN_USBPVID4:
			strncpy(web_api->adv_vlan_pvid->usbport,ret,2);
			fprintf(stderr,"-----fnext\n");
			write_file();
			fprintf(stderr,"-----snext\n");
			gonext();
			break;					
	}
  
	return 0;
}
