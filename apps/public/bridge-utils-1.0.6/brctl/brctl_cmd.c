/*
 * Copyright (C) 2000 Lennert Buytenhek
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <asm/param.h>
#include "libbridge.h"
#include "brctl.h"
#include <linux/version.h>


static int strtotimeval(struct timeval *tv, const char *time)
{
	double secs;
	if (sscanf(time, "%lf", &secs) != 1) 
		return -1;
	tv->tv_sec = secs;
	tv->tv_usec = 1000000 * (secs - tv->tv_sec);
	return 0;
}

static int br_cmd_addbr(int argc, char*const* argv)
{
	int err;

	switch (err = br_add_bridge(argv[1])) {
	case 0:
		return 0;

	case EEXIST:
		fprintf(stderr,	"device %s already exists; can't create "
			"bridge with the same name\n", argv[1]);
		return 1;
	default:
		fprintf(stderr, "add bridge failed: %s\n",
			strerror(err));
		return 1;
	}
}

static int br_cmd_delbr(int argc, char*const* argv)
{
	int err;

	switch (err = br_del_bridge(argv[1])){
	case 0:
		return 0;

	case ENXIO:
		fprintf(stderr, "bridge %s doesn't exist; can't delete it\n",
			argv[1]);
		return 1;

	case EBUSY:
		fprintf(stderr, "bridge %s is still up; can't delete it\n",
			argv[1]);
		return 1;

	default:
		fprintf(stderr, "can't delete bridge %s: %s\n",
			argv[1], strerror(err));
		return 1;
	}
}

static int br_cmd_addif(int argc, char *const* argv)
{
	const char *brname;
	int err;

	argc -= 2;
	brname = *++argv;

	while (argc-- > 0) {
		const char *ifname = *++argv;
		err = br_add_interface(brname, ifname);

		switch(err) {
		case 0:
			continue;

		case ENODEV:
			fprintf(stderr, "interface %s does not exist!\n", ifname);
			break;

		case EBUSY:
			fprintf(stderr,	"device %s is already a member of a bridge; "
				"can't enslave it to bridge %s.\n", ifname,
				brname);
			break;

		case ELOOP:
			fprintf(stderr, "device %s is a bridge device itself; "
				"can't enslave a bridge device to a bridge device.\n",
				ifname);
			break;

		default:
			fprintf(stderr, "can't add %s to bridge %s: %s\n",
				ifname, brname, strerror(err));
		}
		return 1;
	}
	return 0;
}

static int br_cmd_delif(int argc, char *const* argv)
{
	const char *brname;
	int err;

	argc -= 2;
	brname = *++argv;

	while (argc-- > 0) {
		const char *ifname = *++argv;
		err = br_del_interface(brname, ifname);
		switch (err) {
		case 0:
			continue;

		case ENODEV:
			fprintf(stderr, "interface %s does not exist!\n", 
				ifname);
			break;

		case EINVAL:
			fprintf(stderr, "device %s is not a slave of %s\n",
				ifname, brname);
			break;

		default:
			fprintf(stderr, "can't delete %s from %s: %s\n",
				ifname, brname, strerror(err));
		}
		return 1;
	}
	return 0;
}

static int br_cmd_setageing(int argc, char *const* argv)
{
	int err;
	struct timeval tv;
	
	if (strtotimeval(&tv, argv[2])) {
		fprintf(stderr, "bad ageing time value\n");
		return 1;
	}

	err = br_set_ageing_time(argv[1], &tv);
	if (err)
		fprintf(stderr, "set ageing time failed: %s\n",
			strerror(err));

	return err != 0;
}

static int br_cmd_setbridgeprio(int argc, char *const* argv)
{
	int prio;
	int err;

	if (sscanf(argv[2], "%i", &prio) != 1) {
		fprintf(stderr,"bad priority\n");
		return 1;
	}

	err = br_set_bridge_priority(argv[1], prio);
	if (err)
		fprintf(stderr, "set bridge priority failed: %s\n",
			strerror(err));
	return err != 0;
}

static int br_cmd_setfd(int argc, char *const* argv)
{
	struct timeval tv;
	int err;

	if (strtotimeval(&tv, argv[2])) {
		fprintf(stderr, "bad forward delay value\n");
		return 1;
	}

	err = br_set_bridge_forward_delay(argv[1], &tv);
	if (err)
		fprintf(stderr, "set forward delay failed: %s\n",
			strerror(err));

	return err != 0;
}

static int br_cmd_sethello(int argc, char *const* argv)
{
	struct timeval tv;
	int err;

	if (strtotimeval(&tv, argv[2])) {
		fprintf(stderr, "bad hello timer value\n");
		return 1;
	}

	err = br_set_bridge_hello_time(argv[1], &tv);
	if (err)
		fprintf(stderr, "set hello timer failed: %s\n",
			strerror(err));

	return err != 0;
}

static int br_cmd_setmaxage(int argc, char *const* argv)
{
	struct timeval tv;
	int err;

	if (strtotimeval(&tv, argv[2])) {
		fprintf(stderr, "bad max age value\n");
		return 1;
	}
	err = br_set_bridge_max_age(argv[1], &tv);
	if (err)
		fprintf(stderr, "set max age failed: %s\n",
			strerror(err));

	return err != 0;
}

static int br_cmd_setpathcost(int argc, char *const* argv)
{
	int cost, err;

	if (sscanf(argv[3], "%i", &cost) != 1) {
		fprintf(stderr, "bad path cost value\n");
		return 1;
	}

	err = br_set_path_cost(argv[1], argv[2], cost);
	if (err)
		fprintf(stderr, "set path cost failed: %s\n",
			strerror(err));
	return err != 0;
}

static int br_cmd_setportprio(int argc, char *const* argv)
{
	int cost, err;

	if (sscanf(argv[3], "%i", &cost) != 1) {
		fprintf(stderr, "bad path priority value\n");
		return 1;
	}

	err = br_set_path_cost(argv[1], argv[2], cost);
	if (err)
		fprintf(stderr, "set port priority failed: %s\n",
			strerror(errno));

	return err != 0;
}

static int br_cmd_stp(int argc, char *const* argv)
{
	int stp, err;

	if (!strcmp(argv[2], "on") || !strcmp(argv[2], "yes") 
	    || !strcmp(argv[2], "1"))
		stp = 1;
	else if (!strcmp(argv[2], "off") || !strcmp(argv[2], "no") 
		 || !strcmp(argv[2], "0"))
		stp = 0;
	else {
		fprintf(stderr, "expect on/off for argument\n");
		return 1;
	}

	err = br_set_stp_state(argv[1], stp);
	if (err)
		fprintf(stderr, "set stp status failed: %s\n", 
			strerror(errno));
	return err != 0;
}

static int br_cmd_showstp(int argc, char *const* argv)
{
	struct bridge_info info;

	if (br_get_bridge_info(argv[1], &info)) {
		fprintf(stderr, "%s: can't get info %s\n", argv[1],
			strerror(errno));
		return 1;
	}

	br_dump_info(argv[1], &info);
	return 0;
}

static int show_bridge(const char *name, void *arg)
{
	struct bridge_info info;

	printf("%s\t\t", name);
	fflush(stdout);

	if (br_get_bridge_info(name, &info)) {
		fprintf(stderr, "can't get info %s\n",
				strerror(errno));
		return 1;
	}

	br_dump_bridge_id((unsigned char *)&info.bridge_id);
	printf("\t%s\t\t", info.stp_enabled?"yes":"no");

	br_dump_interface_list(name);
	return 0;
}

static int br_cmd_show(int argc, char *const* argv)
{
	printf("bridge name\tbridge id\t\tSTP enabled\tinterfaces\n");
	br_foreach_bridge(show_bridge, NULL);
	return 0;
}

static int compare_fdbs(const void *_f0, const void *_f1)
{
	const struct fdb_entry *f0 = _f0;
	const struct fdb_entry *f1 = _f1;

	return memcmp(f0->mac_addr, f1->mac_addr, 6);
}

static int br_cmd_showmacs(int argc, char *const* argv)
{
	const char *brname = argv[1];
#define CHUNK 128
	int i, n;
	struct fdb_entry *fdb = NULL;
	int offset = 0;

	for(;;) {
		fdb = realloc(fdb, (offset + CHUNK) * sizeof(struct fdb_entry));
		if (!fdb) {
			fprintf(stderr, "Out of memory\n");
			return 1;
		}
			
		n = br_read_fdb(brname, fdb+offset, offset, CHUNK);
		if (n == 0)
			break;

		if (n < 0) {
			fprintf(stderr, "read of forward table failed: %s\n",
				strerror(errno));
			return 1;
		}

		offset += n;
	}

	qsort(fdb, offset, sizeof(struct fdb_entry), compare_fdbs);

	printf("port no\tmac addr\t\tis local?\tageing timer\n");
	for (i = 0; i < offset; i++) {
		const struct fdb_entry *f = fdb + i;
		printf("%3i\t", f->port_no);
		printf("%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\t",
		       f->mac_addr[0], f->mac_addr[1], f->mac_addr[2],
		       f->mac_addr[3], f->mac_addr[4], f->mac_addr[5]);
		printf("%s\t\t", f->is_local?"yes":"no");
		br_show_timer(&f->ageing_timer_value);
		printf("\n");
	}
	return 0;
}

#ifdef TCSUPPORT_IGMP_SNOOPING
static int br_cmd_igmpsnoop(int argc, char *const* argv)
{
	int igmpsnoop, err;

	if (!strcmp(argv[2], "on") || !strcmp(argv[2], "yes") 
	    || !strcmp(argv[2], "1"))
		igmpsnoop = 1;
	else if (!strcmp(argv[2], "off") || !strcmp(argv[2], "no") 
		 || !strcmp(argv[2], "0"))
		igmpsnoop = 0;
	else {
		fprintf(stderr, "expect on/off for argument\n");
		return 1;
	}

	err = br_set_igmpsnoop_state(argv[1], igmpsnoop);
	if (err)
		fprintf(stderr, "set igmpsnoop status failed: %s\n", 
			strerror(errno));
	return err != 0;
}

static int br_cmd_set_igmpsnoop_ageing(int argc, char *const* argv)
{
	int err;
	struct timeval tv;
	
	if (strtotimeval(&tv, argv[2])) {
		fprintf(stderr, "bad ageing time value\n");
		return 1;
	}

	err = br_set_igmpsnoop_ageing_time(argv[1], &tv);
	if (err)
		fprintf(stderr, "set ageing time failed: %s\n",
			strerror(err));

	return err != 0;
}

static int br_cmd_quickleave(int argc, char *const* argv)
{
	int quickleave, err;

	if (!strcmp(argv[2], "on") || !strcmp(argv[2], "yes") 
	    || !strcmp(argv[2], "1"))
		quickleave = 1;
	else if (!strcmp(argv[2], "off") || !strcmp(argv[2], "no") 
		 || !strcmp(argv[2], "0"))
		quickleave = 0;
	else {
		fprintf(stderr, "expect on/off for argument\n");
		return 1;
	}

	err = br_set_igmpsnoop_quickleave(argv[1], quickleave);
	if (err)
		fprintf(stderr, "set igmpsnoop quickleave failed: %s\n", 
			strerror(errno));
	return err != 0;
}

static int br_cmd_routeportflag(int argc, char *const* argv)
{
	int routePortFlag, err;

	if (!strcmp(argv[2], "on") || !strcmp(argv[2], "yes") 
	    || !strcmp(argv[2], "1"))
		routePortFlag = 1;
	else if (!strcmp(argv[2], "off") || !strcmp(argv[2], "no") 
		 || !strcmp(argv[2], "0"))
		routePortFlag = 0;
	else {
		fprintf(stderr, "expect on/off for argument\n");
		return 1;
	}

	err = br_set_igmpsnoop_routeportflag(argv[1], routePortFlag);
	if (err)
		fprintf(stderr, "set igmpsnoop routeportflag failed: %s\n", 
			strerror(errno));
	return err != 0;
}

static int br_cmd_igmpdbg(int argc, char *const* argv)
{
	int igmpdbg, err;

	if (!strcmp(argv[2], "on") || !strcmp(argv[2], "yes") 
	    || !strcmp(argv[2], "1"))
		igmpdbg = 1;
	else if (!strcmp(argv[2], "off") || !strcmp(argv[2], "no") 
		 || !strcmp(argv[2], "0"))
		igmpdbg = 0;
	else {
		fprintf(stderr, "expect on/off for argument\n");
		return 1;
	}

	err = br_set_igmpsnoop_dbg(argv[1], igmpdbg);
	if (err)
		fprintf(stderr, "set igmpsnoop dbg failed: %s\n", 
			strerror(errno));
	return err != 0;
}

static int dump_igmp_port_info(const char *br, const char *p,  void *arg)
{
	struct port_info pinfo;

	if (br_get_port_info(br, p, &pinfo)) {
		printf("Can't get info for %p",p);
		return 1;
	}

	printf("%s (%d)\t\tis_router: %s\n", p, pinfo.port_no, pinfo.is_router?"yes":"no");
	return 0;
}

void br_dump_igmp_info(const char *br, const struct bridge_info *bri)
{
	int err;

	err = br_foreach_port(br, dump_igmp_port_info, NULL);
	if (err < 0)
		printf("can't get ports: %s\n", strerror(-err));
}

static int br_cmd_showigmp(int argc, char *const* argv)
{
	const char *brname = argv[1];
#define CHUNK 128
	int i, n;
	struct mc_fdb_entry *fdb = NULL;
	int offset = 0;
	struct bridge_info info;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
		u_int8_t group_addr_t[64] = {0};
#endif

	if (br_get_bridge_info(argv[1], &info)) {
		fprintf(stderr, "%s: can't get info %s\n", argv[1],
			strerror(errno));
		return 1;
	}
	
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	printf("IGMP snooping enabled: %s\n", info.igmpsnoop_enabled?"yes":"no");
#else
	printf("Snooping enabled: %s\n", info.igmpsnoop_enabled?"yes":"no");
#endif
	printf("quickleave: %s\n", info.igmpsnoop_quickleave?"yes":"no");
#if !defined(TCSUPPORT_IGMPSNOOPING_ENHANCE)
	printf("routeportflag: %s\n", info.igmpsnoop_routeportflag?"on":"off");
#endif
	printf("debug: %s\n", info.igmpsnoop_dbg?"yes":"no");
	printf("ageing time=");
	br_show_timer(&info.igmpsnoop_ageing_time);
	printf("\n\n");

	br_dump_igmp_info(argv[1], &info);
	printf("\n");

	for(;;) {
		fdb = realloc(fdb, (offset + CHUNK) * sizeof(struct mc_fdb_entry));
		if (!fdb) {
			fprintf(stderr, "Out of memory\n");
			return 1;
		}
			
		n = br_read_mc_fdb(brname, fdb+offset, offset, CHUNK);
		if (n == 0)
			break;

		if (n < 0) {
			fprintf(stderr, "read of forward table failed: %s\n",
				strerror(errno));
			return 1;
		}

		offset += n;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
		if(n < CHUNK)
			break;
#endif
	}

	qsort(fdb, offset, sizeof(struct mc_fdb_entry), compare_fdbs);

#ifdef TCSUPPORT_IGMP_SNOOPING_V3
	printf("port no  group addr   host addr     src addr    filter mode  ageing timer\n");
#else
	printf("port no\tgroup addr\t\treporter\t\tageing timer\n");
#endif
	for (i = 0; i < offset; i++) {
		const struct mc_fdb_entry *f = fdb + i;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
		memset(group_addr_t, 0, 64);
#endif

	#ifdef TCSUPPORT_IGMP_SNOOPING_V3
		printf("%3i", f->port_no);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
		if(f->version == 6)
		{
			printf(" [%s]  ", f->group_addr);
			printf(" [%s]   ", f->src_addr);
			printf("%2s  ",(f->filter_mode == MCAST_EXCLUDE)?"EX":"IN");
			printf("\thostMac:%.2x:%.2x:%.2x:%.2x:%.2x:%.2x ",
			       f->host_addr[0], f->host_addr[1], f->host_addr[2],
			       f->host_addr[3], f->host_addr[4], f->host_addr[5]);
			printf("\tgroupMac:%.2x:%.2x:%.2x:%.2x:%.2x:%.2x ",
			       f->group_mac[0], f->group_mac[1], f->group_mac[2],
			       f->group_mac[3], f->group_mac[4], f->group_mac[5]);
		}
		else{
#endif
		printf("%16s  ", f->group_addr);
		printf("%.2x:%.2x:%.2x:%.2x:%.2x:%.2x ",
		       f->host_addr[0], f->host_addr[1], f->host_addr[2],
		       f->host_addr[3], f->host_addr[4], f->host_addr[5]);
		printf("%16s ", f->src_addr);
		printf("%2s     ",(f->filter_mode == MCAST_EXCLUDE)?"EX":"IN");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36)
		}
#endif
	#else
		printf("%3i\t", f->port_no);
		printf("%16s\t", f->group_addr);
		printf("%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\t",
		       f->host_addr[0], f->host_addr[1], f->host_addr[2],
		       f->host_addr[3], f->host_addr[4], f->host_addr[5]);
	#endif
		br_show_timer(&f->ageing_timer_value);
		printf("\n");
	}
	return 0;
}

#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
#ifdef TCSUPPORT_MLD_SNOOPING
static int br_cmd_mldsnooping(int argc, char *const* argv)
{
	int mldsnooping = 1, err;	

	if (!strcmp(argv[2], "on") || !strcmp(argv[2], "yes") 
	    || !strcmp(argv[2], "1"))
		mldsnooping = 1;
	else if (!strcmp(argv[2], "off") || !strcmp(argv[2], "no") 
		 || !strcmp(argv[2], "0"))
		mldsnooping = 0;
	else if (!strcmp(argv[2], "show")){
		err = br_mldsnooping_show(argv[1]);
		if(err)
			fprintf(stderr, "show mld snooping state failed\n");

		return err != 0;
	}
	else {
		fprintf(stderr, "expect on/off for argument\n");
		return 1;
	}
	
	err = br_mldsnooping_set_state(argv[1], mldsnooping);
	if(err)
		fprintf(stderr, "set mld snooping state failed\n");

	return err != 0;
}

static int br_cmd_mldsnooping_set_age(int argc, char *const* argv)
{
	int err;
	struct timeval tv;
	
	if(strtotimeval(&tv, argv[2])){
		fprintf(stderr, "bad ageing time value\n");
	}
	
	err = br_mldsnooping_set_age(argv[1], &tv);
	if(err)
		fprintf(stderr, "set mld snooping age time failed\n");

	return err != 0;
}
#endif
#endif
static const struct command commands[] = {
	{ 1, "addbr", br_cmd_addbr, "<bridge>\t\tadd bridge" },
	{ 1, "delbr", br_cmd_delbr, "<bridge>\t\tdelete bridge" },
	{ 2, "addif", br_cmd_addif, 
	  "<bridge> <device>\tadd interface to bridge" },
	{ 2, "delif", br_cmd_delif,
	  "<bridge> <device>\tdelete interface from bridge" },
	{ 2, "setageing", br_cmd_setageing,
	  "<bridge> <time>\t\tset ageing time" },
	{ 2, "setbridgeprio", br_cmd_setbridgeprio,
	  "<bridge> <prio>\t\tset bridge priority" },
	{ 2, "setfd", br_cmd_setfd,
	  "<bridge> <time>\t\tset bridge forward delay" },
	{ 2, "sethello", br_cmd_sethello,
	  "<bridge> <time>\t\tset hello time" },
	{ 2, "setmaxage", br_cmd_setmaxage,
	  "<bridge> <time>\t\tset max message age" },
	{ 3, "setpathcost", br_cmd_setpathcost, 
	  "<bridge> <port> <cost>\tset path cost" },
	{ 3, "setportprio", br_cmd_setportprio,
	  "<bridge> <port> <prio>\tset port priority" },
	{ 0, "show", br_cmd_show, "\t\t\tshow a list of bridges" },
	{ 1, "showmacs", br_cmd_showmacs, 
	  "<bridge>\t\tshow a list of mac addrs"},
	{ 1, "showstp", br_cmd_showstp, 
	  "<bridge>\t\tshow bridge stp info"},
	{ 2, "stp", br_cmd_stp,
	  "<bridge> {on|off}\tturn stp on/off" },
#ifdef TCSUPPORT_IGMP_SNOOPING
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
	{ 2, "igmpsnoop", br_cmd_igmpsnoop,
	  "<bridge> {on|off}\tturn igmpsnoop on/off" },
	{ 2, "igmpsnoopageing", br_cmd_set_igmpsnoop_ageing,
	  "<bridge> <time>\t\tset igmpsnoop ageing time" },
	{ 2, "quickleave", br_cmd_quickleave,
	  "<bridge> {on|off}\tturn igmpsnoop quickleave on/off" },
	#if !defined(TCSUPPORT_IGMPSNOOPING_ENHANCE)
	{ 2, "routeportflag", br_cmd_routeportflag,
	  "<bridge> {on|off}\tturn igmpsnoop routeportflag on/off" },
	#endif
	{ 2, "igmpdbg", br_cmd_igmpdbg,
	  "<bridge> {on|off}\tturn igmpsnoop dbg on/off" },
	{ 1, "showigmp", br_cmd_showigmp, 
	  "<bridge>\t\tshow a list of multicast addrs"},
#else
{ 2, "snoop", br_cmd_igmpsnoop,
  "<bridge> {on|off}\tturn snoop on/off" },
{ 2, "snoopageing", br_cmd_set_igmpsnoop_ageing,
  "<bridge> <time>\t\tset snoop ageing time" },
{ 2, "quickleave", br_cmd_quickleave,
  "<bridge> {on|off}\tturn snoop quickleave on/off" },
#if !defined(TCSUPPORT_IGMPSNOOPING_ENHANCE)
{ 2, "routeportflag", br_cmd_routeportflag,
  "<bridge> {on|off}\tturn igmpsnoop routeportflag on/off" },
#endif
{ 2, "snoopdbg", br_cmd_igmpdbg,
  "<bridge> {on|off}\tturn snoop dbg on/off" },
{ 1, "showsnoop", br_cmd_showigmp, 
  "<bridge>\t\tshow a list of multicast addrs"},

#endif
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
#ifdef TCSUPPORT_MLD_SNOOPING
	{ 2, "mldsnooping", br_cmd_mldsnooping,
	  "<bridge> {on|off}\tturn mldsnooping on/off" },
	{ 2, "mldsnoopingage", br_cmd_mldsnooping_set_age,
	  "<bridge> <time>\tset mldsnooping age time" },
#endif
#endif
};

const struct command *command_lookup(const char *cmd)
{
	int i;

	for (i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
		if (!strcmp(cmd, commands[i].name))
			return &commands[i];
	}

	return NULL;
}

void command_helpall(void)
{
	int i;

	for (i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
		printf("\t%-10s\t%s\n", commands[i].name, commands[i].help);
	}
}
