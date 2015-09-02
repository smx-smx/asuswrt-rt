#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#include "trendchip-def.h"

extern int asp_output;

void asp_shell2(char *file,char *arg)
{
	int pid;
	
	pid = vfork();
	if(pid == -1)
		fprintf(stderr,"asp_shell2:%s,%s\n",file,arg);
	else if(pid == 0)
	{	
	#if 1
		close(0);
		close(1);
		close(2);
	#endif	
		execlp(file,file,arg,(char*)0);
		fprintf(stderr,"asp_shell2:%s,%s\n",file,arg);
		_exit(1);
	}
	
}

void asp_execute2(char *file,char *arg)
{
	int pid,status,child_pid;
	
	pid = vfork();
	if(pid == -1)
		fprintf(stderr,"asp_execute2:%s,%s\n",file,arg);
	else if(pid == 0)
	{	
		execlp(file,file,arg,(char*)0);
		fprintf(stderr,"asp_execute2:%s,%s\n",file,arg);
		_exit(1);
	}
	else
	{
		child_pid = wait(&status);
	}

}

void asp_shell(char *arg)
{
	int pid;
	//fprintf(stderr,"asp_shell 1 %s \n",arg);
	pid = vfork();
	if(pid == -1)
		fprintf(stderr,"asp_shell:%s\n",arg);
	else if(pid == 0)
	{	
	#if 1
		close(0);
		close(1);
		close(2);
	#endif	
		ASP_EXECLP(arg);
		fprintf(stderr,"asp_shell:%s\n",arg);
		_exit(1);
	}
	fprintf(stderr,"asp_shell 2 \n");
	
}

void asp_execute(char *arg)
{
	int pid,status,child_pid;
	//fprintf(stderr,"asp_execute 1 %s\n",arg);
	pid = vfork();
	if(pid == -1)
		fprintf(stderr,"asp_execute:%s\n",arg);
	else if(pid == 0)
	{	
		ASP_EXECLP(arg);
		fprintf(stderr,"asp_execute:%s\n",arg);
		_exit(1);
	}
	else
	{
		child_pid = wait(&status);
	}
	fprintf(stderr,"asp_execute 2 \n");
}

char *asp_readall(char *arg)
{
	int fd;
	char *stream=NULL;
	struct stat buf;
	int ret;
	
	fd = open(arg,O_RDONLY);
	if ( fd!= -1)
	{
		ret = fstat(fd,&buf);
		if(ret == 0 && buf.st_size)
		{
			stream = (char*)malloc(buf.st_size);
			read(fd,stream,buf.st_size);
		}
		close(fd);
	}	
	return stream;
}

int asp_atoi(char *nptr)
{
	if(nptr)
		return atoi(nptr);
	else
		return 0;
}
long asp_atol(char *nptr)
{
	if(nptr)
		return atol(nptr);
	else
		return 0;
}
char *get_request_form(char *name)
{
	
	return get_param(g_var_post,name);
	
}

char *get_request_query(char *name)
{
	
	return get_param(g_var_post,name);
	
}

void *asp_malloc_mem(unsigned long bytes)
{
	if (bytes)
  {
      void* mem;
      mem = malloc (bytes);
      if (mem)
      {
      	memset(mem,0,bytes);
				return mem;
			}
  }
	fprintf(stderr,"failed to allocate %lu bytes",bytes);
  return NULL;
}

void asp_free_mem(void *mem)
{
	if(mem)
		free(mem);	
}

char *asp_readline(int fd)
{

	char *str_enter;
	char stream[128];
	char *str_return=NULL;
	int len,sub_len,offset=0,ret;
	off_t pos;
	struct stat st;

	ret = fstat(fd,&st);
	if(ret == -1)
		return NULL;

	pos = lseek(fd,0,SEEK_CUR);
	if(pos == -1L)
			return NULL;
	memset(stream,0,128);
	while((len=read(fd,stream,16))>0)
	{
		str_enter = strchr(stream,'\n');
		if(str_enter ==NULL)
		{
			offset +=len;
			continue;
		}
		else
		{
			sub_len = str_enter-stream;
			offset +=sub_len;
			break;
		}
				
	}

	if(!offset)
		return NULL;
	
	lseek(fd,pos,SEEK_SET);
#if 0	
	if(pos == -1L)
		return NULL;
#endif	
	str_return = (char *)malloc(offset+1);
	memset(str_return,0,offset+1);
	ret = read(fd,str_return,offset);
	
	if(ret<0)
	{
		free(str_return);
		return NULL;
	}
	else
		lseek(fd,pos+offset+1,SEEK_SET);
#if 0 	
	if((pos+offset+1) >= st.st_size )
	{
		free(str_return);
		return NULL;
	}
#endif	
	return str_return;
	
}

char *asp_trim(char *string)
{
	char *start;
	if(string == NULL)
		return NULL;
	for (start = string; *start &&  (((unsigned char)*start) == ' '); start++)
   			 ;
	memmove (string, start, strlen ((char *) start) + 1);

	for (start = string + strlen (string) - 1; start >= string && (*start == ' '); start--)
    		*start = '\0';
	return string;
}

static char hex_to_decimal(char char1,char char2)
{
	return (((char1 >= 'A') ? (((char1 & 0xdf) - 'A') + 10) : (char1 - '0')) * 16) + (((char2 >= 'A') ? (((char2 & 0xdf) - 'A') + 10) : (char2 - '0')));
}

int decode_uri(char *uri)
{
	char c, d;
    char *uri_old;

    uri_old = uri;
     while ((c = *uri_old)) {
        if (c == '%') {
            uri_old++;
            if ((c = *uri_old++) && (d = *uri_old++))
                *uri++ = hex_to_decimal(c, d);
            else
                return 0;       /* NULL in chars to be decoded */
        }else
        {
			*uri++ = c;
            uri_old++;
        }
     }
    *uri = '\0';
    return 1;
}

void ascii2punct(char *str, int Length)
{
 char* ptrBuf;
 char *p;
 char c_tmp[3];
 char *delim="%";
 int i=0;
 int numOfPunct = 0;
 
 for(i=0; str[i]!=0; i++){
	if(str[i] == 37) /*punctuation marks % */
		numOfPunct++;
 }
 
 if(numOfPunct == 0)
	return;
 
 ptrBuf = (char *)malloc( Length );
 
 if(str[0]==37)
 {
    sprintf(ptrBuf,"%s",strtok(str,delim));
    strncpy(c_tmp,ptrBuf,2);
    sprintf(ptrBuf,"%c%s",strtol(c_tmp,NULL,16),ptrBuf+2);
 }
 else
 {
   sprintf(ptrBuf,"%s",strtok(str,delim));
 }
 
 while((p= strtok(NULL, delim)))
 {
           strncpy(c_tmp,p,2);
           sprintf(ptrBuf,"%s%c%s",ptrBuf,strtol(c_tmp,NULL,16),p+2);
 }
 
 strcpy(str,ptrBuf);
 free(ptrBuf);
}

/*
void ascii2punct(char *str)
{
 char tmp[128]={0};
 char *p;
 char c_tmp[3];
 char *delim="%";
 int i=0;
 int numOfPunct = 0;
 
 for(i=0; str[i]!=0; i++){
	if(str[i] == 37) //punctuation marks %
		numOfPunct++;
 }
 if(numOfPunct == 0)
	return;

 
 if(str[0]==37)
 {
    sprintf(tmp,"%s",strtok(str,delim));
    strncpy(c_tmp,tmp,2);
    sprintf(tmp,"%c%s",strtol(c_tmp,NULL,16),tmp+2);
 }
 else
 {
   sprintf(tmp,"%s",strtok(str,delim));
 }
 
 while((p= strtok(NULL, delim)))
 {
           strncpy(c_tmp,p,2);
           sprintf(tmp,"%s%c%s",tmp,strtol(c_tmp,NULL,16),p+2);
 }
 
 strcpy(str,tmp);
}
*/

void checkQuotationMarks(char * ptr, int Length)
{
	int i=0, CharShift=0;
	char* ptrBuf;
	int numOfQuot = 0;

	/* cnt how many quotation marks */
	for(i=0; i<Length; i++){
		if(ptr[i] == 34)
			numOfQuot++;
	}
	if(numOfQuot == 0)
		return;
	
	/* 6 = strlen("&quot;") */
	ptrBuf = (char *)malloc( Length+(numOfQuot*6) );
	if(ptrBuf == NULL){
		fprintf(stderr,"\r\n checkQuotationMarks Memory Allocation Fail!");
		return;
	}
	for(i = 0; i < Length; i++)
	{
		if(ptr[i] == 34)
		{
			strcpy((ptrBuf + CharShift), "&quot;");
			CharShift = CharShift + 6;
		}
		else
		{
			*(ptrBuf+CharShift++) = *(ptr+i);
        }
    }
	*(ptrBuf+CharShift) = '\0';
	strcpy(ptr, ptrBuf);
	free(ptrBuf);
}