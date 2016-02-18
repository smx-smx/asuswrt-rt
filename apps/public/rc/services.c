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

#define sin_addr(s) (((struct sockaddr_in *)(s))->sin_addr)

#define DDNS_INFO	"Vram_Entry"

// Pop an alarm to recheck pids in 500 msec.
static const struct itimerval pop_tv = { {0,0}, {0, 500 * 1000} };

// Pop an alarm to reap zombies. 
static const struct itimerval zombie_tv = { {0,0}, {307, 0} };

// -----------------------------------------------------------------------------
#define logs(s) syslog(LOG_NOTICE, s)

void create_passwd(void)
{
	// char s[512];
	// char *p;
	// char salt[32];
	// FILE *f;
	// mode_t m;
	// char *http_user;

#ifdef RTCONFIG_SAMBASRV	//!!TB
	// char *smbd_user;
	
	create_custom_passwd();
#endif

	tcapi_commit("Account");

	// strcpy(salt, "$1$");
	// f_read("/dev/urandom", s, 6);
	// base64_encode(s, salt + 3, 6);
	// salt[3 + 8] = 0;
	// p = salt;
	// while (*p) {
		// if (*p == '+') *p = '.';
		// ++p;
	// }
	// if (((p = nvram_get("http_passwd")) == NULL) || (*p == 0)) p = "admin";

	// if (((http_user = nvram_get("http_username")) == NULL) || (*http_user == 0)) http_user = "admin";

// #ifdef RTCONFIG_SAMBASRV	//!!TB
	// if (((smbd_user = nvram_get("smbd_user")) == NULL) || (*smbd_user == 0) || !strcmp(smbd_user, "root"))
		// smbd_user = "nas";
// #endif

	// m = umask(0777);
	// if ((f = fopen("/etc/shadow", "w")) != NULL) {
		// p = crypt(p, salt);
		// fprintf(f, "%s:%s:0:0:99999:7:0:0:\n"
				   // "nobody:*:0:0:99999:7:0:0:\n", http_user, p);
// #ifdef RTCONFIG_SAMBASRV	//!!TB
		// fprintf(f, "%s:*:0:0:99999:7:0:0:\n", smbd_user);
// #endif

		// fappend(f, "/etc/shadow.custom");
		// fclose(f);
	// }
	// umask(m);
	// chmod("/etc/shadow", 0600);

// #ifdef RTCONFIG_SAMBASRV	//!!TB
	// sprintf(s,
		// "%s:x:0:0:%s:/root:/bin/sh\n"
		// "%s:x:100:100:nas:/dev/null:/dev/null\n"
		// "nobody:x:65534:65534:nobody:/dev/null:/dev/null\n",
		// http_user,
		// http_user,
		// smbd_user);
// #else	//!!TB
	// sprintf(s,
		// "%s:x:0:0:%s:/root:/bin/sh\n"
		// "nobody:x:65534:65534:nobody:/dev/null:/dev/null\n",
		// http_user,
		// http_user);
// #endif	//!!TB
	// f_write_string("/etc/passwd", s, 0, 0644);
	fappend_file("/etc/passwd", "/etc/passwd.custom");

	// sprintf(s,
		// "%s:*:0:\n"
// #ifdef RTCONFIG_SAMBASRV	//!!TB
		// "nas:*:100:\n"
// #endif
		// "nobody:*:65534:\n",
		// http_user);
	// f_write_string("/etc/gshadow", s, 0, 0644);
	// fappend_file("/etc/gshadow", "/etc/gshadow.custom");

	// f_write_string("/etc/group",
		// "root:x:0:\n"
// #ifdef RTCONFIG_SAMBASRV	//!!TB
		// "nas:x:100:\n"
// #endif
		// "nobody:x:65534:\n",
		// 0, 0644);
	// fappend_file("/etc/group", "/etc/group.custom");
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
	tcapi_set(DDNS_INFO, "ddns_cache", buf);
	//~ nvram_set("ddns_ipaddr", ip+1);
	tcapi_set(DDNS_INFO, "ddns_ipaddr", ip+1);
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

	_dprintf("done\n");

	return 0;
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

int
start_services(void)
{
	start_infosvr();
	start_rstats();
#ifdef RTCONFIG_SPECTRUM
	start_spectrum();
#endif
#if defined(RTCONFIG_PPTPD) || defined(RTCONFIG_ACCEL_PPTPD)
	start_pptpd();
#endif
	return 0;
}

void
stop_services(void)
{
	stop_rstats();
	stop_spectrum();
	stop_infosvr();
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
