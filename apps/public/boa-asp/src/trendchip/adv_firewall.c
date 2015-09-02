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

char *TcWebApiAdv_firewall_get(int id)
{	
//	fprintf(stderr,"--firewall get %s,%s,%s,%s,----",web_api->adv_firewall->firewall_status,web_api->adv_firewall->spi_status,val_def->fw_firewall_enable,val_def->fw_spi_disable);
	switch (id)
	{
		case GB_C_FW_FIREWALL_ENABLE:
		       if(strcmp(web_api->adv_firewall->firewall_status,val_def->fw_firewall_enable)==0)
		       {
		        	ASP_OUTPUT("CHECKED");
			}
		    	break;			
		case GB_C_FW_FIREWALL_DISABLE:
		       if(strcmp(web_api->adv_firewall->firewall_status,val_def->fw_firewall_disable)==0)
		       {
		        	ASP_OUTPUT("CHECKED");
		        }
			break;    
		case GB_C_FW_SPI_ENABLE:
		       if(strcmp(web_api->adv_firewall->spi_status,val_def->fw_spi_enable)==0)
		       {
		        	ASP_OUTPUT("CHECKED");
		        }
		    	break;			
		case GB_C_FW_SPI_DISABLE:
		       if(strcmp(web_api->adv_firewall->spi_status,val_def->fw_spi_disable)==0)
		       {
		        	ASP_OUTPUT("CHECKED");
		        }
			break;  					    		
		default: break;   			
  	}
	return ASP_OK;
}

int TcWebApiAdv_firewall_set(int id,char*value)
{
	char *ret;
	ret=ASP_FORM(value);
	if(ret==NULL)
		return -1;
	
	switch (id)
	{
		case GB_C_FW_FIREWALL:
			strncpy(web_api->adv_firewall->firewall_status,ret,16);	
			break;				
		case GB_C_FW_SPI:
			strncpy(web_api->adv_firewall->spi_status,ret,16);	
			break;
		default:
			break;  	        	
	}
	return 0;
}

int TcWebApiAdv_firewall_execute(id)
{
	int fd = -1;
	int i=0;
	char buf[64];
	char wanif[16]={0};
	char spi_stat[2];
	char fw_stat[2];
	char *p_str;
	int ret = 0;
	
	fd = open("/etc/firewall.conf",O_RDONLY);
	if (fd != -1)
	{
		ret = read(fd, buf, sizeof(buf));
		if(ret > 0)
		{
			p_str = strstr(buf, "firewall_status");
			strncpy(fw_stat, p_str+17,1);
			p_str=strstr(buf, "spi_status");
			strncpy(spi_stat, p_str+12,1);
		}	
		close(fd);
	}
	else
	{
		strcpy(fw_stat, "0");
		strcpy(spi_stat, "0");
	}
	
	fd=open("/etc/firewall.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);

	if (fd == -1)
		return -1;
	
	if (id == GB_C_FW_SAVE )
	{
		if((atoi(web_api->adv_firewall->firewall_status) == 1) && (atoi(fw_stat) == 0) )//enable firewall 
		{
			#if 0
			asp_shell("/bin/echo 1 > /proc/sys/net/ipv4/tcp_syncookies");
			asp_shell("/bin/echo 16 > /proc/sys/net/ipv4/tcp_max_syn_backlog");
			asp_shell("/bin/echo 3 > /proc/sys/net/ipv4/netfilter/ip_conntrack_tcp_timeout_syn_recv");
			#endif
			sprintf(buf, "/var/script/fw_start.sh");
			asp_execute(buf);
		}
		else if((atoi(web_api->adv_firewall->firewall_status) == 0) && (atoi(fw_stat) == 1)) //disable firewall
		{
			#if 0
			//asp_shell("/bin/echo 1 > /proc/sys/net/ipv4/tcp_syncookies");
			asp_shell("/bin/echo 128 > /proc/sys/net/ipv4/tcp_max_syn_backlog");
			asp_shell("/bin/echo 60 > /proc/sys/net/ipv4/netfilter/ip_conntrack_tcp_timeout_syn_recv");
			#endif
			sprintf(buf, "/var/script/fw_stop.sh");
			asp_execute(buf);
		}

		if((atoi(web_api->adv_firewall->spi_status) == 1) && (atoi(spi_stat) == 0)) //eanble SPI
		{	
			#if 0
			//create a new SPI_FW chain
			sprintf(buf,"/usr/bin/iptables -N SPI_FW");
			asp_shell(buf);
			sprintf(buf,"/usr/bin/iptables -I INPUT 1 -j SPI_FW");
			asp_shell(buf);
			sprintf(buf,"/usr/bin/iptables -I FORWARD 1 -j SPI_FW");
			asp_shell(buf);
			
			for(i=0; i < 8; i++)
			{
				if(!strcmp(web_api->wan[i]->status, val_def->wan_activated))
  				{
					if(!strcmp(web_api->wan[i]->isp, val_def->wan_pppoe))
						sprintf(wanif, "ppp%d", i);
					else
						sprintf(wanif, "nas%d", i);

					//add SPI firewall rule via iptables command
					sprintf(buf,"/usr/bin/iptables -A SPI_FW -i %s -m state --state NEW,INVALID -j DROP",wanif);
					asp_shell(buf);
					sprintf(buf,"/usr/bin/iptables -A SPI_FW -i %s -p icmp --icmp-type echo-request -j DROP",wanif);
					asp_shell(buf);
				}
			}
			#endif
			sprintf(buf, "/var/script/spi_fw_start.sh");
			asp_execute(buf);
		}
		else if((atoi(web_api->adv_firewall->spi_status) == 0)  &&  (atoi(spi_stat) == 1))
		{
			#if 0
			asp_shell("/usr/bin/iptables -D INPUT -j SPI_FW");
			asp_shell("/usr/bin/iptables -D FORWARD -j SPI_FW");
			asp_shell("/usr/bin/iptables -F SPI_FW");
			asp_shell("/usr/bin/iptables -X SPI_FW");
			#endif
			sprintf(buf, "/var/script/spi_fw_stop.sh");
			asp_execute(buf);
		}
		
		//write to firewall configuration
		sprintf(buf,"firewall_status=\"%s\"\n",web_api->adv_firewall->firewall_status);
		write(fd,buf,strlen(buf));
		
		sprintf(buf,"spi_status=\"%s\"\n",web_api->adv_firewall->spi_status);
		write(fd,buf,strlen(buf));				
	}
	close(fd);
	return 0;
}
