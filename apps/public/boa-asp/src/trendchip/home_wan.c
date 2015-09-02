
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

void wan_execute_dynamic();
void wan_execute_static();
void wan_execute_ppp();
void wan_execute_bridge();
int TcWebApiWan_write(int pvc);
void wan_pvclist();
void pharseigdconf(char * igdconfbuff,char * target,char *value);
void restart_upnp(char *wanif);

static int cur_vc = 0;

/*	Just for adsl link state checkup	*/
typedef struct _INFO{
	char adsl_firmwarever[128];
	char line_state[16];
	char modulation[32];
	char annex[16];
	char snr_down[16];
	char snr_up[16];
	char line_down[16];
	char line_up[16];
	char data_down[16];
	char data_up[16];
}INFO;
static INFO info;


#define GET_VC (web_api->wan_typechange ? 8 : cur_vc)
#define INIT_STATUS \
	if (!ASP_ISPOST){ \
			cur_vc =0; \
			memset(web_api->wan[8],0,sizeof(INTERNET_SETTINGS)); \
	}

/*wan:0x00~0xFF reserve to wan*/
char *TcWebApiWan_get(int id)
{
	int i,vc;
	char buf[256];
	char select[5][9]={"","","","",""};
	
	INIT_STATUS;
	vc = GET_VC;
	switch (id)
	{
		case GB_C_WAN_PVC0_VCI: //lee 12-17 add check vci ,different PVC can't use same VCI
			sprintf(buf,"\"%lu\"",web_api->wan[0]->vci);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_PVC1_VCI: 
			sprintf(buf,"\"%lu\"",web_api->wan[1]->vci);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_PVC2_VCI: 
			sprintf(buf,"\"%lu\"",web_api->wan[2]->vci);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_PVC3_VCI:
			sprintf(buf,"\"%lu\"",web_api->wan[3]->vci);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_PVC4_VCI:
			sprintf(buf,"\"%lu\"",web_api->wan[4]->vci);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_PVC5_VCI: 
			sprintf(buf,"\"%lu\"",web_api->wan[5]->vci);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_PVC6_VCI: 
			sprintf(buf,"\"%lu\"",web_api->wan[6]->vci);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_PVC7_VCI: 
			sprintf(buf,"\"%lu\"",web_api->wan[7]->vci);
			ASP_OUTPUT(buf);
			break;
										
		case GB_C_WAN_VC:
			for(i = 0 ; i<8 ; i++)
			{
				if(i == cur_vc)
					sprintf(buf,"<OPTION SELECTED>%s%d",val_def->wan_vc,i);
				else
					sprintf(buf,"<OPTION >%s%d",val_def->wan_vc,i);
				ASP_OUTPUT(buf);
			}
			break;	
		case GB_C_WAN_ACTIVATED:
			//fprintf(stderr,"----wan:%s,vc:%d,cur_vc:%d,typechange:%d\n",web_api->wan[vc]->status,vc,cur_vc,web_api->wan_typechange);
			if(!strcmp(web_api->wan[vc]->status,val_def->wan_activated))
				ASP_OUTPUT("CHECKED");
			break;
		case GB_C_WAN_DEACTIVATED:
			if(!strcmp(web_api->wan[vc]->status,val_def->wan_deactivated))
				ASP_OUTPUT("CHECKED");
			break;	
		case GB_C_WAN_VPI:
			sprintf(buf,"\"%lu\"",web_api->wan[vc]->vpi);
			ASP_OUTPUT(buf);
			break;	
		case GB_C_WAN_VCI:
			sprintf(buf,"\"%lu\"",web_api->wan[vc]->vci);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_QOS:
			if(!strcmp(web_api->wan[vc]->qos,val_def->wan_cbr))
				sprintf(buf,"<OPTION>%s<OPTION SELECTED>%s<OPTION>%s<OPTION>%s",val_def->wan_ubr,val_def->wan_cbr,val_def->wan_vbr, val_def->wan_nrtvbr);
			else if(!strcmp(web_api->wan[vc]->qos,val_def->wan_vbr))
				sprintf(buf,"<OPTION>%s<OPTION>%s<OPTION SELECTED>%s<OPTION>%s",val_def->wan_ubr,val_def->wan_cbr,val_def->wan_vbr, val_def->wan_nrtvbr);
			else if(!strcmp(web_api->wan[vc]->qos, "nrt-vbr"))
				sprintf(buf,"<OPTION>%s<OPTION>%s<OPTION>%s<OPTION SELECTED>%s",val_def->wan_ubr,val_def->wan_cbr,val_def->wan_vbr, val_def->wan_nrtvbr);
			else
				sprintf(buf,"<OPTION SELECTED>%s<OPTION>%s<OPTION >%s<OPTION>%s",val_def->wan_ubr,val_def->wan_cbr,val_def->wan_vbr, val_def->wan_nrtvbr);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_PCR:
			sprintf(buf,"\"%lu\"",web_api->wan[vc]->pcr);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_SCR:
			sprintf(buf,"\"%lu\"",web_api->wan[vc]->scr);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_MBS:
			sprintf(buf,"\"%lu\"",web_api->wan[vc]->mbs);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_ISP:
			if(web_api->wan[vc]->isp[0] == 0)
				return val_def->wan_dynamic_ip;
			else
				return web_api->wan[vc]->isp;
		case GB_C_WAN_DYNAMICIP:
			if(!strcmp(web_api->wan[vc]->isp,val_def->wan_dynamic_ip) || web_api->wan[vc]->isp[0] == 0)
				ASP_OUTPUT("CHECKED");
			break;
		case GB_C_WAN_STATICIP:
			if(!strcmp(web_api->wan[vc]->isp,val_def->wan_static_ip))
				ASP_OUTPUT("CHECKED");
			break;	
		case GB_C_WAN_PPPOE:
			if(!strcmp(web_api->wan[vc]->isp,val_def->wan_pppoe))
				ASP_OUTPUT("CHECKED");
			break;	
		case GB_C_WAN_BRIDGEMODE:
			if(!strcmp(web_api->wan[vc]->isp,val_def->wan_bridge))
				ASP_OUTPUT("CHECKED");
			break;	
		case GB_C_WAN_ENCAPSULATION:
			if(!strcmp(web_api->wan[vc]->isp,val_def->wan_dynamic_ip) || !strcmp(web_api->wan[vc]->isp,val_def->wan_static_ip) || (web_api->wan[vc]->isp[0] == 0))
			{
				if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_bridgevc))
					sprintf(buf,"<OPTION >%s<OPTION SELECTED>%s<OPTION >%s<OPTION >%s",val_def->wan_encap_bridgellc,val_def->wan_encap_bridgevc,val_def->wan_encap_routellc,val_def->wan_encap_routevc);
				else if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_routellc))
					sprintf(buf,"<OPTION >%s<OPTION >%s<OPTION SELECTED>%s<OPTION >%s",val_def->wan_encap_bridgellc,val_def->wan_encap_bridgevc,val_def->wan_encap_routellc,val_def->wan_encap_routevc);
				else if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_routevc))
					sprintf(buf,"<OPTION >%s<OPTION >%s<OPTION >%s<OPTION SELECTED>%s",val_def->wan_encap_bridgellc,val_def->wan_encap_bridgevc,val_def->wan_encap_routellc,val_def->wan_encap_routevc);
				else
					sprintf(buf,"<OPTION SELECTED>%s<OPTION >%s<OPTION >%s<OPTION >%s",val_def->wan_encap_bridgellc,val_def->wan_encap_bridgevc,val_def->wan_encap_routellc,val_def->wan_encap_routevc);
				//fprintf(stderr,"---- %s %s\n",buf,web_api->wan[vc]->encap);
				ASP_OUTPUT(buf);
			}
			else if(!strcmp(web_api->wan[vc]->isp,val_def->wan_pppoe))
			{
				if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_pppoevc))
					sprintf(buf,"<OPTION >%s<OPTION SELECTED>%s<OPTION >%s<OPTION >%s",val_def->wan_encap_pppoellc,val_def->wan_encap_pppoevc,val_def->wan_encap_pppoallc,val_def->wan_encap_pppoavc);
				else if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_pppoallc))
					sprintf(buf,"<OPTION >%s<OPTION >%s<OPTION SELECTED>%s<OPTION >%s",val_def->wan_encap_pppoellc,val_def->wan_encap_pppoevc,val_def->wan_encap_pppoallc,val_def->wan_encap_pppoavc);
				else if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_pppoavc))
					sprintf(buf,"<OPTION >%s<OPTION >%s<OPTION >%s<OPTION SELECTED>%s",val_def->wan_encap_pppoellc,val_def->wan_encap_pppoevc,val_def->wan_encap_pppoallc,val_def->wan_encap_pppoavc);
				else
					sprintf(buf,"<OPTION SELECTED>%s<OPTION >%s<OPTION >%s<OPTION >%s",val_def->wan_encap_pppoellc,val_def->wan_encap_pppoevc,val_def->wan_encap_pppoallc,val_def->wan_encap_pppoavc);
				ASP_OUTPUT(buf);
			}
			else if(!strcmp(web_api->wan[vc]->isp,val_def->wan_bridge))
			{
				if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_bridgevc))
					sprintf(buf,"<OPTION >%s<OPTION SELECTED>%s",val_def->wan_encap_bridgellc,val_def->wan_encap_bridgevc);
				else
					sprintf(buf,"<OPTION SELECTED>%s<OPTION >%s",val_def->wan_encap_bridgellc,val_def->wan_encap_bridgevc);
				ASP_OUTPUT(buf);	
			}
			break;
		case GB_C_WAN_CONNTYPE_DYN:
		case GB_C_WAN_CONNTYPE_STATIC:
			if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_bridgevc))
				sprintf(buf,"<OPTION >%s<OPTION SELECTED>%s<OPTION >%s<OPTION >%s",val_def->wan_encap_bridgellc,val_def->wan_encap_bridgevc,val_def->wan_encap_routellc,val_def->wan_encap_routevc);
			else if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_routellc))
				sprintf(buf,"<OPTION >%s<OPTION >%s<OPTION SELECTED>%s<OPTION >%s",val_def->wan_encap_bridgellc,val_def->wan_encap_bridgevc,val_def->wan_encap_routellc,val_def->wan_encap_routevc);
			else if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_routevc))
				sprintf(buf,"<OPTION >%s<OPTION >%s<OPTION >%s<OPTION SELECTED>%s",val_def->wan_encap_bridgellc,val_def->wan_encap_bridgevc,val_def->wan_encap_routellc,val_def->wan_encap_routevc);
			else
				sprintf(buf,"<OPTION SELECTED>%s<OPTION >%s<OPTION >%s<OPTION >%s",val_def->wan_encap_bridgellc,val_def->wan_encap_bridgevc,val_def->wan_encap_routellc,val_def->wan_encap_routevc);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_CONNTYPE_PPP:
			if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_pppoevc))
				sprintf(buf,"<OPTION >%s<OPTION SELECTED>%s<OPTION >%s<OPTION >%s",val_def->wan_encap_pppoellc,val_def->wan_encap_pppoevc,val_def->wan_encap_pppoallc,val_def->wan_encap_pppoavc);
			else if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_pppoallc))
				sprintf(buf,"<OPTION >%s<OPTION >%s<OPTION SELECTED>%s<OPTION >%s",val_def->wan_encap_pppoellc,val_def->wan_encap_pppoevc,val_def->wan_encap_pppoallc,val_def->wan_encap_pppoavc);
			else if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_pppoavc))
				sprintf(buf,"<OPTION >%s<OPTION >%s<OPTION >%s<OPTION SELECTED>%s",val_def->wan_encap_pppoellc,val_def->wan_encap_pppoevc,val_def->wan_encap_pppoallc,val_def->wan_encap_pppoavc);
			else
				sprintf(buf,"<OPTION SELECTED>%s<OPTION >%s<OPTION >%s<OPTION >%s",val_def->wan_encap_pppoellc,val_def->wan_encap_pppoevc,val_def->wan_encap_pppoallc,val_def->wan_encap_pppoavc);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_CONNTYPE_BRIDGE:
			if(!strcmp(web_api->wan[vc]->encap,val_def->wan_encap_bridgevc))
				sprintf(buf,"<OPTION >%s<OPTION SELECTED>%s",val_def->wan_encap_bridgellc,val_def->wan_encap_bridgevc);
			else
				sprintf(buf,"<OPTION SELECTED>%s<OPTION >%s",val_def->wan_encap_bridgellc,val_def->wan_encap_bridgevc);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_NATENABLE:
			if(!strcmp(web_api->wan[vc]->nat_enable,val_def->wan_natenable))
				sprintf(buf,"<OPTION >%s<OPTION SELECTED>%s",val_def->wan_natdisable,val_def->wan_natenable);
			else
				sprintf(buf,"<OPTION SELECTED>%s<OPTION >%s",val_def->wan_natdisable,val_def->wan_natenable);
			//fprintf(stderr,"----nat:%s,%s\n",web_api->wan[vc]->nat_enable,val_def->wan_natenable);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_DEFAULTROUTEYES:
			//if(!strcmp(web_api->wan[vc]->def_route,val_def->wan_defroute_check) || web_api->wan[vc]->def_route[0] == 0)
			if(!strcmp(web_api->wan[vc]->def_route,val_def->wan_defroute_check))
				ASP_OUTPUT("CHECKED");
			break;
		case GB_C_WAN_DEFAULTROUTENO:
			if(!strcmp(web_api->wan[vc]->def_route,val_def->wan_defroute_uncheck) || web_api->wan[vc]->def_route[0] == 0)
				ASP_OUTPUT("CHECKED");
			break;	
		case GB_C_WAN_MTU:
			sprintf(buf,"\"%lu\"",web_api->wan[vc]->mtu);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_RIPVERSION:
			if(!strcmp(web_api->wan[vc]->rip_ver,val_def->wan_rip2b))
				sprintf(buf,"<OPTION >%s<OPTION SELECTED>%s<OPTION >%s",val_def->wan_rip1,val_def->wan_rip2b,val_def->wan_rip2m);
			else if(!strcmp(web_api->wan[vc]->rip_ver,val_def->wan_rip2m))
				sprintf(buf,"<OPTION >%s<OPTION >%s<OPTION SELECTED>%s",val_def->wan_rip1,val_def->wan_rip2b,val_def->wan_rip2m);
			else
				sprintf(buf,"<OPTION SELECTED>%s<OPTION >%s<OPTION >%s",val_def->wan_rip1,val_def->wan_rip2b,val_def->wan_rip2m);

			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_DIRECTION:		
			if(!strcmp(web_api->wan[vc]->direction,val_def->wan_direction_both))
				sprintf(select[1],"SELECTED");
			else if(!strcmp(web_api->wan[vc]->direction,val_def->wan_direction_in))
				sprintf(select[2],"SELECTED");
			else if(!strcmp(web_api->wan[vc]->direction,val_def->wan_direction_out))
				sprintf(select[3],"SELECTED");
			else
				sprintf(select[0],"SELECTED");

			sprintf(buf,"<OPTION %s>%s<OPTION %s>%s<OPTION %s>%s<OPTION %s>%s",select[0],val_def->wan_direction_none,select[1],val_def->wan_direction_both,select[2],val_def->wan_direction_in,select[3],val_def->wan_direction_out);

			ASP_OUTPUT(buf);			
			break;
		case GB_C_WAN_IGMP:
			if(!strcmp(web_api->wan[vc]->igmp,"IGMP v1"))
				sprintf(buf,"<OPTION >Disabled<OPTION SELECTED>IGMP v1<OPTION >IGMP v2");
			else if(!strcmp(web_api->wan[vc]->igmp,"IGMP v2"))
				sprintf(buf,"<OPTION >Disabled<OPTION >IGMP v1<OPTION SELECTED>IGMP v2");
			else
				sprintf(buf,"<OPTION SELECTED>Disabled<OPTION >IGMP v1<OPTION >IGMP v2");
			ASP_OUTPUT(buf);
			
			break;
		case GB_C_WAN_IPADDRESS:
			sprintf(buf,"\"%s\"",web_api->wan[vc]->ip_addr);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_NETMASK:
			sprintf(buf,"\"%s\"",web_api->wan[vc]->netmask);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_GATEWAY:
			sprintf(buf,"\"%s\"",web_api->wan[vc]->gateway);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_USRNAME:
			//checkQuotationMarks(web_api->wan[vc]->usr_name,strlen(web_api->wan[vc]->usr_name));
			sprintf(buf,"\"%s\"",web_api->wan[vc]->usr_name);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_PASSWORD:
			//checkQuotationMarks(web_api->wan[vc]->passwd,strlen(web_api->wan[vc]->passwd));
			sprintf(buf,"\"%s\"",web_api->wan[vc]->passwd);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_CONNECTIONONACTIVE:
			if(!strcmp(web_api->wan[vc]->connection,val_def->wan_connect_keepalive) || web_api->wan[vc]->connection[0] == 0)
				ASP_OUTPUT("CHECKED");
			break;
		case GB_C_WAN_CONNECTIONONDEMAND:
			if(!strcmp(web_api->wan[vc]->connection,val_def->wan_connect_ondemand))
				ASP_OUTPUT("CHECKED");
			break;	
		case GB_C_WAN_CLOSEIFIDLE:
			sprintf(buf,"\"%lu\"",web_api->wan[vc]->closeifidle);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_MSS:
			sprintf(buf,"\"%lu\"",web_api->wan[vc]->mss);
			ASP_OUTPUT(buf);
			break;
		case GB_C_WAN_PPPGETIPSTATIC:
			if(!strcmp(web_api->wan[vc]->ppp_get_ip,val_def->wan_pppgetip_static) || web_api->wan[vc]->ppp_get_ip[0] == 0)
				ASP_OUTPUT("CHECKED");
			break;
		case GB_C_WAN_PPPGETIPDYNAMIC:
			if(!strcmp(web_api->wan[vc]->ppp_get_ip,val_def->wan_pppgetip_dynamic))
				ASP_OUTPUT("CHECKED");
			break;	
		case GB_C_WAN_PVCLIST:
			wan_pvclist();
			break;
		case GB_C_WAN_DELETE_BUTTON:
			if( (!strcmp(web_api->wan[vc]->def_route, val_def->wan_defroute_uncheck))
				||(strlen(web_api->wan[vc]->def_route) == 0)
				|| (!strcmp(web_api->wan[vc]->isp, val_def->wan_bridge)))
				ASP_OUTPUT("<INPUT TYPE=\"BUTTON\" NAME=\"DeleteBtn\" VALUE=\"DELETE\" onClick=\"doDelete()\">");

			break;
		default:
			fprintf(stderr,"Warring: TcWebApiWan_get select default.");
			break;
	}
	
	return ASP_OK;
}

int TcWebApiWan_set(int id,char*value)
{
	char *ret;
	int vc;
	
	ret = ASP_FORM(value);
	if (ret ==NULL)
		return -1;

	vc = GET_VC;
	switch (id)
	{
		case GB_C_WAN_VC:
			sscanf(ret,"PVC%d",&cur_vc);	
			break;	
		case GB_C_WAN_CHANGEISP:
			web_api->wan_typechange = 1;
			break;	
		case GB_C_WAN_STATUS:
			strncpy(web_api->wan[vc]->status,ret,16);
			break;
		case GB_C_WAN_VPI:
			web_api->wan[vc]->vpi = atol(ret);	
			break;	
		case GB_C_WAN_VCI:
			web_api->wan[vc]->vci = atol(ret);	
			break;
		case GB_C_WAN_QOS:
			strncpy(web_api->wan[vc]->qos,ret,16);
			if(strcmp(web_api->wan[vc]->qos,val_def->wan_vbr) && strcmp(web_api->wan[vc]->qos, val_def->wan_nrtvbr))
			{
				web_api->wan[vc]->scr = 0;
				web_api->wan[vc]->mbs = 0;
			}
			break;
		case GB_C_WAN_PCR:
			web_api->wan[vc]->pcr = atol(ret);	
			break;
		case GB_C_WAN_SCR:
			web_api->wan[vc]->scr = atol(ret);	
			break;
		case GB_C_WAN_MBS:
			web_api->wan[vc]->mbs = atol(ret);	
			break;
		case GB_C_WAN_ISP:
			strncpy(web_api->wan[vc]->isp,ret,16);
			break;
		case GB_C_WAN_ENCAPSULATION:
			decode_uri(ret);
			strncpy(web_api->wan[vc]->encap,ret,32);
			break;
		case GB_C_WAN_NATENABLE:
			strncpy(web_api->wan[vc]->nat_enable,ret,16);
			break;
		case GB_C_WAN_DEFAULTROUTE:
			strncpy(web_api->wan[vc]->def_route,ret,16);
			break;
		case GB_C_WAN_MTU:
			web_api->wan[vc]->mtu = atol(ret);	
			break;
		case GB_C_WAN_RIPVERSION:
			strncpy(web_api->wan[vc]->rip_ver,ret,16);
			break;
		case GB_C_WAN_DIRECTION:
			strncpy(web_api->wan[vc]->direction,ret,16);
			break;
		case GB_C_WAN_IGMP:
			strncpy(web_api->wan[vc]->igmp,ret,16);
			break;
		case GB_C_WAN_IPADDRESS:
			strncpy(web_api->wan[vc]->ip_addr,ret,16);
			break;
		case GB_C_WAN_NETMASK:
			strncpy(web_api->wan[vc]->netmask,ret,16);
			break;
		case GB_C_WAN_GATEWAY:
			strncpy(web_api->wan[vc]->gateway,ret,16);
			break;
		case GB_C_WAN_USRNAME:
			ascii2punct(ret,128);
			strncpy(web_api->wan[vc]->usr_name,ret,128);
			break;
		case GB_C_WAN_PASSWORD:
			ascii2punct(ret,128);
			strncpy(web_api->wan[vc]->passwd,ret,128);
			break;
		case GB_C_WAN_CONNECTION:
			strncpy(web_api->wan[vc]->connection,ret,32);
			break;
		case GB_C_WAN_CLOSEIFIDLE:
			web_api->wan[vc]->closeifidle = atol(ret);
			break;
		case GB_C_WAN_MSS:
			web_api->wan[vc]->mss = atol(ret);
			break;
		case GB_C_WAN_PPPGETIP:
			strncpy(web_api->wan[vc]->ppp_get_ip,ret,16);
			break;
//		case GB_C_WAN_DELETE_BUTTON:
//			break;
		default:
			fprintf(stderr,"Warring: TcWebApiWan_set select default.");
			break;
	}
	
	return 0;
}

extern int write_ripd_conf();


int TcWebApiWan_execute(int id)
{
//	FILE *PidFile;

	int fd, zfd;
	char  autoconf[16]={0}; 
	FILE * cfp;
	char  igdconf[64]={0};
	char  WAN_IFNAME[16]={0};
	char  cur_wanif[16]={0};
	char  rest_wanif[16]={0};
	int i = 0, pid;
	char *ret;
	

	//lee 9-28 :clean tmp buffer
	memset(web_api->wan[8],0,sizeof(INTERNET_SETTINGS));

	switch(id)
	{
		case GB_C_WAN_SETTING:
			TcWebApiWan_write(cur_vc);
	
#ifdef ASP_DEBUG
			fprintf(stderr,"wan setting PVC%d\n",cur_vc);
#endif
			sprintf(sprintf_buf, "/var/script/wan_stop.sh %d", cur_vc);
			asp_execute(sprintf_buf);

			//If the current PVC actived default route then we need to delete other default route entry.
			if(strcmp(web_api->wan[cur_vc]->def_route, val_def->wan_defroute_check)	==	0)
			{
				for(i = 0 ; i < 8; i++)
				{
					if(i != cur_vc)	
					{
						if(strcmp(web_api->wan[i]->def_route, val_def->wan_defroute_check)	==	0)
						{
							memset(sprintf_buf, 0, sizeof(sprintf_buf));
							strcpy(sprintf_buf, "/sbin/route del default");
							asp_execute(sprintf_buf);
							strcpy(web_api->wan[i]->def_route, "No");
							TcWebApiWan_write(i);
							break;
						}
					}
					else
						continue; 
				}
			}
			web_api->ip_filter_on = check_MTU_change(web_api->ip_filter_on);/*shnwind add 2008.4.17*/
			sprintf(sprintf_buf, "/var/script/wan_start.sh %d", cur_vc);
			asp_execute(sprintf_buf);
                        web_api->nat_state=check_nat_enable(web_api->nat_state);/*shnwind add 2008.4.17*/
//			sprintf(sprintf_buf, "/var/script/nat_start.sh %d", cur_vc);
//			asp_execute(sprintf_buf);

			//sprintf(sprintf_buf, "/var/script/ipfilter_start.sh", cur_vc);
			//asp_execute(sprintf_buf);

			fd = open("/etc/route.sh", O_RDONLY);
			if(fd != -1)
			{
				close(fd);
				fd=0;
				asp_execute(". /etc/route.sh");
			}
			
			
			if(!(cfp=fopen("/etc/igd/igd.conf","r")))
			{
				fprintf(stderr,"upnp config file not found\n");
			}
			else
			{
				fread(igdconf,sizeof(char),64,cfp);
				fclose(cfp);
				pharseigdconf(igdconf,"autoconf=",autoconf);
				pharseigdconf(igdconf,"wanif=",WAN_IFNAME);
				
				if(!strcmp(web_api->wan[cur_vc]->isp, val_def->wan_pppoe))
					sprintf(cur_wanif, "ppp%d", cur_vc);
				else
					sprintf(cur_wanif, "nas%d", cur_vc);
				
				
				if((autoconf[0] == '1'))
  			{
  				if(strcmp(WAN_IFNAME, cur_wanif) == 0)
  				{
  						if(strcmp(web_api->wan[cur_vc]->status, val_def->wan_activated) == 0)
  						{	
  							break;
  						}
  						else
  						{
  							for(i=0 ;i < 8; i++)
  							{
  								if(!strcmp(web_api->wan[i]->status, val_def->wan_activated))
  								{
  									if(!strcmp(web_api->wan[i]->isp, val_def->wan_pppoe))
											sprintf(rest_wanif, "ppp%d", i);
										else
											sprintf(rest_wanif, "nas%d", i);
											
										if(strcmp(WAN_IFNAME, rest_wanif) != 0)			
  										restart_upnp(rest_wanif);
  									break;
  								}
  							}
  						}		
  				}
  				else
  				{
  						if(strcmp(web_api->wan[cur_vc]->status, val_def->wan_activated) == 0)
  						{
  							for(i=0; i <= cur_vc; i++)
  							{
  								if(!strcmp(web_api->wan[i]->status, val_def->wan_activated))
  								{
  									if(i == cur_vc)
  									{
  										if(!strcmp(web_api->wan[i]->isp, val_def->wan_pppoe))
												sprintf(rest_wanif, "ppp%d", i);
											else
												sprintf(rest_wanif, "nas%d", i);
												
  										restart_upnp(rest_wanif);
  									}
  									else
  										break;
  								}
  							}
  						}
  						else
  						{
  							break;
  						}  								
  				}
  			}
			}
					
			break;
		case GB_C_WAN_DELETE:
			sprintf(sprintf_buf, "/var/script/wan_stop.sh %d", cur_vc);
			asp_execute(sprintf_buf);

			sprintf(sprintf_buf, "/bin/rm /etc/isp%d.conf", cur_vc);
			asp_execute(sprintf_buf);

			memset(web_api->wan[cur_vc], 0, sizeof(INTERNET_SETTINGS));
			strcpy(web_api->wan[cur_vc]->status,val_def->wan_deactivated);
			break;
	}
	
	write_ripd_conf();

	ret = asp_readall("/var/run/zebra.pid");
	if(ret)
	{
		pid = atoi(ret);
		kill(pid,SIGTERM);
		free(ret);
	}
	ret = asp_readall("/var/run/ripd.pid");
	if(ret)
	{
		pid = atoi(ret);
		kill(pid,SIGTERM);
		free(ret);
	}
	if((zfd = open("/etc/zebra.conf",O_RDONLY)) != -1)
	{
		close(zfd);
		system("/userfs/bin/zebra -f /etc/zebra.conf -i /var/run/zebra.pid &");
		system("/userfs/bin/ripd -f /etc/ripd.conf -i /var/run/ripd.pid &");
	}
#if 0
	//wan_stop.sh_Start
	sprintf(sprintf_buf,"/bin/iptables -D FORWARD -o nas%d -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu", cur_vc);
	asp_execute(sprintf_buf);
	sprintf(sprintf_buf,"/bin/iptables -D FORWARD -o ppp%d -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu", cur_vc);
	asp_execute(sprintf_buf);
	sprintf(sprintf_buf,"/bin/iptables -t nat -D POSTROUTING -o nas%d -j MASQUERADE", cur_vc);
	asp_execute(sprintf_buf);
	sprintf(sprintf_buf,"/bin/iptables -t nat -D POSTROUTING -o ppp%d -j MASQUERADE", cur_vc);
	asp_execute(sprintf_buf);
	sprintf(sprintf_buf,"/bin/iptables -t nat -D POSTROUTING -j ADDRMAP_POS%d", cur_vc);
	asp_execute(sprintf_buf);
	sprintf(sprintf_buf,"/bin/iptables -t nat -D POSTROUTING -j VS_PRE%d", cur_vc);
	asp_execute(sprintf_buf);
	sprintf(sprintf_buf,"/bin/iptables -t nat -D POSTROUTING -j DMZ_PRE%d", cur_vc);
	asp_execute(sprintf_buf);
	sprintf(sprintf_buf,"/bin/iptables -t nat -D POSTROUTING -j ADDRMAP_PRE%d", cur_vc);
	asp_execute(sprintf_buf);
	sprintf(sprintf_buf,"/bin/iptables -t nat -F VS_PRE%d", cur_vc);
	asp_execute(sprintf_buf);
	sprintf(sprintf_buf,"/bin/iptables -t nat -F DMZ_PRE%d", cur_vc);
	asp_execute(sprintf_buf);
	sprintf(sprintf_buf,"/bin/iptables -t nat -F ADDRMAP_PRE%d", cur_vc);
	asp_execute(sprintf_buf);
	sprintf(sprintf_buf,"/bin/iptables -t nat -F ADDRMAP_POS%d", cur_vc);
	asp_execute(sprintf_buf);
	sprintf(sprintf_buf,"/bin/iptables -F PKT_FLT");
	asp_execute(sprintf_buf);

	sprintf(sprintf_buf,"/var/run/ppp%d.pid", cur_vc);
	PidFile = fopen(sprintf_buf, "r");
	if(PidFile)
	{
		fclose(PidFile);
		sprintf(sprintf_buf,"kill -15 `cat /var/run/ppp%d.pid`", cur_vc);
		asp_execute(sprintf_buf);
		asp_execute("sleep 1");
		sprintf(sprintf_buf,"/var/run/ppp%d.pid", cur_vc);
		PidFile = fopen(sprintf_buf, "r");
		if(PidFile)
		{
			fclose(PidFile);
			sprintf(sprintf_buf,"kill -9 `cat /var/run/ppp%d.pid`", cur_vc);
			asp_execute(sprintf_buf);
		}
	}

	sprintf(sprintf_buf,"/var/run/udhcpc-nas%d.pid", cur_vc);
	PidFile = fopen(sprintf_buf, "r");
	if(PidFile)
	{
		fclose(PidFile);
		sprintf(sprintf_buf,"kill -9 `cat /var/run/udhcpc-nas%d.pid`", cur_vc);
		asp_execute(sprintf_buf);
	}

	sprintf(sprintf_buf,"/sbin/ifconfig nas%d down", cur_vc);
	asp_execute(sprintf_buf);
	sprintf(sprintf_buf,"brctl delif br0 nas%d", cur_vc);
	asp_execute(sprintf_buf);

	sprintf(sprintf_buf,"/var/run/nas%d.pid", cur_vc);
	PidFile = fopen(sprintf_buf, "r");
	if(PidFile)
	{
		fclose(PidFile);
		sprintf(sprintf_buf,"kill -9 `cat /var/run/nas%d.pid`", cur_vc);
		asp_execute(sprintf_buf);
		sprintf(sprintf_buf,"rm /var/run/nas%d.pid", cur_vc);
		asp_execute(sprintf_buf);
	}
	//wan_stop.sh_End
#endif

#if 0
	sprintf(sprintf_buf,"/bin/iptables -t nat -D POSTROUTING -o nas%d -j MASQUERADE",cur_vc);
	asp_execute(sprintf_buf);

	sprintf(sprintf_buf,"/var/log/br2684ctl.%d.pid",cur_vc);
	ret = asp_readall(sprintf_buf);
	if(ret)
	{
		pid = atoi(ret);
		kill(pid,SIGTERM);
		free(ret);
	}
	
	ret = asp_readall("/var/log/dhclient.pid");
	if(ret)
	{
		pid = atoi(ret);
		kill(pid,SIGTERM);
		free(ret);
	}
	
	sprintf(sprintf_buf,"/var/run/ppp-nas%d.pid",cur_vc);
	ret = asp_readall(sprintf_buf);
	if(ret)
	{
		pid = atoi(ret);
		kill(pid,SIGTERM);
		free(ret);
	}

	sprintf(sprintf_buf,"/usr/bin/brctl addbr br0");
	asp_execute(sprintf_buf);

	sprintf(sprintf_buf,"/usr/bin/brctl delif br0 nas%d",cur_vc);
	asp_execute(sprintf_buf);

	sprintf(sprintf_buf,"/usr/bin/brctl show | sed -ne 's/.*\\(nas\\)[0-7]/\\1/p' > /tmp/brctl.tmp");
	asp_execute(sprintf_buf);
	
	ret = asp_readall("/tmp/brctl.tmp");
	if(ret)
	{
		if(strcmp(ret,"nas"))
		{
			asp_execute("/usr/bin/brctl delif br0 eth0");
			asp_execute("/sbin/ifconfig br0 down");
			asp_execute("/usr/bin/brctl delbr br0");
			asp_execute("sh /etc/lanconfig.sh");
		}
		free(ret);
	}

	if(!strcmp(web_api->wan[cur_vc]->status,val_def->wan_deactivated))
	{
		sprintf(sprintf_buf,"/etc/isp%d.conf",cur_vc);
		unlink(sprintf_buf);
		
		memset(web_api->wan[cur_vc],0,sizeof(INTERNET_SETTINGS));
		strncpy(web_api->wan[cur_vc]->status,val_def->wan_deactivated,16);
		return 0;
	}
#endif
#if 0
	if(!strcmp(web_api->wan[cur_vc]->isp,val_def->wan_dynamic_ip))
	{
		wan_execute_dynamic();
	}
	else if(!strcmp(web_api->wan[cur_vc]->isp,val_def->wan_static_ip))
	{
		wan_execute_static();
	}
	else if(!strcmp(web_api->wan[cur_vc]->isp,val_def->wan_pppoe))
	{
		fprintf(stderr, "PPPoE\n");
		wan_execute_ppp();
	}
	else if(!strcmp(web_api->wan[cur_vc]->isp,val_def->wan_bridge))
	{
		wan_execute_bridge();
	}
#endif	
#if 0	
	TcWebApiWan_write();
#endif

	return 0;
}

int TcWebApiWan_write(int pvc)
{
	int i = pvc;
	int fd;
	char buf[128];
	
	
		sprintf(buf,"/etc/isp%d.conf",i);
		fd = open(buf,O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
		if(fd ==-1)
			return -1;
		//write to file
		if(!strcmp(web_api->wan[i]->status,val_def->wan_activated))
			sprintf(buf,"ACTIVE=\"Yes\"\n");
		else if(!strcmp(web_api->wan[i]->status,val_def->wan_deactivated))
			sprintf(buf,"ACTIVE=\"No\"\n");
		write(fd,buf,strlen(buf));
		sprintf(buf,"VPI=\"%ld\"\n",web_api->wan[i]->vpi);
		write(fd,buf,strlen(buf));
		sprintf(buf,"VCI=\"%ld\"\n",web_api->wan[i]->vci);
		write(fd,buf,strlen(buf));
		if(strlen(web_api->wan[i]->qos) == 0)
		{
			sprintf(buf,"QOS=\"%s\"\n","ubr");
		}
		else
		{
		sprintf(buf,"QOS=\"%s\"\n",web_api->wan[i]->qos);
		}
		write(fd,buf,strlen(buf));
		sprintf(buf,"PCR=\"%ld\"\n",web_api->wan[i]->pcr);
		write(fd,buf,strlen(buf));
		sprintf(buf,"SCR=\"%ld\"\n",web_api->wan[i]->scr);
		write(fd,buf,strlen(buf));
		sprintf(buf,"MBS=\"%ld\"\n",web_api->wan[i]->mbs);
		write(fd,buf,strlen(buf));

		sprintf(buf,"ISP=\"%s\"\n",web_api->wan[i]->isp);
		write(fd,buf,strlen(buf));

		if(!strcmp(web_api->wan[i]->isp,val_def->wan_dynamic_ip))
		{
			sprintf(buf,"ENCAP=\"%s\"\n",web_api->wan[i]->encap);
			write(fd,buf,strlen(buf));
			sprintf(buf,"NATENABLE=\"%s\"\n",web_api->wan[i]->nat_enable);
			write(fd,buf,strlen(buf));
			sprintf(buf,"DEFAULTROUTE=\"%s\"\n",web_api->wan[i]->def_route);
			write(fd,buf,strlen(buf));
			sprintf(buf,"MTU=\"%ld\"\n",web_api->wan[i]->mtu);
			write(fd,buf,strlen(buf));
			sprintf(buf,"RIPVERSION=\"%s\"\n",web_api->wan[i]->rip_ver);
			write(fd,buf,strlen(buf));
			sprintf(buf,"DIRECTION=\"%s\"\n",web_api->wan[i]->direction);
			write(fd,buf,strlen(buf));
			sprintf(buf,"IGMP=\"%s\"\n",web_api->wan[i]->igmp);
			write(fd,buf,strlen(buf));
			
		}
		else if(!strcmp(web_api->wan[i]->isp,val_def->wan_static_ip))
		{
			sprintf(buf,"ENCAP=\"%s\"\n",web_api->wan[i]->encap);
			write(fd,buf,strlen(buf));
			sprintf(buf,"IPADDR=\"%s\"\n",web_api->wan[i]->ip_addr);
			write(fd,buf,strlen(buf));
			sprintf(buf,"NETMASK=\"%s\"\n",web_api->wan[i]->netmask);
			write(fd,buf,strlen(buf));
			sprintf(buf,"GATEWAY=\"%s\"\n",web_api->wan[i]->gateway);
			write(fd,buf,strlen(buf));
			sprintf(buf,"NATENABLE=\"%s\"\n",web_api->wan[i]->nat_enable);
			write(fd,buf,strlen(buf));
			sprintf(buf,"DEFAULTROUTE=\"%s\"\n",web_api->wan[i]->def_route);
			write(fd,buf,strlen(buf));
			sprintf(buf,"MTU=\"%ld\"\n",web_api->wan[i]->mtu);
			write(fd,buf,strlen(buf));
			sprintf(buf,"RIPVERSION=\"%s\"\n",web_api->wan[i]->rip_ver);
			write(fd,buf,strlen(buf));
			sprintf(buf,"DIRECTION=\"%s\"\n",web_api->wan[i]->direction);
			write(fd,buf,strlen(buf));
			sprintf(buf,"IGMP=\"%s\"\n",web_api->wan[i]->igmp);
			write(fd,buf,strlen(buf));
			
		}
		else if(!strcmp(web_api->wan[i]->isp,val_def->wan_pppoe))
		{
			sprintf(buf,"USERNAME=\"%s\"\n",web_api->wan[i]->usr_name);
			write(fd,buf,strlen(buf));
			sprintf(buf,"PASSWORD=\"%s\"\n",web_api->wan[i]->passwd);
			write(fd,buf,strlen(buf));
			sprintf(buf,"ENCAP=\"%s\"\n",web_api->wan[i]->encap);
			write(fd,buf,strlen(buf));
			sprintf(buf,"CONNECTION=\"%s\"\n",web_api->wan[i]->connection);
			write(fd,buf,strlen(buf));
			sprintf(buf,"CLOSEIFIDLE=\"%ld\"\n",web_api->wan[i]->closeifidle);
			write(fd,buf,strlen(buf));
			sprintf(buf,"LASTmss=\"%ld\"\n", web_api->wan[i]->last_mss);
			write(fd,buf,strlen(buf));
			sprintf(buf,"MSS=\"%ld\"\n",web_api->wan[i]->mss);
			write(fd,buf,strlen(buf));
			web_api->wan[i]->last_mss = web_api->wan[i]->mss;
			sprintf(buf,"PPPGETIP=\"%s\"\n",web_api->wan[i]->ppp_get_ip);
			write(fd,buf,strlen(buf));
			sprintf(buf,"IPADDR=\"%s\"\n",web_api->wan[i]->ip_addr);
			write(fd,buf,strlen(buf));
			sprintf(buf,"NETMASK=\"%s\"\n",web_api->wan[i]->netmask);
			write(fd,buf,strlen(buf));
			sprintf(buf,"GATEWAY=\"%s\"\n",web_api->wan[i]->gateway);
			write(fd,buf,strlen(buf));
			sprintf(buf,"NATENABLE=\"%s\"\n",web_api->wan[i]->nat_enable);
			write(fd,buf,strlen(buf));
			sprintf(buf,"DEFAULTROUTE=\"%s\"\n",web_api->wan[i]->def_route);
			write(fd,buf,strlen(buf));
			sprintf(buf,"RIPVERSION=\"%s\"\n",web_api->wan[i]->rip_ver);
			write(fd,buf,strlen(buf));
			sprintf(buf,"DIRECTION=\"%s\"\n",web_api->wan[i]->direction);
			write(fd,buf,strlen(buf));
			sprintf(buf,"IGMP=\"%s\"\n",web_api->wan[i]->igmp);
			write(fd,buf,strlen(buf));
		}
		else
		{
			sprintf(buf,"ENCAP=\"%s\"\n",web_api->wan[i]->encap);
			write(fd,buf,strlen(buf));
		}
		
		close(fd);
	
		return 0;
}

void wan_execute_dynamic()
{
	char pcr_v[64];
	char encap_t[16];
	
	if(!strcmp(web_api->wan[cur_vc]->qos,val_def->wan_ubr))
		sprintf(pcr_v,"-p %ld",web_api->wan[cur_vc]->pcr);
	else
		sprintf(pcr_v,"-p %ld -q %ld -m %ld",web_api->wan[cur_vc]->pcr,web_api->wan[cur_vc]->scr,web_api->wan[cur_vc]->mbs);

	if(!strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_bridgellc))
		sprintf(encap_t,"-e 0");
	else if(!strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_bridgevc))
		sprintf(encap_t,"-e 1");
	else if(!strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_routellc))
		sprintf(encap_t,"-e 2");
	else
		sprintf(encap_t,"-e 3");

	sprintf(sprintf_buf,"-c %d -a 0.%ld.%ld %s -t %s %s",cur_vc,web_api->wan[cur_vc]->vpi,web_api->wan[cur_vc]->vci,encap_t,web_api->wan[cur_vc]->qos,pcr_v);
	asp_shell2("br2684ctl",sprintf_buf);

	sprintf(sprintf_buf,"/sbin/ifconfig nas%d 10.11.12.13 up",cur_vc);
	asp_execute(sprintf_buf);
	
	sprintf(sprintf_buf,"/sbin/ifconfig nas%d down",cur_vc);
	asp_execute(sprintf_buf);

	sprintf(sprintf_buf,"-pf /var/log/dhclient.pid -lf /tmp/dhcp.leases nas%d",cur_vc);	
	asp_shell2("dhclient",sprintf_buf);
	
	if(!strcmp(web_api->wan[cur_vc]->nat_enable,val_def->wan_natenable))
	{
		sprintf(sprintf_buf,"/bin/iptables -t nat -A POSTROUTING -o nas%d -j MASQUERADE",cur_vc);
		asp_execute(sprintf_buf);
	}
	
	
}

void wan_execute_static()
{
	char pcr_v[64];
	char encap_t[16];
	char *arg;

	arg = (char*)asp_alloc(my_reent,512);
	
	if(!strcmp(web_api->wan[cur_vc]->qos,val_def->wan_ubr))
		sprintf(pcr_v,"-p %ld",web_api->wan[cur_vc]->pcr);
	else
		sprintf(pcr_v,"-p %ld -q %ld -m %ld",web_api->wan[cur_vc]->pcr,web_api->wan[cur_vc]->scr,web_api->wan[cur_vc]->mbs);

	if(!strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_bridgellc))
		sprintf(encap_t,"-e 0");
	else if(!strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_bridgevc))
		sprintf(encap_t,"-e 1");
	else if(!strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_routellc))
	{
		strncpy(web_api->wan[cur_vc]->netmask,"0.0.0.0",16);
		strncpy(web_api->wan[cur_vc]->gateway,"0.0.0.0",16);
		sprintf(encap_t,"-e 2");
	}
	else
		sprintf(encap_t,"-e 3");

	sprintf(arg,"/usr/bin/br2684ctl -c %d -a 0.%ld.%ld %s -t %s %s",cur_vc,web_api->wan[cur_vc]->vpi,web_api->wan[cur_vc]->vci,encap_t,web_api->wan[cur_vc]->qos,pcr_v);
	asp_shell(arg);

	sprintf(arg,"/sbin/ifconfig nas%d %s netmask %s up",cur_vc,web_api->wan[cur_vc]->ip_addr,web_api->wan[cur_vc]->netmask);
	asp_execute(arg);

	if(!strcmp(web_api->wan[cur_vc]->nat_enable,val_def->wan_natenable))
	{
		sprintf(arg,"/bin/iptables -t nat -A POSTROUTING -o nas%d -j MASQUERADE",cur_vc);
		asp_execute(arg);
	}
	
}

void wan_execute_ppp()
{
#if 0
	char pcr_v[64], encap_t[64];
	char *arg;
	int fd,len=0;
	char PPP_PARAM[200];
	
	arg = (char*)asp_alloc(my_reent,1024);
#endif	

	sprintf(sprintf_buf, "/var/script/wan_start.sh %d", cur_vc);
	asp_execute(sprintf_buf);
#if 0
	sprintf(PPP_PARAM, "unit %d user %s password %s nodetach holdoff 4 maxfail 25 usepeerdns lcp-echo-interval 2 lcp-echo-failure 7",
		   cur_vc, web_api->wan[cur_vc]->usr_name, web_api->wan[cur_vc]->passwd);
	//PPPoE
	if(!strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_pppoellc) || !strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_pppoevc))
	{
		if(!strcmp(web_api->wan[cur_vc]->qos,val_def->wan_ubr))
			sprintf(pcr_v,"-p %ld",web_api->wan[cur_vc]->pcr);
		else
			sprintf(pcr_v,"-p %ld -q %ld -m %ld",web_api->wan[cur_vc]->pcr,web_api->wan[cur_vc]->scr,web_api->wan[cur_vc]->mbs);
		if(!strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_pppoellc))
			sprintf(encap_t, "-e 0");
		else if(!strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_pppoevc))
			sprintf(encap_t, "-e 1");

		sprintf(arg,"/usr/bin/br2684ctl -c %d %s -t %s %s -a 0.%ld.%ld &",cur_vc, encap_t, web_api->wan[cur_vc]->qos, pcr_v, web_api->wan[cur_vc]->vpi, web_api->wan[cur_vc]->vci);
		asp_execute(arg);
		
	}
	//PPPoA
	else if(!strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_pppoallc) || !strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_pppoavc))
	{
		
	}
#endif	
/*		
	if(!strcmp(web_api->wan[cur_vc]->qos,val_def->wan_ubr))
		sprintf(pcr_v,"-p %ld",web_api->wan[cur_vc]->pcr);
	else
		sprintf(pcr_v,"-p %ld -q %ld -m %ld",web_api->wan[cur_vc]->pcr,web_api->wan[cur_vc]->scr,web_api->wan[cur_vc]->mbs);

	sprintf(arg,"/usr/bin/br2684ctl -c %d -a 0.%ld.%ld -t %s %s",cur_vc,web_api->wan[cur_vc]->vpi,web_api->wan[cur_vc]->vci,web_api->wan[cur_vc]->qos,pcr_v);
	
	asp_shell(arg);

	
	fd = open("/etc/ppp/options",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd ==-1)
		return;

	len +=sprintf(arg+len,"nodetach\n");
	if(!strcmp(web_api->wan[cur_vc]->def_route,val_def->wan_defroute_check))
		len +=sprintf(arg+len,"defaultroute\n");
	if(!strcmp(web_api->wan[cur_vc]->ppp_get_ip,val_def->wan_pppgetip_dynamic))
		len +=sprintf(arg+len,"noipdefault\n");
	else
		len +=sprintf(arg+len,"%s\\:\n",web_api->wan[cur_vc]->ip_addr);
	
	len += sprintf(arg+len,"noauth\n");
	len += sprintf(arg+len,"holdoff 4\n");
	len += sprintf(arg+len,"maxfail 25\n");

	if(!strcmp(web_api->wan[cur_vc]->connection,val_def->wan_connect_ondemand))
	{
		len += sprintf(arg+len,"demand\n");
		len +=sprintf(arg+len,"idle %ld\n",web_api->wan[cur_vc]->closeifidle);
	}
	else
		len += sprintf(arg+len,"persist\n");
	
	len +=sprintf(arg+len,"linkname nas%d\n",cur_vc);
	len += sprintf(arg+len,"asyncmap 0\n");
	len += sprintf(arg+len,"lcp-echo-interval  2\n");
	len += sprintf(arg+len,"lcp-echo-failure   7\n");
	len +=sprintf(arg+len,"name %s\n",web_api->wan[cur_vc]->usr_name);
	len +=sprintf(arg+len,"user %s\n",web_api->wan[cur_vc]->usr_name);

	if(!strcmp(web_api->wan[cur_vc]->nat_enable,val_def->wan_natenable))
	{
		len +=sprintf(arg+len,"connect '/bin/iptables -t nat -A POSTROUTING -o \\nas%d -j MASQUERADE'\n",cur_vc);
		len +=sprintf(arg+len,"disconnect '/bin/iptables -t nat -D POSTROUTING -o \\nas%d -j MASQUERADE'\n",cur_vc);
	}

	if(web_api->wan[cur_vc]->netmask[0] != 0)
		len +=sprintf(arg+len,"netmask %s\n",web_api->wan[cur_vc]->netmask);

	if(!strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_pppoellc) || !strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_pppoevc))
	{
		len +=sprintf(arg+len,"plugin pppoe\n");	
		len +=sprintf(arg+len,"nas%d\n",cur_vc);
	}
	else if(!strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_pppoallc) || !strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_pppoavc))
	{
		len +=sprintf(arg+len,"plugin pppoa\n");
		len +=sprintf(arg+len,"llc-encaps\n");
		len +=sprintf(arg+len,"%ld.%ld\n",web_api->wan[cur_vc]->vpi,web_api->wan[cur_vc]->vci);		
	}
	write(fd,arg,len);
	close(fd);

	
	sprintf(arg,"%s    *    %s",web_api->wan[cur_vc]->usr_name,web_api->wan[cur_vc]->passwd);
	fd = open("/etc/ppp/pap-secrets",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd ==-1)
		return;
	
	write(fd,arg,strlen(arg));
	close(fd);


	fd = open("/etc/ppp/chap-secrets",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd ==-1)
		return;
	write(fd,arg,strlen(arg));
	close(fd);
	
	asp_shell("/bin/pppd");
	
*/	
}

void wan_execute_bridge()
{
	char pcr_v[64];
	char encap_t[16];
	char arg[256];
	char *ret;
	
	if(!strcmp(web_api->wan[cur_vc]->qos,val_def->wan_ubr))
		sprintf(pcr_v,"-p %ld",web_api->wan[cur_vc]->pcr);
	else
		sprintf(pcr_v,"-p %ld -q %ld -m %ld",web_api->wan[cur_vc]->pcr,web_api->wan[cur_vc]->scr,web_api->wan[cur_vc]->mbs);

	if(!strcmp(web_api->wan[cur_vc]->encap,val_def->wan_encap_bridgellc))
		sprintf(encap_t,"-e 0");
	else
		sprintf(encap_t,"-e 1");	

	sprintf(arg,"/usr/bin/br2684ctl -c %d -a 0.%ld.%ld %s -t %s %s",cur_vc,web_api->wan[cur_vc]->vpi,web_api->wan[cur_vc]->vci,encap_t,web_api->wan[cur_vc]->qos,pcr_v);
	asp_shell(arg);

	sprintf(arg,"/usr/bin/brctl show | sed -ne 's/.*\\(eth[0-7]\\)/\\1/p' > /tmp/brctl.tmp");
	asp_execute(arg);

	ret = asp_readall("/tmp/brctl.tmp");
	if(!ret)
	{
		asp_execute("/usr/bin/brctl addif br0 eth0");
		asp_execute("/sbin/ifconfig eth0 0.0.0.0");
	}
	else
		free(ret);

	sprintf(arg,"/usr/bin/brctl addif br0 nas%d",cur_vc);
	asp_execute(arg);

	sprintf(arg,"/sbin/ifconfig nas%d 0.0.0.0",cur_vc);
	asp_execute(arg);

	asp_execute("/sbin/ifconfig br0 10.0.0.1 netmask 255.0.0.0 up");
	
}

int wan_pvc_connect(int vc)
{
	int err = -1,tp=-1;
	char *stream=NULL,*sub1,*sub2,*ifconfig=NULL;
	char arg[128];
	
	unlink("/tmp/br0.log");
	sprintf(arg, "/etc/isp%d.conf", vc);
	stream = asp_readall(arg);
	if(stream ==NULL)
		goto wan_pvc_connect_err;
	
	sub1=strstr(stream,"ISP=");
	if(sub1 == NULL)
		goto wan_pvc_connect_err;
	sub2 = strchr(sub1,'\n');
	if(sub2 == NULL)
		goto wan_pvc_connect_err;
	else
		(sub2-1)[0] = '\0';
	
	tp = atoi(sub1+5);
	if(tp ==0 ||tp ==1)
	{
		sprintf(arg,"/sbin/ifconfig nas%d > /tmp/br0.log",vc);
		asp_execute(arg);
	}
	else if(tp ==2)
	{
		sprintf(arg, "/sbin/ifconfig ppp%d > /tmp/br0.log", vc);
		asp_execute(arg);	
	}
	else if(tp==3)
		asp_execute("/sbin/ifconfig br0 > /tmp/br0.log");
	
	ifconfig = asp_readall("/tmp/br0.log");

	if(ifconfig ==NULL)
		goto wan_pvc_connect_err;

	sub1 = strstr(ifconfig,"inet addr:");
	
	if(sub1==NULL)
		goto wan_pvc_connect_err;

	if(!strcmp(web_api->wan[vc]->status,val_def->wan_deactivated))
		goto wan_pvc_connect_err;
/*
	asp_execute("wan adsl st > /tmp/line_status.log");
	line_status = asp_readall("/tmp/line_status.log");
	
	if(!line_status || !strstr(line_status, "up"))
		goto wan_pvc_connect_err;
	else if(line_status)
		fprintf(stderr, "%s", line_status);
*/
	err = 0;
wan_pvc_connect_err:

	if(stream)
		free(stream);
	if(ifconfig)
		free(ifconfig);
	return err;
	
}

void wan_pvclist()
{
	int i;
	char buf[1000];
	char active[8];
	char mux[8];
	char ip[8];
	char connect[16];
	char encap[16];
	char *encap_start;

	/*	get adsl state from /proc file	*/
	FILE *pFile;
	char string[100], *tok;
	pFile = fopen("/proc/tc3162/adsl_stats", "r");
	if(pFile)
	{
		while(fgets(string, 100, pFile))
		{
			if((tok = strstr(string,"ADSL link status:")))
				strncpy(info.line_state,string+strlen("ADSL link status:"),16);
		}
		fclose(pFile);
	}
		
	
	ASP_OUTPUT("<table width=\"590\" border=\"1\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\" bordercolor=\"#CCCCCC\" bgcolor=\"#FFFFFF\">");
	ASP_OUTPUT("<tr height=\"30\"><td width=\"55\"  align=\"center\" class=\"tabdata\"><strong>#</strong></div></td>");
	ASP_OUTPUT("<td width=\"70\"  align=\"center\" class=\"tabdata\"><strong>Active</strong></td>");
	ASP_OUTPUT("<td width=\"50\"  align=\"center\" class=\"tabdata\"><strong>VPI</strong></td>");
	ASP_OUTPUT("<td width=\"50\"  align=\"center\" class=\"tabdata\"><strong>VCI</strong></td>");
	ASP_OUTPUT("<td width=\"200\"  align=\"center\" class=\"tabdata\"><strong>ENCAP</strong></td>");
	ASP_OUTPUT("<td width=\"120\"  align=\"center\" class=\"tabdata\"><strong>Mux</strong></td>");
	ASP_OUTPUT("<td width=\"120\"  align=\"center\" class=\"tabdata\"><strong>IP Address</strong></td>");
	ASP_OUTPUT("<td width=\"70\"  align=\"center\" class=\"tabdata\"><strong>Status</strong></td></tr>");

	for(i = 0 ; i<8; i++)
	{
		if(!strcmp(web_api->wan[i]->status,val_def->wan_activated))
			strcpy(active,"Yes");
		else
			strcpy(active,"No");

		if(encap_start = strstr(web_api->wan[i]->encap,"LLC"))
		{
			strncpy(encap, web_api->wan[i]->encap, encap_start - web_api->wan[i]->encap - 1);
			encap[encap_start - web_api->wan[i]->encap - 1] = '\0';
			strcpy(mux,"LLC");
		}
		else if(encap_start = strstr(web_api->wan[i]->encap,"VC"))
		{
			strncpy(encap, web_api->wan[i]->encap, encap_start - web_api->wan[i]->encap - 1);
			encap[encap_start - web_api->wan[i]->encap - 1] = '\0';
			strcpy(mux,"VC-Mux");
		}
		else
		{
			strcpy(encap, "N/A");
			strcpy(mux,"N/A");
		}

		if(!strcmp(web_api->wan[i]->isp,val_def->wan_dynamic_ip))
			strcpy(ip,"Dynamic");
		else if(!strcmp(web_api->wan[i]->isp,val_def->wan_static_ip))
			strcpy(ip,"Static");
		else if(!strcmp(web_api->wan[i]->isp,val_def->wan_pppoe))
		{
			if(!strcmp(web_api->wan[i]->ppp_get_ip,val_def->wan_pppgetip_static))
				strcpy(ip,"Static");
			else if(!strcmp(web_api->wan[i]->ppp_get_ip,val_def->wan_pppgetip_dynamic))
				strcpy(ip,"Dynamic");
			else
				strcpy(ip,"N/A");
		}
		else
			strcpy(ip,"N/A");
		
		
		if(wan_pvc_connect(i) == 0)	//only if PVC activated
		{
			if(strstr(info.line_state, "up"))	//check if adsl link is connected
				strcpy(connect, "Up");
			else
				strcpy(connect, "Down");
		}
		/*	Once PVC device is deactivated, do not concern adsl link state.	*/
		else
			strcpy(connect, "N/A");


		sprintf(buf,"<tr><td align=\"center\" class=\"tabdata\">PVC%d</td><td align=\"center\" class=\"tabdata\">%s</td><td align=\"center\" class=\"tabdata\">%ld</td><td align=\"center\" class=\"tabdata\">%ld</td><td align=\"center\" class=\"tabdata\">%s</td><td align=\"center\" class=\"tabdata\">%s</td><td align=\"center\" class=\"tabdata\">%s</td><td align=\"center\" class=\"tabdata\">%s</td></tr>",i,active,web_api->wan[i]->vpi,web_api->wan[i]->vci,encap,mux,ip,connect);
		ASP_OUTPUT(buf);
	}
		
	ASP_OUTPUT("</table>");
}

void pharseigdconf(char * igdconfbuff,char * target,char *value)
{
	char * temp;

	temp = value;

	if((igdconfbuff = strstr(igdconfbuff,target)) != NULL)
	{
		
		igdconfbuff +=strlen(target);

		while((*igdconfbuff!=' ')&&(*igdconfbuff!='\r')&&(*igdconfbuff!='\n'))
		{
			*temp = *igdconfbuff;
			temp++;
			igdconfbuff++;
		};
		*temp = '\0';
		
	}
	else
		value[0]='\0';
		
}

void restart_upnp(char *wanif)
{
		int fd;
		char tmp[128]={0};
		int pid;
		char *ret;
		
		ret = asp_readall("/var/log/upnpd.pid");
		if(ret)
		{
			pid = atoi(ret);
			kill(pid,SIGTERM);
			free(ret);
		}
		else
		{
			fprintf(stderr,"Not found /var/log/upnpd.pid file");
		}
				
		fd = open("/etc/igd/igd.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
		if(fd !=-1)
   	{
   		sprintf(tmp,"autoconf=1\n");
   		write(fd,tmp,strlen(tmp));
   		sprintf(tmp,"igd=enable\n");
   		write(fd,tmp,strlen(tmp));
   		sprintf(tmp,"lanif=br0\n");
   		write(fd,tmp,strlen(tmp));
   		if(strlen(wanif) != 0)
  			sprintf(tmp,"wanif=%s\n",wanif);
  		else
  			sprintf(tmp,"wanif=nas0\n");
  		write(fd,tmp,strlen(tmp));
  		sprintf(tmp,"nat=1\n");
   		write(fd,tmp,strlen(tmp));
   		close(fd);
   	}
   system("/userfs/bin/upnpd &");
}
