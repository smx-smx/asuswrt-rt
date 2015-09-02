#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>

#include "trendchip_load.h"
#include "trendchip-def.h"
#include "gb-constants.h"
#if 0
static void ddns_exe();
#endif
char *TcWebApiAccess_ddns_get(int id)
{
	char sign=34;
	char tmp[128];
	
	if(id==GB_C_DDNS_ACTIVATED)
	  if(web_api->access_ddns->status==asp_atoi(val_def->ddns_status_activated))
	    ASP_OUTPUT("CHECKED");
	if(id==GB_C_DDNS_DEACTIVATED)
	  if(web_api->access_ddns->status==asp_atoi(val_def->ddns_status_deactivated))
	    ASP_OUTPUT("CHECKED");  
   	if(id==GB_C_DDNS_PROVIDER)
   	{
//   		 if(strcmp(web_api->access_ddns->provider,val_def->ddns_provider_dyndns)==0 || !web_api->access_ddns->provider[0])
//   		 {
   		 		sprintf(tmp,"<OPTION SELECTED >%s",val_def->ddns_provider_dyndns);		
   		 		ASP_OUTPUT(tmp);
//   		 }
#if 0			 
   		 else
   		 {
   		 		sprintf(tmp,"<OPTION >%s",val_def->ddns_provider_dyndns);		
   		        ASP_OUTPUT(tmp);
   		 }       	
   		 
   		 if(strcmp(web_api->access_ddns->provider,val_def->ddns_provider_justlinux)==0)
   		 {
   		 		sprintf(tmp,"<OPTION SELECTED >%s",val_def->ddns_provider_justlinux);		
   		 		ASP_OUTPUT(tmp);
   		 }
   		 else
   		 {
   		 		sprintf(tmp,"<OPTION >%s",val_def->ddns_provider_justlinux);		
   		        ASP_OUTPUT(tmp);
   		 }     	

   		 if(strcmp(web_api->access_ddns->provider,val_def->ddns_provider_dhs)==0)
   		 {
   		 		sprintf(tmp,"<OPTION SELECTED >%s",val_def->ddns_provider_dhs);	
   		 		ASP_OUTPUT(tmp);	
   		 }
   		 else
   		 {
   		 		sprintf(tmp,"<OPTION >%s",val_def->ddns_provider_dhs);		
   		        ASP_OUTPUT(tmp);
   		 }    

   		 if(strcmp(web_api->access_ddns->provider,val_def->ddns_provider_ods)==0)
   		 {
   		 		sprintf(tmp,"<OPTION SELECTED >%s",val_def->ddns_provider_ods);		
   		 		ASP_OUTPUT(tmp);
   		 }
   		 else
   		 {
   		 		sprintf(tmp,"<OPTION >%s",val_def->ddns_provider_ods);		
   		        ASP_OUTPUT(tmp);
   		 }    

   		 if(strcmp(web_api->access_ddns->provider,val_def->ddns_provider_gnudip)==0)
   		 {
   		 		sprintf(tmp,"<OPTION SELECTED >%s",val_def->ddns_provider_gnudip);	
   		 		ASP_OUTPUT(tmp);	
   		 }
   		 else
   		 {
   		 		sprintf(tmp,"<OPTION >%s",val_def->ddns_provider_gnudip);		
   		        ASP_OUTPUT(tmp);
   		 }    
	   		 
	   		 if(strcmp(web_api->access_ddns->provider,val_def->ddns_provider_tzo)==0)
   		 {
   		 		sprintf(tmp,"<OPTION SELECTED >%s",val_def->ddns_provider_tzo);	
   		 		ASP_OUTPUT(tmp);	
   		 }
   		 else
   		 {
   		 		sprintf(tmp,"<OPTION >%s",val_def->ddns_provider_tzo);		
   		        ASP_OUTPUT(tmp);
   		 }    
   		 	   			    
	   		 if(strcmp(web_api->access_ddns->provider,val_def->ddns_provider_easydns)==0)
   		 {
   		 		sprintf(tmp,"<OPTION SELECTED >%s",val_def->ddns_provider_easydns);	
   		 		ASP_OUTPUT(tmp);	
   		 }
   		 else
   		 {
   		 		sprintf(tmp,"<OPTION >%s",val_def->ddns_provider_easydns);		
   		        ASP_OUTPUT(tmp);
   		 }    
#endif	   		

   	} 
   	if(id==GB_C_DDNS_MYHOST)
   	{   
   	   sprintf(tmp,"%c%s%c",sign,web_api->access_ddns->host,sign);
   	   ASP_OUTPUT(web_api->access_ddns->host);
   	}  
   	if(id==GB_C_DDNS_EMAIL)
   	{   
   	   sprintf(tmp,"%c%s%c",sign,web_api->access_ddns->email,sign);
   	   ASP_OUTPUT(web_api->access_ddns->email);
   	}  
   	if(id==GB_C_DDNS_USRNAME)
   	{   
   	   sprintf(tmp,"%c%s%c",sign,web_api->access_ddns->username,sign);
   	   ASP_OUTPUT(web_api->access_ddns->username);
   	}   
   	if(id==GB_C_DDNS_PASSWORD)
   	{   
   	   sprintf(tmp,"%c%s%c",sign,web_api->access_ddns->password,sign);
   	   ASP_OUTPUT(web_api->access_ddns->password);  
   	}   
   	if(id==GB_C_DDNS_WILDCARDYES)
   	   if(strcmp(web_api->access_ddns->wildcard,val_def->ddns_wildcard_yes)==0)
   	     ASP_OUTPUT("CHECKED");    
   	if(id==GB_C_DDNS_WILDCARDNO)
   	   if(strcmp(web_api->access_ddns->wildcard,val_def->ddns_wildcard_no)==0 || !web_api->access_ddns->wildcard[0])
   	     ASP_OUTPUT("CHECKED");         
  
	return ASP_OK;
}

int TcWebApiAccess_ddns_set(int id,char*value)
{
	char *ret;
	
	ret =	ASP_FORM(value);
	if(ret ==NULL)
	   return -1;
  if(id==GB_C_DDNS_STATUS)
     web_api->access_ddns->status=asp_atoi(ret);
    
  if(web_api->access_ddns->status==1)
  {
  	if(id==GB_C_DDNS_PROVIDER)
  	{  
  	   sprintf(web_api->access_ddns->provider,"%s",ret);
  	}  
  	if(id==GB_C_DDNS_MYHOST)
  	{  
	   ascii2punct(ret,128);
  	   sprintf(web_api->access_ddns->host,"%s",ret);  
  	}   
  	if(id==GB_C_DDNS_EMAIL)
  	{
  	   ascii2punct(ret,128);
  	   sprintf(web_api->access_ddns->email,"%s",ret); 
  	}   
  	if(id==GB_C_DDNS_USRNAME)
  	{  
  	   ascii2punct(ret,128);
  	   sprintf(web_api->access_ddns->username,"%s",ret);
  	}   
  	if(id==GB_C_DDNS_PASSWORD)
  	{  
  	   ascii2punct(ret,128);
  	   sprintf(web_api->access_ddns->password,"%s",ret);
  	   fprintf(stderr,"--password--%s\n",web_api->access_ddns->password);  
  	}   
  	if(id==GB_C_DDNS_WILDCARD)
  	{  
  	   sprintf(web_api->access_ddns->wildcard,"%s",ret);
  	}   
  	 
  }
  
	return 0;
}

int TcWebApiAccess_ddns_execute()
{
	int fd;
	char tmp[128];
	int ret,pid;
	/*shnwind modufy 2008.4.27*/
    	fd = open("/var/log/ez-ipupdate.pid",O_RDONLY);
    	if (fd != -1)
    	{	
		ret = read(fd,tmp,128);
		close(fd);
		if (ret > 0)
		{	
		  pid=asp_atoi(tmp);
		  kill(pid,SIGABRT);	
		}		
	}	
	
	if(web_api->access_ddns->status==1)
	{ 
		remove("/etc/ddns.conf"); 
		fd=open("/etc/ddns.conf",O_WRONLY|O_CREAT|O_APPEND,S_IREAD|S_IWRITE);
		 
		sprintf(tmp,"SERVERNAME=%s\n",web_api->access_ddns->provider);
		write(fd,tmp,strlen(tmp)); 
		sprintf(tmp,"MYHOST=%s\n",web_api->access_ddns->host);
		write(fd,tmp,strlen(tmp)); 
		sprintf(tmp,"EMAIL=%s\n",web_api->access_ddns->email);
		write(fd,tmp,strlen(tmp)); 	 
		sprintf(tmp,"USERNAME=%s\n",web_api->access_ddns->username);
		write(fd,tmp,strlen(tmp)); 	
		sprintf(tmp,"PASSWORD=%s\n",web_api->access_ddns->password);
		write(fd,tmp,strlen(tmp)); 	 
		sprintf(tmp,"WILDCARD=%s\n",web_api->access_ddns->wildcard);
		write(fd,tmp,strlen(tmp)); 
		
		close(fd);
#if 0
		ddns_exe();
      		asp_shell("/var/script/ddns_run");   	
#endif			
		asp_execute("/var/script/ddns.sh");
		asp_shell("/var/script/ddns_run.sh &");
      		return 0;
	   
	} 	
	else
	   	remove("/etc/ddns.conf");	

	return 0;
}
#if 0
static void ddns_exe()
{
	int fd;
	char tmp[128];
	remove("/etc/ipupdate.conf");
	fd=open("/etc/ipupdate.conf",O_WRONLY|O_CREAT|O_APPEND,S_IREAD|S_IWRITE);
	sprintf(tmp,"user=%s:%s\n",web_api->access_ddns->username,web_api->access_ddns->password);
	write(fd,tmp,strlen(tmp)); 
	sprintf(tmp,"host=%s\n",web_api->access_ddns->host);
	write(fd,tmp,strlen(tmp)); 
	sprintf(tmp,"cache-file=/tmp/ez-ipupdate.cache\n");	
	write(fd,tmp,strlen(tmp)); 	
	if(strcmp(web_api->access_ddns->wildcard,"Yes")==0)
	{
		sprintf(tmp,"wildcard\n");	
		write(fd,tmp,strlen(tmp)); 		
	}  
	
	 if(strcmp(web_api->access_ddns->provider,val_def->ddns_provider_dyndns)==0)
	 	sprintf(tmp,"service-type=dyndns\ninterface=eth0\nmax-interval=2073600\n");
	 else if(strcmp(web_api->access_ddns->provider,val_def->ddns_provider_justlinux)==0)
	 	sprintf(tmp,"service-type=justlinux\ninterface=eth0\n");
	 else if(strcmp(web_api->access_ddns->provider,val_def->ddns_provider_dhs)==0)
	 	sprintf(tmp,"service-type=dhs\ninterface=eth0\n");
	 else if(strcmp(web_api->access_ddns->provider,val_def->ddns_provider_ods)==0)
	 	sprintf(tmp,"service-type=ods\ninterface=eth0\n");
	 else if(strcmp(web_api->access_ddns->provider,val_def->ddns_provider_gnudip)==0)
	 	sprintf(tmp,"service-type=gnudip\ninterface=eth0\nmax-interval=2073600\n");
	 else if(strcmp(web_api->access_ddns->provider,val_def->ddns_provider_tzo)==0)
	 	sprintf(tmp,"service-type=tzo\ninterface=eth0\nmax-interval=2073600\n");
	 else if(strcmp(web_api->access_ddns->provider,val_def->ddns_provider_easydns)==0)
	 	sprintf(tmp,"service-type=easydns\ninterface=eth0\nmax-interval=2073600\n");
	 else
	    sprintf(tmp,"error:%s\n",web_api->access_ddns->provider);	
	 write(fd,tmp,strlen(tmp)); 	  	
	 close(fd);    	
	 return ;
	 	
}
#endif
