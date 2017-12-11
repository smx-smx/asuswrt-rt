#ifndef _LIB_TCAPI_H
#define _LIB_TCAPI_H

int
tcapi_set(char* node, char* attr, char* value);

int
tcapi_unset(char* node);

int
tcapi_get(char* node, char* attr, char *retval);

int
tcapi_staticGet(char* node, char* attr, char *retval);

int
tcapi_show(char* node, char * retval);

int
tcapi_commit(char* node);

int
tcapi_save(void);
int
tcapi_read(char* node);

int
tcapi_readAll(void);

void
tcdbg_printf(char *fmt,...);

int
tcapi_reset(char* node);	//restore node to default

/*TR069 message mechanism----------add by brian*/
#define PROJID 2
#define MAX_READ_SIZE 256
#define MAX_SEND_SIZE 256
#define CWMP_MQ_FLAG_PATH "/tmp/cwmp/tr069agent_mq"
#define CWMP_MAX_MSG_LEN 256

/*TR069 message mechanism---------add by brian*/
typedef struct cwmp_msg
{
	char cwmptype;
	union
	{
		char reserved[CWMP_MAX_MSG_LEN - 1];
	}text;
}cwmp_msg_t;

typedef struct tc_msg
{
	long mtype;
	cwmp_msg_t msgtext;//message content
}tc_msg_t;

int 
sendmegq(long type,cwmp_msg_t *buf,int flag);

#endif
