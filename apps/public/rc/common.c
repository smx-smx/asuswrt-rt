#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdarg.h>
#include <arpa/inet.h>
#include <signal.h>
#include <shared.h>
#include "rc.h"
#include "libtcapi.h"

/* remove space in the end of string */
char *trim_r(char *str)
{
	int i;

	if (!str || !*str)
		return str;

	i = strlen(str) - 1;
	while ((i >= 0) && (str[i] == ' ' || str[i] == '\n' || str[i] == '\r'))
		str[i--] = 0;

	return str;
}

int pppstatus(void)
{
	FILE *fp;
	char sline[128], buf[128], *p;

	if ((fp=fopen("/tmp/wanstatus.log", "r")) && fgets(sline, sizeof(sline), fp))
	{
		p = strstr(sline, ",");
		strcpy(buf, p+1);
	}
	else
	{
		strcpy(buf, "unknown reason");
	}

	if(fp) fclose(fp);

	if(strstr(buf, "No response from ISP.")) return WAN_STOPPED_REASON_PPP_NO_ACTIVITY;
	else if(strstr(buf, "Failed to authenticate ourselves to peer")) return WAN_STOPPED_REASON_PPP_AUTH_FAIL;
	else if(strstr(buf, "Terminating connection due to lack of activity")) return WAN_STOPPED_REASON_PPP_LACK_ACTIVITY;
	else return WAN_STOPPED_REASON_NONE;
}

void usage_exit(const char *cmd, const char *help)
{
	fprintf(stderr, "Usage: %s %s\n", cmd, help);
	exit(1);
}

void logmessage(char *logheader, char *fmt, ...)
{
	va_list args;
	char buf[512];

	va_start(args, fmt);

	vsnprintf(buf, sizeof(buf), fmt, args);
	openlog(logheader, 0, 0);
	syslog(6, buf);
	closelog();
	va_end(args);
}

int insertmod(char *mod_full_path)
{
	if(mod_full_path)
		return eval("insmod", mod_full_path);
	else
		return -1;
}

int removemod(char *mod_name)
{
	if(mod_name)
		return eval("rmmod", mod_name);
	else
		return -1;
}

int modprobe_r(const char *mod)
{
	return eval("modprobe", "-r", (char *)mod);
}

#ifndef ct_modprobe
#ifdef LINUX26
#define ct_modprobe(mod, args...) ({ \
		modprobe("nf_conntrack_"mod, ## args); \
		modprobe("nf_nat_"mod); \
})
#else
#define ct_modprobe(mod, args...) ({ \
		modprobe("ip_conntrack_"mod, ## args); \
		modprobe("ip_nat_"mod, ## args); \
})
#endif
#endif

#ifndef ct_modprobe_r
#ifdef LINUX26
#define ct_modprobe_r(mod) ({ \
	modprobe_r("nf_nat_"mod); \
	modprobe_r("nf_conntrack_"mod); \
})
#else
#define ct_modprobe_r(mod) ({ \
	modprobe_r("ip_nat_"mod); \
	modprobe_r("ip_conntrack_"mod); \
})
#endif
#endif

/* 
 * The various child job starting functions:
 * _eval()
 *	Start the child. If ppid param is NULL, wait until the child exits.
 *	Otherwise, store the child's pid in ppid and return immediately.
 * eval()
 *	Call _eval with a NULL ppid, to wait for the child to exit.
 * xstart()
 *	Call _eval with a garbage ppid (to not wait), then return.
 * runuserfile
 *	Execute each executable in a directory that has the specified extention.
 *	Call _eval with a ppid (to not wait), then check every second for the child's pid.
 *	After wtime seconds or when the child has exited, return.
 *	If any such filename has an '&' character in it, then do *not* wait at
 *	all for the child to exit, regardless of the wtime.
 */

int _xstart(const char *cmd, ...)
{
	va_list ap;
	char *argv[16];
	int argc;
	int pid;

	argv[0] = (char *)cmd;
	argc = 1;
	va_start(ap, cmd);
	while ((argv[argc++] = va_arg(ap, char *)) != NULL) {
		//
	}
	va_end(ap);

	return _eval(argv, NULL, 0, &pid);
}



static void write_ct_timeout(const char *type, const char *name, unsigned int val)
{
	char buf[128];
	char v[16];

	sprintf(buf, "/proc/sys/net/ipv4/netfilter/ip_conntrack_%s_timeout%s%s",
		type, (name && name[0]) ? "_" : "", name ? name : "");
	sprintf(v, "%u", val);

	f_write_string(buf, v, 0, 0);
}

#ifndef write_tcp_timeout
#define write_tcp_timeout(name, val) write_ct_timeout("tcp", name, val)
#endif

#ifndef write_udp_timeout
#define write_udp_timeout(name, val) write_ct_timeout("udp", name, val)
#endif

static unsigned int read_ct_timeout(const char *type, const char *name)
{
	char buf[128];
	unsigned int val = 0;
	char v[16];

	sprintf(buf, "/proc/sys/net/ipv4/netfilter/ip_conntrack_%s_timeout%s%s",
		type, (name && name[0]) ? "_" : "", name ? name : "");
	if (f_read_string(buf, v, sizeof(v)) > 0)
		val = atoi(v);

	return val;
}

#ifndef read_tcp_timeout
#define read_tcp_timeout(name) read_ct_timeout("tcp", name)
#endif

#ifndef read_udp_timeout
#define read_udp_timeout(name) read_ct_timeout("udp", name)
#endif

void setup_udp_timeout(int connflag) 
{
	unsigned int v[10];
	const char *p;
	char buf[70];

	if (connflag
// #ifdef RTCONFIG_WIRELESSREPEATER
			// && nvram_get_int("sw_mode")!=SW_MODE_REPEATER
// #endif
	) {
		tcapi_get("SysInfo_Entry", "ct_udp_timeout", buf);
		p = buf;
		// p = nvram_safe_get("ct_udp_timeout");
		if (sscanf(p, "%u%u", &v[0], &v[1]) == 2) {
			write_udp_timeout(NULL, v[0]);
			write_udp_timeout("stream", v[1]);
		}
		else {
			v[0] = read_udp_timeout(NULL);
			v[1] = read_udp_timeout("stream");
			sprintf(buf, "%u %u", v[0], v[1]);
			// nvram_set("ct_udp_timeout", buf);
			tcapi_set("SysInfo_Entry", "ct_udp_timeout", buf);
		}
	}
	else {
		write_udp_timeout(NULL, 1);
		write_udp_timeout("stream", 6); 
	}
}

int scan_icmp_unreplied_conntrack()
{
	FILE *fp = fopen( "/proc/net/nf_conntrack", "r" );
	char buff[1024], ipv[16], ipv_num[16], protocol[16], protocol_num[16];
	int found = 0;

	if (fp == NULL) {
		perror( "openning /proc/net/nf_conntrack" );
		return -1;
	}

	while (fgets(buff, sizeof(buff), fp) != NULL) {
		sscanf(buff, "%s %s %s %s", ipv, ipv_num, protocol, protocol_num);

		if (memcmp(protocol, "icmp", 4) || !strstr(buff, "UNREPLIED")) continue;

//		dbG("\n%s %s %s %s\n", ipv, ipv_num, protocol, protocol_num);

		found++;
		break;
	}

	fclose(fp);

//	if (!found)
//		dbG("No matching conntrack found\n");

	return found;
}

void setup_ct_timeout(int connflag)
{
	unsigned int v[10];
	const char *p;
	char buf[70];
	int i;

	if (connflag
// #ifdef RTCONFIG_WIRELESSREPEATER
			// && nvram_get_int("sw_mode")!=SW_MODE_REPEATER
// #endif
	) {
		tcapi_get("SysInfo_Entry", "ct_udp_timeout", buf);
		p = buf;
		// p = nvram_safe_get("ct_timeout");
		if (sscanf(p, "%u%u", &v[0], &v[1]) == 2) {
//			write_ct_timeout("generic", NULL, v[0]);
			write_ct_timeout("icmp", NULL, v[1]);
		}
		else {
			v[0] = read_ct_timeout("generic", NULL);
			v[1] = read_ct_timeout("icmp", NULL);

			sprintf(buf, "%u %u", v[0], v[1]);
			// nvram_set("ct_timeout", buf);
			tcapi_set("SysInfo_Entry", "ct_udp_timeout", buf);
		}
	}
	else {
		for (i = 0; i < 3; i++)
		{
			if (scan_icmp_unreplied_conntrack() > 0)
			{
//				write_ct_timeout("generic", NULL, 0);
				write_ct_timeout("icmp", NULL, 0);
				sleep(2);
			}
		}
	}
}

void setup_conntrack(void)
{
	unsigned int v[10];
	char p[128] = {0};
	char buf[70];
	int i;
	char *pch;

	// p = nvram_safe_get("ct_tcp_timeout");
	tcapi_get("SysInfo_Entry", "ct_tcp_timeout", p);
	if (sscanf(p, "%u%u%u%u%u%u%u%u%u%u",
		&v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7], &v[8], &v[9]) == 10) {	// lightly verify
		write_tcp_timeout("established", v[1]);
		write_tcp_timeout("syn_sent", v[2]);
		write_tcp_timeout("syn_recv", v[3]);
		write_tcp_timeout("fin_wait", v[4]);
		write_tcp_timeout("time_wait", v[5]);
		write_tcp_timeout("close", v[6]);
		write_tcp_timeout("close_wait", v[7]);
		write_tcp_timeout("last_ack", v[8]);
	}
	else {
		v[1] = read_tcp_timeout("established");
		v[2] = read_tcp_timeout("syn_sent");
		v[3] = read_tcp_timeout("syn_recv");
		v[4] = read_tcp_timeout("fin_wait");
		v[5] = read_tcp_timeout("time_wait");
		v[6] = read_tcp_timeout("close");
		v[7] = read_tcp_timeout("close_wait");
		v[8] = read_tcp_timeout("last_ack");
		sprintf(buf, "0 %u %u %u %u %u %u %u %u 0",
			v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8]);
		// nvram_set("ct_tcp_timeout", buf);
		tcapi_set("SysInfo_Entry", "ct_tcp_timeout", buf);
	}

	setup_udp_timeout(FALSE);

	// p = nvram_safe_get("ct_timeout");
	tcapi_get("SysInfo_Entry", "ct_timeout", p);
	if (sscanf(p, "%u%u", &v[0], &v[1]) == 2) {
//		write_ct_timeout("generic", NULL, v[0]);
		write_ct_timeout("icmp", NULL, v[1]);
	}
	else {
		v[0] = read_ct_timeout("generic", NULL);
		v[1] = read_ct_timeout("icmp", NULL);
		sprintf(buf, "%u %u", v[0], v[1]);
		// nvram_set("ct_timeout", buf);
		tcapi_set("SysInfo_Entry", "ct_timeout", buf);
	}

#ifdef LINUX26
	// p = nvram_safe_get("ct_hashsize");
	tcapi_get("SysInfo_Entry", "ct_hashsize", p);
	i = atoi(p);
	if (i >= 127) {
		f_write_string("/sys/module/nf_conntrack/parameters/hashsize", p, 0, 0);
	}
	else if (f_read_string("/sys/module/nf_conntrack/parameters/hashsize", buf, sizeof(buf)) > 0) {
		if (atoi(buf) > 0) {//nvram_set("ct_hashsize", buf);
			pch = strchr(buf, '\n');
			if(pch != NULL)
				memset(pch, 0, 1);
			tcapi_set("SysInfo_Entry", "ct_hashsize", buf);
		}
	}
#endif

	// p = nvram_safe_get("ct_max");
	tcapi_get("SysInfo_Entry", "ct_max", p);
	i = atoi(p);
	if (i >= 128) {
		f_write_string("/proc/sys/net/ipv4/netfilter/ip_conntrack_max", p, 0, 0);
	}
	else if (f_read_string("/proc/sys/net/ipv4/netfilter/ip_conntrack_max", buf, sizeof(buf)) > 0) {
		if (atoi(buf) > 0) {//nvram_set("ct_max", buf);
			pch = strchr(buf, '\n');
			if(pch != NULL)
				memset(pch, 0, 1);
			tcapi_set("SysInfo_Entry", "ct_max", buf);
		}
	}

	//if (!nvram_match("nf_rtsp", "0")) {
	//	ct_modprobe("rtsp");
	//}
	//else {
	//	ct_modprobe_r("rtsp");
	//}

	// if (!nvram_match("nf_h323", "0")) {
		// ct_modprobe("h323");
	// }
	// else {
		// ct_modprobe_r("h323");
	// }

// #ifdef LINUX26
	// if (!nvram_match("nf_sip", "0")) {
		// ct_modprobe("sip");
	// }
	// else {
		// ct_modprobe_r("sip");
	// }
// #endif

	// !!TB - FTP Server
// #ifdef RTCONFIG_FTP
	// i = nvram_get_int("ftp_port");
	// if (nvram_match("ftp_enable", "1") && (i > 0) && (i != 21))
	// {
		// char ports[32];

		// sprintf(ports, "ports=21,%d", i);
		// ct_modprobe("ftp", ports);
	// }
	// else 
// #endif
	// if (!nvram_match("nf_ftp", "0")
// #ifdef RTCONFIG_FTP
		// || nvram_match("ftp_enable", "1")	// !!TB - FTP Server
// #endif
		// ) {
		// ct_modprobe("ftp");
	// }
	// else {
		// ct_modprobe_r("ftp");
	// }

	// if (!nvram_match("nf_pptp", "0")) {
		// ct_modprobe("proto_gre");
		// ct_modprobe("pptp");
	// }
	// else {
		// ct_modprobe_r("pptp");
		// ct_modprobe_r("proto_gre");
	// }
}

int check_process_exist(int pid){
	char path[32];

	memset(path, 0, 32);
	sprintf(path, "/proc/%d", pid);

	return check_if_dir_exist(path);
}

long fappend(FILE *out, const char *fname)
{
	FILE *in;
	char buf[1024];
	int n;
	long r;

	if ((in = fopen(fname, "r")) == NULL) return -1;
	r = 0;
	while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
		if (fwrite(buf, 1, n, out) != n) {
			r = -1;
			break;
		}
		else {
			r += n;
		}
	}
	fclose(in);
	return r;
}

long fappend_file(const char *path, const char *fname)
{
	FILE *f;
	int r = -1;

	if (f_exists(fname) && (f = fopen(path, "a")) != NULL) {
		r = fappend(f, fname);
		fclose(f);
	}
	return r;
}

int
is_invalid_char_for_hostname(char c)
{
	int ret = 0;

	if (c < 0x20)
		ret = 1;
	else if (c >= 0x21 && c <= 0x2c)
		ret = 1;
	else if (c >= 0x2e && c <= 0x2f)
		ret = 1;
	else if (c >= 0x3a && c <= 0x40)
		ret = 1;
#if 0
	else if (c >= 0x5b && c <= 0x60)
		ret = 1;
#else	/* allow '_' */
	else if (c >= 0x5b && c <= 0x5e)
		ret = 1;
	else if (c == 0x60)
		ret = 1;
#endif
	else if (c >= 0x7b)
		ret = 1;
#if 0
	printf("%c (0x%02x) is %svalid for hostname\n", c, c, (ret == 0) ? "  " : "in");
#endif
	return ret;
}

int
is_valid_hostname(const char *name)
{
	int len, i;

	if (!name)
		return 0;

	len = strlen(name);
	for (i = 0; i < len ; i++) {
		if (is_invalid_char_for_hostname(name[i])) {
			len = 0;
			break;
		}
	}
#if 0
	printf("%s is %svalid for hostname\n", name, len ? "" : "in");
#endif
	return len;
}

#ifdef RTCONFIG_SHP
void restart_lfp()
{
	char v[32];
	char tmp[32];

	tcapi_get("Lan_Entry0", "IP", tmp);
	// if(nvram_get_int("lfp_disable")==0) {
	if(tcapi_get_int("GUITemp_Entry1", "lfp_disable")==0) {
		// sprintf(v, "%x", inet_addr(nvram_safe_get("lan_ipaddr")));
		sprintf(v, "%x", inet_addr(tmp));
		f_write_string("/proc/net/lfpctrl", v, 0, 0);
	}
	else {
		f_write_string("/proc/net/lfpctrl", "", 0, 0);
	}
}
#endif
