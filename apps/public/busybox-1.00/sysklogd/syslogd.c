/* vi: set sw=4 ts=4: */
/*
 * Mini syslogd implementation for busybox
 *
 * Copyright (C) 1999-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * Copyright (C) 2000 by Karl M. Hegbloom <karlheg@debian.org>
 *
 * "circular buffer" Copyright (C) 2001 by Gennady Feldman <gfeldman@gena01.com>
 *
 * Maintainer: Gennady Feldman <gfeldman@gena01.com> as of Mar 12, 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <netdb.h>
#include <paths.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/param.h>

#include "busybox.h"

/* SYSLOG_NAMES defined to pull some extra junk from syslog.h */
#define SYSLOG_NAMES
#include <sys/syslog.h>
#include <sys/uio.h>
#define NAME_LENGTH 32

#if !defined(TCSUPPORT_CT) 

#ifdef TCSUPPORT_SYSLOG_ENHANCE
#include "../../lib/libtcapi.h"
#endif

/* Path for the file where all log messages are written */
#define __LOG_FILE "/var/log/messages"
#ifdef TCSUPPORT_SYSLOG_ENHANCE
//#define	 SYSLOG_DBG

#define 	SELECT_TIMEOUT			5	/* seconds */
#define 	TIMECHANGERANGE			60/*60s*/
#define 	SYSLOG_CONF_PATH		"/var/log/log.conf"

#define		SYSLOG_FROM_FLASH		"LogAccess_FromFlash"
#define		SYSLOG_TO_FILE			"LogAccess_ToFile"
#define		SYSLOG_TO_FLASH			"LogAccess_ToFlash"

enum severity_level{
	LEVEL_EMER = 0,
	LEVEL_ALERT,
	LEVEL_CRITICAL,
	LEVEL_ERROR,
	LEVEL_WARN,
	LEVEL_NOTICE,
	LEVEL_INFO,
	LEVEL_DEBUG,
	LEVEL_NUM,
};

static char severity_name[][16] = {
	"Emergency",
	"Alert",
	"Critical",
	"Error",
	"Warning",
	"Notice",
	"Informational",
	"Debug",	
	"",
};

char syslog_param_name[][NAME_LENGTH]=
{
	{"WriteLevel"},
	{"WriteFlashInterval"},
	{"UpdateInterval"},
	{"AllLogSave"},
	{""},
};

enum syslog_param {
	WRITE_LEVEL,
	WRITE_INTERVAL,
	UPDATE_INTERVAL,
	ALLLOGSAVE,
	SYSLOG_PARAM_NUM,
};

static int gNewLogCnt = 0;
static int gLogWriteLevel = LEVEL_INFO;
static int gWriteFlashInterval = 300;
static int gUpdateFileInterval = 5;

static int gAllSaveFlag = 1;
static char currLogFilePath[24] = "/var/log/currLogFile";

#endif


/* Path to the unix socket */
static char lfile[MAXPATHLEN];

static const char *logFilePath = __LOG_FILE;

#ifdef CONFIG_FEATURE_ROTATE_LOGFILE
/* max size of message file before being rotated */
#ifdef TCSUPPORT_NAND_FLASH
static int logFileSize = 128 * 1024;
#else
static int logFileSize = 64 * 1024;
#endif

/* number of rotated message files */
static int logFileRotate = 1;
#endif

/* interval between marks in seconds */
static int MarkInterval = 20 * 60;

/* localhost's name */
static char LocalHostName[64];

#ifdef CONFIG_FEATURE_REMOTE_LOG
#include <netinet/in.h>
/* udp socket for logging to remote host */
static int remotefd = -1;
static struct sockaddr_in remoteaddr;

/* where do we log? */
static char *RemoteHost;

/* what port to log to? */
static int RemotePort = 514;

/* To remote log or not to remote log, that is the question. */
static int doRemoteLog = FALSE;
static int local_logging = FALSE;
#endif

/* Make loging output smaller. */
static bool small = false;


#define MAXLINE         1024	/* maximum line length */


/* circular buffer variables/structures */
#ifdef CONFIG_FEATURE_IPC_SYSLOG

#if CONFIG_FEATURE_IPC_SYSLOG_BUFFER_SIZE < 4
#error Sorry, you must set the syslogd buffer size to at least 4KB.
#error Please check CONFIG_FEATURE_IPC_SYSLOG_BUFFER_SIZE
#endif

#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

/* our shared key */
static const long KEY_ID = 0x414e4547;	/*"GENA" */

// Semaphore operation structures
static struct shbuf_ds {
	int size;			// size of data written
	int head;			// start of message list
	int tail;			// end of message list
	char data[1];		// data/messages
} *buf = NULL;			// shared memory pointer

static struct sembuf SMwup[1] = { {1, -1, IPC_NOWAIT} };	// set SMwup
static struct sembuf SMwdn[3] = { {0, 0}, {1, 0}, {1, +1} };	// set SMwdn

static int shmid = -1;	// ipc shared memory id
static int s_semid = -1;	// ipc semaphore id
static int shm_size = ((CONFIG_FEATURE_IPC_SYSLOG_BUFFER_SIZE)*1024);	// default shm size
static int circular_logging = FALSE;

/*
 * sem_up - up()'s a semaphore.
 */
static inline void sem_up(int semid)
{
	if (semop(semid, SMwup, 1) == -1) {
		bb_perror_msg_and_die("semop[SMwup]");
	}
}

/*
 * sem_down - down()'s a semaphore
 */
static inline void sem_down(int semid)
{
	if (semop(semid, SMwdn, 3) == -1) {
		bb_perror_msg_and_die("semop[SMwdn]");
	}
}


void ipcsyslog_cleanup(void)
{
	printf("Exiting Syslogd!\n");
	if (shmid != -1) {
		shmdt(buf);
	}

	if (shmid != -1) {
		shmctl(shmid, IPC_RMID, NULL);
	}
	if (s_semid != -1) {
		semctl(s_semid, 0, IPC_RMID, 0);
	}
}

void ipcsyslog_init(void)
{
	if (buf == NULL) {
		if ((shmid = shmget(KEY_ID, shm_size, IPC_CREAT | 1023)) == -1) {
			bb_perror_msg_and_die("shmget");
		}

		if ((buf = shmat(shmid, NULL, 0)) == NULL) {
			bb_perror_msg_and_die("shmat");
		}

		buf->size = shm_size - sizeof(*buf);
		buf->head = buf->tail = 0;

		// we'll trust the OS to set initial semval to 0 (let's hope)
		if ((s_semid = semget(KEY_ID, 2, IPC_CREAT | IPC_EXCL | 1023)) == -1) {
			if (errno == EEXIST) {
				if ((s_semid = semget(KEY_ID, 2, 0)) == -1) {
					bb_perror_msg_and_die("semget");
				}
			} else {
				bb_perror_msg_and_die("semget");
			}
		}
	} else {
		printf("Buffer already allocated just grab the semaphore?");
	}
}

/* write message to buffer */
void circ_message(const char *msg)
{
	int l = strlen(msg) + 1;	/* count the whole message w/ '\0' included */

	sem_down(s_semid);

	/*
	 * Circular Buffer Algorithm:
	 * --------------------------
	 *
	 * Start-off w/ empty buffer of specific size SHM_SIZ
	 * Start filling it up w/ messages. I use '\0' as separator to break up messages.
	 * This is also very handy since we can do printf on message.
	 *
	 * Once the buffer is full we need to get rid of the first message in buffer and
	 * insert the new message. (Note: if the message being added is >1 message then
	 * we will need to "remove" >1 old message from the buffer). The way this is done
	 * is the following:
	 *      When we reach the end of the buffer we set a mark and start from the beginning.
	 *      Now what about the beginning and end of the buffer? Well we have the "head"
	 *      index/pointer which is the starting point for the messages and we have "tail"
	 *      index/pointer which is the ending point for the messages. When we "display" the
	 *      messages we start from the beginning and continue until we reach "tail". If we
	 *      reach end of buffer, then we just start from the beginning (offset 0). "head" and
	 *      "tail" are actually offsets from the beginning of the buffer.
	 *
	 * Note: This algorithm uses Linux IPC mechanism w/ shared memory and semaphores to provide
	 *       a threasafe way of handling shared memory operations.
	 */
	if ((buf->tail + l) < buf->size) {
		/* before we append the message we need to check the HEAD so that we won't
		   overwrite any of the message that we still need and adjust HEAD to point
		   to the next message! */
		if (buf->tail < buf->head) {
			if ((buf->tail + l) >= buf->head) {
				/* we need to move the HEAD to point to the next message
				 * Theoretically we have enough room to add the whole message to the
				 * buffer, because of the first outer IF statement, so we don't have
				 * to worry about overflows here!
				 */
				int k = buf->tail + l - buf->head;	/* we need to know how many bytes
													   we are overwriting to make
													   enough room */
				char *c =
					memchr(buf->data + buf->head + k, '\0',
						   buf->size - (buf->head + k));
				if (c != NULL) {	/* do a sanity check just in case! */
					buf->head = c - buf->data + 1;	/* we need to convert pointer to
													   offset + skip the '\0' since
													   we need to point to the beginning
													   of the next message */
					/* Note: HEAD is only used to "retrieve" messages, it's not used
					   when writing messages into our buffer */
				} else {	/* show an error message to know we messed up? */
					printf("Weird! Can't find the terminator token??? \n");
					buf->head = 0;
				}
			}
		}

		/* in other cases no overflows have been done yet, so we don't care! */
		/* we should be ok to append the message now */
		strncpy(buf->data + buf->tail, msg, l);	/* append our message */
		buf->tail += l;	/* count full message w/ '\0' terminating char */
	} else {
		/* we need to break up the message and "circle" it around */
		char *c;
		int k = buf->tail + l - buf->size;	/* count # of bytes we don't fit */

		/* We need to move HEAD! This is always the case since we are going
		 * to "circle" the message.
		 */
		c = memchr(buf->data + k, '\0', buf->size - k);

		if (c != NULL) {	/* if we don't have '\0'??? weird!!! */
			/* move head pointer */
			buf->head = c - buf->data + 1;

			/* now write the first part of the message */
			strncpy(buf->data + buf->tail, msg, l - k - 1);

			/* ALWAYS terminate end of buffer w/ '\0' */
			buf->data[buf->size - 1] = '\0';

			/* now write out the rest of the string to the beginning of the buffer */
			strcpy(buf->data, &msg[l - k - 1]);

			/* we need to place the TAIL at the end of the message */
			buf->tail = k + 1;
		} else {
			printf
				("Weird! Can't find the terminator token from the beginning??? \n");
			buf->head = buf->tail = 0;	/* reset buffer, since it's probably corrupted */
		}

	}
	sem_up(s_semid);
}
#endif							/* CONFIG_FEATURE_IPC_SYSLOG */


#ifdef TCSUPPORT_SYSLOG_ENHANCE
/* Read syslog config file "/var/log/log.conf"
  * e.g.
  * WriteLevel 7
*/
int read_syslog_conf(void)
{
	FILE *fp = NULL;
	char buf[128] = {0};
	char parm_value[SYSLOG_PARAM_NUM][8];
	int i = 0;
	char *ptr = NULL;
	char tmp[NAME_LENGTH] = {0};
	
	fp = fopen(SYSLOG_CONF_PATH, "r");
	if(fp == NULL) {
		printf("\n==>Syslog Error, open syslog conf file %s failed\n", SYSLOG_CONF_PATH);
		return -1;
	}
	
	for(i=0; i<SYSLOG_PARAM_NUM; i++) {
		memset(buf, 0, sizeof(buf));
		memset(parm_value[i], 0, 8);
		fgets(buf, 128, fp);	
		ptr = strstr(buf, syslog_param_name[i]);		
		if(ptr != NULL) {
			sscanf(ptr, "%s %s", tmp, parm_value[i]);
		}
	}

	if  (strlen(parm_value[WRITE_LEVEL]) != 0)
		gLogWriteLevel = atoi(parm_value[WRITE_LEVEL]);
	
	if(strlen(parm_value[WRITE_INTERVAL]) != 0)
		gWriteFlashInterval = atoi(parm_value[WRITE_INTERVAL]);

	if(strlen(parm_value[UPDATE_INTERVAL]) != 0)
		gUpdateFileInterval = atoi(parm_value[UPDATE_INTERVAL]);

	if (!strcmp(parm_value[ALLLOGSAVE], "No"))
		gAllSaveFlag = 0;
	
#ifdef SYSLOG_DBG
	tcdbg_printf("\n\n===%s[%d] writeflash=%d updatefileinterval=%d  ====\n\n", __FUNCTION__, __LINE__, gWriteFlashInterval, gUpdateFileInterval);
	tcdbg_printf("\n\n===%s[%d] loglevel=%d saveflag=%d====\n", __FUNCTION__, __LINE__, gLogWriteLevel, gAllSaveFlag);
#endif

	return 0;	
}
#endif


/* Note: There is also a function called "message()" in init.c */
/* Print a message to the log file. */
static void message(char *fmt, ...) __attribute__ ((format(printf, 1, 2)));
static void message(char *fmt, ...)
{
	int fd;
	struct flock fl;
	va_list arguments;

	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 1;

#ifdef CONFIG_FEATURE_IPC_SYSLOG
	if ((circular_logging == TRUE) && (buf != NULL)) {
		char b[1024];

		va_start(arguments, fmt);
		vsnprintf(b, sizeof(b) - 1, fmt, arguments);
		va_end(arguments);
		circ_message(b);

	} else
#endif
	if ((fd =
			 device_open(logFilePath,
							 O_WRONLY | O_CREAT | O_NOCTTY | O_APPEND |
							 O_NONBLOCK)) >= 0) {
		fl.l_type = F_WRLCK;
		fcntl(fd, F_SETLKW, &fl);
#ifdef CONFIG_FEATURE_ROTATE_LOGFILE
		if ( logFileSize > 0 ) {
			struct stat statf;
			int r = fstat(fd, &statf);
			if( !r && (statf.st_mode & S_IFREG)
				&& (lseek(fd,0,SEEK_END) > logFileSize) ) {
				if(logFileRotate > 0) {
					int i;
					char oldFile[(strlen(logFilePath)+3)], newFile[(strlen(logFilePath)+3)];
					for(i=logFileRotate-1;i>0;i--) {
						sprintf(oldFile, "%s.%d", logFilePath, i-1);
						sprintf(newFile, "%s.%d", logFilePath, i);
						rename(oldFile, newFile);
					}
					sprintf(newFile, "%s.%d", logFilePath, 0);
					fl.l_type = F_UNLCK;
					fcntl (fd, F_SETLKW, &fl);
					close(fd);
					rename(logFilePath, newFile);
					fd = device_open (logFilePath,
						   O_WRONLY | O_CREAT | O_NOCTTY | O_APPEND |
						   O_NONBLOCK);
					fl.l_type = F_WRLCK;
					fcntl (fd, F_SETLKW, &fl);
				} else {
					ftruncate( fd, 0 );
				}
			}
		}
#endif
		va_start(arguments, fmt);
		vdprintf(fd, fmt, arguments);
		va_end(arguments);
		fl.l_type = F_UNLCK;
		fcntl(fd, F_SETLKW, &fl);
		close(fd);
	#ifdef TCSUPPORT_SYSLOG_ENHANCE
		gNewLogCnt ++;
	#endif
	} else {
		/* Always send console messages to /dev/console so people will see them. */
		if ((fd =
			 device_open(_PATH_CONSOLE,
						 O_WRONLY | O_NOCTTY | O_NONBLOCK)) >= 0) {
			va_start(arguments, fmt);
			vdprintf(fd, fmt, arguments);
			va_end(arguments);
			close(fd);
		} else {
			fprintf(stderr, "Bummer, can't print: ");
			va_start(arguments, fmt);
			vfprintf(stderr, fmt, arguments);
			fflush(stderr);
			va_end(arguments);
		}
	}
}

#ifdef CONFIG_FEATURE_REMOTE_LOG
static void init_RemoteLog(void)
{
	memset(&remoteaddr, 0, sizeof(remoteaddr));
	remotefd = socket(AF_INET, SOCK_DGRAM, 0);

	if (remotefd < 0) {
		bb_error_msg("cannot create socket");
	}

	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_addr = *(struct in_addr *) *(xgethostbyname(RemoteHost))->h_addr_list;
	remoteaddr.sin_port = htons(RemotePort);
}
#endif

#ifdef SYSLOG_DBG
void
tcdbg_printf(char *fmt,...){

	FILE *proc_file;
	char msg[256];
	va_list args;

	va_start(args, fmt);
/* jrchen modified 20100310*/
//	vsprintf(msg, fmt, args);
	vsnprintf(msg, 256, fmt, args);	

    proc_file = fopen("/proc/tc3162/dbg_msg", "w");
	if (!proc_file) {
		printf("open /proc/tc3162/dbg_msg fail\n");
		return;
	}
	fprintf(proc_file, "%s", msg);
	fclose(proc_file);
	va_end(args);
}
#endif

static void logMessage(int pri, char *msg)
{
	time_t now;
	char *timestamp;
	static char res[20] = "";
#ifdef CONFIG_FEATURE_REMOTE_LOG	
	static char line[MAXLINE + 1];
#endif
	CODE *c_pri, *c_fac;
#ifdef TCSUPPORT_SYSLOG_ENHANCE
	struct tm *local;
	time_t t;
	char new_timestamp[24];
	static char new_msg[MAXLINE + 1];
	static char tmp_msg[MAXLINE + 1];


	memset(new_msg, 0, sizeof(new_msg));
	memset(tmp_msg, 0, sizeof(tmp_msg));
#endif

	if (pri != 0) {
		for (c_fac = facilitynames;
			 c_fac->c_name && !(c_fac->c_val == LOG_FAC(pri) << 3); c_fac++);
		for (c_pri = prioritynames;
			 c_pri->c_name && !(c_pri->c_val == LOG_PRI(pri)); c_pri++);
		if (c_fac->c_name == NULL || c_pri->c_name == NULL) {
		#ifndef TCSUPPORT_SYSLOG_ENHANCE
			snprintf(res, sizeof(res), "<%d>", pri);
		#else
			memcpy(res, "[ ]", sizeof(res));
		#endif
		} else {
		#ifndef TCSUPPORT_SYSLOG_ENHANCE
			snprintf(res, sizeof(res), "%s.%s", c_fac->c_name, c_pri->c_name);
		#else
			snprintf(res, sizeof(res), "[%s]", severity_name[LOG_PRI(pri)]);
		#endif
		}
	}

#ifndef TCSUPPORT_SYSLOG_ENHANCE
	if (strlen(msg) < 16 || msg[3] != ' ' || msg[6] != ' ' ||
		msg[9] != ':' || msg[12] != ':' || msg[15] != ' ') {
		time(&now);
		timestamp = ctime(&now) + 4;
		timestamp[15] = '\0';
	} else {
		timestamp = msg;
		timestamp[15] = '\0';
		msg += 16;
	}
#else
	t = time(NULL);
	local = localtime(&t);
	sprintf(new_timestamp, "%d-%02d-%02d %02d:%02d:%02d", local->tm_year+1900,
		local->tm_mon+1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
	new_timestamp[19] = '\0';

	if (strlen(msg) < 16 || msg[3] != ' ' || msg[6] != ' ' ||
		msg[9] != ':' || msg[12] != ':' || msg[15] != ' ') {
		memcpy(tmp_msg, msg, strlen(msg));		
	}
	else {
		memcpy(tmp_msg, msg+16, strlen(msg)-16);	
	}

	/* only recort the specific log */
	if ((gAllSaveFlag != 1) && (strncmp(tmp_msg, "TCSysLog", 8) != 0))
		return;
		
	/* only record log under gLogWriteLevel */
	if (LOG_PRI(pri) > gLogWriteLevel)
		return;
	if (gAllSaveFlag != 1)
		memcpy(new_msg, tmp_msg+9, strlen(tmp_msg)-9);		
	else {
		if (strncmp(tmp_msg, "TCSysLog", 8) == 0) {
			memcpy(new_msg, tmp_msg+9, strlen(tmp_msg)-9);	
		}
		else {
			memcpy(new_msg, tmp_msg, strlen(tmp_msg));
		}
	}
#endif

	/* todo: supress duplicates */

#ifdef CONFIG_FEATURE_REMOTE_LOG
	if (doRemoteLog == TRUE) {
		/* trying connect the socket */
		if (-1 == remotefd) {
			init_RemoteLog();
		}

		/* if we have a valid socket, send the message */
		if (-1 != remotefd) {
			now = 1;
			snprintf(line, sizeof(line), "<%d> %s", pri, msg);

		retry:
			/* send message to remote logger */
			if(( -1 == sendto(remotefd, line, strlen(line), 0,
							(struct sockaddr *) &remoteaddr,
							sizeof(remoteaddr))) && (errno == EINTR)) {
				/* sleep now seconds and retry (with now * 2) */
				sleep(now);
				now *= 2;
				goto retry;
			}
		}
	}

	if (local_logging == TRUE)
#endif
	{
		/* now spew out the message to wherever it is supposed to go */
		if (small)
		#ifndef TCSUPPORT_SYSLOG_ENHANCE
			message("%s %s\n", timestamp, msg);
		#else
			message("%s %s\n", new_timestamp, new_msg);
		#endif
		else
		#ifndef TCSUPPORT_SYSLOG_ENHANCE
			message("%s %s %s %s\n", timestamp, LocalHostName, res, msg);
		#else
			message("%s %s %s\n", new_timestamp, res, new_msg);
		#endif

	}
}

static void quit_signal(int sig)
{
#ifdef TCSUPPORT_SYSLOG_ENHANCE
	char buf[8];
#endif
	char cmd[64] = {0};
	logMessage(LOG_SYSLOG | LOG_INFO, "TCSysLog System log daemon exiting.");
	unlink(lfile);
#ifdef CONFIG_FEATURE_IPC_SYSLOG
	ipcsyslog_cleanup();
#endif

#ifdef TCSUPPORT_SYSLOG_ENHANCE
#ifdef SYSLOG_DBG
	tcdbg_printf("\n===%s[%d]syslog exit gNewLogCnt = %d====\n", __FUNCTION__, __LINE__, gNewLogCnt);
#endif
	if (gNewLogCnt > 0) {
		sprintf(buf, "%d", gNewLogCnt);
		tcapi_set(SYSLOG_TO_FILE, "gNewLogCnt", buf);
		tcapi_commit(SYSLOG_TO_FILE);
		gNewLogCnt = 0;
	}
	tcapi_commit(SYSLOG_TO_FLASH);
#ifdef SYSLOG_DBG
	tcdbg_printf("\n===%s[%d]before exit====\n", __FUNCTION__, __LINE__);
#endif
	/* remove log file */
	unlink(currLogFilePath);
	unlink(logFilePath);
#endif

	sprintf(cmd, "echo syslogd catch signal:%d >/tmp/syslogd_reboot", sig);
	system(cmd);
	sprintf(cmd, "echo syslogd catch signal:%d >/dev/console", sig);
	system(cmd);
	exit(TRUE);
}

static void domark(int sig)
{
	if (MarkInterval > 0) {
		logMessage(LOG_SYSLOG | LOG_INFO, "-- MARK --");
		alarm(MarkInterval);
	}
}

/* This must be a #define, since when CONFIG_DEBUG and BUFFERS_GO_IN_BSS are
 * enabled, we otherwise get a "storage size isn't constant error. */
static int serveConnection(char *tmpbuf, int n_read)
{
	char *p = tmpbuf;

	while (p < tmpbuf + n_read) {

		int pri = (LOG_USER | LOG_NOTICE);
		int num_lt = 0;
		char line[MAXLINE + 1];
		unsigned char c;
		char *q = line;

		while ((c = *p) && q < &line[sizeof(line) - 1]) {
			if (c == '<' && num_lt == 0) {
				/* Parse the magic priority number. */
				num_lt++;
				pri = 0;
				while (isdigit(*(++p))) {
					pri = 10 * pri + (*p - '0');
				}
				if (pri & ~(LOG_FACMASK | LOG_PRIMASK)) {
					pri = (LOG_USER | LOG_NOTICE);
				}
			} else if (c == '\n') {
				*q++ = ' ';
			} else if (iscntrl(c) && (c < 0177)) {
				*q++ = '^';
				*q++ = c ^ 0100;
			} else {
				*q++ = c;
			}
			p++;
		}
		*q = '\0';
		p++;
		/* Now log it */
		logMessage(pri, line);
	}
	return n_read;
}

static void doSyslogd(void) __attribute__ ((noreturn));
static void doSyslogd(void)
{
	struct sockaddr_un sunx;
	socklen_t addrLength;

	int sock_fd;
	fd_set fds;

#ifdef TCSUPPORT_SYSLOG_ENHANCE
	struct timeval tv;
	int ret;
	struct timeval now;
	struct timeval update_file_last;
	struct timeval write_flash_last;
	int dval = 0;
	char writeFlag = 0;
	char buf[8];
	
	gettimeofday(&update_file_last, NULL);
	gettimeofday(&write_flash_last, NULL);
	gettimeofday(&now, NULL);
#endif

	/* Set up signal handlers. */
	signal(SIGINT, quit_signal);
	signal(SIGTERM, quit_signal);
	signal(SIGQUIT, quit_signal);
	signal(SIGHUP, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);
#ifdef SIGCLD
	signal(SIGCLD, SIG_IGN);
#endif
	signal(SIGALRM, domark);
	alarm(MarkInterval);

	/* Create the syslog file so realpath() can work. */
	if (realpath(_PATH_LOG, lfile) != NULL) {
		unlink(lfile);
	}

	memset(&sunx, 0, sizeof(sunx));
	sunx.sun_family = AF_UNIX;
	strncpy(sunx.sun_path, lfile, sizeof(sunx.sun_path));
	if ((sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
		bb_perror_msg_and_die("Couldn't get file descriptor for socket "
						   _PATH_LOG);
	}

	addrLength = sizeof(sunx.sun_family) + strlen(sunx.sun_path);
	if (bind(sock_fd, (struct sockaddr *) &sunx, addrLength) < 0) {
		bb_perror_msg_and_die("Could not connect to socket " _PATH_LOG);
	}

	if (chmod(lfile, 0666) < 0) {
		bb_perror_msg_and_die("Could not set permission on " _PATH_LOG);
	}
#ifdef CONFIG_FEATURE_IPC_SYSLOG
	if (circular_logging == TRUE) {
		ipcsyslog_init();
	}
#endif

#ifdef CONFIG_FEATURE_REMOTE_LOG
	if (doRemoteLog == TRUE) {
		init_RemoteLog();
	}
#endif

	logMessage(LOG_SYSLOG | LOG_INFO, "syslogd started: " BB_BANNER);

	for (;;) {
	#ifdef TCSUPPORT_SYSLOG_ENHANCE
		gettimeofday(&now, NULL);
		/*Check CPE time changed or not*/
		dval = now.tv_sec - update_file_last.tv_sec;
		if(abs(dval) >= TIMECHANGERANGE)	{
			/*Update last time*/
			update_file_last.tv_sec += dval;
			write_flash_last.tv_sec += dval;
		}

		/*Do update currLogFile*/
		if((now.tv_sec - update_file_last.tv_sec) >= gUpdateFileInterval) {
			if(gNewLogCnt > 0) {
				//printf("\n==>Syslog, do update_logFile<==\n");
				//update_logFile();
				sprintf(buf, "%d", gNewLogCnt);
				tcapi_set(SYSLOG_TO_FILE, "gNewLogCnt", buf);
				tcapi_commit(SYSLOG_TO_FILE);
				writeFlag = 1;
				gNewLogCnt = 0;
			}

			/*Update last timeval*/
			update_file_last = now;
		}
	
		/*Do write flash*/
		if((now.tv_sec - write_flash_last.tv_sec) >= gWriteFlashInterval) {
			if(writeFlag == 1) {
				printf("\n==>Syslog, do write_flash<==\n");
				//gFlashLogCnt = gCurrLogCnt;
	 			//write_logMsg_to_flash();
	 			tcapi_commit(SYSLOG_TO_FLASH);
				writeFlag = 0;
			}

			/*Update last timeval*/
			write_flash_last = now;
		}
	#endif

		FD_ZERO(&fds);
		FD_SET(sock_fd, &fds);

	#ifdef TCSUPPORT_SYSLOG_ENHANCE
		//timeout setting
		tv.tv_sec = SELECT_TIMEOUT;
		tv.tv_usec = 0;
	#endif

	#ifndef TCSUPPORT_SYSLOG_ENHANCE
		if (select(sock_fd + 1, &fds, NULL, NULL, NULL) < 0) 
	#else
		ret = select(sock_fd + 1, &fds, NULL, NULL, &tv);
		if (ret < 0) 
	#endif
		{
			if (errno == EINTR) {
				/* alarm may have happened. */
				continue;
			}
			bb_perror_msg_and_die("select error");
		}

		if (FD_ISSET(sock_fd, &fds)) {
			int i;

			RESERVE_CONFIG_BUFFER(tmpbuf, MAXLINE + 1);

			memset(tmpbuf, '\0', MAXLINE + 1);
			if ((i = recv(sock_fd, tmpbuf, MAXLINE, 0)) > 0) {
				serveConnection(tmpbuf, i);
			} else {
				bb_perror_msg_and_die("UNIX socket error");
			}
			RELEASE_CONFIG_BUFFER(tmpbuf);
		}				/* FD_ISSET() */
	}					/* for main loop */
}

extern int syslogd_main(int argc, char **argv)
{
	int opt;

	int doFork = TRUE;

	char *p;

	/* do normal option parsing */
	while ((opt = getopt(argc, argv, "f:m:nO:s:Sb:R:LC::")) > 0) {
		switch (opt) {
		case 'm':
			MarkInterval = atoi(optarg) * 60;
			break;
		case 'n':
			doFork = FALSE;
			break;
		case 'O':
			logFilePath = optarg;
			break;
#ifdef CONFIG_FEATURE_ROTATE_LOGFILE
		case 's':
			logFileSize = atoi(optarg) * 1024;
			break;
		case 'b':
			logFileRotate = atoi(optarg);
			if( logFileRotate > 99 ) logFileRotate = 99;
			break;
#endif
#ifdef CONFIG_FEATURE_REMOTE_LOG
		case 'R':
			RemoteHost = bb_xstrdup(optarg);
			if ((p = strchr(RemoteHost, ':'))) {
				RemotePort = atoi(p + 1);
				*p = '\0';
			}
			doRemoteLog = TRUE;
			break;
		case 'L':
			local_logging = TRUE;
			break;
#endif
#ifdef CONFIG_FEATURE_IPC_SYSLOG
		case 'C':
			if (optarg) {
				int buf_size = atoi(optarg);
				if (buf_size >= 4) {
					shm_size = buf_size * 1024;
				}
			}
			circular_logging = TRUE;
			break;
#endif
		case 'S':
			small = true;
			break;
		default:
			bb_show_usage();
		}
	}

#ifdef CONFIG_FEATURE_REMOTE_LOG
	/* If they have not specified remote logging, then log locally */
	if (doRemoteLog == FALSE)
		local_logging = TRUE;
#endif


	/* Store away localhost's name before the fork */
	gethostname(LocalHostName, sizeof(LocalHostName));
	if ((p = strchr(LocalHostName, '.'))) {
		*p = '\0';
	}

	umask(0);

	if (doFork == TRUE) {
#if defined(__uClinux__)
		vfork_daemon_rexec(0, 1, argc, argv, "-n");
#else /* __uClinux__ */
		if(daemon(0, 1) < 0)
			bb_perror_msg_and_die("daemon");
#endif /* __uClinux__ */
	}
#ifdef TCSUPPORT_SYSLOG_ENHANCE
	/*Read syslog config param, e.g. write level*/
	if(read_syslog_conf() < 0) {
		printf("\n==>Syslog Info, syslog conf file is not exist or empty, just use default parameter value\n");
	}

	/* make sure the last syslogd exit */
	sleep(1);
	if (tcapi_commit(SYSLOG_FROM_FLASH) < 0) {
	#ifdef SYSLOG_DBG
		tcdbg_printf("\n==>Syslog Error, Read log info and message from flash failed\n");
	#endif
		printf("\n==>Syslog Error, Read log info and message from flash failed\n");
		return EXIT_FAILURE;
	}

#endif

	doSyslogd();

	return EXIT_SUCCESS;
}

#endif
/*
Local Variables
c-file-style: "linux"
c-basic-offset: 4
tab-width: 4
End:
*/
