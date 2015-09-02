
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

char *TcWebApiAdvQos_get(int id)
{	
	int i = 1;
	char buf[64];
	switch (id)
	{
		case GB_C_QOS_ACTIVATED:			
			if (!strcmp(web_api->adv_qos->qos_status,val_def->qos_activated))
					ASP_OUTPUT("CHECKED");
			break;			
		case GB_C_QOS_DEACTIVATED:		
			if (!strcmp(web_api->adv_qos->qos_status,val_def->qos_deactivated))
					ASP_OUTPUT("CHECKED");
			break;				
		case GB_C_QOS_8021PACTIVATED:	
			if (!strcmp(web_api->adv_qos->qos_8021_status,val_def->qos_8021_activated))
					ASP_OUTPUT("CHECKED");
			break;			
		case GB_C_QOS_8021PDEACTIVATED:	
			if (!strcmp(web_api->adv_qos->qos_8021_status,val_def->qos_8021_deactivated))
					ASP_OUTPUT("CHECKED");
			break;	

		case GB_C_QOS_IPACTIVATED:
			if (!strcmp(web_api->adv_qos->qos_ip_status,val_def->qos_ip_activated))
			{
					ASP_OUTPUT("CHECKED");
			}
			break;			
		case GB_C_QOS_IPDEACTIVATED:	
			if (!strcmp(web_api->adv_qos->qos_ip_status,val_def->qos_ip_deactivated))
			{
					ASP_OUTPUT("CHECKED");
			}
			break;	
					
		case GB_C_QOS_IPTOS:
			if (!strcmp(web_api->adv_qos->qos_iptype,val_def->qos_iptype_tos))
					ASP_OUTPUT("CHECKED");	
			break;						
		case GB_C_QOS_IPDIFFSERV:
			if (!strcmp(web_api->adv_qos->qos_iptype,val_def->qos_iptype_diffserv))
					ASP_OUTPUT("CHECKED");	
			break;		
						
		case GB_C_QOS_APACTIVATED:
			if (!strcmp(web_api->adv_qos->qos_app_status,val_def->qos_app_activated))
					ASP_OUTPUT("CHECKED");
			break;	
		case GB_C_QOS_APDEACTIVATED:
			if (!strcmp(web_api->adv_qos->qos_app_status,val_def->qos_app_deactivated))
					ASP_OUTPUT("CHECKED");
			break;				
	
		case GB_C_QOS_VLANACTIVATED:
			if (!strcmp(web_api->adv_qos->qos_vlan_status,val_def->qos_vlan_activated))
					ASP_OUTPUT("CHECKED");
			break;	
		case GB_C_QOS_VLANDEACTIVATED:			
			if (!strcmp(web_api->adv_qos->qos_vlan_status,val_def->qos_vlan_deactivated))
			{
					ASP_OUTPUT("CHECKED");
			}
			break;		
	
		case GB_C_QOS_APVOICEQUEUE:
			for (i=1;i<5;i++)
			{
				if(atoi(web_api->adv_qos->qos_voicequeueindex) == i)			
					sprintf(buf,"<OPTION SELECTED >%d",i);
				else
					sprintf(buf,"<OPTION >%d",i);
				ASP_OUTPUT(buf);	
			}			
			break;
		case GB_C_QOS_APVOICERIPFROM:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_voicertprange1);
			ASP_OUTPUT(buf);					
			break;
		case GB_C_QOS_APVOICERIPTO:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_voicertprange2);
			ASP_OUTPUT(buf);		
			break;			
			
		case GB_C_QOS_APVIDEOQUEUE:
			for (i=1;i<5;i++)
			{
				if(atoi(web_api->adv_qos->qos_videoqueueindex) == i)
					sprintf(buf,"<OPTION SELECTED>%d",i);
				else
					sprintf(buf,"<OPTION >%d",i);
				ASP_OUTPUT(buf);	
			}	
			break;
		case GB_C_QOS_APVIDEORIPFROM:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_videortprange1);
			ASP_OUTPUT(buf);		
			break;
		case GB_C_QOS_APVIDEORIPTO:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_videortprange2);
			ASP_OUTPUT(buf);	
			break;			
		case GB_C_QOS_APIGMPQUEUE:
			for (i=1;i<5;i++)
			{
				if(atoi(web_api->adv_qos->qos_igmpqueueindex) == i)
					sprintf(buf,"<OPTION SELECTED>%d",i);
				else
					sprintf(buf,"<OPTION >%d",i);
				ASP_OUTPUT(buf);	
			}	
			break;
		case GB_C_QOS_APDATAQUEUE:
			for (i=1;i<5;i++)
			{
				if(atoi(web_api->adv_qos->qos_dataqueueindex) == i)				
					sprintf(buf,"<OPTION SELECTED>%d",i);
				else
					sprintf(buf,"<OPTION >%d",i);
				ASP_OUTPUT(buf);	
			}	
			break;
			
		case GB_C_QOS_8021PVALUE0:		
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_p8021_value0);
			ASP_OUTPUT(buf);			
			break;
		case GB_C_QOS_8021PVALUE1:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_p8021_value1);
			ASP_OUTPUT(buf);
			break;
		case GB_C_QOS_8021PVALUE2:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_p8021_value2);
			ASP_OUTPUT(buf);
			break;
		case GB_C_QOS_8021PVALUE3:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_p8021_value3);
			ASP_OUTPUT(buf);
			break;
		case GB_C_QOS_8021PVALUE4:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_p8021_value4);
			ASP_OUTPUT(buf);
			break;
		case GB_C_QOS_8021PVALUE5:		
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_p8021_value5);
			ASP_OUTPUT(buf);
			break;
		case GB_C_QOS_8021PVALUE6:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_p8021_value6);
			ASP_OUTPUT(buf);
			break;
		case GB_C_QOS_8021PVALUE7:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_p8021_value7);
			ASP_OUTPUT(buf);
			break;
			
		case GB_C_QOS_IPVALUE0:			
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_ipqos_value0);
			ASP_OUTPUT(buf);					
			break;
		case GB_C_QOS_IPVALUE1:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_ipqos_value1);
			ASP_OUTPUT(buf);	
			break;
		case GB_C_QOS_IPVALUE2:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_ipqos_value2);
			ASP_OUTPUT(buf);	
			break;
		case GB_C_QOS_IPVALUE3:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_ipqos_value3);
			ASP_OUTPUT(buf);	
			break;
		case GB_C_QOS_IPVALUE4:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_ipqos_value4);
			ASP_OUTPUT(buf);	
			break;
		case GB_C_QOS_IPVALUE5:		
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_ipqos_value5);
			ASP_OUTPUT(buf);	
			break;
		case GB_C_QOS_IPVALUE6:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_ipqos_value6);
			ASP_OUTPUT(buf);	
			break;
		case GB_C_QOS_IPVALUE7:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_ipqos_value7);
			ASP_OUTPUT(buf);	
			break;
									
		case GB_C_QOS_VID0:		
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_vlanqos_value0);
			ASP_OUTPUT(buf);		
			break;
		case GB_C_QOS_VID1:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_vlanqos_value1);
			ASP_OUTPUT(buf);			
			break;
		case GB_C_QOS_VID2:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_vlanqos_value2);
			ASP_OUTPUT(buf);			
			break;
		case GB_C_QOS_VID3:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_vlanqos_value3);
			ASP_OUTPUT(buf);				
			break;
		case GB_C_QOS_VID4:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_vlanqos_value4);
			ASP_OUTPUT(buf);			
			break;
		case GB_C_QOS_VID5:		
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_vlanqos_value5);
			ASP_OUTPUT(buf);			
			break;
		case GB_C_QOS_VID6:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_vlanqos_value6);
			ASP_OUTPUT(buf);				
			break;
		case GB_C_QOS_VID7:
			sprintf(buf,"\"%s\"",web_api->adv_qos->qos_vlanqos_value7);
			ASP_OUTPUT(buf);				
			break;			
	}						
	return ASP_OK;
}

int TcWebApiAdvQos_set(int id,char *value)
{
	
	char *ret = NULL;
	
	ret = ASP_FORM(value);
	if (ret ==NULL)
	{
		return -1;
	}

	switch (id)
	{
		case GB_C_QOS_STATUS:		
			strncpy(web_api->adv_qos->qos_status,ret,16);				
			break;	
		case GB_C_QOS_8021PSTATUS:
			strncpy(web_api->adv_qos->qos_8021_status,ret,16);
			break;	
		case GB_C_QOS_IPSTATUS:	
			strncpy(web_api->adv_qos->qos_ip_status,ret,16);			
			break;
		case GB_C_QOS_IPTYPE:
			strncpy(web_api->adv_qos->qos_iptype,ret,16);
			break;			
		case GB_C_QOS_APSTATUS:
			strncpy(web_api->adv_qos->qos_app_status,ret,16);	
			break;	
		case GB_C_QOS_VLANSTATUS:
			strncpy(web_api->adv_qos->qos_vlan_status,ret,16);	
			break;			

		case GB_C_QOS_APVOICEQUEUE:
			strncpy(web_api->adv_qos->qos_voicequeueindex,ret,8);		
			break;
		case GB_C_QOS_APVOICERIPFROM:
			strncpy(web_api->adv_qos->qos_voicertprange1,ret,8);
			break;
		case GB_C_QOS_APVOICERIPTO:
			strncpy(web_api->adv_qos->qos_voicertprange2,ret,8);
			break;
		case GB_C_QOS_APVIDEOQUEUE:
			strncpy(web_api->adv_qos->qos_videoqueueindex,ret,8);
			break;
		case GB_C_QOS_APVIDEORIPFROM:
			strncpy(web_api->adv_qos->qos_videortprange1,ret,8);
			break;
		case GB_C_QOS_APVIDEORIPTO:		
			strncpy(web_api->adv_qos->qos_videortprange2,ret,8);
			break;
		case GB_C_QOS_APIGMPQUEUE:
			strncpy(web_api->adv_qos->qos_igmpqueueindex,ret,8);
			break;
		case GB_C_QOS_APDATAQUEUE:
			strncpy(web_api->adv_qos->qos_dataqueueindex,ret,8);
			break;
			
		case GB_C_QOS_8021PVALUE0:		
			strncpy(web_api->adv_qos->qos_p8021_value0,ret,8);
			break;
		case GB_C_QOS_8021PVALUE1:
			strncpy(web_api->adv_qos->qos_p8021_value1,ret,8);
			break;
		case GB_C_QOS_8021PVALUE2:
			strncpy(web_api->adv_qos->qos_p8021_value2,ret,8);
			break;
		case GB_C_QOS_8021PVALUE3:
			strncpy(web_api->adv_qos->qos_p8021_value3,ret,8);
			break;
		case GB_C_QOS_8021PVALUE4:
			strncpy(web_api->adv_qos->qos_p8021_value4,ret,8);
			break;
		case GB_C_QOS_8021PVALUE5:		
			strncpy(web_api->adv_qos->qos_p8021_value5,ret,8);
			break;
		case GB_C_QOS_8021PVALUE6:
			strncpy(web_api->adv_qos->qos_p8021_value6,ret,8);
			break;
		case GB_C_QOS_8021PVALUE7:
			strncpy(web_api->adv_qos->qos_p8021_value7,ret,8);
			break;
			
		case GB_C_QOS_IPVALUE0:		
			strncpy(web_api->adv_qos->qos_ipqos_value0,ret,8);
			break;
		case GB_C_QOS_IPVALUE1:
			strncpy(web_api->adv_qos->qos_ipqos_value1,ret,8);
			break;
		case GB_C_QOS_IPVALUE2:
			strncpy(web_api->adv_qos->qos_ipqos_value2,ret,8);
			break;
		case GB_C_QOS_IPVALUE3:
			strncpy(web_api->adv_qos->qos_ipqos_value3,ret,8);
			break;
		case GB_C_QOS_IPVALUE4:
			strncpy(web_api->adv_qos->qos_ipqos_value4,ret,8);
			break;
		case GB_C_QOS_IPVALUE5:		
			strncpy(web_api->adv_qos->qos_ipqos_value5,ret,8);
			break;
		case GB_C_QOS_IPVALUE6:
			strncpy(web_api->adv_qos->qos_ipqos_value6,ret,8);
			break;
		case GB_C_QOS_IPVALUE7:
			strncpy(web_api->adv_qos->qos_ipqos_value7,ret,8);
			break;
									
		case GB_C_QOS_VID0:		
			strncpy(web_api->adv_qos->qos_vlanqos_value0,ret,8);
			break;
		case GB_C_QOS_VID1:
			strncpy(web_api->adv_qos->qos_vlanqos_value1,ret,8);
			break;
		case GB_C_QOS_VID2:
			strncpy(web_api->adv_qos->qos_vlanqos_value2,ret,8);
			break;
		case GB_C_QOS_VID3:
			strncpy(web_api->adv_qos->qos_vlanqos_value3,ret,8);
			break;
		case GB_C_QOS_VID4:
			strncpy(web_api->adv_qos->qos_vlanqos_value4,ret,8);
			break;
		case GB_C_QOS_VID5:		
			strncpy(web_api->adv_qos->qos_vlanqos_value5,ret,8);
			break;
		case GB_C_QOS_VID6:
			strncpy(web_api->adv_qos->qos_vlanqos_value6,ret,8);
			break;
		case GB_C_QOS_VID7:
			strncpy(web_api->adv_qos->qos_vlanqos_value7,ret,8);
			break;						
			
	}
	
	return 0;
}


int TcWebApiAdvQos_execute(int id)
{
	int fd;
	char buf[64];	
	if (!strcmp(web_api->adv_qos->qos_status,val_def->qos_activated))
	{	
		fd = open("/etc/qos.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
		if(fd ==-1)
			return -1;
		//write to file
		
		if (!strcmp(web_api->adv_qos->qos_8021_status,val_def->qos_8021_activated))
		{
			sprintf(buf,"QOS8021P_ACTIVE=%s\n",web_api->adv_qos->qos_8021_status);
			write(fd,buf,strlen(buf));
			sprintf(buf,"P8021_VALUE0=%s\n",web_api->adv_qos->qos_p8021_value0);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"P8021_VALUE1=%s\n",web_api->adv_qos->qos_p8021_value1);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"P8021_VALUE2=%s\n",web_api->adv_qos->qos_p8021_value2);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"P8021_VALUE3=%s\n",web_api->adv_qos->qos_p8021_value3);
			write(fd,buf,strlen(buf));
			sprintf(buf,"P8021_VALUE4=%s\n",web_api->adv_qos->qos_p8021_value4);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"P8021_VALUE5=%s\n",web_api->adv_qos->qos_p8021_value5);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"P8021_VALUE6=%s\n",web_api->adv_qos->qos_p8021_value6);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"P8021_VALUE7=%s\n",web_api->adv_qos->qos_p8021_value7);
			write(fd,buf,strlen(buf));	
		}
		
		if (!strcmp(web_api->adv_qos->qos_ip_status,val_def->qos_ip_activated))
		{	
			sprintf(buf,"IPQOS_ACTIVE=%s\n",web_api->adv_qos->qos_ip_status);
			write(fd,buf,strlen(buf));
			sprintf(buf,"IPQOS_TYPE=%s\n",web_api->adv_qos->qos_iptype);
			write(fd,buf,strlen(buf));
			sprintf(buf,"IPQOS_VALUE0=%s\n",web_api->adv_qos->qos_ipqos_value0);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"IPQOS_VALUE1=%s\n",web_api->adv_qos->qos_ipqos_value1);
			write(fd,buf,strlen(buf));						
			sprintf(buf,"IPQOS_VALUE2=%s\n",web_api->adv_qos->qos_ipqos_value2);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"IPQOS_VALUE3=%s\n",web_api->adv_qos->qos_ipqos_value3);
			write(fd,buf,strlen(buf));						
			sprintf(buf,"IPQOS_VALUE4=%s\n",web_api->adv_qos->qos_ipqos_value4);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"IPQOS_VALUE5=%s\n",web_api->adv_qos->qos_ipqos_value5);
			write(fd,buf,strlen(buf));						
			sprintf(buf,"IPQOS_VALUE6=%s\n",web_api->adv_qos->qos_ipqos_value6);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"IPQOS_VALUE7=%s\n",web_api->adv_qos->qos_ipqos_value7);
			write(fd,buf,strlen(buf));		
		}
		if (!strcmp(web_api->adv_qos->qos_app_status,val_def->qos_app_activated))
		{
			sprintf(buf,"APPQOS_ACTIVE=%s\n",web_api->adv_qos->qos_app_status);
			write(fd,buf,strlen(buf));		
			sprintf(buf,"VOICEQUEUEINDEX=%s\n",web_api->adv_qos->qos_voicequeueindex);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VOICERTPRANGE1=%s\n",web_api->adv_qos->qos_voicertprange1);
			write(fd,buf,strlen(buf));						
			sprintf(buf,"VOICERTPRANGE2=%s\n",web_api->adv_qos->qos_voicertprange2);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VIDEOQUEUEINDEX=%s\n",web_api->adv_qos->qos_videoqueueindex);
			write(fd,buf,strlen(buf));						
			sprintf(buf,"VIDEORTPRANGE1=%s\n",web_api->adv_qos->qos_videortprange1);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VIDEORTPRANGE2=%s\n",web_api->adv_qos->qos_videortprange2);
			write(fd,buf,strlen(buf));						
			sprintf(buf,"IGMPQUEUEINDEX=%s\n",web_api->adv_qos->qos_igmpqueueindex);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"DATAQUEUEINDEX=%s\n",web_api->adv_qos->qos_dataqueueindex);
			write(fd,buf,strlen(buf));	
		}	
		
		if (!strcmp(web_api->adv_qos->qos_vlan_status,val_def->qos_vlan_activated))
		{			
			sprintf(buf,"VLANQOS_ACTIVE=%s\n",web_api->adv_qos->qos_vlan_status);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VLANQOS_VALUE0=%s\n",web_api->adv_qos->qos_vlanqos_value0);
			write(fd,buf,strlen(buf));		
			sprintf(buf,"VLANQOS_VALUE1=%s\n",web_api->adv_qos->qos_vlanqos_value1);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VLANQOS_VALUE2=%s\n",web_api->adv_qos->qos_vlanqos_value2);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VLANQOS_VALUE3=%s\n",web_api->adv_qos->qos_vlanqos_value3);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VLANQOS_VALUE4=%s\n",web_api->adv_qos->qos_vlanqos_value4);
			write(fd,buf,strlen(buf));		
			sprintf(buf,"VLANQOS_VALUE5=%s\n",web_api->adv_qos->qos_vlanqos_value5);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VLANQOS_VALUE6=%s\n",web_api->adv_qos->qos_vlanqos_value6);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VLANQOS_VALUE7=%s\n",web_api->adv_qos->qos_vlanqos_value7);
			write(fd,buf,strlen(buf));	
		}						
		close(fd);
	}
	else
	{
		remove("/etc/qos.conf");		
		memset(web_api->adv_qos,0,sizeof(T_ADV_QOS));
		strncpy(web_api->adv_qos->qos_status,val_def->qos_deactivated,16); 	
		strncpy(web_api->adv_qos->qos_8021_status,val_def->qos_8021_deactivated,16); 	
		strncpy(web_api->adv_qos->qos_ip_status,val_def->qos_ip_deactivated,16); 	
		strncpy(web_api->adv_qos->qos_iptype,val_def->qos_iptype_tos,16); 		
		strncpy(web_api->adv_qos->qos_app_status,val_def->qos_app_deactivated,16); 
		strncpy(web_api->adv_qos->qos_vlan_status,val_def->qos_vlan_deactivated,16);			
	}
	return 0;
}

int TcWebApiAdvQos_write()
{
	int fd;
	char buf[64];	
	if (!strcmp(web_api->adv_qos->qos_status,val_def->qos_activated))
	{	
		fd = open("/etc/qos.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
		if(fd ==-1)
			return -1;
		if (!strcmp(web_api->adv_qos->qos_8021_status,val_def->qos_8021_activated))
		{
			sprintf(buf,"QOS8021P_ACTIVE=%s\n",web_api->adv_qos->qos_8021_status);
			write(fd,buf,strlen(buf));
			sprintf(buf,"P8021_VALUE0=%s\n",web_api->adv_qos->qos_p8021_value0);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"P8021_VALUE1=%s\n",web_api->adv_qos->qos_p8021_value1);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"P8021_VALUE2=%s\n",web_api->adv_qos->qos_p8021_value2);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"P8021_VALUE3=%s\n",web_api->adv_qos->qos_p8021_value3);
			write(fd,buf,strlen(buf));
			sprintf(buf,"P8021_VALUE4=%s\n",web_api->adv_qos->qos_p8021_value4);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"P8021_VALUE5=%s\n",web_api->adv_qos->qos_p8021_value5);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"P8021_VALUE6=%s\n",web_api->adv_qos->qos_p8021_value6);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"P8021_VALUE7=%s\n",web_api->adv_qos->qos_p8021_value7);
			write(fd,buf,strlen(buf));	
		}
		
		if (!strcmp(web_api->adv_qos->qos_ip_status,val_def->qos_ip_activated))
		{	
			sprintf(buf,"IPQOS_ACTIVE=%s\n",web_api->adv_qos->qos_ip_status);
			write(fd,buf,strlen(buf));
			sprintf(buf,"IPQOS_TYPE=%s\n",web_api->adv_qos->qos_iptype);
			write(fd,buf,strlen(buf));
			sprintf(buf,"IPQOS_VALUE0=%s\n",web_api->adv_qos->qos_ipqos_value0);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"IPQOS_VALUE1=%s\n",web_api->adv_qos->qos_ipqos_value1);
			write(fd,buf,strlen(buf));						
			sprintf(buf,"IPQOS_VALUE2=%s\n",web_api->adv_qos->qos_ipqos_value2);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"IPQOS_VALUE3=%s\n",web_api->adv_qos->qos_ipqos_value3);
			write(fd,buf,strlen(buf));						
			sprintf(buf,"IPQOS_VALUE4=%s\n",web_api->adv_qos->qos_ipqos_value4);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"IPQOS_VALUE5=%s\n",web_api->adv_qos->qos_ipqos_value5);
			write(fd,buf,strlen(buf));						
			sprintf(buf,"IPQOS_VALUE6=%s\n",web_api->adv_qos->qos_ipqos_value6);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"IPQOS_VALUE7=%s\n",web_api->adv_qos->qos_ipqos_value7);
			write(fd,buf,strlen(buf));		
		}
		if (!strcmp(web_api->adv_qos->qos_app_status,val_def->qos_app_activated))
		{
			sprintf(buf,"APPQOS_ACTIVE=%s\n",web_api->adv_qos->qos_app_status);
			write(fd,buf,strlen(buf));		
			sprintf(buf,"VOICEQUEUEINDEX=%s\n",web_api->adv_qos->qos_voicequeueindex);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VOICERTPRANGE1=%s\n",web_api->adv_qos->qos_voicertprange1);
			write(fd,buf,strlen(buf));						
			sprintf(buf,"VOICERTPRANGE2=%s\n",web_api->adv_qos->qos_voicertprange2);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VIDEOQUEUEINDEX=%s\n",web_api->adv_qos->qos_videoqueueindex);
			write(fd,buf,strlen(buf));						
			sprintf(buf,"VIDEORTPRANGE1=%s\n",web_api->adv_qos->qos_videortprange1);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VIDEORTPRANGE2=%s\n",web_api->adv_qos->qos_videortprange2);
			write(fd,buf,strlen(buf));						
			sprintf(buf,"IGMPQUEUEINDEX=%s\n",web_api->adv_qos->qos_igmpqueueindex);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"DATAQUEUEINDEX=%s\n",web_api->adv_qos->qos_dataqueueindex);
			write(fd,buf,strlen(buf));	
		}	
		
		if (!strcmp(web_api->adv_qos->qos_vlan_status,val_def->qos_vlan_activated))
		{			
			sprintf(buf,"VLANQOS_ACTIVE=%s\n",web_api->adv_qos->qos_vlan_status);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VLANQOS_VALUE0=%s\n",web_api->adv_qos->qos_vlanqos_value0);
			write(fd,buf,strlen(buf));		
			sprintf(buf,"VLANQOS_VALUE1=%s\n",web_api->adv_qos->qos_vlanqos_value1);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VLANQOS_VALUE2=%s\n",web_api->adv_qos->qos_vlanqos_value2);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VLANQOS_VALUE3=%s\n",web_api->adv_qos->qos_vlanqos_value3);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VLANQOS_VALUE4=%s\n",web_api->adv_qos->qos_vlanqos_value4);
			write(fd,buf,strlen(buf));		
			sprintf(buf,"VLANQOS_VALUE5=%s\n",web_api->adv_qos->qos_vlanqos_value5);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VLANQOS_VALUE6=%s\n",web_api->adv_qos->qos_vlanqos_value6);
			write(fd,buf,strlen(buf));	
			sprintf(buf,"VLANQOS_VALUE7=%s\n",web_api->adv_qos->qos_vlanqos_value7);
			write(fd,buf,strlen(buf));	
		}						
		close(fd);
	}	
	return 0;
}

