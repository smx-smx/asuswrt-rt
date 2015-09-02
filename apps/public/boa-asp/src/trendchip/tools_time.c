
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#include "trendchip_load.h"
#include "trendchip-def.h"
#include "gb-constants.h"

void get_nowtime()
{
#if 0
	time_t tm;
	struct tm tm_time;
	char t_buff[201];

	time(&tm);
	memcpy(&tm_time, localtime(&tm), sizeof(tm_time));
	strftime(t_buff, 200, "%a %b %e %H:%M:%S %Z %Y", &tm_time);
	ASP_OUTPUT(t_buff);
	return ;
#else
	int fd;
	char *TZ;
	time_t tm;
	struct tm tm_time;
	char t_buff[201];
	static char InitGet = 0;

	if(!strcmp(web_api->main_timezone->Syn_with,val_def->time_syn_ntp))
	{
		fd = open("/etc/timezone.conf",O_RDONLY);
		fprintf(stderr, "fd = %d\n", fd);
		if(!InitGet && fd != -1)
		{
			InitGet = 1;
			TZ = asp_readline(fd);
			if (!setenv("TZ", TZ + 3, 1))
			{
				fprintf(stderr,"--------setenv TZ ok ! TZ = %s--------\n", TZ + 3);
			}
			asp_execute("/usr/script/ntpclient.sh");
			fprintf(stderr, "/usr/script/ntpclient.sh\n");
			free(TZ);
		}
		close(fd);
		asp_execute("/var/script/getnow.sh > /tmp/now.log");
		fd = open("/tmp/now.log",O_RDONLY);
		if(fd != -1)
		{
//			int size;
//			size = read(fd, TZ, 29);
			TZ = asp_readline(fd);
			if(!strcmp(web_api->main_timezone->daylight_status, val_def->time_daysave_enable))
			{
				char *Hour;
				char NewHour[3];
				char value;
				
//				NewHour[0] = TZ[11];
//				NewHour[1] = TZ[12];
//				NewHour[2] = '\n';
				Hour = strstr(TZ, ":") - 2;
				NewHour[0] = Hour[0];
				NewHour[1] = Hour[1];
				NewHour[2] = '\0';
				value = (atoi(NewHour) + 1)%24;
				if(value < 10)
					sprintf(NewHour, "0%d", value);
				else
					sprintf(NewHour, "%d", value);
				Hour[0] = NewHour[0];
				Hour[1] = NewHour[1];
//				TZ[11] = NewHour[0];
//				TZ[12] = NewHour[1];
//				TZ[28] = '\0';
			}
			ASP_OUTPUT(TZ);
			free(TZ);
		}
		close(fd);
	}
	else
	{
		time(&tm);
		memcpy(&tm_time, localtime(&tm), sizeof(tm_time));
		strftime(t_buff, 200, "%a %b %e %H:%M:%S %Z %Y", &tm_time);
		ASP_OUTPUT(t_buff);
	}
	return ;
#endif		
}

void decode_string (char * des,char * src)
{
	
	char *p = src;
	char *str = des;
	char tmp[3];
  int num	;
	if (src == NULL)
		return ;
	tmp[2]='\0';
	while(*p)
   {    
     if(*p=='%')
     {
      strncpy(tmp,p+1,2);
      num=strtol(tmp,NULL,16);
      *str++ = (char)num;
      p=p+3;
     }
     else
      *str++ = *p++;
   }
   *str = '\0';
   return ;
}

/*void get_nowtime_old()
{
	char *content = NULL;		
	struct stat buf;
	int fd;
	int ret;	
#if 0	
	fd = open("/etc/timezone.conf",O_RDONLY);
	if (fd != -1 )
	{
		setenv("ZT",web_api->main_timezone->time_zone_conf,1);
		close(fd);	
		fd = -1;
	}
#endif	
	asp_execute("date >/tmp/now.log");
	
	fd = open("/tmp/now.log",O_RDONLY);
	fprintf(stdout,"-------before get time -------");		
	if (fd != -1 )	
	{
		fprintf(stdout,"-------open ok-------");			
		ret = fstat(fd,&buf);
		if (ret == -1)
			goto err;
		content = (char*)malloc(buf.st_size+1);			
		if (content == NULL)
			goto err; 
		memset(content,0,buf.st_size+1);	
		ret = read(fd,content,buf.st_size);
		if (ret<=0)
			goto err;
		fprintf(stdout,"-------read ok-------");			
		fprintf(stdout,"-------time : %s-------",content);				
		ASP_OUTPUT(content);
	}
	else
	{
		fprintf(stdout,"-------get time err-------");	
		
	}
err:
	if (fd != -1)
	close(fd);
	if (content != NULL)
	free(content);
	return;		
}
*/

char *TcWebApiTimeZone_get(int id)
{
//	char *buf[128];
	int i = 0;
	char *buf = NULL;
	
	switch (id)
	{
		case GB_C_TIME_SYNC:
			if(strlen(web_api->main_timezone->Syn_with))
				return web_api->main_timezone->Syn_with;
			else
				return val_def->time_syn_ntp;
			break;			
		case GB_C_TIME_NOW:
			get_nowtime();
			break;	
		case GB_C_TIME_SYNCWITHNTP:
			if(!strcmp(web_api->main_timezone->Syn_with,val_def->time_syn_ntp) || !web_api->main_timezone->Syn_with[0])
				ASP_OUTPUT("CHECKED");		
			break;
		case GB_C_TIME_SYNCWITHPC:
			if(!strcmp(web_api->main_timezone->Syn_with,val_def->time_syn_pc))
				ASP_OUTPUT("CHECKED");	
			break;	
		case GB_C_TIME_SYNCWITHMANUALLY:
			if(!strcmp(web_api->main_timezone->Syn_with,val_def->time_syn_manu))
				ASP_OUTPUT("CHECKED");	
			break;	
		case GB_C_TIME_ZONE:
			buf = (char *)ASP_MALLOC(128);
			if (buf == NULL)
			{
				break;
			}	
			memset(buf,0,128);			
			for (;i<28;i++)
			{
				if(web_api->main_timezone->curr_timezone == i)
					sprintf(buf,"<OPTION SELECTED>%s",web_api->main_timezone->timezone[i]);
				else
					sprintf(buf,"<OPTION >%s",web_api->main_timezone->timezone[i]);
				ASP_OUTPUT(buf);				
			}
			if (buf != NULL)
			{
				ASP_FREE(buf);
				buf = NULL;
			}
			break;
		case GB_C_TIME_DAYLIGHTENABLE:
			if(!strcmp(web_api->main_timezone->daylight_status,val_def->time_daysave_enable))
				ASP_OUTPUT("CHECKED");			
			break;
		case GB_C_TIME_DAYLIGHTDISABLE:
			if(!strcmp(web_api->main_timezone->daylight_status,val_def->time_daysave_disable) || !web_api->main_timezone->daylight_status[0])
				ASP_OUTPUT("CHECKED");	
			break;
		case GB_C_TIME_NTPSERVERIP:
			if(strlen(web_api->main_timezone->ntp_server))
				ASP_OUTPUT(web_api->main_timezone->ntp_server);
			else
				ASP_OUTPUT("0.0.0.0");
			break;
	
	}
	
	return ASP_OK;
}

int TcWebApiTimeZone_set(int id,char *value)
{
	int i = 0;
	char *ret = NULL;
	char *start = NULL;
	char *end = NULL;
	char *temp =NULL;
//	char buf[128];	
	char *buf;
			
	ret = ASP_FORM(value);
	if (ret == NULL)
	{
//		strncpy(web_api->main_timezone->Syn_with,val_def->time_syn_ntp,16);
		return -1;
	}

	switch (id)
	{
		case GB_C_TIME_SYNC:		
			strncpy(web_api->main_timezone->Syn_with,ret,16);	
			break;	
		case GB_C_TIME_DAYLIGHT:
			strncpy(web_api->main_timezone->daylight_status,ret,16);
			break;	
		case GB_C_TIME_ZONE:				
			buf = (char *)ASP_MALLOC(128);
			if (buf == NULL)
			{
				break;			
			}
			memset(buf,0,128);
			decode_string(buf,ret);
			if (buf == NULL)
			{
				ASP_FREE(buf);
				buf = NULL;
				break;
			}
			start = strstr(buf,"(");
			end   = strstr(buf,")");
			if ((start == NULL) | (end == NULL))
			{
				ASP_FREE(buf);		
				buf = NULL;						
				break;
			}
			else
			{
				memset(web_api->main_timezone->time_zone_conf,0,sizeof(web_api->main_timezone->time_zone_conf));
				strncpy(web_api->main_timezone->time_zone_conf,start+1,end - start-1);	
				if (strlen(web_api->main_timezone->time_zone_conf) < 9)
				{
					web_api->main_timezone->curr_timezone = 13;					
				}
				else
				{
					for(;i<28;i++)
					{
							if (strstr(web_api->main_timezone->timezone[i],web_api->main_timezone->time_zone_conf )!= NULL)
							{
								web_api->main_timezone->curr_timezone = i;						
								break;	
							}				
					}
	
				}
			}
			ASP_FREE(buf);
			buf = NULL;					
			break;
		case GB_C_TIME_SYNCWITHPC:
			decode_string (web_api->main_timezone->time_pc,ret);
			temp = web_api->main_timezone->time_pc;
			if (temp != NULL)
			{
				start = strstr(temp,"/");
				memset(web_api->main_timezone->time_year,0,8);
				strncpy(web_api->main_timezone->time_year,temp,start-temp);
				temp = start + 1;
				start = strstr(temp,"/");
				memset(web_api->main_timezone->time_month,0,8);
				strncpy(web_api->main_timezone->time_month,temp,start-temp);
				temp = start + 1;
				start = strstr(temp," ");
				memset(web_api->main_timezone->time_date,0,8);				
				strncpy(web_api->main_timezone->time_date,temp,start-temp);
				temp = start + 1;
				start = strstr(temp,":");
				memset(web_api->main_timezone->time_hour,0,8);				
				strncpy(web_api->main_timezone->time_hour,temp,start-temp);				
				temp = start + 1;
				start = strstr(temp,":");
				memset(web_api->main_timezone->time_minute,0,8);				
				strncpy(web_api->main_timezone->time_minute,temp,start-temp);	
				temp = start + 1;	
				memset(web_api->main_timezone->time_second,0,8);																					
				strncpy(web_api->main_timezone->time_second,temp,2);					
			}

			break;			
		case GB_C_TIME_NTPSERVERIP:
			strncpy(web_api->main_timezone->ntp_server,ret,16);	
			break;	
		case GB_C_TIME_DATE:
			strncpy(web_api->main_timezone->time_date,ret,8);		
			break;
		case GB_C_TIME_MONTH:
			strncpy(web_api->main_timezone->time_month,ret,8);
			break;
		case GB_C_TIME_YEAR:
			strncpy(web_api->main_timezone->time_year,ret,8);
			break;
		case GB_C_TIME_HOUR:
			strncpy(web_api->main_timezone->time_hour,ret,8);
			break;
		case GB_C_TIME_MINUTE:
			strncpy(web_api->main_timezone->time_minute,ret,8);
			break;
		case GB_C_TIME_SECOND:		
			strncpy(web_api->main_timezone->time_second,ret,8);
			break;
	}
	
	return 0;
}


int TcWebApiTimeZone_execute()
{
//	int ret;		
	int fd ;
//	int pid;
//	char content[8];
	char arg[128];
	time_t tm;
	struct tm tm_time;
	char TZBuf[16];
#if 0	
	memset(content,0,8);
	fd = open("/var/log/ntpd.pid",O_RDONLY);
	if (fd != -1)
	{
		ret = read(fd,content,7);
		close(fd);	
		if (ret > 0)
		{
			pid = strtoul(content,NULL,10);
			kill(pid ,SIGABRT);	
		}
	}
			
	if (!strcmp(web_api->main_timezone->ntp_server,"0.0.0.0"))
	{
		remove("/etc/ntp.conf");
	}
	else
	{
		fd = open("/etc/ntp.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
		if (fd != -1)
		{
			memset(arg,0,128);						
			sprintf(arg,"server %s",web_api->main_timezone->ntp_server );
			write(fd,arg,strlen(arg));
			close(fd);
		}
	}
#endif	
	fd = open("/etc/timezone.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if (fd != -1)
	{
		strcpy(TZBuf, web_api->main_timezone->time_zone_conf);
		if(TZBuf[3] == '+')
			TZBuf[3] = '-';
		else if(TZBuf[3] == '-')
			TZBuf[3] = '+';
		memset(arg,0,128);
		sprintf(arg,"TZ=%s\nDAYLIGHT=%s\nSERVER=%s\n",TZBuf ,web_api->main_timezone->daylight_status, web_api->main_timezone->ntp_server);
		write(fd,arg,strlen(arg));
		close(fd);	
	}
	//lee 2006-9-21
//	if (!setenv("ZT",TZBuf,1))
//	{
//		fprintf(stderr,"--------setenv ZT ok !--------\n");
//	}
//	
//	fprintf(stderr,"--------synwith:%s,%s,------\n",web_api->main_timezone->Syn_with,val_def->time_syn_ntp);
	
	if (!strcmp(web_api->main_timezone->Syn_with,val_def->time_syn_ntp))
	{	
		char buf[128];
//		asp_shell("ntpd -d -g -c /etc/ntp.conf -p /var/log/ntpd.pid > /dev/null &");
		if(!strcmp(web_api->main_timezone->ntp_server, "0.0.0.0"))
			sprintf(buf, "/bin/echo \"/userfs/bin/ntpclient -s -c 1 -l -h ntp1.cs.wisc.edu\" > /etc/ntp.sh");
		else
			sprintf(buf, "/bin/echo \"/userfs/bin/ntpclient -s -c 1 -l -h %s\" > /etc/ntp.sh", web_api->main_timezone->ntp_server);
		fprintf(stderr, "buf = %s\n", buf);
		asp_execute(buf);
		asp_execute("/usr/script/ntpclient.sh");
		return 0;
	}		
/*	
	fd = open("/etc/timezone.conf",O_RDONLY);
	if (fd != -1 )
	{
		setenv("ZT",web_api->main_timezone->time_zone_conf,1);
		close(fd);	
	}	
*/	

/*
	memset(arg,0,128);
	sprintf(arg,"date \"%s %s %s %s %s\" > /dev/null",web_api->main_timezone->time_month,web_api->main_timezone->time_date,web_api->main_timezone->time_hour,web_api->main_timezone->time_minute,web_api->main_timezone->time_year);	
	asp_execute(arg);
*/	

	if ((strlen(web_api->main_timezone->time_date)>0) && (strlen(web_api->main_timezone->time_year)>=4))
	{
		fprintf(stderr,"--------set time ...--%s,%s,------\n",web_api->main_timezone->time_year,web_api->main_timezone->time_date);
		time(&tm);
		memcpy(&tm_time, localtime(&tm), sizeof(tm_time));
		/* Zero out fields - take her back to midnight! */
		tm_time.tm_sec = 0;
		tm_time.tm_min = 0;
		tm_time.tm_hour = 0;

		tm_time.tm_sec  = asp_atoi( web_api->main_timezone->time_second);
		tm_time.tm_mon  = asp_atoi( web_api->main_timezone->time_month);
		tm_time.tm_mday = asp_atoi( web_api->main_timezone->time_date);
		tm_time.tm_hour = asp_atoi( web_api->main_timezone->time_hour);
		tm_time.tm_min  = asp_atoi( web_api->main_timezone->time_minute);
		tm_time.tm_year = asp_atoi( web_api->main_timezone->time_year);	
	
		/* correct for century  - minor Y2K problem here? */
		if (tm_time.tm_year >= 1900)
			tm_time.tm_year -= 1900;
		/* adjust date */
		if (tm_time.tm_mon >= 1)	
			tm_time.tm_mon -= 1;	
		
		tm = mktime(&tm_time);	
		if (tm <0)
		{
			fprintf(stderr,"--------mktime err!--------\n");
		}
		else
		{
			if (stime(&tm) < 0)
			{
				fprintf(stderr,"--------stime err!--------\n");	
			}	
		}
		asp_execute("rm -f /etc/ntp.sh");
	}

	return 0;
	
}
#if 0
int TcWebApiTimeZone_write()
{
	int fd = -1;
	char arg[128];
	
	if (!strcmp(web_api->main_timezone->ntp_server,"0.0.0.0"))
	{
		remove("/etc/ntp.conf");
	}
	else
	{
		fd = open("/etc/ntp.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
		if (fd != -1)
		{
			memset(arg,0,128);						
			sprintf(arg,"server %s",web_api->main_timezone->ntp_server );
			write(fd,arg,strlen(arg));
			close(fd);
		}
	}	
	
	
	fd = open("/etc/timezone.conf",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
	if (fd != -1)
	{
		memset(arg,0,128);						
		sprintf(arg,"TZ=%s\nDAYLIGHT=%s",web_api->main_timezone->time_zone_conf ,web_api->main_timezone->daylight_status );
		write(fd,arg,strlen(arg));
		close(fd);	
	}	
	
	return 0;
}
#endif
