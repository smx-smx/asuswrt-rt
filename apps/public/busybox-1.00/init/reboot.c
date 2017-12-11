/* vi: set sw=4 ts=4: */
/*
 * Mini reboot implementation for busybox
 *
 * Copyright (C) 1999-2004 by Erik Andersen <andersen@codepoet.org>
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

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/reboot.h>
#include "busybox.h"
#include "init_shared.h"
#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>
#include <string.h>
#include <asm/unistd.h>
#include <asm/fcntl.h>

#if defined(TCSUPPORT_START_TRAP) || defined(TCSUPPORT_SYSLOG_ENHANCE)
#define		SIGNAL_PATH		"/var/tmp/signal_reboot"
#define		SIGNAL_PATH_SYSLOG		"/tmp/syslogd_reboot"
static int quit_signal(void)
{
	int ret;
	FILE *fp = NULL;
	char buf[8];

	memset(buf, 0, sizeof(buf));
	
	fp = fopen(SIGNAL_PATH, "r");

	if (fp == NULL)
		ret = 0;
	else {
		fgets(buf, 8, fp);
		
		if (buf[0] == '1')
			ret = 1;
		else 
			ret = 0;

		fclose(fp);
		unlink(SIGNAL_PATH);
	}

	return ret;
}

static int syslogd_quit_signal(void)
{
	int ret;
	FILE *fp = NULL;

	fp = fopen(SIGNAL_PATH_SYSLOG, "r");
	if (fp == NULL)
		ret = 0;
	else {
		ret = 1;
		fclose(fp);
		unlink(SIGNAL_PATH_SYSLOG);
		system("echo the syslogd is terminated by signal > /dev/console");
	}

	return ret;
}
#endif

int if_down(char* ifname)
{
	struct ifreq ifr;
	int fd;

	if(!ifname)
		return -1;
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	fd = socket(PF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		//_dprintf("%s: %d %s", __FUNCTION__, __LINE__, strerror(errno));
		return -1;
	}

	//down
	if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0) {
		//_dprintf("%s: %d SIOCGIFFLAGS: %s", __FUNCTION__, __LINE__, strerror(errno));
		close(fd);
		return -1;
	}
	ifr.ifr_flags &= ~IFF_UP;
	if (ioctl(fd, SIOCSIFFLAGS, &ifr) < 0) {
		//_dprintf("%s: %d SIOCSIFFLAGS: %s", __FUNCTION__, __LINE__, strerror(errno));
		close(fd);
		return -1;
	}
	return 0;
}

extern int reboot_main(int argc, char **argv)
{
	char *delay; /* delay in seconds before rebooting */
	unsigned long opt;
	int fw_upgrade = 0;
	
#if defined(TCSUPPORT_START_TRAP) || defined(TCSUPPORT_SYSLOG_ENHANCE)
	int count = 0;
#endif
	if((opt = bb_getopt_ulflags(argc, argv, "d:u", &delay)) > 0) {
		if (opt & 0x1)
			sleep(atoi(delay));

		if (opt & 0x2)
			fw_upgrade = 1;
	}
	
	system("killall -9 monitorcfgmgr");	//Andy Chiu, 2015/03/18. stop monitorcfgmgr before stop cfg_manager
	if (fw_upgrade)
	{
		system("killall -9 cfg_manager");
	}
	else
	{
		system("killall -SIGTERM cfg_manager");	//Andy Chiu, 2015/05/12. send SIGTERM to cfg_manager first and make sure it 
		/* wait cfg_manager done */
		while (!quit_signal() && count++ < 10)
			sleep(1);
	}
#ifdef TCSUPPORT_SYSLOG_ENHANCE
	count = 0;
	system("killall syslogd");
	/* wait syslogd storing system log */
	while (!syslogd_quit_signal() && count++ < 10)
		sleep(1);
#endif
#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520)
#if defined(MT7592)
	if_down("wds0");
	if_down("wds1");
	if_down("wds2");
	if_down("wds3");
	if_down("ra0");
	if_down("ra1");
	if_down("ra2");
	if_down("ra3");
	syscall(__NR_delete_module, "mt7603eap", O_NONBLOCK|O_EXCL);
#endif
#if defined(MT7612E)
	if_down("wdsi0");
	if_down("wdsi1");
	if_down("wdsi2");
	if_down("wdsi3");
	if_down("rai0");
	if_down("rai1");
	if_down("rai2");
	if_down("rai3");
	syscall(__NR_delete_module, "mt7662e_ap", O_NONBLOCK|O_EXCL);
#endif
#if defined(MT7610E)
	if_down("wdsi0");
	if_down("wdsi1");
	if_down("wdsi2");
	if_down("wdsi3");
	if_down("rai0");
	if_down("rai1");
	if_down("rai2");
	if_down("rai3");
	syscall(__NR_delete_module, "mt7610e_ap", O_NONBLOCK|O_EXCL);
#endif
#endif

#ifndef CONFIG_INIT
#ifndef RB_AUTOBOOT
#define RB_AUTOBOOT		0x01234567
#endif
	return(bb_shutdown_system(RB_AUTOBOOT));
#else
	return kill_init(SIGTERM);
#endif
}

/*
Local Variables:
c-file-style: "linux"
c-basic-offset: 4
tab-width: 4
End:
*/
