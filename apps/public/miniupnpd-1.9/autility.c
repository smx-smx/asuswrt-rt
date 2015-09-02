#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define LOG_FILE_PATH	"/var/tmp/upnp.log"
#define MAX_DBG_MSG_SIZE	2048
#define MAX_LOG_SIZE	(128*1024)

void _write_to_upnp_log(const char *funname, int lineno, const char *fmt, ...)
{
    long maxbytes = MAX_LOG_SIZE;
    va_list ap;
    char lmsg[MAX_DBG_MSG_SIZE];
    struct stat stbuf;
    FILE *fp; 

    if(stat(LOG_FILE_PATH, &stbuf))
        stbuf.st_size = maxbytes;

    if(stbuf.st_size < maxbytes)
        fp=fopen(LOG_FILE_PATH,"a+"); 
    else
        fp=fopen(LOG_FILE_PATH,"w+"); 

    if(fp)
    {
		fprintf(fp, "%ld", time(0));
        if(funname) {
            fprintf(fp, "[%s:%d]", funname, lineno);
        }
        va_start(ap, fmt);
        vsnprintf(lmsg, MAX_DBG_MSG_SIZE - 1, fmt, ap);
        va_end(ap);
        fwrite(lmsg, strlen(lmsg), 1, fp);
        fflush(fp);
        fclose(fp);
    }
}

