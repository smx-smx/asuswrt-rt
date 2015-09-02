/* inetd.c: Start up network services
 *
 * Copyright (C) 1998  Kenneth Albanowski <kjahds@kjahds.com>
 * Copyright (C) 1999  D. Jeff Dionne     <jeff@lineo.ca>
 * Copyright (C) 2000  Lineo, Inc.  (www.lineo.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/* Reworked code to allocate service structures dynamically.
 * Also compacted this structure a little and inlined functions
 * which are only called in one place.
 */
/* Fixed Null Pointer references caused by code in read_config()
 *                    - Rajiv Dhinakaran, 1 June 2000.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <termios.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/utsname.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>
#ifdef TCSUPPORT_IPV6_INETD
#include <stdarg.h>
#endif
#include "cfgfile.h"

/* The MODIFY_CONFIG option means that our configuration file lives
 * in a writable file system and that we should support reloading of it
 * via a HUP signal.
 */
#undef MODIFY_CONFIG
#undef DEBUG

#define MAX_CONNECT 8

static void read_config(void);

#ifdef TCSUPPORT_IPV6_INETD
const struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;
#endif
#ifdef arm
#define inline
#endif

#ifdef EMBED 
static void
set_signal(int sig, void (*handler)(int), int flags)
{
  struct sigaction action, oaction;
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = handler;
  action.sa_flags = flags;
  (void) sigaction(sig, &action, &oaction);
}
#endif
static void close_on_exec(int f)
{
	if (fcntl(f, F_SETFD, 1))
		perror("close on exec: ");
}

static inline void close_all_fds(int keep_top)
{
	int i, n;
	n = getdtablesize();
	for(i=keep_top+1;i<n;i++)
		close(i);
}

struct stService {
	struct stService *next;			// Reference to next service record
	int               port;			// Port to hang off
	int               master_socket;
	
	pid_t             pid[MAX_CONNECT];	// Seems less wasteful
        char             *args[6];		// Command args

	short             limit;		// Max number of such processes
	short             current;		// How many child processes

	// Put the bit fields together to help reduce space use
	unsigned	  enabled :1;		// Service enabled
	unsigned	  changed :1;
	#ifdef TCSUPPORT_IPV6_INETD
	unsigned	  tcp :2;		// tcp,tcp6,udp
	#else
		unsigned	  tcp :1;		// TCP or UDP
	#endif
	unsigned	  reconfig :1;
	
	// Dynamically allocated section for arguments
        char		  arg[0];
} *servicelist = NULL;


static inline int reap_child(int pid) {
  int reaped = 0;
  struct stService *p;

  for(p=servicelist; p!=NULL; p=p->next) {
    int j;
    for(j=0;j<MAX_CONNECT;j++)
      if (p->pid[j] == pid) {
        p->pid[j] = 0;
        p->current--;
      }
  }
  return reaped;
}

static inline int generate_select_fds(fd_set * readmask, fd_set * writemask)
{
  int max=0;
  struct stService *p;
  FD_ZERO(readmask);
  FD_ZERO(writemask);
#define FD_MSET(x,y) FD_SET((x),(y)); if ((x)>max) max = (x);

  for(p=servicelist; p!=NULL; p=p->next) {
    if (!strlen(p->args[0]) || (p->current >= p->limit) || !p->enabled)
      continue;

    FD_MSET(p->master_socket, readmask);

  }

  return max+1;
}


static pid_t
start_child(struct stService *p, int fd)
{
	pid_t	pid;

	pid = vfork();

	if (pid == 0) {
		if (fd != 0)
			dup2(fd, 0);
		if (fd != 1)
			dup2(fd, 1);
		if (fd != 2)
			dup2(fd, 2);
		if (fd > 2)
			close(fd);
		close_all_fds(2);
		execlp(p->args[0],
				p->args[0],
				p->args[1],
				p->args[2],
				p->args[3],
				p->args[4],
				p->args[5],
				NULL
				);
		_exit(0);
	}
	return(pid);
}


static inline void handle_incoming_fds(fd_set * readmask, fd_set * writemask)
{
  struct stService *p;

  for(p=servicelist; p!=NULL; p=p->next) {
    int fd;
    if (p->master_socket && FD_ISSET(p->master_socket, readmask)) {
      int j;
      for(j=0;j<MAX_CONNECT;j++)
        if (p->pid[j] == 0)
          break;

      if (p->tcp) {
        struct sockaddr_in remote;
        unsigned int remotelen = sizeof(remote);
        fd = accept(p->master_socket, (struct sockaddr*)&remote, &remotelen);
        if (fd < 0) {
          printf("accept failed\n");
          break;
        }
      } else {
        fd = p->master_socket;
      }

	  p->current++;
	  if ((p->pid[j] = start_child(p, fd)) == -1) {
		  /*
		   * if we fail to start the child,  disable the service
		   */
          p->enabled = 0;
          close(p->master_socket);
          p->master_socket = 0;
		  p->current--;
	  }

      if (p->tcp) {
        close(fd);
      }
    }
  }
}

static inline void start_services(void) {
  int s;
  struct stService *p;
  struct server_sockaddr;

  for(p=servicelist; p!=NULL; p=p->next) {
#ifdef TCSUPPORT_IPV6_INETD
	struct sockaddr_in6 server_sockaddr6;
#endif
    struct sockaddr_in server_sockaddr;

    if (p->master_socket || !strlen(p->args[0]))
      continue;

#ifdef TCSUPPORT_IPV6_INETD
    if (1 == p->tcp) {
#else
    if (p->tcp) {
#endif
      int true;
      
      if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("Unable to create socket\n");
      }

      close_on_exec(s);

      server_sockaddr.sin_family = AF_INET;
      server_sockaddr.sin_port = htons(p->port);
      server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
     
      true = 1;

      if((setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (void *)&true, 
         sizeof(true))) == -1) {
        perror("setsockopt: ");
      }

      if(bind(s, (struct sockaddr *)&server_sockaddr, 
        sizeof(server_sockaddr)) == -1)  {
        perror("Unable to bind server socket: ");
        close(s);
      }

      if(listen(s, 1) == -1) {
        printf("Unable to listen to socket port=%d\n",p->port);
      }
    
    }
#ifdef TCSUPPORT_IPV6_INETD
  	else if(2 == p->tcp) {
      int true;
      if ((s = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("Unable to create socket\n");
      }

      close_on_exec(s);
      server_sockaddr6.sin6_family = AF_INET6;
      memcpy(&server_sockaddr6.sin6_addr, &in6addr_any, sizeof (in6addr_any));
      server_sockaddr6.sin6_port = htons(p->port);
    
      true = 1;
      if((setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (void *)&true, 
         sizeof(true))) == -1) {
        perror("setsockopt: ");
      }

      if(bind(s, (struct sockaddr *)&server_sockaddr6, 
        sizeof(server_sockaddr6)) == -1)  {
        perror("Unable to bind server socket: ");
        close(s);
      }

      if(listen(s, 256) == -1) {
        printf("Unable to listen to socket port=%d\n",p->port);
      }

    }
#endif
	else {
		
      if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        printf("Unable to create socket\n");
      }

      close_on_exec(s);

      server_sockaddr.sin_family = AF_INET;
      server_sockaddr.sin_port = htons(p->port);
      server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

      if(bind(s, (struct sockaddr *)&server_sockaddr,
        sizeof(server_sockaddr)) == -1)  {
        printf("Unable to bind socket\n");
        close(s);
      }
    }
    p->master_socket = s;
    p->reconfig = 1;
    p->changed = 0;
  }  
}

static inline void stop_services(void) {
  struct stService *p;
  struct server_sockaddr;
  
  for(p=servicelist; p!=NULL; p=p->next) {
    if(p->master_socket)
      close(p->master_socket);
    p->master_socket = 0;
  }
}

static void reap_children(void)
{
  int child;
  int status;
  while ((child = waitpid(-1, &status, WNOHANG)) > 0) {
    reap_child(child);
  }
}

#ifdef MODIFY_CONFIG
volatile int got_hup;
static void hup_handler(int signo)
{
	got_hup = 1;
}
#endif

#ifdef EMBED
volatile int got_cont;
static void cont_handler(int signo)
{
	got_cont = 1;
}

static void stop_handler(int signo)
{
	got_cont = 0;
	/* To reduce memory usage & prevent callers from getting gummed up */
	stop_services(); 
	while(!got_cont) {
		pause();
		reap_children();
	}
	got_cont = 0;
}

static void child_handler(int signo)
{
	/* Don't reap, just interrupt the syscall */
}
#endif
static inline void kill_changed_things(void)
{
  struct stService *p;

	for(p=servicelist; p!= NULL; p=p->next) {
		int j;

		if (!p->changed || !p->reconfig)
			continue;
		
		if (p->master_socket) {
			close(p->master_socket);
			p->master_socket = 0;
		}

		for (j=0;j<MAX_CONNECT;j++) {
			if (p->pid[j] != 0) {
				kill(p->pid[j], SIGTERM);
				kill(p->pid[j], SIGHUP);
				p->pid[j] = 0;
			}
		}
		p->changed = p->reconfig = 0;
	}
}

static inline void run(void)
{
  fd_set rfds, wfds;
  struct timeval tv;
  int max;
  for(;;) {
    reap_children();
    start_services();

    max = generate_select_fds(&rfds, &wfds);
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (select(max, &rfds, &wfds, 0, &tv) > 0) {
      handle_incoming_fds(&rfds, &wfds);
    }

#ifdef MODIFY_CONFIG
    if (got_hup) {
    	got_hup = 0;
	read_config();
    	kill_changed_things();
    	continue;
    }
#endif
  }
}

/* Very simple file format, lines of the form
 * port# ignored tcp/udp ignored ignored <exec path> args...
 * this should be compatible with berkeley derived inetd
 */
#ifdef MODIFY_CONFIG
static void
#else
static inline void
#endif
read_config()
{
  FILE *cfp;
  char **args;
  char *ap;

  int j, k;
  struct stService *p;
  size_t sz;
  struct servent *sent;
#ifdef MODIFY_CONFIG
  //
  struct stService *q;

  // Trash the old service list
  for (p=servicelist; p!=NULL; p=q) {
	  q = p->next;
	  free(p);
  }
  servicelist = NULL;
#endif

  if (!(cfp = fopen(INETD_CONF,"r"))) {
    perror("Can't open " INETD_CONF);
    exit(1);
  }

  for (;;) {
    if (!(args = cfgread(cfp))) break;

    for (j=0; (j<11) && (args[j]); j++);
    if (j<6) {
      fprintf(stderr,"Bad line in config file %s...\n",
	      args[0] ? args[0] : "");
      continue;
    }

    sz = sizeof(struct stService);
    for (k=5; k<j; k++) {
	    sz += strlen(args[k]) + 1;		// Must remember the terminator
    }
    
    p = malloc(sz);
    if (p == NULL) {
	    fprintf(stderr, "Malloc failed\n");
	    continue;
    }
	//bzero(p, sz);//for 3262 toolchain, shnwind
	memset(p,0,sz);
    /* copy in the args and exec path. */
    /* No possible overrun here as we've already calculated the sizes */
//    p->args[j - 5] = NULL;
    ap = p->arg;
    for (k=5; k<j; k++) {                       //
      p->args[k - 5] = ap;              	//
      if (args[k]) {                            // Fixup for NULL PTR
        strcpy(ap, args[k]);               	//      - Rajiv
        ap += strlen(args[k]);                  //
      }                                         //
      *ap++ = '\0';
    }

#ifdef TCSUPPORT_IPV6_INETD
	if(!strcmp(args[2], "tcp"))
	{
		p->tcp = 1;
	}
	else if(!strcmp(args[2], "tcp6"))
	{
		p->tcp = 2;
		strcpy(args[2],"tcp");
	}
	else
	{
		p->tcp = 0;
	}
#else
    p->tcp = !strcmp(args[2], "tcp") ? 1 : 0;

#endif
	/* for "wait" processes,  only ever allow one to be run */

    p->limit = (strcmp(args[3], "wait") == 0) ? 1 : MAX_CONNECT;

    sent = getservbyname(args[0], args[2]);
    if (sent == NULL) {
      	fprintf(stderr, "can't find service\r\n");
      	free(p);
      	continue;
    }
    p->port = ntohs(sent->s_port);

    p->master_socket = 0;
    p->changed = 1;
    p->enabled = 1;

    p->next = servicelist;
    servicelist = p;
  }

  fclose(cfp);
#ifdef DEBUG
  /* Print this stuff out outside the above loop to ensure we
   * get the list processing correct too.
   */
  for (p=servicelist; p!=NULL; p=p->next)
    printf("service %s port %d %s %s\n",
	   p->args[0],
	   p->port,
	   p->tcp ? "tcp" : "udp",
	   p->enabled ? "enabled" : "disabled");
#endif
}

int
main(int argc, char *argv[], char *env[])
{
	FILE *fp;
	fp = fopen("/var/log/inetd.pid", "wb");
	if(fp)
       	{
		fprintf(fp, "%d", getpid());
		fclose(fp);
       	}

#ifdef EMBED
  set_signal(SIGPIPE, SIG_IGN, 0);
  set_signal(SIGSTOP, stop_handler, 0);
  set_signal(SIGTSTP, stop_handler, 0);
  set_signal(SIGCONT, cont_handler, 0);
  set_signal(SIGCHLD, child_handler, SA_INTERRUPT);
#ifdef MODIFY_CONFIG
  // This only makes sense if we've got a modifyable filesystem.
  set_signal(SIGHUP, hup_handler, 0);
#endif
  /*
   * we must have an open FD 0 as we use 0 to signify no-fd :-(
   */
  if (fcntl(0, F_GETFL) == -1)
  	open("/dev/null", O_RDWR);
#endif  

  /* Hack to fix problems with not dealing cleanly with fds of 0 */
  open("/dev/null", O_RDWR);

  read_config();

  run();
  
  /* not reached */
  return 0;
}
