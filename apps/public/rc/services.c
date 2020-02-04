#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <shutils.h>
#include <dirent.h>
#include <sys/mount.h>
#include <sys/vfs.h>
#include <rc.h>
#include <semaphore_mfp.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <sys/reboot.h>
#include <errno.h>
#include <sys/stat.h>
#include <shared.h>
#ifdef RTCONFIG_NOTIFICATION_CENTER
#include <libnt.h>
#endif
#ifdef RTCONFIG_USB
#include <disk_io_tools.h>	//mkdir_if_none()
#else
extern int mkdir_if_none(const char *path)
{
	DIR *dp;
	char cmd[PATH_MAX];

	if (!(dp = opendir(path))) {
		memset(cmd, 0, PATH_MAX);
		sprintf(cmd, "mkdir -m 0777 -p '%s'", (char *)path);
		system(cmd);
		return 1;
	}
	closedir(dp);

	return 0;
}
#endif /* RTCONFIG_USB */

#define sin_addr(s) (((struct sockaddr_in *)(s))->sin_addr)

#define DDNS_INFO	"Vram_Entry"
#define NODE_LE_CONF	"Https_Entry"

// Pop an alarm to recheck pids in 500 msec.
static const struct itimerval pop_tv = { {0,0}, {0, 500 * 1000} };

// Pop an alarm to reap zombies. 
static const struct itimerval zombie_tv = { {0,0}, {307, 0} };

// -----------------------------------------------------------------------------
#define logs(s) syslog(LOG_NOTICE, s)

void setup_passwd(void)
{
	create_passwd();
}

void create_passwd(void)
{
#ifdef RTCONFIG_SAMBASRV
	create_custom_passwd();
#endif
#ifdef RTCONFIG_OPENVPN
	mkdir_if_none("/etc/pam.d");
	f_write_string("/etc/pam.d/openvpn",
		"auth required pam_unix.so\n"
		"account optional pam_unix.so\n"
		"password optional pam_unix.so\n"
		"session optional pam_unix.so\n"
		, 0, 0644);
	create_ovpn_passwd();
#endif

	tcapi_commit("Account");

	fappend_file("/etc/passwd", "/etc/passwd.custom");
#ifdef RTCONFIG_OPENVPN
	append_ovpn_accnt("/etc/passwd", "/etc/passwd.openvpn");
#endif
}


//int restart_dnsmasq(char *ifname, char *dns)
void start_dnsmasq(void)
{
	char cmd[256];
	char tmp[64];
	char word[256], *next;
	int unit;
	char attr[MAXLEN_ATTR_NAME] = {0};
	char value[MAXLEN_TCAPI_MSG] = {0};
	char vpn_dns[64] = {0};
	int i;
	int wan_primary = wan_primary_ifunit();
	int wan_secondary = wan_secondary_ifunit();
	int wan_pool[] = {wan_primary, wan_secondary, -1};	//wan_secondary=-1 if dualwan disable

	TRACE_PT("begin\n");

	stop_dnsmasq();

	snprintf(cmd, sizeof(cmd), "/userfs/bin/dnsmasq");

	//dns ip
	//check vpn client type and status, or choose wan dns
	memset(value, 0, sizeof(value));
	tcapi_get("VPNC_Entry", "proto", value);
	if( strncmp(value, "disable", 7) ) { //activate vpn client
		if(!strncmp(value, "openvpn", 7) ) {
			//TODO:
			//always Disabled currently,
			//Disabled: ignore server DNS
			//Relaxed: choose server based on response time from first query. 
			//Strict: use server DNS unless it doesn't work.
			//Exclusive: use server DNS. Period.
		}
		else {	//pptp, l2p
			if( tcapi_match("VPNC_Entry", "state_t", "2")
					&& tcapi_match("VPNC_Entry", "sbstate_t", "0")
				) {
				tcapi_get("VPNC_Entry", "dns_x", vpn_dns);
			}
		}
	}

	if(strlen(vpn_dns)) {
		foreach(word, vpn_dns, next) {
			snprintf(tmp, sizeof(tmp), " -S %s", word);
			strncat(cmd, tmp, (sizeof(cmd) - strlen(cmd) -1));
		}
	}
	else {
		//check active wan dns
		for(i = 0; unit = wan_pool[i], unit != -1; i++)
		{
			if(!is_wan_connect(unit))
				continue;

			snprintf(attr, sizeof(attr), "wan%d_dns", unit);
			if(tcapi_get("Wanduck_Common", attr, value) != TCAPI_PROCESS_OK)
			{
				_dprintf("%s : get dns failed\n", __FUNCTION__);
				continue;
			}

			if (strlen(value))
			{
				foreach(word, value, next)
				{
					snprintf(tmp, sizeof(tmp), " -S %s", word);
					strncat(cmd, tmp, (sizeof(cmd) - strlen(cmd) -1));
				}
			}
		}
	}

	//interface to listen on
	snprintf(tmp, sizeof(tmp), " -i br0");
	strncat(cmd, tmp, (sizeof(cmd) - strlen(cmd) -1));
#ifdef RTCONFIG_ACCEL_PPTPD
	if(tcapi_match("PPTP_Entry", "pptpd_enable", "1")) {
		snprintf(tmp, sizeof(tmp), " -i ppp2* -i ppp3*");
		strncat(cmd, tmp, (sizeof(cmd) - strlen(cmd) -1));
	}
#endif
#ifdef RTCONFIG_OPENVPN
	char vpn_serverx_dns[32];
	char *pos;
	int cur;
	char node[MAXLEN_NODE_NAME];

	if(tcapi_match("VPNControl_Entry", "VPNServer_enable", "1")
		) {
		tcapi_get("OpenVPN_Common", "vpn_serverx_dns", vpn_serverx_dns);
		for ( pos = strtok(&vpn_serverx_dns[0],","); pos != NULL; pos=strtok(NULL, ",") )
		{
			cur = atoi(pos);
			if ( cur )
			{
				snprintf(node, sizeof(node), "OpenVPN_Entry%d", OVPN_SERVER_BASE + cur);
				tcapi_get(node, "iface", value);
				snprintf(tmp, sizeof(tmp), " -i %s%d", value, OVPN_SERVER_BASE + cur);
				strncat(cmd, tmp, (sizeof(cmd) - strlen(cmd) -1));
			}
		}
	}
#endif

	_dprintf("%s : run (%s)\n", __FUNCTION__, cmd);
	system(cmd);

	TRACE_PT("end\n");
}

void stop_dnsmasq(void)
{
	TRACE_PT("begin\n");

	killall_tk("dnsmasq");
	unlink("/etc/dnsmasq.conf");

	TRACE_PT("end\n");
}

// -----------------------------------------------------------------------------
#ifdef LINUX26

static pid_t pid_hotplug2 = -1;

void stop_hotplug2(void)
{
	pid_hotplug2 = -1;
	killall_tk("hotplug2");
}

void start_hotplug2()
{
	stop_hotplug2();

	f_write_string("/proc/sys/kernel/hotplug", "", FW_NEWLINE, 0);
	xstart("hotplug2", "--persistent", "--no-coldplug");
	// FIXME: Don't remember exactly why I put "sleep" here -
	// but it was not for a race with check_services()... - TB
	sleep(1);

	// if (!nvram_contains_word("debug_norestart", "hotplug2")) {
		// pid_hotplug2 = -2;
	// }
}

#endif	//LINUX26

void
stop_infosvr()
{
	if (pids("infosvr"))
		killall_tk("infosvr");
}

int
start_infosvr()
{
	char *infosvr_argv[] = {"/userfs/bin/infosvr", "br0", NULL};
	pid_t pid;

	tcapi_set("SysInfo_Entry", "asus_mfg", "0");

	return _eval(infosvr_argv, NULL, 0, &pid);
}

int
ddns_updated_main(int argc, char *argv[])
{
	FILE *fp;
	char buf[64], *ip;

	if (!(fp=fopen("/tmp/ddns.cache", "r"))) return 0;

	fgets(buf, sizeof(buf), fp);
	fclose(fp);

	if (!(ip=strchr(buf, ','))) return 0;

	//~ nvram_set("ddns_cache", buf);
	tcapi_set(DDNS_INFO, "ddns_cache", trim_r(buf));
	//~ nvram_set("ddns_ipaddr", ip+1);
	tcapi_set(DDNS_INFO, "ddns_ipaddr", trim_r(ip+1));
	//~ nvram_set("ddns_status", "1");
	tcapi_set(DDNS_INFO, "ddns_status", "1");
	//~ nvram_set("ddns_server_x_old", nvram_safe_get("ddns_server_x"));
	memset(buf, 0, sizeof(buf));
	tcapi_get("Ddns_Entry", "SERVERNAME", buf);
	tcapi_set(DDNS_INFO, "ddns_server_x_old", buf);
	//~ nvram_set("ddns_hostname_old", nvram_safe_get("ddns_hostname_x"));
	memset(buf, 0, sizeof(buf));
	tcapi_get("Ddns_Entry", "MYHOST", buf);
	tcapi_set(DDNS_INFO, "ddns_hostname_old", buf);
	//~ nvram_set("ddns_updated", "1");
	tcapi_set(DDNS_INFO, "ddns_updated", "1");
	tcapi_set(DDNS_INFO, "ddns_check", "0");

	logmessage("ddns", "ddns update ok");

#ifdef RTCONFIG_OPENVPN
	update_ovpn_profie_remote();
#endif

	_dprintf("done\n");

	return 0;
}

void
start_httpd(void)
{
	char path[64] = {0};
	char *httpd_argv[] = { path, NULL };
	pid_t pid;

#ifdef TCSUPPORT_WEBSERVER_SSL
#ifdef RTCONFIG_LETSENCRYPT
	if(tcapi_get_int(NODE_LE_CONF, "le_enable") && !is_correct_le_certificate(HTTPD_CERT))
		copy_le_certificate(HTTPD_CERT, HTTPD_KEY);
#endif
	snprintf(path, sizeof(path), "/tmp/var/restart_boa.sh");
#else
	snprintf(path, sizeof(path), "/usr/script/restart_boa.sh");
#endif
	_eval(httpd_argv, NULL, 0, &pid);
}

void
stop_httpd(void)
{
	if (pids("boa"))
		killall_tk("boa");
}

void
stop_rstats(void)
{
	if (pids("rstats"))
		killall_tk("rstats");
}

void
start_rstats()
{
	stop_rstats();
	xstart("rstats");
}

#ifdef RTCONFIG_SPECTRUM
void stop_spectrum(void)
{
	if (pids("spectrum"))
		killall_tk("spectrum");
}

void start_spectrum(void)
{
	stop_spectrum();
	xstart("spectrum");
}
#endif

#ifdef RTCONFIG_SYSSTATE
void stop_sysstate(void)
{
	if (pids("sysstate"))
		killall_tk("sysstate");
}

void start_sysstate(void)
{
	stop_sysstate();
	xstart("sysstate");
}
#endif

#ifdef RTCONFIG_NOTIFICATION_CENTER
int
start_notification_center(void)
{
	char *nt_monitor_argv[] = {"nt_monitor", NULL};
	pid_t pid;

	am_setup_email_conf();
	am_setup_email_info();
	
	return _eval(nt_monitor_argv, NULL, 0, &pid);
}

int
stop_notification_center(void)
{
	killall_tk("nt_monitor");
	killall_tk("nt_actMail");
	killall_tk("nt_center");
}
#endif

#ifdef RTCONFIG_PROTECTION_SERVER
int
start_ptcsrv(void)
{
	char *ptcsrv_argv[] = {"protect_srv", NULL};
	pid_t pid;

	return _eval(ptcsrv_argv, NULL, 0, &pid);
}

int
stop_ptcsrv(void)
{
	killall_tk("protect_srv");
	return 0;
}
#endif

int
start_services(void)
{
#ifdef RTCONFIG_NOTIFICATION_CENTER
	start_notification_center();
#endif
#ifdef RTCONFIG_PROTECTION_SERVER
	start_ptcsrv();
#endif
#ifdef TCSUPPORT_SSH
	start_sshd();
#endif
#ifdef RTCONFIG_CROND
	start_cron();
#endif
#ifdef RTCONFIG_LETSENCRYPT
	start_letsencrypt();
#endif
	start_httpd();
	start_infosvr();
	start_rstats();
#ifdef RTCONFIG_SPECTRUM
	start_spectrum();
#endif
#ifdef RTCONFIG_SYSSTATE
	start_sysstate();
#endif
#if defined(RTCONFIG_PPTPD) || defined(RTCONFIG_ACCEL_PPTPD)
	start_pptpd();
#endif
#ifdef ASUS_DISK_UTILITY
	start_diskmon();
#endif
	return 0;
}

void
stop_services(void)
{
	stop_rstats();
#ifdef RTCONFIG_SPECTRUM
	stop_spectrum();
#endif
	stop_infosvr();
#ifdef ASUS_DISK_UTILITY
	stop_diskmon();
#endif
	//stop_httpd();	//firmware upgrade still need boa for a while
#ifdef RTCONFIG_LETSENCRYPT
	stop_letsencrypt();
#endif
#ifdef RTCONFIG_CROND
	stop_cron();
#endif
#ifdef TCSUPPORT_SSH
	stop_sshd();
#endif
#ifdef RTCONFIG_PROTECTION_SERVER
	stop_ptcsrv();
#endif
#ifdef RTCONFIG_NOTIFICATION_CENTER
	stop_notification_center();
#endif
}
/*
// 2008.10 magic 
int start_wanduck(void)
{	
	char *argv[] = {"/sbin/wanduck", NULL};
	pid_t pid;
#if 0
	int sw_mode = nvram_get_int("sw_mode");

	if(sw_mode != SW_MODE_ROUTER && sw_mode != SW_MODE_REPEATER)
		return -1;
#endif

	if(!strcmp(nvram_safe_get("wanduck_down"), "1"))
		return 0;

	return _eval(argv, NULL, 0, &pid);
}
*/
void stop_wanduck(void)
{	
	killall("wanduck", SIGTERM);
}

#ifdef RTCONFIG_CROND
void start_cron(void)
{
	stop_cron();
	eval("crond");
}


void stop_cron(void)
{
	killall_tk("crond");
}
#endif

void start_script(int argc, char *argv[])
{
	int pid;

	argv[argc] = NULL;
	_eval(argv, NULL, 0, &pid);

}

int start_ddns(void)
{
	int ret = -1;
	ret = tcapi_commit("Ddns");

	if(ret != TCAPI_PROCESS_OK)
	{
		_dprintf("start_ddns(), failed to commit Ddns, ret=[%d]\n", ret);
	}
	return ret;
}

void stop_ddns(void)
{
	if (pids("ez-ipupdate"))
	{
		killall_tk("ez-ipupdate");
	}

	tcapi_set("Vram_Entry", "ddns_return_code", "");
	tcapi_set("Vram_Entry", "ddns_return_code_chk", "");
	unlink("/etc/ddns.conf");
	_dprintf("stop_ddns(), remove /etc/ddns.conf.\n");
}

int asusddns_reg_domain(int reg)
{
	char value[32] = {0};
	char wan_ifname[256];
	char wan_ip[16] = {0};
	char ddns_ipaddr[16] = {0};
	char ddns_server_x[64] = {0};
	char ddns_server_x_old[64] = {0};
	char ddns_hostname_x[128] = {0};
	char ddns_hostname_old[128] = {0};
	char ddns_cache[32] = {0};
	char nserver[64] = {0};
	int unit = -1;
	FILE *fp = NULL;
	char ddns_cmd[300] = {0};

	tcapi_get("Ddns_Entry", "Active", value);
	syslog(LOG_DEBUG, "[%s]Begin...\n", __FUNCTION__);
	if (reg) { //0:Aidisk, 1:Advanced Setting
		if (strcmp(value, "1") != 0)
		{
			syslog(LOG_ERR, "[%s]DDNS is not enabled.\n", __FUNCTION__);
			return -1;
		}
	}

	if(!find_default_route_ifname(wan_ifname, sizeof(wan_ifname)))
	{
		syslog(LOG_ERR, "[%s] Cannot find_default_route_ifname().\n", __FUNCTION__);
		return -1;
	}

#ifdef RTCONFIG_DUALWAN
		memset(value, 0 , sizeof(value));
		if(tcapi_match("Dualwan_Entry", "wans_mode", "lb"))
		{
			memset(value, 0 , sizeof(value));
			tcapi_get("Ddns_Entry", "ddns_wan_unit", value);
			syslog(LOG_INFO, "[%s], ddns_wan_unit[%s]\n", __FUNCTION__, value);
			if(strcmp(value, "0") == 0)
			{
				//for interface = Primary WAN
				get_wan_primary_ifname(wan_ifname);
			}
			else if(strcmp(value, "1") == 0)
			{
				//for interface = Secondary WAN
				get_wan_secondary_ifname(wan_ifname);
			}
			else
			{
				//for interface = Auto
				get_first_connected_public_wan_ifname(wan_ifname);
			}
		}
#endif /* RTCONFIG_DUALWAN */
	unit = wan_ifunit(wan_ifname);
	if(unit == -1)
	{
		syslog(LOG_ERR, "[%s] Cannot find correct ifunit.\n", __FUNCTION__);
		return -1;
	}
	syslog(LOG_INFO, "[%s]Interface[%s], ifunit[%d]\n", __FUNCTION__, wan_ifname, unit);

	memset(value, 0 , sizeof(value));
	snprintf(value, sizeof(value), "wan%d_ipaddr", unit);
	tcapi_get("Wanduck_Common", value, wan_ip);
	if (strcmp(wan_ip, "") == 0 || !inet_addr(wan_ip)) {
		syslog(LOG_ERR, "[%s] WAN IP is empty.\n", __FUNCTION__);
		return -1;
	}

	_dprintf("wan_ip=[%s]\n", wan_ip);

	// TODO : Check /tmp/ddns.cache to see current IP in DDNS,
	// update when ipaddr!= ipaddr in cache.
	// nvram ddns_cache, the same with /tmp/ddns.cache

	tcapi_get("Vram_Entry", "ddns_ipaddr", ddns_ipaddr);

	tcapi_get("Ddns_Entry", "MYHOST", ddns_hostname_x);
	tcapi_get("Vram_Entry", "ddns_hostname_old", ddns_hostname_old);

	tcapi_get("Ddns_Entry", "SERVERNAME", ddns_server_x);
	tcapi_get("Vram_Entry", "ddns_server_x_old", ddns_server_x_old);

	_dprintf("ddns_ipaddr=[%s]\n", ddns_ipaddr);
	_dprintf("ddns_hostname_x=[%s]\n", ddns_hostname_x);
	_dprintf("ddns_hostname_old=[%s]\n", ddns_hostname_old);
	_dprintf("ddns_server_x=[%s]\n", ddns_server_x);
	_dprintf("ddns_server_x_old=[%s]\n", ddns_server_x_old);

	if ((inet_addr(wan_ip) == inet_addr(ddns_ipaddr)) &&
		(strcmp(ddns_server_x, ddns_server_x_old) == 0) &&
		(strcmp(ddns_hostname_x, ddns_hostname_old) == 0)) {
		tcapi_set("Vram_Entry", "ddns_return_code", "no_change");
		syslog(LOG_INFO, "[%s]IP address, server and hostname have not changed since the last update.", __FUNCTION__);
		return -1;
	}

	tcapi_get("Vram_Entry", "ddns_cache", ddns_cache);

	if (	(strcmp("ddns_server_x_old", "") != 0 &&
		strcmp(ddns_server_x, ddns_server_x_old)) ||
		(strcmp("ddns_hostname_x_old", "") != 0 &&
		strcmp(ddns_hostname_x, ddns_hostname_old))
	) {
		syslog(LOG_INFO, "[%s]clear ddns cache file for server/hostname change", __FUNCTION__);
		unlink("/tmp/ddns.cache");
	}
	else if (!(fp = fopen("/tmp/ddns.cache", "r")) && (strcmp(ddns_cache, "") != 0)) {
		if ((fp = fopen("/tmp/ddns.cache", "w+"))) {
			fprintf(fp, "%s", ddns_cache);
			fclose(fp);
		}
	}
	else {
		if (fp) {
			fclose(fp);
		}
	}

	tcapi_set("Vram_Entry", "ddns_return_code", "ddns_query");

	if (pids("ez-ipupdate"))
	{
		killall("ez-ipupdate", SIGINT);
		sleep(1);
	}

	tcapi_get("Ddns_Entry", "ddns_serverhost_x", nserver);
	if(strlen(nserver) == 0)
	{
		snprintf(nserver, sizeof(nserver), "nwsrv-ns1.asus.com");
	}

	snprintf(ddns_cmd, sizeof(ddns_cmd), "/userfs/bin/ez-ipupdate -S dyndns -i %s -h %s -A 1 -s %s -e /sbin/ddns_updated -b /tmp/ddns.cache", wan_ifname, ddns_hostname_x, nserver);
	system(ddns_cmd);

	syslog(LOG_DEBUG, "[%s]End...\n", __FUNCTION__);
	return 0;
}

int asusddns_unregister(void)
{
	char wan_ifname[256];
	char nserver[64] = {0};
	char value[32] = {0};
	char ddns_cmd[300] = {0};
	char ddns_hostname_x[128] = {0};

	syslog(LOG_DEBUG, "[%s]Begin...\n", __FUNCTION__);
	if(!find_default_route_ifname(wan_ifname, sizeof(wan_ifname)))
	{
		syslog(LOG_ERR, "[%s] Cannot find_default_route_ifname().\n", __FUNCTION__);
		return -1;
	}

#ifdef RTCONFIG_DUALWAN
		memset(value, 0 , sizeof(value));
		if(tcapi_match("Dualwan_Entry", "wans_mode", "lb"))
		{
			memset(value, 0 , sizeof(value));
			tcapi_get("Ddns_Entry", "ddns_wan_unit", value);
			syslog(LOG_INFO, "[%s], ddns_wan_unit[%s]\n", __FUNCTION__, value);
			if(strcmp(value, "0") == 0)
			{
				//for interface = Primary WAN
				get_wan_primary_ifname(wan_ifname);
			}
			else if(strcmp(value, "1") == 0)
			{
				//for interface = Secondary WAN
				get_wan_secondary_ifname(wan_ifname);
			}
			else
			{
				//for interface = Auto
				get_first_connected_public_wan_ifname(wan_ifname);
			}
		}
#endif /* RTCONFIG_DUALWAN */
	syslog(LOG_INFO, "[%s]Interface[%s]\n", __FUNCTION__, wan_ifname);

	tcapi_set("Vram_Entry", "ddns_return_code", "ddns_unregister");
	tcapi_set("Vram_Entry", "asusddns_reg_result", "");

	if (pids("ez-ipupdate"))
	{
		killall("ez-ipupdate", SIGINT);
		sleep(1);
	}

	tcapi_get("Ddns_Entry", "ddns_serverhost_x", nserver);
	if(strlen(nserver) == 0)
	{
		snprintf(nserver, sizeof(nserver), "nwsrv-ns1.asus.com");
	}

	tcapi_get("Ddns_Entry", "MYHOST", ddns_hostname_x);
	_dprintf("%s: do ez-ipupdate to unregister!\n", __FUNCTION__);
	_dprintf("wan_ifname = %s, nserver = %s, hostname = %s\n", wan_ifname, nserver, ddns_hostname_x);

	snprintf(ddns_cmd, sizeof(ddns_cmd), "/userfs/bin/ez-ipupdate -S dyndns -i %s -h %s -A 3 -s %s", wan_ifname, ddns_hostname_x, nserver);
	system(ddns_cmd);

	syslog(LOG_DEBUG, "[%s]End...\n", __FUNCTION__);
	return 0;
}

// -----------------------------------------------------------------------------

/* -1 = Don't check for this program, it is not expected to be running.
 * Other = This program has been started and should be kept running.  If no
 * process with the name is running, call func to restart it.
 * Note: At startup, dnsmasq forks a short-lived child which forks a
 * long-lived (grand)child.  The parents terminate.
 * Many daemons use this technique.
 */
static void _check(pid_t pid, const char *name, void (*func)(void))
{
	if (pid == -1) return;

	if (pidof(name) > 0) return;

	syslog(LOG_DEBUG, "%s terminated unexpectedly, restarting.\n", name);
	func();

	// Force recheck in 500 msec
	setitimer(ITIMER_REAL, &pop_tv, NULL);
}

void check_services(void)
{
//	TRACE_PT("keep alive\n");

	// Periodically reap any zombies
	setitimer(ITIMER_REAL, &zombie_tv, NULL);

#ifdef LINUX26
	_check(pids("hotplug2"), "hotplug2", start_hotplug2);
#endif
#ifdef RTCONFIG_CROND
	_check(pids("crond"), "crond", start_cron);
#endif
}
#define RC_SERVICE_STOP 0x01
#define RC_SERVICE_START 0x02

void handle_notifications(char* rc_service)
{
	char nv[256], nvtmp[32], *cmd[8], *script;
	char *nvp, *b, *nvptr;
	int action = 0;
	int count;

	// handle command one by one only
	// handle at most 7 parameters only
	strncpy(nv, rc_service, 255);
	nvptr = nv;
again:
	nvp = strsep(&nvptr, ";");

	count = 0;
	while ((b = strsep(&nvp, " ")) != NULL)
	{
		_dprintf("cmd[%d]=%s\n", count, b);
		cmd[count] = b;
		count ++;
		if(count == 7) break;
	}
	cmd[count] = 0;

	if(cmd[0]==0 || strlen(cmd[0])==0) {
		//nvram_set("rc_service", "");
		return;
	}

	if(strncmp(cmd[0], "start_", 6)==0) {
		action |= RC_SERVICE_START;
		script = &cmd[0][6];
	}
	else if(strncmp(cmd[0], "stop_", 5)==0) {
		action |= RC_SERVICE_STOP;
		script = &cmd[0][5];
	}
	else if(strncmp(cmd[0], "restart_", 8)==0) {
		action |= (RC_SERVICE_START | RC_SERVICE_STOP);
		script = &cmd[0][8];
	}
	else {
		action = 0;
		script = cmd[0];
	}

	TRACE_PT("running: %d %s\n", action, script);

	if(0) {
		//test
	}
#ifdef RTCONFIG_USB_MODEM	//only usb modem use this currently
	else if (strcmp(script, "wan_if") == 0) {
		_dprintf("%s: wan_if: %s.\n", __FUNCTION__, cmd[1]);
		if(cmd[1]) {
			if(action & RC_SERVICE_STOP)
			{
				stop_wan_if(atoi(cmd[1]));
			}
			if(action & RC_SERVICE_START)
			{
				start_wan_if(atoi(cmd[1]));
			}
		}
	}
#endif
	else if (strcmp(script, "ddns") == 0)
	{
		if(action & RC_SERVICE_STOP) stop_ddns();
		if(action & RC_SERVICE_START) start_ddns();
	}
	else if (strcmp(script, "aidisk_asusddns_register") == 0)
	{
		asusddns_reg_domain(0);
	}
	else if (strcmp(script, "adm_asusddns_register") == 0)
	{
		asusddns_reg_domain(1);
	}
	else if (strcmp(script, "asusddns_unregister") == 0)
	{
		asusddns_unregister();
	}
	else if (strcmp(script, "httpd") == 0 || strcmp(script, "boa") == 0)
	{
		if(action & RC_SERVICE_STOP) stop_httpd();
		if(action & RC_SERVICE_START) start_httpd();
	}
	else if (strcmp(script, "dnsmasq") == 0)
	{
		if(action & RC_SERVICE_STOP) stop_dnsmasq();
		if(action & RC_SERVICE_START) start_dnsmasq();
	}
#ifdef RTCONFIG_CROND
	else if (strcmp(script, "crond") == 0)
	{
		if(action & RC_SERVICE_STOP) stop_cron();
		if(action & RC_SERVICE_START) start_cron();
	}
#endif
	else if (strcmp(script, "chpass") == 0)
	{
		setup_passwd();
	}
#ifdef TCSUPPORT_SSH
	else if (strcmp(script, "ssh") == 0)
	{
		if(action & RC_SERVICE_STOP) stop_sshd();
		if(action & RC_SERVICE_START) start_sshd();
	}
#endif
#ifdef RTCONFIG_USB
	else if (strcmp(script, "nasapps") == 0)
	{
		if(action&RC_SERVICE_STOP){
//_dprintf("restart_nas_services(%d): test 10.\n", getpid());
			restart_nas_services(1, 0);
		}
		if(action&RC_SERVICE_START){
//_dprintf("restart_nas_services(%d): test 11.\n", getpid());
			restart_nas_services(0, 1);
		}
	}
#if defined(RTCONFIG_SAMBASRV) && defined(RTCONFIG_FTP)
	else if (strcmp(script, "ftpsamba") == 0)
	{
		if(action & RC_SERVICE_STOP) {
			stop_ftpd();
			stop_samba();
		}
		if(action & RC_SERVICE_START) {
			setup_passwd();
			start_samba();
			start_ftpd();
		}
	}
#endif
#ifdef RTCONFIG_SAMBASRV
	else if (strcmp(script, "samba") == 0)
	{
		if(action & RC_SERVICE_STOP) stop_samba();
		if(action & RC_SERVICE_START) start_samba();
	}
#endif
#ifdef RTCONFIG_USB_PRINTER
	else if (strcmp(script, "lpd") == 0)
	{
		if(action & RC_SERVICE_STOP) stop_lpd();
		if(action & RC_SERVICE_START) start_lpd();
	}
	else if (strcmp(script, "u2ec") == 0)
	{
		if(action & RC_SERVICE_STOP) stop_u2ec();
		if(action & RC_SERVICE_START) start_u2ec();
	}
#endif

#endif	//#ifdef RTCONFIG_USB

	else if (strcmp(script, "pppoe_relay") == 0)
	{
		if(action & RC_SERVICE_STOP)
			stop_pppoe_relay();
		if(action & RC_SERVICE_START) {
			char wan_ifname[32];
			if(cmd[1] && strlen(cmd[1]))
				strncpy(wan_ifname, get_wanx_ifname(atoi(cmd[1])), sizeof(wan_ifname)-1);
			else
				strncpy(wan_ifname, get_wanx_ifname(wan_primary_ifunit()), sizeof(wan_ifname)-1);
			//_dprintf("Interface %s for PPPoE server\n", wan_ifname);
			start_pppoe_relay(wan_ifname);
		}
	}
#if defined(RTCONFIG_PPTPD) || defined(RTCONFIG_ACCEL_PPTPD)
	else if (strcmp(script, "pptpd") == 0)
	{
		if (action & RC_SERVICE_STOP)
		{
			stop_pptpd();
		}
		if (action & RC_SERVICE_START)
		{
			start_pptpd();
			tcapi_commit("Firewall");
		}
	}
#endif

#ifdef RTCONFIG_OPENVPN
	else if (strncmp(script, "vpnclient", 9) == 0) {
		if (action & RC_SERVICE_STOP) stop_ovpn_client(atoi(&script[9]));
		if (action & RC_SERVICE_START) start_ovpn_client(atoi(&script[9]));
	}
	else if (strncmp(script, "vpnserver" ,9) == 0) {
		if (action & RC_SERVICE_STOP) stop_ovpn_server(atoi(&script[9]));
		if (action & RC_SERVICE_START) start_ovpn_server(atoi(&script[9]));
	}
#endif
#if defined(RTCONFIG_PPTPD) || defined(RTCONFIG_ACCEL_PPTPD)
	else if (strcmp(script, "vpnd") == 0)
	{
		if (action & RC_SERVICE_STOP){
			stop_pptpd();
		}
		if (action & RC_SERVICE_START){
				start_pptpd();
				tcapi_commit("Firewall");
		}
	}
#endif
#if defined(RTCONFIG_OPENVPN)
	else if (strcmp(script, "openvpnd") == 0)
	{
		int openvpn_unit = tcapi_get_int("OpenVPN_Common", "vpn_server_unit");
		if (action & RC_SERVICE_STOP){
			stop_ovpn_server(openvpn_unit);
		}
		if (action & RC_SERVICE_START){
			start_ovpn_server(openvpn_unit);
 		}
 	}
#endif
#ifdef RTCONFIG_VPNC
	else if (strcmp(script, "vpncall") == 0)
	{
#if defined(RTCONFIG_OPENVPN)
		char buf[32] = {0};
		int i;
		int openvpnc_unit = tcapi_get_int("OpenVPN_Common", "vpn_client_unit");
#endif
		if (action & RC_SERVICE_STOP){
			stop_vpnc();
#if defined(RTCONFIG_OPENVPN)
			for( i = 1; i <= OVPN_CLIENT_MAX; i++ )
			{
				sprintf(buf, "vpnclient%d", i);
				if ( pidof(buf) >= 0 )
				{
					stop_ovpn_client(i);
				}
			}
#endif
		}

		if (action & RC_SERVICE_START){
#if defined(RTCONFIG_OPENVPN)
			if(tcapi_match("VPNC_Entry", "proto", "openvpn")){
				if(openvpnc_unit)
					start_ovpn_client(openvpnc_unit);
				stop_vpnc();
			}
			else{
				for( i = 1; i <= OVPN_CLIENT_MAX; i++ )
				{
					sprintf(buf, "vpnclient%d", i);
					if ( pidof(buf) >= 0 )
					{
						stop_ovpn_client(i);
					}
				}
#endif
				start_vpnc();
#if defined(RTCONFIG_OPENVPN)
			}
#endif
		}
	}
#endif
#ifdef RTCONFIG_TR069
	else if (strncmp(script, "tr", 2) == 0) {
		if (action & RC_SERVICE_STOP) stop_tr();
		if (action & RC_SERVICE_START) start_tr();
	}
#endif
	else if (strcmp(script, "sh") == 0) {
		_dprintf("%s: shell: %s\n", __FUNCTION__, cmd[1]);
		if(cmd[1]) system(cmd[1]);
	}
	else if (strcmp(script, "app") == 0) {
#if defined(RTCONFIG_APP_PREINSTALLED) || defined(RTCONFIG_APP_NETINSTALLED)
		if(action & RC_SERVICE_STOP)
			stop_app();
#endif
	}
	else if(!strncmp(script, "webs_", 5))
	{
		if(action & RC_SERVICE_START) {
#ifdef DEBUG_RCTEST // Left for UI debug
			char *webscript_dir;
			webscript_dir = nvram_safe_get("webscript_dir");
			if(strlen(webscript_dir))
				sprintf(nvtmp, "%s/%s.sh", webscript_dir, script);
			else
#endif
			sprintf(nvtmp, "%s.sh", script);
			cmd[0] = nvtmp;
			start_script(count, cmd);
		}
	}
#ifdef RTCONFIG_NOTIFICATION_CENTER
	else if(strcmp(script, "email_conf") == 0) {
		am_setup_email_conf();
	}
	else if (strcmp(script, "email_info") == 0)
	{
		am_setup_email_info();
		char buf[8];
		if (f_read_string(NOTIFY_ACTION_MAIL_PID_PATH, buf, sizeof(buf)) > 0) {
			kill(atoi(buf), SIGUSR2);
		}
	}
#endif
#ifdef RTCONFIG_LETSENCRYPT
	else if (strcmp(script, "letsencrypt") == 0)
	{
		if(action & RC_SERVICE_STOP) stop_letsencrypt();
		if(action & RC_SERVICE_START) start_letsencrypt();
	}
#endif
#ifdef RTCONFIG_BWDPI
	else if (strcmp(script, "sig_check") == 0)
	{
		int sig_state_flag = 0;

		if(action & RC_SERVICE_START){
			eval("sig2nd_update.sh");
			sig_state_flag = tcapi_get_int("Vram_Entry", "sig_state_flag");

			if(sig_state_flag == 1)
			{
				eval("sig2nd_upgrade.sh");
			}
		}
	}
#endif
	else
	{
		fprintf(stderr,
			"WARNING: rc notified of unrecognized event `%s'.\n",
					script);
	}

	if(nvptr){
_dprintf("goto again(%d)...\n", getpid());
		goto again;
	}

}

// string = S20transmission -> return value = transmission.
int get_apps_name(const char *string)
{
	char *ptr;

	if(string == NULL)
		return 0;

	if((ptr = rindex(string, '/')) != NULL)
		++ptr;
	else
		ptr = (char*) string;
	if(ptr[0] != 'S')
		return 0;
	++ptr; // S.

	while(ptr != NULL){
		if(isdigit(ptr[0]))
			++ptr;
		else
			break;
	}

	printf("%s", ptr);

	return 1;
}

int run_app_script(const char *pkg_name, const char *pkg_action)
{
	char app_name[128];

	if(pkg_action == NULL || strlen(pkg_action) <= 0)
		return -1;

	memset(app_name, 0, 128);
	if(pkg_name == NULL)
		strcpy(app_name, "allpkg");
	else
		strcpy(app_name, pkg_name);

	return doSystem("app_init_run.sh %s %s", app_name, pkg_action);
}

void start_nat_rules(void)
{
	int len;
	char *fn = NAT_RULES, ln[PATH_MAX];
	struct stat s;
	char tmp[4] = {0};

	// all rules applied directly according to currently status, wanduck help to triger those not cover by normal flow
	tcapi_get("SysInfo_Entry", "x_Setting", tmp);
	if(!strcmp(tmp, "0")) {
	// if(nvram_match("x_Setting", "0")){
		stop_nat_rules();
		return;
	}
	
	memset(tmp, 0, sizeof(tmp));
	tcapi_get("Wanduck_Common", "nat_state", tmp);
	if (atoi(tmp) == NAT_STATE_NORMAL) return;
	// if(nvram_get_int("nat_state")==NAT_STATE_NORMAL) return;
	
	sprintf(tmp, "%d", NAT_STATE_NORMAL);
	tcapi_set("Wanduck_Common", "nat_state", tmp);
	// nvram_set_int("nat_state", NAT_STATE_NORMAL);

	if (!lstat(NAT_RULES, &s) && S_ISLNK(s.st_mode) &&
	    (len = readlink(NAT_RULES, ln, sizeof(ln))) > 0) {
		ln[len] = '\0';
		fn = ln;
	}

	_dprintf("%s: apply the nat_rules(%s)!\n", __FUNCTION__, fn);
	logmessage("start_nat_rules", "apply the nat_rules(%s)!", fn);

	setup_ct_timeout(TRUE);
	setup_udp_timeout(TRUE);

	// eval("iptables-restore", NAT_RULES);
	tcapi_commit("Wanduck");

	return;
}

void stop_nat_rules(void)
{
	char tmp[4] = {0};

	if(tcapi_match("SysInfo_Entry", "nat_redirect_enable", "0"))
		return;

	tcapi_get("Wanduck_Common", "nat_state", tmp);
	if (atoi(tmp) == NAT_STATE_REDIRECT) return;
	// if (nvram_get_int("nat_state")==NAT_STATE_REDIRECT) return ;

	sprintf(tmp, "%d", NAT_STATE_REDIRECT);
	tcapi_set("Wanduck_Common", "nat_state", tmp);
	// nvram_set_int("nat_state", NAT_STATE_REDIRECT);

	_dprintf("%s: apply the redirect_rules!\n", __FUNCTION__);
	logmessage("stop_nat_rules", "apply the redirect_rules!");

	setup_ct_timeout(FALSE);
	setup_udp_timeout(FALSE);

	// eval("iptables-restore", "/tmp/redirect_rules");
	tcapi_commit("Wanduck");

	return;
}


int service_main(int argc, char *argv[])
{
	if (argc != 2) usage_exit(argv[0], "<action_service>");
	handle_notifications(argv[1]);
	printf("\nDone.\n");
	return 0;
}
