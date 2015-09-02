#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/syslog.h>
#include <signal.h>

#include "trendchip-def.h"
#include "gb-constants.h"

char *TcWebApiSyslog_get()
{
	FILE *fp;
	int fd,ret;
	char *msg;
	system("/bin/dmesg > /var/log/kmsg");
	sleep(1);
	sync();
	
	fp=fopen("/var/log/kmsg","a+");
	if(fp == NULL)
		return ASP_FAIL;
	fputc('\0',fp);
	fclose(fp);
	
	fd = open("/var/log/kmsg",O_RDONLY);
	if(fd ==-1)
		return ASP_OK;
	msg = (char*) malloc(8192);
	ret = read(fd,msg,8192);
	close(fd);
	if(ret)
		ASP_OUTPUT(msg);
	free(msg);

	if ((-1==unlink ("/var/log/kmsg")))
   	perror ("unlink of temporary kmsg file failed");
 	
 	return ASP_OK;
}
