#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "trendchip_load.h"
#include "trendchip-def.h"
#include "gb-constants.h"

static void route_exe(int type);

int TcWebApiRoute_write();



static int userDefinedMaxCount;

void init();
void writeBack();

void init()
{
	FILE *fp;
	char buf[4];
	
	fp = fopen("/etc/route.count", "r");

	if(fp==NULL){
		return ;
	}
	else{
		fgets(buf, 4 ,fp);
		userDefinedMaxCount = atoi(buf);
		fclose(fp);
	}
#if 0
	if(fp == NULL)
		fprintf(stderr, "init(): File open failed\n");
	else
	{
		while((tmp = fgetc(fp)) != EOF)
		{
			i++;
			buf[i] = tmp;
		}
#if 0		
		for(i = 0; i < 4; i++)
			fprintf(stderr, "buf[%d] = %d\n", i, buf[i]);
#endif
		// use fgetc(), ruturn each char (presented by ASCII code)
		if(buf[2] < 47)	// 47 means returned negative int is a non-acceptable value
			userDefinedMaxCount = buf[1] - 48;
		// buf[1] stores the first digit; buf[2] stores the second digit.
		else
			userDefinedMaxCount = (buf[1] - 48)*10 + buf[2] - 48;
		fprintf(stderr, "init(): userDefinedMaxCount = %d\n", userDefinedMaxCount);
		fclose(fp);
	}
#endif 
}
void writeBack()
{
	FILE *fp;
	char output[16];
	
	fp = fopen("/etc/route.count", "r+");

	if(fp == NULL)
		fprintf(stderr, "writeBack(): File open failed\n");
	else
	{
		sprintf(output, "%d", userDefinedMaxCount);
		fputs(output, fp);
		fprintf(stderr, "writeBack(): userDefinedMaxCount = %d\n", userDefinedMaxCount);
		fclose(fp);
	}
}

void route_list()
{
	int i,total = 0;
        char tmp[16];
	read_route_conf();
	
	sprintf(sprintf_buf,"<table width=\"580\" border=\"1\""
		"align=center cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\"><INPUT TYPE=\"HIDDEN\" NAME=\"routeFlg\" VALUE=\"0\"><INPUT TYPE=\"HIDDEN\" NAME=\"delnum\" VALUE=\"0\">");
	ASP_OUTPUT(sprintf_buf);
	sprintf(sprintf_buf,"<tr height=\"30\"><td width=\"40\"  align=center class=\"tabdata\"><strong>#</strong></td><td width=\"110\" align=center class=\"tabdata\"><strong>"
	"<font color=\"#000000\">Dest IP</font></strong></td><td width=\"50\" align=center class=\"tabdata\"><strong>"
	"<font color=\"#000000\">Mask</font></strong></td><td width=\"110\" align=center class=\"tabdata\"><strong>");
	ASP_OUTPUT(sprintf_buf);
	sprintf(sprintf_buf, "<font color=\"#000000\">Gateway IP</font></strong></td><td width=\"50\"  align=center class=\"tabdata\"><strong>"
	"<font color=\"#000000\">Metric</font></strong></td><td width=\"70\"  align=center class=\"tabdata\"><strong>"
	"<font color=\"#000000\">Device</font></strong></td><td width=\"50\" align=center class=\"tabdata\"><strong>");
	ASP_OUTPUT(sprintf_buf);
	sprintf(sprintf_buf, "<font color=\"#000000\">Use</font></strong></td><td width=\"45\"  align=center class=\"tabdata\"><strong>"
	"<font color=\"#000000\">Edit</font></strong></td><td align=center class=\"tabdata\"><strong>"
	"<font color=\"#000000\">Drop</font></strong></td></tr>");
	ASP_OUTPUT(sprintf_buf);

	for(i = 0; i<50; i++)
	{
		if(web_api->route[i]->dest_ip[0] == 0)
			continue;
		total ++;
                /* if no gateway -> display N/A  shnwind add 2008.4.17*/
		strcpy(tmp,web_api->route[i]->gw_ip);
		if (!strlen(tmp))
		  strcpy(tmp,"N/A");   
		if(web_api->route[i]->isdefault == 0)
		{
		sprintf(sprintf_buf,"<tr><td align='center' class='tabdata'>%d</td>"
							 "<td align='center' class='tabdata'>&nbsp;%s</td>"
							 "<td align='center' class='tabdata'>&nbsp;%s</td>"
							 "<td align='center' class='tabdata'>&nbsp;%s</td>"
							 "<td align='center' class='tabdata'>&nbsp;%d</td>"
							 "<td align='center' class='tabdata'>&nbsp;%s</td>"
							 "<td align='center' class='tabdata'>&nbsp;%d</td>"
							 "<td align='center' class='tabdata'><IMG onclick=\"window.location='/cgi-bin/adv_static_route.asp?editnum=%d'\"; src='/pen.gif'></td>"
							 "<td align='center' class='tabdata'><IMG onclick=\"window.location='/cgi-bin/adv_routing_table.asp?delnum=%d'\"; src='/cross.gif'></td></tr>",total,web_api->route[i]->dest_ip,web_api->route[i]->netmask,tmp,atoi(web_api->route[i]->metric),web_api->route[i]->gw_interface,atoi(web_api->route[i]->use),i,i);
		}
		else
		{
		sprintf(sprintf_buf,"<tr><td align='center' class='tabdata'>%d</td>"
							 "<td align='center' class='tabdata'>&nbsp;%s</td>"
							 "<td align='center' class='tabdata'>&nbsp;%s</td>"
							 "<td align='center' class='tabdata'>&nbsp;%s</td>"
							 "<td align='center' class='tabdata'>&nbsp;%d</td>"
							 "<td align='center' class='tabdata'>&nbsp;%s</td>"
							 "<td align='center' class='tabdata'>&nbsp;%d</td>"
							 "<td align='center' class='tabdata'>&nbsp;</td>"
							 "<td align='center' class='tabdata'>&nbsp;</td></tr>",total,web_api->route[i]->dest_ip,web_api->route[i]->netmask,tmp,atoi(web_api->route[i]->metric),web_api->route[i]->gw_interface,atoi(web_api->route[i]->use));
		
		}	
		ASP_OUTPUT(sprintf_buf);
	}

	sprintf(sprintf_buf,"</table>");
	ASP_OUTPUT(sprintf_buf);
}

char *TcWebApiRoute_get(int id)
{
	ROUTE_SETUP *pt = web_api->route[web_api->route_index];
			
	switch(id)
	{
		case GB_C_ROUTING_LIST:
			route_list();
			return ASP_OK;
		case GB_C_ROUTING_DESTINATIONIP:
			if(strlen(pt->dest_ip))
				sprintf(sprintf_buf,"\"%s\"",pt->dest_ip);
			else
				sprintf(sprintf_buf,"\"0.0.0.0\"");
			break;
		case GB_C_ROUTING_NETMASK:
			if(strlen(pt->netmask))
				sprintf(sprintf_buf,"\"%s\"",pt->netmask);
			else
				sprintf(sprintf_buf,"\"0.0.0.0\"");
			break;
		case GB_C_ROUTING_GATEWAY:
			if(strlen(pt->gw_ip))
				sprintf(sprintf_buf,"\"%s\"",pt->gw_ip);
			else
				sprintf(sprintf_buf,"\"0.0.0.0\"");
			break;
		case GB_C_ROUTING_PVCGATEWAYPVCNO:
			if(!strcmp(pt->gw_type,"gw") || pt->gw_type[0]==0)
				sprintf(sprintf_buf,"CHECKED");
			break;
		case GB_C_ROUTING_PVCGATEWAYPVCYES:
			if(!strcmp(pt->gw_type,"dev"))
				sprintf(sprintf_buf,"CHECKED");
			break;
		case GB_C_ROUTING_PVC:
			{
				int i,vc=0,len=0;
				if(pt->gw_interface[0] == 'n')
					sscanf(pt->gw_interface,"nas%d",&vc);
				else if(pt->gw_interface[0] == 'p')
					sscanf(pt->gw_interface,"ppp%d",&vc);
				for(i = 0;i<8;i++)
				{
					if(i == vc)
						len +=sprintf(sprintf_buf+len,"<OPTION SELECTED>PVC%d",i);
					else
						len +=sprintf(sprintf_buf+len,"<OPTION >PVC%d",i);
				}
			}
			break;
		case GB_C_ROUTING_METRIC:
			if(strlen(pt->metric))
				sprintf(sprintf_buf,"\"%s\"",pt->metric);
			else
				sprintf(sprintf_buf,"\"0\"");
			break;
		case GB_C_ROUTING_ANNOUNCEDINRIP:
			if(!strcmp(pt->announced_in_rip,val_def->announcedinrip_yes))
				sprintf(sprintf_buf,"<OPTION >%s<OPTION SELECTED>%s",val_def->announcedinrip_no,val_def->announcedinrip_yes);
			else
				sprintf(sprintf_buf,"<OPTION SELECTED>%s<OPTION >%s",val_def->announcedinrip_no,val_def->announcedinrip_yes);
			break;
			
	}
	ASP_OUTPUT(sprintf_buf);
	return ASP_OK;
}

int TcWebApiRoute_set(int id,char *value)
{
	char *ret;
	ROUTE_SETUP *pt = web_api->route[web_api->route_index];
	
	if(id == GB_C_ROUTING_INDEX)
	{
		fprintf(stderr, "value = %s\n", value);
		ret = ASP_FORM(value);
		fprintf(stderr, "ret = %s\n", ret);
		if(ret)
		{
			web_api->route_index = atoi(ret);
			fprintf(stderr,"----route edit/del index:%d\n",web_api->route_index);
		}
		else
		{
			int i;
			web_api->route_index = 49;
			for(i =0 ; i<50; i++)
			{
				if(web_api->route[i]->dest_ip[0] == 0)
				{
					web_api->route_index = i;
					fprintf(stderr,"----route add index:%d\n",web_api->route_index);
					break;
				}
			}
		}
		return 0;
	}

	
	ret = ASP_FORM(value);
	if(ret ==NULL)
		return -1;	
	switch (id)
	{
		case GB_C_ROUTING_DESTINATIONIP:
			strncpy(pt->dest_ip,ret,16);
			break;
		case GB_C_ROUTING_NETMASK:
			strncpy(pt->netmask,ret,16);
			break;
		case GB_C_ROUTING_GATEWAY:
			strncpy(pt->gw_ip,ret,16);
			break;
		case GB_C_ROUTING_PVCGATEWAY:
			if(!strcmp(ret,val_def->pvcgateway_no))
			{
				strncpy(pt->gw_type,"gw",16);
				strncpy(pt->gw_interface,"eth0",16);
			}
			else
			{
				strncpy(pt->gw_type,"dev",16);
				memset(pt->gw_ip,0,16);
			}
			break;
		case GB_C_ROUTING_PVC:
			{
				int vc = atoi(&ret[3]);
				if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_pppoallc) ||
				   !strcmp(web_api->wan[vc]->encap,val_def->wan_encap_pppoavc) ||
				   !strcmp(web_api->wan[vc]->encap,val_def->wan_encap_pppoellc) ||
				   !strcmp(web_api->wan[vc]->encap,val_def->wan_encap_pppoevc))
					sprintf(pt->gw_interface,"ppp%d",vc);
				else
					sprintf(pt->gw_interface,"nas%d",vc);
			}
			break;
		case GB_C_ROUTING_METRIC:
			strncpy(pt->metric,ret,8);
			break;
		case GB_C_ROUTING_ANNOUNCEDINRIP:
			strncpy(pt->announced_in_rip,ret,16);
			break; 
	}
	return 0;
}

static void route_exe(int type)
{
	ROUTE_SETUP *pt = web_api->route[web_api->route_index];
	char opt[4];
	int len=0;
	
	if(type == 0)
		strcpy(opt,"del");
	else
		strcpy(opt,"add");
	
	len +=sprintf(sprintf_buf+len,"/sbin/route %s -net %s netmask %s ",opt,pt->dest_ip,pt->netmask);
	if(!strcmp(pt->gw_type,"gw"))
		len +=sprintf(sprintf_buf+len,"gw %s",pt->gw_ip);
	else
		len +=sprintf(sprintf_buf+len,"dev %s",pt->gw_interface);
	if((atoi(pt->metric))>0)
		len +=sprintf(sprintf_buf+len," metric %s",pt->metric);

	fprintf(stderr,"----%s route %d :%s\n",opt,web_api->route_index,sprintf_buf);	
	asp_execute(sprintf_buf);
}

int TcWebApiRoute_execute(int id)
{
	ROUTE_SETUP *pt = web_api->route[web_api->route_index];
	
	if(id == GB_C_ROUTING_DELETE)
	{
		if(pt->dest_ip[0] == 0)
			return 0;
		route_exe(0);
		userDefinedMaxCount--;
		writeBack();
		memset(pt,0,sizeof(ROUTE_SETUP));

	}
	else if(id == GB_C_ROUTING_ADD)
	{
		init();
		if(userDefinedMaxCount >= 16)
		{
			ASP_OUTPUT("<script language=\"JavaScript\"> alert(\"User defined routing rules reach maximum limit !\"); </script>");
			return 1;
		}
		else
		{
			route_exe(1);
			userDefinedMaxCount++;
			writeBack();
		}
	}
	TcWebApiRoute_write();
	return 0;
}

int TcWebApiRoute_write()
{
	int fd,i;

	fd = open("/etc/route.sh",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd ==-1)
		return -1;
	for(i = 0;i<50;i++)
	{
		ROUTE_SETUP *pt = web_api->route[i];
		int len = 0;
		if(pt->dest_ip[0]==0 || pt->isdefault ==1)
			continue;
		len +=sprintf(sprintf_buf+len,"/sbin/route add -net %s netmask %s ",pt->dest_ip,pt->netmask);
		if(!strcmp(pt->gw_type,"gw"))
			len +=sprintf(sprintf_buf+len,"gw %s",pt->gw_ip);
		else
			len +=sprintf(sprintf_buf+len,"dev %s",pt->gw_interface);
		if((atoi(pt->metric))>0)
			len +=sprintf(sprintf_buf+len," metric %s",pt->metric);
		write(fd,sprintf_buf,strlen(sprintf_buf));
		write(fd,"\n",1);
	}
	close(fd);
	if( chmod("/etc/route.sh", S_IRUSR|S_IWUSR|S_IXUSR) ==0)
	{
	return 0;
}
	else
	{
		fprintf(stderr,"chmod /etc/route.sh fail.\r\n");
		return -1;
	}
	
}
