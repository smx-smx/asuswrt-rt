/*

bftpd Copyright (C) 1999-2003 Max-Wilhelm Bruker

This program is is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2 of the
License as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#include <config.h>
#include <stdio.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_ASM_SOCKET_H
#include <asm/socket.h>
#endif
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#ifdef HAVE_WAIT_H
# include <wait.h>
#else
# ifdef HAVE_SYS_WAIT_H
#  include <sys/wait.h>
# endif
#endif

#include "main.h"
#include "cwd.h"
#include "mystring.h"
#include "logging.h"
#include "dirlist.h"
#include "bftpdutmp.h"
#include "options.h"
#include "login.h"
#include "list.h"

int global_argc;
char **global_argv;
char **my_argv_list;   // jesse
#ifdef TCSUPPORT_IPV6_FTP
struct sockaddr_storage name;
#else
struct sockaddr_in name;
#endif
int isparent = 1;
#ifdef TCSUPPORT_IPV6_FTP
static const char *standalone_port;
static const char *standalone_ip;
char straddr[INET6_ADDRSTRLEN];
int v6ready =0;
int no_ipv4 = 0;
const struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;
#endif
#ifdef TCSUPPORT_IPV6_FTP
int listensocket = -1,listensocket6 = -1;
int tempsocket = -1;
int sock;
#else
int listensocket, sock;
#endif
FILE *passwdfile = NULL, *groupfile = NULL, *devnull;
#ifdef TCSUPPORT_IPV6_FTP
struct sockaddr_storage remotename;
#else
struct sockaddr_in remotename;
#endif
char *remotehostname;
#if defined(TCSUPPORT_ACCOUNT_ACL) || defined(TCSUPPORT_FW_UPGRADE_16M)
unsigned int mark;
int marklen;
#define 	IP_SKB_MARK 	50
#endif

int control_timeout, data_timeout;
int alarm_type = 0;

struct bftpd_list_element *child_list;

/* Command line parameters */
char *configpath = PATH_BFTPD_CONF;
int daemonmode = 0;

void print_file(int number, char *filename)
{
	FILE *phile;
	char foo[MAX_STRING_LENGTH];
	phile = fopen(filename, "r");
	if (phile) {
		while (fgets(foo, MAX_STRING_LENGTH, phile)) {
			foo[strlen(foo) - 1] = '\0';
			control_printf(SL_SUCCESS, "%i-%s", number, foo);
		}
		fclose(phile);
	}
}

void end_child()
{
	if (passwdfile)
		fclose(passwdfile);
	if (groupfile)
		fclose(groupfile);
	config_end();
	bftpd_log("Quitting.\n");
    bftpd_statuslog(1, 0, "quit");
    bftpdutmp_end();
	log_end();
	login_end();
	bftpd_cwd_end();
	if (daemonmode) {
		close(sock);
		close(0);
		close(1);
		close(2);
	}
}



/*
This function causes the program to 
re-read parts of the config file.

-- Jesse
*/
void handler_sighup(int sig)
{
   bftpd_log("Caught HUP signal. Re-reading config file.\n");
   Reread_Config_File();
   signal(sig, handler_sighup);
}




void handler_sigchld(int sig)
{
	pid_t pid;
	int i;
	struct bftpd_childpid *childpid; 

        /* Get the child's return code so that the zombie dies */
	pid = wait(NULL);          
	for (i = 0; i < bftpd_list_count(child_list); i++) {
		childpid = bftpd_list_get(child_list, i);
		if ( (childpid) && (childpid->pid == pid) ) {
			close(childpid->sock);
			bftpd_list_del(&child_list, i);
			free(childpid);
                        /* make sure the child is removed from the log */
                        bftpdutmp_remove_pid(pid);
		}
	}
}

void handler_sigterm(int signum)
{
#ifdef TCSUPPORT_IPV6_FTP  
 	 stop_server = 1;
    	if (listensocket != -1) {
        	shutdown(listensocket, 2);
        	(void) close(listensocket);
    	}
   	 if (listensocket6 != -1) {
        	shutdown(listensocket6, 2);
        	(void) close(listensocket6);
    	}
#endif
     bftpdutmp_end();
	 exit(0);	/* Force normal termination so that end_child() is called */
}

void handler_sigalrm(int signum)
{
    /* Log user out. -- Jesse <slicer69@hotmail.com> */
    bftpdutmp_end();

    if (alarm_type) {
        close(alarm_type);
        bftpd_log("Kicked from the server due to data connection timeout.\n");
        control_printf(SL_FAILURE, "421 Kicked from the server due to data connection timeout.");
        exit(0);
    } else {
        bftpd_log("Kicked from the server due to control connection timeout.\n");
        control_printf(SL_FAILURE, "421 Kicked from the server due to control connection timeout.");
        exit(0);
    }
}

void init_everything()
{
	if (!daemonmode) {
		config_init();
        hidegroups_init();
    }
	log_init();
    bftpdutmp_init();
	login_init();
}

#ifdef TCSUPPORT_IPV6_FTP
static void check_ipv6_support(void)     /* check for ipv6 support in kernel */
{
    int p;

    if ((p = socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP)) != -1) {
        (void) close(p);
        v6ready++;
    }
}
static void safe_fd_set(const int fd, fd_set * const fds)
{
    if (fd == -1) {
        return;
    }
    FD_SET(fd, fds);    
}

static int safe_fd_isset(const int fd, const fd_set * const fds)
{
    if (fd == -1) {
        return 0;
    }
    return FD_ISSET(fd, fds);
}

in_port_t *storage_port(const struct sockaddr_storage * const ss)
{
    struct sockaddr_in * const si = (struct sockaddr_in *) ss;
    
    return &si->sin_port;
}

in_port_t *storage_port6(const struct sockaddr_storage * const ss)
{
    struct sockaddr_in6 * const si = (struct sockaddr_in6 *) ss;
    
    return &si->sin6_port;
}

struct in_addr *storage_sin_addr(const struct sockaddr_storage * const ss)
{
    struct sockaddr_in * const si = (struct sockaddr_in *) ss;

    return &si->sin_addr;
}

struct in6_addr *storage_sin_addr6(const struct sockaddr_storage * const ss)
{
    struct sockaddr_in6 * const si = (struct sockaddr_in6 *) ss;

    return &si->sin6_addr;
}

static void set_cloexec_flag(const int fd)
{
    fcntl(fd, F_SETFD, FD_CLOEXEC);
}

/* Convert a 4-in-6 address to pure IPv4 */

static void fourinsix(struct sockaddr_storage *v6)
{
    struct sockaddr_storage v4;
    
    if (STORAGE_FAMILY(*v6) != AF_INET6 ||
        IN6_IS_ADDR_V4MAPPED(&STORAGE_SIN_ADDR6_NF(*v6)) == 0) {
        return;
    }
    memset(&v4, 0, sizeof v4);
    STORAGE_FAMILY(v4) = AF_INET;
    memcpy(&STORAGE_SIN_ADDR(v4),
           (unsigned char *) &STORAGE_SIN_ADDR6(*v6) + 12,
           sizeof STORAGE_SIN_ADDR(v4));
    STORAGE_PORT(v4) = STORAGE_PORT6(*v6);
    SET_STORAGE_LEN(v4, sizeof(struct sockaddr_in));
    *v6 = v4;
}

static int checkvalidaddr(const struct sockaddr_storage * const addr)
{
    if (addr == NULL) {
        return 0;
    }

    if (STORAGE_FAMILY(*addr) == AF_INET6) {
        if (IN6_IS_ADDR_MULTICAST(&STORAGE_SIN_ADDR6_NF(*addr)) ||
            IN6_IS_ADDR_UNSPECIFIED(&STORAGE_SIN_ADDR6_NF(*addr))) {
            return 0;
        }
        return 1;
    } else if (STORAGE_FAMILY(*addr) == AF_INET) {
        if (ntohl(STORAGE_SIN_ADDR(*addr)) == INADDR_ANY ||
            ntohl(STORAGE_SIN_ADDR(*addr)) == INADDR_NONE ||
            ntohl(STORAGE_SIN_ADDR(*addr)) == INADDR_BROADCAST ||
            IN_MULTICAST(ntohl(STORAGE_SIN_ADDR(*addr))) ||
            IN_BADCLASS(ntohl(STORAGE_SIN_ADDR(*addr)))) {
            return 0;
        }
        return 1;
    }
    return 0;
}

#endif

int main(int argc, char **argv)
{
	char str[MAX_STRING_LENGTH + 1];
#ifndef TCSUPPORT_IPV6_FTP
	static struct hostent *he;
	char *temp_string = NULL;
#endif

	int i = 1, port;
	int retval;
#ifdef TCSUPPORT_IPV6_FTP
       	char portstr[15] = {0};
	char ipstr[200] = {0};
	char strip[150] = {0};
#endif
 	unsigned long get_value;
   	socklen_t my_length;

        my_argv_list = argv;
        signal(SIGHUP, handler_sighup);

	while (((retval = getopt(argc, argv, "c:hdDin"))) > -1) {
		switch (retval) {
			case 'h':
				printf(
					#if TCSUPPORT_IPV6_FTP
					"Usage: %s [-h] [-i|-d|-D] [-c <filename>|-n][-6]\n"
					#else
					"Usage: %s [-h] [-i|-d|-D] [-c <filename>|-n]\n"
					#endif
					"-h print this help\n"
					"-i (default) run from inetd\n"
					"-d daemon mode: fork() and run in TCP listen mode\n"
					"-D run in TCP listen mode, but don't pre-fork()\n"
					"-c read the config file named \"filename\" instead of " PATH_BFTPD_CONF "\n"
					"-n no config file, use defaults\n", argv[0]);
				return 0;
			case 'i': daemonmode = 0; break;
			case 'd': daemonmode = 1; break;
			case 'D': daemonmode = 2; break;
			case 'c': configpath = strdup(optarg); break;
			case 'n': configpath = NULL; break;
			#if TCSUPPORT_IPV6_FTP
			case '6': no_ipv4 = 1; break;
			#endif
		}
	}
	
#if TCSUPPORT_IPV6_FTP
    check_ipv6_support();
	if (daemonmode) 
	{
	   	struct addrinfo hints, *res, *res6;
		struct sockaddr_storage sa;
		socklen_t dummy;
		fd_set rs;
    	int max_fd;
		
		memset(&hints, 0, sizeof hints);
   		hints.ai_flags = AI_PASSIVE;
   		hints.ai_family = AF_INET;
    	hints.ai_socktype = SOCK_STREAM;
    	hints.ai_addr = NULL;
	   	if (daemonmode == 1) {
			if (fork())
				exit(0);  /* Exit from parent process */
			setsid();
			if (fork())
				return 0;
		}
		signal(SIGCHLD, handler_sigchld);
		config_init();
		chdir("/");
        hidegroups_init();

     	if (!((port = strtoul(config_getoption("PORT"), NULL, 10))))
        	port = DEFAULT_PORT;
			
	 	sprintf(portstr,"%d",port);
		standalone_port = portstr;
	 	if (!strcasecmp(config_getoption("BIND_TO_ADDR"), "any")
			|| !config_getoption("BIND_TO_ADDR")[0])
				standalone_ip = NULL;
		else
		{
			strcpy(ipstr,config_getoption("BIND_TO_ADDR"));
			standalone_ip = ipstr;
		}
		
		i = 1;
		if ((listensocket == -1) && no_ipv4 == 0 && (getaddrinfo(standalone_ip, standalone_port, &hints, &res) == 0))
	    {
	        listensocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#ifdef SO_REUSEADDR
			setsockopt(listensocket, SOL_SOCKET, SO_REUSEADDR, (void *) &i,
					   sizeof(i));
#endif
#ifdef SO_REUSEPORT
			setsockopt(listensocket, SOL_SOCKET, SO_REUSEPORT, (void *) &i,
					   sizeof(i));
#endif
	      	if (bind(listensocket, res->ai_addr, (socklen_t) res->ai_addrlen)<0)
	        {
				fprintf(stderr, "Bind failed: %s\n", strerror(errno));
				exit(1);
	        }
		
	     	if (listen(listensocket, 256))
			{
				fprintf(stderr, "Listen failed: %s\n", strerror(errno));
				exit(1);
			}
			
	        freeaddrinfo(res);  
			set_cloexec_flag(listensocket);
		}

		hints.ai_family = AF_INET6;
			if ((listensocket6 == -1) && (v6ready != 0) && (getaddrinfo(standalone_ip, standalone_port, &hints, &res6) == 0))
		    {
		        listensocket6 = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
#ifdef SO_REUSEADDR
				setsockopt(listensocket6, SOL_SOCKET, SO_REUSEADDR, (void *) &i,
						   sizeof(i));
#endif
  				(void) setsockopt(listensocket6, IPPROTO_IPV6, IPV6_V6ONLY,
                              (char *) &i, sizeof(i));
#ifdef SO_REUSEPORT
				setsockopt(listensocket6, SOL_SOCKET, SO_REUSEPORT, (void *) &i,
						   sizeof(i));
#endif	    
				if (bind(listensocket6, res6->ai_addr, (socklen_t) res6->ai_addrlen)<0)
		        {
					fprintf(stderr, "Bind failed: %s\n", strerror(errno));
					exit(1);
		        }
					
		     	if (listen(listensocket6, 256))
				{
					fprintf(stderr, "Listen failed: %s\n", strerror(errno));
					exit(1);
				}
		        freeaddrinfo(res6);  
				set_cloexec_flag(listensocket6);
			}

		    FD_ZERO(&rs);
		    if (listensocket > listensocket6) 
		   	{
		        max_fd = listensocket;
		    } 
			else 
		    {
		        max_fd = listensocket6;
		    }
			
		    max_fd++;	
		    while (stop_server == 0) 
			{      
		        safe_fd_set(listensocket, &rs);
		        safe_fd_set(listensocket6, &rs);
		        if (select(max_fd, &rs, NULL, NULL, NULL) <= 0) 
				{
		           goto errorhandle;
		        }

		        if (safe_fd_isset(listensocket, &rs)) 
				{
		             tempsocket = listensocket;
		        } 
		        if (safe_fd_isset(listensocket6, &rs)) 
				{
		            tempsocket = listensocket6;
		        }
			
			    memset(&sa, 0, sizeof sa);
			    dummy = (socklen_t) sizeof sa;  
			    if ((sock = accept
			         (tempsocket, (struct sockaddr *) &sa, &dummy)) == -1)
			    {
			        goto errorhandle;
			    }
				
			    if (STORAGE_FAMILY(sa) != AF_INET && STORAGE_FAMILY(sa) != AF_INET6)
				{
			        (void) close(sock);
			        sock = -1;
			        goto errorhandle;
			    } 
				
			   	if (sock > 0)
				{
					pid_t pid;
					pid = fork();
					if (!pid) {       /* child */
						close(0);
						close(1);
						close(2);
						isparent = 0;
						dup2(sock, fileno(stdin));
						dup2(sock, fileno(stderr));
						break;
					} else {          /* parent */
						struct bftpd_childpid *tmp_pid = malloc(sizeof(struct bftpd_childpid));
						tmp_pid->pid = pid;
						tmp_pid->sock = sock;
						bftpd_list_add(&child_list, tmp_pid);
					}
				}

				errorhandle:
					if (errno != EINTR)
					{
		                (void) sleep(1);
		            }
   		 	}
    }
#else
	if (daemonmode) {
		struct sockaddr_in myaddr, new;
		if (daemonmode == 1) {
			if (fork())
				exit(0);  /* Exit from parent process */
			setsid();
			if (fork())
				return 0;
		}
		signal(SIGCHLD, handler_sigchld);
		config_init();
		chdir("/");
                hidegroups_init();
		listensocket = socket(AF_INET, SOCK_STREAM, 0);
#ifdef SO_REUSEADDR
		setsockopt(listensocket, SOL_SOCKET, SO_REUSEADDR, (void *) &i,
				   sizeof(i));
#endif
#ifdef SO_REUSEPORT
		setsockopt(listensocket, SOL_SOCKET, SO_REUSEPORT, (void *) &i,
				   sizeof(i));
#endif
		memset((void *) &myaddr, 0, sizeof(myaddr));
        if (!((port = strtoul(config_getoption("PORT"), NULL, 10))))
            port = DEFAULT_PORT;
		myaddr.sin_port = htons(port);
		if (!strcasecmp(config_getoption("BIND_TO_ADDR"), "any")
			|| !config_getoption("BIND_TO_ADDR")[0])
			myaddr.sin_addr.s_addr = INADDR_ANY;
		else
			myaddr.sin_addr.s_addr = inet_addr(config_getoption("BIND_TO_ADDR"));
		if (bind(listensocket, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0) {
			fprintf(stderr, "Bind failed: %s\n", strerror(errno));
			exit(1);
		}
		if (listen(listensocket, 5)) {
			fprintf(stderr, "Listen failed: %s\n", strerror(errno));
			exit(1);
		}
                
		for (i = 0; i < 3; i++) {
			close(i);		/* Remove fd pointing to the console */
			open("/dev/null", O_RDWR);	/* Create fd pointing nowhere */
		}
		my_length = sizeof(new);
		while ((sock = accept(listensocket, (struct sockaddr *) &new, &my_length))) {
			pid_t pid;
			/* If accept() becomes interrupted by SIGCHLD, it will return -1.
			 * So in order not to create a child process when that happens,
			 * we have to check if accept() returned an error.
			 */
			if (sock > 0) {
				pid = fork();
				if (!pid) {       /* child */
					close(0);
					close(1);
					close(2);
					isparent = 0;
					dup2(sock, fileno(stdin));
					dup2(sock, fileno(stderr));
					break;
				} else {          /* parent */
					struct bftpd_childpid *tmp_pid = malloc(sizeof(struct bftpd_childpid));
					tmp_pid->pid = pid;
					tmp_pid->sock = sock;
					bftpd_list_add(&child_list, tmp_pid);
				}
			}
		}
	}
#endif
        /* Child only. From here on... */

	devnull = fopen("/dev/null", "w");
	global_argc = argc;
	global_argv = argv;
	init_everything();
	atexit(end_child);
	signal(SIGTERM, handler_sigterm);
    signal(SIGALRM, handler_sigalrm);


    /* If we do not have getpwnam() for some reason, then
       we must use FILE_AUTH or exit. */
    #ifdef NO_GETPWNAM
    {
        char *file_auth_option;

        file_auth_option = config_getoption("FILE_AUTH");
        if (! file_auth_option[0] )
        {
           bftpd_log("Exiting, becasue we have no way to authorize clients.\n");
           exit(0);
        }
    }
    #endif

    get_value = strtoul(config_getoption("CONTROL_TIMEOUT"), NULL, 0);
    if (get_value <= INT_MAX)
       control_timeout = get_value;
    else
       control_timeout = 0;
    if (!control_timeout)
        control_timeout = CONTROL_TIMEOUT;

    get_value = strtoul(config_getoption("DATA_TIMEOUT"), NULL, 0);
    if (get_value <= INT_MAX)
       data_timeout = get_value;
    else
       data_timeout = 0;
    if (!data_timeout)
        data_timeout = DATA_TIMEOUT;
    get_value = strtoul(config_getoption("XFER_BUFSIZE"), NULL, 0);
    if (get_value <= INT_MAX)
       xfer_bufsize = get_value;
    else
       xfer_bufsize = 9;

    /* handled below
    if (!xfer_bufsize)
        xfer_bufsize = XFER_BUFSIZE;
    */
    /* do range check on xfer_bufsize, which is an int */
    if ( (xfer_bufsize <= 0) || (xfer_bufsize > 1000000) )
       xfer_bufsize = XFER_BUFSIZE;

    /* get scripts to run pre and post write */
    pre_write_script = config_getoption("PRE_WRITE_SCRIPT");
    if (! pre_write_script[0])
       pre_write_script = NULL;
    post_write_script = config_getoption("POST_WRITE_SCRIPT");
    if (! post_write_script[0])
       post_write_script = NULL;


	my_length = sizeof(remotename);
    if (getpeername(fileno(stderr), (struct sockaddr *) &remotename, &my_length)) {
		control_printf(SL_FAILURE, "421-Could not get peer IP address.\r\n421 %s.",
		               strerror(errno));
		return 0;
	}
#ifdef TCSUPPORT_IPV6_FTP
	fourinsix(&remotename);
    if (checkvalidaddr(&remotename) == 0) {
       	control_printf(SL_FAILURE, "425-Invalid peer IP address.\r\n425 %s.",
		               strerror(errno));
    }
#endif
	i = 1;
	setsockopt(fileno(stdin), SOL_SOCKET, SO_OOBINLINE, (void *) &i,
			   sizeof(i));
	setsockopt(fileno(stdin), SOL_SOCKET, SO_KEEPALIVE, (void *) &i,
			   sizeof(i));
#if defined(TCSUPPORT_ACCOUNT_ACL) || defined(TCSUPPORT_FW_UPGRADE_16M)
	getsockopt(fileno(stdin), SOL_IP, IP_SKB_MARK, &mark, &marklen);
#endif

	/* If option is set, determine the client FQDN */
#ifdef TCSUPPORT_IPV6_FTP
	 if (STORAGE_FAMILY(remotename) == AF_INET6)
	 {
		inet_ntop(AF_INET6,&(((struct sockaddr_in6 *)(&remotename))->sin6_addr),straddr,sizeof(straddr));
   	 }
	 else
	 {
	 	inet_ntop(AF_INET,&(((struct sockaddr_in*)(&remotename))->sin_addr),straddr,sizeof(straddr));
	 }

	 remotehostname = straddr;
#else	 
	if (!strcasecmp((char *) config_getoption("RESOLVE_CLIENT_IP"), "yes")) {
		if ((he = gethostbyaddr((char *) &remotename.sin_addr, sizeof(struct in_addr), AF_INET)))
                {
                   if (he->h_name)
			remotehostname = strdup(he->h_name);
                   else
                        remotehostname = "";
                }
		else
                {
                        temp_string = inet_ntoa(remotename.sin_addr);
                        if (temp_string)
                           remotehostname = strdup(temp_string);
                        else
                           remotehostname = "";
			/* remotehostname = strdup(inet_ntoa(remotename.sin_addr)); */
                }
	}
        else
        {
              temp_string = inet_ntoa(remotename.sin_addr);
              if (temp_string)
                 remotehostname = strdup(temp_string);
              else
                 remotehostname = "";
		/* remotehostname = strdup(inet_ntoa(remotename.sin_addr)); */
        }
#endif
	bftpd_log("Incoming connection from %s.\n", remotehostname);
        bftpd_statuslog(1, 0, "connect %s", remotehostname);
	my_length = sizeof(name);
	getsockname(fileno(stdin), (struct sockaddr *) &name, &my_length);
#ifdef TCSUPPORT_IPV6_FTP	
	fourinsix(&name);
	if (checkvalidaddr(&name) == 0) {
  	 	control_printf(SL_FAILURE, "425-Invalid local IP address.\r\n425 %s.",
	               strerror(errno));
	}
#endif
	print_file(220, config_getoption("MOTD_GLOBAL"));
	/* Parse hello message */
	strcpy(str, (char *) config_getoption("HELLO_STRING"));
	replace(str, "%v", VERSION, MAX_STRING_LENGTH);
#ifdef TCSUPPORT_IPV6_FTP		
	if (strstr(str, "%h")) {
		 if (STORAGE_FAMILY(name) == AF_INET6)
		 {
			replace(str, "%h", (char *) inet_ntop(AF_INET6,&(((struct sockaddr_in6 *)(&name))->sin6_addr),strip,sizeof(strip)), MAX_STRING_LENGTH);
	   	 }
		 else
		 {
		 	replace(str, "%h", (char *)inet_ntop(AF_INET,&(((struct sockaddr_in*)(&name))->sin_addr),strip,sizeof(strip)), MAX_STRING_LENGTH);
		 }
	}
	
	replace(str, "%i", (char *)inet_ntop(AF_INET,&(((struct sockaddr_in*)(&name))->sin_addr),strip,sizeof(strip)), MAX_STRING_LENGTH);
	
#else
	if (strstr(str, "%h")) {
		if ((he = gethostbyaddr((char *) &name.sin_addr, sizeof(struct in_addr), AF_INET)))
			replace(str, "%h", he->h_name, MAX_STRING_LENGTH);
		else
			replace(str, "%h", (char *) inet_ntoa(name.sin_addr), MAX_STRING_LENGTH);
	}
	replace(str, "%i", (char *) inet_ntoa(name.sin_addr), MAX_STRING_LENGTH);
#endif
	control_printf(SL_SUCCESS, "220 %s", str);

        /* We might not get any data, so let's set an alarm before the
           first read. -- Jesse <slicer69@hotmail.com> */
        alarm(control_timeout);
        
	/* Read lines from client and execute appropriate commands */
	while (fgets(str, MAXCMD, stdin)) {
        alarm(control_timeout);
        str[strlen(str) - 2] = 0;
        bftpd_statuslog(2, 0, "%s", str);
#ifdef DEBUG
		bftpd_log("Processing command: %s\n", str);
#endif
		parsecmd(str);
	}

    if (remotehostname) free(remotehostname);
  
	return 0;
}

