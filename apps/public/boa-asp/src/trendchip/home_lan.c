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

int get_default_dns(char *buf);
int write_dhcp_relay();
//int write_gate_conf();
int write_ripd_conf();
int write_dhcp_conf();


char *TcWebApiLan_get(int id)
{
	int index;
	char select[5][9]={"","","","",""};
	char buf[256],ip[20];
	
	if(web_api->lan_typechange ==0 || !ASP_ISPOST)
	{
		index = 0;
		if(!ASP_ISPOST)
			memset(web_api->lan[1],0,sizeof(LAN_SETTINGS));
	}
	else
		index = 1;
	
	switch(id)
	{
		case GB_C_LAN_IP:
			sprintf(buf,"\"%s\"",web_api->lan[index]->ip_addr);
			ASP_OUTPUT(buf);
			break;
		case GB_C_LAN_NETMASK:
			sprintf(buf,"\"%s\"",web_api->lan[index]->netmask);
			ASP_OUTPUT(buf);
			break;
		case GB_C_LAN_DYNAMICROUTE:
			if(!strcmp(web_api->lan[index]->rip_ver,val_def->wan_rip1))
				strcpy(select[1],"SELECTED");
			if(!strcmp(web_api->lan[index]->rip_ver,val_def->wan_rip2b))
				strcpy(select[2],"SELECTED");
			if(!strcmp(web_api->lan[index]->rip_ver,val_def->wan_rip2m))
				strcpy(select[3],"SELECTED");
			sprintf(buf,"<OPTION %s>%s<OPTION %s>%s<OPTION %s>%s",select[1],val_def->wan_rip1,select[2],val_def->wan_rip2b,select[3],val_def->wan_rip2m);
			ASP_OUTPUT(buf);
			break;
		case GB_C_LAN_DIRECTION:
			if(!strcmp(web_api->lan[index]->direction,val_def->wan_direction_both))
				strcpy(select[1],"SELECTED");
			if(!strcmp(web_api->lan[index]->direction,val_def->wan_direction_in))
				strcpy(select[2],"SELECTED");
			if(!strcmp(web_api->lan[index]->direction,val_def->wan_direction_out))
				strcpy(select[3],"SELECTED");
			else
				strcpy(select[0],"");
			sprintf(buf,"<OPTION %s>%s<OPTION %s>%s<OPTION %s>%s<OPTION %s>%s",select[0],val_def->wan_direction_none,select[1],val_def->wan_direction_both,select[2],val_def->wan_direction_in,select[3],val_def->wan_direction_out);
			ASP_OUTPUT(buf);
			break;
		case GB_C_LAN_MULTICAST:
			if(!strcmp(web_api->lan[index]->igmp,val_def->wan_igmpv1))
				strcpy(select[1],"SELECTED");
			if(!strcmp(web_api->lan[index]->igmp,val_def->wan_igmpv2))
				strcpy(select[2],"SELECTED");
			else
				strcpy(select[0],"");
			sprintf(buf,"<OPTION %s>%s<OPTION %s>%s<OPTION %s>%s",select[0],val_def->wan_igmpdisable,select[1],val_def->wan_igmpv1,select[2],val_def->wan_igmpv2);
			ASP_OUTPUT(buf);
			break;
		case GB_C_DHCP_STATUS:
			return web_api->lan[index]->dhcp_type;
		case GB_C_DHCP_DISABLE:
			if(!strcmp(web_api->lan[index]->dhcp_type,val_def->lan_dhcp_disable) || web_api->lan[index]->dhcp_type[0] == 0)
				ASP_OUTPUT("CHECKED");
			break;
		case GB_C_DHCP_ENABLE:
			if(!strcmp(web_api->lan[index]->dhcp_type,val_def->lan_dhcp_enable))
				ASP_OUTPUT("CHECKED");
			break;
		case GB_C_DHCP_RELAY:
			if(!strcmp(web_api->lan[index]->dhcp_type,val_def->lan_dhcp_relay))
				ASP_OUTPUT("CHECKED");
			break;	
		case GB_C_DHCP_STARTIP:
			sprintf(buf,"\"%s\"",web_api->lan[index]->dhcp_startip);
			ASP_OUTPUT(buf);
			break;
		case GB_C_DHCP_POOLSIZE:
			sprintf(buf,"\"%s\"",web_api->lan[index]->dhcp_poolsize);
			ASP_OUTPUT(buf);
			break;	
		case GB_C_DHCP_LEASETIME:
			/*change 0 -> index because it should display after change lan type,
			    change lan type example : dhcp disable -> enable. shnwind 2008.4.17*/
			if(atoi(web_api->lan[index]->dhcp_leasetime) == 0)
			{
				sprintf(buf,"\"%d\"",0);
			}
			else
			{
				sprintf(buf,"\"%s\"",web_api->lan[index]->dhcp_leasetime);
			}
			ASP_OUTPUT(buf);
			break;
#if 0
		case GB_C_DHCP_NETMASK:
			sprintf(buf,"\"%s\"",web_api->lan[index]->dhcp_netmask);
			ASP_OUTPUT(buf);
			break;	
		case GB_C_DHCP_ROUTER:
			sprintf(buf,"\"%s\"",web_api->lan[index]->dhcp_router);
			ASP_OUTPUT(buf);
			break;		
		case GB_C_DHCP_WINS:
			sprintf(buf,"\"%s\"",web_api->lan[index]->dhcp_wins);
			ASP_OUTPUT(buf);
			break;
#endif			
		case GB_C_DHCP_SERVERIP:
			sprintf(buf,"\"%s\"",web_api->lan[index]->dhcp_serverip);
			ASP_OUTPUT(buf);
			break;	
		case GB_C_DNS_DIRECTORY:
			if(!strcmp(web_api->lan[index]->dns_type,val_def->lan_dns_directory) || web_api->lan[index]->dns_type[0] == 0)
				ASP_OUTPUT("CHECKED");
			break;
		case GB_C_DNS_RELAY:
			if(!strcmp(web_api->lan[index]->dns_type,val_def->lan_dns_relay))
				ASP_OUTPUT("CHECKED");
			break;
		case GB_C_DNS_PRIMARY:
			//read_diag_dns(0);
			if((strlen(web_api->lan[index]->dns_primary) == 0) \
				|| atoi(web_api->lan[index]->dns_type) == 0)
			{
				if(get_default_dns(ip) == 0)
				{
					fprintf(stderr,"get_default_dns ok!!\r\n");
				}	
				else
				{
					strcpy(ip,DNS_IP); //Replace by default DNS IP;
				}	
				sprintf(buf,"\"%s\"",ip);
			}
			else
			{
			web_api->lan[index]->dns_primary[15]='\0';
			sprintf(buf,"\"%s\"",web_api->lan[index]->dns_primary);
			}
			ASP_OUTPUT(buf);
			break;
		case GB_C_DNS_SECOND:
			if(atoi(web_api->lan[index]->dns_type) == 0)
			{
				sprintf(buf,"\"%s\"","");
			}
			else
			{
			sprintf(buf,"\"%s\"",web_api->lan[index]->dns_second);
			}
			ASP_OUTPUT(buf);
			break;	
	}

	return ASP_OK;
}

int TcWebApiLan_set(int id,char *value)
{
	char *ret;
	int index;
	char ip[20];
	
	ret = ASP_FORM(value);
	if(ret ==NULL)
		return -1;
	if(web_api->lan_typechange ==1)
		index = 1;
	else
		index = 0;
	
	switch(id)
	{
		case GB_C_LAN_CHANGEDHCP:
			web_api->lan_typechange =1;
			break;
		case GB_C_LAN_IP:
			fprintf(stderr,"----LAN IP:%d,%s\n",index,web_api->lan[index]->ip_addr);
			strncpy(web_api->lan[index]->ip_addr,ret,16);
			strncpy(web_api->lan[index]->dhcp_router,ret,16);
			strncpy(web_api->lan[index]->dhcp_wins,ret,16);
			break;
		case GB_C_LAN_NETMASK:
			strncpy(web_api->lan[index]->netmask,ret,16);
			strncpy(web_api->lan[index]->dhcp_netmask,ret,16);
			break;
		case GB_C_LAN_DYNAMICROUTE:
			strncpy(web_api->lan[index]->rip_ver,ret,16);
			break;
		case GB_C_LAN_DIRECTION:
			strncpy(web_api->lan[index]->direction,ret,16);
			break;
		case GB_C_LAN_MULTICAST:
			strncpy(web_api->lan[index]->igmp,ret,16);
			break;	
		case GB_C_DHCP_STATUS:
			fprintf(stderr,"GB_C_DHCP_STATUS:%s\n",ret);
			strncpy(web_api->lan[index]->dhcp_type,ret,16);
			break;
		case GB_C_DHCP_STARTIP:
			fprintf(stderr,"----GB_C_DHCP_STARTIP %d\n",index);
			strncpy(web_api->lan[index]->dhcp_startip,ret,16);
			break;
		case GB_C_DHCP_POOLSIZE:
			strncpy(web_api->lan[index]->dhcp_poolsize,ret,16);
			break;
		case GB_C_DHCP_LEASETIME:
                  /* if lease time =0 set to 259200 and save value. shnwind 2008.4.17*/
                  if(!atoi(ret)){
                    sprintf(web_api->lan[index]->dhcp_leasetime,"259200");
			}
			else
			{	
				strncpy(web_api->lan[index]->dhcp_leasetime,ret,16);
			}
			break;
#if 0
		case GB_C_DHCP_NETMASK:
			strncpy(web_api->lan[index]->dhcp_netmask,ret,16);
			break;	
		case GB_C_DHCP_ROUTER:
			strncpy(web_api->lan[index]->dhcp_router,ret,16);
			break;	
		case GB_C_DHCP_WINS:
			strncpy(web_api->lan[index]->dhcp_wins,ret,16);
			break;	
#endif			
		case GB_C_DHCP_SERVERIP:
			strncpy(web_api->lan[index]->dhcp_serverip,ret,16);
			break;	
		case GB_C_DNS_TYPE:
			strncpy(web_api->lan[index]->dns_type,ret,16);
			break;
		case GB_C_DNS_PRIMARY:
			if(atoi(web_api->lan[index]->dns_type) == 0)
			{
				if(get_default_dns(ip) == 0)
				{
					fprintf(stderr,"get_default_dns ok!!\r\n");
				}	
				else
				{
					strcpy(ip,DNS_IP); //Replace by default DNS IP;
				}	
				strncpy(web_api->lan[index]->dns_primary,ip,16);
			}
			else
			{
			strncpy(web_api->lan[index]->dns_primary,ret,16);
			}
			web_api->lan[index]->dns_primary[15]='\0';
			break;
		case GB_C_DNS_SECOND:
			if(atoi(web_api->lan[index]->dns_type) == 0)
			{
				strcpy(web_api->lan[index]->dns_second,"");
			}
			else
			{
			strncpy(web_api->lan[index]->dns_second,ret,16);
			}	
			break;	
	}

	return 0;
}

int TcWebApiLan_execute()
{
	char *ret;
	int pid, zfd;
	char arg[128];

	//lee 9-28 :clean tmp buffer
	memset(web_api->lan[1],0,sizeof(LAN_SETTINGS));
	
	ret = asp_readall("/var/log/gated.pid");
	if(ret)
	{
		pid = atoi(ret);
		kill(pid,SIGTERM);
		free(ret);
	}
	
#if 0	
	if(!strcmp(web_api->lan[0]->rip_ver,val_def->wan_rip_disable))
		unlink("/etc/gated.conf");
	else
		write_gate_conf();
#endif	
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
	
	ret = asp_readall("/var/run/dnrd.pid");
	if(ret)
	{
		pid = atoi(ret);
		kill(pid,SIGTERM);
		free(ret);
	}

	ret = asp_readall("/var/run/udhcpd.pid");
	if(ret)
	{
		asp_execute("/bin/kill -9 `pidof udhcpd`");
		free(ret);
	}

	ret = asp_readall("/var/log/dhcrelay.pid");
	if(ret)
	{
		pid = atoi(ret);
		kill(pid,SIGTERM);
		free(ret);
	}
	asp_execute("/bin/kill -9 `pidof dproxy`");
	
	if(!strcmp(web_api->lan[0]->dhcp_type,val_def->lan_dhcp_disable))
	{
		unlink("/etc/udhcpd.conf");
		unlink("/etc/config/dhcprelay.sh");
	}
	else if(!strcmp(web_api->lan[0]->dhcp_type,val_def->lan_dhcp_enable))
	{
		int /*fd_temp, fd_conf,*/ dns_resolv;
		char buf[128];
		FILE *fd_temp, *fd_conf;
	
#if 0	//lee 12-25
		asp_execute("/bin/kill -9 `pidof dproxy`");
#endif	
		write_dhcp_conf();
		unlink("/etc/config/dhcprelay.sh");
		//11.30
		//asp_shell("/bin/udhcpd");
		
//		fd_temp = open("/etc/dproxy.conf.tmp",O_RDONLY);
//		fd_conf = open("/etc/dproxy.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
		fd_temp = fopen("/etc/dproxy.conf.tmp", "r");
		fd_conf = fopen("/etc/dproxy.conf", "w");
//		if(fd_temp == -1 || fd_conf == -1)
		if(!fd_temp || !fd_conf)
			fprintf(stderr, "dproxy.conf error\n");
		else
		{
//			char *line;
			char line[100];
			char *dns_line[2];
			FILE *dproxy_auto;
			
			//lee 12-25 init *dns_line[2]
			dns_line[0] = NULL;
			dns_line[1] = NULL;
			if(!strcmp(web_api->lan[0]->dns_type,val_def->lan_dns_directory))
			{ 
#if 0	//lee 12-25			
				char i;
#endif	
				dproxy_auto = fopen("/etc/dproxy.auto", "w");
				if(dproxy_auto)
					fputs("1", dproxy_auto);
				fclose(dproxy_auto);
				
				if(!strcmp(web_api->wan[0]->encap,val_def->wan_encap_pppoallc) ||
				   !strcmp(web_api->wan[0]->encap,val_def->wan_encap_pppoavc) ||
				   !strcmp(web_api->wan[0]->encap,val_def->wan_encap_pppoellc) ||
				   !strcmp(web_api->wan[0]->encap,val_def->wan_encap_pppoevc))
				{
					dns_resolv = open("/etc/ppp/resolv.conf",O_RDONLY);
					dns_line[0] = asp_readline(dns_resolv);
				}
				else
				{
					dns_resolv = open("/etc/resolv.conf",O_RDONLY);
					dns_line[0] = asp_readline(dns_resolv);
					dns_line[1] = asp_readline(dns_resolv);
				}
//				dns_line[0] = asp_readline(dns_resolv);
//				dns_line[1] = asp_readline(dns_resolv);
//				if(dns_line[0])
//					strcpy(web_api->lan[0]->dns_primary, (strstr(dns_line[0], " ") + 1));
//				if(dns_line[1])
//					strcpy(web_api->lan[0]->dns_second, (strstr(dns_line[1], " ") + 1));
				
#if 0	//lee 12-25			
				for(i = 0; i < 2; i++)
					fprintf(stderr, "dns_line[%d] = %s\n", i, dns_line[i]);
#endif	
			}
			else
			{
				dproxy_auto = fopen("/etc/dproxy.auto", "w");
				if(dproxy_auto)
					fputs("0", dproxy_auto);
				fclose(dproxy_auto);
			}
//			while(line = asp_readline(fd_temp))
			while(fgets(line, 100, fd_temp))
			{
				if(strstr(line, "%%PRIMARY_DNS%%"))
				{
					if(dns_line[0])
						sprintf(buf,"nameserver =  %s\n", (strstr(dns_line[0], " ") + 1));
					else
						sprintf(buf,"nameserver =  %s\n", web_api->lan[0]->dns_primary);
//					write(fd_conf, buf, strlen(buf));
//					write(fd_conf, "\n", 1);
					fputs(buf, fd_conf);
				}
				else if(strstr(line, "%%SECONDARY_DNS%%"))
				{
					if(dns_line[1])
						sprintf(buf,"nameserver =  %s\n", (strstr(dns_line[1], " ") + 1));
					else
						sprintf(buf,"nameserver =  %s\n", web_api->lan[0]->dns_second);
//					write(fd_conf, buf, strlen(buf));
//					write(fd_conf, "\n", 1);
					fputs(buf, fd_conf);
				}
				else
				{
//					write(fd_conf, line, strlen(line));
//					write(fd_conf, "\n", 1);
					fputs(line, fd_conf);
				}
//				free(line);
			}
#if 0	//lee 12-25		
			free(dns_line[0]);
			free(dns_line[1]);
#else
			if(dns_line[0])
			free(dns_line[0]);
			if(dns_line[1])
			free(dns_line[1]);		
#endif	
		}
//		close(fd_temp);
//		close(fd_conf);
		fclose(fd_temp);
		fclose(fd_conf);
#if 0 //lee 12-25		
		sprintf(arg, "/bin/dproxy -d -c /etc/dproxy.conf &");
//		asp_execute(arg);
		asp_execute("/bin/kill -9 `pidof dproxy`");
//		asp_execute(arg);
		asp_shell(arg);
		fprintf(stderr, "arg = %s\n", arg);
#else
		int fd;
		if((fd=open("/var/log/dproxy.pid",O_RDONLY)) != -1)
	  	{
	  	pid =0;	
	  	memset(arg,0,128);
	    if(read(fd,buf,8)<=0)
	    	fprintf(stderr, "error reading pid file: /var/log/dproxy.pid \n");
	    else
	    	pid = atoi(buf);	
	    
	    close(fd);
	    // check to see if the pid is valid
	    if(kill(pid, 0) == 0)
	    {
	      // if it is alive then we kill it
	      fprintf(stderr, "there is another program already running with pid %d.\n", pid);
	      kill (pid,SIGKILL);
	    }
	  } 
	  sprintf(arg, "/userfs/bin/dproxy -d -c /etc/dproxy.conf &");
	  pid = vfork();
		if(pid == -1)
			fprintf(stderr,"vfork\n");
		else if(pid == 0)
		{	
			close(0);
			close(1);
			close(2);
			execlp("/userfs/bin/dproxy","/userfs/bin/dproxy", "-d", "-c" ,"/etc/dproxy.conf",(char*)0);
			fprintf(stderr,"execlp\n");
			_exit(1);
		}	
		else
		{
			fd = open("/var/log/dproxy.pid",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);	
			if(fd!=-1)
			{
				sprintf(buf,"%d",pid);
				write(fd,buf,strlen(buf));
				close(fd);	
			}
		}
	  
#endif	//lee 12-25		
			
	}
	else if(!strcmp(web_api->lan[0]->dhcp_type,val_def->lan_dhcp_relay))
	{
		asp_execute("/bin/kill -9 `pidof dproxy`");
		unlink("/etc/dproxy.auto");
		unlink("/etc/dproxy.conf");
		write_dhcp_relay();
		unlink("/etc/udhcpd.conf");
		sprintf(arg,"/userfs/bin/dhcrelay -d -pf /var/log/dhcrelay.pid %s &",web_api->lan[0]->dhcp_serverip);
		system(arg);
		//asp_shell(arg);
	}

	/*set ip addr*/
	if(web_api->lan[0]->ip_addr[0] != 0)
	{
		int len=0,fd;
	
		len += sprintf(arg,"/sbin/ifconfig br0 %s",web_api->lan[0]->ip_addr);
		if(web_api->lan[0]->netmask[0] != 0)
			len += sprintf(arg+len," netmask %s",web_api->lan[0]->netmask);
		len += sprintf(arg+len," up ");
		fd = open("/etc/lanconfig.sh",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
		if(fd !=-1)
		{
//			write(fd,"\"",1);
			write(fd,arg,strlen(arg));
//			write(fd,"\"",1);
			close(fd);			
		}
		asp_shell(arg);
		//11.30
		//system("/bin/kill -9 `pidof udhcpd`");
	}
        /* because use busybox->udhcpd. shnwind 2008.4.17*/
	asp_shell("/usr/sbin/udhcpd");
	//asp_shell("/userfs/bin/udhcpd");
	return 0;
}

int write_dhcp_relay()
{
	int fd;
	fd = open("/etc/config/dhcprelay.sh",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd == -1)
		return -1;
	write(fd,"/userbin/bin/dhcrelay -pf /var/log/dhcrelay.pid ",strlen("/userbin/bin/dhcrelay -pf /var/log/dhcrelay.pid "));
	write(fd,web_api->lan[0]->dhcp_serverip,15);
	close(fd);
	return 0;
}

#if 0
int write_gate_conf()
{
	int fd_r,fd_w;
	char *line;
	char  *rip_val[3] = {"version 1","version 2 boardcast","version 2 multicast"};
	char  *dir_val[4] = {"noripin noripout","ripin ripout","ripin","ripout"};
	char  *igmp_val[3] = {"IGMP no","IGMP on","IGMP yes"};
	char  *rip,*dir,*igmp;
	
	
	if(!strcmp(web_api->lan[0]->rip_ver,val_def->wan_rip1))
		rip = rip_val[0];
	else if(!strcmp(web_api->lan[0]->rip_ver,val_def->wan_rip2b))
		rip = rip_val[1];
	else
		rip = rip_val[2];

	if(!strcmp(web_api->lan[0]->direction,val_def->wan_direction_both))
		dir = dir_val[1];
	else if(!strcmp(web_api->lan[0]->direction,val_def->wan_direction_in))
		dir = dir_val[2];
	else if(!strcmp(web_api->lan[0]->direction,val_def->wan_direction_out))
		dir = dir_val[3];
	else
		dir = dir_val[0];

	if(!strcmp(web_api->lan[0]->igmp,val_def->wan_igmpv1))
		igmp = igmp_val[1];
	else if(!strcmp(web_api->lan[0]->igmp,val_def->wan_igmpv2))
		igmp = igmp_val[2];
	else
		igmp = igmp_val[0];
			
	fprintf(stderr,"---- %s %d rip:%s dir:%s igmp:%s\n",__FUNCTION__,__LINE__,rip,dir,igmp);
	
	fd_r = open("/etc/gated.conf.tmp",O_RDONLY);
	if(fd_r== -1)
		return -1;
	
	fd_w = open("/etc/gated.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);		
	if(fd_w == -1)
		return -1;
		
	while((line= asp_readline(fd_r)))
	{
		fprintf(stderr,"---- %s %d read line:%s\n",__FUNCTION__,__LINE__,line);
		if(strstr(line,"%%VERSION%%"))
		{
			write(fd_w,rip,strlen(rip));
		}
		else if(strstr(line,"%%DIRECTION%%"))
		{
			write(fd_w,dir,strlen(dir));
		}
		else if(strstr(line,"%%IGMP%%"))
		{
			write(fd_w,igmp,strlen(igmp));
		}
		else
			write(fd_w,line,strlen(line));
		write(fd_w,"\n",1);
		free(line);
	}
	close(fd_r);
	close(fd_w);

	return 0;
			
}
#endif
int write_ripd_conf()
{
	int fd, i, IfCnt;
	char buf[128], Ifs[9][5];
	unsigned char deny;

	fd = open("/etc/lan_rip.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd ==-1)
		return -1;
	
	sprintf(buf,"RIPVERSION=\"%s\"\n",web_api->lan[0]->rip_ver);
	write(fd,buf,strlen(buf));
	sprintf(buf,"DIRECTION=\"%s\"\n",web_api->lan[0]->direction);
	write(fd,buf,strlen(buf));
	
	close(fd);

	fd = open("/etc/zebra.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd ==-1)
		return -1;
	
	sprintf(buf,"hostname TCZebra\n");
	write(fd,buf,strlen(buf));
	sprintf(buf,"password 1234\n");
	write(fd,buf,strlen(buf));
	sprintf(buf,"enable password 1234\n");
	write(fd,buf,strlen(buf));
	if(strcmp(web_api->lan[0]->direction,val_def->wan_direction_none))
	{
		sprintf(buf,"interface br0\n");
		write(fd,buf,strlen(buf));
		if(!strcmp(web_api->lan[0]->rip_ver, val_def->wan_rip2m))
			sprintf(buf,"multicast\n");
		else
			sprintf(buf,"no multicast\n");
		write(fd,buf,strlen(buf));
	}
	for(i = 0; i < 8; i++)
	{
		if(!strcmp(web_api->wan[i]->status,val_def->wan_deactivated))
			continue;
		if(!strcmp(web_api->wan[i]->direction,val_def->wan_direction_none))
			continue;
		switch(atoi(web_api->wan[i]->isp))
		{
			case 0:
			case 1:
				sprintf(buf,"interface nas%d\n", i);
				write(fd,buf,strlen(buf));
				break;
			case 2:
				sprintf(buf,"interface ppp%d\n", i);
				write(fd,buf,strlen(buf));
				break;
			case 3:
			default:
				fprintf(stderr, "isp%d is in bridge mode\r\n", i);
				continue;
		}
		if(!strcmp(web_api->wan[i]->rip_ver, val_def->wan_rip2m))
			sprintf(buf,"multicast\n");
		else
			sprintf(buf,"no multicast\n");
		write(fd,buf,strlen(buf));
	}
	
	close(fd);
	
	for(i = 0; i < 9; i++)
		memset(Ifs[i], 0, 5);

	fd = open("/etc/ripd.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd ==-1)
		return -1;
	
	sprintf(buf,"hostname TCRipd\n");
	write(fd,buf,strlen(buf));
	sprintf(buf,"password 1234\n");
	write(fd,buf,strlen(buf));
	if(strcmp(web_api->lan[0]->direction,val_def->wan_direction_none))
	{
		sprintf(buf,"interface br0\n");
		write(fd,buf,strlen(buf));
		sprintf(Ifs[0],"br0");
		if(!strcmp(web_api->lan[0]->rip_ver, val_def->wan_rip1))
		{
			sprintf(buf,"ip rip send version 1\n");
			write(fd,buf,strlen(buf));
			sprintf(buf,"ip rip receive version 1\n");
			write(fd,buf,strlen(buf));
		}
		else
		{
			sprintf(buf,"ip rip send version 2\n");
			write(fd,buf,strlen(buf));
			sprintf(buf,"ip rip receive version 2\n");
			write(fd,buf,strlen(buf));
		}
		sprintf(buf,"no ip rip authentication mode\n");
		write(fd,buf,strlen(buf));
	}

	for(i = 0; i < 8; i++)
	{
		if(!strcmp(web_api->wan[i]->status,val_def->wan_deactivated))
			continue;
		if(!strcmp(web_api->wan[i]->direction,val_def->wan_direction_none))
			continue;
		switch(atoi(web_api->wan[i]->isp))
		{
			case 0:
			case 1:
				sprintf(buf,"interface nas%d\n", i);
				write(fd,buf,strlen(buf));
				sprintf(Ifs[i+1],"nas%d", i);
				break;
			case 2:
				sprintf(buf,"interface ppp%d\n", i);
				write(fd,buf,strlen(buf));
				sprintf(Ifs[i+1],"ppp%d", i);
				break;
			case 3:
			default:
				fprintf(stderr, "isp%d is in bridge mode\r\n", i);
				continue;
		}
		if(!strcmp(web_api->wan[i]->rip_ver, val_def->wan_rip1))
		{
			sprintf(buf,"ip rip send version 1\n");
			write(fd,buf,strlen(buf));
			sprintf(buf,"ip rip receive version 1\n");
			write(fd,buf,strlen(buf));
		}
		else
		{
			sprintf(buf,"ip rip send version 2\n");
			write(fd,buf,strlen(buf));
			sprintf(buf,"ip rip receive version 2\n");
			write(fd,buf,strlen(buf));
		}
		sprintf(buf,"no ip rip authentication mode\n");
		write(fd,buf,strlen(buf));
	}

	sprintf(buf,"router rip\n");
	write(fd,buf,strlen(buf));
	sprintf(buf,"redistribute connected\n");
	write(fd,buf,strlen(buf));

	for(i = 0, IfCnt = 0; i < 9; i++)
	{
		char temp[15];

		if(!strlen(Ifs[i]))
		{
			IfCnt++;
			continue;
		}
		sprintf(temp, "network %s\n", Ifs[i]);
		write(fd, temp, strlen(temp));
	}
	if(IfCnt == 9)
	{
		close(fd);
		system("rm /etc/zebra.conf");
		system("rm /etc/ripd.conf");
		return 0;
	}

	deny = 0;
	if(!strcmp(web_api->lan[0]->direction,val_def->wan_direction_in))
	{
		sprintf(buf,"distribute-list DENY out br0\n");
		write(fd,buf,strlen(buf));
		deny = 1;
	}
	else if(!strcmp(web_api->lan[0]->direction,val_def->wan_direction_out))
	{
		sprintf(buf,"distribute-list DENY in br0\n");
		write(fd,buf,strlen(buf));
		deny = 1;
	}
	for(i = 0; i < 8; i++)
	{
		if(!strcmp(web_api->wan[i]->status,val_def->wan_deactivated))
			continue;
		if(!strcmp(web_api->wan[i]->direction,val_def->wan_direction_in))
		{
			sprintf(buf,"distribute-list DENY out %s\n", Ifs[i+1]);
			write(fd,buf,strlen(buf));
			deny = 1;
		}
		else if(!strcmp(web_api->wan[i]->direction,val_def->wan_direction_out))
		{
			sprintf(buf,"distribute-list DENY in %s\n", Ifs[i+1]);
			write(fd,buf,strlen(buf));
			deny = 1;
		}
	}
	if(deny)
	{
		sprintf(buf,"access-list DENY deny any\n");
		write(fd,buf,strlen(buf));
	}
	
	close(fd);
	
	return 0;
}

int write_dhcp_conf()
{
	int fd;
	char buf[128];
	char *delim=".";
	int i=0;
	char tmpIP[20];
	char endIP[4][4];
	
	fd = open("/etc/udhcpd.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if(fd ==-1)
		return -1;
	
	strcpy(tmpIP,web_api->lan[0]->dhcp_startip);
	sprintf(buf,"start %s \n",web_api->lan[0]->dhcp_startip);
	write(fd,buf,strlen(buf));
	for (i=0;i < 4 ; i++)
	{
       if(i==0)
        strcpy(endIP[i],strtok(tmpIP,delim));
	else
        strcpy(endIP[i],strtok(NULL,delim));
}
  /* modify because busybox->udhcpd don't need -1. shnwind 2008.4.17*/
  sprintf(endIP[3],"%d",atoi(endIP[3])+ atoi(web_api->lan[0]->dhcp_poolsize));
	sprintf(buf,"end %s.%s.%s.%s\n",endIP[0],endIP[1],endIP[2],endIP[3]);
	write(fd,buf,strlen(buf));
	strcpy(buf,"interface br0\nlease_file	/etc/udhcpd.leases \n");
	write(fd,buf,strlen(buf));
	sprintf(buf,"opt subnet %s\n",web_api->lan[0]->dhcp_netmask);
	write(fd,buf,strlen(buf));
	sprintf(buf,"opt router %s\n",web_api->lan[0]->dhcp_router);
	write(fd,buf,strlen(buf));
	sprintf(buf,"opt dns %s\n",web_api->lan[0]->ip_addr);
	write(fd,buf,strlen(buf));
	sprintf(buf,"opt wins %s\n",web_api->lan[0]->dhcp_wins);
	write(fd,buf,strlen(buf));
	if(atoi(web_api->lan[0]->dhcp_leasetime) == 0)
	{
		sprintf(buf,"option lease %d \n",DHCP_LEASE_TIME);
	write(fd,buf,strlen(buf));
	}
	else
	{
		sprintf(buf,"option lease %s \n",web_api->lan[0]->dhcp_leasetime);
	write(fd,buf,strlen(buf));
	}

	close(fd);
	return 0;
	
}

int TcWebApiLan_write()
{
	return 0;	
}

int get_default_dns(char *buf)
{
	  FILE *fd;
	  char line[50];
	  
	  fd = fopen("/etc/resolv.conf", "r");
  	if(fd == NULL)
  	{
    	fprintf(stderr,"------ load  /etc/resolv.conf  err\n");
    	return -1;
    }
    else
{
      fprintf(stderr,"------ load  /etc/resolv.conf  sucessfully\n");
      fgets(line, 50, fd);
      strcpy(buf,(line+11));
    }
    fclose(fd);
	return 0;	
}
