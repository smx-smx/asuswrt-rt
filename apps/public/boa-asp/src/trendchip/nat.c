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

static int ruleindex = 0;
static int SetServer = 0;
#define ADD_RULE 1
#define DEL_RULE  2

//int write_nat_conf();
int TcWebApiDmz_execute();
int TcWebApiVsrv_execute(int action);
int TcWebApiIpmap_execute(int action);
void ipmap_execute_rule1(int action);
void ipmap_execute_rule2(int action);
void ipmap_execute_rule3(int action);
void ipmap_write_conf(char *arg,int index);
//int vsrv_write_conf(char *file,int index);
int TcWebApiDmz_write(char *file,int index);

int find3rdDot(char* IP)
{
	unsigned char i, counter = 0;
	for(i = 0; i < 15; i++)
	{
		if(IP[i] == '.')
			if(++counter == 3)
				break;
	}
	return i;
}

char *Dmz_get(int id,int index)
{
	char buf[128];
	switch(id)
	{
		case GB_C_DMZ_ENABLE:
			if(!strcmp(web_api->nat[index]->dmz->status,val_def->nat_dmz_enable))
				ASP_OUTPUT("CHECKED");
			break;
		case GB_C_DMZ_DISABLE:
			if(!strcmp(web_api->nat[index]->dmz->status,val_def->nat_dmz_disable))
				ASP_OUTPUT("CHECKED");
			break;
		case GB_C_DMZ_HOSTIP:
			sprintf(buf,"\"%s\"",web_api->nat[index]->dmz->hostip);
			ASP_OUTPUT(buf);
			break;
	}
	return ASP_OK;
}

int vsrv_list(int index,int vc)
{
	int i,j;
	char arg[500];
	
	ASP_OUTPUT("<table width=\"441\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\">");
	ASP_OUTPUT("<tr height=\"30\"><td width=\"70\" align=\"center\" class=\"tabdata\"><strong>Rule</strong></td><td width=\"70\"  align=\"center\" class=\"tabdata\"><strong>Start Port</strong></td><td width=\"70\"  align=\"center\" class=\"tabdata\"><strong>End port</strong></td><td width=\"120\" align=\"center\" class=\"tabdata\"><strong>Local IP Address</strong></td></tr>");
	for(j = index; j<vc;j++)
	{
		for(i = 0 ; i<10; i++)
		{
			if(web_api->nat[j]->vsrv[i]->local_ip[0] == 0)
				continue;
			sprintf(arg,"<TR><TD align=\"center\" class=\"tabdata\">%d</TD><TD align=\"center\" class=\"tabdata\">%s</TD><TD align=\"center\" class=\"tabdata\">%s</TD><TD align=\"center\" class=\"tabdata\">%s</TD></TR>",i+1,web_api->nat[j]->vsrv[i]->start_port,web_api->nat[j]->vsrv[i]->end_port,web_api->nat[j]->vsrv[i]->local_ip);
			ASP_OUTPUT(arg);
		}
	}
	ASP_OUTPUT(" </table>");
	return 0;
}

char *Vserver_get(int id,int index)
{
	int i;
	char buf[128];
	
	if(!ASP_ISPOST)
		ruleindex= 0;
	switch(id)
	{
		case GB_C_VSERVER_RULEINDEX:
			for(i = 0; i<10;i++)
			{
				if(i == ruleindex)
					sprintf(buf,"<OPTION SELECTED>%d",i+1);
				else
					sprintf(buf,"<OPTION >%d",i+1);
				ASP_OUTPUT(buf);
			}
			break;
		case GB_C_VSERVER_STARTPORT:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[ruleindex]->start_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_ENDPORT:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[ruleindex]->end_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_LOCALIP:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[ruleindex]->local_ip);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_LIST:
			vsrv_list(index,index+1);
			break;
		case GB_C_VSERVER_LISTALL:
			vsrv_list(0,8);
			break;	
		case GB_C_VSERVER_PORT11:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[0]->start_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT12:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[0]->end_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT21:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[1]->start_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT22:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[1]->end_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT31:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[2]->start_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT32:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[2]->end_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT41:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[3]->start_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT42:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[3]->end_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT51:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[4]->start_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT52:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[4]->end_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT61:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[5]->start_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT62:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[5]->end_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT71:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[6]->start_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT72:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[6]->end_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT81:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[7]->start_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT82:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[7]->end_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT91:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[8]->start_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT92:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[8]->end_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT101:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[9]->start_port);
			ASP_OUTPUT(buf);
			break;
		case GB_C_VSERVER_PORT102:
			sprintf(buf,"\"%s\"",web_api->nat[index]->vsrv[9]->end_port);
			ASP_OUTPUT(buf);
			break;
	}
	
	return ASP_OK;
}
int ipmap_list(int index,int vc)
{
	int i,j;
	char *arg=sprintf_buf;

	ASP_OUTPUT("<table width=\"492\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\">");
	ASP_OUTPUT("<tr height=\"30\"><td width=\"70\"  align=\"center\" class=\"tabdata\"><strong>Rule</strong></td><td width=\"100\"  align=\"center\" class=\"tabdata\"><strong>Type</strong></td><td width=\"100\" align=\"center\" class=\"tabdata\"><strong>Local Start IP</strong></td><td width=\"100\" align=\"center\" class=\"tabdata\"><strong>Local End IP</strong></td><td width=\"100\" align=\"center\" class=\"tabdata\"><strong>Public Start IP</strong></td><td width=\"100\" align=\"center\" class=\"tabdata\"><strong>Public End IP</strong></td></tr>");
	for(j = index; j<vc; j++)
	{
		for(i = 0; i<16; i++)
		{
			if(web_api->nat[j]->ipmap[i]->rule[0] == 0)
				continue;
			sprintf(arg,"<TR><TD align=\"center\" class=\"tabdata\">%d</TD><TD align=\"center\" class=\"tabdata\">&nbsp;%s</TD><TD align=\"center\" class=\"tabdata\">&nbsp;%s</TD><TD align=\"center\" class=\"tabdata\">&nbsp;%s</TD><TD align=\"center\" class=\"tabdata\">&nbsp;%s</TD><TD align=\"center\" class=\"tabdata\">&nbsp;%s</TD></TR>",i+1,web_api->nat[j]->ipmap[i]->rule,web_api->nat[j]->ipmap[i]->local_startip,web_api->nat[j]->ipmap[i]->local_endip,web_api->nat[j]->ipmap[i]->public_startip,web_api->nat[j]->ipmap[i]->public_endip);
			ASP_OUTPUT(arg);
		}
	}
	ASP_OUTPUT("</table>");
	return 0;
}

char *IPmapping_get(int id,int index)
{
	int i;
	char buf[256];
	
	if(!ASP_ISPOST)
		ruleindex= 0;
	switch(id)
	{
		case GB_C_IPMAPPING_RULEINDEX:
			for(i = 0; i<16;i++)
			{
				if(i == ruleindex)
					sprintf(buf,"<OPTION SELECTED>%d",i+1);
				else
					sprintf(buf,"<OPTION >%d",i+1);
				ASP_OUTPUT(buf);
			}
			break;
		case GB_C_IPMAPPING_RULETYPE:
			{
				char select[5][8]={"","","","",""};
				int i = 0;
				if(!strcmp(web_api->nat[index]->ipmap[ruleindex]->rule,val_def->nat_ipmap_rule_many_to_one))
					i =1;
				else if(!strcmp(web_api->nat[index]->ipmap[ruleindex]->rule,val_def->nat_ipmap_rule_many_to_many_overload))
					i =2;
				else if(!strcmp(web_api->nat[index]->ipmap[ruleindex]->rule,val_def->nat_ipmap_rule_many_to_many_no_overload))
					i =3;
				else if(!strcmp(web_api->nat[index]->ipmap[ruleindex]->rule,val_def->nat_ipmap_rule_server))
					i =4;
				strcpy(select[i],"SELECTED");
				sprintf(buf,"<OPTION %s>%s<OPTION %s>%s<OPTION %s>%s<OPTION %s>%s<OPTION %s>%s",select[0],val_def->nat_ipmap_rule_one_to_one,select[1],val_def->nat_ipmap_rule_many_to_one,select[2],val_def->nat_ipmap_rule_many_to_many_overload,select[3],val_def->nat_ipmap_rule_many_to_many_no_overload,select[4],val_def->nat_ipmap_rule_server);
				ASP_OUTPUT(buf);
			}
			break;
		case GB_C_IPMAPPING_LOCALSTARTIP:
			sprintf(buf,"\"%s\"",web_api->nat[index]->ipmap[ruleindex]->local_startip);
			ASP_OUTPUT(buf);
			break;
		case GB_C_IPMAPPING_LOCALENDIP:
			sprintf(buf,"\"%s\"",web_api->nat[index]->ipmap[ruleindex]->local_endip);
			ASP_OUTPUT(buf);
			break;
		case GB_C_IPMAPPING_PUBLICSTARTIP:
			sprintf(buf,"\"%s\"",web_api->nat[index]->ipmap[ruleindex]->public_startip);
			ASP_OUTPUT(buf);
			break;
		case GB_C_IPMAPPING_PUBLICENDIP:
			sprintf(buf,"\"%s\"",web_api->nat[index]->ipmap[ruleindex]->public_endip);
			ASP_OUTPUT(buf);
			break;
		case GB_C_IPMAPPING_LIST:
			ipmap_list(index,index+1);
			break;
			
		case GB_C_IPMAPPING_LISTALL:
			ipmap_list(0,8);
			break;	
		case GB_C_IPMAPPING_SERVERIP:
			sprintf(buf,"\"%s\"",web_api->nat[index]->serverip);
			ASP_OUTPUT(buf);
			break;
	}
	return ASP_OK;
}

char *TcWebApiNat_get(int id)
{
	int i,index = web_api->nat_vc;
	char buf[128];
	int fd;

	
	if(id>GB_C_NAT_SETTING && id<GB_C_NAT_TOPEND)
	{
		switch(id)
		{
		case GB_C_NAT_SHOWVC:
			sprintf(buf,"PVC%d",index);
			ASP_OUTPUT(buf);
			return ASP_OK;
			
		case GB_C_NAT_VC:
			for(i = 0 ; i<8 ; i++)
			{
				if(i == web_api->nat_vc)
					sprintf(buf,"<OPTION SELECTED>PVC%d",i);
				else
					sprintf(buf,"<OPTION >PVC%d",i);
				ASP_OUTPUT(buf);
			}
			return ASP_OK;
			
		case GB_C_NAT_STATUS:
			if(!strcmp(web_api->wan[index]->nat_enable,val_def->wan_natenable))
				strncpy(web_api->nat[index]->status,val_def->nat_status_activated,16);
			else
				strncpy(web_api->nat[index]->status,val_def->nat_status_deactivated,16);
				
			ASP_OUTPUT(web_api->nat[index]->status);
			fd = open("/var/tmp/nat_pvc.conf", O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
			sprintf(buf, "PVC=nas%d\n", index);
			write(fd,buf,strlen(buf));
			sprintf(buf, "STATUS=%s\n", web_api->nat[index]->status);
			write(fd,buf,strlen(buf));
			close(fd);
			fd = open("/var/tmp/nat_ip.conf", O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
			write(fd, web_api->nat[index]->numberofips, strlen(web_api->nat[index]->numberofips));
			write(fd, "\n", strlen("\n"));
			close(fd);
			sprintf(buf, "/bin/mkdir -p /etc/nat_pvc%d", index);
			asp_execute(buf);
			sprintf(buf, "/etc/nat_pvc%d/ipmode", index);
			fd = open(buf, O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
			sprintf(buf, "IPMODE=%s\n", web_api->nat[index]->numberofips);
			write(fd, buf, strlen(buf));
			close(fd);
			return ASP_OK;
			
		case GB_C_NAT_CHECKSTATUS:
			return web_api->nat[index]->status;
			
		case GB_C_NAT_NUMBEROFIPS:
			return web_api->nat[index]->numberofips;
		case GB_C_NAT_SINGLE:
			if(!strcmp(web_api->nat[index]->numberofips,val_def->nat_ip_single))
				ASP_OUTPUT("CHECKED");
			return ASP_OK;
			
		case GB_C_NAT_MULTIPLE:
			if(!strcmp(web_api->nat[index]->numberofips,val_def->nat_ip_multiple))
				ASP_OUTPUT("CHECKED");
			return ASP_OK;
			
		}
	}
	
	if(id>GB_C_DMZ_SETTING && id<GB_C_DMZ_END)
		return Dmz_get(id,index);
	
	if((id>GB_C_VSERVER_SETTING && id<GB_C_VSERVER_END) || (id>=0x56 && id<=0x69))
		return Vserver_get(id,index);
	
	if(id>GB_C_IPMAPPING_SETTING && id<GB_C_IPMAPPING_END)
		return IPmapping_get(id,index);
	
	return ASP_OK;
}

int Dmz_set(int id,int index,char *form_val)
{
	int fd;
	char buf[128];
	switch(id)
	{
		case GB_C_DMZ_STATUS:
			strncpy(web_api->nat[index]->dmz->status,form_val,16);
			sprintf(buf, "/etc/nat_pvc%d/dmz", index);
			fd = open(buf, O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
			if(!strcmp(web_api->nat[index]->dmz->status,val_def->nat_dmz_disable))
				write(fd, "DMZ_ACTIVE=No\n", strlen("DMZ_ACTIVE=No\n"));
			sprintf(buf, "DMZ_IP=\n");
			write(fd, buf, strlen(buf));
			close(fd);
			sprintf(buf, "/var/script/dmz.sh %d", index);
			asp_execute(buf);
			break;
		case GB_C_DMZ_HOSTIP:
			strncpy(web_api->nat[index]->dmz->hostip,form_val,16);
			sprintf(buf, "/etc/nat_pvc%d/dmz", index);
			fd = open(buf, O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
			if(!strcmp(web_api->nat[index]->dmz->status,val_def->nat_dmz_enable))
			{
				write(fd, "DMZ_ACTIVE=Yes\n", strlen("DMZ_ACTIVE=Yes\n"));
				sprintf(buf, "DMZ_IP=%s\n", web_api->nat[index]->dmz->hostip);
			}
			else if(!strcmp(web_api->nat[index]->dmz->status,val_def->nat_dmz_disable))
			{
				write(fd, "DMZ_ACTIVE=No\n", strlen("DMZ_ACTIVE=No\n"));
				sprintf(buf, "DMZ_IP=\n");				
			}
			write(fd, buf, strlen(buf));
			close(fd);
			sprintf(buf, "/var/script/dmz.sh %d", index);
			asp_execute(buf);
			break;
	}
	return 0;
}

int Vserver_set(int id,int index,char *form_val)
{
	int fd;
	char buf[128];
	switch(id)
	{
		case GB_C_VSERVER_RULEINDEX:
			ruleindex = atoi(form_val)-1;
			if(ruleindex<0)
				ruleindex = 0;
			break;
		case GB_C_VSERVER_STARTPORT:
			strncpy(web_api->nat[index]->vsrv[ruleindex]->start_port,form_val,8);
			break;
		case GB_C_VSERVER_ENDPORT:
			strncpy(web_api->nat[index]->vsrv[ruleindex]->end_port,form_val,8);
			break;
		case GB_C_VSERVER_LOCALIP:
			strncpy(web_api->nat[index]->vsrv[ruleindex]->local_ip,form_val,16);
			sprintf(buf, "/etc/nat_pvc%d/vserver%d", index, ruleindex);
			fd = open(buf, O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
			sprintf(buf, "STARTPORT=%s\n", web_api->nat[index]->vsrv[ruleindex]->start_port);
			write(fd, buf, strlen(buf));
			sprintf(buf, "ENDPORT=%s\n", web_api->nat[index]->vsrv[ruleindex]->end_port);
			write(fd, buf, strlen(buf));
			sprintf(buf, "LOCALIP=%s\n", web_api->nat[index]->vsrv[ruleindex]->local_ip);
			write(fd, buf, strlen(buf));
			close(fd);
			break;
				
	}		
	return 0;
}
int IPmapping_set(int id,int index,char *form_val)
{
	switch(id)
	{
		case GB_C_IPMAPPING_RULEINDEX:
			ruleindex = atoi(form_val)-1;
			if(ruleindex < 0)
				ruleindex = 0;
			break;
		case GB_C_IPMAPPING_RULETYPE:
			if(strcmp(form_val, val_def->nat_ipmap_rule_server))
			{
				strncpy(web_api->nat[index]->ipmap[ruleindex]->rule,form_val,32);
				SetServer = 0;
			}
			else
				SetServer = 1;
			break;
		case GB_C_IPMAPPING_LOCALSTARTIP:
			strncpy(web_api->nat[index]->ipmap[ruleindex]->local_startip,form_val,16);
			break;
		case GB_C_IPMAPPING_LOCALENDIP:
			strncpy(web_api->nat[index]->ipmap[ruleindex]->local_endip,form_val,16);
			break;
		case GB_C_IPMAPPING_PUBLICSTARTIP:
			strncpy(web_api->nat[index]->ipmap[ruleindex]->public_startip,form_val,16);	
			break;
		case GB_C_IPMAPPING_PUBLICENDIP:
			strncpy(web_api->nat[index]->ipmap[ruleindex]->public_endip,form_val,16);
			break;
		case GB_C_IPMAPPING_SERVERIP:
			strncpy(web_api->nat[index]->serverip,form_val,16);
			break;
	}
	return 0;
}

int TcWebApiNat_set(int id,char *value)
{
   char *ret;	
   int index = web_api->nat_vc;

   ret = ASP_FORM(value);
   if(ret == NULL)
   	return -1;
   
	if(id>GB_C_NAT_SETTING && id<GB_C_NAT_TOPEND)
	{
		int fd;
		char buf[128];
		
		switch(id)
		{
		case GB_C_NAT_VC:
			sscanf(ret,"PVC%d",&index);
			web_api->nat_vc = index;
			return 0;
		case GB_C_NAT_NUMBEROFIPS:
			strncpy(web_api->nat[index]->numberofips,ret,16);
			fd = open("/var/tmp/nat_ip.conf", O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
			write(fd, web_api->nat[index]->numberofips, strlen(web_api->nat[index]->numberofips));
			write(fd, "\n", strlen("\n"));
			close(fd);
//			sprintf(buf, "/bin/mkdir -p /etc/nat_pvc%d", index);
//			asp_execute(buf);
			sprintf(buf, "/etc/nat_pvc%d/ipmode", index);
			fd = open(buf, O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
			sprintf(buf, "IPMODE=%s\n", web_api->nat[index]->numberofips);
			write(fd, buf, strlen(buf));
			close(fd);
//			sprintf(buf, "/var/script/nat_stop.sh %d", index);
//			asp_execute(buf);
//			sprintf(buf, "/var/script/nat_start.sh %d", index);
//			asp_execute(buf);
//			write_nat_conf();
			return 0;
		}
	}
	
	if(id>GB_C_DMZ_SETTING && id<GB_C_DMZ_END)
		return Dmz_set(id,index,ret);
	
	if(id>GB_C_VSERVER_SETTING && id<GB_C_VSERVER_END)
		return Vserver_set(id,index,ret);
	
	if(id>GB_C_IPMAPPING_SETTING && id<GB_C_IPMAPPING_END)
		return IPmapping_set(id,index,ret);
	
	return 0;
}
#if 0
int write_nat_conf()
{
	char buf[64];
	int fd;
	
	sprintf(buf,"/etc/nat%d.conf",web_api->nat_vc);
	fd = open(buf,O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd ==-1)
		return -1;
	sprintf(buf,"NUMBER_OF_IPS=%s\n",web_api->nat[web_api->nat_vc]->numberofips);
	write(fd,buf,strlen(buf));
	if(!strcmp(web_api->nat[web_api->nat_vc]->numberofips,val_def->nat_ip_multiple))
	{
		sprintf(buf,"SERVER_IP=%s\n",web_api->nat[web_api->nat_vc]->serverip);
		write(fd,buf,strlen(buf));	
	}
	close(fd);
	return 0;
}
#endif
int TcWebApiNat_execute(int id)
{
	if(id ==GB_C_IPMAPPING_DELETE)
		TcWebApiIpmap_execute(DEL_RULE);
	else if(id ==GB_C_IPMAPPING_ADD)
		TcWebApiIpmap_execute(ADD_RULE);
	else if(id ==GB_C_DMZ_SETTING)
		TcWebApiDmz_execute();
	else if(id ==GB_C_VSERVER_ADD)
		TcWebApiVsrv_execute(ADD_RULE);
	else if(id ==GB_C_VSERVER_DELETE)
		TcWebApiVsrv_execute(DEL_RULE);
			
	return 0;
}
int TcWebApiDmz_write(char *file,int index)
{
	int fd;
	char buf[128];
	
	fd = open(file,O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd == -1)
		return -1;
	sprintf(buf,"DMZ_IP=%s\nNUMBER_OF_IPS=%s\nSERVER_IP=%s\n",web_api->nat[index]->dmz->hostip,web_api->nat[index]->numberofips,web_api->nat[index]->serverip);
	write(fd,buf,strlen(buf));
	close(fd);

	return 0;
}

int TcWebApiDmz_execute()
{
#if 0
	char arg[128];
	int index = web_api->nat_vc;
	char external_ip[16],internal_ip[16];
	char *str;
	
	sprintf(arg,"/etc/dmz%d.conf",index);
	if(!strcmp(web_api->nat[index]->dmz->status,val_def->nat_dmz_disable))
	{
		
		asp_execute("/bin/iptables -t nat -X DMZPOST");
		asp_execute("/bin/iptables -t nat -X DMZPRE");
		asp_execute("/bin/iptables -t filter -X DMZFORWARD");
		memset(web_api->nat[index]->dmz->hostip,0,16);
		unlink(arg);
		return 0;
	}
	TcWebApiDmz_write(arg,index);
	if(!strcmp(web_api->nat[index]->numberofips,val_def->nat_ip_single))
	{
		
		sprintf(arg,"ifconfig nas%d | sed -ne 's/ *inet addr:\\([0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\) *B.*/\\1/p' > /tmp/ip.tmp",index);
		asp_execute(arg);
		str = asp_readall("/tmp/ip.tmp");
		strncpy(external_ip,str,16);
		free(str);
	}
	else
	{
		if(web_api->nat[index]->serverip[0] == 0)
			return -1;
		strncpy(external_ip,web_api->nat[index]->serverip,16); 
	}

	strncpy(internal_ip,web_api->lan[0]->ip_addr,16); 

	asp_execute("/bin/iptables -t nat -X DMZPOST");
	asp_execute("/bin/iptables -t nat -X DMZPRE");

	asp_execute("/bin/iptables -t nat -N DMZPOST");
	asp_execute("/bin/iptables -t nat -N DMZPRE");

	if(!strcmp(external_ip,"0.0.0.0"))
		sprintf(arg,"/bin/iptables -t nat -A DMZPOST -s %s/24 -d 0/0 -o nas%d -j MASQUERADE",internal_ip,index);
	else
		sprintf(arg,"/bin/iptables -t nat -A DMZPOST -s %s/24 -d 0/0 -o nas%d -j SNAT --to %s",internal_ip,index,external_ip);
	asp_execute(arg);

	sprintf(arg,"/bin/iptables -t nat -A DMZPRE -s 0/0 -d %s/24 -i nas%d -j DROP",internal_ip,index);
	asp_execute(arg);

	if(!strcmp(external_ip,"0.0.0.0"))
		sprintf(arg,"/bin/iptables -t nat -A DMZPRE -p tcp  -i nas%d -j DNAT --to %s",index,web_api->nat[index]->dmz->hostip);
	else
		sprintf(arg,"/bin/iptables -t nat -A DMZPRE -p tcp -d %s/32 -s 0/0 -i nas%d -j DNAT --to %s",external_ip,index,web_api->nat[index]->dmz->hostip);
	asp_execute(arg);

	sprintf(arg,"/bin/iptables -t nat -A DMZPRE -s 0/0 -d %s -i nas%d -j DROP",web_api->nat[index]->dmz->hostip,index);
	asp_execute(arg);

	
	asp_execute("/bin/iptables -t nat -A PREROUTING -j DMZPRE");
	asp_execute("/bin/iptables -t nat -A POSTROUTING -j DMZPOST");
#endif	
	return 0;
}
#if 0
int vsrv_execute(int action,char *external_ip)
{
	char opt[3];
	char arg[128];
	int index = web_api->nat_vc;
	char *start = web_api->nat[index]->vsrv[ruleindex]->start_port;
	char *end = web_api->nat[index]->vsrv[ruleindex]->end_port;
	char *local = web_api->nat[index]->vsrv[ruleindex]->local_ip;
	
	if(action ==ADD_RULE)
		strcpy(opt,"-I");
	else
		strcpy(opt,"-D");

	if(!strcmp(external_ip,"0.0.0.0"))
	{
		sprintf(arg,"/bin/iptables -t nat %s PREROUTING -p tcp --dport %s:%s -i nas%d -j DNAT --to %s:%s-%s",opt,start,end,index,local,start,end);
		asp_execute(arg);
		sprintf(arg,"/bin/iptables -t nat %s PREROUTING -p udp --dport %s:%s -i nas%d -j DNAT --to %s:%s-%s",opt,start,end,index,local,start,end);
		asp_execute(arg);

	}
	else
	{
		sprintf(arg,"/bin/iptables -t nat %s PREROUTING -p tcp --dport %s:%s -d %s -s 0/0  -j DNAT --to %s:%s-%s",opt,start,end,external_ip,local,start,end);
		asp_execute(arg);
		sprintf(arg,"/bin/iptables -t nat %s PREROUTING -p udp --dport %s:%s -d %s -s 0/0  -j DNAT --to %s:%s-%s",opt,start,end,external_ip,local,start,end);
		asp_execute(arg);
	}

	return 0;	
}

int vsrv_write_conf(char *file,int index)
{
	char arg[128];
	int fd;
	
	fd = open(file,O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd ==-1)
		return -1;

	sprintf(arg,"STARTPORT=%s\n",web_api->nat[index]->vsrv[ruleindex]->start_port);
	write(fd,arg,strlen(arg));

	sprintf(arg,"ENDPORT=%s\n",web_api->nat[index]->vsrv[ruleindex]->end_port);
	write(fd,arg,strlen(arg));
	sprintf(arg,"LOCALIP=%s\n",web_api->nat[index]->vsrv[ruleindex]->local_ip);
	write(fd,arg,strlen(arg));
	sprintf(arg,"NUMBER_OF_IPS=%s\nSERVER_IP=%s\n",web_api->nat[index]->numberofips,web_api->nat[index]->serverip);
	write(fd,arg,strlen(arg));
	
	close(fd);
	return 0;
}
#endif
int TcWebApiVsrv_execute(int action)
{
	char buf[128];
	switch(action)
	{
		case ADD_RULE:
			sprintf(buf, "/var/script/vserver.sh add %d %d", web_api->nat_vc, ruleindex);
			asp_execute(buf);
			break;
		case DEL_RULE:
			memset(web_api->nat[web_api->nat_vc]->vsrv[ruleindex]->start_port, 0, 8);
			memset(web_api->nat[web_api->nat_vc]->vsrv[ruleindex]->end_port, 0, 8);
			memset(web_api->nat[web_api->nat_vc]->vsrv[ruleindex]->local_ip, 0, 16);
			sprintf(buf, "/var/script/vserver.sh del %d %d", web_api->nat_vc, ruleindex);
			asp_execute(buf);
			sprintf(buf, "rm -f /etc/nat_pvc%d/vserver%d", web_api->nat_vc, ruleindex);
			asp_execute(buf);
			break;
	}
	
#if 0
	char *arg= sprintf_buf;
	int index = web_api->nat_vc;
	char external_ip[16];
	char *str;

	if(web_api->nat[index]->vsrv[ruleindex]->local_ip[0] == 0)
		return 0;
	
	if(action == ADD_RULE)
	{
		strncpy(web_api->nat[index]->vsrv[ruleindex]->numberofips,web_api->nat[index]->numberofips,16);
		strncpy(web_api->nat[index]->vsrv[ruleindex]->serverip,web_api->nat[index]->serverip,16);
	}
	
	if(!strcmp(web_api->nat[index]->vsrv[ruleindex]->numberofips,val_def->nat_ip_single))
	{
		
		sprintf(arg,"ifconfig nas%d | sed -ne 's/ *inet addr:\\([0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\.[0-9]\\{1,3\\}\\) *B.*/\\1/p' > /tmp/ip.tmp",index);
		asp_shell(arg);
		str = asp_readall("/tmp/ip.tmp");
		strncpy(external_ip,str,16);
		free(str);
	}
	else
	{
		if(web_api->nat[index]->vsrv[ruleindex]->serverip[0] == 0)
		{
			if(action == DEL_RULE)
			{
				sprintf(arg,"/etc/vserver_%d_%d.conf",ruleindex,index);
				memset(web_api->nat[index]->vsrv[ruleindex],0,sizeof(VSRV_SETUP));
				unlink(arg);
			}
			return -1;
		}
		strncpy(external_ip,web_api->nat[index]->vsrv[ruleindex]->serverip,16); 
	}
	
	sprintf(arg,"/etc/vserver_%d_%d.conf",ruleindex,index);
	if(action ==ADD_RULE)
	{
		vsrv_execute(ADD_RULE,external_ip);
		vsrv_write_conf(arg,index);
	}
	else if(action == DEL_RULE)
	{
		vsrv_execute(DEL_RULE,external_ip);
		memset(web_api->nat[index]->vsrv[ruleindex],0,sizeof(VSRV_SETUP));
		unlink(arg);
	}
#endif		
	return 0;
}
int TcWebApiIpmap_execute(int action)
{	
	int index = web_api->nat_vc;
	char *rule = web_api->nat[index]->ipmap[ruleindex]->rule;
	char *ext_start = web_api->nat[index]->ipmap[ruleindex]->public_startip;
//	char *server = web_api->nat[index]->serverip;
	char arg[128];
	
	if(web_api->nat[index]->ipmap[ruleindex]->rule[0] == 0 && !SetServer)
		return 0;
	
#if 0
	if(!strcmp(rule,val_def->nat_ipmap_rule_server))
	{
		if(action == ADD_RULE)
			strncpy(server,ext_start,16);
		else
		{
			int i;
			memset(server,0,16);
			for(i = 0 ; i<16;i++)
			{
				if(!strcmp(web_api->nat[index]->ipmap[i]->rule,val_def->nat_ipmap_rule_server))
				{
					strncpy(server,web_api->nat[index]->ipmap[i]->public_startip,16);		
					break;
				}
			}
		}
		write_nat_conf();
		return 0;
	}
#endif
/*
	if(!strcmp(rule,val_def->nat_ipmap_rule_server))
	{
		int fd;
		
		fprintf(stderr, "Add Server\n");

		if(action == ADD_RULE)
		{
			sprintf(arg, "/etc/nat_pvc%d/server_ip", index);
			fd = open(arg, O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
			if(fd != -1)
			{
				sprintf(arg, "SERVER_IP=%s\n", ext_start);
				write(fd, arg, strlen(arg));
				close(fd);
			}
			else
				return -1;
			sprintf(arg, "/var/script/nat_stop.sh %d", index);
			asp_execute(arg);
			sprintf(arg, "/var/script/nat_start.sh %d", index);
			asp_execute(arg);
			sprintf(arg,"/var/script/ipaddr_mapping.sh add %d %d", index, ruleindex);
			asp_execute(arg);
		}
		else if(action == DEL_RULE)
		{
			sprintf(arg, "/bin/rm -f /etc/nat_pvc%d/server_ip", index);
			asp_execute(arg);
			sprintf(arg, "/var/script/nat_stop.sh %d", index);
			asp_execute(arg);
			sprintf(arg, "/var/script/nat_start.sh %d", index);
			asp_execute(arg);
		}
		return 0;
	}
*/
#if 0
	sprintf(arg,"/etc/ipmapping_%d_%d.conf",ruleindex,index);
	if(!strcmp(rule,val_def->nat_ipmap_rule_one_to_one))
	{

		
		if(action ==ADD_RULE)
		{
			ipmap_execute_rule1(ADD_RULE);
			
		}
		else if(action == DEL_RULE)
		{
			ipmap_execute_rule1(DEL_RULE);
			memset(web_api->nat[index]->ipmap[ruleindex],0,sizeof(IPMAP_SETUP));
			unlink(arg);
			return 0;
		}	
	}
	else if(!strcmp(rule,val_def->nat_ipmap_rule_many_to_one))
	{

		
		if(action ==ADD_RULE)
		{
			ipmap_execute_rule2(ADD_RULE);
			
		}
		else if(action == DEL_RULE)
		{
			ipmap_execute_rule2(DEL_RULE);
			memset(web_api->nat[index]->ipmap[ruleindex],0,sizeof(IPMAP_SETUP));
			unlink(arg);
			return 0;
		}	
	}
	else
	{

		
		if(action ==ADD_RULE)
		{
			
			ipmap_execute_rule3(ADD_RULE);
			
		}
		else if(action == DEL_RULE)
		{
			ipmap_execute_rule3(DEL_RULE);
			memset(web_api->nat[index]->ipmap[ruleindex],0,sizeof(IPMAP_SETUP));
			unlink(arg);
			return 0;
		}	
	}
#endif	
	if(action == ADD_RULE)
	{
//		if(!strcmp(rule,val_def->nat_ipmap_rule_server))
		if(SetServer)
		{
			int fd, i;

			sprintf(arg, "/etc/nat_pvc%d/server_ip", index);
			fd = open(arg, O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
			if(fd != -1)
			{
//				sprintf(arg, "SERVER_IP=%s\n", ext_start);
				sprintf(arg, "SERVER_IP=%s\n", web_api->nat[index]->serverip);
				write(fd, arg, strlen(arg));
				close(fd);
			}
			else
				return -1;
			sprintf(arg, "iptables -t nat -F VS_PRE%d", index);
			asp_execute(arg);
			for(i = 0; i < 9; i++)
			{
				sprintf(arg, "/etc/nat_pvc%d/vserver%d", index, i);
				fd = open(arg, O_RDONLY);
				if(fd)
				{
					close(fd);
					sprintf(arg, "/var/script/vserver.sh add %d %d", index, i);
					asp_execute(arg);
				}
			}
/*			sprintf(arg, "/var/script/nat_stop.sh %d", index);
			asp_execute(arg);
			sprintf(arg, "/var/script/nat_start.sh %d", index);
			asp_execute(arg);*/
			SetServer = 0;
		}
		else
		{
			sprintf(arg,"/etc/nat_pvc%d/ipaddr_mapping%d", index, ruleindex);
			ipmap_write_conf(arg,index);
			sprintf(arg,"/var/script/ipaddr_mapping.sh add %d %d", index, ruleindex);
			asp_execute(arg);
		}
	}
	else if(action == DEL_RULE)
	{
//		if(!strcmp(rule,val_def->nat_ipmap_rule_server))
		if(SetServer)
		{
			int fd, i;

			sprintf(arg, "/bin/rm -f /etc/nat_pvc%d/server_ip", index);
			asp_execute(arg);
			sprintf(arg, "iptables -t nat -F VS_PRE%d", index);
			asp_execute(arg);
			for(i = 0; i < 9; i++)
			{
				sprintf(arg, "/etc/nat_pvc%d/vserver%d", index, i);
				fd = open(arg, O_RDONLY);
				if(fd)
				{
					close(fd);
					sprintf(arg, "/var/script/vserver.sh add %d %d", index, i);
					asp_execute(arg);
				}
			}
/*			sprintf(arg, "/var/script/nat_stop.sh %d", index);
			asp_execute(arg);
			sprintf(arg, "/var/script/nat_start.sh %d", index);
			asp_execute(arg);*/
			memset(web_api->nat[index]->serverip, 0, 16);
			SetServer = 0;
		}	
		else
		{
			sprintf(arg,"/var/script/ipaddr_mapping.sh del %d %d", index, ruleindex);
			asp_execute(arg);
			sprintf(arg,"/bin/rm -f /etc/nat_pvc%d/ipaddr_mapping%d", index, ruleindex);
			asp_execute(arg);
			memset(web_api->nat[index]->ipmap[ruleindex]->rule, 0, 32);
			memset(web_api->nat[index]->ipmap[ruleindex]->local_startip, 0, 16);
			memset(web_api->nat[index]->ipmap[ruleindex]->local_endip, 0, 16);
			memset(web_api->nat[index]->ipmap[ruleindex]->public_startip, 0, 16);	
			memset(web_api->nat[index]->ipmap[ruleindex]->public_endip, 0, 16);
		}
	}
	return 0;
}

void ipmap_execute_rule1(int action)
{
	int index = web_api->nat_vc;
	char *int_start =web_api->nat[index]->ipmap[ruleindex]->local_startip;
	char *ext_start = web_api->nat[index]->ipmap[ruleindex]->public_startip;
	char opt[3];

	
	if(action ==ADD_RULE)
		strcpy(opt,"-I");
	else
		strcpy(opt,"-D");
	
	if(!strcmp(ext_start,"0.0.0.0"))
	{
		sprintf(sprintf_buf,"/bin/iptables -t nat %s   -s 0/0  -i nas%d  -j DNAT --to %s",opt,index,int_start);
		asp_execute(sprintf_buf);
		sprintf(sprintf_buf,"/bin/iptables -t nat %s POSTROUTING  -s %s  -o nas%d -j MASQUERADE",opt,int_start,index);
		asp_execute(sprintf_buf);
	}
	else
	{
		sprintf(sprintf_buf,"/bin/iptables -t nat %s PREROUTING   -d %s -s 0/0  -j DNAT --to %s",opt,ext_start,int_start);
		asp_execute(sprintf_buf);
		sprintf(sprintf_buf,"/bin/iptables -t nat %s POSTROUTING  -s %s   -j SNAT --to %s",opt,int_start,ext_start);
		asp_execute(sprintf_buf);
	}
	
}
extern int calc_netmask(char *start, char *end, char *ostart, char *omask);
void ipmap_execute_rule2(int action)
{
	int index = web_api->nat_vc;
	char *int_start =web_api->nat[index]->ipmap[ruleindex]->local_startip;
	char *int_end = web_api->nat[index]->ipmap[ruleindex]->local_endip;
	char *ext_start = web_api->nat[index]->ipmap[ruleindex]->public_startip;
	char opt[3];
	char calc_start[32],calc_mask[32];
	if(calc_netmask(int_start,int_end,calc_start,calc_mask)<0)
		return;
	fprintf(stderr,"calc_netmask ip:%s mask:%s\n",calc_start,calc_mask);
	if(strcmp(calc_start,int_start))
		return;
	
	if(action ==ADD_RULE)
		strcpy(opt,"-I");
	else
		strcpy(opt,"-D");
	
	if(!strcmp(ext_start,"0.0.0.0"))
	{
		sprintf(sprintf_buf,"/bin/iptables -t nat %s PREROUTING   -s 0/0  -i nas%d  -j DNAT --to %s-%s",opt,index,int_start,int_end);
		asp_execute(sprintf_buf);
		sprintf(sprintf_buf,"/bin/iptables -t nat %s POSTROUTING  -s %s/%s  -o nas%d -j MASQUERADE",opt,calc_start,calc_mask,index);
		asp_execute(sprintf_buf);
	}
	else
	{
		sprintf(sprintf_buf,"/bin/iptables -t nat %s PREROUTING  -d %s -s 0/0  -j DNAT --to %s-%s",opt,ext_start,int_start,int_end);
		asp_execute(sprintf_buf);
		sprintf(sprintf_buf,"/bin/iptables -t nat %s POSTROUTING -s %s/%s  -j SNAT --to %s",opt,calc_start,calc_mask,ext_start);
		asp_execute(sprintf_buf);
	}
	
}

void ipmap_execute_rule3(int action)
{
	int index = web_api->nat_vc;
	char *int_start =web_api->nat[index]->ipmap[ruleindex]->local_startip;
	char *int_end = web_api->nat[index]->ipmap[ruleindex]->local_endip;
	char *ext_start = web_api->nat[index]->ipmap[ruleindex]->public_startip;
	char *ext_end = web_api->nat[index]->ipmap[ruleindex]->public_endip;
	char opt[3];
	char calc_start1[32],calc_mask1[32],calc_start2[32],calc_mask2[32];

	
	if(action ==ADD_RULE)
		strcpy(opt,"-I");
	else
		strcpy(opt,"-D");

	if(!strcmp(ext_start,"0.0.0.0"))
	{
		if(calc_netmask(int_start,int_end,calc_start1,calc_mask1)<0)
			return;
		fprintf(stderr,"calc_netmask ip:%s mask:%s\n",calc_start1,calc_mask1);
		
		sprintf(sprintf_buf,"/bin/iptables -t nat %s PREROUTING   -s 0/0  -i nas%d  -j DNAT --to %s-%s",opt,index,int_start,int_end);
		asp_execute(sprintf_buf);
		sprintf(sprintf_buf,"/bin/iptables -t nat %s POSTROUTING  -s %s/%s  -o nas%d -j MASQUERADE",opt,calc_start1,calc_mask1,index);
		asp_execute(sprintf_buf);
	}
	else
	{
		if(calc_netmask(int_start,int_end,calc_start1,calc_mask1)<0)
			return;
		if(calc_netmask(int_start,int_end,calc_start2,calc_mask2)<0)
			return;
		fprintf(stderr,"calc_netmask ip:%s mask:%s ip %s mask %s \n",calc_start1,calc_mask1,calc_start2,calc_mask2);
		if(strcmp(calc_start1,int_start) || strcmp(calc_start2,ext_start))
			return;
		sprintf(sprintf_buf,"/bin/iptables -t nat %s PREROUTING   -d %s/%s -s 0/0  -j DNAT --to %s-%s",opt,calc_start2,calc_mask2,int_start,int_end);
		asp_execute(sprintf_buf);
		
		sprintf(sprintf_buf,"/bin/iptables -t nat %s POSTROUTING  -s %s/%s  -j SNAT --to %s-%s",opt,calc_start1,calc_mask1,ext_start,ext_end);
		asp_execute(sprintf_buf);
		

	}
	
}

void ipmap_write_conf(char *arg,int index)
{	
	char *PublicIPs;
	char int_end[16], ext_end[16];
	char *rule = web_api->nat[index]->ipmap[ruleindex]->rule;
	char *int_start =web_api->nat[index]->ipmap[ruleindex]->local_startip;
//	char *int_end = web_api->nat[index]->ipmap[ruleindex]->local_endip;
	char *ext_start = web_api->nat[index]->ipmap[ruleindex]->public_startip;
//	char *ext_end = web_api->nat[index]->ipmap[ruleindex]->public_endip;
	int fd;//,len = 0;

	fd = open(arg,O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd == -1)
		return;
#if 0	
	len +=sprintf(sprintf_buf+len,"RULETYPE=%s\n",rule);
#endif
	sprintf(sprintf_buf,"RULETYPE=\"%s\"\n",rule);
	if(!strcmp(rule,val_def->nat_ipmap_rule_one_to_one))
	{
#if 0
		len +=sprintf(sprintf_buf+len,"LOCALSTARTIP=\"%s\"\n",int_start);
		len +=sprintf(sprintf_buf+len,"LOCALENDIP=\"%s\"\n", "0.0.0.0");
		len +=sprintf(sprintf_buf+len,"PUBLICSTARTIP=\"%s\"\n",ext_start);
		len +=sprintf(sprintf_buf+len,"PUBLICENDIP=\"%s\"\n", "0.0.0.0");
#endif		
		memset(web_api->nat[index]->ipmap[ruleindex]->local_endip, 0, 16);
		strcpy(web_api->nat[index]->ipmap[ruleindex]->local_endip, "0.0.0.0");
		memset(web_api->nat[index]->ipmap[ruleindex]->public_endip, 0, 16);
		strcpy(web_api->nat[index]->ipmap[ruleindex]->public_endip, "0.0.0.0");
	}
	else if(!strcmp(rule,val_def->nat_ipmap_rule_many_to_one))
	{
#if 0	
		len +=sprintf(sprintf_buf+len,"LOCALSTARTIP=\"%s\"\n",int_start);
		len +=sprintf(sprintf_buf+len,"LOCALENDIP=\"%s\"\n",int_end);
		len +=sprintf(sprintf_buf+len,"PUBLICSTARTIP=\"%s\"\n",ext_start);
		len +=sprintf(sprintf_buf+len,"PUBLICENDIP=\"%s\"\n", "0.0.0.0");
#endif		
		memset(web_api->nat[index]->ipmap[ruleindex]->public_endip, 0, 16);
		strcpy(web_api->nat[index]->ipmap[ruleindex]->public_endip, "0.0.0.0");
//		fprintf(stderr, "public_endip = %s\n", web_api->nat[index]->ipmap[ruleindex]->public_endip);
	}
	else if(!strcmp(rule,val_def->nat_ipmap_rule_server))
	{
		memset(web_api->nat[index]->ipmap[ruleindex]->local_startip, 0, 16);
		strcpy(web_api->nat[index]->ipmap[ruleindex]->local_startip, "0.0.0.0");
		memset(web_api->nat[index]->ipmap[ruleindex]->local_endip, 0, 16);
		strcpy(web_api->nat[index]->ipmap[ruleindex]->local_endip, "0.0.0.0");
		memset(web_api->nat[index]->ipmap[ruleindex]->public_endip, 0, 16);
		strcpy(web_api->nat[index]->ipmap[ruleindex]->public_endip, "0.0.0.0");
	}
#if 0	
	else
	{
		len +=sprintf(sprintf_buf+len,"LOCALSTARTIP=\"%s\"\n",int_start);
		len +=sprintf(sprintf_buf+len,"LOCALENDIP=\"%s\"\n",int_end);
		len +=sprintf(sprintf_buf+len,"PUBLICSTARTIP=\"%s\"\n",ext_start);
		len +=sprintf(sprintf_buf+len,"PUBLICENDIP=\"%s\"\n",ext_end);
	}
#endif
	strcpy(ext_end, web_api->nat[index]->ipmap[ruleindex]->public_endip);

	if(!strcmp(ext_end, "0.0.0.0"))
	{
		PublicIPs = (char*)malloc(16);
		strcpy(PublicIPs, web_api->nat[index]->ipmap[ruleindex]->public_startip);
//		PublicIPs = web_api->nat[index]->ipmap[ruleindex]->public_startip;
	}
	else
	{
		unsigned char i, IP_3rdDot, IP1, IP2;
		char PreIP[16], TempIP[16];
		unsigned int size;

		IP_3rdDot = find3rdDot(ext_start);
		strcpy(PreIP, ext_start);
		PreIP[IP_3rdDot + 1] = '\0';
		IP1 = atoi(ext_start + IP_3rdDot + 1);
		IP2 = atoi(ext_end + IP_3rdDot + 1);
		size = 15 * (IP2 - IP1 + 1) + (IP2 - IP1 + 1);
		PublicIPs = (char*)malloc(size + 12);
		sprintf(PublicIPs, "%s", "PUBLICIP=\"");
//		memset(PublicIPs, 0, size);
		for(i = 0; i <= IP2 - IP1; i++)
		{
			sprintf(TempIP, "%s%d", PreIP, IP1 + i);
			strcat(PublicIPs, TempIP);
			if(i < IP2 - IP1)
				strcat(PublicIPs, " ");
		}
//		strcat(PublicIPs, "\0");
		strcat(PublicIPs, "\"\n");
	}
	write(fd,sprintf_buf, strlen(sprintf_buf));
	sprintf(sprintf_buf,"LOCALSTARTIP=\"%s\"\n", int_start);
	write(fd,sprintf_buf, strlen(sprintf_buf));
	strcpy(int_end, web_api->nat[index]->ipmap[ruleindex]->local_endip);
	sprintf(sprintf_buf,"LOCALENDIP=\"%s\"\n", int_end);
	write(fd,sprintf_buf, strlen(sprintf_buf));
	sprintf(sprintf_buf,"PUBLICSTARTIP=\"%s\"\n", ext_start);
	write(fd,sprintf_buf, strlen(sprintf_buf));
	sprintf(sprintf_buf,"PUBLICENDIP=\"%s\"\n", ext_end);
	write(fd,sprintf_buf, strlen(sprintf_buf));
//	sprintf(sprintf_buf,"PUBLICIP=\"%s\"\n", PublicIPs);
//	write(fd,sprintf_buf, strlen(sprintf_buf));
	write(fd,PublicIPs, strlen(PublicIPs));
	close(fd);
	free(PublicIPs);
}

