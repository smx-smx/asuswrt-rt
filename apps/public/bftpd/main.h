#ifndef __BFTPD_MAIN_H
#define __BFTPD_MAIN_H


#define TRUE 1
#define FALSE 0

#include <sys/types.h>

struct bftpd_childpid {
	pid_t pid;
	int sock;
};

#ifdef TCSUPPORT_IPV6_FTP
#define STORAGE_FAMILY(X) ((X).ss_family)
#define STORAGE_PORT(X)  (*storage_port(&(X)))
#define STORAGE_PORT6(X) (*storage_port6(&(X)))
#define STORAGE_SIN_ADDR(X) (storage_sin_addr(&(X))->s_addr)
#define STORAGE_SIN_ADDR6(X) (storage_sin_addr6(&(X))->s6_addr)
#define STORAGE_SIN_ADDR6_NF(X) (*(storage_sin_addr6(&(X))))
#define STORAGE_LEN(X) (STORAGE_FAMILY(X) == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6))
#define SET_STORAGE_LEN(X, Y) (void) 0

#endif
extern int global_argc;
extern char **global_argv;
#ifdef TCSUPPORT_IPV6_FTP
extern struct sockaddr_storage name;
#else
extern struct sockaddr_in name;
#endif
extern FILE *passwdfile, *groupfile, *devnull;
extern char *remotehostname;
#ifdef TCSUPPORT_IPV6_FTP
extern struct sockaddr_storage remotename;
#else
extern struct sockaddr_in remotename;
#endif
extern int control_timeout, data_timeout;
extern int alarm_type;

/* Command line options */
char *configpath;
int daemonmode;

#ifdef TCSUPPORT_IPV6_FTP
static volatile int stop_server;
#endif
/* scripts to run before and after writing to the file system */
char *pre_write_script;
char *post_write_script;


#ifdef TCSUPPORT_IPV6_FTP
unsigned short *storage_port(const struct sockaddr_storage * const ss);
unsigned short int *storage_port6(const struct sockaddr_storage * const ss);
struct in_addr *storage_sin_addr(const struct sockaddr_storage * const ss);
struct in6_addr *storage_sin_addr6(const struct sockaddr_storage * const ss);
#endif
void print_file(int number, char *filename);

#endif
