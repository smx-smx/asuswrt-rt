#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>

#include   <stddef.h>    
#include   <dirent.h>  

#include "trendchip_load.h"
#include "trendchip-def.h"
#include "gb-constants.h"

static int set_index=0;
static int rule_index=0;
static int list_index=0;
static int urlfilter_url_index = 0;      //wjx 2007.3.6
char ipfilter_check[4];

static int rule_group[]={0,0,0,0,0,0};//wkw 4.18

//wtw del static void ipfilter_exe(char act);
void ipfilter_exe(void); //wtw add
char *ipfilter_extend_get(int id);
int ipfilter_extend_set(int id,char*value);
int ipfilter_extend_exe(int id);

void ipfilter_mac_save();
void ipfilter_mac_delete();

int ipfilter_url_exe(int id);  
int ipfilter_app_exe(int id);  
extern void decode_string (char * des,char * src);

void remove_ipfilter_set();////by wkw 4.15

char *TcWebApiAccess_ipfilter_get(int id)
{
	int i;
	char tmp[64],buf[128];
//	fprintf(stderr,"***filter *%x*\n",id);   
	
	if(id>=GB_C_IPFILTER_EXTEND&&id<=GB_C_IPFILTER_EXTEND_END)	
		return ipfilter_extend_get(id);
	
	switch(id)
	{
		case GB_C_IPFILTER_SETINDEX:
			for(i = 0 ; i < 12 ; i++)
			{
				if(i == set_index)
					sprintf(buf,"<OPTION SELECTED>%d",i+1);
				else
					sprintf(buf,"<OPTION >%d",i+1);
				ASP_OUTPUT(buf);
			}
			break;
		case GB_C_IPFILTER_INTERFACE:
			for(i = 0 ; i<8 ; i++)
			{
				sprintf(tmp,"%s%d",val_def->ipfilter_interface_pvc,i);
				if(strcmp(web_api->access_ipfilter[set_index]->interface,tmp)==0)
					sprintf(buf,"<OPTION SELECTED>%s%d",val_def->ipfilter_interface_pvc,i);
				else
					sprintf(buf,"<OPTION >%s%d",val_def->ipfilter_interface_pvc,i);
				ASP_OUTPUT(buf);
			}
			if(strcmp(web_api->access_ipfilter[set_index]->interface,val_def->ipfilter_interface_lan)==0)
				sprintf(buf,"<OPTION SELECTED>%s",val_def->ipfilter_interface_lan);
			else
				sprintf(buf,"<OPTION >%s",val_def->ipfilter_interface_lan);
			ASP_OUTPUT(buf);
			break;
		case GB_C_IPFILTER_DIRECTION:
			if(strcmp(web_api->access_ipfilter[set_index]->direction,val_def->ipfilter_direction_both)==0)
				sprintf(buf,"<OPTION SELECTED>%s",val_def->ipfilter_direction_both);
			else
				sprintf(buf,"<OPTION >%s",val_def->ipfilter_direction_both);	
			ASP_OUTPUT(buf);
			if(strcmp(web_api->access_ipfilter[set_index]->direction,val_def->ipfilter_direction_incoming)==0)
				sprintf(buf,"<OPTION SELECTED>%s",val_def->ipfilter_direction_incoming);
			else
				sprintf(buf,"<OPTION >%s",val_def->ipfilter_direction_incoming);	
			ASP_OUTPUT(buf);				
			if(strcmp(web_api->access_ipfilter[set_index]->direction,val_def->ipfilter_direction_outgoing)==0)
				sprintf(buf,"<OPTION SELECTED>%s",val_def->ipfilter_direction_outgoing);
			else
				sprintf(buf,"<OPTION >%s",val_def->ipfilter_direction_outgoing);	
			ASP_OUTPUT(buf);		
			break;
		case GB_C_IPFILTER_RULEINDEX:
			for(i = 0 ; i < 6 ; i++)
			{
				if(i == rule_index)
					sprintf(buf,"<OPTION SELECTED>%d",i+1);
				else
					sprintf(buf,"<OPTION >%d",i+1);
				ASP_OUTPUT(buf);
			}			
			break;
		case GB_C_IPFILTER_RULEACTIVEYES:
		//fprintf(stderr,"----ruleactivate:set_index:%d,rule_index:%d,%s\n",set_index,rule_index,val_def->ipfilter_active_yes);
			if(strcmp(web_api->access_ipfilter[set_index]->rule[rule_index]->active,val_def->ipfilter_active_yes)==0)
				ASP_OUTPUT("CHECKED");
			break;
			
		case GB_C_IPFILTER_RULEACTIVENO:
			if(strcmp(web_api->access_ipfilter[set_index]->rule[rule_index]->active,val_def->ipfilter_active_no)==0)
			ASP_OUTPUT("CHECKED");		
			break;
		case GB_C_IPFILTER_SOURCEIP:
			sprintf(buf,"\"%s\"",web_api->access_ipfilter[set_index]->rule[rule_index]->source_ip);
			ASP_OUTPUT(buf);
			break;
		case GB_C_IPFILTER_SOURCENETMASK:
			sprintf(buf,"\"%s\"",web_api->access_ipfilter[set_index]->rule[rule_index]->source_mask);
			ASP_OUTPUT(buf);				
			break;
		case GB_C_IPFILTER_SOURCEPORT:
			sprintf(buf,"\"%s\"",web_api->access_ipfilter[set_index]->rule[rule_index]->source_port);

			ASP_OUTPUT(buf);			
			break;
		case GB_C_IPFILTER_DESTINATIONIP:
			sprintf(buf,"\"%s\"",web_api->access_ipfilter[set_index]->rule[rule_index]->des_ip);
			ASP_OUTPUT(buf);				
			break;
		case GB_C_IPFILTER_DESTINATIONNETMASK:
			sprintf(buf,"\"%s\"",web_api->access_ipfilter[set_index]->rule[rule_index]->des_mask);
			ASP_OUTPUT(buf);				
			break;
		case GB_C_IPFILTER_DESTINATIONPORT:
			sprintf(buf,"\"%s\"",web_api->access_ipfilter[set_index]->rule[rule_index]->des_port);

			ASP_OUTPUT(buf);			
			break;		
		case GB_C_IPFILTER_PROTOCOL:
			if(strcmp(web_api->access_ipfilter[set_index]->rule[rule_index]->protocol,val_def->ipfilter_protocol_tcp)==0)
				sprintf(buf,"<OPTION SELECTED>%s",val_def->ipfilter_protocol_tcp);
			else
				sprintf(buf,"<OPTION >%s",val_def->ipfilter_protocol_tcp);	
			ASP_OUTPUT(buf);
			if(strcmp(web_api->access_ipfilter[set_index]->rule[rule_index]->protocol,val_def->ipfilter_protocol_udp)==0)
				sprintf(buf,"<OPTION SELECTED>%s",val_def->ipfilter_protocol_udp);
			else
				sprintf(buf,"<OPTION >%s",val_def->ipfilter_protocol_udp);	
			ASP_OUTPUT(buf);				
			if(strcmp(web_api->access_ipfilter[set_index]->rule[rule_index]->protocol,val_def->ipfilter_protocol_icmp)==0)
				sprintf(buf,"<OPTION SELECTED>%s",val_def->ipfilter_protocol_icmp);
			else
				sprintf(buf,"<OPTION >%s",val_def->ipfilter_protocol_icmp);	
			ASP_OUTPUT(buf);	
			break;
		/* wtw del case GB_C_IPFILTER_RULEUNMATCHED:
			if(strcmp(web_api->access_ipfilter[set_index]->rule[rule_index]->unmatched,val_def->ipfilter_unmatched_forward)==0)
				sprintf(buf,"<OPTION SELECTED>%s",val_def->ipfilter_unmatched_forward);
			else
				sprintf(buf,"<OPTION >%s",val_def->ipfilter_unmatched_forward);	
			ASP_OUTPUT(buf);				
			if(strcmp(web_api->access_ipfilter[set_index]->rule[rule_index]->unmatched,val_def->ipfilter_unmatched_next)==0)
				sprintf(buf,"<OPTION SELECTED>%s",val_def->ipfilter_unmatched_next);
			else
				sprintf(buf,"<OPTION >%s",val_def->ipfilter_unmatched_next);	
			ASP_OUTPUT(buf);			
			break;*/
		case GB_C_IPFILTER_LISTINDEX:
			for(i = 0 ; i < 12 ; i++)
			{
				if(i == list_index)
					sprintf(buf,"<OPTION SELECTED>%d",i+1);
				else
					sprintf(buf,"<OPTION >%d",i+1);
				ASP_OUTPUT(buf);
			}
			break;		
		case GB_C_IPFILTER_LISTINTERFACE:
			ASP_OUTPUT(web_api->access_ipfilter[list_index]->interface);
			break;
		case GB_C_IPFILTER_LISTDIRECTION:
			ASP_OUTPUT(web_api->access_ipfilter[list_index]->direction);
			break;			
		case GB_C_IPFILTER_LIST:
			for(i=0;i<6;i++)
			{
				sprintf(buf,"<tr><td align=center class='tabdata'> %d </td><td align=center class='tabdata'>",i+1);
				ASP_OUTPUT(buf);
				if(strlen(web_api->access_ipfilter[list_index]->rule[i]->active)>0)
					sprintf(buf,"%s </td><td align=center class='tabdata'>",web_api->access_ipfilter[list_index]->rule[i]->active);//wkw 07.3.7
				else
					sprintf(buf,"- </td><td align=center class='tabdata'>");
				ASP_OUTPUT(buf);	
				
		if(strncmp(web_api->access_ipfilter[list_index]->rule[i]->rule_type,val_def->ipfilter_rule_mac,strlen(val_def->ipfilter_rule_mac))==0)		
		{						
				sprintf(buf,"%s/- </td><td align=center class='tabdata'>",web_api->access_ipfilter[list_index]->rule[i]->mac);
				ASP_OUTPUT(buf);	
		}	
		else
		{
				if(strlen(web_api->access_ipfilter[list_index]->rule[i]->source_ip)>0)
					sprintf(buf,"%s/",web_api->access_ipfilter[list_index]->rule[i]->source_ip);
				else
					sprintf(buf,"-/");
				ASP_OUTPUT(buf);	
				
				if(strlen(web_api->access_ipfilter[list_index]->rule[i]->source_mask)>0)
					sprintf(buf,"%s </td><td align=center class='tabdata'>",web_api->access_ipfilter[list_index]->rule[i]->source_mask);
				else
					sprintf(buf,"- </td><td align=center class='tabdata'>");
				ASP_OUTPUT(buf);	
				
				if(strlen(web_api->access_ipfilter[list_index]->rule[i]->des_ip)>0)
					sprintf(buf,"%s/",web_api->access_ipfilter[list_index]->rule[i]->des_ip);
				else
					sprintf(buf,"-/");
				ASP_OUTPUT(buf);					
		}
				
				if(strlen(web_api->access_ipfilter[list_index]->rule[i]->des_mask)>0)
					sprintf(buf,"%s </td><td align=center class='tabdata'>",web_api->access_ipfilter[list_index]->rule[i]->des_mask);
				else
					sprintf(buf,"- </td><td align=center class='tabdata'>");
				ASP_OUTPUT(buf);	
				
				if(strlen(web_api->access_ipfilter[list_index]->rule[i]->source_port)>0)
					sprintf(buf,"%s </td><td align=center class='tabdata'>",web_api->access_ipfilter[list_index]->rule[i]->source_port);
				else
					sprintf(buf,"- </td><td align=center class='tabdata'>");
				ASP_OUTPUT(buf);	
							
				if(strlen(web_api->access_ipfilter[list_index]->rule[i]->des_port)>0)
					sprintf(buf,"%s </td><td align=center class='tabdata'>",web_api->access_ipfilter[list_index]->rule[i]->des_port);
				else
					sprintf(buf,"- </td><td align=center class='tabdata'>");
				ASP_OUTPUT(buf);
				//wtw add begin
				if(strlen(web_api->access_ipfilter[list_index]->rule[i]->protocol)>0)
					sprintf(buf,"%s </td></tr>",web_api->access_ipfilter[list_index]->rule[i]->protocol);
				else
					sprintf(buf,"- </td></tr>");
				ASP_OUTPUT(buf);	
				//wtw add end
				/*wtw del if(strlen(web_api->access_ipfilter[list_index]->rule[i]->protocol)>0)
					sprintf(buf,"%s </td><td align=center class='tabdata'>",web_api->access_ipfilter[list_index]->rule[i]->protocol);
				else
					sprintf(buf,"- </td><td align=center class='tabdata'>");
				ASP_OUTPUT(buf);	
							
				 wtw del if(strlen(web_api->access_ipfilter[list_index]->rule[i]->unmatched)>0)
					sprintf(buf,"%s </td></tr>",web_api->access_ipfilter[list_index]->rule[i]->unmatched);
				else
					sprintf(buf,"- </td></tr>");
				ASP_OUTPUT(buf);	*/													
			}
			break;				
			
	}
	
	return ASP_OK;
}

int TcWebApiAccess_ipfilter_set(int id,char*value)
{
	char *ret;

	if(id>=GB_C_IPFILTER_EXTEND&&id<=GB_C_IPFILTER_EXTEND_END)
		return ipfilter_extend_set(id,value);
		
	ret = ASP_FORM(value);
	if (ret ==NULL)
		return -1;
	switch(id)
	{
		case GB_C_IPFILTER_SETINDEX:
			set_index=asp_atoi(ret);
			if(set_index>0)
				set_index--;		
		
			break;
		case GB_C_IPFILTER_INTERFACE:
			sprintf(web_api->access_ipfilter[set_index]->interface,"%s",ret);
			break;
		case GB_C_IPFILTER_DIRECTION:
			sprintf(web_api->access_ipfilter[set_index]->direction,"%s",ret);		
			break;
		case GB_C_IPFILTER_RULEINDEX:
		{//wkw 4.18
			int tmp_index=asp_atoi(ret);
			if(tmp_index>0)
				tmp_index--;
			if(tmp_index!=rule_index)
			{
				rule_index=tmp_index;
				rule_group[set_index]=rule_index;
			}
			else
			{
				rule_index=rule_group[set_index];
			}
		}					
			break;			
		case GB_C_IPFILTER_RULEACTIVE:
			sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->active,"%s",ret);	
			break;
		case GB_C_IPFILTER_SOURCEIP:
			sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->source_ip,"%s",ret);			
			break;
		case GB_C_IPFILTER_SOURCENETMASK:
			sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->source_mask,"%s",ret);			
			break;
		case GB_C_IPFILTER_SOURCEPORT:
			sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->source_port,"%s",ret);
			break;		
		case GB_C_IPFILTER_DESTINATIONIP:
			sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->des_ip,"%s",ret);			
			break;
		case GB_C_IPFILTER_DESTINATIONNETMASK:
			sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->des_mask,"%s",ret);			
			break;
		case GB_C_IPFILTER_DESTINATIONPORT:
			sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->des_port,"%s",ret);				
			break;			
		case GB_C_IPFILTER_PROTOCOL:
			sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->protocol,"%s",ret);
			break;
		/*wtw del case GB_C_IPFILTER_RULEUNMATCHED:
			sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->unmatched,"%s",ret);		
			break;*/
		case GB_C_IPFILTER_LISTINDEX:
			list_index=asp_atoi(ret);
			if(list_index>0)
				list_index--;
			break;			
	}
	return 0;
}

int TcWebApiAccess_ipfilter_execute(int id)
{
	int fd;
	char path[64];
	char *buf;
	//wtw add begin
	buf=(char *)ASP_MALLOC(64);
			if(buf==NULL)
				return -1; 
	//wtw add end
	if(id>=GB_C_IPFILTER_EXTEND&&id<=GB_C_IPFILTER_EXTEND_END)
		return ipfilter_extend_exe(id);

	  if(id==GB_C_IPFILTER_SETTING)
	  {		
	  		mkdir("/etc/ipfilter",0777);
	  	/***********ipfilter extend*********/  	
				if(strcmp(ipfilter_check,"2")==0)
				{
					ipfilter_mac_save();
					ipfilter_exe(); //wtw add
					return 0;					
				}	  	
	  	/***********************************/
	  	
			
			//wtw del ipfilter_exe('D');
			
			/* wtw del buf=(char *)ASP_MALLOC(64);
			if(buf==NULL)
				return -1; wtw del*/
			
			sprintf(path,"/etc/ipfilter/ipfilter_%d_0.conf",set_index+1);//wkw 9.21 change
			remove(path);//wkw 07.3.7 change
			fd=open(path,O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
			sprintf(buf,"INTERFACE=\"%s\"\nDIRECTION=\"%s\"\n",web_api->access_ipfilter[set_index]->interface,web_api->access_ipfilter[set_index]->direction);
			write(fd,buf,strlen(buf));
			close(fd);
			
			sprintf(path,"/etc/ipfilter/ipfilter_%d_%d.conf",set_index+1,rule_index+1);//wkw 9.21 change
			remove(path);
			fd=open(path,O_WRONLY|O_CREAT|O_APPEND,S_IREAD|S_IWRITE);
			if(fd)
			{	
				/***********ipfilter extend*********/
				sprintf(buf,"RULE_TYPE=\"%s\"\n",web_api->access_ipfilter[set_index]->rule[rule_index]->rule_type);
				write(fd,buf,strlen(buf));	
				/***********************************/
				sprintf(buf,"ACTIVE=\"%s\"\n",web_api->access_ipfilter[set_index]->rule[rule_index]->active);
				write(fd,buf,strlen(buf));
				sprintf(buf,"SRCIP=\"%s\"\n",web_api->access_ipfilter[set_index]->rule[rule_index]->source_ip);
				write(fd,buf,strlen(buf));
				sprintf(buf,"SRCMASK=\"%s\"\n",web_api->access_ipfilter[set_index]->rule[rule_index]->source_mask);
				write(fd,buf,strlen(buf));
				sprintf(buf,"SRCPORT=\"%s\"\n",web_api->access_ipfilter[set_index]->rule[rule_index]->source_port);
				write(fd,buf,strlen(buf));
				sprintf(buf,"DESIP=\"%s\"\n",web_api->access_ipfilter[set_index]->rule[rule_index]->des_ip);
				write(fd,buf,strlen(buf));
				sprintf(buf,"DESMASK=\"%s\"\n",web_api->access_ipfilter[set_index]->rule[rule_index]->des_mask);
				write(fd,buf,strlen(buf));
				sprintf(buf,"DESPORT=\"%s\"\n",web_api->access_ipfilter[set_index]->rule[rule_index]->des_port);
				write(fd,buf,strlen(buf));
				sprintf(buf,"PROTOCOL=\"%s\"\n",web_api->access_ipfilter[set_index]->rule[rule_index]->protocol);
				write(fd,buf,strlen(buf));
				//wtw del sprintf(buf,"UNMATCHED=\"%s\"\n",web_api->access_ipfilter[set_index]->rule[rule_index]->unmatched);
				//wtw del write(fd,buf,strlen(buf));	
						
				
			}
			close(fd);				
			ASP_FREE(buf);
//wtw del ipfilter_exe('A');
	  }
		
		
	  if(id==GB_C_IPFILTER_DELETE)
	  {	  	
	  	/***********ipfilter extend*********/
			if(strcmp(ipfilter_check,"2")==0)
			{
				ipfilter_mac_delete();
				ipfilter_exe(); //wtw add
				return 0;					
			}	  	
	  	/***********************************/	
	  	//wtw del ipfilter_exe('D');	//wkw 9.25 change
	  	
	  	/***********ipfilter extend*********/	  	
	  	memset(web_api->access_ipfilter[set_index]->rule[rule_index]->rule_type,0,sizeof(web_api->access_ipfilter[set_index]->rule[rule_index]->rule_type));	
			/***********************************/	
//			memset(web_api->access_ipfilter[set_index]->rule[rule_index]->active,0,sizeof(web_api->access_ipfilter[set_index]->rule[rule_index]->active));
			sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->active,"%s",val_def->ipfilter_active_no);	//by wkw 4.15
			memset(web_api->access_ipfilter[set_index]->rule[rule_index]->source_ip,0,sizeof(web_api->access_ipfilter[set_index]->rule[rule_index]->source_ip));
			memset(web_api->access_ipfilter[set_index]->rule[rule_index]->source_mask,0,sizeof(web_api->access_ipfilter[set_index]->rule[rule_index]->source_mask));
			memset(web_api->access_ipfilter[set_index]->rule[rule_index]->source_port,0,sizeof(web_api->access_ipfilter[set_index]->rule[rule_index]->source_port));
			memset(web_api->access_ipfilter[set_index]->rule[rule_index]->des_ip,0,sizeof(web_api->access_ipfilter[set_index]->rule[rule_index]->des_ip));
			memset(web_api->access_ipfilter[set_index]->rule[rule_index]->des_mask,0,sizeof(web_api->access_ipfilter[set_index]->rule[rule_index]->des_mask));		
			memset(web_api->access_ipfilter[set_index]->rule[rule_index]->des_port,0,sizeof(web_api->access_ipfilter[set_index]->rule[rule_index]->des_port));
			memset(web_api->access_ipfilter[set_index]->rule[rule_index]->protocol,0,sizeof(web_api->access_ipfilter[set_index]->rule[rule_index]->protocol));
			//wtw del memset(web_api->access_ipfilter[set_index]->rule[rule_index]->unmatched,0,sizeof(web_api->access_ipfilter[set_index]->rule[rule_index]->unmatched));
			
			sprintf(path,"/etc/ipfilter/ipfilter_%d_%d.conf",set_index+1,rule_index+1);//wkw 9.21 change
	  	remove(path);
	  	
	  	remove_ipfilter_set(); //wkw 4.15		
	  	
	  }
	  ipfilter_exe(); //wtw add
	return 0;
}
//wtw add begin
void ipfilter_exe(void){
	char buf[64];
	sprintf(buf,"/var/script/ipfilter_stop.sh");
	asp_execute(buf);
	sprintf(buf,"/var/script/ipfilter_start.sh");
	asp_execute(buf);
}
//wtw add end
/* wtw del static void ipfilter_exe(char act)
{
	char INTERFACE[4],*DIRECTION,PERMIT[8],*PROTOCOL,*SRCIP,*SRCMASK,*DESIP,*DESMASK,*SPORT,*DPORT,*buf;
	int len;
	SPORT=(char *)ASP_MALLOC(32);
	if(SPORT==NULL)//wkw 3.7
		goto err;
	DPORT=(char *)ASP_MALLOC(32);	
	if(DPORT==NULL)//wkw 3.7
		goto err;
//wtw del	buf=(char *)ASP_MALLOC(32);	
	buf=(char *)ASP_MALLOC(128);	//wtw add
	if(buf==NULL)//wkw 3.7
		goto err;	
	memset(SPORT,0,sizeof(SPORT));
	memset(DPORT,0,sizeof(DPORT));	
		
	len=strlen(val_def->ipfilter_interface_pvc);
	if(web_api->access_ipfilter[set_index]->interface[len]=='0')
		strncpy(INTERFACE,"nas0",4);
	else if(web_api->access_ipfilter[set_index]->interface[len]=='1')
		strncpy(INTERFACE,"nas1",4);		
	else if(web_api->access_ipfilter[set_index]->interface[len]=='2')
		strncpy(INTERFACE,"nas2",4);			
	else if(web_api->access_ipfilter[set_index]->interface[len]=='3')
		strncpy(INTERFACE,"nas3",4);		
	else if(web_api->access_ipfilter[set_index]->interface[len]=='4')
		strncpy(INTERFACE,"nas4",4);		
	else if(web_api->access_ipfilter[set_index]->interface[len]=='5')
		strncpy(INTERFACE,"nas5",4);			
	else if(web_api->access_ipfilter[set_index]->interface[len]=='6')
		strncpy(INTERFACE,"nas6",4);	
	else if(web_api->access_ipfilter[set_index]->interface[len]=='7')
		strncpy(INTERFACE,"nas7",4);			
	else 
		strncpy(INTERFACE,"br0",4);	
		
//	if(strcmp(web_api->access_ipfilter[set_index]->rule[rule_index]->active,val_def->ipfilter_active_yes)==0)
//		sprintf(PERMIT,"ACCEPT");
//	else 
		sprintf(PERMIT,"DROP");		
	
	DIRECTION=web_api->access_ipfilter[set_index]->direction;
	PROTOCOL=web_api->access_ipfilter[set_index]->rule[rule_index]->protocol;
	SRCIP=web_api->access_ipfilter[set_index]->rule[rule_index]->source_ip;
	SRCMASK=web_api->access_ipfilter[set_index]->rule[rule_index]->source_mask;
	DESIP=web_api->access_ipfilter[set_index]->rule[rule_index]->des_ip;
	DESMASK=web_api->access_ipfilter[set_index]->rule[rule_index]->des_mask;
	
	
	if(strcmp(PROTOCOL,val_def->ipfilter_protocol_icmp)!=0)
	{
		if(asp_atoi(web_api->access_ipfilter[set_index]->rule[rule_index]->source_port)!=0)
			sprintf(SPORT,"--source-port %s",web_api->access_ipfilter[set_index]->rule[rule_index]->source_port);
		if(asp_atoi(web_api->access_ipfilter[set_index]->rule[rule_index]->des_port)!=0)
			sprintf(DPORT,"--destination-port %s",web_api->access_ipfilter[set_index]->rule[rule_index]->des_port);
	}

	if(strcmp(DIRECTION,val_def->ipfilter_direction_incoming)==0)	
	{
		sprintf(buf,"iptables -%c IP_FLT -i %s -p %s -j %s -s %s/%s %s -d %s/%s %s",act,INTERFACE,PROTOCOL,PERMIT,SRCIP,SRCMASK,SPORT,DESIP,DESMASK,DPORT);	
		asp_execute(buf);
		fprintf(stderr,"incoming%s\n",buf);
	
	}
	else if(strcmp(DIRECTION,val_def->ipfilter_direction_outgoing)==0)
	{
	sprintf(buf,"iptables -%c IP_FLT -o %s -p %s -j %s -s %s/%s %s -d %s/%s %s0 %s1",act,INTERFACE,PROTOCOL,PERMIT,SRCIP,SRCMASK,SPORT,DESIP,DESMASK,DPORT,DPORT);	
	sprintf(buf,"iptables -%c IP_FLT -o %s -p %s -j %s -s %s/%s %s -d %s/%s %s",act,INTERFACE,PROTOCOL,PERMIT,SRCIP,SRCMASK,SPORT,DESIP,DESMASK,DPORT);	
		asp_execute(buf);		
		fprintf(stderr,"outgoing:%s\n",buf);
	}
	else if(strcmp(DIRECTION,val_def->ipfilter_direction_both)==0)
	{
		sprintf(buf,"iptables -%c IP_FLT -i %s -p %s -j %s -s %s/%s %s -d %s/%s %s",act,INTERFACE,PROTOCOL,PERMIT,SRCIP,SRCMASK,SPORT,DESIP,DESMASK,DPORT);	
		asp_execute(buf);
		fprintf(stderr,"both1:%s\n",buf);
		sprintf(buf,"iptables -%c IP_FLT -o %s -p %s -j %s -s %s/%s %s -d %s/%s %s",act,INTERFACE,PROTOCOL,PERMIT,SRCIP,SRCMASK,SPORT,DESIP,DESMASK,DPORT);	
		asp_execute(buf);	
		fprintf(stderr,"both2:%s\n",buf);
	}
err:
	if(SPORT)//wkw 3.7
		ASP_FREE(SPORT);
	if(DPORT)		//wkw 3.7
		ASP_FREE(DPORT);
	if(buf)			//wkw 3.7
		ASP_FREE(buf);
} wtw del */ 

/*********************************************************************/
char *ipfilter_extend_get(int id)
{
	char *buf;
	buf=(char *)ASP_MALLOC(128);
	if(buf==NULL)//wkw 07.3.7
		return 0;
	int i;
//	fprintf(stderr,"-----ipfilter_extend_get id=%x!------------\n",id);
	switch(id)
	{
		case GB_C_IPFILTER_IPMAC:
			sprintf(buf,"<OPTION selected>%s<OPTION>%s<OPTION>%s</OPTION>",
								val_def->ipfilter_type_ipmac,val_def->ipfilter_type_application,val_def->ipfilter_type_url);
			ASP_OUTPUT(buf);			  
			break;
		case GB_C_IPFILTER_APP:
			sprintf(buf,"<OPTION>%s<OPTION selected>%s<OPTION>%s</OPTION>",
								val_def->ipfilter_type_ipmac,val_def->ipfilter_type_application,val_def->ipfilter_type_url);
			ASP_OUTPUT(buf);				break;
		case GB_C_IPFILTER_URL:
			sprintf(buf,"<OPTION>%s<OPTION>%s<OPTION selected>%s</OPTION>",
								val_def->ipfilter_type_ipmac,val_def->ipfilter_type_application,val_def->ipfilter_type_url);
			ASP_OUTPUT(buf);				break;
		case GB_C_IPFILTER_RULE_TYPE:
			memset(ipfilter_check,0,4);
			if(strcmp(web_api->access_ipfilter[set_index]->rule[rule_index]->rule_type,val_def->ipfilter_rule_ip)==0)
			{	
				sprintf(buf,"<OPTION SELECTED>%s",val_def->ipfilter_rule_ip);
				sprintf(ipfilter_check,"1");
			}
			else
				sprintf(buf,"<OPTION >%s",val_def->ipfilter_rule_ip);	
			ASP_OUTPUT(buf);			
			
			if(strcmp(web_api->access_ipfilter[set_index]->rule[rule_index]->rule_type,val_def->ipfilter_rule_mac)==0)
			{	
				sprintf(buf,"<OPTION SELECTED>%s",val_def->ipfilter_rule_mac);
				sprintf(ipfilter_check,"2");
			}
			else
				sprintf(buf,"<OPTION >%s",val_def->ipfilter_rule_mac);	
			
			ASP_OUTPUT(buf);					
			
			break;
		case GB_C_IPFILTER_MAC:
			sprintf(buf,"\"%s\"",web_api->access_ipfilter[set_index]->rule[rule_index]->mac);
			fprintf(stderr,"*******get mac=%s*\n",buf);
			ASP_OUTPUT(buf);				
			break;	
		case GB_C_IPFILTER_CHECKPAGE:
						
			if(strlen(ipfilter_check)==0)
				sprintf(ipfilter_check,"1");
			ASP_FREE(buf);
			return  ipfilter_check;		
			
		case GB_C_URLFILTER_RULEINDEX_ACTIVATED:              //wjx 2007.3.6
//			if(strcmp(web_api->ipfilter_extend->urlfilter[urlfilter_url_index]->index_status,val_def->urlfilter_activated)==0)
				if(strcmp(web_api->ipfilter_extend->urlfilter_status,val_def->urlfilter_activated)==0)   //wjx 2007.4.13
				ASP_OUTPUT("CHECKED");	
			break;	
		case GB_C_URLFILTER_RULEINDEX_DEACTIVATED:
//			if(strcmp(web_api->ipfilter_extend->urlfilter[urlfilter_url_index]->index_status,val_def->urlfilter_deactivated)==0)
				if(strcmp(web_api->ipfilter_extend->urlfilter_status,val_def->urlfilter_deactivated)==0)			//wjx 2007.4.13	
				ASP_OUTPUT("CHECKED");	
			break;	
		case GB_C_URLFILTER_INDEX:
			for(i = 0 ; i <16 ; i++)
			{
				if(i == urlfilter_url_index)
					sprintf(buf,"<OPTION SELECTED>%d",i+1);
				else
					sprintf(buf,"<OPTION >%d",i+1);
				ASP_OUTPUT(buf);
			}
			break;					
		case GB_C_URLFILTER_URL:
			ASP_OUTPUT(web_api->ipfilter_extend->urlfilter[urlfilter_url_index]->url);
			break;							
		case GB_C_URLFILTER_LIST:
			for(i = 0 ; i <16 ; i++)
			{
//				ASP_OUTPUT("<TR><TD class=tabdata align=middle>");
				sprintf(buf,"<TR><TD class=tabdata align=middle>%d",i+1);
				ASP_OUTPUT(buf);
				ASP_OUTPUT("<TD class=tabdata align=middle width=130>");
				sprintf(buf,"%s</TD></TR><TR>",web_api->ipfilter_extend->urlfilter[i]->url);
				ASP_OUTPUT(buf);
			}
			break;				
			
			/*ipfilter app begin wjx 2007.3.7*/
		case GB_C_APPFILTER_STATUS_ACTIVATED:
			if(strcmp(web_api->ipfilter_extend->appfilter_status,val_def->appfilter_activated)==0)
				ASP_OUTPUT("CHECKED");	
			break;	
		case GB_C_APPFILTER_STATUS_DEACTIVATED:
			if(strcmp(web_api->ipfilter_extend->appfilter_status,val_def->appfilter_deactivated)==0)
				ASP_OUTPUT("CHECKED");	
			break;				
		case GB_C_APPFILTER_ICQ_ALLOW:
			if(strcmp(web_api->ipfilter_extend->appfilter_icq_allowance,val_def->appfilter_icq_allow)==0)
				ASP_OUTPUT("CHECKED");	
			break;				
		case GB_C_APPFILTER_ICQ_DENY:
			if(strcmp(web_api->ipfilter_extend->appfilter_icq_allowance,val_def->appfilter_icq_deny)==0)
				ASP_OUTPUT("CHECKED");	
			break;				
		case GB_C_APPFILTER_MSN_ALLOW:
			if(strcmp(web_api->ipfilter_extend->appfilter_msn_allowance,val_def->appfilter_msn_allow)==0)
				ASP_OUTPUT("CHECKED");	
			break;	
		case GB_C_APPFILTER_MSN_DENY:
			if(strcmp(web_api->ipfilter_extend->appfilter_msn_allowance,val_def->appfilter_msn_deny)==0)
				ASP_OUTPUT("CHECKED");	
			break;				
		case GB_C_APPFILTER_YMSG_ALLOW:
			if(strcmp(web_api->ipfilter_extend->appfilter_ymsn_allowance,val_def->appfilter_ymsn_allow)==0)
				ASP_OUTPUT("CHECKED");	
			break;				
		case GB_C_APPFILTER_YMSG_DENY:
			if(strcmp(web_api->ipfilter_extend->appfilter_ymsn_allowance,val_def->appfilter_ymsn_deny)==0)
				ASP_OUTPUT("CHECKED");	
			break;				
		case GB_C_APPFILTER_RA_ALLOW:
			if(strcmp(web_api->ipfilter_extend->appfilter_ra_allowance,val_def->appfilter_ra_allow)==0)
				ASP_OUTPUT("CHECKED");	
			break;				
		case GB_C_APPFILTER_RA_DENY:
			if(strcmp(web_api->ipfilter_extend->appfilter_ra_allowance,val_def->appfilter_ra_deny)==0)
				ASP_OUTPUT("CHECKED");	
			break;	
			/*ipfilter app end wjx 2007.3.7*/							
	}
	ASP_FREE(buf);
	return 0;
}
int ipfilter_extend_set(int id,char*value)
{
	char *ret;
	ret = ASP_FORM(value);
	if (ret == NULL)
		return -1;
	fprintf(stderr,"----ipfilter_extend_set ret=%s-----\n",ret);    //wjx 2007.3.6
	switch(id)
	{
		case GB_C_IPFILTER_RULE_TYPE:
			sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->rule_type,"%s",ret);
			break;	
		case GB_C_IPFILTER_MAC:
			if(strcmp(ipfilter_check,"2")==0)
				decode_string(web_api->access_ipfilter[set_index]->rule[rule_index]->mac,ret);
			break;						
		case GB_C_URLFILTER_INDEX:                  //wjx 2007.3.6
			urlfilter_url_index = atoi(ret) - 1;
			strncpy(web_api->ipfilter_extend->urlfilter_index,ret,8);
			break;
		case GB_C_URLFILTER_RULEINDEX_ACTIVE:
//			strncpy(web_api->ipfilter_extend->urlfilter[urlfilter_url_index]->index_status,ret,16);
			strncpy(web_api->ipfilter_extend->urlfilter_status,ret,16);
			break;			
		case GB_C_URLFILTER_URL:
			strncpy(web_api->ipfilter_extend->urlfilter[urlfilter_url_index]->url,ret,48);
			break;
			
				
			/*ipfilter app begin wjx 2007.3.7*/
		case GB_C_APPFILTER_STATUS:
			strncpy(web_api->ipfilter_extend->appfilter_status,ret,16);
			break;				
		case GB_C_APPFILTER_ICQ_ALLOWANCE:
			strncpy(web_api->ipfilter_extend->appfilter_icq_allowance,ret,16);
			break;					
		case GB_C_APPFILTER_MSN_ALLOWANCE:
			strncpy(web_api->ipfilter_extend->appfilter_msn_allowance,ret,16);
			break;	
		case GB_C_APPFILTER_YMSG_ALLOWANCE:
			strncpy(web_api->ipfilter_extend->appfilter_ymsn_allowance,ret,16);
			break;					
		case GB_C_APPFILTER_RA_ALLOWANCE:
			strncpy(web_api->ipfilter_extend->appfilter_ra_allowance,ret,16);
			break;		
			/*ipfilter app end wjx 2007.3.7*/	
																		
	}		
	return 0;
}
int ipfilter_extend_exe(int id)
{
	switch (id)
	{
		case GB_C_URLFILTER_SAVE:
		case GB_C_URLFILTER_DELETE:		
			return ipfilter_url_exe(id);
			break;
		case GB_C_APPFILTER_SAVE:	
			return ipfilter_app_exe(id);
			break;	
		default:
			break;			
	}
	return 0;
}

int ipfilter_url_exe(int id)
{
	int fd = -1;
	int i;
	char tmp[32];
	fd=open("/etc/ipfilter/ipfilter_url.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if (fd == -1)
	{
		mkdir("/etc/ipfilter",0777);     //wjx 2007.4.13
		fd=open("/etc/ipfilter/ipfilter_url.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);	
		if (fd == -1)
		{
			return -1;		
		}	
	}	
	if (id == GB_C_URLFILTER_SAVE)
	{
//		fprintf(stderr,"-----urlfilter save config file------\n");
		sprintf(tmp,"urlfilter_index=\"%s\"\n",web_api->ipfilter_extend->urlfilter_index);
		write(fd,tmp,strlen(tmp));
		sprintf(tmp,"urlfilter_active=\"%s\"\n",web_api->ipfilter_extend->urlfilter_status);   //wjx 2007.4.13
		write(fd,tmp,strlen(tmp));		
		
		for(i=0;i<16;i++)
		{
/*			
			sprintf(tmp,"urlfilter_active%d=\"%s\"\n",i,web_api->ipfilter_extend->urlfilter[i]->index_status);
			write(fd,tmp,strlen(tmp));
*/			
			sprintf(tmp,"urlfilter_url%d=\"%s\"\n",i,web_api->ipfilter_extend->urlfilter[i]->url);
			write(fd,tmp,strlen(tmp));						
		}
	}
	else if(id == GB_C_URLFILTER_DELETE)
	{
//		strncpy(web_api->ipfilter_extend->urlfilter[urlfilter_url_index]->index_status,val_def->urlfilter_deactivated,16);
		strncpy(web_api->ipfilter_extend->urlfilter_status,val_def->urlfilter_deactivated,16);
		strncpy(web_api->ipfilter_extend->urlfilter[urlfilter_url_index]->url,"",48);
					
		sprintf(tmp,"urlfilter_index=\"%s\"\n",web_api->ipfilter_extend->urlfilter_index);
		write(fd,tmp,strlen(tmp));		
		sprintf(tmp,"urlfilter_active=\"%s\"\n",web_api->ipfilter_extend->urlfilter_status);   //wjx 2007.4.13
		write(fd,tmp,strlen(tmp));				
		for(i=0;i<16;i++)
		{
/*
			sprintf(tmp,"urlfilter_active%d=\"%s\"\n",i,web_api->ipfilter_extend->urlfilter[i]->index_status);
			write(fd,tmp,strlen(tmp));
*/			
			sprintf(tmp,"urlfilter_url%d=\"%s\"\n",i,web_api->ipfilter_extend->urlfilter[i]->url);
			write(fd,tmp,strlen(tmp));						
		}		
	}
	close(fd);	
	//wtw add begin
	sprintf(tmp,"/var/script/urlfilter_stop.sh");
	asp_execute(tmp);
	sprintf(tmp,"/var/script/urlfilter_start.sh");
	asp_execute(tmp);
	//wtw add end
	return 0;		
} 



int ipfilter_app_exe(int id)       //wjx 2007.3.7
{
	int fd = -1;
	char tmp[64];
	fd=open("/etc/ipfilter/ipfilter_app.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if (fd == -1)
	{
		mkdir("/etc/ipfilter",0777);           //wjx 2007.4.13
		fd=open("/etc/ipfilter/ipfilter_app.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);	
		if (fd == -1)
		{
			return -1;		
		}	
	}	
	if (id == GB_C_APPFILTER_SAVE)
	{
		sprintf(tmp,"appfilter_status=\"%s\"\n",web_api->ipfilter_extend->appfilter_status);
		write(fd,tmp,strlen(tmp));	
		sprintf(tmp,"appfilter_icq_allowance=\"%s\"\n",web_api->ipfilter_extend->appfilter_icq_allowance);
		write(fd,tmp,strlen(tmp));			
		sprintf(tmp,"appfilter_msn_allowance=\"%s\"\n",web_api->ipfilter_extend->appfilter_msn_allowance);
		write(fd,tmp,strlen(tmp));			
		sprintf(tmp,"appfilter_ymsn_allowance=\"%s\"\n",web_api->ipfilter_extend->appfilter_ymsn_allowance);
		write(fd,tmp,strlen(tmp));		
		sprintf(tmp,"appfilter_ra_allowance=\"%s\"\n",web_api->ipfilter_extend->appfilter_ra_allowance);
		write(fd,tmp,strlen(tmp));				
	}		
	close(fd);
	//wtw add begin
	sprintf(tmp,"/var/script/urlfilter_stop.sh");
	asp_execute(tmp);
	sprintf(tmp,"/var/script/urlfilter_start.sh");
	asp_execute(tmp);
	//wtw add end
	return 0;
}

void ipfilter_mac_save()
{
	int fd;
	char *exbuf=(char*)ASP_MALLOC(64),*expath=(char*)ASP_MALLOC(64);;
	if(exbuf==NULL)
		goto err;
	if(expath==NULL)
		goto err;

	sprintf(expath,"/etc/ipfilter/ipfilter_%d_0.conf",set_index+1);
	remove(expath);
	fd=open(expath,O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd)
	{	
		sprintf(exbuf,"INTERFACE=\"%s\"\nDIRECTION=\"%s\"\n",web_api->access_ipfilter[set_index]->interface,web_api->access_ipfilter[set_index]->direction);
		write(fd,exbuf,strlen(exbuf));
		close(fd);	
	}
	
	sprintf(expath,"/etc/ipfilter/ipfilter_%d_%d.conf",set_index+1,rule_index+1);//wkw 9.21 change
	remove(expath);
	fd=open(expath,O_WRONLY|O_CREAT|O_APPEND,S_IREAD|S_IWRITE);
	if(fd)
	{	
		sprintf(exbuf,"RULE_TYPE=\"%s\"\n",web_api->access_ipfilter[set_index]->rule[rule_index]->rule_type);
		write(fd,exbuf,strlen(exbuf));				
		sprintf(exbuf,"ACTIVE=\"%s\"\n",web_api->access_ipfilter[set_index]->rule[rule_index]->active);
		write(fd,exbuf,strlen(exbuf));
		sprintf(exbuf,"MAC=\"%s\"\n",web_api->access_ipfilter[set_index]->rule[rule_index]->mac);
		write(fd,exbuf,strlen(exbuf));
		//wtw del sprintf(exbuf,"UNMATCHED=\"%s\"\n",web_api->access_ipfilter[set_index]->rule[rule_index]->unmatched);
		//wtw del write(fd,exbuf,strlen(exbuf));			
	}
			
err:
	if(exbuf)
		ASP_FREE(exbuf);
	if(expath)
		ASP_FREE(expath);
		
			
}

void ipfilter_mac_delete()
{
	char *expath;
	expath=(char*)ASP_MALLOC(128);
	if(expath==NULL)
		goto err;	 
		
	memset(web_api->access_ipfilter[set_index]->rule[rule_index]->rule_type,0,sizeof(web_api->access_ipfilter[set_index]->rule[rule_index]->rule_type));	
	//memset(web_api->access_ipfilter[set_index]->rule[rule_index]->active,0,sizeof(web_api->access_ipfilter[set_index]->rule[rule_index]->active));
	//memset(web_api->access_ipfilter[set_index]->rule[rule_index]->mac,0,sizeof(web_api->access_ipfilter[set_index]->rule[rule_index]->mac));
	//wtw del memset(web_api->access_ipfilter[set_index]->rule[rule_index]->unmatched,0,sizeof(web_api->access_ipfilter[set_index]->rule[rule_index]->unmatched));
	
	sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->active,"%s",val_def->ipfilter_active_no);	////by wkw 4.15
	sprintf(web_api->access_ipfilter[set_index]->rule[rule_index]->mac,"00:00:00:00:00:00");		
	
			
	sprintf(expath,"/etc/ipfilter/ipfilter_%d_%d.conf",set_index+1,rule_index+1);//wkw 9.21 change
	remove(expath);	
	 
	remove_ipfilter_set(); 
	 
err:
	if(expath)
		ASP_FREE(expath);		  	
}



void remove_ipfilter_set()////by wkw 4.15
{
    DIR*   dp;   
      struct   dirent*   ep;   
      int flag=1,i=0;
      char  *rmpath=(char*)ASP_MALLOC(128);
      if(rmpath==NULL)
       goto err;
      
    
      dp   =   opendir("/etc/ipfilter/");   
      if   (dp   !=   NULL)   
      {   
          while((ep=readdir(dp))) 
          {
           for(i=0;i<6;i++)
           {
            sprintf(rmpath,"ipfilter_%d_%d.conf",set_index+1,i+1);
//            fprintf("~~~%s,%s\n",ep->d_name,rmpath);
            if(strncmp(ep->d_name,rmpath,strlen(rmpath))==0)
            {
             flag=0;
             break; 
            }            
           }
      if(flag==0)
       break;
           
          }                
          (void)   closedir(dp);   
      }   
      if(flag==1)
      {
     memset(web_api->access_ipfilter[set_index]->interface,0,16);   
     memset(web_api->access_ipfilter[set_index]->direction,0,16);  
     
        sprintf(rmpath,"/etc/ipfilter/ipfilter_%d_0.conf",set_index+1);
        remove(rmpath); 
     }

          
err:
 if(rmpath)
  ASP_FREE(rmpath);  
}
//wtwang test
