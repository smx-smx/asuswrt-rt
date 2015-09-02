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

#define SNMPD_ACTIVE  "ACTIVE=\"Yes\""

char * TcWebApiAccess_snmp_get(int id)
{
	char tmp[32],value[32];
	char sign=34;
	switch (id)
	{
		case GB_C_SNMP_GETCOMMUNITY:
		      strncpy(tmp,web_api->access_snmp->rocommunity,32);
			checkQuotationMarks(tmp,strlen(tmp));
		      sprintf(value,"%c%s%c",sign,tmp,sign);
		      ASP_OUTPUT(value);
		    	break;			
		case GB_C_SNMP_SETCOMMUNITY:
		      strncpy(tmp,web_api->access_snmp->rwcommunity,32);
			checkQuotationMarks(tmp,strlen(tmp));
		      sprintf(value,"%c%s%c",sign,tmp,sign);
		      ASP_OUTPUT(value);
		    	break;    	
		 default :break;   	
				
  }
  
  
	return ASP_OK;
}

int TcWebApiAccess_snmp_set(int id,char*value)
{

	char *ret;
	ret =	ASP_FORM(value);
	if(ret ==NULL)
	{
		fprintf(stderr,"TcWebApiAccess_snmp_set get form ASP_FORM Fail!\r\n ");
		return -1;
	}
	if(id == GB_C_SNMP_GETCOMMUNITY)
	{	
		ascii2punct(ret,32);
		strncpy(web_api->access_snmp->rocommunity,ret,32);
		fprintf(stderr,"web_api->access_snmp->rocommunity:%s\t ret:%s\r\n",web_api->access_snmp->rocommunity,ret);
	}
	else if(id == GB_C_SNMP_SETCOMMUNITY)
	{	
		ascii2punct(ret,32);
		strncpy(web_api->access_snmp->rwcommunity,ret,32);
		fprintf(stderr,"web_api->access_snmp->rwcommunity:%s\t ret:%s\r\n",web_api->access_snmp->rwcommunity,ret);
	}
	return 0;
}

int TcWebApiAccess_snmp_execute()
{
	
	int fd;
	int ret;
	char *content,*tmp,*p,sp[64];
	struct stat buf;
	FILE *fp=NULL; /*Update snmp_stat.conf file*/
	
	fd = open("/usr/etc/snmp/snmpd.conf.tmp",O_RDONLY);
	if (fd == -1)
		goto err;
	ret = fstat(fd,&buf);
	if (ret == -1)
		goto err;
	content = (char*)ASP_MALLOC(buf.st_size);
	if(read(fd,content,buf.st_size) <= 0){
		ASP_FREE(content);
		goto err;
	}   
  close(fd);
  
  tmp=strstr(content,"%%ROCOMMUNITY%%");
  memset(tmp,' ',15);
  p=ASP_FORM("SNMP_get");  
  if(p)
  {
  	sprintf(sp,"%s",p);
    	memcpy(tmp,sp,strlen(sp));
  }
//  else
//    *tmp='\n';
//  memcpy(tmp,p,strlen(p));
  
  tmp=strstr(content,"%%RWCOMMUNITY%%");
  memset(tmp,' ',15);
  p=ASP_FORM("SNMP_set");
  if(p)
  {
  	sprintf(sp,"%s",p);
    	memcpy(tmp,sp,strlen(sp));
  }
//  else
//    *tmp='\n';
    
  fd=open("/etc/snmp/snmpd.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
  write(fd,content,strlen(content));  
  close(fd);
  ASP_FREE(content); 	   	
	
 	 /*Create snmp_stat.conf*/
  	fp=fopen(SNMPD_STAT_CONF, "w");
  	if(fp == NULL){
		goto shell;
	}
	else{
		fputs(SNMPD_ACTIVE, fp);
	}
	fclose(fp);
	
	fd=open("/var/log/snmpd.pid",O_RDONLY);
	if(fd!=-1)
	{ 
	  ret = fstat(fd,&buf);
	  if (ret == -1)
	  {
	  	goto shell;
	  }	
  	content = (char*)ASP_MALLOC(buf.st_size+1);	
  	ret = read(fd,content,buf.st_size);
  	if (ret <= 0)
  	{	
  		ASP_FREE(content);
	  	goto shell;
  	}	
	  kill(asp_atoi(content),SIGTERM);
	close(fd);
	ASP_FREE(content);  
	} 
shell:	 
	
	asp_shell("/userfs/bin/snmpd -f -c /etc/snmp/snmpd.conf -P /var/log/snmpd.pid");
	
	return 0;
	
	err:
	fprintf(stderr,"----shell snmp err\n");
	  return -1;
}
