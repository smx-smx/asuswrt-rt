#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

#include "trendchip_load.h"
#include "trendchip-def.h"
#include "gb-constants.h"

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
static int cur_vc = 0;
static char ip[16],mask[16];
int wan_conn_status;

char control[4];/*shnwind add 2008.4.17*/
int tcWebApiStat_execute(int id);
extern int wan_pvc_connect(int vc);
/* remove this unused function shnwind 2008.04.17*/
#if 0
static int compare_ipaddr(char *ip,char *gw,char*mask)
{
	int ip_num[4],gw_num[4],mask_num[4],ret =0,i;
	sscanf(ip,"%d.%d.%d.%d",&ip_num[0],&ip_num[1],&ip_num[2],&ip_num[3]);
	sscanf(gw,"%d.%d.%d.%d",&gw_num[0],&gw_num[1],&gw_num[2],&gw_num[3]);
	sscanf(mask,"%d.%d.%d.%d",&mask_num[0],&mask_num[1],&mask_num[2],&mask_num[3]);

	for(i = 0;i<4;i++)
	{
		if((ip_num[i]&mask_num[i])!=(gw_num[i]&mask_num[i]) )
			return ret;
	}
	ret = 1;
	return ret;
}
#endif
char *get_default_gw(int pvc)
{
	int fd;
	char *line,*sub1;
	char IF[6], IF2[6];

	sprintf(IF, "nas%d", pvc);
	sprintf(IF2, "ppp%d", pvc);
	
	sprintf(sprintf_buf,"N/A");
	if(!strcmp(ip,"N/A") || !strcmp(mask,"N/A"))
		return sprintf_buf;
		
	asp_execute("/sbin/route -n > /tmp/route.log");
	fd = open("/tmp/route.log",O_RDONLY);
	if(fd == -1)
		return sprintf_buf;

	while((line = asp_readline(fd)))
	{
		if( (sub1 = strstr(line,"0.0.0.0"))  && sub1 == line && (strstr(line, IF) || strstr(line, IF2)))
		{
			sub1 = line+16;
			strncpy(sprintf_buf, sub1, 15);
			sprintf_buf[15] = '\0';
		}	
		free(line);
	}
	close(fd);
		
					
	
	return sprintf_buf;
}

char *TcWebApiDeviceinfo_get(int id)
{
	int now_time,start_time,connect_time;/*for caclated ppp link-up time*/
        int day,hour,min,sec;
        char t_tmp[32],value[32];
        int fd_t;
	read_eth_info();

	char test[32];
	sprintf(test, "ifconfig ppp%d > /etc/pppoe_%d", cur_vc, cur_vc);
	system(test);
	
	switch (id)
	{
		case GB_C_DEVICEINFO_FIRMWAREVER :
			ASP_OUTPUT(web_api->dev_info->device_firmwarever);
			break;
		case GB_C_DEVICEINFO_MACADDRESS:
			ASP_OUTPUT(web_api->lan[0]->mac_addr);
			break;
		case GB_C_DEVICEINFO_LANIP:
			ASP_OUTPUT(web_api->lan[0]->ip_addr);
			break;
		case GB_C_DEVICEINFO_LANNETMASK:
			ASP_OUTPUT(web_api->lan[0]->netmask);
			break;
		case GB_C_DEVICEINFO_DHCPSERVER:  
			if(!strcmp(web_api->lan[0]->dhcp_type,val_def->lan_dhcp_enable))
				ASP_OUTPUT("Enable");  
			else if(!strcmp(web_api->lan[0]->dhcp_type,val_def->lan_dhcp_relay))
				ASP_OUTPUT("Relay");  
			else
				ASP_OUTPUT("Disable");  
			break;
		case GB_C_DEVICEINFO_PVC:
			{
				int i;
				for(i = 0 ; i<8 ; i++)
				{
					if(i == cur_vc)
						sprintf(sprintf_buf,"<OPTION SELECTED>PVC%d",i);
					else
						sprintf(sprintf_buf,"<OPTION >PVC%d",i);
					ASP_OUTPUT(sprintf_buf);
				}
			}
			break;
			
		case GB_C_DEVICEINFO_WANSTATUS:
			wan_conn_status = wan_pvc_connect(cur_vc);
			if(!wan_conn_status && strstr(info.line_state, "up"))
			{
				//Marfada 07.11.20
				if(!strcmp(web_api->wan[cur_vc]->isp, val_def->wan_pppoe))
				{
					char _mySprintf[512];
					int fd;
					sprintf(_mySprintf, "/etc/pppoe_%d", cur_vc);
					fd = open(_mySprintf, O_RDONLY);
					char _stream[512], *sub1, *sub2;
					int size;
					if(fd == -1)
					{
						ASP_OUTPUT("N/A");
						break;
					}
					size = read(fd, _stream, sizeof(_stream));
					close(fd);
					fprintf(stderr, "size = %d", size);
					if(size <= 0)
					{
						ASP_OUTPUT("N/A");
						//close(fd);
						break;
					}
					sub1 = strstr(_stream, "inet addr:");
					sub2 = strstr(_stream, "inet addr:10.");
					if(!sub1)
					{
						ASP_OUTPUT("N/A");
						//close(fd);
						break;
					}
					if(sub2)
					{
						ASP_OUTPUT("Not Connected");
						//close(fd);
						break;
					}
					else
					{
						//close(fd);
                                           /* modify to currect the error when using on demand. when link is idle, status should be Not Connected.
				             I create a file to record the ppp start time and delete it when ppp terminate. 
                                             Using this file to know the ppp link status. shnwind 2008.0417                           */  
				           sprintf(sprintf_buf,"/tmp/pppuptime-ppp%d",cur_vc); 
                                           fd_t=open(sprintf_buf,O_RDONLY);             
                                           if(fd_t == -1){
                                             ASP_OUTPUT("Not Connected");
                                           }else{
						ASP_OUTPUT("Connected");
                                             close(fd_t);
		                           }
					}
				}
				
				else
				{
					char *stream,*sub1,*sub2;
					ASP_OUTPUT("Connected");
					stream = asp_readall("/tmp/br0.log");
					if(stream)
					{
						//ip
						sub1 = strstr(stream,"inet addr:");
						sub2 = strstr(stream,"Bcast:");
						if(!sub2)
							sub2 = strstr(stream,"P-t-P:");
						if(sub1!=NULL && sub2!=NULL)
						{
							strncpy(ip,sub1+10,sub2-sub1-12);
							ip[sub2-sub1-12]=0;
						}
						//mask
						sub1 = strstr(stream,"Mask:");
						if(sub1)
							sub2 = strchr(sub1,'\n');
						if(sub1 && sub2)
						{
							strncpy(mask,sub1+5,sub2-sub1-5);
							mask[sub2-sub1-5]=0;
						}
						free(stream);
					}
				}
			}
			else
			{
				ASP_OUTPUT("Not Connected");
				sprintf(ip,"N/A");
				sprintf(mask,"N/A");
			}
			break;
		case GB_C_DEVICEINFO_WANCONNECTIONTYPE:
			if(web_api->wan[cur_vc]->isp[0] == 0)
				ASP_OUTPUT("Not Defined");
			else if(!wan_conn_status)
			{
				if(!strcmp(web_api->wan[cur_vc]->isp,val_def->wan_dynamic_ip))
					ASP_OUTPUT("Dynamic IP Address");
				else if(!strcmp(web_api->wan[cur_vc]->isp,val_def->wan_static_ip))
					ASP_OUTPUT("Static IP Address");
				else if(!strcmp(web_api->wan[cur_vc]->isp,val_def->wan_pppoe))
					ASP_OUTPUT("PPPoA/PPPoE");
				else if(!strcmp(web_api->wan[cur_vc]->isp,val_def->wan_bridge))
					ASP_OUTPUT("Bridge Mode");
				else
					ASP_OUTPUT("Not Defined");
			}
			else
				ASP_OUTPUT("N/A");
			break;
		case GB_C_DEVICEINFO_WANIP:
			if(!strcmp(web_api->wan[cur_vc]->isp, val_def->wan_dynamic_ip))
				ASP_OUTPUT(ip);			    
			else if(!strcmp(web_api->wan[cur_vc]->isp, val_def->wan_static_ip))
				ASP_OUTPUT(ip);
			else if(!strcmp(web_api->wan[cur_vc]->isp, val_def->wan_pppoe))
			{
				//Marfada 07.11.20
				char _mySprintf[512];
				int fd;
				sprintf(_mySprintf, "/etc/pppoe_%d", cur_vc);
				fd = open(_mySprintf, O_RDONLY);
				char _stream[512], *sub0, *sub1, *sub2, _sprintf[16];
				int size;
				if(fd == -1)
				{
					ASP_OUTPUT("N/A");
					break;
				}
				size = read(fd, _stream, sizeof(_stream));
				close(fd);
				fprintf(stderr, "size = %d", size);
				if(size <= 0)
				{
					ASP_OUTPUT("N/A");
					//close(fd);
					break;
				}
				sub0 = strstr(_stream, "inet addr:");
				sub1 = strstr(_stream, "inet addr:10.");
				if(!sub0)
				{
					ASP_OUTPUT("N/A");
					//close(fd);
					break;
				}
				if(sub1)
				{
					ASP_OUTPUT("N/A");
					//close(fd);
					break;
				}
				else
				{
/* modify to currect the error when using on demand. when link is idle, it should display IP.
				             I create a file to record the ppp start time and delete it when ppp terminate. 
                                             Using this file to know the ppp link status. shnwind 2008.0417                           */  
				           sprintf(sprintf_buf,"/tmp/pppuptime-ppp%d",cur_vc); 
                                           fd_t=open(sprintf_buf,O_RDONLY);             
                                           if(fd_t == -1){
                                             ASP_OUTPUT("N/A");
					     break;
                                           }else{		
                                             close(fd_t);
		                           }
					sub2 = strstr(_stream, "inet addr:");
					strncpy(_sprintf, sub2+10, 16);
					int i;
					for(i = 0; i<16; i++)
					{
						if(!isdigit(_sprintf[i]) && _sprintf[i] != '.')
							_sprintf[i] = '\0';
					}
					ASP_OUTPUT(_sprintf);
					//close(fd);
				}
			}
			else
				ASP_OUTPUT("N/A");
			break;
		case GB_C_DEVICEINFO_WANNETMASK:
			if(!strcmp(web_api->wan[cur_vc]->isp,val_def->wan_dynamic_ip))
				ASP_OUTPUT(mask);
			else if(!strcmp(web_api->wan[cur_vc]->isp,val_def->wan_static_ip))
				ASP_OUTPUT(mask);
			else if(!strcmp(web_api->wan[cur_vc]->isp,val_def->wan_pppoe))
			{
				//Marfada 07.11.20
				char _mySprintf[512];
				int fd;
				sprintf(_mySprintf, "/etc/pppoe_%d", cur_vc);
				fd = open(_mySprintf, O_RDONLY);
				char _stream[512], *sub0, *sub1, *sub2, _sprintf[16];
				int size;
				if(fd == -1)
				{
					ASP_OUTPUT("N/A");
					break;
				}
				size = read(fd, _stream, sizeof(_stream));
				close(fd);
				fprintf(stderr, "size = %d", size);
				if(size <= 0)
				{
					ASP_OUTPUT("N/A");
					//close(fd);
					break;
				}
				sub0 = strstr(_stream, "inet addr:");
				sub1 = strstr(_stream, "inet addr:10");
				if(!sub0)
				{
					ASP_OUTPUT("N/A");
					//close(fd);
					break;
				}
				if(sub1)
				{
					ASP_OUTPUT("N/A");
					//close(fd);
					break;
				}
				else
				{
/* modify to currect the error when using on demand. when link is idle, it should display NETMASK.
				             I create a file to record the ppp start time and delete it when ppp terminate. 
                                             Using this file to know the ppp link status. shnwind 2008.0417                           */  
				           sprintf(sprintf_buf,"/tmp/pppuptime-ppp%d",cur_vc); 
                                           fd_t=open(sprintf_buf,O_RDONLY);             
                                           if(fd_t == -1){
                                             ASP_OUTPUT("N/A");
					     break;
                                           }else{		
                                             close(fd_t);
		                           }
					sub2 = strstr(_stream, "Mask:");
					strncpy(_sprintf, sub2+5, 16);
					int i;
                    for(i = 0; i<16; i++)
                  	{
						if(!isdigit(_sprintf[i]) && _sprintf[i] != '.')
							_sprintf[i] = '\0';
					}
					ASP_OUTPUT(_sprintf);
					//close(fd);
				}
			}										
			else
				ASP_OUTPUT("N/A");
			break;
		case GB_C_DEVICEINFO_WANDEFAULTGATEWAY:
			//Marfada 07.11.20
			if(strcmp(web_api->wan[cur_vc]->isp, val_def->wan_pppoe))
			{
				if(!strcmp(web_api->wan[cur_vc]->isp, val_def->wan_bridge))
					ASP_OUTPUT("N/A");
				else
				{
					get_default_gw(cur_vc);
	           		ASP_OUTPUT(sprintf_buf);
				}
				break;
			}
			else
			{
				char _mySprintf[512];
			    int fd;
			    sprintf(_mySprintf, "/etc/pppoe_%d", cur_vc);
			    fd = open(_mySprintf, O_RDONLY);
				char _stream[512], *sub0, *sub1, *sub2, _sprintf[16];
				int size;
				if(fd == -1)
				{
					ASP_OUTPUT("N/A");
					break;
				}
				size = read(fd, _stream, sizeof(_stream));
				close(fd);
				fprintf(stderr, "size = %d", size);
				if(size <= 0)
				{
					ASP_OUTPUT("N/A");
					//close(fd);
					break;
				}
				sub0 = strstr(_stream, "inet addr:");
				sub1 = strstr(_stream, "inet addr:10.");
				if(!sub0)
				{
					ASP_OUTPUT("N/A");
					//close(fd);
					break;
				}
				if(sub1)
				{
					ASP_OUTPUT("N/A");
					//close(fd);
					break;
				}
				else
				{
/* modify to currect the error when using on demand. when link is idle, it should display GATEWAY.
				             I create a file to record the ppp start time and delete it when ppp terminate. 
                                             Using this file to know the ppp link status. shnwind 2008.0417                           */  
				           sprintf(sprintf_buf,"/tmp/pppuptime-ppp%d",cur_vc); 
                                           fd_t=open(sprintf_buf,O_RDONLY);             
                                           if(fd_t == -1){
                                             ASP_OUTPUT("N/A");
					     break;
                                           }else{		
                                             close(fd_t);
		                           }
					sub2 = strstr(_stream, "P-t-P:");
					strncpy(_sprintf, sub2+6, 16);
					int i;
					for(i = 0; i<16; i++)
					{
						if(!isdigit(_sprintf[i]) && _sprintf[i] != '.')
							_sprintf[i] = '\0';
					}
					ASP_OUTPUT(_sprintf);
					//close(fd);
				}
			}
			break;
		case GB_C_DEVICEINFO_DNSSERVER:
			//Marfada 07.11.20
			if(!strcmp(web_api->wan[cur_vc]->isp, val_def->wan_bridge))
			{
				ASP_OUTPUT("N/A");
				break;
			}
			if(!wan_conn_status && strstr(info.line_state, "up"))
			{
				if(!strcmp(web_api->wan[cur_vc]->isp, val_def->wan_bridge))
				{
					ASP_OUTPUT("N/A");
					break;
				}
				if(!strcmp(web_api->wan[cur_vc]->isp, val_def->wan_pppoe))
				{
					char _mySprintf[512];
					int fd;
					sprintf(_mySprintf, "/etc/pppoe_%d", cur_vc);
					fd = open(_mySprintf, O_RDONLY);
					char _stream[512], *sub1, *sub2;
					int size;
					if(fd == -1)
					{
						ASP_OUTPUT("N/A");
						break;
					}
					size = read(fd, _stream, sizeof(_stream));
					close(fd);
					fprintf(stderr, "size = %d", size);
					if(size <= 0)
					{
						ASP_OUTPUT("N/A");
						//close(fd);
						break;
					}
					sub1 = strstr(_stream, "inet addr:");
					sub2 = strstr(_stream, "inet addr:10.");
					if(!sub1)
					{
						ASP_OUTPUT("N/A");
						//close(fd);
						break;
					}
					if(sub2)
					{
						ASP_OUTPUT("N/A");
						//close(fd);
						break;
					}
					else
					{
/* modify to currect the error when using on demand. when link is idle, it should display DNS.
				             I create a file to record the ppp start time and delete it when ppp terminate. 
                                             Using this file to know the ppp link status. shnwind 2008.0417                           */  
				           sprintf(sprintf_buf,"/tmp/pppuptime-ppp%d",cur_vc); 
                                           fd_t=open(sprintf_buf,O_RDONLY);             
                                           if(fd_t == -1){
                                             ASP_OUTPUT("N/A");
					     break;
                                           }else{		
                                             close(fd_t);
		                           }
						read_diag_dns(cur_vc);
                                                /*if no DNS server, display N/A. shnwind 2008.4.17*/
						if(!strcmp(web_api->main_diagnostics->dns_server,"0.0.0.0")||!strlen(web_api->main_diagnostics->dns_server)){
                                                  ASP_OUTPUT("N/A");
                                                }else{
						ASP_OUTPUT(web_api->main_diagnostics->dns_server);
					}
				}
				}
				else
				{
					read_diag_dns(cur_vc);
					if((!strcmp(web_api->main_diagnostics->dns_server,"0.0.0.0")) || (!strlen(web_api->main_diagnostics->dns_server))){
                                          ASP_OUTPUT("N/A");
                                        }else{
					ASP_OUTPUT(web_api->main_diagnostics->dns_server);
				}
			}
			}
			else
				ASP_OUTPUT("N/A");
			break;
		case GB_C_DEVICEINFO_RENEW_BUTTON:/*this will decide to open the release or relnew*/
                  memset(control,0,sizeof(control));
                  if(!strcmp(web_api->wan[cur_vc]->isp, val_def->wan_dynamic_ip) && strstr(info.line_state,"up")){
                    sprintf(control,"on");
                  }else{
                    sprintf(control,"off");
                  }
                  break;
		case GB_C_DEVICEINFO_SHOW_PPP_TIME:/*this decide to show ppp link-up time*/
                  memset(control,0,sizeof(control));
                  if(!strcmp(web_api->wan[cur_vc]->isp, val_def->wan_pppoe) && strstr(info.line_state,"up")){
                    sprintf(control,"on");
                  }else{
                    sprintf(control,"off");
                  }
                  break;
		case GB_C_DEVICEINFO_CONNECTION_TIME:	
                  now_time = time((time_t*)NULL);
                  
                  /*get link-start time*/
                  sprintf(t_tmp,"/tmp/pppuptime-ppp%d",cur_vc);
                  fd_t = open(t_tmp,O_RDONLY);
                  if(fd_t == -1){
			ASP_OUTPUT("N/A");
			break;
		  }
                  read(fd_t,value,32);
                  close(fd_t);

                  /*caculate link-up time*/
                  start_time=atoi(value);
                  connect_time=now_time-start_time;
                  day=connect_time/SEC_PER_DAY;
                  hour=(connect_time-SEC_PER_DAY*day)/SEC_PER_HOUR;
                  min=(connect_time-SEC_PER_DAY*day-SEC_PER_HOUR*hour)/SEC_PER_MIN;
                  sec=connect_time-SEC_PER_DAY*day-SEC_PER_HOUR*hour-SEC_PER_MIN*min;
                  sprintf(t_tmp,"%d:%2d:%2d:%2d",day,hour,min,sec);           
      
                  ASP_OUTPUT(t_tmp);
		  break; 
		case GB_C_ADSL_FIRMWAREVER:
			ASP_OUTPUT(info.adsl_firmwarever);
			break;
		case GB_C_ADSL_LINESTATE:
			ASP_OUTPUT(info.line_state);
			break;
		case GB_C_ADSL_MODULATION:
			ASP_OUTPUT(info.modulation);
			break;
		case GB_C_ADSL_ANNEXMODE:
			ASP_OUTPUT(info.annex);
			break;
		case GB_C_ADSL_SNRMARGIN_DOWN:
			ASP_OUTPUT(info.snr_down);
			break;
		case GB_C_ADSL_SNRMARGIN_UP:
			ASP_OUTPUT(info.snr_up);
			break;
		case GB_C_ADSL_LINEATTENUATION_DOWN:
			ASP_OUTPUT(info.line_down);
			break;
		case GB_C_ADSL_LINEATTENUATION_UP:
			ASP_OUTPUT(info.line_up);
			break;
		case GB_C_ADSL_DATERATE_DOWN:
			ASP_OUTPUT(info.data_down);
			break;
		case GB_C_ADSL_DATERATE_UP:
			ASP_OUTPUT(info.data_up);
			break;
	
			
	}
  if(strlen(control)){ /*fot return control*/
    return control;
  }else{
	return ASP_OK;
  }
}

int TcWebApiDeviceinfo_set(int id,char *value)
{
	char *ret,*line,*sub1;
	char string[100],tmp[16];
	int fd,flag=0,i;/*flag use to decide channel is interleave or fast*/ 
	FILE *pFile;
	
	cur_vc =0;
	ret = ASP_FORM(value);
	if(ret !=NULL)	
		sscanf(ret,"PVC%d",&cur_vc);
	
	memset(&info,0,sizeof(INFO));

	fd = open("/proc/tc3162/adsl_fwver",O_RDONLY);
	if(fd != -1)
	{
		line = asp_readline(fd);
		if(line)
		{
			strncpy(info.adsl_firmwarever,line,128);
#if 0
			sub1 = strchr(line,'_');
			sub2 = strchr(sub1+1,'_');
			if(sub1 && sub2)
				strncpy(info.annex,sub1+1,sub2-sub1-1);	
#endif			
			free(line);
		}
		close(fd);
	}

	if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_a))
		sprintf(info.annex, "%s", val_def->adsl_type_annex_a);
	else if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_i))
		sprintf(info.annex, "%s", val_def->adsl_type_annex_i);
	else if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_a_l))
		sprintf(info.annex, "%s", val_def->adsl_type_annex_a_l);
	else if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_m))
		sprintf(info.annex, "%s", val_def->adsl_type_annex_m);
	else if(!strcmp(web_api->adv_adsl->type,val_def->adsl_type_annex_all))
		sprintf(info.annex, "%s", val_def->adsl_type_annex_all);
	#if 0
	asp_execute("cat /proc/tc3162/adsl_stats > /tmp/adsl_stats");
	fd = open("/tmp/adsl_stats",O_RDONLY);
	#else
	pFile = fopen ("/proc/tc3162/adsl_stats" , "r");
	#endif
	strncpy(info.line_state,"N/A",16);
	strncpy(info.modulation,"N/A",32);
	strncpy(info.snr_down,"N/A",16);
	strncpy(info.snr_up,"N/A",16);
	strncpy(info.line_down,"N/A",16);
	strncpy(info.line_up,"N/A",16);
	strncpy(info.data_down,"N/A",16);
	strncpy(info.data_up,"N/A",16);
	if(pFile)
	{
		while(fgets(string, 100, pFile))
		{
			if((sub1 = strstr(string,"ADSL link status:")))
			{
				strncpy(info.line_state,string+strlen("ADSL link status:"),16);	
			}
			else if((sub1 = strstr(string,"Opmode:")))
			{
				strncpy(info.modulation,string+strlen("Opmode:"),32);	
			}
			else if((sub1 = strstr(string,"noise margin downstream:")))
			{
				strncpy(info.snr_down,string+strlen("noise margin downstream:"),16);	
			}
			else if((sub1 = strstr(string,"noise margin upstream:")))
			{
				strncpy(info.snr_up,string+strlen("noise margin upstream:"),16);	
			}
			else if((sub1 = strstr(string,"attenuation downstream:")))
			{
				strncpy(info.line_down,string+strlen("attenuation downstream:"),16);	
			}
			else if((sub1 = strstr(string,"attenuation upstream:")))
			{
				strncpy(info.line_up,string+strlen("attenuation upstream:"),16);	
			}
			else if((sub1 = strstr(string,"near-end interleaved channel bit rate:")))
			{
                          strcpy(tmp,string+strlen("near-end interleaved channel bit rate:"));
                          for(i = 0;i < 16;i++){/*check it is interleaved channel or fast channel adn set flag. shnwind 2008.4.17*/
                            if(tmp[i] == 'k'){
                              tmp[i] = '\0';
                              break;
                            }
                          }
                          if(atoi(tmp) > 0){
                            strcpy(info.data_down,string+strlen("near-end interleaved channel bit rate:"));
                            flag = 0;
                          }else{
                            flag = 1;
                          }
			}else if((sub1 = strstr(string,"near-end fast channel bit rate:")))
			{
                          if (flag){	
				  strcpy(info.data_down,string+strlen("near-end fast channel bit rate:"));
			}
			}else if((sub1 = strstr(string,"far-end interleaved channel bit rate:")))
			{
			  if(!flag){
			   strcpy(info.data_up,string+strlen("far-end interleaved channel bit rate:"));
                          }
			}else if((sub1 = strstr(string,"far-end fast channel bit rate:")))
			{
                           if (flag){	
				  strcpy(info.data_up,string+strlen("far-end fast channel bit rate:"));
                           }	
			}
			
		}
		fclose(pFile);
	}
	return 0;
}
/*______________________________________________________________________________
**	function name:tcWebApiStat_execute
**
**	description: 
*     this function is use for doing release ip or renew ip
**	parameters: 
*     id:use for ASP 
**	global:
*     none
**	return: 
*     0:success
*     -1:fail
**	call:
*     none
**	revision:
*     1.shnwind 20080417
**____________________________________________________________________________*/

int 
tcWebApiStat_execute(int id){
  
  char tmp[32];
  int fd,pid;

  sprintf(tmp, "/var/run/udhcpc-nas%d.pid", cur_vc);
  fd = open(tmp,O_RDONLY);
  if(fd == -1){
    fprintf(stderr,"no dhcp pid \n");          
    return -1;
  }
  memset(tmp,0,sizeof(tmp));
  if(read(fd,tmp,32) == -1){
    close(fd);
    return -1;
  }    
  pid=atoi(tmp);
  close(fd);
   
  switch(id)
  {
    case GB_C_DEVICEINFO_RENEW_IP:
      kill(pid,SIGUSR1);      
      break;
    case GB_C_DEVICEINFO_RELEASE_IP:
      kill(pid,SIGUSR2);
      break;
    default:break;  
  }
  return 0;
}/*end TcWebApiStat_execute*/

