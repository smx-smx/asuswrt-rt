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

#define WLAN_SHELL 	"/etc/Wireless/RT61AP/WLAN_exec.sh"
#define SSID_INDEX atoi(web_api->home_wireless->ssid_index)-1

extern void decode_string (char * des,char * src);
void ChannelCountry();
void Wireless_WriteFile();
int check_wireless_card();/* check the CPE has wireless card or not shnwind*/

int Chr_to_Hex(char *src,char *target);/* this two file should have return value shnwind 2008.4.17*/
int Hex_to_Chr(char *src,char *target);
void  insertChar2string(char *str, int len, int ascii);

char check_page[4]={0,0,0,0};
unsigned char StartChannelNumber = 1; 
unsigned char EndChannelNumber = 13; 
static char LastWepKey[4][32];
static char commands[22][200];
char wireless_card_open[6];/*for check wireless card shnwind 2008.4.17*/

char *TcWebApiWireless_get(int id)
{
	int i;
	char *tmp;
	char buf[64],value[64];
	char sign=34;
	
	tmp=(char*)ASP_MALLOC(128);	
	if(tmp==NULL)
		goto err;
	memset(tmp,0,128);
		
	switch (id)
	{       /*shnwind add 2008.4.17*/
	        case GB_C_WIRELESS_DEVICE_STATUS:
	              memset(wireless_card_open,0,sizeof(wireless_card_open));
                      switch(check_wireless_card()){
                        case 0:
                        sprintf(wireless_card_open,"off");
                          break;
                        case 1:
                        sprintf(wireless_card_open,"on");
                          break;
                        case 2:
                          sprintf(wireless_card_open,"error");
                          break;
                        default:break;
                      }
                      break;
		case GB_C_WIRELESS_ACCESS_ACTIVATED:
		      if(strcmp(web_api->home_wireless->access,val_def->wireless_access_activated)==0)
		        	ASP_OUTPUT("CHECKED");
		    	break;			
		case GB_C_WIRELESS_ACCESS_DEACTIVATED:
		      if(strcmp(web_api->home_wireless->access,val_def->wireless_access_deactivated)==0)
		       	ASP_OUTPUT("CHECKED");
					break;    	
		case GB_C_WIRELESS_CHANNEL:
    			ChannelCountry();
    			break;
		case GB_C_WIRELESS_CHANNELNUM://need check wkw
/*		    	if(!strcmp(web_api->home_wireless->channel_num,val_def->wireless_channel_auto))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_auto);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_auto);
		    	ASP_OUTPUT(tmp);	
*/
			    for(i = StartChannelNumber;i <= EndChannelNumber;i++)        //wjx 2007.4.16  
		      {
		      		if(i == atoi(web_api->home_wireless->channel_num))
		      			if(i < 10)
		      				sprintf(tmp,"<OPTION SELECTED >0%d",i);
		      			else
		      				sprintf(tmp,"<OPTION SELECTED >%d",i);
		      		else
		      			if(i < 10)
		      				sprintf(tmp,"<OPTION>0%d",i);
		      			else
		      				sprintf(tmp,"<OPTION>%d",i);
		      		ASP_OUTPUT(tmp);	
		   		}			    	   		
		    	break;			
		    	
		case GB_C_WIRELESS_CURRENTCHANNEL:
		     	sprintf(tmp,"\"%s\"",web_api->home_wireless->current_channel);
		      ASP_OUTPUT(tmp);
		    	break;  		 

		case GB_C_WIRELESS_INTERVAL:
		     	sprintf(tmp,"\"%s\"",web_api->home_wireless->beacon);
		      ASP_OUTPUT(tmp);
		    	break;  		 
		case GB_C_WIRELESS_RTSCTS:
		     	sprintf(tmp,"\"%s\"",web_api->home_wireless->RTS_CTS);
		      ASP_OUTPUT(tmp);
		    	break;  		 		    			    	 		    	  			    				
		case GB_C_WIRELESS_FRAGMENTATION:
		     	sprintf(tmp,"\"%s\"",web_api->home_wireless->Fragmentation);
		      ASP_OUTPUT(tmp);
		    	break;  		 
		case GB_C_WIRELESS_DTIM:
		     	sprintf(tmp,"\"%s\"",web_api->home_wireless->DTIM);
		      ASP_OUTPUT(tmp);
		    	break;  		 
		case GB_C_WIRELESS_80211BG:
		    	if(!strcmp(web_api->home_wireless->bg80211,val_def->wireless_80211bg_80211b))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_80211bg_80211b);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_80211bg_80211b);
		    	ASP_OUTPUT(tmp);	
		    		
		    	if(!strcmp(web_api->home_wireless->bg80211,val_def->wireless_80211bg_80211g))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_80211bg_80211g);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_80211bg_80211g);
		    	ASP_OUTPUT(tmp);	
		    			    	    			    			
		    	if(!strcmp(web_api->home_wireless->bg80211,val_def->wireless_80211bg_80211bg))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_80211bg_80211bg);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_80211bg_80211bg);
		    	ASP_OUTPUT(tmp);	
		    	
		    	break;			
		    	
//		case GB_C_WIRELESS_SSIDINDEX: 
//		      for(i=0;i<SSID_COUNT;i++)
//		      {
//		      		if(i==SSID_INDEX)
//		      			sprintf(tmp,"<OPTION SELECTED >%d",i+1);
//		      		else
//		      			sprintf(tmp,"<OPTION>%d",i+1);
//		      		ASP_OUTPUT(tmp);	
//		   		}  		
//		    	break;			    			    	
/***************************************************///ssid 		    	
		case GB_C_WIRELESS_SSID: 
			#if 0
		     	sprintf(tmp,"\"%s\"",web_api->home_wireless->SSID[SSID_INDEX]->SSID);		     	
		      ASP_OUTPUT(tmp);
			#endif
			 strcpy(buf, web_api->home_wireless->SSID[SSID_INDEX]->SSID);
			 checkQuotationMarks(buf,strlen(buf));
			 sprintf(value,"%c%s%c",sign,buf,sign);
			 ASP_OUTPUT(value);
		    	break;  		 											  	
		case GB_C_WIRELESS_BROADCAST_YES: 
					if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->broadcast,val_def->wireless_broadcast_yes))
		     	   	ASP_OUTPUT("CHECKED");
		    	break;  	
		case GB_C_WIRELESS_BROADCAST_NO: 
					if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->broadcast,val_def->wireless_broadcast_no))
		     	   	ASP_OUTPUT("CHECKED");
		    	break;  			    					
		case GB_C_WIRELESS_AUTHENTICATION: 	
		    	if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_disabled))
		    	{	
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_authentication_disabled);
		    		sprintf(check_page,"0");
		    	}	
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_authentication_disabled);
		    	ASP_OUTPUT(tmp);			
		    					    	
		    	if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep64bits))
		    	{
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_authentication_wep64bits);
		    		sprintf(check_page,"1");
		    	}	
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_authentication_wep64bits);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep128bits))
		    	{	
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_authentication_wep128bits);
		    		sprintf(check_page,"1");
		    	}
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_authentication_wep128bits);
		    	ASP_OUTPUT(tmp);
		    	
		    	if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wpapsk))
		    	{	
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_authentication_wpapsk);
		    		sprintf(check_page,"2");
		    	}
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_authentication_wpapsk);
		    	ASP_OUTPUT(tmp);

		    	if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wpa2psk))
		    	{	
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_authentication_wpa2psk);
		    		sprintf(check_page,"2");
		    	}
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_authentication_wpa2psk);
		    	ASP_OUTPUT(tmp);
		    	break;

		case 	GB_C_WIRELESS_KEYO1:	
					if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep64bits))
					{	
						if(atoi(web_api->home_wireless->SSID[SSID_INDEX]->keyIndex_64)==1)
					 	   	ASP_OUTPUT("CHECKED");
					}
					else if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep128bits))
					{
						if(atoi(web_api->home_wireless->SSID[SSID_INDEX]->keyIndex_128)==1)
					 	   	ASP_OUTPUT("CHECKED");							
					}					 	   	
		    	break; 
		case 	GB_C_WIRELESS_KEYO2:	
					if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep64bits))
					{	
						if(atoi(web_api->home_wireless->SSID[SSID_INDEX]->keyIndex_64)==2)
					 	   	ASP_OUTPUT("CHECKED");
					}
					else if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep128bits))
					{
						if(atoi(web_api->home_wireless->SSID[SSID_INDEX]->keyIndex_128)==2)
					 	   	ASP_OUTPUT("CHECKED");							
					}					 	   	
		    	break; 	 									    	
		case 	GB_C_WIRELESS_KEYO3:	
					if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep64bits))
					{	
						if(atoi(web_api->home_wireless->SSID[SSID_INDEX]->keyIndex_64)==3)
					 	   	ASP_OUTPUT("CHECKED");
					}
					else if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep128bits))
					{
						if(atoi(web_api->home_wireless->SSID[SSID_INDEX]->keyIndex_128)==3)
					 	   	ASP_OUTPUT("CHECKED");							
					}					 	   	
		    	break; 
		case 	GB_C_WIRELESS_KEYO4:	
					if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep64bits))
					{	
						if(atoi(web_api->home_wireless->SSID[SSID_INDEX]->keyIndex_64)==4)
					 	   	ASP_OUTPUT("CHECKED");
					}
					else if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep128bits))
					{
						if(atoi(web_api->home_wireless->SSID[SSID_INDEX]->keyIndex_128)==4)
					 	   	ASP_OUTPUT("CHECKED");							
					}					 	   	
		    	break; 

		case 	GB_C_WIRELESS_KEYT1:	
					memset(tmp,'0',127);
					if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep64bits))
					{
						if(Chr_to_Hex(web_api->home_wireless->SSID[SSID_INDEX]->key1_64,tmp)==0)
							sprintf(tmp,"0x0000000000");
					}
					else if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep128bits))
					{
						if(Chr_to_Hex(web_api->home_wireless->SSID[SSID_INDEX]->key1_128,tmp)==0)
							sprintf(tmp,"0x00000000000000000000000000");
					}				
		      ASP_OUTPUT(tmp);
		    	break; 
		case 	GB_C_WIRELESS_KEYT2:	
					if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep64bits))
					{
						if(Chr_to_Hex(web_api->home_wireless->SSID[SSID_INDEX]->key2_64,tmp)==0)
							sprintf(tmp,"0x0000000000");
					}
					else if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep128bits))
					{
						if(Chr_to_Hex(web_api->home_wireless->SSID[SSID_INDEX]->key2_128,tmp)==0)
							sprintf(tmp,"0x00000000000000000000000000");
					}				
		      ASP_OUTPUT(tmp);
		    	break; 		  
		case 	GB_C_WIRELESS_KEYT3:	
					if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep64bits))
					{
						if(Chr_to_Hex(web_api->home_wireless->SSID[SSID_INDEX]->key3_64,tmp)==0)
							sprintf(tmp,"0x0000000000");
					}
					else if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep128bits))
					{
						if(Chr_to_Hex(web_api->home_wireless->SSID[SSID_INDEX]->key3_128,tmp)==0)
							sprintf(tmp,"0x00000000000000000000000000");
					}				
		      ASP_OUTPUT(tmp);
		    	break; 		    	
		case 	GB_C_WIRELESS_KEYT4:	
					if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep64bits))
					{
						if(Chr_to_Hex(web_api->home_wireless->SSID[SSID_INDEX]->key4_64,tmp)==0)
							sprintf(tmp,"0x0000000000");
					}
					else if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep128bits))
					{
						if(Chr_to_Hex(web_api->home_wireless->SSID[SSID_INDEX]->key4_128,tmp)==0)
							sprintf(tmp,"0x00000000000000000000000000");
					}				
		      ASP_OUTPUT(tmp);
		    	break; 		    		

		case	GB_C_WIRELESS_ENCRYPTION:
		    	if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->encryption,val_def->wireless_encryption_tkip))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_encryption_tkip);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_encryption_tkip);
		    	ASP_OUTPUT(tmp);

		    	if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->encryption,val_def->wireless_encryption_aes))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_encryption_aes);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_encryption_aes);
		    	ASP_OUTPUT(tmp);					
					
					break;
		case	GB_C_WIRELESS_PRESHARED:
		     	sprintf(tmp,"\"%s\"",web_api->home_wireless->SSID[SSID_INDEX]->pre_shared);
		      ASP_OUTPUT(tmp);
		    	break;  
		    	
		case	GB_C_WIRELESS_CHECKPAGE:  
					{  	
						ASP_FREE(tmp);	
						return check_page;
					}

		case GB_C_WIRELESS_ACTIVE_ACTIVATED:
					if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->active,val_def->wireless_active_activated))
		     	   	ASP_OUTPUT("CHECKED");
		    	break;
		case GB_C_WIRELESS_ACTIVE_DEACTIVATED: 
					if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->active,val_def->wireless_active_deactivated))
		     	   	ASP_OUTPUT("CHECKED");
		    	break;  			 				
		case GB_C_WIRELESS_ACTION:  
		    	if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->action,val_def->wireless_action_allow))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_action_allow);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_action_allow);
		    	ASP_OUTPUT(tmp);

		    	if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->action,val_def->wireless_action_deny))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_action_deny);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_action_deny);
		    	ASP_OUTPUT(tmp);
		    	
		    	break;									
					
		case GB_C_WIRELESS_MAC1: 
					if(strlen(web_api->home_wireless->SSID[SSID_INDEX]->mac1)==0)
						sprintf(tmp,"\"%s\"",val_def->wireless_default_mac);
					else
		     		sprintf(tmp,"\"%s\"",web_api->home_wireless->SSID[SSID_INDEX]->mac1);
		      ASP_OUTPUT(tmp);
		    	break;  									
		case GB_C_WIRELESS_MAC2: 
					if(strlen(web_api->home_wireless->SSID[SSID_INDEX]->mac2)==0)
						sprintf(tmp,"\"%s\"",val_def->wireless_default_mac);
					else
		     		sprintf(tmp,"\"%s\"",web_api->home_wireless->SSID[SSID_INDEX]->mac2);
		      ASP_OUTPUT(tmp);
		    	break;  			
		case GB_C_WIRELESS_MAC3: 
					if(strlen(web_api->home_wireless->SSID[SSID_INDEX]->mac3)==0)
						sprintf(tmp,"\"%s\"",val_def->wireless_default_mac);
					else
		     		sprintf(tmp,"\"%s\"",web_api->home_wireless->SSID[SSID_INDEX]->mac3);
		      ASP_OUTPUT(tmp);
		    	break;  									
		case GB_C_WIRELESS_MAC4: 
					if(strlen(web_api->home_wireless->SSID[SSID_INDEX]->mac4)==0)
						sprintf(tmp,"\"%s\"",val_def->wireless_default_mac);
					else
		     		sprintf(tmp,"\"%s\"",web_api->home_wireless->SSID[SSID_INDEX]->mac4);
		      ASP_OUTPUT(tmp);
		    	break;  
		case GB_C_WIRELESS_MAC5: 
					if(strlen(web_api->home_wireless->SSID[SSID_INDEX]->mac5)==0)
						sprintf(tmp,"\"%s\"",val_def->wireless_default_mac);
					else
		     		sprintf(tmp,"\"%s\"",web_api->home_wireless->SSID[SSID_INDEX]->mac5);
		      ASP_OUTPUT(tmp);
		    	break;  									
		case GB_C_WIRELESS_MAC6: 
					if(strlen(web_api->home_wireless->SSID[SSID_INDEX]->mac6)==0)
						sprintf(tmp,"\"%s\"",val_def->wireless_default_mac);
					else
		     		sprintf(tmp,"\"%s\"",web_api->home_wireless->SSID[SSID_INDEX]->mac6);
		      ASP_OUTPUT(tmp);
		    	break;  			
		case GB_C_WIRELESS_MAC7: 
					if(strlen(web_api->home_wireless->SSID[SSID_INDEX]->mac7)==0)
						sprintf(tmp,"\"%s\"",val_def->wireless_default_mac);
					else
		     		sprintf(tmp,"\"%s\"",web_api->home_wireless->SSID[SSID_INDEX]->mac7);
		      ASP_OUTPUT(tmp);
		    	break;  									
		case GB_C_WIRELESS_MAC8: 
					if(strlen(web_api->home_wireless->SSID[SSID_INDEX]->mac8)==0)
						sprintf(tmp,"\"%s\"",val_def->wireless_default_mac);
					else
		     		sprintf(tmp,"\"%s\"",web_api->home_wireless->SSID[SSID_INDEX]->mac8);
		      ASP_OUTPUT(tmp);
		    	break; 					
							    	
	}	    	  	
			    						
err:
	if(tmp)	
		ASP_FREE(tmp);	
  /*for return wireless state shnwind 2008.4.17*/
  if (strlen(wireless_card_open)){
    return wireless_card_open;
  }else{						
	return ASP_OK; 
}
}

int TcWebApiWireless_set(int id,char*value)
{
	char *ret=NULL;
	unsigned char pre_access, now_access;
	
	ret =	ASP_FORM(value);
	if(ret ==NULL)
	   return -1;	
	
	switch (id)
	{
		case GB_C_WIRELESS_ACCESS:
			pre_access = atoi(web_api->home_wireless->access);
			now_access = atoi(ret);
			if(pre_access != now_access)
			{
				if(!now_access)
					strcpy(commands[0], "ifconfig ra0 down\n");//add '\n', shnwind 2008.4.29
				else
					strcpy(commands[0], "ifconfig ra0 0.0.0.0 up\n");//add '\n', shnwind 2008.4.29
			}
			else
				memset(commands[0], 0, 200);
			sprintf(web_api->home_wireless->access,"%s",ret);
			break;
		case GB_C_WIRELESS_CHANNEL:
			sprintf(web_api->home_wireless->channel,"%s",ret);
			if (!strcmp(ret,val_def->wireless_channel_spain))
			{
			 	StartChannelNumber = 10;
				EndChannelNumber = 11;				
			}
			else if (!strcmp(ret,val_def->wireless_channel_isarel))
			{
			 	StartChannelNumber = 5;
				EndChannelNumber = 7;				
			}			
			else if (!strcmp(ret,val_def->wireless_channel_japan))
			{
			 	StartChannelNumber = 1;
				EndChannelNumber = 14;				
			}		
			else if ((!strcmp(ret,val_def->wireless_channel_france)) || (!strcmp(ret,val_def->wireless_channel_jordan)))
			{
			 	StartChannelNumber = 10;
				EndChannelNumber = 13;				
			}							
			else if((!strcmp(ret,val_def->wireless_channel_canada)) || (!strcmp(ret,val_def->wireless_channel_colombia)) \
				|| (!strcmp(ret,val_def->wireless_channel_dominicanrepublic)) || (!strcmp(ret,val_def->wireless_channel_georgia)) \
				|| (!strcmp(ret,val_def->wireless_channel_mexico)) || (!strcmp(ret,val_def->wireless_channel_panama)) \
				|| (!strcmp(ret,val_def->wireless_channel_puerto_rico)) || (!strcmp(ret,val_def->wireless_channel_unitedstates)) \
				|| (!strcmp(ret,val_def->wireless_channel_guatemala)) )
			{
			 	StartChannelNumber = 1;
				EndChannelNumber = 11;
			}
			else
			{
			 	StartChannelNumber = 1;
				EndChannelNumber = 13;
			}
			
			break;			
		case GB_C_WIRELESS_CHANNELNUM:
			sprintf(web_api->home_wireless->channel_num,"%s",ret);
//			if(!strcmp(ret,val_def->wireless_channel_auto))
//				sprintf(web_api->home_wireless->current_channel,"%s",ret);//**when atou
//			else
				sprintf(web_api->home_wireless->current_channel,"%s",ret);
			break;
		case GB_C_WIRELESS_INTERVAL:
			sprintf(web_api->home_wireless->beacon,"%s",ret);
			break;
		case GB_C_WIRELESS_RTSCTS:
			sprintf(web_api->home_wireless->RTS_CTS,"%s",ret);
			break;				
		case GB_C_WIRELESS_FRAGMENTATION:
			sprintf(web_api->home_wireless->Fragmentation,"%s",ret);
			break;
		case GB_C_WIRELESS_DTIM:
			sprintf(web_api->home_wireless->DTIM,"%s",ret);
			break;			
		case GB_C_WIRELESS_80211BG:					
			decode_string(web_api->home_wireless->bg80211,ret);
			break;	
			
//		case GB_C_WIRELESS_SSIDINDEX:
//			sprintf(web_api->home_wireless->ssid_index,"%s",ret);
//			break;				
		case GB_C_WIRELESS_SSID:
			ascii2punct(ret,32);
			sprintf(web_api->home_wireless->SSID[SSID_INDEX]->SSID,"%s",ret);
			break;
		case GB_C_WIRELESS_BROADCAST:
			sprintf(web_api->home_wireless->SSID[SSID_INDEX]->broadcast,"%s",ret);
			break;			
		case GB_C_WIRELESS_AUTHENTICATION:
			sprintf(web_api->home_wireless->SSID[SSID_INDEX]->authentication,"%s",ret);
			break;		
			
		case GB_C_WIRELESS_KEYO:
			if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep64bits))
				sprintf(web_api->home_wireless->SSID[SSID_INDEX]->keyIndex_64,"%s",ret);
			else if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep128bits))
				sprintf(web_api->home_wireless->SSID[SSID_INDEX]->keyIndex_128,"%s",ret);
			break;			
		case GB_C_WIRELESS_KEYT1:
			if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep64bits))
			{
				if(*ret=='0'&&*(ret+1)=='x')
				{
					Hex_to_Chr(ret,web_api->home_wireless->SSID[SSID_INDEX]->key1_64);
				}
				else
					decode_string(web_api->home_wireless->SSID[SSID_INDEX]->key1_64,ret);
			}
			else if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep128bits))
			{	
				if(*ret=='0'&&*(ret+1)=='x')
				{
					Hex_to_Chr(ret,web_api->home_wireless->SSID[SSID_INDEX]->key1_128);
				}		
				else		
					decode_string(web_api->home_wireless->SSID[SSID_INDEX]->key1_128,ret);
			}	
			break;													
		case GB_C_WIRELESS_KEYT2:
			if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep64bits))
			{
				if(*ret=='0'&&*(ret+1)=='x')
				{
					Hex_to_Chr(ret,web_api->home_wireless->SSID[SSID_INDEX]->key2_64);
				}
				else
					decode_string(web_api->home_wireless->SSID[SSID_INDEX]->key2_64,ret);
			}
			else if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep128bits))
			{	
				if(*ret=='0'&&*(ret+1)=='x')
				{
					Hex_to_Chr(ret,web_api->home_wireless->SSID[SSID_INDEX]->key2_128);
				}		
				else		
					decode_string(web_api->home_wireless->SSID[SSID_INDEX]->key2_128,ret);
			}	
			break;			
		case GB_C_WIRELESS_KEYT3:
			if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep64bits))
			{
				if(*ret=='0'&&*(ret+1)=='x')
				{
					Hex_to_Chr(ret,web_api->home_wireless->SSID[SSID_INDEX]->key3_64);
				}
				else
					decode_string(web_api->home_wireless->SSID[SSID_INDEX]->key3_64,ret);
			}
			else if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep128bits))
			{	
				if(*ret=='0'&&*(ret+1)=='x')
				{
					Hex_to_Chr(ret,web_api->home_wireless->SSID[SSID_INDEX]->key3_128);
				}		
				else		
					decode_string(web_api->home_wireless->SSID[SSID_INDEX]->key3_128,ret);
			}	
			break;													
		case GB_C_WIRELESS_KEYT4:
			if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep64bits))
			{
				if(*ret=='0'&&*(ret+1)=='x')
				{
					Hex_to_Chr(ret,web_api->home_wireless->SSID[SSID_INDEX]->key4_64);
				}
				else
					decode_string(web_api->home_wireless->SSID[SSID_INDEX]->key4_64,ret);
			}
			else if(!strcmp(web_api->home_wireless->SSID[SSID_INDEX]->authentication,val_def->wireless_authentication_wep128bits))
			{	
				if(*ret=='0'&&*(ret+1)=='x')
				{
					Hex_to_Chr(ret,web_api->home_wireless->SSID[SSID_INDEX]->key4_128);
				}		
				else		
					decode_string(web_api->home_wireless->SSID[SSID_INDEX]->key4_128,ret);
			}	
			break;		
			
		case GB_C_WIRELESS_ENCRYPTION:
			sprintf(web_api->home_wireless->SSID[SSID_INDEX]->encryption,"%s",ret);
			break;													
		case GB_C_WIRELESS_PRESHARED:
			sprintf(web_api->home_wireless->SSID[SSID_INDEX]->pre_shared,"%s",ret);
			break;	
			
		case GB_C_WIRELESS_ACTIVE:
			sprintf(web_api->home_wireless->SSID[SSID_INDEX]->active,"%s",ret);
			break;				
		case GB_C_WIRELESS_ACTION:
			sprintf(web_api->home_wireless->SSID[SSID_INDEX]->action,"%s",ret);
			break;
		case GB_C_WIRELESS_MAC1:
			decode_string(web_api->home_wireless->SSID[SSID_INDEX]->mac1,ret);
			break;			
		case GB_C_WIRELESS_MAC2:
			decode_string(web_api->home_wireless->SSID[SSID_INDEX]->mac2,ret);
			break;													
		case GB_C_WIRELESS_MAC3:
			decode_string(web_api->home_wireless->SSID[SSID_INDEX]->mac3,ret);
			break;			
		case GB_C_WIRELESS_MAC4:
			decode_string(web_api->home_wireless->SSID[SSID_INDEX]->mac4,ret);
			break;													
		case GB_C_WIRELESS_MAC5:
			decode_string(web_api->home_wireless->SSID[SSID_INDEX]->mac5,ret);
			break;			
		case GB_C_WIRELESS_MAC6:
			decode_string(web_api->home_wireless->SSID[SSID_INDEX]->mac6,ret);
			break;													
		case GB_C_WIRELESS_MAC7:
			decode_string(web_api->home_wireless->SSID[SSID_INDEX]->mac7,ret);
			break;			
		case GB_C_WIRELESS_MAC8:
			decode_string(web_api->home_wireless->SSID[SSID_INDEX]->mac8,ret);
			break;				
											
	}
	return 0;
}

int TcWebApiWireless_execute(int id)
{
	unsigned char i;
	int fd;
	
	Wireless_WriteFile();	
	if(atoi(web_api->home_wireless->access))
	{
		fd=open(WLAN_SHELL,O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
		for(i = 0; i < 22; i++)
		{
			if(strlen(commands[i]))
			{
				fprintf(stderr, "%s", commands[i]);
				write(fd, commands[i], strlen(commands[i]));
	//			system(commands[i]);
			}
		}
		close(fd);
		if(chmod(WLAN_SHELL, S_IRUSR|S_IWUSR|S_IXUSR|S_IXGRP|S_IXOTH) == 0){
			//system("/bin/echo execute_wlan_sh > /tmp/log");
			//system(WLAN_SHELL);
			//system(". /etc/Wireless/RT61AP/WLAN_exec.sh");
			asp_execute(WLAN_SHELL);
		}
		else{
			return -1;
		}
	}
	else if(strlen(commands[0])){
		system(commands[0]);
	}
	return 0;
}


#if 1

void Wireless_WriteFile()
{
	int fd, i,j;
	char *tmp,*tmp_mac;	
	char buf[32];
	
	tmp=(char *)ASP_MALLOC(256);
	if(tmp==NULL)
		goto err;
#if 0
	remove("/etc/wireless.conf");
	fd=open("/etc/wireless.conf",O_WRONLY|O_CREAT|O_APPEND,S_IREAD|S_IWRITE);	
#endif
	remove("/etc/Wireless/RT61AP/RT61AP.dat");
	fd=open("/etc/Wireless/RT61AP/RT61AP.dat",O_WRONLY|O_CREAT|O_APPEND,S_IREAD|S_IWRITE);	
	if(fd)
	{
		write(fd,"Default\n",strlen("Default\n"));
		sprintf(tmp,"APOn=%s\n",web_api->home_wireless->access);
		write(fd,tmp,strlen(tmp));		
		sprintf(tmp,"Country=%s\n",web_api->home_wireless->channel);
		write(fd,tmp,strlen(tmp));
		switch(StartChannelNumber)
		{
			case 1:
				switch(EndChannelNumber)
				{
					case 11:
						write(fd,"CountryRegion=0\n",strlen("CountryRegion=0\n"));
						strcpy(commands[1], "/userfs/bin/iwpriv ra0 set CountryRegion=0\n");
						break;
					case 13:
						write(fd,"CountryRegion=1\n",strlen("CountryRegion=1\n"));
						strcpy(commands[1], "/userfs/bin/iwpriv ra0 set CountryRegion=1\n");
						break;
					case 14:
						write(fd,"CountryRegion=5\n",strlen("CountryRegion=5\n"));
						strcpy(commands[1], "/userfs/bin/iwpriv ra0 set CountryRegion=5\n");
						break;
				}
				break;
			case 5:
				write(fd,"CountryRegion=6\n",strlen("CountryRegion=6\n"));
				strcpy(commands[1], "/userfs/bin/iwpriv ra0 set CountryRegion=6\n");
				break;
			case 10:
				switch(EndChannelNumber)
				{
					case 11:
						write(fd,"CountryRegion=2\n",strlen("CountryRegion=2\n"));
						strcpy(commands[1], "/userfs/bin/iwpriv ra0 set CountryRegion=2\n");
						break;
					case 13:
						write(fd,"CountryRegion=3\n",strlen("CountryRegion=3\n"));
						strcpy(commands[1], "/userfs/bin/iwpriv ra0 set CountryRegion=3\n");
						break;
				}
				break;
		}
//		if(!strcmp(web_api->home_wireless->channel_num, val_def->wireless_channel_auto))
//		{
//			write(fd,"AutoChannelSelect=1\n",strlen("AutoChannelSelect=1\n"));						
//			strcpy(commands[2], "/userfs/bin/iwpriv ra0 set AutoChannelSelect=1\n");
//		}
//		else
//		{
			write(fd,"AutoChannelSelect=0\n",strlen("AutoChannelSelect=0\n"));
//			strcpy(commands[2], "/userfs/bin/iwpriv ra0 set AutoChannelSelect=0\n");
//		}
			
		sprintf(tmp,"Channel=%s\n",web_api->home_wireless->current_channel);
		write(fd,tmp,strlen(tmp));
		sprintf(commands[3], "/userfs/bin/iwpriv ra0 set %s", tmp);
		sprintf(tmp,"BeaconPeriod=%s\n",web_api->home_wireless->beacon);
		write(fd,tmp,strlen(tmp));				
		sprintf(commands[4], "/userfs/bin/iwpriv ra0 set %s", tmp);
		sprintf(tmp,"RTSThreshold=%s\n",web_api->home_wireless->RTS_CTS);
		write(fd,tmp,strlen(tmp));		
		sprintf(commands[5], "/userfs/bin/iwpriv ra0 set %s", tmp);
		sprintf(tmp,"FragThreshold=%s\n",web_api->home_wireless->Fragmentation);
		write(fd,tmp,strlen(tmp));
		sprintf(commands[6], "/userfs/bin/iwpriv ra0 set %s", tmp);
		sprintf(tmp,"DtimPeriod=%s\n",web_api->home_wireless->DTIM);
		write(fd,tmp,strlen(tmp));
		sprintf(commands[7], "/userfs/bin/iwpriv ra0 set %s", tmp);
		if(!strcmp(web_api->home_wireless->bg80211, val_def->wireless_80211bg_80211bg))	
		{
			write(fd,"WirelessMode=0\n",strlen("WirelessMode=0\n"));		
			write(fd,"BasicRate=15\n",strlen("BasicRate=15\n"));
			strcpy(commands[8], "/userfs/bin/iwpriv ra0 set WirelessMode=0\n");
			strcpy(commands[9], "/userfs/bin/iwpriv ra0 set BasicRate=15\n");
		}
		else if(!strcmp(web_api->home_wireless->bg80211, val_def->wireless_80211bg_80211b))
		{
			write(fd,"WirelessMode=1\n",strlen("WirelessMode=1\n"));
			write(fd,"BasicRate=3\n",strlen("BasicRate=3\n"));
			strcpy(commands[8], "/userfs/bin/iwpriv ra0 set WirelessMode=1\n");
			strcpy(commands[9], "/userfs/bin/iwpriv ra0 set BasicRate=3\n");
		}
		else if(!strcmp(web_api->home_wireless->bg80211, val_def->wireless_80211bg_80211g))
		{
			write(fd,"WirelessMode=2\n",strlen("WirelessMode=2\n"));
			write(fd,"BasicRate=351\n",strlen("BasicRate=351\n"));
			strcpy(commands[8], "/userfs/bin/iwpriv ra0 set WirelessMode=2\n");
			strcpy(commands[9], "/userfs/bin/iwpriv ra0 set BasicRate=351\n");
		}

		write(fd,"BssidNum=1\n",strlen("BssidNum=1\n"));
//		for(i=0;i<SSID_COUNT;++i)
		for(i = 0; i < 1; ++i)
		{
			char tmp2[200], AccessMac = 0;
		
			sprintf(tmp,"SSID=%s\n", web_api->home_wireless->SSID[i]->SSID);
			write(fd,tmp,strlen(tmp));	
			sprintf(buf, "%s", web_api->home_wireless->SSID[i]->SSID);
			insertChar2string(buf, strlen(buf), 34); // " asscii number  = 34;
			if(strlen(buf) < 32)
			{
				if(buf[strlen(buf)-1] == 92)
					sprintf(commands[19], "/userfs/bin/iwpriv ra0 set SSID=\"%s\\\"\n", buf);
				else
					sprintf(commands[19], "/userfs/bin/iwpriv ra0 set SSID=\"%s\"\n", buf);
			}
			else
			{
				if(buf[strlen(buf)-2] == 92)
					sprintf(commands[19], "/userfs/bin/iwpriv ra0 set SSID=\"%s\\\"\n", buf);
				else
					sprintf(commands[19], "/userfs/bin/iwpriv ra0 set SSID=\"%s\"\n", buf);
			}
			if(!strcmp(web_api->home_wireless->SSID[i]->broadcast, val_def->wireless_broadcast_yes))
			{
				write(fd, "HideSSID=0\n",strlen("HideSSID=0\n"));		
				strcpy(commands[10], "/userfs/bin/iwpriv ra0 set HideSSID=0\n");
			}
			else if(!strcmp(web_api->home_wireless->SSID[i]->broadcast, val_def->wireless_broadcast_no))
			{
				write(fd, "HideSSID=1\n",strlen("HideSSID=1\n"));				
				strcpy(commands[10], "/userfs/bin/iwpriv ra0 set HideSSID=1\n");
			}
			if(!strcmp(web_api->home_wireless->SSID[i]->authentication, val_def->wireless_authentication_disabled))
			{
				unsigned char j;
				
				write(fd,"AuthMode=OPEN\n",strlen("AuthMode=OPEN\n"));
				write(fd,"EncrypType=NONE\n",strlen("EncrypType=NONE\n"));
				
				sprintf(tmp,"WPAPSK=%s\n", web_api->home_wireless->SSID[i]->pre_shared);
				write(fd,tmp,strlen(tmp));
				strcpy(commands[11], "/userfs/bin/iwpriv ra0 set AuthMode=OPEN\n");
				strcpy(commands[12], "/userfs/bin/iwpriv ra0 set EncrypType=NONE\n");
				for(j = 13; j < 19; j++)
					memset(commands[j], 0, 200);
			}
			else if(!strcmp(web_api->home_wireless->SSID[i]->authentication, val_def->wireless_authentication_wep64bits) )
			{
				write(fd,"AuthMode=WEPAUTO\n",strlen("AuthMode=WEPAUTO\n"));   
				strcpy(commands[11], "/userfs/bin/iwpriv ra0 set AuthMode=WEPAUTO\n");
				write(fd,"EncrypType=WEP\n",strlen("EncrypType=WEP\n"));
				strcpy(commands[12], "/userfs/bin/iwpriv ra0 set EncrypType=WEP\n");
				strcpy(LastWepKey[0], web_api->home_wireless->SSID[i]->key1_64);
				strcpy(LastWepKey[1], web_api->home_wireless->SSID[i]->key2_64);
				strcpy(LastWepKey[2], web_api->home_wireless->SSID[i]->key3_64);
				strcpy(LastWepKey[3], web_api->home_wireless->SSID[i]->key4_64);
				sprintf(tmp,"DefaultKeyID=%s\n", web_api->home_wireless->SSID[i]->keyIndex_64);
				write(fd,tmp,strlen(tmp));
				sprintf(commands[14], "/userfs/bin/iwpriv ra0 set %s", tmp);
				write(fd,"Key1Type=1\n",strlen("Key1Type=1\n"));
				sprintf(tmp,"Key1Str=%s\n", web_api->home_wireless->SSID[i]->key1_64);
				write(fd,tmp,strlen(tmp));
				sprintf(commands[15], "/userfs/bin/iwpriv ra0 set Key1=%s\n", web_api->home_wireless->SSID[i]->key1_64);
				write(fd,"Key2Type=1\n",strlen("Key2Type=1\n"));
				sprintf(tmp,"Key2Str=%s\n", web_api->home_wireless->SSID[i]->key2_64);
				write(fd,tmp,strlen(tmp));
				sprintf(commands[16], "/userfs/bin/iwpriv ra0 set Key2=%s\n", web_api->home_wireless->SSID[i]->key2_64);
				write(fd,"Key3Type=1\n",strlen("Key3Type=1\n"));
				sprintf(tmp,"Key3Str=%s\n", web_api->home_wireless->SSID[i]->key3_64);
				write(fd,tmp,strlen(tmp));
				sprintf(commands[17], "/userfs/bin/iwpriv ra0 set Key3=%s\n", web_api->home_wireless->SSID[i]->key3_64);
				write(fd,"Key4Type=1\n",strlen("Key4Type=1\n"));
				sprintf(tmp,"Key4Str=%s\n", web_api->home_wireless->SSID[i]->key4_64);
				write(fd,tmp,strlen(tmp));
				sprintf(commands[18], "/userfs/bin/iwpriv ra0 set Key4=%s\n", web_api->home_wireless->SSID[i]->key4_64);
				sprintf(tmp,"WPAPSK=%s\n", web_api->home_wireless->SSID[i]->pre_shared);
				write(fd,tmp,strlen(tmp));
				memset(commands[13], 0, 200);
			}
			else if(!strcmp(web_api->home_wireless->SSID[i]->authentication, val_def->wireless_authentication_wep128bits))
			{
				write(fd,"AuthMode=WEPAUTO\n",strlen("AuthMode=WEPAUTO\n"));
				strcpy(commands[11], "/userfs/bin/iwpriv ra0 set AuthMode=WEPAUTO\n");
				write(fd,"EncrypType=WEP\n",strlen("EncrypType=WEP\n"));
				strcpy(commands[12], "/userfs/bin/iwpriv ra0 set EncrypType=WEP\n");
				strcpy(LastWepKey[0], web_api->home_wireless->SSID[i]->key1_128);
				strcpy(LastWepKey[1], web_api->home_wireless->SSID[i]->key2_128);
				strcpy(LastWepKey[2], web_api->home_wireless->SSID[i]->key3_128);
				strcpy(LastWepKey[3], web_api->home_wireless->SSID[i]->key4_128);
				sprintf(tmp,"DefaultKeyID=%s\n", web_api->home_wireless->SSID[i]->keyIndex_128);
				write(fd,tmp,strlen(tmp));
				sprintf(commands[14], "/userfs/bin/iwpriv ra0 set %s", tmp);
				write(fd,"Key1Type=1\n",strlen("Key1Type=1\n"));
				sprintf(tmp,"Key1Str=%s\n", web_api->home_wireless->SSID[i]->key1_128);
				write(fd,tmp,strlen(tmp));
				sprintf(commands[15], "/userfs/bin/iwpriv ra0 set Key1=%s\n", web_api->home_wireless->SSID[i]->key1_128);
				write(fd,"Key2Type=1\n",strlen("Key2Type=1\n"));
				sprintf(tmp,"Key2Str=%s\n", web_api->home_wireless->SSID[i]->key2_128);
				write(fd,tmp,strlen(tmp));
				sprintf(commands[16], "/userfs/bin/iwpriv ra0 set Key2=%s\n", web_api->home_wireless->SSID[i]->key2_128);
				write(fd,"Key3Type=1\n",strlen("Key3Type=1\n"));
				sprintf(tmp,"Key3Str=%s\n", web_api->home_wireless->SSID[i]->key3_128);
				write(fd,tmp,strlen(tmp));
				sprintf(commands[17], "/userfs/bin/iwpriv ra0 set Key3=%s\n", web_api->home_wireless->SSID[i]->key3_128);
				write(fd,"Key4Type=1\n",strlen("Key4Type=1\n"));
				sprintf(tmp,"Key4Str=%s\n", web_api->home_wireless->SSID[i]->key4_128);
				write(fd,tmp,strlen(tmp));
				sprintf(commands[18], "/userfs/bin/iwpriv ra0 set Key4=%s\n", web_api->home_wireless->SSID[i]->key4_128);
				sprintf(tmp,"WPAPSK=%s\n", web_api->home_wireless->SSID[i]->pre_shared);
				write(fd,tmp,strlen(tmp));
				memset(commands[13], 0, 200);
			}
			else if(!strcmp(web_api->home_wireless->SSID[i]->authentication, val_def->wireless_authentication_wpapsk))
			{
				write(fd,"AuthMode=WPAPSK\n",strlen("AuthMode=WPAPSK\n"));
				strcpy(commands[11], "/userfs/bin/iwpriv ra0 set AuthMode=WPAPSK\n");
				sprintf(tmp,"EncrypType=%s\n", web_api->home_wireless->SSID[i]->encryption);
				write(fd,tmp,strlen(tmp));
				sprintf(commands[12], "/userfs/bin/iwpriv ra0 set %s", tmp);
				write(fd,"DefaultKeyID=2\n",strlen("DefaultKeyID=2\n"));
				strcpy(commands[14], "/userfs/bin/iwpriv ra0 set DefaultKeyID=2\n");
				write(fd,"Key1Type=1\n",strlen("Key1Type=1\n"));
				sprintf(tmp,"Key1Str=%s\n", LastWepKey[0]);
				write(fd,tmp,strlen(tmp));
				memset(commands[15], 0, 200);
				write(fd,"Key2Type=1\n",strlen("Key2Type=1\n"));
				sprintf(tmp,"Key2Str=%s\n", LastWepKey[1]);
				write(fd,tmp,strlen(tmp));
				memset(commands[16], 0, 200);
				write(fd,"Key3Type=1\n",strlen("Key3Type=1\n"));
				sprintf(tmp,"Key3Str=%s\n", LastWepKey[2]);
				write(fd,tmp,strlen(tmp));
				memset(commands[17], 0, 200);
				write(fd,"Key4Type=1\n",strlen("Key4Type=1\n"));
				sprintf(tmp,"Key4Str=%s\n", LastWepKey[3]);
				write(fd,tmp,strlen(tmp));
				memset(commands[18], 0, 200);
				sprintf(tmp,"WPAPSK=%s\n", web_api->home_wireless->SSID[i]->pre_shared);
				write(fd,tmp,strlen(tmp));
				sprintf(commands[13], "/userfs/bin/iwpriv ra0 set %s", tmp);
			}
			else if(!strcmp(web_api->home_wireless->SSID[i]->authentication, val_def->wireless_authentication_wpa2psk))
			{
				write(fd,"AuthMode=WPA2PSK\n",strlen("AuthMode=WPA2PSK\n"));
				strcpy(commands[11], "/userfs/bin/iwpriv ra0 set AuthMode=WPA2PSK\n");
				sprintf(tmp,"EncrypType=%s\n", web_api->home_wireless->SSID[i]->encryption);
				write(fd,tmp,strlen(tmp));
				sprintf(commands[12], "/userfs/bin/iwpriv ra0 set %s", tmp);
				write(fd,"DefaultKeyID=2\n",strlen("DefaultKeyID=2\n"));
				strcpy(commands[14], "/userfs/bin/iwpriv ra0 set DefaultKeyID=2\n");
				write(fd,"Key1Type=1\n",strlen("Key1Type=1\n"));
				sprintf(tmp,"Key1Str=%s\n", LastWepKey[0]);
				write(fd,tmp,strlen(tmp));
				memset(commands[15], 0, 200);
				write(fd,"Key2Type=1\n",strlen("Key2Type=1\n"));
				sprintf(tmp,"Key2Str=%s\n", LastWepKey[1]);
				write(fd,tmp,strlen(tmp));
				memset(commands[16], 0, 200);
				write(fd,"Key3Type=1\n",strlen("Key3Type=1\n"));
				sprintf(tmp,"Key3Str=%s\n", LastWepKey[2]);
				write(fd,tmp,strlen(tmp));
				memset(commands[17], 0, 200);
				write(fd,"Key4Type=1\n",strlen("Key4Type=1\n"));
				sprintf(tmp,"Key4Str=%s\n", LastWepKey[3]);
				write(fd,tmp,strlen(tmp));			
				memset(commands[18], 0, 200);
				sprintf(tmp,"WPAPSK=%s\n", web_api->home_wireless->SSID[i]->pre_shared);
				write(fd,tmp,strlen(tmp));
				sprintf(commands[13], "/userfs/bin/iwpriv ra0 set %s", tmp);
			}

			write(fd,"TxRate=0\n",strlen("TxRate=0\n"));
			write(fd,"TxPower=100\n",strlen("TxPower=100\n"));
			write(fd,"BGProtection=0\n",strlen("BGProtection=0\n"));
			write(fd,"TxPreamble=0\n",strlen("TxPreamble=0\n"));
			write(fd,"TxBurst=1\n",strlen("TxBurst=1\n"));
			write(fd,"PktAggregate=0\n",strlen("PktAggregate=0\n"));
			write(fd,"TurboRate=0\n",strlen("TurboRate=0\n"));
			write(fd,"NoForwarding=0\n",strlen("NoForwarding=0\n"));
			write(fd,"NoForwardingBTNBSSID=0\n",strlen("NoForwardingBTNBSSID=0\n"));
			write(fd,"ShortSlot=1\n",strlen("ShortSlot=1\n"));
			write(fd,"PreAuth=0\n",strlen("PreAuth=0\n"));
			write(fd,"RekeyInterval=0\n",strlen("RekeyInterval=0\n"));
			write(fd,"RekeyMethod=DISABLE\n",strlen("ReKeyMethod=DISABLE\n"));
			write(fd,"PMKCachePeriod=10\n",strlen("PMKCachePeriod=10\n"));

			switch(atoi(web_api->home_wireless->SSID[i]->active))
			{
				case 0:
					sprintf(tmp,"AccessPolicy%d=0\n", i);
					sprintf(tmp2,"AccessPolicy=0\n");
					write(fd,tmp,strlen(tmp));
					break;
				case 1:
					if(!strcmp(web_api->home_wireless->SSID[i]->action, val_def->wireless_action_allow))
					{
						sprintf(tmp,"AccessPolicy%d=1\n", i);
						sprintf(tmp2,"AccessPolicy=1\n");
						write(fd,tmp,strlen(tmp));
					}
					else if(!strcmp(web_api->home_wireless->SSID[i]->action, val_def->wireless_action_deny))
					{
						sprintf(tmp,"AccessPolicy%d=2\n", i);
						sprintf(tmp2,"AccessPolicy=2\n");
						write(fd,tmp,strlen(tmp));
					}
					break;
			}
			sprintf(commands[20], "/userfs/bin/iwpriv ra0 set %s", tmp2);
			sprintf(tmp,"AccessControlList%d=", i);
			sprintf(tmp2,"AccessControlList=\"");
			tmp_mac=web_api->home_wireless->SSID[i]->mac1;
			for(j = 0; j < 8; j++)
			{			
				fprintf(stderr, "tmp_mac = %s\n", tmp_mac);
				if(strlen(tmp_mac) && strcmp(tmp_mac, "00:00:00:00:00:00"))
				{
					if(j > 0 && AccessMac)
					{
						strcat(tmp, ";");
						strcat(tmp2, ";");
					}
					sprintf(tmp,"%s%s", tmp, tmp_mac);
					sprintf(tmp2,"%s%s", tmp2, tmp_mac);
					AccessMac = 1;
				}
				tmp_mac+=20;
			}
			AccessMac = 0;
			strcat(tmp, "\n");
			strcat(tmp2, "\"");
			write(fd,tmp,strlen(tmp));
			sprintf(commands[21], "/userfs/bin/iwpriv ra0 set %s", tmp2);
			
		}
#if 0	
		sprintf(tmp,"ACCESS=\"%s\"\n",web_api->home_wireless->access);
		write(fd,tmp,strlen(tmp));		
		sprintf(tmp,"CHANNEL=\"%s\"\n",web_api->home_wireless->channel);
		write(fd,tmp,strlen(tmp));				
		sprintf(tmp,"CHANNELNUM=\"%s\"\n",web_api->home_wireless->channel_num);
		write(fd,tmp,strlen(tmp));						
		sprintf(tmp,"CURRENTCHANNEL=\"%s\"\n",web_api->home_wireless->current_channel);
		write(fd,tmp,strlen(tmp));		
		sprintf(tmp,"BEACON=\"%s\"\n",web_api->home_wireless->beacon);
		write(fd,tmp,strlen(tmp));				
		sprintf(tmp,"RTS_CTS=\"%s\"\n",web_api->home_wireless->RTS_CTS);
		write(fd,tmp,strlen(tmp));						
		sprintf(tmp,"FRAGMENTATION=\"%s\"\n",web_api->home_wireless->Fragmentation);
		write(fd,tmp,strlen(tmp));		
		sprintf(tmp,"DTIM=\"%s\"\n",web_api->home_wireless->DTIM);
		write(fd,tmp,strlen(tmp));				
		sprintf(tmp,"80211BG=\"%s\"\n",web_api->home_wireless->bg80211);
		write(fd,tmp,strlen(tmp));		
		
		sprintf(tmp,"SSIDINDEX=\"%s\"\n",web_api->home_wireless->ssid_index);
		write(fd,tmp,strlen(tmp));		
		
		for(i=0;i<SSID_COUNT;++i)
		{
			sprintf(tmp,"SSID_%d=\"%s\"\n",i+1,web_api->home_wireless->SSID[i]->SSID);
			write(fd,tmp,strlen(tmp));	
			sprintf(tmp,"BROADCAST_%d=\"%s\"\n",i+1,web_api->home_wireless->SSID[i]->broadcast);
			write(fd,tmp,strlen(tmp));				
			sprintf(tmp,"AUTHENTICATION_%d=\"%s\"\n",i+1,web_api->home_wireless->SSID[i]->authentication);
			write(fd,tmp,strlen(tmp));					

			sprintf(tmp,"KEYINDEX_64_%d=\"%s\"\n",i+1,web_api->home_wireless->SSID[i]->keyIndex_64);
			write(fd,tmp,strlen(tmp));		
		
			tmp_64=web_api->home_wireless->SSID[i]->key1_64;
			tmp_128=web_api->home_wireless->SSID[i]->key1_128;				
			for(j=0;j<4;j++)
			{
				sprintf(tmp,"KEY%d_64_%d=\"%s\"\n",j+1,i+1,tmp_64);
				write(fd,tmp,strlen(tmp));	
				tmp_64+=8;
			}
			
			sprintf(tmp,"KEYINDEX_128_%d=\"%s\"\n",i+1,web_api->home_wireless->SSID[i]->keyIndex_128);
			write(fd,tmp,strlen(tmp));				
			for(j=0;j<4;j++)
			{			
				sprintf(tmp,"KEY%d_128_%d=\"%s\"\n",j+1,i+1,tmp_128);
				write(fd,tmp,strlen(tmp));	
				tmp_128+=32;						
			}							
					
			sprintf(tmp,"ENCRYPTION_%d=\"%s\"\n",i+1,web_api->home_wireless->SSID[i]->encryption);
			write(fd,tmp,strlen(tmp));				
			sprintf(tmp,"PRE-SHARED_%d=\"%s\"\n",i+1,web_api->home_wireless->SSID[i]->pre_shared);
			write(fd,tmp,strlen(tmp));		
		
						
			sprintf(tmp,"ACTIVE_%d=\"%s\"\n",i+1,web_api->home_wireless->SSID[i]->active);
			write(fd,tmp,strlen(tmp));		
			sprintf(tmp,"ACTION_%d=\"%s\"\n",i+1,web_api->home_wireless->SSID[i]->action);
			write(fd,tmp,strlen(tmp));	
			
			tmp_mac=web_api->home_wireless->SSID[i]->mac1;
			for(j=0;j<8;j++)
			{							
				sprintf(tmp,"MAC%d_%d=\"%s\"\n",j+1,i+1,tmp_mac);
				write(fd,tmp,strlen(tmp));				
				tmp_mac+=20;	
			}
			
			
			
		}
		
#endif			
		close(fd);	
	}
err:
	if(tmp)	
		ASP_FREE(tmp);	
}

void ChannelCountry()
{
	
	char *tmp;
	
	tmp=(char*)ASP_MALLOC(128);	
	if(tmp==NULL)
		goto err;
		
		
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_albania))//wkw 4.18
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_albania);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_albania);
		    	ASP_OUTPUT(tmp);		
		
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_algeria))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_algeria);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_algeria);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_argentina))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_argentina);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_argentina);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_armenia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_armenia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_armenia);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_australia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_australia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_australia);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_austria))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_austria);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_austria);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_azerbaijan))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_azerbaijan);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_azerbaijan);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_bahrain))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_bahrain);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_bahrain);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_belarus))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_belarus);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_belarus);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_belgium))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_belgium);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_belgium);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_belize))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_belize);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_belize);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_bolvia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_bolvia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_bolvia);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_brazil))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_brazil);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_brazil);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_brunei_darussalam))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_brunei_darussalam);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_brunei_darussalam);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_bulgaria))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_bulgaria);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_bulgaria);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_canada))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_canada);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_canada);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_chile))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_chile);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_chile);
		    	ASP_OUTPUT(tmp);
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_china))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_china);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_china);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_colombia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_colombia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_colombia);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_costa_rica))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_costa_rica);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_costa_rica);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_croatia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_croatia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_croatia);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_cyprus))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_cyprus);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_cyprus);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_czch_republic))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_czch_republic);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_czch_republic);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_denmark))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_denmark);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_denmark);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_dominicanrepublic))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_dominicanrepublic);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_dominicanrepublic);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_ecuador))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_ecuador);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_ecuador);
		    	ASP_OUTPUT(tmp);			    	
	    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_egypt))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_egypt);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_egypt);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_elsalvador))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_elsalvador);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_elsalvador);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_estonia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_estonia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_estonia);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_finland))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_finland);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_finland);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_france))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_france);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_france);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_georgia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_georgia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_georgia);
		    	ASP_OUTPUT(tmp);				  //32

		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_germany))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_germany);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_germany);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_greece))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_greece);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_greece);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_guatemala))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_guatemala);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_guatemala);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_honduras))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_honduras);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_honduras);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_hongkong))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_hongkong);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_hongkong);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_hungary))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_hungary);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_hungary);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_iceland))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_iceland);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_iceland);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_india))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_india);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_india);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_indonesia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_indonesia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_indonesia);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_iran))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_iran);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_iran);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_ireland))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_ireland);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_ireland);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_isarel))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_isarel);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_isarel);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_italy))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_italy);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_italy);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_japan))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_japan);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_japan);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_jordan))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_jordan);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_jordan);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_kazakhstan))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_kazakhstan);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_kazakhstan);
		    	ASP_OUTPUT(tmp);
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_northkorea))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_northkorea);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_northkorea);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_korearepublic))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_korearepublic);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_korearepublic);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_kuwait))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_kuwait);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_kuwait);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_latvia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_latvia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_latvia);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_lebanon))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_lebanon);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_lebanon);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_liechtenstein))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_liechtenstein);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_liechtenstein);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_lithuania))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_lithuania);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_lithuania);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_luxembourg))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_luxembourg);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_luxembourg);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_macau))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_macau);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_macau);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_macedonia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_macedonia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_macedonia);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_malaysia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_malaysia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_malaysia);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_mexico))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_mexico);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_mexico);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_monaco))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_monaco);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_monaco);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_morocco))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_morocco);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_morocco);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_netherlands))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_netherlands);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_netherlands);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_new_zealand))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_new_zealand);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_new_zealand);
		    	ASP_OUTPUT(tmp);		//64		  
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_norway))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_norway);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_norway);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_oman))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_oman);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_oman);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_pakistan))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_pakistan);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_pakistan);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_panama))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_panama);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_panama);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_peru))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_peru);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_peru);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_philippines))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_philippines);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_philippines);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_poland))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_poland);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_poland);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_portugal))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_portugal);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_portugal);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_puerto_rico))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_puerto_rico);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_puerto_rico);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_qatar))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_qatar);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_qatar);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_romania))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_romania);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_romania);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_russia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_russia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_russia);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_saudi_arabia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_saudi_arabia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_saudi_arabia);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_singapore))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_singapore);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_singapore);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_slovakia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_slovakia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_slovakia);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_slovenia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_slovenia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_slovenia);
		    	ASP_OUTPUT(tmp);
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_south_africa))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_south_africa);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_south_africa);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_spain))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_spain);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_spain);//17
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_swedan))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_swedan);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_swedan);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_switzerland))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_switzerland);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_switzerland);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_syriaarabrepublic))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_syriaarabrepublic);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_syriaarabrepublic);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_taiwan))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_taiwan);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_taiwan);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_thailand))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_thailand);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_thailand);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_trianidadandtobago))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_trianidadandtobago);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_trianidadandtobago);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_tunisia))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_tunisia);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_tunisia);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_turkey))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_turkey);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_turkey);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_ukraine))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_ukraine);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_ukraine);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_united_arab_emirates))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_united_arab_emirates);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_united_arab_emirates);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_united_kingdom))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_united_kingdom);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_united_kingdom);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_unitedstates))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_unitedstates);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_unitedstates);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_uruguay))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_uruguay);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_uruguay);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_uzbekistan))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_uzbekistan);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_uzbekistan);
		    	ASP_OUTPUT(tmp);			    	  	  	//32
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_venezuela))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_venezuela);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_venezuela);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_vietnam))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_vietnam);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_vietnam);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_yemen))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_yemen);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_yemen);
		    	ASP_OUTPUT(tmp);	
		    			    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_zimbabwe))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_zimbabwe);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_zimbabwe);
		    	ASP_OUTPUT(tmp);	
		    	
		    	if(!strcmp(web_api->home_wireless->channel,val_def->wireless_channel_undefined))
		    		sprintf(tmp,"<OPTION SELECTED> %s",val_def->wireless_channel_undefined);
		    	else
		    		sprintf(tmp,"<OPTION> %s",val_def->wireless_channel_undefined);
		    	ASP_OUTPUT(tmp);		
err:
	if(tmp)		    	
		ASP_FREE(tmp);		    	
}	
#endif

#define Byte_to_Hex(byte,hex)	sprintf((hex),"%x",(byte))
/* should have return value shnwind 2008.4.17*/
int Chr_to_Hex(char *src,char *target)
{
	char hex[3],*tmp_target;
	int src_long=0,i,target_num=0;

	if(src==NULL||target==NULL)
		return 0;

	target[target_num++]='0';
	target[target_num++]='x';

	tmp_target=target;
	src_long=strlen(src);
	if(src_long<=0)
		return 0;

	for(i=0;i<src_long;i++)
	{
		Byte_to_Hex(src[i],hex);
		strncpy(tmp_target+target_num,hex,2);
		target_num+=2;
	}
	target[target_num]=0;

  return 1;
}
/* should have return value shnwind 2008.4.17*/
int Hex_to_Chr(char *src,char *target)
{
	char *tmp_src;
	int i,src_long=0;

	char hex[3];

	if(src==NULL||target==NULL)
		return 0;

	hex[2]=0;

	if(*src!='0'||*(src+1)!='x')
		return 0;
	tmp_src=src+2;
	src_long=strlen(src);

	if(src_long<=0)
		return 0;
	
	for(i=0;i<src_long;i++)
	{		
		strncpy(hex,tmp_src,2);
		target[i]=(char)strtol(hex,NULL,16);
		tmp_src+=2;
	}
	target[i]=0;
	return 1;
}

/*
Ex: 1234"56 --> 1234\"56
If the filed of daemon's confiuguration  is inclued  double quote character 
then will be occur error.You need to add back slash character before it.
*/
void  insertChar2string(char *str, int len, int ascii)
{

 char tmp[32]={0};
 int i=0,j=0,k=0,numOfchar=0;
 int pos[10]={0};
 /*int ascii = 34;*//* " */  

 for(i=0,j=0; str[i]!=0; i++){
 	if(str[i] == ascii) /* " */
 	{ 
		numOfchar++;
		pos[j]=i;
		j++;
 	}
 }
 
 if(numOfchar == 0)
	return;
 
 for(i=0,j=0,k=0;i < len; i++ )
 {

   if(pos[j]==0 && j ==0)
   {
     tmp[k]=92;	// 92: '\'
     tmp[k+1]=str[i];
     k+=2;
     j++;
   }
   else
   {
      if(pos[j] != 0 && pos[j]==i)
      {
        tmp[k] = 92;
        tmp[k+1]=str[i];
        k+=2;
        j++;
      }
      else
      {
       tmp[k] = str[i];
       k++;
      }
   }
 }
 
 strncpy(str,tmp,strlen(tmp));
}
/*______________________________________________________________________________
**function name:check_wireless_card
**
**description: 
*     this function is use for checking the CPEhas wireless card or not.
**parameters: 
*     none 
**global:
*     none
**return: 
*     2:eeprom has error 
*     1:have card
*     0:does not have card
**call:
*     none
**revision:
*     1.shnwind 20080417
**____________________________________________________________________________*/
int
check_wireless_card()
{
  int fd,len;  
  char stream[64];

  system("/sbin/ifconfig ra0 > /tmp/wireless_stat.tmp");
  memset(stream,0,sizeof(stream)); 
  fd = open("/tmp/wireless_stat.tmp",O_RDONLY);
  if (fd == -1){
    return 0;
  }
  
  len=read(fd,stream,64);
  close(fd);
  remove("/tmp/wireless_stat.tmp");
  if((len == 0) || (len == -1)){
    return 0;
  }

  if((strstr(stream,"HWaddr FF:FF:FF:FF:FF:FF") != 0) ||
    (strstr(stream,"HWaddr 00:00:00:00:00:00") != 0)){
      fprintf(stderr,"There are some error when CPE getting data from EEPROM");
      return 2;
  }else{
    return 1;
  }
} 

