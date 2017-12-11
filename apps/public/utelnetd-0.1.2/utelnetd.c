/* utelnetd.c
 *
 * Simple telnet server
 *
 * Bjorn Wesen, Axis Communications AB 
 * <bjornw@axis.com>
 * 
 * Joerg Schmitz-Linneweber, Aston GmbH
 * <schmitz-linneweber@aston-technologie.de>
 *
 * Vladimir Oleynik
 * <dzo@simtreas.ru>
 *
 * Robert Schwebel, Pengutronix
 * <r.schwebel@pengutronix.de>
 * 
 *
 * This file is distributed under the GNU General Public License (GPL),
 * please see the file LICENSE for further information.
 * 
 * ---------------------------------------------------------------------------
 * (C) 2000, 2001, 2002 by the authors mentioned above
 * ---------------------------------------------------------------------------
 *
 * The telnetd manpage says it all:
 *
 *   Telnetd operates by allocating a pseudo-terminal device (see pty(4))  for
 *   a client, then creating a login process which has the slave side of the
 *   pseudo-terminal as stdin, stdout, and stderr. Telnetd manipulates the
 *   master side of the pseudo-terminal, implementing the telnet protocol and
 *   passing characters between the remote client and the login process.
 */

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

/* we need getpty() 0.1.6 modify*/
#define __USE_GNU 1
#define __USE_XOPEN 1
#include <stdlib.h>
#undef __USE_GNU
#undef __USE_XOPEN

#include <errno.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <termios.h>
#ifdef DEBUG
#define TELCMDS
#define TELOPTS
#endif
#include <arpa/telnet.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <net/if.h>

#define BUFSIZE 4000

#define MIN(a,b) ((a) > (b) ? (b) : (a))

#ifdef TCSUPPORT_IPV6_TELNET
#define IPV4_TO_V6_PREFIX	"::ffff:"
#endif
									 
static char *loginpath = NULL;

/* shell name and arguments */

#if !defined(TCSUPPORT_ACCOUNT_ACL) 
static char *argv_init[] = {NULL, NULL};
#endif

#ifdef TCSUPPORT_TFTP_UPGRADE_PROTECT	
static char *telnet_path = "/tmp/telnet_info";
#endif

/* structure that describes a session */

struct tsession {
	struct tsession *next;
	int sockfd, ptyfd;
	int shell_pid;
	/* two circular buffers */
	char *buf1, *buf2;
	int rdidx1, wridx1, size1;
	int rdidx2, wridx2, size2;
	#ifdef TCSUPPORT_TFTP_UPGRADE_PROTECT	
	/* frank added bug 9609*/
	#ifdef TCSUPPORT_IPV6_TELNET
	struct sockaddr_in6 sockinfo;
	#else
	struct sockaddr_in sockinfo;
	#endif
	#endif	
};

#ifdef DEBUG
#define DEBUG_OUT(...) fprintf(stderr, __VA_ARGS__)
#else
#define DEBUG_OUT(...)
//static inline void DEBUG_OUT(const char *format, ...) {};
#endif

/*

   This is how the buffers are used. The arrows indicate the movement
   of data.

   +-------+     wridx1++     +------+     rdidx1++     +----------+
   |       | <--------------  | buf1 | <--------------  |          |
   |       |     size1--      +------+     size1++      |          |
   |  pty  |                                            |  socket  |
   |       |     rdidx2++     +------+     wridx2++     |          |
   |       |  --------------> | buf2 |  --------------> |          |
   +-------+     size2++      +------+     size2--      +----------+

   Each session has got two buffers.

*/

static int maxfd;

static struct tsession *sessions;

/* 
 * This code was ported from a version which was made for busybox. 
 * So we have to define some helper functions which are originally 
 * available in busybox...
 */  

void show_usage(void)
{
	printf("Usage: telnetd [-p port] [-l loginprogram] [-d]\n");
	printf("\n");
	printf("   -p port          specify the tcp port to connect to\n");
	printf("   -l loginprogram  program started by the server\n");
	printf("   -d               daemonize\n");
	printf("\n");         
	exit(1);
}

void perror_msg_and_die(char *text)
{
	fprintf(stderr,text);
	exit(1);
}

/*remove unuse function shnwind 2008.4.17*/
/*void error_msg_and_die(char *text, char *foo)
{
	perror_msg_and_die(text);
}*/


/* 
   Remove all IAC's from the buffer pointed to by bf (recieved IACs are ignored
   and must be removed so as to not be interpreted by the terminal).  Make an
   uninterrupted string of characters fit for the terminal.  Do this by packing
   all characters meant for the terminal sequentially towards the end of bf. 

   Return a pointer to the beginning of the characters meant for the terminal.
   and make *processed equal to the number of characters that were actually
   processed and *num_totty the number of characters that should be sent to
   the terminal.  
   
   Note - If an IAC (3 byte quantity) starts before (bf + len) but extends
   past (bf + len) then that IAC will be left unprocessed and *processed will be
   less than len.
  
   FIXME - if we mean to send 0xFF to the terminal then it will be escaped,
   what is the escape character?  We aren't handling that situation here.

  */
static char *
remove_iacs(unsigned char *bf, int len, int *processed, int *num_totty) {
    unsigned char *ptr = bf;
    unsigned char *totty = bf;
    unsigned char *end = bf + len;
   
    while (ptr < end) {
	if (*ptr != IAC) {
	    *totty++ = *ptr++;
	}
	else {
	    if ((ptr+2) < end) {
		/* the entire IAC is contained in the buffer 
		   we were asked to process. */
		DEBUG_OUT("Ignoring IAC 0x%02x, %s, %s\n", *ptr, TELCMD(*(ptr+1)), TELOPT(*(ptr+2)));
		ptr += 3;
	    } else {
		/* only the beginning of the IAC is in the 
		   buffer we were asked to process, we can't
		   process this char. */
		break;
	    }
	}
    }

    *processed = ptr - bf;
    *num_totty = totty - bf;
    /* move the chars meant for the terminal towards the end of the 
       buffer. */
    return memmove(ptr - *num_totty, bf, *num_totty);
}

/* This function will remove LF that follows a CR. Some telnet clients sends 
   CR followed by LF whenever user types an Enter key. Example windows 2000
   client
*/
static int subst_crlf_cr(unsigned char *bf, int len)
{
	unsigned char *ptr = bf;
	unsigned char *end = bf + len;
	int cr_found = 0;
	int lf_removed = 0;

	while (ptr < end) 
    {
		/* check if we have \r, then if it is followed by \n remove \n and write to terminal
           buffer
         */
		if (*ptr == '\r')
		{
			cr_found = 1;
		}
		else 
		{
			if ((*ptr == '\n') && (cr_found))
			{
				memmove(ptr,ptr+1, end-(ptr+1));
				lf_removed++;
				end--;
				ptr--;
			}
			cr_found = 0;
		}
		ptr++;
	}
	return lf_removed;
}

static int
getpty(char *line)
{
	int p;
#ifdef HAVE_DEVPTS_FS
	p = open("/dev/ptmx", 2);
	if (p > 0) {
		grantpt(p);
		unlockpt(p);
		strcpy(line, ptsname(p));
		return(p);
	}
#else
	struct stat stb;
	int i;
	int j;

	strcpy(line, "/dev/ptyXX");

	for (i = 0; i < 16; i++) {
		line[8] = "pqrstuvwxyzabcde"[i];
		line[9] = '0';
		if (stat(line, &stb) < 0) {
			continue;
		}
		for (j = 0; j < 16; j++) {
			line[9] = "0123456789abcdef"[j];
			if ((p = open(line, O_RDWR | O_NOCTTY)) >= 0) {
				line[5] = 't';
				return p;
			}
		}
	}
#endif /* HAVE_DEVPTS_FS */
	return -1;
}



static void
send_iac(struct tsession *ts, unsigned char command, int option)
{
	/* We rely on that there is space in the buffer for now.  */
	char *b = ts->buf2 + ts->rdidx2;
	*b++ = IAC;
	*b++ = command;
	*b++ = option;
	ts->rdidx2 += 3;
	ts->size2 += 3;
}


#if !defined(TCSUPPORT_ACCOUNT_ACL) 
#ifdef RTCONFIG_PROTECTION_SERVER
static struct tsession *
make_new_session(int sockfd, char *remoteAddr)
#else
static struct tsession *
make_new_session(int sockfd)
#endif
#endif
{
	struct termios termbuf;
	int pty, pid;
	static char tty_name[32];
	struct tsession *ts = (struct tsession *)malloc(sizeof(struct tsession));
	int t1, t2;

	ts->buf1 = (char *)malloc(BUFSIZE);
	ts->buf2 = (char *)malloc(BUFSIZE);

	ts->sockfd = sockfd;


	ts->rdidx1 = ts->wridx1 = ts->size1 = 0;
	ts->rdidx2 = ts->wridx2 = ts->size2 = 0;

	/* Got a new connection, set up a tty and spawn a shell.  */

	pty = getpty(tty_name);

	if (pty < 0) {
		fprintf(stderr, "All network ports in use!\n");
		return 0;
	}

	if (pty > maxfd)
		maxfd = pty;

	ts->ptyfd = pty;

	/* Make the telnet client understand we will echo characters so it 
	 * should not do it locally. We don't tell the client to run linemode,
	 * because we want to handle line editing and tab completion and other
	 * stuff that requires char-by-char support.
	 */

	send_iac(ts, DO, TELOPT_ECHO);
	send_iac(ts, DO, TELOPT_LFLOW);
	send_iac(ts, WILL, TELOPT_ECHO);
	send_iac(ts, WILL, TELOPT_SGA);


	if ((pid = fork()) < 0) {
		perror("fork");
	}
	if (pid == 0) {
		/* In child, open the child's side of the tty.  */
		int i, t;

		for(i = 0; i <= maxfd; i++)
			close(i);
		/* make new process group */
		if (setsid() < 0)
			perror_msg_and_die("setsid");

		t = open(tty_name, O_RDWR | O_NOCTTY);// modify to make ctrl+c possible
		//t = open(tty_name, O_RDWR);
		if (t < 0)
			perror_msg_and_die("Could not open tty");

		t1 = dup(0);
		t2 = dup(1);

		tcsetpgrp(0, getpid());
		/* modify to make ctrl+c possible*/
		if (ioctl(t, TIOCSCTTY, NULL)) {
			perror_msg_and_die("could not set controlling tty");
		} 
  
		/* The pseudo-terminal allocated to the client is configured to operate in
		 * cooked mode, and with XTABS CRMOD enabled (see tty(4)).
		 */

		tcgetattr(t, &termbuf);
		termbuf.c_lflag |= ECHO; /* if we use readline we dont want this */
		termbuf.c_oflag |= ONLCR|XTABS;
		termbuf.c_iflag |= ICRNL;
		termbuf.c_iflag &= ~IXOFF;
		/* termbuf.c_lflag &= ~ICANON; */
		tcsetattr(t, TCSANOW, &termbuf);

		DEBUG_OUT("stdin, stdout, stderr: %d %d %d\n", t, t1, t2);

		/* exec shell, with correct argv and env */
#ifdef RTCONFIG_PROTECTION_SERVER
		char * argv[ ]={"/bin/login", "-a", remoteAddr ,(char *)0};
		execv(loginpath, argv);
#else
		execv(loginpath, argv_init);
#endif
		
	    	/* NOT REACHED */
		perror_msg_and_die("execv");
	}

	ts->shell_pid = pid;

	return ts;
}

static void
free_session(struct tsession *ts)
{
	struct tsession *t = sessions;
#ifdef TCSUPPORT_TFTP_UPGRADE_PROTECT
	FILE *fp = NULL;
	char str[64];
#endif

	/* Unlink this telnet session from the session list.  */
	if(t == ts)
		sessions = ts->next;
	else {
		while(t->next != ts)
			t = t->next;
		t->next = ts->next;
	}

	free(ts->buf1);
	free(ts->buf2);

	kill(ts->shell_pid, SIGKILL);

	wait4(ts->shell_pid, NULL, 0, NULL);

	close(ts->ptyfd);
	close(ts->sockfd);

	if(ts->ptyfd == maxfd || ts->sockfd == maxfd)
		maxfd--;
	if(ts->ptyfd == maxfd || ts->sockfd == maxfd)
		maxfd--;

#ifdef TCSUPPORT_TFTP_UPGRADE_PROTECT
	// frank added
	memset(str, 0, sizeof(str));
#ifdef TCSUPPORT_IPV6_TELNET
	if(IN6_IS_ADDR_V4MAPPED(&(ts->sockinfo.sin6_addr))) {	
		//Take the low-order 32bits binary representation as file name. 			
		sprintf(str, "%s/%08X_%X.pid", telnet_path, 
			ts->sockinfo.sin6_addr.s6_addr32[3],
			ts->sockinfo.sin6_port);
	}
	else {//Take the whole IPv6 address binary representation as file name.
		sprintf(str, "%s/%08X%08X%08X%08X_%X.pid", telnet_path, 
			ts->sockinfo.sin6_addr.s6_addr32[0], ts->sockinfo.sin6_addr.s6_addr32[1],
			ts->sockinfo.sin6_addr.s6_addr32[2], ts->sockinfo.sin6_addr.s6_addr32[3],
			ts->sockinfo.sin6_port);
	}
	//printf("==>utelnetd debug, free, ipv6 str=%s\n", str);
#else
	sprintf(str, "%s/%08X_%X.pid", telnet_path, ts->sockinfo.sin_addr, ts->sockinfo.sin_port);
#endif
	remove(str);
	memset(str, 0, sizeof(str));
	sprintf(str, "%s/session%d", telnet_path, ts->shell_pid);
	remove(str);
#endif
	free(ts);
}


int main(int argc, char **argv)
{
#ifdef TCSUPPORT_IPV6_TELNET
	struct sockaddr_in6 sa;
#else
	struct sockaddr_in sa;
#endif
	int master_fd;
	fd_set rdfdset, wrfdset;
	int selret;
	int on = 1;
	int portnbr = 23;
	int c, ii;
	int daemonize = 0;
	int timeout = 0;

#ifdef TCSUPPORT_TFTP_UPGRADE_PROTECT
	FILE *fp = NULL;
	char str[64];
#endif

	/* check if user supplied a port number */

	for (;;) {
		c = getopt( argc, argv, "p:l:I:hd");
		if (c == EOF) break;
		switch (c) {
			case 'I':
				timeout = atoi(optarg);
				break;
			case 'p':
				portnbr = atoi(optarg);
				break;
			case 'l':
				loginpath = strdup (optarg);
				break;
			case 'd':
				daemonize = 1;
				break;
			case 'h': 
			default:
				show_usage();
				exit(1);
		}
	}

	if (!loginpath) {
	  loginpath = "/bin/login";
	  if (access(loginpath, X_OK) < 0)
	    loginpath = "/bin/sh";
	}
	  
	if (access(loginpath, X_OK) < 0) {
		/* workaround: error_msg_and_die has doesn't understand
 		   variable argument lists yet*/
		fprintf(stderr,"\"%s\"",loginpath);
		perror_msg_and_die(" is no valid executable!\n");
	}

	printf("telnetd: starting\n");
	printf("  port: %i; login program: %s\n",portnbr,loginpath);

	argv_init[0] = loginpath;
	sessions = 0;

	/* Grab a TCP socket.  */
#ifdef TCSUPPORT_IPV6_TELNET
	master_fd = socket(AF_INET6, SOCK_STREAM, 0);
#else
	master_fd = socket(AF_INET, SOCK_STREAM, 0);
#endif

	if (master_fd < 0) {
		perror("socket");
		return 1;
	}
	(void)setsockopt(master_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	/* Set it to listen to specified port.  */
#ifdef TCSUPPORT_IPV6_TELNET
	memset((void *)&sa, 0, sizeof(sa));
	sa.sin6_family = AF_INET6;
	sa.sin6_port = htons(portnbr);
#else
	memset((void *)&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(portnbr);
#endif

	if (bind(master_fd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		perror("bind");
		return 1;
	}

	if (listen(master_fd, 1) < 0) {
		perror("listen");
		return 1;
	}

	if (daemonize) 
	{
		DEBUG_OUT("  daemonizing\n");
		if (daemon(0, 1) < 0) perror_msg_and_die("daemon");
	}

	maxfd = master_fd;

#ifdef TCSUPPORT_TFTP_UPGRADE_PROTECT	
	// frank added
	mkdir(telnet_path, 0755);
#endif	
	do {
		struct tsession *ts;
		struct timeval  *tv_ptr = NULL;
		struct timeval  tmout;

		FD_ZERO(&rdfdset);
		FD_ZERO(&wrfdset);
		if (timeout) {
			tmout.tv_sec = timeout;
			tmout.tv_usec = 0;
			tv_ptr = &tmout;
		}

		/* select on the master socket, all telnet sockets and their
		 * ptys if there is room in their respective session buffers.
		 */

		FD_SET(master_fd, &rdfdset);

		ts = sessions;
		while (ts) {
			/* buf1 is used from socket to pty
			 * buf2 is used from pty to socket
			 */
			if (ts->size1 > 0) {
				FD_SET(ts->ptyfd, &wrfdset);  /* can write to pty */
			}
			if (ts->size1 < BUFSIZE) {
				FD_SET(ts->sockfd, &rdfdset); /* can read from socket */
			}
			if (ts->size2 > 0) {
				FD_SET(ts->sockfd, &wrfdset); /* can write to socket */
			}
			if (ts->size2 < BUFSIZE) {
				FD_SET(ts->ptyfd, &rdfdset);  /* can read from pty */
			}
			ts = ts->next;
		}

		selret = select(maxfd + 1, &rdfdset, &wrfdset, 0, tv_ptr);

		if (!selret) {
			/* timeout occurred. Let us do some clean up and exit */
			/* treat all sessions as the same timeout value */
			ts = sessions;
			while (ts) {
            	free_session(ts);
				ts = ts->next;
			}
			continue;
			/* exit(0); */
		}

		/* First check for and accept new sessions.  */
		if (FD_ISSET(master_fd, &rdfdset)) {
			int fd, salen;

			salen = sizeof(sa);	
			if ((fd = accept(master_fd, (struct sockaddr *)&sa,
					 &salen)) < 0) {
				continue;
			} else {
				/* Create a new session and link it into
				   our active list.  */
#if !defined(TCSUPPORT_ACCOUNT_ACL)
#ifdef RTCONFIG_PROTECTION_SERVER
				struct in_addr ip_addr_t;
				char addr[8];
#ifdef TCSUPPORT_IPV6_TELNET
				//inet_ntop(PF_INET6,&sa.sin6_addr, addr, sizeof(addr));
				if(IN6_IS_ADDR_V4MAPPED(&sa.sin6_addr)) {
					sprintf(addr, "%08x", sa.sin6_addr.s6_addr32[3]);
					sscanf(addr , "%08x", &ip_addr_t.s_addr);
					printf("[%s:%d] %s\n", __FUNCTION__, __LINE__, inet_ntoa(ip_addr_t));
				}
#else
				sprintf(addr, "%x", sa.sin_addr);
				sscanf(addr , "%x", &ip_addr_t.s_addr);
				printf("[%s:%d] %s\n", __FUNCTION__, __LINE__, inet_ntoa(ip_addr_t));
#endif
				struct tsession *new_ts = make_new_session(fd, inet_ntoa(ip_addr_t));
#else
				struct tsession *new_ts = make_new_session(fd);
#endif
#endif
				if (new_ts) {
					new_ts->next = sessions;
					sessions = new_ts;
					// frank added bug 9603
					#ifdef TCSUPPORT_TFTP_UPGRADE_PROTECT				
					new_ts->sockinfo = sa;
					memset(str, 0, sizeof(str));
					#ifdef TCSUPPORT_IPV6_TELNET
					//e.g.The ipv4-mapped ipv6 address text address representation is"::ffff:192.168.1.10".
					//And the binary representation is "00000000000000000000FFFFC0A8010A".
					//The IPv4 address will be stored in the low-order 32bits. 
					//In this case, just take the last 32bits binary representation as file name. 
					//The macro "IN6_IS_ADDR_V4MAPPED" is defined as:
					//#define IN6_IS_ADDR_V4MAPPED  (new_ts->sockinfo.sin6_addr.s6_addr32[0] == 0x0 && /
					//				new_ts->sockinfo.sin6_addr.s6_addr32[1] == 0x0 && /
					//				new_ts->sockinfo.sin6_addr.s6_addr32[2] == 0x0000ffff) 
					if(IN6_IS_ADDR_V4MAPPED(&(new_ts->sockinfo.sin6_addr))) {	
						sprintf(str, "%s/%08X_%X.pid", telnet_path, 
							new_ts->sockinfo.sin6_addr.s6_addr32[3],
							new_ts->sockinfo.sin6_port);
						//struct in_addr ip_addr;
						//char ip[8];
						//sprintf(ip, "%08x", new_ts->sockinfo.sin6_addr.s6_addr32[3]);
						//sscanf(ip, "%08x", &ip_addr.s_addr);
						//printf("[%s:%d] %s\n", __FUNCTION__, __LINE__, inet_ntoa(ip_addr));
					}
					else {//Take the whole IPv6 address binary representation as file name.
						sprintf(str, "%s/%08X%08X%08X%08X_%X.pid", telnet_path, 
							new_ts->sockinfo.sin6_addr.s6_addr32[0], new_ts->sockinfo.sin6_addr.s6_addr32[1],
							new_ts->sockinfo.sin6_addr.s6_addr32[2], new_ts->sockinfo.sin6_addr.s6_addr32[3],
							new_ts->sockinfo.sin6_port);
					}
					//printf("==>utelnetd debug, ipv6 str=%s\n", str);
					#else
					sprintf(str, "%s/%X_%X.pid", telnet_path, 
							new_ts->sockinfo.sin_addr, 	new_ts->sockinfo.sin_port);
					#endif
					fp = fopen(str, "w");
					if (fp!=NULL) {
						sprintf(str, "%d", new_ts->shell_pid);
						fputs(str, fp);
						fclose(fp);
						sprintf(str, "%s/session%d", telnet_path, new_ts->shell_pid);
						fp = fopen(str, "w");
						if (fp!=NULL) { 
							fputs("0", fp);
							fclose(fp);	
						} else {
							
							printf("the file for tftp upgrade protect cannot be created\n");
						}
					} else {
						
						printf("the file for tftp upgrade protect cannot be created\n");
					}
					#endif

					if (fd > maxfd)
						maxfd = fd;
				} else {
					close(fd);
				}
			}
		}

		/* Then check for data tunneling.  */

		ts = sessions;
		while (ts) { /* For all sessions...  */
			int maxlen, w, r;
			struct tsession *next = ts->next; /* in case we free ts. */

			if (ts->size1 && FD_ISSET(ts->ptyfd, &wrfdset)) {
			    int processed, num_totty, num_lfs;
			    char *ptr;
				/* Write to pty from buffer 1.  */
				
				maxlen = MIN(BUFSIZE - ts->wridx1,
					     ts->size1);
				ptr = remove_iacs(ts->buf1 + ts->wridx1, maxlen, 
					&processed, &num_totty);
		
				/* the difference between processed and num_totty
				   is all the iacs we removed from the stream.
				   Adjust buf1 accordingly. */
				ts->wridx1 += processed - num_totty;
				ts->size1 -= processed - num_totty;

				/* Do some raw data handling for converting CRLF to CRNL. This is needed
 				since some of the telnet clients sends CRLF to indicate end of line */
				num_lfs=subst_crlf_cr(ts->buf1 + ts->wridx1, maxlen);
				ts->size1-=num_lfs;
				num_totty-=num_lfs;

				w = write(ts->ptyfd, ptr, num_totty);
				if (w < 0) {
					perror("write");
					free_session(ts);
					ts = next;
					continue;
				}
				ts->wridx1 += w;
				ts->size1 -= w;
				if (ts->wridx1 == BUFSIZE)
					ts->wridx1 = 0;
			}

			if (ts->size2 && FD_ISSET(ts->sockfd, &wrfdset)) {
				/* Write to socket from buffer 2.  */
				maxlen = MIN(BUFSIZE - ts->wridx2,
					     ts->size2);
				w = write(ts->sockfd, ts->buf2 + ts->wridx2, maxlen);
				if (w < 0) {
					perror("write");
					free_session(ts);
					ts = next;
					continue;
				}
				ts->wridx2 += w;
				ts->size2 -= w;
				if (ts->wridx2 == BUFSIZE)
					ts->wridx2 = 0;
			}

			if (ts->size1 < BUFSIZE && FD_ISSET(ts->sockfd, &rdfdset)) {
				/* Read from socket to buffer 1. */
				maxlen = MIN(BUFSIZE - ts->rdidx1,
					     BUFSIZE - ts->size1);
				r = read(ts->sockfd, ts->buf1 + ts->rdidx1, maxlen);
				if (!r || (r < 0 && errno != EINTR)) {
					free_session(ts);
					ts = next;
					continue;
				}
				if(!*(ts->buf1 + ts->rdidx1 + r - 1)) {
					r--;
					if(!r)
						continue;
				}
				ts->rdidx1 += r;
				ts->size1 += r;
				if (ts->rdidx1 == BUFSIZE)
					ts->rdidx1 = 0;
			}

			if (ts->size2 < BUFSIZE && FD_ISSET(ts->ptyfd, &rdfdset)) {
				/* Read from pty to buffer 2.  */
				maxlen = MIN(BUFSIZE - ts->rdidx2,
					     BUFSIZE - ts->size2);
				r = read(ts->ptyfd, ts->buf2 + ts->rdidx2, maxlen);
				if (!r || (r < 0 && errno != EINTR)) {
					free_session(ts);
					ts = next;
					continue;
				}
				for (ii=0; ii < r; ii++)
				  if (*(ts->buf2 + ts->rdidx2 + ii) == 3)
				    fprintf(stderr, "found <CTRL>-<C> in data!\n");
				ts->rdidx2 += r;
				ts->size2 += r;
				if (ts->rdidx2 == BUFSIZE)
					ts->rdidx2 = 0;
			}

			if (ts->size1 == 0) {
				ts->rdidx1 = 0;
				ts->wridx1 = 0;
			}
			if (ts->size2 == 0) {
				ts->rdidx2 = 0;
				ts->wridx2 = 0;
			}
			ts = next;
		}

	} while (1);

	return 0;
}
