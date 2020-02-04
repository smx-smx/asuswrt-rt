/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifdef RTCONFIG_USB_MODEM
#include <usb_info.h>
#endif

#include "wanduck.h"
#include "libtcapi.h"
#include "tcutils.h"
#include <shared.h>

#define NO_DETECT_INTERNET

static void safe_leave(int signo){
	csprintf("\n## wanduck.safeexit ##\n");
	signal(SIGTERM, SIG_IGN);
	signal(SIGUSR1, SIG_IGN);
	signal(SIGINT, SIG_IGN);

	FD_ZERO(&allset);

	int i, ret;
	for(i = 0; i <= maxfd; ++i){
		ret = close(i);
		csprintf("## close %d: result=%d.\n", i, ret);
		if(ret == -1)
			csprintf("## error= %s.\n", strerror(errno));
	}

#ifndef RTCONFIG_BCMARM
	sleep(1);
#endif

	if(rule_setup == 1){
		int len;
		char *fn = NAT_RULES, ln[PATH_MAX];
		struct stat s;

		csprintf("\n# Disable direct rule(exit wanduck)\n");

		rule_setup = 0;
		conn_changed_state[current_wan_unit] = CONNED; // for cleaning the redirect rules.

#if 0
		// in the function safe_leave(), couldn't set any nvram using nvram_set().
		// So the notify mechanism would be invalid.
#if 0
		handle_wan_line(current_wan_unit, rule_setup);
#else // or
		start_nat_rules();
#endif
#else
		// Couldn't directly use nvram_set().
		// Must use the command: nvram, and set the nat_state.
		char buf[16];
		FILE *fp = fopen(NAT_RULES, "r");

		memset(buf, 0, 16);
		if(fp != NULL){
			fclose(fp);

			if (!lstat(NAT_RULES, &s) && S_ISLNK(s.st_mode) &&
			    (len = readlink(NAT_RULES, ln, sizeof(ln))) > 0) {
				ln[len] = '\0';
				fn = ln;
			}

#if ASUSWRT
			sprintf(buf, "nat_state=%d", NAT_STATE_NORMAL);
			eval("nvram", "set", buf);
#else
			sprintf(buf, "%d", NAT_STATE_NORMAL);
			tcapi_set("Wanduck_Common", "nat_state", buf);
#endif

			csprintf("%s: apply the nat_rules(%s): %s!\n", __FUNCTION__, fn, buf);
			logmessage("wanduck exit", "apply the nat_rules(%s)!", fn);

			setup_ct_timeout(TRUE);
			setup_udp_timeout(TRUE);

#if ASUSWRT
			eval("iptables-restore", NAT_RULES);
#else
			tcapi_commit("Wanduck");
#endif
		}
		else{
#if ASUSWRT
			sprintf(buf, "nat_state=%d", NAT_STATE_INITIALIZING);
			eval("nvram", "set", buf);
#else
			sprintf(buf, "%d", NAT_STATE_INITIALIZING);
			tcapi_set("Wanduck_Common", "nat_state", buf);
#endif

			csprintf("%s: initial the nat_rules: %s!\n", __FUNCTION__, buf);
			logmessage("wanduck exit", "initial the nat_rules!");
		}
#endif
	}

	remove(WANDUCK_PID_FILE);

	csprintf("\n# return(exit wanduck)\n");
	exit(0);
}

void get_related_nvram(){
#if ASUSWRT
	sw_mode = nvram_get_int("sw_mode");
#endif
	boot_end = nvram_get_int("success_start_service");

	if(nvram_match("x_Setting", "1"))
		isFirstUse = 0;
	else
		isFirstUse = 1;

	if(nvram_match("nat_redirect_enable", "0"))
		nat_redirect_enable = 0;
	else
		nat_redirect_enable = 1;

#ifdef RTCONFIG_DUALWAN
	memset(dualwan_mode, 0, 8);
	strcpy(dualwan_mode, nvram_safe_get("wans_mode"));

	memset(wandog_target, 0, PATH_MAX);
	if(sw_mode == SW_MODE_ROUTER){
		wandog_enable = nvram_get_int("wandog_enable");
		scan_interval = nvram_get_int("wandog_interval");
		max_disconn_count = nvram_get_int("wandog_maxfail");
		wandog_delay = nvram_get_int("wandog_delay");

		if((!strcmp(dualwan_mode, "fo") || !strcmp(dualwan_mode, "fb"))
				&& wandog_enable == 1
				){
			strncpy(wandog_target,
				nvram_invmatch("wandog_target", "") ?
				nvram_safe_get("wandog_target") :
				"8.8.8.8", sizeof(wandog_target)-1);
		}

		if(!strcmp(dualwan_mode, "fb")){
			max_fb_count = nvram_get_int("wandog_fb_count");
			max_fb_wait_time = scan_interval*max_fb_count;
		}

		if (scan_interval == 0)
			scan_interval = DEFAULT_SCAN_INTERVAL;
	}
	else{
		wandog_enable = 0;
		scan_interval = DEFAULT_SCAN_INTERVAL;
		max_disconn_count = DEFAULT_MAX_DISCONN_COUNT;
		wandog_delay = -1;
	}
#else
	wandog_enable = 0;
	scan_interval = DEFAULT_SCAN_INTERVAL;
	max_disconn_count = DEFAULT_MAX_DISCONN_COUNT;
#endif
	max_wait_time = scan_interval*max_disconn_count;

	Force_off_GuestWL = nvram_get_int("wandog_off_guestwl");
	Resume_GuestWL = nvram_get_int("wandog_resume_guestwl");
}

#if ASUSWRT
void get_lan_nvram(){
	char nvram_name[16];

	current_lan_unit = nvram_get_int("lan_unit");

	memset(prefix_lan, 0, 8);
	if(current_lan_unit < 0)
		strcpy(prefix_lan, "lan_");
	else
		sprintf(prefix_lan, "lan%d_", current_lan_unit);

	memset(current_lan_ifname, 0, 16);
	strcpy(current_lan_ifname, nvram_safe_get(strcat_r(prefix_lan, "ifname", nvram_name)));

	memset(current_lan_proto, 0, 16);
	strcpy(current_lan_proto, nvram_safe_get(strcat_r(prefix_lan, "proto", nvram_name)));

	memset(current_lan_ipaddr, 0, 16);
	strcpy(current_lan_ipaddr, nvram_safe_get(strcat_r(prefix_lan, "ipaddr", nvram_name)));

	memset(current_lan_netmask, 0, 16);
	strcpy(current_lan_netmask, nvram_safe_get(strcat_r(prefix_lan, "netmask", nvram_name)));

	memset(current_lan_gateway, 0, 16);
	strcpy(current_lan_gateway, nvram_safe_get(strcat_r(prefix_lan, "gateway", nvram_name)));

	memset(current_lan_dns, 0, 256);
	strcpy(current_lan_dns, nvram_safe_get(strcat_r(prefix_lan, "dns", nvram_name)));

	memset(current_lan_subnet, 0, 11);
	strcpy(current_lan_subnet, nvram_safe_get(strcat_r(prefix_lan, "subnet", nvram_name)));

	csprintf("# wanduck: Got LAN(%d) information:\n", current_lan_unit);
	if(nvram_match("wanduck_debug", "1")){
		csprintf("# wanduck:   ifname=%s.\n", current_lan_ifname);
		csprintf("# wanduck:    proto=%s.\n", current_lan_proto);
		csprintf("# wanduck:   ipaddr=%s.\n", current_lan_ipaddr);
		csprintf("# wanduck:  netmask=%s.\n", current_lan_netmask);
		csprintf("# wanduck:  gateway=%s.\n", current_lan_gateway);
		csprintf("# wanduck:      dns=%s.\n", current_lan_dns);
		csprintf("# wanduck:   subnet=%s.\n", current_lan_subnet);
	}
}

static void get_network_nvram(int signo){
	if(sw_mode == SW_MODE_AP)
		get_lan_nvram();
}
#endif

int do_ping_detect(int wan_unit){
#ifdef RTCONFIG_DUALWAN
	char cmd[256];
#endif
#if 0
	char buf[16], *next;
	char prefix_wan[8], nvram_name[16], wan_dns[256];

	memset(prefix_wan, 0, 8);
	sprintf(prefix_wan, "wan%d_", wan_unit);

	memset(wan_dns, 0, 256);
	strcpy(wan_dns, nvram_safe_get(strcat_r(prefix_wan, "dns", nvram_name)));

	foreach(buf, wan_dns, next){
		sprintf(cmd, "ping -c 1 -W %d %s && touch %s", TCPCHECK_TIMEOUT, buf, PING_RESULT_FILE);
		system(cmd);

		if(check_if_file_exist(PING_RESULT_FILE)){
			unlink(PING_RESULT_FILE);
			return 1;
		}
	}
#elif defined(RTCONFIG_DUALWAN)
	if((wandog_target_timeout >= 3) && (strncmp(wandog_target, "8.8.8.8", 7) != 0)) {
		tcapi_set(WEBCURSET_DATA, "wandog_pingfail", "1");
		tcapi_set(WEBCURSET_DATA, "wandog_target_orig", wandog_target);
		tcapi_set(DUALWAN_DATA, "wandog_target", "8.8.8.8");
		strncpy(wandog_target, "8.8.8.8", sizeof(wandog_target)-1);
	}
	char *delim = ".";
	char *pch;
	char buf[128], tmp[128];
	snprintf(buf, sizeof(buf), "%s", wandog_target);
	pch = strtok(buf, delim);
	sprintf(tmp, "%d", atoi(pch));

	if(strncmp(tmp, pch, strlen(pch)) == 0) {
		csprintf("wanduck: gethostbyaddr %s ...", wandog_target);
		struct hostent *stHost = (struct hostent *)NULL;
		struct in_addr stInAddr, stTempInAddr;
		int idx = -1;
		struct sockaddr_in stSkAddr;

		bzero((void *)&stInAddr, sizeof(struct in_addr));
		inet_aton(wandog_target, &stInAddr);
		if(gethostbyaddr(&stInAddr, sizeof(struct in_addr), AF_INET) != NULL) {
			csprintf("ok.\n");
			wandog_target_timeout = 0;
			return 1;
		}
		else
			wandog_target_timeout++;
	}
	else {
		csprintf("wanduck: gethostbyname %s ...", wandog_target);
		if(gethostbyname(wandog_target) != NULL) {
			csprintf("ok.\n");
			wandog_target_timeout = 0;
			return 1;
		}
		else
			wandog_target_timeout++;
	}
#else
	return 1;
#endif
	_dprintf("\n ping failed.\n");
	return 0;
}

#ifndef NO_DETECT_INTERNET
int get_packets_of_net_dev(const char *net_dev, unsigned long *rx_packets, unsigned long *tx_packets){
	FILE *fp;
	char buf[256];
	char *ifname;
	char *ptr;
	int i, got_packets = 0;

	if((fp = fopen(PROC_NET_DEV, "r")) == NULL){
		_dprintf("%s: Can't open the file: %s.\n", __FUNCTION__, PROC_NET_DEV);
		return got_packets;
	}

	fcntl(fileno(fp), F_SETFL, fcntl(fileno(fp), F_GETFL) | O_NONBLOCK);

	// headers.
	for(i = 0; i < 2; ++i){
		if(fgets(buf, sizeof(buf), fp) == NULL){
			csprintf("get_packets_of_net_dev: Can't read out the headers of %s.\n", PROC_NET_DEV);
			fclose(fp);
			logmessage("wanduck", "%s: Can't read out the headers of %s.\n", __FUNCTION__, PROC_NET_DEV);
			if(errno == EAGAIN || errno == EWOULDBLOCK)
				got_packets = 1;

			return got_packets;
		}
	}

	while(fgets(buf, sizeof(buf), fp) != NULL){
		if((ptr = strchr(buf, ':')) == NULL)
			continue;

		*ptr = 0;
		if((ifname = strrchr(buf, ' ')) == NULL)
			ifname = buf;
		else
			++ifname;

		if(strcmp(ifname, net_dev))
			continue;

		// <rx bytes, packets, errors, dropped, fifo errors, frame errors, compressed, multicast><tx ...>
		if(sscanf(ptr+1, "%*u%lu%*u%*u%*u%*u%*u%*u%*u%lu", rx_packets, tx_packets) != 2){
			csprintf("get_packets_of_net_dev: Can't read the packet's number in %s.\n", PROC_NET_DEV);
			fclose(fp);
			logmessage("wanduck", "%s: Can't read the packet's number in %s.\n", __FUNCTION__, PROC_NET_DEV);
			if(errno == EAGAIN || errno == EWOULDBLOCK)
				got_packets = 1;
		
			return got_packets;
		}

		got_packets = 1;
		break;
	}
	fclose(fp);

	return got_packets;
}

char *organize_tcpcheck_cmd(char *dns_list, char *cmd, int size){
	char buf[256], *next;

	if(cmd == NULL || size <= 0)
		return NULL;

	memset(cmd, 0, size);

	sprintf(cmd, "/sbin/tcpcheck %d", TCPCHECK_TIMEOUT);

	foreach(buf, dns_list, next){
		sprintf(cmd, "%s %s:53", cmd, buf);
	}

	sprintf(cmd, "%s >>%s", cmd, DETECT_FILE);

	return cmd;
}

int do_tcp_dns_detect(int wan_unit){
	FILE *fp = NULL;
	char line[80], cmd[PATH_MAX];
	char prefix_wan[8], nvram_name[16], wan_dns[256];

	if(remove(DETECT_FILE) < 0)
	{
		logmessage("%s: cannot remove DETECT_FILE.\n", __FUNCTION__);
		return 0;
	}

	memset(prefix_wan, 0, 8);
	sprintf(prefix_wan, "wan%d_", wan_unit);

	memset(wan_dns, 0, 256);
	strcpy(wan_dns, nvram_safe_get(strcat_r(prefix_wan, "dns", nvram_name)));

	if(organize_tcpcheck_cmd(wan_dns, cmd, PATH_MAX) == NULL){
		csprintf("wanduck: No tcpcheck cmd.\n");
		return 0;
	}
	system(cmd);

	if((fp = fopen(DETECT_FILE, "r")) == NULL){
		_dprintf("wanduck: No file: %s.\n", DETECT_FILE);
		return 0;
	}
	
	fcntl(fileno(fp), F_SETFL, fcntl(fileno(fp), F_GETFL) | O_NONBLOCK);

	while(fgets(line, sizeof(line), fp) != NULL){
		if(strstr(line, "alive")){
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);

	return 0;
}
#endif // NO_DETECT_INTERNET

static void do_none_default_route_ping(int wan_unit)
{
	int i;
	char node[MAXLEN_NODE_NAME];
	char attr[MAXLEN_ATTR_NAME];
	char gateway[16];
	char cmd[128];

	//_dprintf("%s: wan_unit: %d\n", __FUNCTION__, wan_unit);

	if(wan_unit == WAN_UNIT_ATM) {
		for(i = 0; i < 8; i++) {
			snprintf(node, sizeof(node), "Wan_PVC%d", i);
			if(tcapi_match(node, "Active", "Yes")
				&& !tcapi_match(node, "DEFAULTROUTE", "Yes")
			) {
				snprintf(attr, sizeof(attr), "wan%d_gateway", i);
				snprintf(cmd, sizeof(cmd), "ping -c 1 -W 1 %s &", tcapi_get_string("Wanduck_Common", attr, gateway));
				system(cmd);
			}
		}
	}
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
	else if(wan_unit == WAN_UNIT_PTM0 || wan_unit == WAN_UNIT_ETH) {
		for(i = 0; i < 8; i++) {
			snprintf(node, sizeof(node), "WanExt_PVC%de%d", wan_unit, i);
			if(tcapi_match(node, "Active", "Yes")
				&& !tcapi_match(node, "DEFAULTROUTE", "Yes")
			) {
				snprintf(attr, sizeof(attr), "wan%d%d_gateway", wan_unit, i);
				snprintf(cmd, sizeof(cmd), "ping -c 1 -W 1 %s -&", tcapi_get_string("Wanduck_Common", attr, gateway));
				system(cmd);
			}
		}
	}
#endif
	else {
		snprintf(node, sizeof(node), "Wan_PVC%d", wan_unit);
		if(tcapi_match(node, "Active", "Yes")
			 && !tcapi_match(node, "DEFAULTROUTE", "Yes")
		) {
			snprintf(attr, sizeof(attr), "wan%d_gateway", wan_unit);
			snprintf(cmd, sizeof(cmd), "ping -c 1 -W 1 %s &", tcapi_get_string("Wanduck_Common", attr, gateway));
			system(cmd);
		}
	}
}

int detect_internet(int wan_unit){
	int link_internet;
#ifndef NO_DETECT_INTERNET
	unsigned long rx_packets, tx_packets;
#endif
	char prefix_wan[8], wan_ifname[16];
	int ori_link_internet;	//Andy Chiu, 2015/11/11
	char buf[32] = {0}, buf2[32] = {0};		//Andy Chiu, 2015/11/11

	memset(prefix_wan, 0, 8);
	sprintf(prefix_wan, "wan%d_", wan_unit);

	memset(wan_ifname, 0, 16);
	strcpy(wan_ifname, get_wan_ifname(wan_unit));

	//Andy Chiu, 2015/11/11. Get original link_internet.
/*	//Andy Chiu, 2015/11/18. There is some problem on this modification on main trunk. 
	memset(buf, 0, sizeof(buf));
	memset(buf2, 0, sizeof(buf2));
	sprintf(buf, "link_wan%d", wan_unit);
	tcapi_get("Wanduck_Common", buf, buf2);
	ori_link_internet = atoi(buf2);
*/
	if(
#ifdef RTCONFIG_DUALWAN
			strcmp(dualwan_mode, "lb") &&
#endif
			!found_default_route(wan_unit)
			)
		link_internet = DISCONN;
#ifndef NO_DETECT_INTERNET
	else if(!get_packets_of_net_dev(wan_ifname, &rx_packets, &tx_packets) || rx_packets <= RX_THRESHOLD)
		link_internet = DISCONN;
	else if(!isFirstUse && (!do_dns_detect() && !do_tcp_dns_detect(wan_unit) && !do_ping_detect(wan_unit)))
		link_internet = DISCONN;
#endif
#ifdef RTCONFIG_DUALWAN
	else if((!strcmp(dualwan_mode, "fo") || !strcmp(dualwan_mode, "fb"))
			&& wandog_enable == 1 && !isFirstUse && !do_ping_detect(wan_unit))
		link_internet = DISCONN;
#endif
	else
		link_internet = CONNED;

	if(link_internet == DISCONN){
#ifndef NO_DETECT_INTERNET
		nvram_set_int("link_internet", 1);
#endif
		record_wan_state_nvram(wan_unit, -1, -1, WAN_AUXSTATE_NO_INTERNET_ACTIVITY);

		if(!(nvram_get_int("web_redirect")&WEBREDIRECT_FLAG_NOINTERNET)) {
#ifndef NO_DETECT_INTERNET
			nvram_set_int("link_internet", 2);
#endif
			link_internet = CONNED;
		}
	}
	else{
#ifndef NO_DETECT_INTERNET
		nvram_set_int("link_internet", 2);
#endif
		record_wan_state_nvram(wan_unit, -1, -1, WAN_AUXSTATE_NONE);
	}

	do_none_default_route_ping(wan_unit);

	//Andy Chiu, 2015/11/11
#if 0		//Andy Chiu, 2015/11/18. There is some problem on this modification on main trunk. 
	if(link_internet != ori_link_internet)	//check current link status and old
	{
		if(0 == link_internet)	//check link status, if it's different from old, update Wanduck status
		{
			//get state_t
			sprintf(prefix_wan, "wan%d_state_t", wan_unit);
			tcapi_get("Wanduck_Common", prefix_wan, buf);
			int conn_status = atoi(buf);

			if(conn_status == 2)
			{
				char prefix[] = "wanXXXXXXXXXX_";
				//wan_down(get_wan_ifname(wan_unit));
				_dprintf("%s(%s)\n", __FUNCTION__, wan_ifname);

				/* Skip physical interface of VPN connections */
				if(wan_ifunit(wan_ifname) < 0)
					return link_internet;

				/* Figure out nvram variable name prefix for this i/f */
				if(wan_prefix(wan_ifname, prefix) < 0)
					return link_internet;

				update_wan_state(prefix, WAN_STATE_DISCONNECTED, WAN_STOPPED_REASON_NONE);
			}
		}
	}
#endif	
	return link_internet;
}

int passivesock(char *service, int protocol_num, int qlen){
	//struct servent *pse;
	struct sockaddr_in sin;
	int s, type, on;
	
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	
	// map service name to port number
	if((sin.sin_port = htons((u_short)atoi(service))) == 0){
		perror("cannot get service entry");
		
		return -1;
	}
	
	if(protocol_num == IPPROTO_UDP)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;
	
	s = socket(PF_INET, type, protocol_num);
	if(s < 0){
		perror("cannot create socket");
		
		return -1;
	}
	
	on = 1;
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0){
		perror("cannot set socket's option: SO_REUSEADDR");
		close(s);
		
		return -1;
	}
	
	if(bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0){
		perror("cannot bind port");
		close(s);
		
		return -1;
	}
	
	if(type == SOCK_STREAM && listen(s, qlen) < 0){
		perror("cannot listen to port");
		close(s);
		
		return -1;
	}
	
	return s;
}

#if 0 
int check_ppp_exist(){
	DIR *dir;
	struct dirent *dent;
	char task_file[64], cmdline[64];
	int pid, fd;

	if((dir = opendir("/proc")) == NULL){
		perror("open proc");
		return 0;
	}

	while((dent = readdir(dir)) != NULL){
		if((pid = atoi(dent->d_name)) > 1){
			memset(task_file, 0, 64);
			sprintf(task_file, "/proc/%d/cmdline", pid);
			if((fd = open(task_file, O_RDONLY | O_NONBLOCK)) > 0){
				memset(cmdline, 0, 64);
				read(fd, cmdline, 64);
				close(fd);

				if(strstr(cmdline, "pppd")
						|| strstr(cmdline, "l2tpd")
						|| (errno==EAGAIN || errno==EWOULDBLOCK)
						){
					closedir(dir);
					return 1;
				}
			}
			else
				printf("cannot open %s\n", task_file);
		}
	}
	closedir(dir);
	
	return 0;
}
#endif

int chk_proto(int wan_unit, int wan_state){
	int wan_sbstate = nvram_get_int(nvram_sbstate[wan_unit]);
	char prefix_wan[8], nvram_name[16], wan_proto[16];

	memset(prefix_wan, 0, 8);
	sprintf(prefix_wan, "wan%d_", wan_unit);

	memset(wan_proto, 0, 16);
	strcpy(wan_proto, nvram_safe_get(strcat_r(prefix_wan, "proto", nvram_name)));

	// Start chk_proto() in SW_MODE_ROUTER.
#ifdef RTCONFIG_USB_MODEM
	if (dualwan_unit__usbif(wan_unit)) {
		if(wan_state == WAN_STATE_INITIALIZING){
			disconn_case[wan_unit] = CASE_PPPFAIL;
			return DISCONN;
		}
		else if(wan_state == WAN_STATE_CONNECTING){
			ppp_fail_state = pppstatus();
			if(ppp_fail_state == WAN_STOPPED_REASON_PPP_AUTH_FAIL)
				record_wan_state_nvram(wan_unit, -1, -1, WAN_AUXSTATE_PPP_AUTH_FAIL);

			disconn_case[wan_unit] = CASE_PPPFAIL;
			return DISCONN;
		}
		else if(wan_state == WAN_STATE_DISCONNECTED){
			disconn_case[wan_unit] = CASE_PPPFAIL;
			return DISCONN;
		}
		else if(wan_state == WAN_STATE_STOPPED){
			if(wan_sbstate == WAN_STOPPED_REASON_INVALID_IPADDR)
				disconn_case[wan_unit] = CASE_THESAMESUBNET;
			else
				disconn_case[wan_unit] = CASE_PPPFAIL;
			return DISCONN;
		}
	}
	else
#endif
	if(!strcmp(wan_proto, "dhcp")
			|| !strcmp(wan_proto, "static")){
		if(wan_state == WAN_STATE_STOPPED) {
			if(wan_sbstate == WAN_STOPPED_REASON_INVALID_IPADDR)
				disconn_case[wan_unit] = CASE_THESAMESUBNET;
			else disconn_case[wan_unit] = CASE_DHCPFAIL;
			return DISCONN;
		}
		else if(wan_state == WAN_STATE_INITIALIZING){
			disconn_case[wan_unit] = CASE_DHCPFAIL;
			return DISCONN;
		}
		else if(wan_state == WAN_STATE_CONNECTING){
			disconn_case[wan_unit] = CASE_DHCPFAIL;
			return DISCONN;
		}
		else if(wan_state == WAN_STATE_DISCONNECTED){
			disconn_case[wan_unit] = CASE_DHCPFAIL;
			return DISCONN;
		}
	}
	else if(!strcmp(wan_proto, "pppoe")
			|| !strcmp(wan_proto, "pptp")
			|| !strcmp(wan_proto, "l2tp")
			){
		ppp_fail_state = pppstatus();

		if(wan_state != WAN_STATE_CONNECTED
				&& ppp_fail_state == WAN_STOPPED_REASON_PPP_LACK_ACTIVITY){
			// PPP is into the idle mode.
			if(wan_state == WAN_STATE_STOPPED) // Sometimes ip_down() didn't set it.
				record_wan_state_nvram(wan_unit, -1, -1, WAN_STOPPED_REASON_PPP_LACK_ACTIVITY);
		}
		else if(wan_state == WAN_STATE_INITIALIZING){
			disconn_case[wan_unit] = CASE_PPPFAIL;
			return DISCONN;
		}
		else if(wan_state == WAN_STATE_CONNECTING){
			if(ppp_fail_state == WAN_STOPPED_REASON_PPP_AUTH_FAIL)
				record_wan_state_nvram(wan_unit, -1, -1, WAN_AUXSTATE_PPP_AUTH_FAIL);

			disconn_case[wan_unit] = CASE_PPPFAIL;
			return DISCONN;
		}
		else if(wan_state == WAN_STATE_DISCONNECTED){
			disconn_case[wan_unit] = CASE_PPPFAIL;
			return DISCONN;
		}
		else if(wan_state == WAN_STATE_STOPPED){
			disconn_case[wan_unit] = CASE_PPPFAIL;
			return DISCONN;
		}
	}
	else if(!strcmp(wan_proto, "bridge"))
	{
		disconn_case[wan_unit] = CASE_OTHERS;
	}

	return CONNED;
}

int if_wan_phyconnected(int wan_unit){
	char wired_link_nvram[16];
	int link_changed = 0;
	char prefix_wan[8], nvram_name[16], wan_proto[16];
#ifdef RTCONFIG_USB_MODEM
	int sim_state = 0;
#endif

#if ASUSWRT
	if(wan_unit)
		wired_link_nvram = "link_wan1";
	else
		wired_link_nvram = "link_wan";
#else
	sprintf(wired_link_nvram, "link_wan%d", wan_unit);
#endif

#ifdef RTCONFIG_USB_MODEM
	if(dualwan_unit__usbif(wan_unit)){
		char prefix[32] = "wanXXXXXX_", tmp[100] = "";
		int find_modem_node = 0;
		int wan_state = nvram_get_int(nvram_state[wan_unit]);

		modem_act_reset = nvram_get_int("usb_modem_act_reset");
		if(modem_act_reset == 1 || modem_act_reset == 2)
			return CONNED;

#if ASUSWRT
		if(wan_state == WAN_STATE_CONNECTING)
#else
		if(wan_state == WAN_STATE_USBMODEM_CONNECTING)
#endif
			return CONNED;

		link_wan[wan_unit] = is_usb_modem_ready(); // include find_modem_type.sh
		snprintf(modem_type, 32, "%s", nvram_safe_get("usb_modem_act_type"));
		sim_state = nvram_get_int("usb_modem_act_sim");

		if(!strcmp(modem_type, "qmi") || !strcmp(modem_type, "gobi")){
			if(link_wan[wan_unit] == 1){
				if(!strcmp(nvram_safe_get("usb_modem_act_int"), "")){
					if(!strcmp(modem_type, "qmi")){	// e.q. Huawei E398.
						csprintf("wanduck(%d): Sleep 3 seconds to wait modem nodes.\n", wan_unit);
						sleep(3);
					}
					else{
						csprintf("wanduck(%d): Sleep 2 seconds to wait modem nodes.\n", wan_unit);
						sleep(2);
					}

					wan_state = nvram_get_int(nvram_state[wan_unit]); // after sleep(), wan_state is changed.

					if(!strcmp(nvram_safe_get("usb_modem_act_int"), ""))
						find_modem_node = 1;
				}

				if((!strcmp(modem_type, "tty") || !strcmp(modem_type, "mbim"))
						&& !strcmp(nvram_safe_get("usb_modem_act_bulk"), "")
						&& strcmp(nvram_safe_get("usb_modem_act_vid"), "6610") // e.q. ZTE MF637U
						){
					csprintf("wanduck(%d): finding the second tty node...\n", wan_unit);
					find_modem_node = 1;
				}

				if(find_modem_node)
#if ASUSWRT
					eval("find_modem_node.sh");
#else
					system("find_modem_node.sh");
#endif

				if(!strcmp(modem_type, "tty") && !strcmp(nvram_safe_get("usb_modem_act_vid"), "6610")){ // e.q. ZTE MF637U
					if(wan_state == WAN_STATE_INITIALIZING)
#if ASUSWRT
						eval("modem_status.sh", "sim");
#else
						system("modem_status.sh sim");
#endif
				}
				else if(wan_state != WAN_STATE_CONNECTING)
#if ASUSWRT
					eval("modem_status.sh", "sim");
#else
					system("modem_status.sh sim");
#endif

				sim_state = nvram_get_int("usb_modem_act_sim");
				if(sim_state == 2 || sim_state == 3){
					if(sim_state == 3 || !strcmp(nvram_safe_get("modem_pincode"), "") || nvram_get_int(nvram_auxstate[wan_unit]) == WAN_STOPPED_REASON_PINCODE_ERR)
						link_wan[wan_unit] = 3;
				}
				else if(sim_state != 1)
					link_wan[wan_unit] = 0;
			}
		}

		snprintf(prefix, sizeof(prefix), "wan%d_", wan_unit);
		nvram_set_int(strcat_r(prefix, "is_usb_modem_ready", tmp), link_wan[wan_unit]);
if(test_log)
_dprintf("# wanduck: if_wan_phyconnected: x_Setting=%d, link_modem=%d, sim_state=%d.\n", !isFirstUse, link_wan[wan_unit], sim_state);

		if(link_wan[wan_unit] != nvram_get_int(wired_link_nvram)){
			nvram_set_int(wired_link_nvram, link_wan[wan_unit]);

			if(link_wan[wan_unit] == 2)
				logmessage("wanduck", "The local subnet is the same with the USB ethernet.");
			else if(link_wan[wan_unit] == 3){
				if(sim_state == 3)
					logmessage("wanduck", "The modem need the PUK code to reset PIN.");
				else
					logmessage("wanduck", "The modem need the PIN code to unlock.");
			}
			//else if(strcmp(modem_type, "ncm"))
			else
				link_changed = 1;
		}

		if(link_wan[wan_unit] == 2)
			return SET_ETH_MODEM;
		else if(link_wan[wan_unit] == 3)
			return SET_PIN;
	}
	else
#endif
	if (dualwan_unit__nonusbif(wan_unit)) {
		memset(prefix_wan, 0, 8);
		sprintf(prefix_wan, "wan%d_", wan_unit);

		memset(wan_proto, 0, 16);
		strcpy(wan_proto, nvram_safe_get(strcat_r(prefix_wan, "proto", nvram_name)));

		// check wan port.
		link_wan[wan_unit] = get_wanports_status(wan_unit);

		if(link_wan[wan_unit] != nvram_get_int(wired_link_nvram)){
			if(link_wan[wan_unit])
				nvram_set_int(wired_link_nvram, CONNED);
			else
				nvram_set_int(wired_link_nvram, DISCONN);

			link_changed = 1;
		}
	}

	if(sw_mode == SW_MODE_ROUTER){
		// this means D2C because of reconnecting the WAN port.
		if(link_changed){
#ifdef RTCONFIG_USB_MODEM
			if(dualwan_unit__usbif(wan_unit) && link_wan[wan_unit]){
				csprintf("wanduck(%d): PHY_RECONN.\n", wan_unit);
				return PHY_RECONN;
			}
			else
#endif
			// WAN port was disconnected, arm reconnect
			if(!link_setup[wan_unit] && !link_wan[wan_unit]){
				link_setup[wan_unit] = 1;
			}
			// WAN port was connected, fire reconnect if armed
			else if(link_setup[wan_unit]){
				link_setup[wan_unit] = 0;
#if ASUSWRT
				// Only handle this case when WAN proto is DHCP or Static
				if(!strcmp(wan_proto, "static")){
					disconn_case[wan_unit] = CASE_DISWAN;
					return PHY_RECONN;
				}
				else if(!strcmp(wan_proto, "dhcp")){
					disconn_case[wan_unit] = CASE_DHCPFAIL;
					return PHY_RECONN;
				}
#else
				disconn_case[wan_unit] = CASE_DISWAN;
				return PHY_RECONN;
#endif
			}
		}
		else if(!link_wan[wan_unit]){
#ifndef NO_DETECT_INTERNET
#ifdef RTCONFIG_DUALWAN
			if(strcmp(dualwan_mode, "lb"))
#endif
				nvram_set_int("link_internet", 1);
#endif

			record_wan_state_nvram(wan_unit, -1, -1, WAN_AUXSTATE_NOPHY);

			if((nvram_get_int("web_redirect")&WEBREDIRECT_FLAG_NOLINK)){
				disconn_case[wan_unit] = CASE_DISWAN;
			}

			return DISCONN;
		}
	}
	else if(sw_mode == SW_MODE_AP){
#if 0
		if(!link_wan[wan_unit]){
			// ?: type error?
			nvram_set_int("link_internet", 1);

			record_wan_state_nvram(wan_unit, -1, -1, WAN_AUXSTATE_NOPHY);

			if((nvram_get_int("web_redirect")&WEBREDIRECT_FLAG_NOLINK)){
				disconn_case[wan_unit] = CASE_DISWAN;
				return DISCONN;
			}
		}
#else
		if(nvram_get_int("link_internet") != 2)
			nvram_set_int("link_internet", 2);

		return CONNED;
#endif
	}

	return CONNED;
}

int if_wan_connected(int wan_unit, int wan_state){
	if(chk_proto(wan_unit, wan_state) != CONNED){
		return DISCONN;
	} else if(sw_mode == SW_MODE_ROUTER){ // TODO: temparily let detect_internet() service in SW_MODE_ROUTER.
		return detect_internet(wan_unit);
	}

	return CONNED;
}

void handle_wan_line(int wan_unit, int action){
	char cmd[32];

	// Redirect rules.
	if(action){
		stop_nat_rules();
		//Andy Chiu, 2016/08/03. Stop Wan
		//stop_wan_if(wan_unit);
	}
	/*
	 * When C2C and remove the redirect rules,
	 * it means dissolve the default state.
	 */
	else if(conn_changed_state[wan_unit] == D2C || conn_changed_state[wan_unit] == CONNED){
		start_nat_rules();
	}
	else{ // conn_changed_state[wan_unit] == PHY_RECONN
		_dprintf("\n# wanduck(%d): Try to restart_wan_if [%d].\n", action, wan_unit);
		memset(cmd, 0, 32);
#if ASUSWRT
		sprintf(cmd, "restart_wan_if %d", wan_unit);
		notify_rc_and_wait(cmd);
#else
		int i;
		if(wan_unit == WAN_UNIT_ATM) {
			for(i = 0; i < 8; i++) {
				snprintf(cmd, sizeof(cmd), "Wan_PVC%d", i);
				if(tcapi_match(cmd, "Active", "Yes"))
					tcapi_commit(cmd);
			}
		}
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
		else if(wan_unit == WAN_UNIT_PTM0 || wan_unit == WAN_UNIT_ETH) {
			for(i = 0; i < 8; i++) {
				snprintf(cmd, sizeof(cmd), "WanExt_PVC%de%d", wan_unit, i);
				if(tcapi_match(cmd, "Active", "Yes"))
					tcapi_commit(cmd);
			}
		}
#endif
		else {
			snprintf(cmd, sizeof(cmd), "Wan_PVC%d", wan_unit);
			tcapi_commit(cmd);
		}
#endif
	}
}

void close_socket(int sockfd, char type){
	close(sockfd);
	FD_CLR(sockfd, &allset);
	client[fd_i].sfd = -1;
	client[fd_i].type = 0;
}

int build_socket(char *http_port, char *dns_port, int *hd, int *dd){
	if((*hd = passivesock(http_port, IPPROTO_TCP, 10)) < 0){
		csprintf("Fail to socket for httpd port: %s.\n", http_port);
		return -1;
	}
	
	if((*dd = passivesock(dns_port, IPPROTO_UDP, 10)) < 0){
		csprintf("Fail to socket for DNS port: %s.\n", dns_port);
		return -1;
	}
	
	return 0;
}

void send_page(int wan_unit, int sfd, char *file_dest, char *url){
	char buf[2*MAXLINE];
	time_t now;
	char timebuf[100];
	char dut_addr[64];

	memset(buf, 0, sizeof(buf));
	now = uptime();
	(void)strftime(timebuf, sizeof(timebuf), RFC1123FMT, gmtime(&now));

#ifdef NO_IOS_DETECT_INTERNET
	// disable iOS popup window. Jerry5 2012.11.27
	if (!strcmp(url,"www.apple.com/library/test/success.html") && nvram_get_int("disiosdet") == 1){
		sprintf(buf, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", buf, "HTTP/1.0 200 OK\r\n", "Server: Apache/2.2.3 (Oracle)\r\n", "Content-Type: text/html; charset=UTF-8\r\n", "Cache-Control: max-age=557\r\n","Expires: ", timebuf, "\r\n", "Date: ", timebuf, "\r\n", "Content-Length: 127\r\n", "Connection: close\r\n\r\n","<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n","<HTML>\n","<HEAD>\n\t","<TITLE>Success</TITLE>\n","</HEAD>\n","<BODY>\n","Success\n","</BODY>\n","</HTML>\n");
	}
	else{
#endif

	sprintf(buf, "%s%s%s%s%s%s", buf, "HTTP/1.0 302 Moved Temporarily\r\n", "Server: wanduck\r\n", "Date: ", timebuf, "\r\n");

	memset(dut_addr, 0, 64);

	if(isFirstUse)
		strcpy(dut_addr, DUT_DOMAIN_NAME);
	else
#if ASUSWRT
		strcpy(dut_addr, nvram_safe_get("lan_ipaddr"));
#else
		tcapi_get("Lan_Entry0", "IP", dut_addr);
#endif

	// TODO: Only send pages for the wan(0)'s state.
#ifdef RTCONFIG_USB_MODEM
	if (dualwan_unit__usbif(wan_unit)) {
		if((conn_changed_state[wan_unit] == SET_ETH_MODEM) ||
			((conn_changed_state[wan_unit] == C2D || conn_changed_state[wan_unit] == DISCONN) && disconn_case[wan_unit] == CASE_THESAMESUBNET))
			sprintf(buf, "%s%s%s%s%s%d%s%s" ,buf , "Connection: close\r\n", "Location:http://", dut_addr, "/cgi-bin/error_page.asp?flag=", CASE_THESAMESUBNET, "\r\nContent-Type: text/plain\r\n", "\r\n<html></html>\r\n");
		else{
			close_socket(sfd, T_HTTP);
			return;
		}
	}
	else
#endif
	if((conn_changed_state[wan_unit] == C2D || conn_changed_state[wan_unit] == DISCONN) && disconn_case[wan_unit] == CASE_THESAMESUBNET)
		sprintf(buf, "%s%s%s%s%s%d%s%s" ,buf , "Connection: close\r\n", "Location:http://", dut_addr, "/cgi-bin/error_page.asp?flag=", disconn_case[wan_unit], "\r\nContent-Type: text/plain\r\n", "\r\n<html></html>\r\n");
	else if(isFirstUse){
			sprintf(buf, "%s%s%s%s%s%s%s" ,buf , "Connection: close\r\n", "Location:http://", dut_addr, "/cgi-bin/index.asp", "\r\nContent-Type: text/plain\r\n", "\r\n<html></html>\r\n");
	}
	else if(conn_changed_state[wan_unit] == C2D || conn_changed_state[wan_unit] == DISCONN)
		sprintf(buf, "%s%s%s%s%s%d%s%s" ,buf , "Connection: close\r\n", "Location:http://", dut_addr, "/cgi-bin/error_page.asp?flag=", disconn_case[wan_unit], "\r\nContent-Type: text/plain\r\n", "\r\n<html></html>\r\n");

#ifdef NO_IOS_DETECT_INTERNET
	}
#endif
	write(sfd, buf, strlen(buf));
	close_socket(sfd, T_HTTP);
}

void parse_dst_url(char *page_src){
	int i, j;
	char dest[PATHLEN], host[64];
	char host_strtitle[7], *hp;
	
	j = 0;
	memset(dest, 0, sizeof(dest));
	memset(host, 0, sizeof(host));
	memset(host_strtitle, 0, sizeof(host_strtitle));
	
	for(i = 0; i < strlen(page_src); ++i){
		if(i >= PATHLEN)
			break;
		
		if(page_src[i] == ' ' || page_src[i] == '?'){
			dest[j] = '\0';
			break;
		}
		
		dest[j++] = page_src[i];
	}
	
	host_strtitle[0] = '\n';
	host_strtitle[1] = 'H';
	host_strtitle[2] = 'o';
	host_strtitle[3] = 's';
	host_strtitle[4] = 't';
	host_strtitle[5] = ':';
	host_strtitle[6] = ' ';
	
	if((hp = strstr(page_src, host_strtitle)) != NULL){
		hp += 7;
		j = 0;
		for(i = 0; i < strlen(hp); ++i){
			if(i >= 64)
				break;
			
			if(hp[i] == '\r' || hp[i] == '\n'){
				host[j] = '\0';
				break;
			}
			
			host[j++] = hp[i];
		}
	}
	
	memset(dst_url, 0, sizeof(dst_url));
	sprintf(dst_url, "%s/%s", host, dest);
}

void handle_http_req(int sfd, char *line){
	int len;
	
	if(!strncmp(line, "GET /", 5)){
		parse_dst_url(line+5);
		len = strlen(dst_url);
		if((dst_url[len-4] == '.') &&
				(dst_url[len-3] == 'i') &&
				(dst_url[len-2] == 'c') &&
				(dst_url[len-1] == 'o')){
			close_socket(sfd, T_HTTP);
			
			return;
		}

		send_page(current_wan_unit, sfd, NULL, dst_url);
	}
	else
		close_socket(sfd, T_HTTP);
}

void handle_dns_req(int sfd, unsigned char *request, int maxlen, struct sockaddr *pcliaddr, int clen){
	char buf[MAXLEN_TCAPI_MSG] = {0};

	unsigned char reply_content[MAXLINE], *ptr, *end;
	dns_header *d_req, *d_reply;
	dns_queries queries;
	dns_answer answer;
	uint16_t opcode;

	/* validation */
	d_req = (dns_header *)request;
	if (maxlen <= sizeof(dns_header) ||			/* incomplete header */
	    d_req->flag_set.flag_num & htons(0x8000) ||		/* not query */
	    d_req->questions == 0)				/* no questions */
		return;
	opcode = d_req->flag_set.flag_num & htons(0x7800);
	ptr = request + sizeof(dns_header);
	end = request + maxlen;

	/* query, only first so far */
	memset(&queries, 0, sizeof(queries));
	while (ptr < end) {
		size_t len = *ptr++;
		if (len > 63 || end - ptr < (len ? : 4))
			return;
		if (len == 0) {
			memcpy(&queries.type, ptr, 2);
			memcpy(&queries.ip_class, ptr + 2, 2);
			ptr += 4;
			break;
		}
		if (*queries.name)
			strcat(queries.name, ".");
		strncat(queries.name, (char *)ptr, len);
		ptr += len;
	}
	if (queries.type == 0 || queries.ip_class == 0 || strlen(queries.name) > 1025)
		return;
	maxlen = ptr - request;

	/* reply */
	if (maxlen > sizeof(reply_content))
		return;
	ptr = memcpy(reply_content, request, maxlen) + maxlen;
	end = reply_content + sizeof(reply_content);

	/* header */
	d_reply = (dns_header *)reply_content;
	d_reply->flag_set.flag_num = htons(0x8180);
	d_reply->questions = htons(1);
	d_reply->answer_rrs = htons(0);
	d_reply->auth_rrs = htons(0);
	d_reply->additional_rss = htons(0);

	/* answer */
	memset(&answer, 0, sizeof(answer));
	answer.name = htons(0xc00c);
	answer.type = queries.type;
	answer.ip_class = queries.ip_class;
	answer.ttl = htonl(0);
	answer.data_len = htons(4);

	if (opcode != 0) {
		/* not implemented, non-Query op */
		d_reply->flag_set.flag_num = htons(0x8184) | opcode;
	} else if (queries.ip_class == htons(1) && queries.type == htons(1)) {
		/* class IN type A */
		if (strcasecmp(queries.name, router_name) == 0
		) {
			/* no error, authoritative */
			d_reply->flag_set.flag_num = htons(0x8580);
			d_reply->answer_rrs = htons(1);
			answer.addr = inet_addr_(tcapi_get_string("Lan_Entry0", "IP", buf));
		}
		else if (strcasecmp(queries.name, "ntp01.mvp.tivibu.com.tr") == 0) {
			d_reply->answer_rrs = htons(1);
			answer.addr = htonl(0xc0a87946);
		}
		else if (strcasecmp(queries.name, "ntp02.mvp.tivibu.com.tr") == 0) {
			d_reply->answer_rrs = htons(1);
			answer.addr = htonl(0xc0a87947);

		} else if (*queries.name) {
			/* no error */
			d_reply->answer_rrs = htons(1);
			answer.addr = htonl(0x0a000001);	// 10.0.0.1
		}
	} else {
		/* not implemented */
		d_reply->flag_set.flag_num = htons(0x8184);
	}

	if (d_reply->answer_rrs) {
		if (end - ptr < sizeof(answer))
			return;
		ptr = memcpy(ptr, &answer, sizeof(answer)) + sizeof(answer);
	}

	sendto(sfd, reply_content, ptr - reply_content, 0, pcliaddr, clen);
}

void run_http_serv(int sockfd){
	ssize_t n;
	char line[MAXLINE];
	
	memset(line, 0, sizeof(line));
	
	if((n = read(sockfd, line, MAXLINE)) == 0){	// client close
		close_socket(sockfd, T_HTTP);
		
		return;
	}
	else if(n < 0){
		perror("wanduck serv http");
		return;
	}
	else{
		if(client[fd_i].type == T_HTTP)
			handle_http_req(sockfd, line);
		else
			close_socket(sockfd, T_HTTP);
	}
}

void run_dns_serv(int sockfd){
	int n;
	char line[MAXLINE];
	struct sockaddr_in cliaddr;
	int clilen = sizeof(cliaddr);
	
	memset(line, 0, MAXLINE);
	memset(&cliaddr, 0, clilen);
	
	if((n = recvfrom(sockfd, line, MAXLINE, 0, (struct sockaddr *)&cliaddr, (socklen_t *)&clilen)) == 0)	// client close
		return;
	else if(n < 0){
		perror("wanduck serv dns");
		return;
	}
	else
		handle_dns_req(sockfd, line, n, (struct sockaddr *)&cliaddr, clilen);
}

void record_wan_state_nvram(int wan_unit, int state, int sbstate, int auxstate){
	if(state != -1 && state != nvram_get_int(nvram_state[wan_unit]))
		nvram_set_int(nvram_state[wan_unit], state);

	if(sbstate != -1 && sbstate != nvram_get_int(nvram_sbstate[wan_unit]))
		nvram_set_int(nvram_sbstate[wan_unit], sbstate);

	if(auxstate != -1 && auxstate != nvram_get_int(nvram_auxstate[wan_unit]))
		nvram_set_int(nvram_auxstate[wan_unit], auxstate);
}

void record_conn_status(int wan_unit){
	char prefix_wan[8], nvram_name[16], wan_proto[16];
	char log_title[32];

	memset(log_title, 0, 32);
#ifdef RTCONFIG_DUALWAN
	if(!strcmp(dualwan_mode, "lb") || !strcmp(dualwan_mode, "fb"))
		sprintf(log_title, "WAN(%d) Connection", wan_unit);
	else
#endif
		strcpy(log_title, "WAN Connection");

	memset(prefix_wan, 0, 8);
	sprintf(prefix_wan, "wan%d_", wan_unit);

	memset(wan_proto, 0, 16);
	strcpy(wan_proto, nvram_safe_get(strcat_r(prefix_wan, "proto", nvram_name)));

	if(conn_changed_state[wan_unit] == DISCONN || conn_changed_state[wan_unit] == C2D){
		if(disconn_case[wan_unit] == CASE_DISWAN){
			if(disconn_case_old[wan_unit] == CASE_DISWAN)
				return;
			disconn_case_old[wan_unit] = CASE_DISWAN;

			logmessage(log_title, "Wan link down.");	//ethernet, dsl
		}
		else if(disconn_case[wan_unit] == CASE_PPPFAIL){
			if(disconn_case_old[wan_unit] == CASE_PPPFAIL)
				return;
			disconn_case_old[wan_unit] = CASE_PPPFAIL;

			if(ppp_fail_state == WAN_STOPPED_REASON_PPP_AUTH_FAIL)
				logmessage(log_title, "VPN authentication failed.");
			else if(ppp_fail_state == WAN_STOPPED_REASON_PPP_NO_ACTIVITY)
				logmessage(log_title, "No response from ISP.");
			else
				logmessage(log_title, "Fail to connect with some issues.");
		}
		else if(disconn_case[wan_unit] == CASE_DHCPFAIL){
			if(disconn_case_old[wan_unit] == CASE_DHCPFAIL)
				return;
			disconn_case_old[wan_unit] = CASE_DHCPFAIL;

			if(!strcmp(wan_proto, "dhcp")
					)
				logmessage(log_title, "ISP's DHCP did not function properly.");
			else
				logmessage(log_title, "Detected that the WAN Connection Type was PPPoE. But the PPPoE Setting was not complete.");
		}
		else if(disconn_case[wan_unit] == CASE_MISROUTE){
			if(disconn_case_old[wan_unit] == CASE_MISROUTE)
				return;
			disconn_case_old[wan_unit] = CASE_MISROUTE;

			logmessage(log_title, "The router's ip was the same as gateway's ip. It led to your packages couldn't dispatch to internet correctly.");
		}
		else if(disconn_case[wan_unit] == CASE_THESAMESUBNET){
			if(disconn_case_old[wan_unit] == CASE_THESAMESUBNET)
				return;
			disconn_case_old[wan_unit] = CASE_THESAMESUBNET;

			logmessage(log_title, "The LAN's subnet may be the same with the WAN's subnet.");
		}
		else{	// disconn_case[wan_unit] == CASE_OTHERS
			if(disconn_case_old[wan_unit] == CASE_OTHERS)
				return;
			disconn_case_old[wan_unit] = CASE_OTHERS;

			logmessage(log_title, "WAN was exceptionally disconnected.");
		}
	}
	else if(conn_changed_state[wan_unit] == D2C){
		if(disconn_case_old[wan_unit] == 10)
			return;
		disconn_case_old[wan_unit] = 10;

		logmessage(log_title, "WAN was restored.");
	}
	else if(conn_changed_state[wan_unit] == PHY_RECONN){
		logmessage(log_title, "Wan link up.");	//ethernet, dsl
	}
}

int get_disconn_count(int wan_unit){
	return changed_count[wan_unit];
}

void set_disconn_count(int wan_unit, int flag){
	changed_count[wan_unit] = flag;
}

int switch_wan_line(const int wan_unit, const int restart_other){
#ifdef RTCONFIG_USB_MODEM
	int retry, lock;
#endif
#if ASUSWRT
	char cmd[32];
#endif
	int unit;
	char prefix[32] = "wanXXXXXX_", tmp[100] = "";

	for(unit = WAN_UNIT_FIRST; unit < WAN_UNIT_MAX; ++unit)
		if(unit == wan_unit)
			break;
	if(unit == WAN_UNIT_MAX)
		return 0;

	if(wan_primary_ifunit() == wan_unit) // Already have no running modem.
		return 0;
#ifdef RTCONFIG_USB_MODEM
	else if (dualwan_unit__usbif(wan_unit)) {
		if(!link_wan[wan_unit]) {
			snprintf(prefix, sizeof(prefix), "wan%d_", wan_unit);
			nvram_set_int(strcat_r(prefix, "is_usb_modem_ready", tmp), link_wan[wan_unit]);
			return 0; // No modem in USB ports.
		}
	}
#endif

	csprintf("%s: wan(%d) Starting...\n", __FUNCTION__, wan_unit);
	// Set the modem to be running.
	set_wan_primary_ifunit(wan_unit);

#ifdef RTCONFIG_USB_MODEM
	if (nvram_invmatch("modem_enable", "4") && dualwan_unit__usbif(wan_unit)) {
		// Wait the PPP config file to be done.
		retry = 0;
		while((lock = file_lock("3g")) == -1 && retry < MAX_WAIT_FILE)
			sleep(1);

		if(lock == -1){
			csprintf("(%d): No pppd conf file and turn off the state of USB Modem.\n", wan_unit);
			set_wan_primary_ifunit(!wan_unit);
			return 0;
		}
		else
			file_unlock(lock);
	}
#endif

	// TODO: don't know if it's necessary?
	// clean or restart the other line.
#if ASUSWRT
	if(restart_other)
	{
		for(unit = WAN_UNIT_FIRST; unit < WAN_UNIT_MAX; ++unit){
			if(unit == wan_unit)
				continue;

			csprintf("wanduck1: restart_wan_if %d.\n", unit);
			snprintf(cmd, 32, "restart_wan_if %d", unit);
			notify_rc_and_wait(cmd);
			sleep(1);
		}
	}
#ifdef RTCONFIG_USB_MODEM
	else{
		for(unit = WAN_UNIT_FIRST; unit < WAN_UNIT_MAX; ++unit){
			if(unit == wan_unit)
				continue;

			if(dualwan_unit__nonusbif(unit))
				continue;

			csprintf("wanduck1: stop_wan_if %d.\n", unit);
			snprintf(cmd, 32, "stop_wan_if %d", unit);
			notify_rc_and_wait(cmd);
			sleep(1);
		}
	}
#endif
#else //TCLINUX

	if(restart_other)
	{
		unit = wan_secondary_ifunit();
		csprintf("wanduck1: restart_wan_if %d.\n", unit);
		memset(tmp, 0, sizeof(tmp));
#ifdef TCSUPPORT_MULTISERVICE_ON_WAN
		if(unit == WAN_UNIT_PTM0 || unit == WAN_UNIT_ETH)
		sprintf(tmp, "WanExt_PVC%de0", unit);
		else
#endif
		sprintf(tmp, "Wan_PVC%d", unit);
		//tcapi_set(tmp, "Active", "Yes");
		tcapi_commit(tmp);
	}
#endif

	// restart the primary line.
#if ASUSWRT
	memset(cmd, 0, 32);
	if(get_wan_state(wan_unit) == WAN_STATE_CONNECTED)
		snprintf(cmd, 32, "restart_wan_line %d", wan_unit);
	else
		snprintf(cmd, 32, "restart_wan_if %d", wan_unit);
	csprintf("wanduck2: %s.\n", cmd);
	notify_rc_and_wait(cmd);

#else //TCLINUX
	memset(tmp, 0, sizeof(tmp));
#ifdef TCSUPPORT_MULTISERVICE_ON_WAN
	if(wan_unit == WAN_UNIT_PTM0 || wan_unit == WAN_UNIT_ETH)
	sprintf(tmp, "WanExt_PVC%de0", wan_unit);
	else
#endif
	sprintf(tmp, "Wan_PVC%d", wan_unit);
	//tcapi_set(tmp, "Active", "Yes");
	csprintf("Start %s\n", tmp);
	tcapi_commit(tmp);
#endif

	if (SW_GUEST_NETWORK_F) {
		tcapi_commit("WLan");
		SW_GUEST_NETWORK_F = 0;
	}

#ifdef RTCONFIG_DUALWAN
	if(sw_mode == SW_MODE_ROUTER
			&& (!strcmp(dualwan_mode, "fo") || !strcmp(dualwan_mode, "fb"))
			){
		delay_detect = 1;
	}
#endif

	csprintf("%s: wan(%d) End.\n", __FUNCTION__, wan_unit);
	return 1;
}


/* 
   value = 0, force disable guest network 
   value = 1, turn back guest network
*/
static void ForceControlWLan(char *value)
{
	int i, j;
	int bandnum;
	int restart = 0;
	char wlEntry[20];
	char record_en[4];

#if defined(TCSUPPORT_DUAL_WLAN)
	bandnum = 1;
#else
	bandnum = 0;
#endif
	for (i=0; i <= bandnum; i++) {
		for (j=1; j <= 3; j++) {
			snprintf(wlEntry, sizeof(wlEntry), "wl%d.%d_bss_enabled",i ,j);
			memset(record_en, 0, sizeof(record_en));
			if (!strcmp(value, "0")) { /* OFF */
				tcapi_get("WLan_Entry0", wlEntry, record_en);
				if(!strcmp(record_en, "1")) { 
					tcapi_set("WLan_Entry0", wlEntry, value);
					restart = 1;
				}
				csprintf("# wanduck: Store %s:[%s]\n", wlEntry, record_en);
				tcapi_set("Vram_Entry" , wlEntry, record_en);

			} else { /* ON */
				tcapi_get("Vram_Entry" , wlEntry, record_en);
				tcapi_set("WLan_Entry0", wlEntry, record_en);
				if(!strcmp(record_en, "1")) 
					restart = 1;
			}
		}
	}
	
	if(restart) {
#if defined(TCSUPPORT_DUAL_WLAN)
		/* For WLan Guest Network 2.4G and 5G restart flag */
		tcapi_set("WLan_Common", "force_switch", "1");
#endif
		/* For Genreate correct Wireless Profile */
		tcapi_set("WLan_Common", "editFlag", "0");
		tcapi_set("WLan_Common", "MBSSID_changeFlag", "0");
		
		if(!strcmp(value, "0")) {
			csprintf("# wanduck: Force Disable Guest Network wireless Lan\n");
			tcapi_set("WLan_Common", "MBSSID_able_Flag", "1");
		} else {
			csprintf("# wanduck: Turn on back Guest Network wireless Lan\n");
			tcapi_set("WLan_Common", "MBSSID_able_Flag", "0");
		}
		
		/* Enable FLAG */
		SW_GUEST_NETWORK_F = 1;
	}
}

int wanduck_main(int argc, char *argv[]){
	char *http_servport, *dns_servport;
	int clilen;
	struct sockaddr_in cliaddr;
	struct timeval  tval;
	int nready, maxi, sockfd;
	int wan_unit;
	char prefix_wan[8];
	char cmd[32];
	char tmp[100]="";
#if !ASUSWRT
	int i;
	int wan_primary = wan_primary_ifunit();
	int wan_secondary = wan_secondary_ifunit();
#endif

	unsigned int now;

	signal(SIGHUP, SIG_IGN);
	signal(SIGTERM, safe_leave);
	signal(SIGCHLD, chld_reap);
#if ASUSWRT
	signal(SIGUSR1, get_network_nvram);
#endif

	if(argc < 3){
		http_servport = DFL_HTTP_SERV_PORT;
		dns_servport = DFL_DNS_SERV_PORT;
	}
	else{
		if(atoi(argv[1]) <= 0)
			http_servport = DFL_HTTP_SERV_PORT;
		else
			http_servport = (char *)argv[1];

		if(atoi(argv[2]) <= 0)
			dns_servport = DFL_DNS_SERV_PORT;
		else
			dns_servport = (char *)argv[2];
	}

	if(build_socket(http_servport, dns_servport, &http_sock, &dns_sock) < 0){
		csprintf("\n*** Fail to build socket! ***\n");
		exit(0);
	}
	if(fcntl(dns_sock, F_SETFL, fcntl(dns_sock, F_GETFL, 0)|O_NONBLOCK) < 0){
		_dprintf("wanduck set dnssock [%d] nonblock fail !\n", dns_sock);
		exit(0);
	}

	test_log = nvram_get_int("wanduck_debug");

	FILE *fp = fopen(WANDUCK_PID_FILE, "w");

	if(fp != NULL){
		fprintf(fp, "%d", getpid());
		fclose(fp);
	}

	maxfd = (http_sock > dns_sock)?http_sock:dns_sock;
	maxi = -1;

	FD_ZERO(&allset);
	FD_SET(http_sock, &allset);
	FD_SET(dns_sock, &allset);

	for(fd_i = 0; fd_i < MAX_USER; ++fd_i){
		client[fd_i].sfd = -1;
		client[fd_i].type = 0;
	}

	rule_setup = 0;
	got_notify = 0;
	clilen = sizeof(cliaddr);
#ifdef RTCONFIG_USB_MODEM
	modem_act_reset = 0;
#endif

	sprintf(router_name, "%s", DUT_DOMAIN_NAME);

#if ASUSWRT
	nvram_set_int("link_wan", 0);
	nvram_set_int("link_wan1", 0);
#endif
#ifndef NO_DETECT_INTERNET
	nvram_set_int("link_internet", 0);
#endif

	for(wan_unit = WAN_UNIT_FIRST; wan_unit < WAN_UNIT_MAX; ++wan_unit){
		link_setup[wan_unit] = 0;
		link_wan[wan_unit] = 0;

		changed_count[wan_unit] = S_IDLE;
		disconn_case[wan_unit] = CASE_NONE;

		memset(prefix_wan, 0, 8);
		sprintf(prefix_wan, "wan%d_", wan_unit);

		strcat_r(prefix_wan, "state_t", nvram_state[wan_unit]);
		strcat_r(prefix_wan, "sbstate_t", nvram_sbstate[wan_unit]);
		strcat_r(prefix_wan, "auxstate_t", nvram_auxstate[wan_unit]);

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "link_wan%d", wan_unit);
		nvram_set(tmp, "0");

		set_disconn_count(wan_unit, S_IDLE);
	}
#ifdef RTCONFIG_USB_MODEM
	nvram_set("wan11_is_usb_modem_ready", "0");
#endif

	loop_sec = uptime();

	get_related_nvram(); // initial the System's variables.
#if ASUSWRT
	get_lan_nvram(); // initial the LAN's variables.
#endif

	nat_redirect_enable_old = nat_redirect_enable;

#ifdef RTCONFIG_DUALWAN
#if ASUSWRT
	WAN_FB_UNIT = WAN_UNIT_FIRST;
#else
	WAN_FB_UNIT = wan_primary;
	wan_pool[0] = wan_primary;
	wan_pool[1] = wan_secondary;
#endif

	if(sw_mode == SW_MODE_ROUTER && !strcmp(dualwan_mode, "lb")){
		cross_state = DISCONN;
#if ASUSWRT
		for(wan_unit = WAN_UNIT_FIRST; wan_unit < WAN_UNIT_MAX; ++wan_unit)
#else
		for(i = 0; wan_unit = wan_pool[i], wan_unit != -1; i++)
#endif
		{
			conn_state[wan_unit] = if_wan_phyconnected(wan_unit);
			if(conn_state[wan_unit] == CONNED){
#ifdef RTCONFIG_USB_MODEM
				current_state[wan_unit] = nvram_get_int(nvram_state[wan_unit]);
				if(!(dualwan_unit__usbif(wan_unit) && current_state[wan_unit] == WAN_STATE_INITIALIZING))
#endif
					conn_state[wan_unit] = if_wan_connected(wan_unit, nvram_get_int(nvram_state[wan_unit]));
			}
			else
				conn_state[wan_unit] = DISCONN;

			conn_changed_state[wan_unit] = conn_state[wan_unit];

			if(conn_state[wan_unit] == CONNED && cross_state != CONNED)
				cross_state = CONNED;

			conn_state_old[wan_unit] = conn_state[wan_unit];

			record_conn_status(wan_unit);

			if(cross_state == CONNED)
				set_disconn_count(wan_unit, S_IDLE);
			else
				set_disconn_count(wan_unit, S_COUNT);
		}
	}
	else if(sw_mode == SW_MODE_ROUTER
			&& (!strcmp(dualwan_mode, "fo") || !strcmp(dualwan_mode, "fb"))
			){
		if(wandog_delay > 0){
			csprintf("wanduck: delay %d seconds...\n", wandog_delay);
			sleep(wandog_delay);
			delay_detect = 0;
		}

		// To check the phy connection of the standby line.
		for(wan_unit = WAN_UNIT_FIRST; wan_unit < WAN_UNIT_MAX; ++wan_unit){
#if ASUSWRT
			if(get_dualwan_by_unit(wan_unit) != WANS_DUALWAN_IF_NONE)
#else
			if(get_dualwan_by_pvcunit(wan_unit) != WANS_DUALWAN_IF_NONE)
#endif
				conn_state[wan_unit] = if_wan_phyconnected(wan_unit);
		}

		current_wan_unit = wan_primary_ifunit();
#if ASUSWRT
		other_wan_unit = !current_wan_unit;
#else
		other_wan_unit = wan_secondary_ifunit();
#endif
if(test_log)
_dprintf("wanduck(%d) 1: conn_state %d, conn_state_old %d, conn_changed_state %d, current_state %d.\n"
		, current_wan_unit, conn_state[current_wan_unit], conn_state_old[current_wan_unit], conn_changed_state[current_wan_unit], current_state[current_wan_unit]);

		if(conn_state[current_wan_unit] == CONNED){
#ifdef RTCONFIG_USB_MODEM
			current_state[current_wan_unit] = nvram_get_int(nvram_state[current_wan_unit]);
			if(!(dualwan_unit__usbif(current_wan_unit) && current_state[current_wan_unit] == WAN_STATE_INITIALIZING))
#endif
				conn_state[current_wan_unit] = if_wan_connected(current_wan_unit, nvram_get_int(nvram_state[current_wan_unit]));

			cross_state = conn_state[current_wan_unit];
		}
		else
			cross_state = DISCONN;

		conn_changed_state[current_wan_unit] = conn_state[current_wan_unit];

		conn_state_old[current_wan_unit] = conn_state[current_wan_unit];

		record_conn_status(current_wan_unit);
	}
	else
#endif // RTCONFIG_DUALWAN
	if(sw_mode == SW_MODE_ROUTER){
		current_wan_unit = wan_primary_ifunit();
#if ASUSWRT
		other_wan_unit = !current_wan_unit;
#else
		other_wan_unit = wan_secondary_ifunit();
#endif

		conn_state[current_wan_unit] = if_wan_phyconnected(current_wan_unit);
		if(conn_state[current_wan_unit] == CONNED){
#ifdef RTCONFIG_USB_MODEM
			current_state[current_wan_unit] = nvram_get_int(nvram_state[current_wan_unit]);
			if(!(dualwan_unit__usbif(current_wan_unit) && current_state[current_wan_unit] == WAN_STATE_INITIALIZING))
#endif
				conn_state[current_wan_unit] = if_wan_connected(current_wan_unit, nvram_get_int(nvram_state[current_wan_unit]));

			cross_state = conn_state[current_wan_unit];
		}
		else
			cross_state = DISCONN;

		conn_changed_state[current_wan_unit] = conn_state[current_wan_unit];

		conn_state_old[current_wan_unit] = conn_state[current_wan_unit];

		record_conn_status(current_wan_unit);
	}
	else{ // sw_mode == SW_MODE_AP, SW_MODE_REPEATER.
		current_wan_unit = WAN_UNIT_FIRST;

		conn_state[current_wan_unit] = if_wan_phyconnected(current_wan_unit);

		cross_state = conn_state[current_wan_unit];

		conn_state_old[current_wan_unit] = conn_state[current_wan_unit];
	}

	/*
	 * Because start_wanduck() is run early than start_wan()
	 * and the redirect rules is already set before running wanduck,
	 * handle_wan_line() must not be run at the first detect.
	 */
	if(cross_state == DISCONN){
		csprintf("\n# Enable direct rule\n");
		rule_setup = 1;
	}
	else if(cross_state == CONNED && isFirstUse){
		csprintf("\n#CONNED : Enable direct rule\n");
		rule_setup = 1;
	}
#if !ASUSWRT
	handle_wan_line(current_wan_unit ,rule_setup);
#endif
if(test_log)
_dprintf("wanduck(%d) 2: conn_state %d, conn_state_old %d, conn_changed_state %d, current_state %d.\n"
		, current_wan_unit, conn_state[current_wan_unit], conn_state_old[current_wan_unit], conn_changed_state[current_wan_unit], current_state[current_wan_unit]);

	int first_loop = 1;
	unsigned int diff;
	for(;;){
		if(!first_loop){
			now = uptime();
			diff = now-loop_sec;

			if(diff < scan_interval){
				rset = allset;
				tval.tv_sec = scan_interval-diff;
				tval.tv_usec = 0;

				goto WANDUCK_SELECT;
			}

			loop_sec = now;
		}
		else
			first_loop = 0;

		rset = allset;
		tval.tv_sec = scan_interval;
		tval.tv_usec = 0;

		get_related_nvram();

		// Sam 2014/10/14
		// rule_setup depend on wan status,
		// if nat_redirect_enable changed, rebuild nat rule.
		if(rule_setup) {
			if(nat_redirect_enable_old == 0 && nat_redirect_enable == 1)	//need redirect
				stop_nat_rules();
			else if(nat_redirect_enable_old == 1 && nat_redirect_enable == 0)	//don't redirect
				start_nat_rules();
		}
		if(nat_redirect_enable_old != nat_redirect_enable)
			nat_redirect_enable_old = nat_redirect_enable;
		//_dprintf("rule_setup: %d, nat_state: %s\n", rule_setup, nvram_safe_get("nat_state"));

#ifdef RTCONFIG_DUALWAN
		if(sw_mode == SW_MODE_ROUTER && !strcmp(dualwan_mode, "lb")){
			cross_state = DISCONN;
#if ASUSWRT
			for(wan_unit = WAN_UNIT_FIRST; wan_unit < WAN_UNIT_MAX; ++wan_unit)
#else
			for(i = 0; wan_unit = wan_pool[i], wan_unit != -1; i++)
#endif
			{
#if 0
#ifdef RTCONFIG_USB_MODEM
				if(dualwan_unit__usbif(wan_unit) && !link_wan[wan_unit]){
					if_wan_phyconnected(wan_unit);
					continue;
				}
#endif
#endif

				current_state[wan_unit] = nvram_get_int(nvram_state[wan_unit]);

				if(current_state[wan_unit] == WAN_STATE_DISABLED){
					//record_wan_state_nvram(wan_unit, WAN_STATE_STOPPED, WAN_STOPPED_REASON_MANUAL, -1);

					disconn_case[wan_unit] = CASE_OTHERS;
					conn_state[wan_unit] = DISCONN;
				}
#ifdef RTCONFIG_USB_MODEM
				else if(dualwan_unit__usbif(wan_unit)
						&& (modem_act_reset == 1 || modem_act_reset == 2)
						){
_dprintf("wanduck(%d): detect the modem to be reset...\n", wan_unit);
					disconn_case[wan_unit] = CASE_OTHERS;
					conn_state[wan_unit] = DISCONN;
					set_disconn_count(wan_unit, S_IDLE);
				}
#endif
				else{
					conn_state[wan_unit] = if_wan_phyconnected(wan_unit);
					if(conn_state[wan_unit] == CONNED){
#ifdef RTCONFIG_USB_MODEM
						if(!(dualwan_unit__usbif(wan_unit) && current_state[wan_unit] == WAN_STATE_INITIALIZING))
#endif
							conn_state[wan_unit] = if_wan_connected(wan_unit, current_state[wan_unit]);
					}
				}

				if(conn_state[wan_unit] == CONNED && cross_state != CONNED)
					cross_state = CONNED;

#ifdef RTCONFIG_USB_MODEM
				if(dualwan_unit__usbif(wan_unit)){
					if(link_wan[wan_unit] == 1 && current_state[wan_unit] == WAN_STATE_INITIALIZING && boot_end == 1){
						csprintf("wanduck: start_wan_if %d.\n", wan_unit);
						snprintf(cmd, 32, "start_wan_if %d", wan_unit);
#if ASUSWRT
						notify_rc(cmd);
#else
						system(cmd);
#endif
						continue;
					}
					else if(!link_wan[wan_unit] && current_state[wan_unit] != WAN_STATE_INITIALIZING){
						csprintf("wanduck: stop_wan_if %d.\n", wan_unit);
						snprintf(cmd, 32, "stop_wan_if %d", wan_unit);
#if ASUSWRT
						notify_rc(cmd);
#else
						system(cmd);
#endif
						continue;
					}
				}

				if(conn_state[wan_unit] == SET_ETH_MODEM){
					conn_changed_state[wan_unit] = SET_ETH_MODEM;
					set_disconn_count(wan_unit, S_IDLE);
				}
				else if(conn_state[wan_unit] == SET_PIN){
					conn_changed_state[wan_unit] = SET_PIN;
					set_disconn_count(wan_unit, S_IDLE);
				}
				else
#endif
				if(conn_state[wan_unit] != conn_state_old[wan_unit]){
					if(conn_state[wan_unit] == PHY_RECONN){
						conn_changed_state[wan_unit] = PHY_RECONN;
					}
					else if(conn_state[wan_unit] == DISCONN){
						conn_changed_state[wan_unit] = C2D;

#ifdef RTCONFIG_USB_MODEM
						if (dualwan_unit__usbif(wan_unit))
							set_disconn_count(wan_unit, max_disconn_count);
						else
#endif
							set_disconn_count(wan_unit, S_COUNT);
					}
					else if(conn_state[wan_unit] == CONNED){
						if(rule_setup == 1 && !isFirstUse){
							csprintf("\n# DualWAN: Disable direct rule(D2C)\n");
#if ASUSWRT
							rule_setup = 0;
#endif
						}

						conn_changed_state[wan_unit] = D2C;
						set_disconn_count(wan_unit, S_IDLE);
					}
					else
						conn_changed_state[wan_unit] = CONNED;

					conn_state_old[wan_unit] = conn_state[wan_unit];

					record_conn_status(wan_unit);
				}

				if(get_disconn_count(wan_unit) != S_IDLE){
					if(conn_state[wan_unit] == PHY_RECONN)
						set_disconn_count(wan_unit, max_disconn_count);

					if(get_disconn_count(wan_unit) >= max_disconn_count){
						set_disconn_count(wan_unit, S_IDLE);

#ifdef RTCONFIG_USB_MODEM
						if(dualwan_unit__usbif(wan_unit) &&
								(!strcmp(modem_type, "ecm") || !strcmp(modem_type, "ncm") || !strcmp(modem_type, "rndis") || !strcmp(modem_type, "asix") || !strcmp(modem_type, "qmi") || !strcmp(modem_type, "gobi"))
								)
							;
						else
#endif
						{
#if ASUSWRT
							memset(cmd, 0, 32);
							sprintf(cmd, "restart_wan_if %d", wan_unit);
							notify_rc_and_period_wait(cmd, 30);
#else
							memset(tmp, 0, sizeof(tmp));
							sprintf(tmp, "Wan_PVC%d", wan_unit);
							csprintf("%d %s\n", __LINE__, tmp);
							wan_down(get_wan_ifname(wan_unit));
							tcapi_commit(tmp);
#endif
						}

#if ASUSWRT
						if(get_wan_state(!wan_unit) == WAN_STATE_CONNECTED){
							memset(cmd, 0, 32);
							sprintf(cmd, "restart_wan_line %d", !wan_unit);
							notify_rc(cmd);
						}
#else
						if(wan_unit == wan_primary && get_wan_state(wan_secondary) == WAN_STATE_CONNECTED){
							memset(tmp, 0, sizeof(tmp));
							sprintf(tmp, "Wan_PVC%d", wan_secondary);
							csprintf("%d %s\n", __LINE__, tmp);
							tcapi_commit(tmp);
						}
						else if (wan_unit == wan_secondary && get_wan_state(wan_primary) == WAN_STATE_CONNECTED){
							memset(tmp, 0, sizeof(tmp));
							sprintf(tmp, "Wan_PVC%d", wan_primary);
							csprintf("%d %s\n", __LINE__, tmp);
							tcapi_commit(tmp);
						}
#endif
					}
					else
						set_disconn_count(wan_unit, get_disconn_count(wan_unit)+1);

					csprintf("%s: wan(%d) disconn count = %d/%d ...\n", __FUNCTION__, wan_unit, get_disconn_count(wan_unit), max_disconn_count);
				}
			}
		}
		else if(sw_mode == SW_MODE_ROUTER && !strcmp(dualwan_mode, "fo")){
			if(delay_detect == 1 && wandog_delay > 0){
				csprintf("wanduck: delay %d seconds...\n", wandog_delay);
				sleep(wandog_delay);
				delay_detect = 0;
			}

			// To check the phy connection of the standby line. 
			for(wan_unit = WAN_UNIT_FIRST; wan_unit < WAN_UNIT_MAX; ++wan_unit){
#if ASUSWRT
				if(get_dualwan_by_unit(wan_unit) != WANS_DUALWAN_IF_NONE)
#else
				if(get_dualwan_by_pvcunit(wan_unit) != WANS_DUALWAN_IF_NONE)
#endif
					conn_state[wan_unit] = if_wan_phyconnected(wan_unit);
			}

			current_wan_unit = wan_primary_ifunit();
#if ASUSWRT
			other_wan_unit = !current_wan_unit;
#else
			other_wan_unit = wan_secondary_ifunit();
#endif

			current_state[current_wan_unit] = nvram_get_int(nvram_state[current_wan_unit]);
if(test_log)
_dprintf("wanduck(%d) 3: conn_state %d, conn_state_old %d, conn_changed_state %d, current_state %d.\n"
		, current_wan_unit, conn_state[current_wan_unit], conn_state_old[current_wan_unit], conn_changed_state[current_wan_unit], current_state[current_wan_unit]);

			if(current_state[current_wan_unit] == WAN_STATE_DISABLED){
				//record_wan_state_nvram(current_wan_unit, WAN_STATE_STOPPED, WAN_STOPPED_REASON_MANUAL, -1);

				disconn_case[current_wan_unit] = CASE_OTHERS;
				conn_state[current_wan_unit] = DISCONN;
				set_disconn_count(current_wan_unit, S_IDLE);
			}
#ifdef RTCONFIG_USB_MODEM
			else if(dualwan_unit__usbif(current_wan_unit)
					&& (modem_act_reset == 1 || modem_act_reset == 2)
					){
_dprintf("wanduck(%d): detect the modem to be reset...\n", current_wan_unit);
				disconn_case[current_wan_unit] = CASE_OTHERS;
				conn_state[current_wan_unit] = DISCONN;
				set_disconn_count(current_wan_unit, S_IDLE);
			}
#endif
			else{
				if(conn_state[current_wan_unit] == CONNED){
#ifdef RTCONFIG_USB_MODEM
					if(!(dualwan_unit__usbif(current_wan_unit) && current_state[current_wan_unit] == WAN_STATE_INITIALIZING))
#endif
						conn_state[current_wan_unit] = if_wan_connected(current_wan_unit, current_state[current_wan_unit]);
if(test_log)
_dprintf("wanduck(%d) 4: conn_state %d, conn_state_old %d, conn_changed_state %d, current_state %d.\n"
		, current_wan_unit, conn_state[current_wan_unit], conn_state_old[current_wan_unit], conn_changed_state[current_wan_unit], current_state[current_wan_unit]);
				}
			}

			if(conn_state[current_wan_unit] == PHY_RECONN || conn_changed_state[current_wan_unit] == PHY_RECONN){
				conn_changed_state[current_wan_unit] = PHY_RECONN;

				conn_state_old[current_wan_unit] = DISCONN;

				// When the current line is re-plugged and the other line has plugged, the count has to be reset.
				if(link_wan[other_wan_unit]){
					csprintf("# wanduck: set S_COUNT: PHY_RECONN.\n");
					set_disconn_count(current_wan_unit, S_COUNT);
				}
			}
#ifdef RTCONFIG_USB_MODEM
			else if(conn_state[current_wan_unit] == SET_ETH_MODEM){
				conn_changed_state[current_wan_unit] = SET_ETH_MODEM;

				conn_state_old[current_wan_unit] = DISCONN;
				// The USB modem is a router type dongle, and must let the local subnet not be the "192.168.1.x".
				set_disconn_count(current_wan_unit, S_IDLE);
			}
			else if(conn_state[current_wan_unit] == SET_PIN){
				conn_changed_state[current_wan_unit] = SET_PIN;

				conn_state_old[current_wan_unit] = DISCONN;
				// The USB modem needs the PIN code to unlock.
				set_disconn_count(current_wan_unit, S_IDLE);
			}
#endif
			else if(conn_state[current_wan_unit] == CONNED){
				if(conn_state_old[current_wan_unit] == DISCONN)
					conn_changed_state[current_wan_unit] = D2C;
				else
					conn_changed_state[current_wan_unit] = CONNED;

				conn_state_old[current_wan_unit] = conn_state[current_wan_unit];
				set_disconn_count(current_wan_unit, S_IDLE);
			}
			else if(conn_state[current_wan_unit] == DISCONN){
				if(conn_state_old[current_wan_unit] == CONNED)
					conn_changed_state[current_wan_unit] = C2D;
				else
					conn_changed_state[current_wan_unit] = DISCONN;

				conn_state_old[current_wan_unit] = conn_state[current_wan_unit];

				if(disconn_case[current_wan_unit] == CASE_THESAMESUBNET){
					csprintf("# wanduck: set S_IDLE: CASE_THESAMESUBNET.\n");
					set_disconn_count(current_wan_unit, S_IDLE);
				}
#ifdef RTCONFIG_USB_MODEM
				// when the other line is modem and not plugged, the current disconnected line would not count.
				else if(!link_wan[other_wan_unit] && dualwan_unit__usbif(other_wan_unit))
					set_disconn_count(current_wan_unit, S_IDLE);
#endif
				else if(get_disconn_count(current_wan_unit) == S_IDLE && current_state[current_wan_unit] != WAN_STATE_DISABLED
#if ASUSWRT
						&& get_dualwan_by_unit(other_wan_unit) != WANS_DUALWAN_IF_NONE
#else
						&& get_dualwan_by_pvcunit(other_wan_unit) != WANS_DUALWAN_IF_NONE
#endif
						)
					set_disconn_count(current_wan_unit, S_COUNT);
			}

			if(get_disconn_count(current_wan_unit) != S_IDLE){
				if(get_disconn_count(current_wan_unit) < max_disconn_count){
					set_disconn_count(current_wan_unit, get_disconn_count(current_wan_unit)+1);
					csprintf("# wanduck(%d): wait time for switching: %d/%d.\n", current_wan_unit, get_disconn_count(current_wan_unit)*scan_interval, max_wait_time);
				}
				else{
					csprintf("# wanduck(%d): set S_COUNT: changed_count[] >= max_disconn_count.\n", current_wan_unit);
					set_disconn_count(current_wan_unit, S_COUNT);
				}
			}

			record_conn_status(current_wan_unit);
if(test_log)
_dprintf("wanduck(%d) 5: conn_state %d, conn_state_old %d, conn_changed_state %d, current_state %d.\n"
		, current_wan_unit, conn_state[current_wan_unit], conn_state_old[current_wan_unit], conn_changed_state[current_wan_unit], current_state[current_wan_unit]);
		}
		else if(sw_mode == SW_MODE_ROUTER && !strcmp(dualwan_mode, "fb")){
			if(delay_detect == 1 && wandog_delay > 0){
				csprintf("wanduck: delay %d seconds...\n", wandog_delay);
				sleep(wandog_delay);
				delay_detect = 0;
			}

			// To check the phy connection of the standby line. 
			for(wan_unit = WAN_UNIT_FIRST; wan_unit < WAN_UNIT_MAX; ++wan_unit){
#if ASUSWRT
				if(get_dualwan_by_unit(wan_unit) != WANS_DUALWAN_IF_NONE)
#else
				if(get_dualwan_by_pvcunit(wan_unit) != WANS_DUALWAN_IF_NONE)
#endif
					conn_state[wan_unit] = if_wan_phyconnected(wan_unit);
			}

			current_wan_unit = wan_primary_ifunit();
#if ASUSWRT
			other_wan_unit = !current_wan_unit;
#else
			other_wan_unit = wan_secondary_ifunit();
#endif

			current_state[current_wan_unit] = nvram_get_int(nvram_state[current_wan_unit]);

			if(current_state[current_wan_unit] == WAN_STATE_DISABLED){
				//record_wan_state_nvram(current_wan_unit, WAN_STATE_STOPPED, WAN_STOPPED_REASON_MANUAL, -1);

				disconn_case[current_wan_unit] = CASE_OTHERS;
				conn_state[current_wan_unit] = DISCONN;
				set_disconn_count(current_wan_unit, S_IDLE);
			}
#ifdef RTCONFIG_USB_MODEM
			else if(dualwan_unit__usbif(current_wan_unit)
					&& (modem_act_reset == 1 || modem_act_reset == 2)
					){
_dprintf("wanduck(%d): detect the modem to be reset...\n", current_wan_unit);
				disconn_case[current_wan_unit] = CASE_OTHERS;
				conn_state[current_wan_unit] = DISCONN;
				set_disconn_count(current_wan_unit, S_IDLE);
			}
#endif
			else{
				if(conn_state[current_wan_unit] == CONNED){
#ifdef RTCONFIG_USB_MODEM
					if(!(dualwan_unit__usbif(current_wan_unit) && current_state[current_wan_unit] == WAN_STATE_INITIALIZING))
#endif
						conn_state[current_wan_unit] = if_wan_connected(current_wan_unit, current_state[current_wan_unit]);
				}

				if(other_wan_unit == WAN_FB_UNIT && conn_state[other_wan_unit] == CONNED){
					current_state[other_wan_unit] = nvram_get_int(nvram_state[other_wan_unit]);
#ifdef RTCONFIG_USB_MODEM
					if(!(dualwan_unit__usbif(other_wan_unit) && current_state[other_wan_unit] == WAN_STATE_INITIALIZING))
#endif
						conn_state[other_wan_unit] = if_wan_connected(other_wan_unit, current_state[other_wan_unit]);
					csprintf("wanduck: detect the fail-back line(%d)...\n", other_wan_unit);
if(test_log)
_dprintf("wanduck(%d) fail-back: conn_state %d, conn_state_old %d, conn_changed_state %d, current_state %d.\n"
		, other_wan_unit, conn_state[other_wan_unit], conn_state_old[other_wan_unit], conn_changed_state[other_wan_unit], current_state[other_wan_unit]);
				}
			}

			if(conn_state[current_wan_unit] == PHY_RECONN || conn_changed_state[current_wan_unit] == PHY_RECONN){
				conn_changed_state[current_wan_unit] = PHY_RECONN;

				conn_state_old[current_wan_unit] = DISCONN;

				// When the current line is re-plugged and the other line has plugged, the count has to be reset.
				if(link_wan[other_wan_unit]){
					csprintf("# wanduck: set S_COUNT: PHY_RECONN.\n");
					set_disconn_count(current_wan_unit, S_COUNT);
				}
			}
#ifdef RTCONFIG_USB_MODEM
			else if(conn_state[current_wan_unit] == SET_ETH_MODEM){
				conn_changed_state[current_wan_unit] = SET_ETH_MODEM;

				conn_state_old[current_wan_unit] = DISCONN;
				// The USB modem is a router type dongle, and must let the local subnet not be the "192.168.1.x".
				set_disconn_count(current_wan_unit, S_IDLE);
			}
			else if(conn_state[current_wan_unit] == SET_PIN){
				conn_changed_state[current_wan_unit] = SET_PIN;

				conn_state_old[current_wan_unit] = DISCONN;
				// The USB modem needs the PIN code to unlock.
				set_disconn_count(current_wan_unit, S_IDLE);
			}
#endif
			else if(conn_state[current_wan_unit] == CONNED){
				if(conn_state_old[current_wan_unit] == DISCONN)
					conn_changed_state[current_wan_unit] = D2C;
				else
					conn_changed_state[current_wan_unit] = CONNED;

				conn_state_old[current_wan_unit] = conn_state[current_wan_unit];
				set_disconn_count(current_wan_unit, S_IDLE);
			}
			else if(conn_state[current_wan_unit] == DISCONN){
				if(conn_state_old[current_wan_unit] == CONNED)
					conn_changed_state[current_wan_unit] = C2D;
				else
					conn_changed_state[current_wan_unit] = DISCONN;

				conn_state_old[current_wan_unit] = conn_state[current_wan_unit];

				if(disconn_case[current_wan_unit] == CASE_THESAMESUBNET){
					csprintf("# wanduck: set S_IDLE: CASE_THESAMESUBNET.\n");
					set_disconn_count(current_wan_unit, S_IDLE);
				}
#ifdef RTCONFIG_USB_MODEM
				// when the other line is modem and not plugged, the current disconnected line would not count.
				else if(!link_wan[other_wan_unit] && dualwan_unit__usbif(other_wan_unit))
					set_disconn_count(current_wan_unit, S_IDLE);
#endif
				else if(get_disconn_count(current_wan_unit) == S_IDLE && current_state[current_wan_unit] != WAN_STATE_DISABLED
#if ASUSWRT
						&& get_dualwan_by_unit(other_wan_unit) != WANS_DUALWAN_IF_NONE
#else
						&& get_dualwan_by_pvcunit(other_wan_unit) != WANS_DUALWAN_IF_NONE
#endif
						)
					set_disconn_count(current_wan_unit, S_COUNT);
			}

			if(other_wan_unit == WAN_FB_UNIT){
				if(conn_state[other_wan_unit] == CONNED
						&& get_disconn_count(other_wan_unit) == S_IDLE
						)
					set_disconn_count(other_wan_unit, S_COUNT);
				else if(conn_state[other_wan_unit] == DISCONN)
					set_disconn_count(other_wan_unit, S_IDLE);
			}
			else
				set_disconn_count(other_wan_unit, S_IDLE);

			if(get_disconn_count(current_wan_unit) != S_IDLE){
				if(get_disconn_count(current_wan_unit) < max_disconn_count){
					set_disconn_count(current_wan_unit, get_disconn_count(current_wan_unit)+1);
					csprintf("# wanduck(%d): wait time for switching: %d/%d.\n", current_wan_unit, get_disconn_count(current_wan_unit)*scan_interval, max_wait_time);
				}
				else{
					csprintf("# wanduck(%d): set S_COUNT: changed_count[] >= max_disconn_count.\n", current_wan_unit);
					set_disconn_count(current_wan_unit, S_COUNT);
				}
			}

			if(get_disconn_count(other_wan_unit) != S_IDLE){
				if(get_disconn_count(other_wan_unit) < max_fb_count){
					set_disconn_count(other_wan_unit, get_disconn_count(other_wan_unit)+1);
					csprintf("# wanduck: wait time for returning: %d/%d.\n", get_disconn_count(other_wan_unit)*scan_interval, max_fb_wait_time);
				}
				else{
					csprintf("# wanduck: set S_COUNT: changed_count[] >= max_fb_count.\n");
					set_disconn_count(other_wan_unit, S_COUNT);
				}
			}

			record_conn_status(current_wan_unit);
		}
		else
#endif // RTCONFIG_DUALWAN
		if(sw_mode == SW_MODE_ROUTER){
			// To check the phy connection of the standby line. 
			for(wan_unit = WAN_UNIT_FIRST; wan_unit < WAN_UNIT_MAX; ++wan_unit)
				conn_state[wan_unit] = if_wan_phyconnected(wan_unit);

			current_wan_unit = wan_primary_ifunit();
#if ASUSWRT
			other_wan_unit = !current_wan_unit;
#else
			other_wan_unit = current_wan_unit;
#endif

			current_state[current_wan_unit] = nvram_get_int(nvram_state[current_wan_unit]);

			if(current_state[current_wan_unit] == WAN_STATE_DISABLED){
				//record_wan_state_nvram(current_wan_unit, WAN_STATE_STOPPED, WAN_STOPPED_REASON_MANUAL, -1);

				disconn_case[current_wan_unit] = CASE_OTHERS;
				conn_state[current_wan_unit] = DISCONN;

				set_disconn_count(current_wan_unit, S_IDLE);
			}
#ifdef RTCONFIG_USB_MODEM
			else if(dualwan_unit__usbif(current_wan_unit)
					&& (modem_act_reset == 1 || modem_act_reset == 2)
					){
_dprintf("wanduck(%d): detect the modem to be reset...\n", current_wan_unit);
				disconn_case[current_wan_unit] = CASE_OTHERS;
				conn_state[current_wan_unit] = DISCONN;
				set_disconn_count(current_wan_unit, S_IDLE);
			}
#endif
			else{
				conn_state[current_wan_unit] = if_wan_phyconnected(current_wan_unit);
				if(conn_state[current_wan_unit] == CONNED){
#ifdef RTCONFIG_USB_MODEM
					if(!(dualwan_unit__usbif(current_wan_unit) && current_state[current_wan_unit] == WAN_STATE_INITIALIZING))
#endif
						conn_state[current_wan_unit] = if_wan_connected(current_wan_unit, current_state[current_wan_unit]);
				}
			}

			if(conn_state[current_wan_unit] == PHY_RECONN || conn_changed_state[current_wan_unit] == PHY_RECONN){
				conn_changed_state[current_wan_unit] = PHY_RECONN;

				conn_state_old[current_wan_unit] = DISCONN;

				// When the current line is re-plugged and the other line has plugged, the count has to be reset.
				if(link_wan[other_wan_unit]){
					csprintf("# wanduck: set S_COUNT: PHY_RECONN.\n");
					set_disconn_count(current_wan_unit, S_COUNT);
				}
			}
#ifdef RTCONFIG_USB_MODEM
			else if(conn_state[current_wan_unit] == SET_ETH_MODEM){
				conn_changed_state[current_wan_unit] = SET_ETH_MODEM;

				conn_state_old[current_wan_unit] = DISCONN;
				// The USB modem is a router type dongle, and must let the local subnet not be the "192.168.1.x".
				set_disconn_count(current_wan_unit, S_IDLE);
			}
			else if(conn_state[current_wan_unit] == SET_PIN){
				conn_changed_state[current_wan_unit] = SET_PIN;

				conn_state_old[current_wan_unit] = DISCONN;
				// The USB modem needs the PIN code to unlock.
				set_disconn_count(current_wan_unit, S_IDLE);
			}
#endif
			else if(conn_state[current_wan_unit] == CONNED){
				if(conn_state_old[current_wan_unit] == DISCONN)
					conn_changed_state[current_wan_unit] = D2C;
				else
					conn_changed_state[current_wan_unit] = CONNED;

				conn_state_old[current_wan_unit] = conn_state[current_wan_unit];

				set_disconn_count(current_wan_unit, S_IDLE);
			}
			else if(conn_state[current_wan_unit] == DISCONN){
				if(conn_state_old[current_wan_unit] == CONNED)
					conn_changed_state[current_wan_unit] = C2D;
				else
					conn_changed_state[current_wan_unit] = DISCONN;

				conn_state_old[current_wan_unit] = conn_state[current_wan_unit];

				if(disconn_case[current_wan_unit] == CASE_THESAMESUBNET){
					csprintf("# wanduck: set S_IDLE: CASE_THESAMESUBNET.\n");
					set_disconn_count(current_wan_unit, S_IDLE);
				}
#ifdef RTCONFIG_USB_MODEM
				// when the other line is modem and not plugged, the current disconnected line would not count.
				else if(!link_wan[other_wan_unit] && dualwan_unit__usbif(other_wan_unit))
					set_disconn_count(current_wan_unit, S_IDLE);
				else if(get_disconn_count(current_wan_unit) == S_IDLE && current_state[current_wan_unit] != WAN_STATE_DISABLED)
					set_disconn_count(current_wan_unit, S_COUNT);
#else
				else
					set_disconn_count(current_wan_unit, S_IDLE);
#endif
			}

			if(get_disconn_count(current_wan_unit) != S_IDLE){
				if(get_disconn_count(current_wan_unit) < max_disconn_count){
					set_disconn_count(current_wan_unit, get_disconn_count(current_wan_unit)+1);
					csprintf("# wanduck(%d): wait time for switching: %d/%d.\n", current_wan_unit, get_disconn_count(current_wan_unit)*scan_interval, max_wait_time);
				}
				else{
					csprintf("# wanduck(%d): set S_COUNT: changed_count[] >= max_disconn_count.\n", current_wan_unit);
					set_disconn_count(current_wan_unit, S_COUNT);
				}
			}

			record_conn_status(current_wan_unit);
		}
		else{ // sw_mode == SW_MODE_AP, SW_MODE_REPEATER.
			current_wan_unit = WAN_UNIT_FIRST;
			conn_state[current_wan_unit] = if_wan_phyconnected(current_wan_unit);

			if(conn_state[current_wan_unit] == DISCONN){
				if(conn_state_old[current_wan_unit] == CONNED)
					conn_changed_state[current_wan_unit] = C2D;
				else
					conn_changed_state[current_wan_unit] = DISCONN;
			}
			else{
				if(conn_state_old[current_wan_unit] == DISCONN)
					conn_changed_state[current_wan_unit] = D2C;
				else
					conn_changed_state[current_wan_unit] = CONNED;
			}

			conn_state_old[current_wan_unit] = conn_state[current_wan_unit];
		}
if(test_log)
_dprintf("wanduck(%d) 6: conn_state %d, conn_state_old %d, conn_changed_state %d, current_state %d.\n"
		, current_wan_unit, conn_state[current_wan_unit], conn_state_old[current_wan_unit], conn_changed_state[current_wan_unit], current_state[current_wan_unit]);

#ifdef RTCONFIG_DUALWAN
		if(sw_mode == SW_MODE_ROUTER && !strcmp(dualwan_mode, "lb")){
			if(rule_setup && cross_state == CONNED && !isFirstUse){
				csprintf("\n# DualWAN: Disable direct rule(isFirstUse)\n");
				rule_setup = 0;
				start_nat_rules();
			}
		}
		else
#endif
		if(conn_changed_state[current_wan_unit] == C2D || (conn_changed_state[current_wan_unit] == CONNED && isFirstUse)){
			if(rule_setup == 0){
				if(conn_changed_state[current_wan_unit] == C2D){
					csprintf("\n# Enable direct rule(C2D)\n");
				}
				else
					csprintf("\n# Enable direct rule(isFirstUse)\n");
				rule_setup = 1;

				handle_wan_line(current_wan_unit, rule_setup);

				if(conn_changed_state[current_wan_unit] == C2D
#ifdef RTCONFIG_DUALWAN
						&& strcmp(dualwan_mode, "off")
#endif
						){
#ifdef RTCONFIG_USB_MODEM
					// the current line is USB and be plugged off.
					if(!link_wan[current_wan_unit] && dualwan_unit__usbif(current_wan_unit)){
#if ASUSWRT
						if(get_dualwan_by_unit(other_wan_unit) != WANS_DUALWAN_IF_NONE)
#else
						if(get_dualwan_by_pvcunit(other_wan_unit) != WANS_DUALWAN_IF_NONE)
#endif
						{
							csprintf("\n# wanduck(C2D): Modem was plugged off and try to Switch the other line.\n");
							switch_wan_line(other_wan_unit, 0);
						}
						else if(current_state[current_wan_unit] != WAN_STATE_INITIALIZING){
							csprintf("wanduck: stop_wan_if %d.\n", current_wan_unit);
#if ASUSWRT
							snprintf(cmd, 32, "stop_wan_if %d", current_wan_unit);
							notify_rc(cmd);
#else
							memset(tmp, 0, sizeof(tmp));
#ifdef TCSUPPORT_MULTISERVICE_ON_WAN
							if(current_wan_unit == WAN_UNIT_PTM0 || current_wan_unit == WAN_UNIT_ETH)
								sprintf(tmp, "WanExt_PVC%de0", current_wan_unit);
							else
#endif
								sprintf(tmp, "Wan_PVC%d", current_wan_unit);
							//tcapi_set(tmp, "Active", "No");
							csprintf("Stop %s\n", tmp);
							tcapi_commit(tmp);
#endif
						}
					}
					else
#endif
					// C2D: Try to prepare the backup line.
					if(link_wan[other_wan_unit] == 1){
						if(get_wan_state(other_wan_unit) != WAN_STATE_CONNECTED){
							csprintf("\n# wanduck(C2D): Try to prepare the backup line.\n");
#if ASUSWRT
							memset(cmd, 0, 32);
							sprintf(cmd, "restart_wan_if %d", other_wan_unit);
							notify_rc_and_wait(cmd);
#else
							memset(tmp, 0, sizeof(tmp));
							sprintf(tmp, "Wan_PVC%d", other_wan_unit);
							tcapi_commit(tmp);
#endif
						}
					}
				}
			}
		}
		else if(conn_changed_state[current_wan_unit] == D2C || conn_changed_state[current_wan_unit] == CONNED){
			if(rule_setup == 1 && !isFirstUse){
				csprintf("\n# Disable direct rule(D2C)\n");
				rule_setup = 0;
				handle_wan_line(current_wan_unit, rule_setup);
			}
		}
		/*
		 * when all lines are plugged in and the currect line is disconnected over max_wait_time seconds,
		 * switch the connect to the other line.
		 */
		else if(conn_changed_state[current_wan_unit] == DISCONN){
			if(
#if ASUSWRT
			get_dualwan_by_unit(other_wan_unit) != WANS_DUALWAN_IF_NONE
#else
			get_dualwan_by_pvcunit(other_wan_unit) != WANS_DUALWAN_IF_NONE
#endif
					&& (get_disconn_count(current_wan_unit) >= max_disconn_count
#ifdef RTCONFIG_USB_MODEM
							|| (!link_wan[current_wan_unit] && dualwan_unit__usbif(current_wan_unit))
#endif
							)
					)
			{
#if ASUSWRT
				if(current_wan_unit)
					csprintf("# wanduck(%d): Switching the connect to the first WAN line...\n", current_wan_unit);
				else
					csprintf("# wanduck(%d): Switching the connect to the second WAN line...\n", current_wan_unit);
#else
				csprintf("# Switching the connect from WAN_PVC %d to %d...\n", current_wan_unit, other_wan_unit);
#endif
				if (Force_off_GuestWL && other_wan_unit != WAN_FB_UNIT) {
					/* only turn off when switch to secondary wan */
					ForceControlWLan("0");
				} else {
					ForceControlWLan("1");
				}
				
				if(!link_wan[current_wan_unit] && dualwan_unit__usbif(current_wan_unit))
					switch_wan_line(other_wan_unit, 0);
				else
					switch_wan_line(other_wan_unit, 1);
			}
		}
		// phy connected -> disconnected -> connected
		else if(conn_changed_state[current_wan_unit] == PHY_RECONN){
#ifdef RTCONFIG_USB_MODEM
			if(dualwan_unit__usbif(current_wan_unit)){
				//wan_start.sh will change the state to WAN_STATE_CONNECTING before wan_if_up()...
				if(current_state[current_wan_unit] == WAN_STATE_INITIALIZING
#if !ASUSWRT
						|| current_state[current_wan_unit] == WAN_STATE_CONNECTING
#endif
						){
					if(!strcmp(modem_type, "qmi")){
						csprintf("wanduck(%d): Sleep 1 seconds to wait QMI modem nodes.\n", current_wan_unit);
						sleep(1);
					}
					handle_wan_line(current_wan_unit, 0);
				}
				else
					csprintf("wanduck(%d): the modem had been run...\n", current_wan_unit);
			}
			else
#endif
				handle_wan_line(current_wan_unit, 0);
#if ASUSWRT
			conn_changed_state[current_wan_unit] = conn_state[current_wan_unit];
#else
			conn_changed_state[current_wan_unit] = CONNED;
#endif
		}

#ifdef RTCONFIG_DUALWAN
		if(!strcmp(dualwan_mode, "fb") && other_wan_unit == WAN_FB_UNIT && conn_state[other_wan_unit] == CONNED
				&& get_disconn_count(other_wan_unit) >= max_fb_count
				){
#if ASUSWRT
			csprintf("# wanduck: returning to the primary WAN line(%d)...\n", other_wan_unit);
#else
			csprintf("# wanduck: returning the connect to the %d WAN line...\n", other_wan_unit);
#endif
			rule_setup = 1;
			handle_wan_line(other_wan_unit, rule_setup);
			
			if (Resume_GuestWL) {
				ForceControlWLan("1");
			}

			switch_wan_line(other_wan_unit, 1);
		}
		// hot-standby: Try to prepare the backup line.
		else if(!strcmp(dualwan_mode, "fo") || !strcmp(dualwan_mode, "fb")){
			if(nvram_get_int("wans_standby") == 1 && link_wan[other_wan_unit] == 1 && get_wan_state(other_wan_unit) == WAN_STATE_INITIALIZING){
				csprintf("\n# wanduck(hot-standby): Try to prepare the backup line.\n");
#if ASUSWRT
				memset(cmd, 0, 32);
				sprintf(cmd, "restart_wan_if %d", other_wan_unit);
				notify_rc_and_wait(cmd);
#else
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "Wan_PVC%d", other_wan_unit);
				tcapi_commit(tmp);
#endif
			}
		}
#endif

#if ASUSWRT
		start_demand_ppp(current_wan_unit, 1);
#endif

WANDUCK_SELECT:
		if((nready = select(maxfd+1, &rset, NULL, NULL, &tval)) <= 0)
			continue;

		if(FD_ISSET(dns_sock, &rset)){
			run_dns_serv(dns_sock);
			if(--nready <= 0)
				continue;
		}
		else if(FD_ISSET(http_sock, &rset)){
			if((cur_sockfd = accept(http_sock, (struct sockaddr *)&cliaddr, (socklen_t *)&clilen)) <= 0){
				perror("http accept");
				continue;
			}

			for(fd_i = 0; fd_i < MAX_USER; ++fd_i){
				if(client[fd_i].sfd < 0){
					client[fd_i].sfd = cur_sockfd;
					client[fd_i].type = T_HTTP;
					break;
				}
			}

			if(fd_i == MAX_USER){
				csprintf("# wanduck servs full\n");
				close(cur_sockfd);
				continue;
			}

			FD_SET(cur_sockfd, &allset);
			if(cur_sockfd > maxfd)
				maxfd = cur_sockfd;
			if(fd_i > maxi)
				maxi = fd_i;
			
			if(--nready <= 0)
				continue;	// no more readable descriptors
		}

		// polling
		for(fd_i = 0; fd_i <= maxi; ++fd_i){
			if((sockfd = client[fd_i].sfd) < 0)
				continue;

			if(FD_ISSET(sockfd, &rset)){
				int nread;
				ioctl(sockfd, FIONREAD, &nread);
				if(nread == 0){
					close_socket(sockfd, T_HTTP);
					continue;
				}
				if(fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0)|O_NONBLOCK) < 0){
					_dprintf("wanduck set http req [%d] nonblock fail !\n", sockfd);
					continue;
				}

				cur_sockfd = sockfd;

				run_http_serv(sockfd);

				if(--nready <= 0)
					break;
			}
		}
	}

	csprintf("# wanduck exit error\n");
	exit(1);
}
