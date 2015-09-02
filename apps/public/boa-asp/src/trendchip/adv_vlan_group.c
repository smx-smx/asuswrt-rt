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

static int vlan_index=1;

unsigned char bu_atmt[8]={0,0,0,0,0,0,0,0};
unsigned char bu_atmp[8]={0,0,0,0,0,0,0,0};
unsigned char bu_ethernett[4]={0,0,0,0};
unsigned char bu_ethernetp[4]={0,0,0,0};
unsigned char bu_usbt;
unsigned char bu_usbp;
char bu_active[8]={0,0,0,0,0,0,0,0};
char bu_vlan_id[8]={0,0,0,0,0,0,0,0};

static void group_add()
{
	char *tmp;
	int i ;
	
	if(strcmp(web_api->adv_vlan_group[vlan_index-1]->active,val_def->vlan_group_active_yes)==0)
	{
		tmp=(char *)ASP_MALLOC(128);
		for(i=0;i<4;i++)
		{
			if(web_api->adv_vlan_group[vlan_index-1]->ethernetp[i]==1)
			{
				sprintf(tmp,"/bin/vconfig add eth%d %s",i+1,web_api->adv_vlan_group[vlan_index-1]->vlan_id);
				asp_execute(tmp);
				sprintf(tmp,"/bin/vconfig set_flag eth%d.%s 1 0",i+1,web_api->adv_vlan_group[vlan_index-1]->vlan_id);
				asp_execute(tmp);
			}
			if(web_api->adv_vlan_group[vlan_index-1]->ethernett[i]==1)
			{
				sprintf(tmp,"/bin/vconfig set_flag eth%d.%s 1 1",i+1,web_api->adv_vlan_group[vlan_index-1]->vlan_id);
				asp_execute(tmp);
			}
		}
		for(i=0;i<8;i++)
		{
			if(web_api->adv_vlan_group[vlan_index-1]->atmp[i]==1)
			{
				sprintf(tmp,"/bin/vconfig add nas%d %s",i,web_api->adv_vlan_group[vlan_index-1]->vlan_id);
				asp_execute(tmp);
				sprintf(tmp,"/bin/vconfig set_flag nas%d.%s 1 0",i,web_api->adv_vlan_group[vlan_index-1]->vlan_id);
				asp_execute(tmp);				
			}
			if(web_api->adv_vlan_group[vlan_index-1]->atmt[i]==1)
			{
				sprintf(tmp,"/bin/vconfig set_flag nas%d.%s 1 1",i,web_api->adv_vlan_group[vlan_index-1]->vlan_id);
				asp_execute(tmp);				
			}
		}
		if(web_api->adv_vlan_group[vlan_index-1]->usbp==1)
		{
				sprintf(tmp,"/bin/vconfig add usb0 %s",web_api->adv_vlan_group[vlan_index-1]->vlan_id);
				asp_execute(tmp);
				sprintf(tmp,"/bin/vconfig set_flag usb0.%s 1 0",web_api->adv_vlan_group[vlan_index-1]->vlan_id);
				asp_execute(tmp);				
		}
		if(web_api->adv_vlan_group[vlan_index-1]->usbt==1)
		{
				sprintf(tmp,"/bin/vconfig set_flag usb0.%s 1 1",web_api->adv_vlan_group[vlan_index-1]->vlan_id);
				asp_execute(tmp);				
		}
		ASP_FREE(tmp);
	}
}

static void group_del()
{
	char *tmp;
	int i ;	
	if(strcmp(bu_active,val_def->vlan_group_active_yes)==0)
	{
		tmp=(char *)ASP_MALLOC(128);
		for(i=0;i<4;i++)
		{
			if(bu_ethernetp[i]==1)
			{
				sprintf(tmp,"/bin/vconfig rem eth0.%s",bu_vlan_id);
				asp_execute(tmp);
			}
		}				
		for(i=0;i<8;i++)
		{
			if(bu_atmp[i]==1)
			{
				sprintf(tmp,"/bin/vconfig rem nas%d.%s",i,bu_vlan_id);
				asp_execute(tmp);
			}
		}		
		
		if(bu_usbp==1)
		{
				sprintf(tmp,"/bin/vconfig rem usb0.%s",bu_vlan_id);
				asp_execute(tmp);				
		}		
		ASP_FREE(tmp);
	}
}

static void group_write()
{
	int fd ,i,j;
	char *tmp;
	
	tmp=(char *)ASP_MALLOC(128);
	remove("/etc/vlan_group.conf");
	fd = open ("/etc/vlan_group.conf",O_WRONLY|O_CREAT|O_APPEND,S_IREAD|S_IWRITE);
	for(i=0;i<8;i++)
	{
		sprintf(tmp,"GROUP%d=\"",i+1);
		write(fd,tmp,strlen(tmp));
		if(strcmp(web_api->adv_vlan_group[i]->active,val_def->vlan_group_active_yes)==0)
			sprintf(tmp," Yes");
		else
			sprintf(tmp," No");	
		write(fd,tmp,strlen(tmp));
		if(strlen(web_api->adv_vlan_group[i]->vlan_id)>0)
			sprintf(tmp," ID%s",web_api->adv_vlan_group[i]->vlan_id);
		else
			sprintf(tmp," ID0");
		write(fd,tmp,strlen(tmp));
		for(j=0;j<8;j++)
		{
			if(web_api->adv_vlan_group[i]->atmp[j]==1)
			{
				sprintf(tmp," Pp%d",j);
				write(fd,tmp,strlen(tmp));
			}
			if(web_api->adv_vlan_group[i]->atmt[j]==1)
			{
				sprintf(tmp," Tp%d",j);
				write(fd,tmp,strlen(tmp));
			}			
		}
		for(j=0;j<4;j++)
		{
			if(web_api->adv_vlan_group[i]->ethernetp[j]==1)
			{
				sprintf(tmp," Pe%d",j+1);
				write(fd,tmp,strlen(tmp));
			}
			if(web_api->adv_vlan_group[i]->ethernett[j]==1)
			{
				sprintf(tmp," Te%d",j+1);
				write(fd,tmp,strlen(tmp));
			}			
		}	
		if(web_api->adv_vlan_group[i]->usbp==1)	
			write(fd," PU1",3);
		if(web_api->adv_vlan_group[i]->usbt==1)	
			write(fd," TU1",3);
		sprintf(tmp," \"\n");		
		write(fd,tmp,strlen(tmp));	
	}
	close(fd);
	ASP_FREE(tmp);
}
char * TcWebApiAdv_vlan_group_get(int id)
{
	int i,j;
	char *tmp;
	tmp=(char *)ASP_MALLOC(128);
	if(vlan_index<1 ||vlan_index>8)
		vlan_index=1;
	switch(id)
	{
		case GB_C_VLAN_INDEX:
			for(i=1;i<9;i++)
			{
				if(i==vlan_index)
					sprintf(tmp,"<OPTION SELECTED>%d",i);
				else
					sprintf(tmp,"<OPTION >%d",i);
				ASP_OUTPUT(tmp);	
			}
			break;
		case GB_C_VLAN_ACTIVEYES:
			if(strcmp(web_api->adv_vlan_group[vlan_index-1]->active,val_def->vlan_group_active_yes)==0)
				ASP_OUTPUT("CHECKED");
			break;		
		case GB_C_VLAN_ACTIVENO:
			if(strcmp(web_api->adv_vlan_group[vlan_index-1]->active,val_def->vlan_group_active_no)==0)
				ASP_OUTPUT("CHECKED");		
			break;  
		case GB_C_VLAN_ID:
			if(strlen(web_api->adv_vlan_group[vlan_index-1]->vlan_id)>0)
				sprintf(tmp,"\"%s\"",web_api->adv_vlan_group[vlan_index-1]->vlan_id);	
			else
				sprintf(tmp,"\"0\"");
			ASP_OUTPUT(tmp);
			break;  	
		case GB_C_VLAN_ATMTAGGED0:
			if(web_api->adv_vlan_group[vlan_index-1]->atmt[0]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ATMTAGGED1:
			if(web_api->adv_vlan_group[vlan_index-1]->atmt[1]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ATMTAGGED2:
			if(web_api->adv_vlan_group[vlan_index-1]->atmt[2]==1)
				ASP_OUTPUT("CHECKED");
			break; 				
		case GB_C_VLAN_ATMTAGGED3:
			fprintf(stderr,"----geton?--%d\n",web_api->adv_vlan_group[vlan_index-1]->atmt[3]);
			if(web_api->adv_vlan_group[vlan_index-1]->atmt[3]==1)
				ASP_OUTPUT("CHECKED");
			break;	
		case GB_C_VLAN_ATMTAGGED4:
			if(web_api->adv_vlan_group[vlan_index-1]->atmt[4]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ATMTAGGED5:
			if(web_api->adv_vlan_group[vlan_index-1]->atmt[5]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ATMTAGGED6:
			if(web_api->adv_vlan_group[vlan_index-1]->atmt[6]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ATMTAGGED7:
			if(web_api->adv_vlan_group[vlan_index-1]->atmt[7]==1)
				ASP_OUTPUT("CHECKED");
			break; 
			
		case GB_C_VLAN_ATMPORT0:
			if(web_api->adv_vlan_group[vlan_index-1]->atmp[0]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ATMPORT1:
			if(web_api->adv_vlan_group[vlan_index-1]->atmp[1]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ATMPORT2:
			if(web_api->adv_vlan_group[vlan_index-1]->atmp[2]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ATMPORT3:
			if(web_api->adv_vlan_group[vlan_index-1]->atmp[3]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ATMPORT4:
			if(web_api->adv_vlan_group[vlan_index-1]->atmp[4]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ATMPORT5:
			if(web_api->adv_vlan_group[vlan_index-1]->atmp[5]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ATMPORT6:
			if(web_api->adv_vlan_group[vlan_index-1]->atmp[6]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ATMPORT7:
			if(web_api->adv_vlan_group[vlan_index-1]->atmp[7]==1)
				ASP_OUTPUT("CHECKED");
			break; 
				
		case GB_C_VLAN_ETHERNETTAGGED1:
			if(web_api->adv_vlan_group[vlan_index-1]->ethernett[0]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ETHERNETTAGGED2:
			if(web_api->adv_vlan_group[vlan_index-1]->ethernett[1]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ETHERNETTAGGED3:
			if(web_api->adv_vlan_group[vlan_index-1]->ethernett[2]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ETHERNETTAGGED4:
			if(web_api->adv_vlan_group[vlan_index-1]->ethernett[3]==1)
				ASP_OUTPUT("CHECKED");
			break; 
										
		case GB_C_VLAN_ETHERNETPORT1:
			if(web_api->adv_vlan_group[vlan_index-1]->ethernetp[0]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ETHERNETPORT2:
			if(web_api->adv_vlan_group[vlan_index-1]->ethernetp[1]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ETHERNETPORT3:
			if(web_api->adv_vlan_group[vlan_index-1]->ethernetp[2]==1)
				ASP_OUTPUT("CHECKED");
			break; 
		case GB_C_VLAN_ETHERNETPORT4:
			if(web_api->adv_vlan_group[vlan_index-1]->ethernetp[3]==1)
				ASP_OUTPUT("CHECKED");	
			break; 		
				
		case GB_C_VLAN_USBTAGGED1:	
			if(web_api->adv_vlan_group[vlan_index-1]->usbt==1)
				ASP_OUTPUT("CHECKED");	
			break; 
					
		case GB_C_VLAN_USBPORT1:
			if(web_api->adv_vlan_group[vlan_index-1]->usbp==1)
				ASP_OUTPUT("CHECKED");	
			break;   
		case GB_C_VLAN_SUMMARY:
			for(i=0;i<8;i++)
			{	
			  if(strcmp(web_api->adv_vlan_group[i]->active,val_def->vlan_group_active_yes)==0)
			  {	
				sprintf(tmp,"<tr><td align='center' class='tabdata'> %d </td><td align='center' class='tabdata'>Yes</td>",i+1);
				ASP_OUTPUT(tmp);
				sprintf(tmp,"<td align='center' class='tabdata'> %s </td><td align='center' class='tabdata'>",web_api->adv_vlan_group[i]->vlan_id);
				ASP_OUTPUT(tmp);
				memset(tmp,0,sizeof(tmp));
				if(web_api->adv_vlan_group[i]->usbp==1)
				{
					sprintf(tmp,"u1,");
					ASP_OUTPUT(tmp);				
				}
				for(j=0;j<4;j++)
					if(web_api->adv_vlan_group[i]->ethernetp[j]==1)
					{
						sprintf(tmp,"e%d,",j+1);
						ASP_OUTPUT(tmp);	
					}
				for(j=0;j<8;j++)
					if(web_api->adv_vlan_group[i]->atmp[j]==1)
					{
						sprintf(tmp,"p%d,",j);
						ASP_OUTPUT(tmp);	
					}
				if(strlen(tmp)==0)
					ASP_OUTPUT(" - ");	
				else				
					memset(tmp,0,sizeof(tmp));
				ASP_OUTPUT("</td><td align='center' class='tabdata'>");
				if(web_api->adv_vlan_group[i]->usbt==1)
				{
					sprintf(tmp,"u1,");
					ASP_OUTPUT(tmp);				
				}
				for(j=0;j<4;j++)
					if(web_api->adv_vlan_group[i]->ethernett[j]==1)
					{
						sprintf(tmp,"e%d,",j+1);
						ASP_OUTPUT(tmp);	
					}
				for(j=0;j<8;j++)
					if(web_api->adv_vlan_group[i]->atmt[j]==1)
					{
						sprintf(tmp,"p%d,",j);
						ASP_OUTPUT(tmp);	
					}
				if(strlen(tmp)==0)
					ASP_OUTPUT(" - ");	
				ASP_OUTPUT("</td></tr>");	
			  }
			}
			break;		
			
	}		
		
	
	ASP_FREE(tmp);  
	return ASP_OK;
}

int TcWebApiAdv_vlan_group_set(int id,char*value)
{
	char *ret,newret[1];
	
	
	newret[0]=0;
	ret=ASP_FORM(value);
	if(ret==NULL)
		ret=newret; 

	
	switch(id)
	{
		case GB_C_VLAN_INDEX:
			vlan_index=asp_atoi(ret);
			break;	
		case GB_C_VLAN_ACTIVE:	
			strncpy(bu_active,web_api->adv_vlan_group[vlan_index-1]->active,8);		
			if(strcmp(ret,val_def->vlan_group_active_yes)==0)
				strncpy(web_api->adv_vlan_group[vlan_index-1]->active,ret,8);
			else if(strcmp(ret,val_def->vlan_group_active_no)==0)
				strncpy(web_api->adv_vlan_group[vlan_index-1]->active,ret,8);		
			break;	
		case GB_C_VLAN_ID:
			strncpy(bu_vlan_id,web_api->adv_vlan_group[vlan_index-1]->vlan_id,8);
			while(*ret=='0')
				ret++;
			if(ret)	
				strncpy(web_api->adv_vlan_group[vlan_index-1]->vlan_id,ret,8);		
			break;	
		case GB_C_VLAN_ATMTAGGED0:
			bu_atmt[0]=web_api->adv_vlan_group[vlan_index-1]->atmt[0];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmt[0]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmt[0]=0;
			break;	
		case GB_C_VLAN_ATMTAGGED1:
			bu_atmt[1]=web_api->adv_vlan_group[vlan_index-1]->atmt[1];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmt[1]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmt[1]=0;
			break;	
		case GB_C_VLAN_ATMTAGGED2:
			bu_atmt[2]=web_api->adv_vlan_group[vlan_index-1]->atmt[2];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmt[2]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmt[2]=0;
			break;	
		case GB_C_VLAN_ATMTAGGED3:
			bu_atmt[3]=web_api->adv_vlan_group[vlan_index-1]->atmt[3];
			fprintf(stderr,"----seton?--%s\n",ret);
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmt[3]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmt[3]=0;
			break;		
		case GB_C_VLAN_ATMTAGGED4:
			bu_atmt[4]=web_api->adv_vlan_group[vlan_index-1]->atmt[4];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmt[4]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmt[4]=0;
			break;	
		case GB_C_VLAN_ATMTAGGED5:
			bu_atmt[5]=web_api->adv_vlan_group[vlan_index-1]->atmt[5];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmt[5]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmt[5]=0;
			
			break;	
		case GB_C_VLAN_ATMTAGGED6:
			bu_atmt[6]=web_api->adv_vlan_group[vlan_index-1]->atmt[6];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmt[6]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmt[6]=0;
			break;	
	
		case GB_C_VLAN_ATMTAGGED7:
			bu_atmt[7]=web_api->adv_vlan_group[vlan_index-1]->atmt[7];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmt[7]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmt[7]=0;
			break;	
		case GB_C_VLAN_ATMPORT0:
			bu_atmp[0]=web_api->adv_vlan_group[vlan_index-1]->atmp[0];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmp[0]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmp[0]=0;
			break;	
		case GB_C_VLAN_ATMPORT1:
			bu_atmp[1]=web_api->adv_vlan_group[vlan_index-1]->atmp[1];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmp[1]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmp[1]=0;
			break;	
		case GB_C_VLAN_ATMPORT2:
			bu_atmp[2]=web_api->adv_vlan_group[vlan_index-1]->atmp[2];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmp[2]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmp[2]=0;
			break;	
		case GB_C_VLAN_ATMPORT3:
			bu_atmp[3]=web_api->adv_vlan_group[vlan_index-1]->atmp[3];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmp[3]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmp[3]=0;
			break;	
		case GB_C_VLAN_ATMPORT4:
			bu_atmp[4]=web_api->adv_vlan_group[vlan_index-1]->atmp[4];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmp[4]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmp[4]=0;
			break;	
		case GB_C_VLAN_ATMPORT5:
			bu_atmp[5]=web_api->adv_vlan_group[vlan_index-1]->atmp[5];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmp[5]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmp[5]=0;
			break;	
		case GB_C_VLAN_ATMPORT6:
			bu_atmp[6]=web_api->adv_vlan_group[vlan_index-1]->atmp[6];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmp[6]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmp[6]=0;
			break;	
		case GB_C_VLAN_ATMPORT7:
			bu_atmp[7]=web_api->adv_vlan_group[vlan_index-1]->atmp[7];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->atmp[7]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->atmp[7]=0;
			break;	
			
		case GB_C_VLAN_ETHERNETTAGGED1:
			bu_ethernett[0]=web_api->adv_vlan_group[vlan_index-1]->ethernett[0];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->ethernett[0]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->ethernett[0]=0;
			break;		
		case GB_C_VLAN_ETHERNETTAGGED2:
			bu_ethernett[1]=web_api->adv_vlan_group[vlan_index-1]->ethernett[1];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->ethernett[1]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->ethernett[1]=0;
			break;				
		case GB_C_VLAN_ETHERNETTAGGED3:
			bu_ethernett[2]=web_api->adv_vlan_group[vlan_index-1]->ethernett[2];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->ethernett[2]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->ethernett[2]=0;
			break;		
		case GB_C_VLAN_ETHERNETTAGGED4:
			bu_ethernett[3]=web_api->adv_vlan_group[vlan_index-1]->ethernett[3];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->ethernett[3]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->ethernett[3]=0;
			break;	
			
		case GB_C_VLAN_ETHERNETPORT1:
			bu_ethernetp[0]=web_api->adv_vlan_group[vlan_index-1]->ethernetp[0];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->ethernetp[0]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->ethernetp[0]=0;
			break;		
		case GB_C_VLAN_ETHERNETPORT2:
			bu_ethernetp[1]=web_api->adv_vlan_group[vlan_index-1]->ethernetp[1];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->ethernetp[1]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->ethernetp[1]=0;
			break;				
		case GB_C_VLAN_ETHERNETPORT3:
			bu_ethernetp[2]=web_api->adv_vlan_group[vlan_index-1]->ethernetp[2];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->ethernetp[2]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->ethernetp[2]=0;
			break;		
		case GB_C_VLAN_ETHERNETPORT4:
			bu_ethernetp[3]=web_api->adv_vlan_group[vlan_index-1]->ethernetp[3];
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->ethernetp[3]=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->ethernetp[3]=0;
			break;				
					
		case GB_C_VLAN_USBTAGGED1:
			bu_usbt=web_api->adv_vlan_group[vlan_index-1]->usbt;
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->usbt=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->usbt=0;
			break;	
				
		case GB_C_VLAN_USBPORT1:
			bu_usbp=web_api->adv_vlan_group[vlan_index-1]->usbp;
			if(strcmp(ret,"on")==0)
				web_api->adv_vlan_group[vlan_index-1]->usbp=1;
			else
				web_api->adv_vlan_group[vlan_index-1]->usbp=0;
			break;			
			

	}
	return 0;
}

int TcWebApiAdv_vlan_group_execute(int id)
{
		int i;
		if(id==GB_C_VLAN_GROUPSETTING)
		{fprintf(stderr,"-----save\n");
			group_del();
			group_add();
		}



		if(id==GB_C_VLAN_GROUPDELETE)
		{
			strncpy(bu_active,web_api->adv_vlan_group[vlan_index-1]->active,8);
			strncpy(web_api->adv_vlan_group[vlan_index-1]->active,val_def->vlan_group_active_no,8);	
			
			strncpy(bu_vlan_id,web_api->adv_vlan_group[vlan_index-1]->vlan_id,8);
			memset(web_api->adv_vlan_group[vlan_index-1]->vlan_id,0,sizeof(web_api->adv_vlan_group[vlan_index-1]->vlan_id));
			
			for(i=0;i<8;i++)
			{
				bu_atmt[i]=web_api->adv_vlan_group[vlan_index-1]->atmt[i];
				web_api->adv_vlan_group[vlan_index-1]->atmt[i]=0;
				
				bu_atmp[i]=web_api->adv_vlan_group[vlan_index-1]->atmp[i];
				web_api->adv_vlan_group[vlan_index-1]->atmp[i]=0;
			}
			for(i=0;i<4;i++)
			{
				bu_ethernett[i]=web_api->adv_vlan_group[vlan_index-1]->ethernett[i];
				web_api->adv_vlan_group[vlan_index-1]->ethernett[i]=0;
				
				bu_ethernetp[i]=web_api->adv_vlan_group[vlan_index-1]->ethernetp[i];
				web_api->adv_vlan_group[vlan_index-1]->ethernetp[i]=0;
			}
			bu_usbt=web_api->adv_vlan_group[vlan_index-1]->usbt;
			web_api->adv_vlan_group[vlan_index-1]->usbt=0;
			
			bu_usbp=web_api->adv_vlan_group[vlan_index-1]->usbp;
			web_api->adv_vlan_group[vlan_index-1]->usbp=0;	
	
			group_del();
		}

	group_write();
	return 0;
}


