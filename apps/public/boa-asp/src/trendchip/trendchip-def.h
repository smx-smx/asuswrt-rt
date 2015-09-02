#ifndef  TRENDCHIP_DEF_H
#define TRENDCHIP_DEF_H

#include "../asp/http-get-utils.h"
#include "../asp/mini-asp.h"

asp_reent *my_reent;

char sprintf_buf[512];

#define ASP_FORM(name) 			(get_request_form(name)) 
#define ASP_QUERY(name)			(get_request_query(name))
#define ASP_MALLOC(bytes)   (asp_malloc_mem(bytes))
#define ASP_FREE(mem)				(asp_free_mem(mem))
#define ASP_OUTPUT(str)			(asp_send_response(NULL,str,strlen(str)))
#define ASP_ERROR(str)			(fprintf(stderr,"%s\n",str))
#define ASP_ISPOST				(g_var_post ? 1 : 0)	
#define ASP_EXECLP(arg)			(execlp("/bin/sh","/bin/sh","-c",arg,(char*)0))
#define ASP_OK					"0"
#define ASP_FAIL				"-1"

#define DHCP_LEASE_TIME	259200
#define DNS_IP	"168.95.1.1"

#define SEC_PER_DAY 86400 //60*60*24  for ppp link-up time
#define SEC_PER_HOUR 3600 //60*60         shnwind 2008.4.14
#define SEC_PER_MIN 60
 
void asp_shell2(char *file,char *arg);
void asp_execute2(char *file,char *arg);
void asp_shell(char *arg);
void asp_execute(char *arg);
char *asp_readall(char *arg);
int asp_atoi(char *nptr);
long asp_atol(char *nptr);
char *get_request_form(char *name);
char *get_request_query(char *name);
void *asp_malloc_mem(unsigned long bytes);
void asp_free_mem(void *mem);
char *asp_readline(int fd);
char *asp_trim(char *string);
int decode_uri(char *uri);

void ascii2punct(char *str, int Length);
//void ascii2punct(char *str);
void checkQuotationMarks(char * ptr, int Length);

#endif
