#include "rc.h"
#include <sys/prctl.h>
#include <dirent.h>
#include <openssl/x509.h>
#include <openssl/pem.h>

#define LE_TAG	"LetsEncrypt"
#define LE_PID_FILE	"/var/run/letsencrypt.pid"
#define LE_JOBS_DAY	7
#define LE_JOBS_MIN	5
#define LE_ACME_PID_FILE	"/var/run/acme.pid"
#define LE_FW_SCRIPT	"/tmp/.le/le-fw.sh"
#define ACME_PROGRAM	"/usr/script/acme.sh"
#define ACME_WORKHOME	"/tmp/.le"
#define ACME_DNSAPI	"/tmp/.le/dnsapi"
#if defined(TCSUPPORT_JFFS_FS)
#define ACME_CERTHOME	"/jffs/.le"
#define ACME_ACCKEY	"/jffs/.le/account.key"
#define ACME_ACCCONF	"/jffs/.le/account.conf"
#elif defined(TCSUPPORT_YAFFS_FS)
#define ACME_CERTHOME	"/yaffs/.le"
#define ACME_ACCKEY	"/yaffs/.le/account.key"
#define ACME_ACCCONF	"/yaffs/.le/account.conf"
#endif
#define NODE_LE_CONF	"Https_Entry"
#define NODE_LE_STAT	"Vram_Entry"
#define NODE_DDNS_CONF	"Ddns_Entry"
#define NODE_DDNS_STAT	"Vram_Entry"
#define NODE_TZ_STAT	"Timezone_Entry"

static int _le_acme_exit = 0;

enum {
	ACME_ISSUE=0,
	ACME_RENEW,
	ACME_REVOKE
};

enum {
	ACME_AUTH_DNS=0,
	ACME_AUTH_HTTP,
	ACME_AUTH_TLS
};

enum {
	LE_ST_NONE=0,
	LE_ST_ISSUED,
};

enum {
	LE_STS_DISABLED=0,
	LE_STS_STOPPING,
	LE_STS_STOPPED,
	LE_STS_CHECKING,
	LE_STS_WAIT_RETRY,
	LE_STS_REVOKE,
	LE_STS_ISSUE,
	LE_STS_RENEW,
	LE_STS_WAIT_RENEW
};
enum {
	LE_AUX_NONE=0,
	LE_AUX_CONFIG,
	LE_AUX_INTERNET,
	LE_AUX_NTP,
	LE_AUX_DDNS,
	LE_AUX_ACME
};

static int _le_check_configure()
{
	if(!tcapi_match(NODE_LE_CONF, "le_enable", "1")) {
		tcapi_set_int(NODE_LE_STAT, "le_state_t", LE_STS_DISABLED);
		return -1;
	}
	else if (!tcapi_match(NODE_DDNS_CONF, "Active", "1")) {
		logmessage(LE_TAG, "DDNS not enable");
		tcapi_set_int(NODE_LE_STAT, "le_auxstate_t", LE_AUX_CONFIG);
		return -1;
	}
	else if (!tcapi_match(NODE_DDNS_CONF, "SERVERNAME", "WWW.ASUS.COM")) {
		logmessage(LE_TAG, "only support ASUS DDNS");
		tcapi_set_int(NODE_LE_STAT, "le_auxstate_t", LE_AUX_CONFIG);
		return -1;
	}
	else {
		return 0;
	}
}

static void _le_check_dir()
{
	if(!d_exists(ACME_WORKHOME)) {
		mkdir(ACME_WORKHOME, 0600);
		symlink("/usr/script/", ACME_DNSAPI);
	}

	if(!d_exists(ACME_CERTHOME)) {
		mkdir(ACME_CERTHOME, 0600);
	}
}

static int _le_check_status()
{
	if(!is_wan_connect(wan_primary_ifunit())) {
		tcapi_set_int(NODE_LE_STAT, "le_auxstate_t", LE_AUX_INTERNET);
		return 0;
	}
	else if(!tcapi_match(NODE_TZ_STAT, "status", "0")) {
		tcapi_set_int(NODE_LE_STAT, "le_auxstate_t", LE_AUX_NTP);
		return 0;
	}
	else if(tcapi_match(NODE_LE_CONF, "le_acme_auth", "http") && !tcapi_match(NODE_DDNS_STAT, "ddns_status", "1")) {
		tcapi_set_int(NODE_LE_STAT, "le_auxstate_t", LE_AUX_DDNS);
		return 0;
	}
	else
		return 1;
}

int f_diff(char *path1, char *path2)
{
	FILE *fp1, *fp2;
	int ch1, ch2;
	int ret = 0;

	fp1 = fopen(path1, "r");
	fp2 = fopen(path2, "r");
	if(fp1 == NULL || fp2 == NULL)
		return 1;

	ch1 = getc(fp1);
	ch2 = getc(fp2);

	while ((ch1 != EOF) && (ch2 != EOF) && (ch1 == ch2)) {
		ch1 = getc(fp1);
		ch2 = getc(fp2);
	}

	if (ch1 != ch2)
		ret = 1;

	fclose(fp1);
	fclose(fp2);

	//_dprintf("%s(%d)\n", __FUNCTION__, ret);
	return ret;
}

/*
 * Install issued certificate for httpd, lighttpd
 * called by standalone le_acme since using notify_rc
 */
static void _le_install_certificate(char *le_cert_path)
{
	if(f_diff(HTTPD_CERT, le_cert_path)) {
		if( !copy_le_certificate(HTTPD_CERT, HTTPD_KEY)) {
			system("/tmp/var/restart_boa.sh");
		}
	}
	//TODO: lighttpd
}

static int _le_jobs_install(int min)
{
	char buf[128];
	char *argv[] = {"/usr/script/cru",
		"a", LE_TAG,
		buf,	//*/5 * * * * service restart_letsencrypt
		NULL };
	int le_jobs_min = tcapi_get_int(NODE_LE_CONF, "le_jobs_min");
	unsigned long sn, rand_hr, rand_min;

	if(le_jobs_min || min) {
		snprintf(buf, sizeof(buf), "*/%d * * * * service restart_letsencrypt", le_jobs_min?:min);
	}
	else {
		f_read("/dev/urandom", &sn, sizeof(sn));
		rand_hr = sn % 24;
		f_read("/dev/urandom", &sn, sizeof(sn));
		rand_min = sn % 60;
		snprintf(buf, sizeof(buf), "%lu %lu */%d * * service restart_letsencrypt", rand_min, rand_hr, LE_JOBS_DAY);
	}
	return _eval(argv, NULL, 0, NULL);
}

static int _le_jobs_remove(void)
{
	char *argv[] = {"/usr/script/cru",
		"d", LE_TAG,
		NULL };

	return _eval(argv, NULL, 0, NULL);
}

static int _le_acme_need_issue(char *domain)
{
	char buf[256] = {0};

	snprintf(buf, sizeof(buf), "%s/%s/%s.cer", ACME_CERTHOME, domain, domain);
	if(is_correct_le_certificate(buf)) {
		tcapi_set_int(NODE_LE_STAT, "le_state", LE_ST_ISSUED);
		return 0;
	}
	else
		return 1;
}

static int _le_acme_need_renew(char *domain)
{
	char buf[256] = {0};
	FILE *fp;
	time_t cur_time, renew_time;
	int ret = 1;

	snprintf(buf, sizeof(buf), "%s/%s/%s.conf", ACME_CERTHOME, domain, domain);
	if(!f_exists(buf)) {
		tcapi_set(NODE_LE_CONF, "le_acme_force", "1");
		return 1;
	}

	fp = fopen(buf, "r");
	if(fp) {
		while(fgets(buf, sizeof(buf), fp)) {
			if(strstr(buf, "Le_NextRenewTime=")) {
				time(&cur_time);
				sscanf(buf, "Le_NextRenewTime=\"%d\"", (int*)&renew_time);
				if( renew_time > cur_time)
					ret = 0;
				break;
			}
			else
				continue;
		}
		fclose(fp);
	}
	return ret;
}

static int _le_acme_need_revoke(char *domain)
{
	DIR *dirp;
	struct dirent *direntp;
	char buf[256] = {0};
	int ret = 0;

	if ((dirp = opendir(ACME_CERTHOME)) != NULL) {
		while((direntp = readdir(dirp)) != NULL) {
			if(direntp->d_type == DT_DIR
				|| d_exists(direntp->d_name)
			) {
				if(!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, "..")) {
					continue;
				}
				else if(strcmp(direntp->d_name, domain)) {
					snprintf(buf, sizeof(buf), "%s/%s/%s.cer", ACME_CERTHOME, direntp->d_name, direntp->d_name);
					if(f_exists(buf)) {
						ret = 1;
						tcapi_set_int(NODE_LE_STAT, "le_state", LE_ST_NONE);
						break;
					}
					else {
						snprintf(buf, sizeof(buf), "%s/%s", ACME_CERTHOME, direntp->d_name);
						eval("rm", "-rf", buf);
					}
				}
				else
					continue;
			}
			else
				continue;
		}
	}

	return ret;
}

/*
 * e.g.
 * acme.sh
 * --home /jffs/.le
 * --certhome /jffs/.le
 * --accountkey /jffs/.le/account.key
 * --accountconf /jffs/.le/account.conf
 * --domain samwentest.asuscomm.com
 * --issue
 * --dns dns_asusapi
 * --certpath /etc/cert.pem --keypath /etc/key.pem --reloadCmd "service restart_httpd"
 * --staging --debug 1 --force
 */
static void _le_acme_do(int cmd, char *domain, char *port)
{
	char *acme_argv[] = { ACME_PROGRAM,
		"--home", ACME_WORKHOME,
		"--certhome", ACME_CERTHOME,
		"--accountkey", ACME_ACCKEY,
		"--accountconf", ACME_ACCCONF,
		"--domain", domain,
		NULL,
		NULL, NULL,	NULL,	//--dns dns_asusapi, or --statndalone --httpport xx, or --webroot /www
		NULL, NULL,	//--debug 2
		NULL,	//--force
		NULL,	//--staging
		NULL };
	int index = 11;	//first NULL
	char le_acme_debug[4] = {0};
	char le_acme_logpath[128] = {0};

	_dprintf("%s %d %s\n", __FUNCTION__, cmd, domain);
	if(cmd == ACME_REVOKE) {
		acme_argv[index++] = "--revoke";
	}
	else {
		if(cmd == ACME_ISSUE)
			acme_argv[index++] = "--issue";
		else if(cmd == ACME_RENEW)
			acme_argv[index++] = "--renew";
		else
			return;

		if(tcapi_match(NODE_LE_CONF, "le_acme_auth", "dns")) {
			acme_argv[index++] = "--dns";
			if(tcapi_match(NODE_DDNS_CONF, "ddns_server_x", "WWW.ASUS.COM"))
				acme_argv[index++] = "dns_asusapi";
			else
				return;
		}
		else if(tcapi_match(NODE_LE_CONF, "le_acme_auth", "tls")) {
			acme_argv[index++] = "--tls";
			if(port) {
				acme_argv[index++] = "--tlsport";
				acme_argv[index++] = port;
			}
			_dprintf("%s challenge with tls port %s\n", __FUNCTION__, port?:"443");
		}
		else if(tcapi_match(NODE_LE_CONF, "le_acme_auth", "http")) {
			acme_argv[index++] = "--standalone";
			if(port) {
				acme_argv[index++] = "--httpport";
				acme_argv[index++] = port;
			}
			_dprintf("%s challenge with http port %s\n", __FUNCTION__, port?:"80");
		}
			else
				return;
	}

	//debug
	if(tcapi_get_int(NODE_LE_CONF, "le_acme_debug")) {
		acme_argv[index++] = "--debug";
		tcapi_get(NODE_LE_CONF, "le_acme_debug", le_acme_debug);
		acme_argv[index++] = le_acme_debug;
	}
	if(tcapi_get_int(NODE_LE_CONF, "le_acme_force"))
		acme_argv[index++] = "--force";
	if(tcapi_get_int(NODE_LE_CONF, "le_acme_stage"))
		acme_argv[index++] = "--staging";

	//log
	tcapi_get(NODE_LE_CONF, "le_acme_logpath", le_acme_logpath);
	if(strlen(le_acme_logpath)) {
		char path[128] = {0};
		if(le_acme_logpath[0] == '>')
			snprintf(path, sizeof(path), "%s", le_acme_logpath);
		else
			snprintf(path, sizeof(path), ">%s", le_acme_logpath);
//int i = 0; _dprintf("\n=====\n"); for(i=0; acme_argv[i]; i++) _dprintf("%s ", acme_argv[i]); _dprintf("\n=====\n");
		_eval(acme_argv, path, 0, NULL);
	}
	else {
		_eval(acme_argv, ">/dev/console", 0, NULL);
	}
}

static char* _pick_port(char *port, size_t len)
{
	int sockfd;
	struct sockaddr_in addr;
	socklen_t slen;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		return NULL;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = 0;
	slen = sizeof(struct sockaddr_in);
	if (bind(sockfd, (struct sockaddr *) &addr, slen) < 0) {
		close(sockfd);
		return NULL;
	}
	if(getsockname(sockfd, (struct sockaddr *) &addr, &slen) < 0) {
		close(sockfd);
		return NULL;
	}
	snprintf(port, len, "%d", ntohs (addr.sin_port));
	close(sockfd);
	return port;
}

static void _le_add_fw_rule(int le_acme_auth, const char* port)
{
	FILE* fp;
	char lan_ipaddr[16] = {0};

	tcapi_get("Lan_Entry0", "IP", lan_ipaddr);

	fp = fopen(LE_FW_SCRIPT, "w");
	if(fp) {
		fprintf(fp, "#!/bin/sh\n\n");
		fprintf(fp, "iptables -I INPUT -m conntrack --ctstate DNAT -p tcp -m tcp"
			" -d %s --dport %s -j ACCEPT\n", lan_ipaddr, port);
		fprintf(fp, "iptables -t nat -I VSERVER -p tcp -m tcp --dport %d"
			" -j DNAT --to-destination %s:%s\n",
			(le_acme_auth == ACME_AUTH_TLS)?443:80,
			lan_ipaddr, port);
		fclose(fp);
		chmod(LE_FW_SCRIPT, S_IRUSR|S_IWUSR|S_IXUSR);
		eval(LE_FW_SCRIPT);
	}
}

static void _le_del_fw_rule()
{
	if(f_exists(LE_FW_SCRIPT)) {
		eval("sed", "-i", "s/-I/-D/", LE_FW_SCRIPT);
		eval(LE_FW_SCRIPT);
		unlink(LE_FW_SCRIPT);
	}
}

static void _le_acme_do_issue(char *domain, int le_acme_auth)
{
	if(le_acme_auth == ACME_AUTH_DNS) {
		_le_acme_do(ACME_ISSUE, domain, NULL);
	}
	else {
		char port[8];
		_pick_port(port, sizeof(port));
		_le_add_fw_rule(le_acme_auth, port);
		_le_acme_do(ACME_ISSUE, domain, port);
		_le_del_fw_rule();
	}
}

static void _le_acme_do_renew(char *domain, int le_acme_auth)
{
	if(le_acme_auth == ACME_AUTH_DNS) {
		_le_acme_do(ACME_RENEW, domain, NULL);
	}
	else {
		char port[8];
		_pick_port(port, sizeof(port));
		_le_add_fw_rule(le_acme_auth, port);
		_le_acme_do(ACME_RENEW, domain, port);
		_le_del_fw_rule();
	}
}

static void _le_acme_do_revoke(char *domain, int force)
{
	DIR *dirp;
	struct dirent *direntp;
	char buf[256] = {0};

	if((dirp = opendir(ACME_CERTHOME)) == NULL)
		return;
	while((direntp = readdir(dirp)) != NULL) {
		if(direntp->d_type == DT_DIR
			|| d_exists(direntp->d_name)
		) {
			if(!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, "..")) {
				continue;
			}
			else if(strcmp(direntp->d_name, domain) || force) {
				snprintf(buf, sizeof(buf), "%s/%s/%s.cer", ACME_CERTHOME, direntp->d_name, direntp->d_name);
				if(f_exists(buf)) {
					snprintf(buf, sizeof(buf), "%s", direntp->d_name);
					_le_acme_do(ACME_REVOKE, buf, NULL);
				}
				else {
					snprintf(buf, sizeof(buf), "%s/%s", ACME_CERTHOME, direntp->d_name);
					eval("rm", "-rf", buf);
				}
			}
			else
				continue;
		}
		else
			continue;
	}
}

static void _le_acme_result_handler(char *domain)
{
	char cert_path[256] = {0};

	_le_jobs_remove();

	snprintf(cert_path, sizeof(cert_path), "%s/%s/%s.cer", ACME_CERTHOME, domain, domain);
	if(is_correct_le_certificate(cert_path)) {
		_le_install_certificate(cert_path);
		_le_jobs_install(0);
		tcapi_set(NODE_LE_CONF, "le_acme_force", "0");
		tcapi_set_int(NODE_LE_STAT, "le_state", LE_ST_ISSUED);
		tcapi_set_int(NODE_LE_STAT, "le_state_t", LE_STS_WAIT_RENEW);
	}
	else {
		_le_jobs_install(LE_JOBS_MIN);
		tcapi_set_int(NODE_LE_STAT, "le_state_t", LE_STS_WAIT_RETRY);
		tcapi_set_int(NODE_LE_STAT, "le_auxstate_t", LE_AUX_ACME);
	}
}

static void _le_acme_term(int signo)
{
	_le_acme_exit = 1;
	killall_tk(ACME_PROGRAM);
}

int start_letsencrypt(void)
{
	pid_t pid;
	FILE *fp;
	char *acme_argv[] = {"/sbin/le_acme", NULL};
	char path[256];
	char myhost[128] = {0};

	tcapi_set(NODE_LE_STAT, "le_sbstate_t", "0");
	tcapi_set(NODE_LE_STAT, "le_auxstate_t", "0");
	tcapi_set_int(NODE_LE_STAT, "le_state_t", LE_STS_CHECKING);

	if(_le_check_configure())
		return 0;

	_le_check_dir();

	if(_le_check_status()) {
		_eval(acme_argv, NULL, 0, &pid);
		if ((fp = fopen(LE_PID_FILE, "w")) != NULL) {
			fprintf(fp, "%u", pid);
			fclose(fp);
		}
	}
	else {
		_le_jobs_install(LE_JOBS_MIN);
		tcapi_set_int(NODE_LE_STAT, "le_state_t", LE_STS_WAIT_RETRY);

		tcapi_get(NODE_DDNS_CONF, "MYHOST", myhost);
		snprintf(path, sizeof(path), "%s/%s/%s.cer", ACME_CERTHOME, myhost, myhost);
		if(is_correct_le_certificate(path))
			tcapi_set_int(NODE_LE_STAT, "le_state", LE_ST_ISSUED);
	}

	return 0;
}

int stop_letsencrypt(void)
{
	tcapi_set_int(NODE_LE_STAT, "le_state_t", LE_STS_STOPPING);
	kill_pidfile_s_rm(LE_PID_FILE, SIGTERM);
	_le_jobs_remove();
	tcapi_set_int(NODE_LE_STAT, "le_state_t", LE_STS_STOPPED);
	return 0;
}

int le_acme_main(int argc, char **argv)
{
	char domain[128] = {0};
	int le_acme_force = 0;
	int le_acme_auth = ACME_AUTH_DNS;

	tcapi_get(NODE_DDNS_CONF, "MYHOST", domain);
	le_acme_force = tcapi_get_int(NODE_LE_CONF, "le_acme_force");
	if(tcapi_match(NODE_LE_CONF, "le_acme_auth", "tls"))
		le_acme_auth = ACME_AUTH_TLS;
	else if(tcapi_match(NODE_LE_CONF, "le_acme_auth", "dns"))
		le_acme_auth = ACME_AUTH_DNS;
	else
		le_acme_auth = ACME_AUTH_HTTP;

	_le_acme_exit = 0;
	signal(SIGTERM, _le_acme_term);

	if(!_le_acme_exit && (_le_acme_need_revoke(domain) || le_acme_force)) {
		tcapi_set_int(NODE_LE_STAT, "le_state_t", LE_STS_REVOKE);
		_le_acme_do_revoke(domain, le_acme_force);
	}
	if(!_le_acme_exit) {
		if(_le_acme_need_issue(domain) || le_acme_force) {
			tcapi_set_int(NODE_LE_STAT, "le_state_t", LE_STS_ISSUE);
			_le_acme_do_issue(domain, le_acme_auth);
		}
		else if(_le_acme_need_renew(domain)) {
			tcapi_set_int(NODE_LE_STAT, "le_state_t", LE_STS_RENEW);
			_le_acme_do_renew(domain, le_acme_auth);
		}
	}
	if(!_le_acme_exit)
		_le_acme_result_handler(domain);

	return 0;
}

/*
 * copy letsencrypt certificate
 * success: return 0
 */
int copy_le_certificate(char *dst_cert, char *dst_key)
{
	char path[256] = {0};
	char myhost[128] = {0};
	char *argv[] = {
		"cp", path, NULL,
		NULL };
	int ret = 0;

	tcapi_get(NODE_DDNS_CONF, "MYHOST", myhost);
	snprintf(path, sizeof(path), "%s/%s/%s.cer", ACME_CERTHOME, myhost, myhost);
	if(!f_exists(path))
		return -1;
	argv[2] = dst_cert;
	ret |= _eval(argv, NULL, 0, NULL);

	snprintf(path, sizeof(path), "%s/%s/%s.key", ACME_CERTHOME, myhost, myhost);
	if(!f_exists(path))
		return -1;
	argv[2] = dst_key;
	ret |= _eval(argv, NULL, 0, NULL);

	return ret;
}

static void _get_common_name(char *src, char *dst, size_t len)
{
	char *pb, *pe;

	if((pb = strstr(src, "CN=")) != NULL)
		pb += 3;
	else
		pb = src;

	if((pe = strchr(pb, '/')) != NULL)
		*pe = '\0';

	snprintf(dst, len, "%s", pb);
}

/*
 * check the issuer, subject of certificate
 * issuer should be Let's Encrypt
 * common name of subject should be the ddns hostname
 */
int is_correct_le_certificate(char *cert_path)
{
	FILE *fp;
	X509 *x509data = NULL;
	char buf[256] = {0};
	char issuer[64] = {0};
	char subject[64] = {0};
	int ret = 0;

	if(!f_exists(cert_path))
		return 0;

	fp = fopen(cert_path, "r");
	if(!fp)
		return 0;
	if(!PEM_read_X509(fp, &x509data, NULL, NULL))
		d2i_X509_fp(fp, &x509data);
	fclose(fp);
	if(x509data == NULL)
		return 0;

	X509_NAME_oneline(X509_get_issuer_name(x509data), buf, sizeof(buf));
	_get_common_name(buf, issuer, sizeof(issuer));
	if(strstr(issuer, "Let's Encrypt")) {
		ret = 1;
	}
	else if(tcapi_get_int(NODE_LE_CONF, "le_acme_stage") && strstr(issuer,"Fake LE Intermediate")) {
		ret = 1;
	}
	X509_NAME_oneline(X509_get_subject_name(x509data), buf, sizeof(buf));
	_get_common_name(buf, subject, sizeof(subject));
	memset(buf, 0, sizeof(buf));
	tcapi_get(NODE_DDNS_CONF, "MYHOST", buf);
	if(strcmp(buf, subject))
		ret = 0;

	return ret;
}
