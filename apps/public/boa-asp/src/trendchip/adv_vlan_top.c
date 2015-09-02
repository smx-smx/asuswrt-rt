
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "trendchip_load.h"
#include "trendchip-def.h"
#include "gb-constants.h"

char *TcWebApiAdvVlanTop_get(int id)
{	
	switch (id)
	{
		case GB_C_VLAN_ACTIVATED:				
			if (!strcmp(web_api->vlan_top_status,val_def->vlan_top_activated))
					ASP_OUTPUT("CHECKED");
			break;			
		case GB_C_VLAN_DEACTIVATED:	
			if (!strcmp(web_api->vlan_top_status,val_def->vlan_top_deactivated))
					ASP_OUTPUT("CHECKED");
			break;				
	}						
	return ASP_OK;
}

int TcWebApiAdvVlanTop_set(int id,char *value)
{
	
	char *ret = NULL;
	int fd = -1;	
	
	ret = ASP_FORM(value);
	if (ret ==NULL)
	{
		return -1;
	}

	switch (id)
	{
		case GB_C_VLAN_STATUS:		
			strncpy(web_api->vlan_top_status,ret,16);	
			if (!strcmp(web_api->vlan_top_status,val_def->vlan_top_activated))
			{	
				fd = open("/etc/vlan_pvid.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
				if(fd ==-1)
					return -1;
				//write to file
				write(fd,"1",1);					
				close(fd);
			}
			else
			{
				remove("/etc/vlan_pvid.conf");		
				strncpy(web_api->vlan_top_status,val_def->vlan_top_deactivated,16); 			
			}
			break;	
	}	
	return 0;
}


int TcWebApiAdvVlanTop_write()
{
	int fd = -1;
	if (!strcmp(web_api->vlan_top_status,val_def->vlan_top_activated))
	{	
		fd = open("/etc/vlan_pvid.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
		if(fd ==-1)
			return -1;
		
		if (!strcmp(web_api->vlan_top_status,val_def->vlan_top_activated))
		{
			write(fd,"1",1);
		}						
		close(fd);
	}
	return 0;
}

