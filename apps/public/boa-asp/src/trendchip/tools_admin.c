#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <pwd.h>
#include <sys/types.h>

#include "trendchip_load.h"
#include "trendchip-def.h"
#include "gb-constants.h"


#if 0
static void writepasswd(char *enpasswd)
{
	FILE *fp;
	int maxSize=64;
	char pwd[2][maxSize];
	int fd,ret,i;
	struct stat buf;
	char *strf,*start,*enter,*strpw,*strpw2;
	
	fd=open("/etc/passwd",O_RDONLY);
	if (fd ==-1)
		goto err;
	ret = fstat(fd,&buf);
	if (ret == -1)
		goto err;
	strf=(char*)ASP_MALLOC(buf.st_size+1);
	ret = read(fd,strf,buf.st_size);
	if(ret <=0){	
		ASP_FREE(strf);
		goto err;
  }
  close(fd);
  /*
  fprintf(stderr, "buf.st_size = %d\n", buf.st_size);
  fprintf(stderr, "ret = %d\n", ret);
  */
  start=strstr(strf,"admin:");
	enter=strchr(start,'\n')+1;
	if(enter!=NULL)
	{
	  memcpy(start,enter,strlen(enter));
	  i=strlen(start)-strlen(enter);
	  i=strlen(strf)-i;
  }
  else
    i=strlen(strf)-strlen(start);
  while((enter=strchr(start,'\n'))) 
  	start=enter+1; 
  
  strpw=(char *)ASP_MALLOC(256);
  strpw2=(char *)ASP_MALLOC(256);

  fprintf(stderr, "i = %d\n", i);
  fprintf(stderr, "strf = %s\n", strf);
  
  memcpy(strpw,strf,i);
  sprintf(strpw2,"%sadmin:%s:0:0:root:/:/bin/sh\n",strpw,enpasswd);  


  fd=open("/etc/passwd",O_WRONLY|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE);
  write(fd,strpw2,strlen(strpw2));
  
  close(fd);
  ASP_FREE(strpw);
  ASP_FREE(strpw2);
  ASP_FREE(strf);

 fp=fopen("/etc/passwd.conf","r");
 if(fp==NULL){
 	return;
 }
 else{
 	while(fgets(pwd[1],maxSize,fp)){
		if(strstr(pwd[1],"console_passwd")){
			break;
		}
	}
 }
 fclose(fp);
  /*To store plain text in passwd.conf file*/
  fp=fopen("/etc/passwd.conf","w");
  if(fp == NULL){
  	return;
  }else{
  	sprintf(pwd[0],"web_passwd=%s\n",web_api->admin_passwd);
  	fputs(pwd[0],fp);
	fputs(pwd[1],fp);
  }
    fclose(fp);
  return;
  err:
    if(fd!=-1)
      close(fd);

}
#endif
int TcWebApiTools_set(int id,char*value)
{
	char *passwd;
	
	switch (id)
	{
		case GB_C_ADMIN_PASSWORD:
		if((passwd=ASP_FORM(value)))
		{
			ascii2punct(passwd, 128);
			strncpy(web_api->admin_passwd,passwd,128);
		        fprintf(stderr, "passwd: %s\n", passwd);
		}
		else
		{
			fprintf(stderr,"TcWebApiTools_set get form ASP_FORM Fail!\r\n ");
			return -1;
		}
		
		break;			
		
		default :
			break;
	}
	
	return 0;
}


int TcWebApiAdmin_execute()
{
	struct passwd  *realpwd;
	char* cpwd;
	
	realpwd = getpwnam("admin");
	
	if(realpwd == NULL)
	{
		fprintf(stderr, "There is not 'admin' passport.\r\n");
		cpwd = crypt(web_api->admin_passwd, "$1$");
	}
	else
	{
		cpwd = crypt(web_api->admin_passwd, realpwd->pw_passwd);
	}

	fprintf(stderr, "before write,pwd:%s\n",cpwd);
#if 0
	writepasswd(cpwd);
#endif
	fprintf(stderr, "after write\n");
	return 0;
	
}
