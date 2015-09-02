#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>


void getip()
{
	int pid,status,child_pid;
	
	pid = vfork();
	if(pid == -1)
		printf("getipf\n");
	else if(pid == 0)
	{	
		close(0);
		close(1);
		close(2);
		execlp("/bin/sh","/bin/sh","-c","/sbin/ifconfig eth0 >/tmp/ip.tmp",(char*)0);
		printf("getipc\n");
		_exit(1);
	}
	else
	{
		child_pid = wait(&status);
	}
}

void ddns_run()
{
	int pid,status,child_pid;
	
	pid = vfork();
	if(pid == -1)
		printf("ddns_runf\n");
	else if(pid == 0)
	{	

		execlp("/bin/sh","/bin/sh","-c","/bin/ez-ipupdate -c  /etc/ipupdate.conf &",(char*)0);
		printf("ddns_runc\n");
		_exit(1);
	}
	else
	{
		child_pid = wait(&status);
	}
}

void main()
{
	char ip[15],tmp[16],*ipstr,ip2[15];
	int fd,i,f1,pid;
	int ret;
	char *content;
	struct stat buf;
	getip();
	
	fd = open("/tmp/ip.tmp",O_RDONLY);
	if (fd != -1)
    {
		ret = fstat(fd,&buf);
		if (ret == -1)
			goto err;
		content = (char*)malloc(buf.st_size+1);	
		ret = read(fd,content,buf.st_size);
		if (ret <= 0)
		{	
			free(content);
			goto err;
		}
		ipstr=strstr(content,"inet addr:")+10;

		strncpy(ip,ipstr,15);

		ddns_run();
		free(content);
		
	}
	close(fd);
		
		
	
	for(i=0;i<5;i++)
	{
		
		
		fd = open("/tmp/ip.tmp",O_RDONLY);
		if (fd != -1)
 	    {
			ret = fstat(fd,&buf);
			if (ret == -1)
				goto err;
			content = (char*)malloc(buf.st_size+1);	
			ret = read(fd,content,buf.st_size);
			if (ret <= 0)
			{	
				free(content);
				goto err;
			}
			ipstr=strstr(content,"inet addr:")+10;
			
			strncpy(ip2,ipstr,15);
			if(strncmp(ip2,ip,strlen(ip))==0)
				sleep(10);
			else
			{
				ddns_run();
				strncpy(ip,ip2,15);
			}
	
		
			free(content);
		}
		close(fd);
		
	}
	return ;
err:
	fprintf(stderr,"open /var/log/ez-ipupdate.pid error");
	return ;
}