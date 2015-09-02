#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>

#include "trendchip_load.h"
#include "trendchip-def.h"
#include "gb-constants.h"

char *TcWebApiAccess_upnp_get(int id)
{
	switch (id)
	{
		case GB_C_UPNP_ACTIVATED:
		      if(web_api->access_upnp->status==asp_atoi(val_def->upnp_status_activated))
		        ASP_OUTPUT("CHECKED");
		    	break;			
		case GB_C_UPNP_DEACVIVATED:
		      if(web_api->access_upnp->status==asp_atoi(val_def->upnp_status_deactivated))
		      ASP_OUTPUT("CHECKED");
		    	break;    	
		case GB_C_UPNP_AUTOACTIVATED:
		      if(web_api->access_upnp->status==asp_atoi(val_def->upnp_status_activated) && web_api->access_upnp->auto_conf==asp_atoi(val_def->adsl_auto_activated))
		        ASP_OUTPUT("CHECKED");
		    	break;			
		case GB_C_UPNP_AUTODEACTIVATED:
		      if(web_api->access_upnp->status==asp_atoi(val_def->upnp_status_deactivated) || web_api->access_upnp->auto_conf==asp_atoi(val_def->adsl_auto_deactivated) )
		      ASP_OUTPUT("CHECKED");
		    	break;    	    	
		 default :break;   	
				
  }
  
  
	return ASP_OK;
}

int TcWebApiAccess_upnp_set(int id,char*value)
{

  if(id==GB_C_UPNP_STATUS)
     web_api->access_upnp->status=asp_atoi(ASP_FORM("UPnP_active"));
  if(id==GB_C_UPNP_AUTO)
     web_api->access_upnp->auto_conf=asp_atoi(ASP_FORM("UPnP_auto"));   
  
  return 0;   
  
	/*char *ret;
	
	ret =	ASP_FORM(value);
	if(ret ==NULL)
		return -1;
	if(id == GB_C_SNMP_GETCOMMUNITY)
		strncpy(web_api->access_snmp->rocommunity,ret,32);
	else if(id == GB_C_SNMP_SETCOMMUNITY)
		strncpy(web_api->access_snmp->rwcommunity,ret,32);
	return 0;*/
}

int TcWebApiAccess_upnp_execute()
{
    int fd;
	int ret,pid;
	char *content,tmp[128];
	struct stat buf;
	
    fd = open("/var/log/upnpd.pid",O_RDONLY);
	if (fd != -1)
    {

		ret = fstat(fd,&buf);
		if (ret == -1)
			goto exe;
		content = (char*)ASP_MALLOC(buf.st_size+1);	
		ret = read(fd,content,buf.st_size);
		if (ret <= 0)
		{	
			ASP_FREE(content);
			goto exe;
		}
		pid=asp_atoi(content);
		kill(pid,SIGABRT);
		close(fd);
		ASP_FREE(content);
	}

exe:
	if(web_api->access_upnp->status==1)
   	{
   		//lee 1-3 igd.conf wanif=ppp0 (pppoe/pppoa) or wanif =nas0
   		fd = open("/etc/igd/igd.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
   		if(fd !=-1){
   			int i;
   			if(web_api->access_upnp->auto_conf==asp_atoi(val_def->adsl_auto_activated))
   				sprintf(tmp,"autoconf=1\n");
   			else
   				sprintf(tmp,"autoconf=0\n");
   			write(fd,tmp,strlen(tmp));
   			sprintf(tmp,"igd=enable\n");
   			write(fd,tmp,strlen(tmp));
   			sprintf(tmp,"lanif=br0\n");
   			write(fd,tmp,strlen(tmp));
			sprintf(tmp,"wanif=nas0\n"); //default
   			for(i =0; i<8;i++)
   			{
   					if(!strcmp(web_api->wan[i]->isp,val_def->wan_pppoe) && !strcmp(web_api->wan[i]->status,val_def->wan_activated))
   					{
   						sprintf(tmp,"wanif=ppp%d\n",i);
   						break;			
   					}
					else if(!strcmp(web_api->wan[i]->status,val_def->wan_activated))
					{
						sprintf(tmp,"wanif=nas%d\n",i);
   						break;			
   					}
   					
   			}
   			write(fd,tmp,strlen(tmp));
   			sprintf(tmp,"nat=1\n");
   			write(fd,tmp,strlen(tmp));
   			close(fd);
   		}
   		system("/userfs/bin/upnpd &");
   	}else{
	   	fd = open("/etc/igd/igd.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	   	if(fd != -1){
	   		sprintf(tmp,"autoconf=0\nigd=disabled\nlanif=br0\nwanif=nas0\nnat=0\n");
			write(fd,tmp,strlen(tmp));
			close(fd);
	   	}
   		remove("/etc/upnp.conf");	   		
   	}
	return 0;
}
