/*

	Tomato Firmware
	Copyright (C) 2006-2009 Jonathan Zarate

*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>

#include "shared.h"


//# cat /proc/1/stat
//1 (init) S 0 0 0 0 -1 256 287 10043 109 21377 7 110 473 1270 9 0 0 0 27 1810432 126 2147483647 4194304 4369680 2147450688 2147449688 717374852 0 0 0 514751 2147536844 0 0 0 0

char *psname(int pid, char *buffer, int maxlen)
{
	char buf[512];
	char path[64];
	char *p;

	if (maxlen <= 0) return NULL;
	*buffer = 0;
	sprintf(path, "/proc/%d/stat", pid);
	if ((f_read_string(path, buf, sizeof(buf)) > 4) && ((p = strrchr(buf, ')')) != NULL)) {
		*p = 0;
		if (((p = strchr(buf, '(')) != NULL) && (atoi(buf) == pid)) {
			strlcpy(buffer, p + 1, maxlen);
		}
	}
	return buffer;
}

/* There is a race condition when a brand new daemon starts up using the double-fork method.
 *   Example: dnsmasq
 * There are 2 windows of vulnerability.
 * 1) At the beginning of process startup, the new process has the wrong name, such as "init" because
 * init forks a child which execve()'s dnsmasq, but the execve() hasn't happened yet.
 * 2) At the end of process startup, the timing can be such that we don't see the long-lived process,
 * only the pid(s) of the short-lived process(es), but the psname fails because they've exited by then.
 *
 * The 1st can be covered by a retry after a slight delay.
 * The 2nd can be covered by a retry immediately.
 */
static int _pidof(const char *name, pid_t **pids)
{
	const char *p;
	char *e;
	DIR *dir;
	struct dirent *de;
	pid_t i;
	int count = 0;
	char buf[256];

	count = 0;
	if (pids != NULL)
		*pids = NULL;
	if ((p = strrchr(name, '/')) != NULL) name = p + 1;
	if ((dir = opendir("/proc")) != NULL) {
		while ((de = readdir(dir)) != NULL) {
			i = strtol(de->d_name, &e, 10);
			if (*e != 0) continue;
			if (strcmp(name, psname(i, buf, sizeof(buf))) == 0) {
				if (pids == NULL) {
					count = i;
					break;
				}
				if ((*pids = realloc(*pids, sizeof(pid_t) * (count + 1))) == NULL) {
					return -1;
				}
				(*pids)[count++] = i;
			}
		}
	}
	closedir(dir);
	return count;
}

/* If we hit both windows, it will take three tries to discover the pid. */
// 0: none, -1: failed
int pidof(const char *name)
{
	pid_t p;

	p = _pidof(name, NULL);
	if (p < 1) {
		usleep(10 * 1000);
		p = _pidof(name, NULL);
		if (p < 1)
			p = _pidof(name, NULL);
	}
	//if (p < 1)
		//return -1;
	return p;
}

int killall(const char *name, int sig)
{
	pid_t *pids;
	int i;
	int r;

	if ((i = _pidof(name, &pids)) > 0) {
		r = 0;
		do {
			r |= kill(pids[--i], sig);
		} while (i > 0);
		free(pids);
		return r;
	}
	return -2;
}

void killall_tk(const char *name)
{
	int n;

	if (killall(name, SIGTERM) == 0) {
		n = 10;
		while ((killall(name, 0) == 0) && (n-- > 0)) {
			_dprintf("%s: waiting name=%s n=%d\n", __FUNCTION__, name, n);
			usleep(100 * 1000);
		}
		if (n < 0) {
			n = 10;
			while ((killall(name, SIGKILL) == 0) && (n-- > 0)) {
				_dprintf("%s: SIGKILL name=%s n=%d\n", __FUNCTION__, name, n);
				usleep(100 * 1000);
			}
		}
	}
}

void kill_pidfile_tk(const char *pidfile)
{
	FILE *fp;
	char buf[256];
	pid_t pid = 0;
	int n;

	if ((fp = fopen(pidfile, "r")) != NULL) {
		if (fgets(buf, sizeof(buf), fp) != NULL)
			pid = strtoul(buf, NULL, 0);
		fclose(fp);
	}

	if (pid > 1 && kill(pid, SIGTERM) == 0) {
		n = 10;
		while ((kill(pid, 0) == 0) && (n-- > 0)) {
			_dprintf("%s: waiting pid=%d n=%d\n", __FUNCTION__, pid, n);
			usleep(100 * 1000);
		}
		if (n < 0) {
			n = 10;
			while ((kill(pid, SIGKILL) == 0) && (n-- > 0)) {
				_dprintf("%s: SIGKILL pid=%d n=%d\n", __FUNCTION__, pid, n);
				usleep(100 * 1000);
			}
		}
	}
}

