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

static int rule_index=0;
static int status_flag=0;
static int deactivated = 0;//steven
static char bu_rule_status[8]={0,0,0,0,0,0,0,0};
static char bu_secure_ip[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static char bu_application[8]={0,0,0,0,0,0,0,0};
static char bu_interface[8]={0,0,0,0,0,0,0,0};


static void exe_drop(char act);
static void acl_exe(char act,char *rule_status,char *secure_ip,char *application,char *interface);


static char* config_get_value(char *src,char*dst,int size,char *item)
{
	char *start,*enter;
	int len;

	dst[0] = '\0';
	start = strstr(src,item);
	if (start == NULL)
		return dst;
	
	start += strlen(item);
	enter = strchr(start,'\n');
	if (enter == NULL)
		return dst;
	
	len = enter-start;
	if(len >= size)
		len = size-1;
	strncpy(dst,start,len);
	dst[len] = '\0';
	
	return dst;
	
}

char *TcWebApiAccess_acl_get(int id)
{
	int i;
	char *tmp;
	
	fprintf(stderr, "TcWebApiAccess_acl_get (id  = %d)\n", id);	
	tmp=(char*)ASP_MALLOC(128);

	switch (id)
	{
		case GB_C_ACL_ACTIVATED:
		       if(strcmp(web_api->access_acl->status,val_def->acl_status_activated)==0)
		        	ASP_OUTPUT("CHECKED");
		    	break;			
		case GB_C_ACL_DEACTIVATED:
		       if(strcmp(web_api->access_acl->status,val_def->acl_status_deactivated)==0)
		       	ASP_OUTPUT("CHECKED");
			break;    	
		case GB_C_ACL_RULEINDEX:
		      	for(i=0;i<16;i++)
		      	{
		      		if(i==rule_index)
		      			sprintf(tmp,"<OPTION SELECTED >%d",i+1);
		      		else
		      			sprintf(tmp,"<OPTION>%d",i+1);
		      		ASP_OUTPUT(tmp);	
		   	}
		    	break;			
		case GB_C_ACL_RULEACTIVEYES:
			if(!strcmp(web_api->access_acl->acl_rule[rule_index]->rule_status,val_def->acl_rule_activated))
		     	   	ASP_OUTPUT("CHECKED");
		    	break;  
		case GB_C_ACL_RULEACTIVENO:
		     	if(!strcmp(web_api->access_acl->acl_rule[rule_index]->rule_status,val_def->acl_rule_deactivated))
		        	ASP_OUTPUT("CHECKED");
			else if(strcmp(web_api->access_acl->acl_rule[rule_index]->rule_status,val_def->acl_rule_activated))
				ASP_OUTPUT("CHECKED");
		    	break;  
		case GB_C_ACL_SECUREIP:
		     	sprintf(tmp,"\"%s\"",web_api->access_acl->acl_rule[rule_index]->secure_ip);
		        ASP_OUTPUT(tmp);
		    	break;  		    	
		case GB_C_ACL_APPLICATION:
		    	if(!strcmp(web_api->access_acl->acl_rule[rule_index]->application,val_def->acl_application_web))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->acl_application_web);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->acl_application_web);
		    	ASP_OUTPUT(tmp);
		    	
		    	if(!strcmp(web_api->access_acl->acl_rule[rule_index]->application,val_def->acl_application_ftp))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->acl_application_ftp);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->acl_application_ftp);
		    	ASP_OUTPUT(tmp);	
		    				
		    	if(!strcmp(web_api->access_acl->acl_rule[rule_index]->application,val_def->acl_application_telnet))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->acl_application_telnet);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->acl_application_telnet);
		    	ASP_OUTPUT(tmp);
		    	
		    	if(!strcmp(web_api->access_acl->acl_rule[rule_index]->application,val_def->acl_application_snmp))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->acl_application_snmp);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->acl_application_snmp);
		    	ASP_OUTPUT(tmp);
		    	
		    	if(!strcmp(web_api->access_acl->acl_rule[rule_index]->application,val_def->acl_application_ping))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->acl_application_ping);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->acl_application_ping);
		    	ASP_OUTPUT(tmp);
		    	
		    	if(!strcmp(web_api->access_acl->acl_rule[rule_index]->application,val_def->acl_application_all))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->acl_application_all);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->acl_application_all);
		    	ASP_OUTPUT(tmp);		    	
		    	
		    	break;  
		case GB_C_ACL_INTERFACE:
		    	if(!strcmp(web_api->access_acl->acl_rule[rule_index]->interface,val_def->acl_application_wan))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->acl_application_wan);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->acl_application_wan);
		    	ASP_OUTPUT(tmp);	
		    			 		
		    	if(!strcmp(web_api->access_acl->acl_rule[rule_index]->interface,val_def->acl_application_lan))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->acl_application_lan);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->acl_application_lan);
		    	ASP_OUTPUT(tmp);	
		    			 		
		    	if(!strcmp(web_api->access_acl->acl_rule[rule_index]->interface,val_def->acl_application_both))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->acl_application_both);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->acl_application_both);
		    	ASP_OUTPUT(tmp);			 		
		 		 		 
		    	break;  			    	
		case GB_C_ACL_LIST:
				for(i=0;i<16;i++)
				{
				  if(strcmp(web_api->access_acl->status,val_def->acl_status_activated)==0)	
					if(strlen(web_api->access_acl->acl_rule[i]->rule_status)>0)
					{
						ASP_OUTPUT("<tr height='30'>");
						sprintf(tmp,"<td width='60'  align=center class='tabdata'><strong><font color='#000000'> %d </font></strong></td>",i+1);
						ASP_OUTPUT(tmp);
						sprintf(tmp,"<td width='60'  align=center class='tabdata'><strong><font color='#000000'> %s </font></strong></td>",web_api->access_acl->acl_rule[i]->rule_status);
						ASP_OUTPUT(tmp);
						sprintf(tmp,"<td width='60'  align=center class='tabdata'><strong><font color='#000000'> %s </font></strong></td>",web_api->access_acl->acl_rule[i]->secure_ip);
						ASP_OUTPUT(tmp);
						sprintf(tmp,"<td width='60'  align=center class='tabdata'><strong><font color='#000000'> %s </font></strong></td>",web_api->access_acl->acl_rule[i]->application);
						ASP_OUTPUT(tmp);
						sprintf(tmp,"<td width='60'  align=center class='tabdata'><strong><font color='#000000'> %s </font></strong></td>",web_api->access_acl->acl_rule[i]->interface);
						ASP_OUTPUT(tmp);
					}
				}
		 
		    	break;  		    	
		    		
		default:break;   	
				
  }
  
  	ASP_FREE(tmp);
	return ASP_OK;
}

int TcWebApiAccess_acl_set(int id,char*value)
{
	char *ret;
	int i;
	int fd;
	int retf;
	char *content,*tmp;
 	struct stat buf;	
	
	fprintf(stderr, "TcWebApiAccess_acl_set (id  = %d, value = %s)\n", id, value);	
	ret=ASP_FORM(value);
	if(ret==NULL)
		return -1;

	fprintf(stderr, "ret = %s\n", ret);
	fprintf(stderr, "web_api->access_acl->status = %s\n", web_api->access_acl->status);
	fprintf(stderr, "val_def->acl_status_activated = %s\n", val_def->acl_status_activated);
	
	if(id==GB_C_ACL_STATUS)
	{
		if(strcmp(web_api->access_acl->status,ret)!=0)
			status_flag=1;
		else
			status_flag=0;	
		fprintf(stderr, "status_flag == %d\n", status_flag);
		strncpy(web_api->access_acl->status,ret,8);	
	  	if(status_flag==1)
	  	{	
	  		fd=open("/etc/acl.conf.bak",O_RDONLY);
			if(fd!=-1)
			{
				tmp=(char *)ASP_MALLOC(128);
				retf = fstat(fd,&buf);
				if (retf == -1)
				{
					close(fd);
					ASP_FREE(tmp);
					return -1;
				}
				content = (char*)ASP_MALLOC(buf.st_size+1);	
				retf = read(fd,content,buf.st_size);
				if (retf <= 0)
				{	
					ASP_FREE(tmp);
					ASP_FREE(content);
					close(fd);
					return -1;
				}	
		
				for(i=0;i<16;i++)
				{
					sprintf(tmp,"ACTIVE%d=",i+1);
					config_get_value(content,web_api->access_acl->acl_rule[i]->rule_status,8,tmp);
					sprintf(tmp,"SECUREIP%d=",i+1);
					config_get_value(content,web_api->access_acl->acl_rule[i]->secure_ip,16,tmp);			
					sprintf(tmp,"APPLICATION%d=",i+1);
					config_get_value(content,web_api->access_acl->acl_rule[i]->application,8,tmp);
					sprintf(tmp,"INTERFACE%d=",i+1);
					config_get_value(content,web_api->access_acl->acl_rule[i]->interface,8,tmp);
				}	
#if 0//steven
				for(i=0;i<16;i++)
				{
					if(strlen(web_api->access_acl->acl_rule[i]->rule_status)>0)
						acl_exe('A',web_api->access_acl->acl_rule[i]->rule_status,web_api->access_acl->acl_rule[i]->secure_ip,web_api->access_acl->acl_rule[i]->application,web_api->access_acl->acl_rule[i]->interface);	
				}
#endif
				ASP_FREE(content);	
				ASP_FREE(tmp);	
	    		}
			close(fd); 	
	  	}		
	}
	if(!strcmp(web_api->access_acl->status,val_def->acl_status_activated)/*&&!status_flag*/)
	{
		if(id==GB_C_ACL_RULEINDEX)
		{
			rule_index=asp_atoi(ret);
			if(rule_index>0)
				rule_index--;
		}	
		else if(id==GB_C_ACL_RULEACTIVE)
		{
			strncpy(bu_rule_status,web_api->access_acl->acl_rule[rule_index]->rule_status,8);
			
				strncpy(web_api->access_acl->acl_rule[rule_index]->rule_status,ret,8);
		}

		else if(id==GB_C_ACL_SECUREIP)
		{
			strncpy(bu_secure_ip,web_api->access_acl->acl_rule[rule_index]->secure_ip,16);
				strncpy(web_api->access_acl->acl_rule[rule_index]->secure_ip,ret,16);
		}	
		else if(id==GB_C_ACL_APPLICATION)
		{
			strncpy(bu_application,web_api->access_acl->acl_rule[rule_index]->application,8);
				strncpy(web_api->access_acl->acl_rule[rule_index]->application,ret,8);
		}
		else if(id==GB_C_ACL_INTERFACE)
		{
			strncpy(bu_interface,web_api->access_acl->acl_rule[rule_index]->interface,8);
				strncpy(web_api->access_acl->acl_rule[rule_index]->interface,ret,8);
		}			
	}
	return 0;
}

int TcWebApiAccess_acl_execute(int id)
{
	int i;
	int fd;
	int ret;
	char *content,*tmp;
 	struct stat buf;

 	
	fprintf(stderr, "TcWebApiAccess_acl_execute (id  = %d)\n", id);	
 	tmp=(char *)ASP_MALLOC(128);


	if(id==GB_C_ACL_SETTING)  
	{
		if(strcmp(web_api->access_acl->status,val_def->acl_status_deactivated)==0)
		{
			char *buf;
			buf=(char *)ASP_MALLOC(128);
			sprintf(buf,"/bin/mv /etc/acl.conf /etc/acl.conf.bak");
			asp_execute(buf);
#if 0
			fd=open("/etc/acl.conf",O_RDONLY);
			if(fd!=-1)
			{	
				ret = fstat(fd,&buf);
				if (ret != -1)
				{
					content = (char*)ASP_MALLOC(buf.st_size+1);	
					ret = read(fd,content,buf.st_size);
					close(fd);
					if(ret>0)
					{
						fd=open("/etc/acl.conf.bak",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
						write(fd,content,strlen(content));
						close(fd);
					}
					ASP_FREE(content);
				}
				else
					close(fd);
			}
			else
				close(fd);
			remove("/etc/acl.conf");
#endif				
			for(i=0;i<16;i++)
			{	
				if(strlen(web_api->access_acl->acl_rule[i]->rule_status)>0)
				{
					acl_exe('D',web_api->access_acl->acl_rule[i]->rule_status,web_api->access_acl->acl_rule[i]->secure_ip,web_api->access_acl->acl_rule[i]->application,web_api->access_acl->acl_rule[i]->interface);
/*					sprintf(buf, "/var/script/acl.sh del %s %s %s %s", 
						    web_api->access_acl->acl_rule[i]->rule_status,
						    web_api->access_acl->acl_rule[i]->secure_ip,
						    web_api->access_acl->acl_rule[i]->application,
						    web_api->access_acl->acl_rule[i]->interface);
					asp_execute(buf);*/
				}
				memset(web_api->access_acl->acl_rule[i]->rule_status,0,sizeof(web_api->access_acl->acl_rule[i]->rule_status));
				memset(web_api->access_acl->acl_rule[i]->secure_ip,0,sizeof(web_api->access_acl->acl_rule[i]->secure_ip));
				memset(web_api->access_acl->acl_rule[i]->application,0,sizeof(web_api->access_acl->acl_rule[i]->application));
				memset(web_api->access_acl->acl_rule[i]->interface,0,sizeof(web_api->access_acl->acl_rule[i]->interface));
			}
			if(status_flag==1)
				exe_drop('D');
			deactivated = 1;//steven
			
		}
		else if(strcmp(web_api->access_acl->status,val_def->acl_status_activated)==0)
		{
			remove("/etc/acl.conf.bak");
			if(strlen(bu_rule_status)>0)
			{
				acl_exe('D',bu_rule_status,bu_secure_ip,bu_application,bu_interface);
/*				char *buf;
				buf=(char *)ASP_MALLOC(128);
				sprintf(buf, "/var/script/acl.sh del %s %s %s %s", 
					   web_api->access_acl->acl_rule[i]->rule_status,
					   web_api->access_acl->acl_rule[i]->secure_ip,
					   web_api->access_acl->acl_rule[i]->application,
					   web_api->access_acl->acl_rule[i]->interface);
				asp_execute(buf);*/
			}
#if 1//steven
			if (deactivated == 1)
			{
				for(i=0;i<16;i++)
				{
					if (i == rule_index)
						continue;
					if(strlen(web_api->access_acl->acl_rule[i]->rule_status)>0)
						acl_exe('A',web_api->access_acl->acl_rule[i]->rule_status,web_api->access_acl->acl_rule[i]->secure_ip,web_api->access_acl->acl_rule[i]->application,web_api->access_acl->acl_rule[i]->interface);	
				}
				deactivated = 0;
			}
#endif
			if(strlen(web_api->access_acl->acl_rule[rule_index]->rule_status)>0)
			{
				acl_exe('A',web_api->access_acl->acl_rule[rule_index]->rule_status,web_api->access_acl->acl_rule[rule_index]->secure_ip,web_api->access_acl->acl_rule[rule_index]->application,web_api->access_acl->acl_rule[rule_index]->interface);	
/*				char *buf;
				buf=(char *)ASP_MALLOC(128);
				sprintf(buf, "/var/script/acl.sh add %s %s %s %s", 
					   web_api->access_acl->acl_rule[i]->rule_status,
					   web_api->access_acl->acl_rule[i]->secure_ip,
					   web_api->access_acl->acl_rule[i]->application,
					   web_api->access_acl->acl_rule[i]->interface);
				asp_execute(buf);*/
			}
			remove("/etc/acl.conf");
			fd=open("/etc/acl.conf",O_WRONLY|O_CREAT|O_APPEND,S_IREAD|S_IWRITE);
			for(i=0;i<16;i++)
			{
			  if(strlen(web_api->access_acl->acl_rule[i]->rule_status)>0)
			  {	
				sprintf(tmp,"ACTIVE%d=%s\n",i+1,web_api->access_acl->acl_rule[i]->rule_status);
				write(fd,tmp,strlen(tmp));
				sprintf(tmp,"SECUREIP%d=%s\n",i+1,web_api->access_acl->acl_rule[i]->secure_ip);
				write(fd,tmp,strlen(tmp));
				sprintf(tmp,"APPLICATION%d=%s\n",i+1,web_api->access_acl->acl_rule[i]->application);
				write(fd,tmp,strlen(tmp));
				sprintf(tmp,"INTERFACE%d=%s\n",i+1,web_api->access_acl->acl_rule[i]->interface);	
				write(fd,tmp,strlen(tmp));			
			  }					
			}
			close(fd);
		}
	
	}
	else if(id==GB_C_ACL_DELETE)
	{
	
			remove("/etc/acl.conf.bak");
			if(strlen(web_api->access_acl->acl_rule[rule_index]->rule_status)>0)
				acl_exe('D',web_api->access_acl->acl_rule[rule_index]->rule_status,web_api->access_acl->acl_rule[rule_index]->secure_ip,web_api->access_acl->acl_rule[rule_index]->application,web_api->access_acl->acl_rule[rule_index]->interface);
				
			memset(web_api->access_acl->acl_rule[rule_index]->rule_status,0,sizeof(web_api->access_acl->acl_rule[rule_index]->rule_status));
			memset(web_api->access_acl->acl_rule[rule_index]->secure_ip,0,sizeof(web_api->access_acl->acl_rule[rule_index]->secure_ip));			
			memset(web_api->access_acl->acl_rule[rule_index]->application,0,sizeof(web_api->access_acl->acl_rule[rule_index]->application));			
			memset(web_api->access_acl->acl_rule[rule_index]->interface,0,sizeof(web_api->access_acl->acl_rule[rule_index]->interface));			
		
				
			remove("/etc/acl.conf");
			fd=open("/etc/acl.conf",O_WRONLY|O_CREAT|O_APPEND,S_IREAD|S_IWRITE);
			for(i=0;i<16;i++)
			{
			  if(strlen(web_api->access_acl->acl_rule[i]->rule_status)>0)
			  {	
				sprintf(tmp,"ACTIVE%d=%s\n",i+1,web_api->access_acl->acl_rule[i]->rule_status);
				write(fd,tmp,strlen(tmp));
				sprintf(tmp,"SECUREIP%d=%s\n",i+1,web_api->access_acl->acl_rule[i]->secure_ip);
				write(fd,tmp,strlen(tmp));
				sprintf(tmp,"APPLICATION%d=%s\n",i+1,web_api->access_acl->acl_rule[i]->application);
				write(fd,tmp,strlen(tmp));
				sprintf(tmp,"INTERFACE%d=%s\n",i+1,web_api->access_acl->acl_rule[i]->interface);	
				write(fd,tmp,strlen(tmp));			
			  }					
			}
			close(fd);
	}
	
	ASP_FREE(tmp);
	
    return 0;
}

static void exe_drop(char act)
{
	char *buf;
	buf=(char *)ASP_MALLOC(128);
	sprintf(buf,"iptables -%c INPUT -p TCP --destination-port 80 -j DROP",act);
	asp_execute(buf);
	sprintf(buf,"iptables -%c INPUT -p TCP --destination-port 21 -j DROP",act);
	asp_execute(buf);
	sprintf(buf,"iptables -%c INPUT -p TCP --destination-port 23 -j DROP",act);		
	asp_execute(buf);			
	sprintf(buf,"iptables -%c INPUT -p UDP --destination-port 161 -j DROP",act);
	asp_execute(buf);		
	sprintf(buf,"iptables -%c INPUT -p ICMP -j DROP",act);
	asp_execute(buf);
	ASP_FREE(buf);				
}

static void acl_exe(char act,char *rule_status,char *secure_ip,char *application,char *interface)
{
	char *buf,*INTERFACE,*PROTOCOL,*IPADDR,*PORT,*ACTIVE;
	buf=(char *)ASP_MALLOC(128);
	INTERFACE=(char *)ASP_MALLOC(8);
	PROTOCOL=(char *)ASP_MALLOC(8);
	PORT=(char *)ASP_MALLOC(32);
	ACTIVE=(char *)ASP_MALLOC(8);
	IPADDR=(char *)ASP_MALLOC(20);
	
	if(strcmp(rule_status,val_def->acl_rule_activated)==0)
		strncpy(ACTIVE,"ACCEPT",8);
	else if(strcmp(rule_status,val_def->acl_rule_deactivated)==0)
		strncpy(ACTIVE,"DROP",8);
	else
		memset(ACTIVE,0,sizeof(ACTIVE));
		
	if(strcmp(application,val_def->acl_application_web)==0)
	{
		strncpy(PROTOCOL,"TCP",8);
		strncpy(PORT,"--destination-port 80",32);
	}
	else if(strcmp(application,val_def->acl_application_ftp)==0)
	{
		strncpy(PROTOCOL,"TCP",8);
		strncpy(PORT,"--destination-port 21",32);		
	}
	else if(strcmp(application,val_def->acl_application_telnet)==0)
	{
		strncpy(PROTOCOL,"TCP",8);
		strncpy(PORT,"--destination-port 23",32);		
	}
	else if(strcmp(application,val_def->acl_application_snmp)==0)
	{
		strncpy(PROTOCOL,"UDP",8);
		strncpy(PORT,"--destination-port 161",32);		
	}
	else if(strcmp(application,val_def->acl_application_ping)==0)
	{
		strncpy(PROTOCOL,"ICMP",8);
		memset(PORT,0,sizeof(PORT));		
	}
	else if(strcmp(application,val_def->acl_application_all)==0)
	{
		strncpy(PROTOCOL,"all",8);
		memset(PORT,0,sizeof(PORT));			
	}		
	else
	{
		memset(PROTOCOL,0,sizeof(PROTOCOL));			
		memset(PORT,0,sizeof(PORT));		
	}
	
	if(strcmp(interface,val_def->acl_application_wan)==0)
#if 0//steven
		strncpy(INTERFACE,"-i nas0",8);
#else
		strncpy(INTERFACE,"-i ! br0",8);
#endif
	else if(strcmp(interface,val_def->acl_application_lan)==0)	
		strncpy(INTERFACE,"-i br0",8);
	else
		memset(INTERFACE,0,sizeof(INTERFACE));
	
	if(strcmp(secure_ip,"0.0.0.0")==0)
		memset(IPADDR,0,sizeof(IPADDR));	
	else	
		sprintf(IPADDR,"-s %s",secure_ip);	
	
	exe_drop('D');
	sprintf(buf,"iptables -%c INPUT %s -p %s %s %s -j %s",act,INTERFACE,PROTOCOL,IPADDR,PORT,ACTIVE);
	asp_execute(buf);
	fprintf(stderr,"---acl_exe:%s",buf);
	exe_drop('A');
	
	ASP_FREE(buf);
	ASP_FREE(INTERFACE);
	ASP_FREE(PROTOCOL);
	ASP_FREE(PORT);
	ASP_FREE(ACTIVE);
	ASP_FREE(IPADDR); 
}
