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
 *
 * Copyright 2004, ASUSTeK Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND ASUS GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 */
#include "rc.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>															
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <signal.h>
#include "libtcapi.h"

#define L2TP_VPNC_PID	"/var/run/l2tpd-vpnc.pid"
#define L2TP_VPNC_CTRL	"/var/run/l2tpctrl-vpnc"
#define L2TP_VPNC_CONF	"/tmp/l2tp-vpnc.conf"
#define VPNC_NODE	"VPNC_Entry"

int vpnc_unit = 20;

static int
handle_special_char_for_vpnclient(char *buf, size_t buf_size, char *src)
{
	const char special_chars[] = "'\\";
	char *p, *q;
	size_t len;

	if (!buf || !src || buf_size <= 1)
		return -1;

	for (p = src, q = buf, len = buf_size; *p != '\0' && len > 1; ++p, --len) {
		if (strchr(special_chars, *p))
			*q++ = '\\';

		*q++ = *p;
	}

	*q++ = '\0';

	return 0;
}

int vpnc_pppstatus(void)
{
	FILE *fp;
	char sline[128], buf[128], *p;

	if ((fp=fopen("/tmp/vpncstatus.log", "r")) && fgets(sline, sizeof(sline), fp))
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

int
start_vpnc(void)
{
	FILE *fp;
	char options[80];
	char *pppd_argv[] = { "/usr/sbin/pppd", "file", options, NULL};
	char tmp[100], wan_prefix[] = "WanXXXXXXXXXX_";
	char buf[256];	/* although maximum length of pppoe_username/pppoe_passwd is 64. pppd accepts up to 256 characters. */
	mode_t mask;
	int ret = 0;
	char proto[8], ip_ver[16];

	snprintf(wan_prefix, sizeof(wan_prefix), "Wan_PVC%d", wan_primary_ifunit());
#if 0
	if (nvram_match(strcat_r(wan_prefix, "proto", tmp), "pptp") || nvram_match(strcat_r(wan_prefix, "proto", tmp), "l2tp"))
		return 0;
#endif
	memset(proto, 0, sizeof(proto));
	tcapi_get(VPNC_NODE, "proto", proto);
	if (!strcmp(proto, "pptp"))
		sprintf(options, "/tmp/ppp/vpnc_options.pptp");
	else if (!strcmp(proto, "l2tp"))
		sprintf(options, "/tmp/ppp/vpnc_options.l2tp");
	else
		return 0;

	/* shut down previous instance if any */
	stop_vpnc();

	/* reset dut_disc */
	//tcapi_set(VPNC_NODE, "dut_disc", "0");

	update_vpnc_state(WAN_STATE_INITIALIZING, 0);

	mask = umask(0000);

	/* Generate options file */
	if (!(fp = fopen(options, "w"))) {
		perror(options);
		umask(mask);
		return -1;
	}

	umask(mask);
#if 0
	/* route for pptp/l2tp's server */
	if (nvram_match(strcat_r(wan_prefix, "proto", tmp), "pptp") || nvram_match(strcat_r(wan_prefix, "proto", tmp), "l2tp")) {
		char *wan_ifname = nvram_safe_get(strcat_r(wan_prefix, "pppoe_ifname", tmp));
		route_add(wan_ifname, 0, nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0", "255.255.255.255");
	}
#endif

	/* do not authenticate peer and do not use eap */
	fprintf(fp, "noauth\n");
	fprintf(fp, "refuse-eap\n");
	tcapi_get(VPNC_NODE, "username", tmp);
	handle_special_char_for_vpnclient(buf, sizeof(buf), tmp);
	fprintf(fp, "user '%s'\n", buf);
	tcapi_get(VPNC_NODE, "passwd", tmp);
	handle_special_char_for_vpnclient(buf, sizeof(buf), tmp);
	fprintf(fp, "password '%s'\n", buf);

	//if (nvram_match(strcat_r(prefix, "proto", tmp), "pptp")) {
	if (!strcmp(proto, "pptp")) {
		fprintf(fp, "plugin pptp.so\n");

		memset(tmp, 0, sizeof(tmp));
		tcapi_get(VPNC_NODE, "heartbeat", tmp);
		fprintf(fp, "pptp_server '%s'\n", tmp);
			//nvram_invmatch(strcat_r(prefix, "heartbeat_x", tmp), "") ?
			//nvram_safe_get(strcat_r(prefix, "heartbeat_x", tmp)) :
			//nvram_safe_get(strcat_r(prefix, "gateway_x", tmp)));
		fprintf(fp, "vpnc 1\n");
		/* see KB Q189595 -- historyless & mtu */
#if 0
		if (nvram_match(strcat_r(wan_prefix, "proto", tmp), "pptp") || nvram_match(strcat_r(wan_prefix, "proto", tmp), "l2tp"))
			fprintf(fp, "nomppe-stateful mtu 1300\n");
		else
			fprintf(fp, "nomppe-stateful mtu 1400\n");
#endif
		fprintf(fp, "nomppe-stateful mtu 1400\n");

		memset(tmp, 0, sizeof(tmp));
		tcapi_get(VPNC_NODE, "pptp_options", tmp);
		//if (nvram_match(strcat_r(prefix, "pptp_options_x", tmp), "-mppc")) {
		if (!strcmp(tmp, "-mppc")) {
			fprintf(fp, "nomppe nomppc\n");
		} else
		//if (nvram_match(strcat_r(prefix, "pptp_options_x", tmp), "+mppe-40")) {
		if (!strcmp(tmp, "+mppe-40")) {
			fprintf(fp, //"nomppe-56\n"
					"nomppe-128\n"
					"require-mppe\n"
					"require-mppe-40\n");
		} else
		//if (nvram_match(strcat_r(prefix, "pptp_options_x", tmp), "+mppe-56")) {
		//if (!strcmp(tmp, "+mppe-56")) {
			//fprintf(fp, "nomppe-40\n"
				    //"nomppe-128\n"
				    //"require-mppe\n"
				    //"require-mppe-56\n");
		//} else
		//if (nvram_match(strcat_r(prefix, "pptp_options_x", tmp), "+mppe-128")) {
		if (!strcmp(tmp, "+mppe-128")) {
			fprintf(fp, "nomppe-40\n"
					//"nomppe-56\n"
					"require-mppe\n"
					"require-mppe-128\n");
		}
	} else {
		//fprintf(fp, "nomppe nomppc\n");
		fprintf(fp, "nomppe\n");
#if 0
		if (nvram_match(strcat_r(wan_prefix, "proto", tmp), "pptp") || nvram_match(strcat_r(wan_prefix, "proto", tmp), "l2tp"))
			fprintf(fp, "mtu 1300\n");
		else
			fprintf(fp, "mtu 1400\n");
#endif			
	}

#if 0
	if (nvram_invmatch(strcat_r(prefix, "proto", tmp), "l2tp")) {
		ret = nvram_get_int(strcat_r(prefix, "pppoe_idletime", tmp));
		if (ret && nvram_get_int(strcat_r(prefix, "pppoe_demand", tmp))) {
			fprintf(fp, "idle %d ", ret);
			if (nvram_invmatch(strcat_r(prefix, "pppoe_txonly_x", tmp), "0"))
				fprintf(fp, "tx_only ");
			fprintf(fp, "demand\n");
		}
		fprintf(fp, "persist\n");
	}
#endif
	fprintf(fp, "persist\n");

#if 0
	fprintf(fp, "holdoff %d\n", nvram_get_int(strcat_r(prefix, "pppoe_holdoff", tmp)) ? : 10);
	fprintf(fp, "maxfail %d\n", nvram_get_int(strcat_r(prefix, "pppoe_maxfail", tmp)));
#endif
	fprintf(fp, "holdoff 10\n");
	fprintf(fp, "maxfail 0\n");

#if 0
	if (nvram_invmatch(strcat_r(prefix, "dnsenable_x", tmp), "0"))
		fprintf(fp, "usepeerdns\n");
#endif
	fprintf(fp, "usepeerdns\n");

	fprintf(fp, "ipcp-accept-remote ipcp-accept-local noipdefault\n");
	fprintf(fp, "ktune\n");

	/* pppoe set these options automatically */
	/* looks like pptp also likes them */
	fprintf(fp, "default-asyncmap nopcomp noaccomp\n");

	/* pppoe disables "vj bsdcomp deflate" automagically */
	/* ccp should still be enabled - mppe/mppc requires this */
	fprintf(fp, "novj nobsdcomp nodeflate\n");

	/* echo failures */
	fprintf(fp, "lcp-echo-interval 6\n");
	fprintf(fp, "lcp-echo-failure 10\n");

#if 0
	/* pptp has Echo Request/Reply, l2tp has Hello packets */
	if (nvram_match(strcat_r(prefix, "proto", tmp), "pptp") ||
	    nvram_match(strcat_r(prefix, "proto", tmp), "l2tp"))
		fprintf(fp, "lcp-echo-adaptive\n");
#endif

	fprintf(fp, "unit %d\n", vpnc_unit);
	fprintf(fp, "linkname vpn%d\n", vpnc_unit);
	fprintf(fp, "ip-up-script %s\n", "/tmp/ppp/vpnc-ip-up");
	fprintf(fp, "ip-down-script %s\n", "/tmp/ppp/vpnc-ip-down");
	fprintf(fp, "ip-pre-up-script %s\n", "/tmp/ppp/vpnc-ip-pre-up");
	fprintf(fp, "auth-fail-script %s\n", "/tmp/ppp/vpnc-auth-fail");

#ifdef TCSUPPORT_IPV6
	memset(ip_ver, 0, sizeof(ip_ver));
	tcapi_get(wan_prefix, "IPVERSION", ip_ver);
	if (strcmp(ip_ver, "IPv4")) {
		fprintf(fp, "ip-up-script %s\n", "/tmp/ppp/vpnc-ipv6-up");
		fprintf(fp, "ip-down-script %s\n", "/tmp/ppp/vpnc-ipv6-down");
	}

	if (!strcmp(ip_ver, "IPv4/IPv6"))
		fprintf(fp, "ipv6 , \n"); 	
	else if (!strcmp(ip_ver, "IPv6"))
		fprintf(fp, "ipv6 , noip\n");
#endif
	
	/* user specific options */
	//fprintf(fp, "%s\n", nvram_safe_get(strcat_r(prefix, "pppoe_options_x", tmp)));
	memset(tmp, 0, sizeof(tmp));
	tcapi_get(VPNC_NODE, "pppoe_options", tmp);
	fprintf(fp, "%s\n", tmp);
	
	fclose(fp);

	//if (nvram_match(strcat_r(prefix, "proto", tmp), "l2tp"))
	if (!strcmp(proto, "l2tp"))
	{
		if (!(fp = fopen(L2TP_VPNC_CONF, "w"))) {
			perror(options);
			return -1;
		}
		
		memset(tmp, 0, sizeof(tmp));
		tcapi_get(VPNC_NODE, "heartbeat", tmp);

		fprintf(fp, "# automagically generated\n"
			"global\n\n"
			"load-handler \"sync-pppd.so\"\n"
			"load-handler \"cmd.so\"\n\n"
			"section sync-pppd\n\n"
			"lac-pppd-opts \"file %s\"\n\n"
			"section peer\n"
			"port 1701\n"
			"peername %s\n"
			"vpnc 1\n"
			//"hostname %s\n"
			"hostname localhost\n"
			"lac-handler sync-pppd\n"
			"persist yes\n"
			//"maxfail %d\n"
			"maxfail 32767\n"
			//"holdoff %d\n"
			"holdoff 10\n"
			"hide-avps no\n"
			"section cmd\n\n",
			//"socket-path " L2TP_VPNC_CTRL "\n\n",
			options, tmp);
#if 0
                        nvram_invmatch(strcat_r(prefix, "heartbeat_x", tmp), "") ?
                                nvram_safe_get(strcat_r(prefix, "heartbeat_x", tmp)) :
                                nvram_safe_get(strcat_r(prefix, "gateway_x", tmp)),
			nvram_invmatch(strcat_r(prefix, "hostname", tmp), "") ?
				nvram_safe_get(strcat_r(prefix, "hostname", tmp)) : "localhost",
			nvram_get_int(strcat_r(prefix, "pppoe_maxfail", tmp))  ? : 32767,
			nvram_get_int(strcat_r(prefix, "pppoe_holdoff", tmp)) ? : 10);
#endif

		fclose(fp);

		/* launch l2tp */
		eval("/usr/sbin/l2tpd", "-c", L2TP_VPNC_CONF, "-p", L2TP_VPNC_PID);

		ret = 3;
		do {
			_dprintf("%s: wait l2tpd up at %d seconds...\n", __FUNCTION__, ret);
			usleep(1000*1000);
		} while (!pids("l2tpd") && ret--);

		/* start-session */
		//ret = eval("/usr/sbin/l2tp-control", "-s", L2TP_VPNC_CTRL, "start-session 0.0.0.0");
		ret = eval("/usr/sbin/l2tp-control", "start-session 0.0.0.0");
		
		/* pppd sync nodetach noaccomp nobsdcomp nodeflate */
		/* nopcomp novj novjccomp file /tmp/ppp/options.l2tp */

	} else
		ret = _eval(pppd_argv, NULL, 0, NULL);

	update_vpnc_state(WAN_STATE_CONNECTING, 0);
	
	return ret;
}

void
stop_vpnc(void)
{
	char pidfile[sizeof("/var/run/ppp-vpnXXXXXXXXXX.pid")];
	FILE *fp;
	char buf[256];
	pid_t pid = 0;
	int n;

	snprintf(pidfile, sizeof(pidfile), "/var/run/ppp-vpn%d.pid", vpnc_unit);

	/* Reset the state of dut_disc */
	//tcapi_set(VPNC_NODE, "dut_disc", "1");

	/* Stop l2tp */
	if(check_if_file_exist(L2TP_VPNC_PID))
	{
		kill_pidfile_tk(L2TP_VPNC_PID);
		usleep(1000*10000);
	}

	/* Stop pppd */
	if ((fp = fopen(pidfile, "r")) != NULL) {
		if (fgets(buf, sizeof(buf), fp) != NULL)
			pid = strtoul(buf, NULL, 0);
		fclose(fp);
	}
	if (pid > 1 && kill(pid, SIGHUP) == 0 && kill(pid, SIGTERM) == 0) {
		n = 10;
		while ((kill(pid, 0) == 0) && (n-- > 0)) {
			_dprintf("%s: waiting pid=%d n=%d\n", __FUNCTION__, pid, n);
			sleep(1);
		}
		if (n < 0) {
			n = 10;
			while ((kill(pid, SIGKILL) != 0) && (n-- > 0)) {
				_dprintf("%s: SIGKILL pid=%d n=%d\n", __FUNCTION__, pid, n);
				sleep(1);
			}
			start_dnsmasq();
		}
	}
}

int
vpnc_ppp_linkunit(char *linkname)
{
	if (strncmp(linkname, "vpn", 3))
		return -1;
	if (!isdigit(linkname[3]))
		return -1;
	return atoi(&linkname[3]);
}

void update_vpnc_state(int state, int reason)
{
	char tmp[16];

	_dprintf("%s(%d, %d)\n", __FUNCTION__, state, reason);

	//nvram_set_int(strcat_r(prefix, "state_t", tmp), state);
	//nvram_set_int(strcat_r(prefix, "sbstate_t", tmp), 0);
	snprintf(tmp, sizeof(tmp), "%d", state);
	tcapi_set(VPNC_NODE, "state_t", tmp);
	tcapi_set(VPNC_NODE, "sbstate_t", "0"); 

	// 20110610, reset auxstate each time state is changed
	//nvram_set_int(strcat_r(prefix, "auxstate_t", tmp), 0);

	if (state == WAN_STATE_STOPPED) {
		// Save Stopped Reason
		// keep ip info if it is stopped from connected
		//nvram_set_int(strcat_r(prefix, "sbstate_t", tmp), reason);
		snprintf(tmp, sizeof(tmp), "%d", reason);
		tcapi_set(VPNC_NODE, "sbstate_t", tmp);
	}
	else if(state == WAN_STATE_STOPPING){
		unlink("/tmp/vpncstatus.log");
	}
}

void vpnc_add_firewall_rule(char *vpnc_ifname)
{
	char tmp[100];//, prefix[] = "vpnc_", wan_prefix[] = "WanXXXXXXXXXX_";
	//char *vpnc_ifname = nvram_safe_get(strcat_r(prefix, "pppoe_ifname", tmp));
	//char *wan_proto = NULL;
	//char lan_if[IFNAMSIZ+1];

	//strcpy(lan_if, nvram_safe_get("lan_ifname"));

	if (check_if_file_exist(strcat_r("/tmp/ppp/link.", vpnc_ifname, tmp)))
	{
#if 0
		snprintf(wan_prefix, sizeof(wan_prefix), "wan%d_", wan_primary_ifunit());
		wan_proto = nvram_safe_get(strcat_r(wan_prefix, "proto", tmp));
		if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static"))
			//eval("iptables", "-I", "FORWARD", "-p", "tcp", "--syn", "-j", "TCPMSS", "--clamp-mss-to-pmtu");
			eval("iptables", "-I", "FORWARD", "-p", "tcp", "--tcp-flags", "SYN,RST", "SYN", "-j", "TCPMSS", "--clamp-mss-to-pmtu");
#ifdef RTCONFIG_BCMARM
		else	/* mark tcp connection to bypass CTF */
			eval("iptables", "-t", "mangle", "-A", "FORWARD", "-p", "tcp", 
				"-m", "state", "--state", "NEW","-j", "MARK", "--set-mark", "0x01");
#endif

		eval("iptables", "-A", "FORWARD", "-o", vpnc_ifname, "!", "-i", lan_if, "-j", "DROP");
		eval("iptables", "-t", "nat", "-I", "PREROUTING", "-d", 
			nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)), "-j", "VSERVER");
		eval("iptables", "-t", "nat", "-I", "POSTROUTING", "-o", 
			vpnc_ifname, "!", "-s", nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)), "-j", "MASQUERADE");
#endif
		eval("iptables", "-A", "FORWARD", "-o", vpnc_ifname, "-p", "tcp", "--tcp-flags", "SYN,RST", "SYN", "-j", "TCPMSS", "--clamp-mss-to-pmtu");
		eval("iptables", "-A", "FORWARD", "-i", vpnc_ifname, "-j", "FORWARD_WAN");
		eval("iptables", "-t", "nat", "-A", "PREROUTING", "-i", vpnc_ifname,"-j", "VSERVER");
		eval("iptables", "-t", "nat", "-A", "POSTROUTING", "-o", vpnc_ifname, "-j", "MASQUERADE");
	}
}

void
vpnc_up(char *vpnc_ifname)
{
	char tmp[100], wan_prefix[] = "WanXXXXXXXXXX_";
//	char *wan_ifname = NULL, *wan_proto = NULL;
	char wan_ifname[16], wan_proto[16], wan_gateway[16];
	int wan_pvcunit = wan_primary_ifunit();

	snprintf(wan_prefix, sizeof(wan_prefix), "wan%d_", wan_pvcunit);
	snprintf(wan_proto, sizeof(wan_proto), "%s", nvram_safe_get(strcat_r(wan_prefix, "proto", tmp)));

	if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static"))
		snprintf(wan_ifname, sizeof(wan_ifname), "%s", nvram_safe_get(strcat_r(wan_prefix, "ifname", tmp)));
	else
		snprintf(wan_ifname, sizeof(wan_ifname), "%s", nvram_safe_get(strcat_r(wan_prefix, "pppoe_ifname", tmp)));

#if 0
	/* Reset default gateway route via PPPoE interface */
	if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static")) {
		route_del(wan_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
		route_add(wan_ifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
	}
	else if (!strcmp(wan_proto, "pppoe") || !strcmp(wan_proto, "pptp") || !strcmp(wan_proto,  "l2tp"))
	{
		char *wan_xgateway = nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp));

		route_del(wan_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
		route_add(wan_ifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");

		if (strlen(wan_xgateway) > 0 && strcmp(wan_xgateway, "0.0.0.0")) {
			char *wan_xifname =  nvram_safe_get(strcat_r(wan_prefix, "ifname", tmp));

			route_del(wan_xifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp)), "0.0.0.0");
			route_add(wan_xifname, 3, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp)), "0.0.0.0");
		}
	}
#endif
	
	/* Reset default gateway route via PPPoE interface */
	tcapi_get(WANDUCK_DATA, strcat_r(wan_prefix, "gateway", tmp), wan_gateway);
	eval("route", "del", "default", "gw", wan_gateway, "dev", wan_ifname);
	eval("route", "add", "default", "gw", wan_gateway, "metric", "1", "dev", wan_ifname);

	/* Add the default gateway of VPN client */
	//route_add(vpnc_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(prefix, "gateway", tmp)), "0.0.0.0");
	tcapi_get(VPNC_NODE, "gateway_x", tmp);
	eval("route", "add", "default", "gw", tmp, "dev", vpnc_ifname);

	/* Remove route to the gateway - no longer needed */
	//route_del(vpnc_ifname, 0, nvram_safe_get(strcat_r(prefix, "gateway", tmp)), NULL, "255.255.255.255");
	eval("route", "del", "-net", tmp, "netmask", "255.255.255.255", "dev", vpnc_ifname);

	/* Add firewall rules for VPN client */
	vpnc_add_firewall_rule(vpnc_ifname);

	update_vpnc_state(WAN_STATE_CONNECTED, 0);

	/* Restart dnsmasq for vpnc */
	start_dnsmasq();
}

int
vpnc_ipup_main(int argc, char **argv)
{
	FILE *fp;
	char *vpnc_ifname = safe_getenv("IFNAME");
	char *vpnc_linkname = safe_getenv("LINKNAME");
	char tmp[100];//, prefix[] = "vpnc_";
	char buf[256], *value;
	int unit;

	_dprintf("%s():: %s\n", __FUNCTION__, argv[0]);

	/* Get unit from LINKNAME: vpn[UNIT] */
	if ((unit = vpnc_ppp_linkunit(vpnc_linkname)) < 0)
		return 0;

	_dprintf("%s: unit=%d ifname=%s\n", __FUNCTION__, unit, vpnc_ifname);

	/* Touch connection file */
	if (!(fp = fopen(strcat_r("/tmp/ppp/link.", vpnc_ifname, tmp), "a"))) {
		perror(tmp);
		return errno;
	}
	fclose(fp);

	if ((value = getenv("IPLOCAL"))) {
		//if (nvram_invmatch(strcat_r(prefix, "ipaddr", tmp), value))
		//	ifconfig(vpnc_ifname, IFUP, "0.0.0.0", NULL);
		//_ifconfig(vpnc_ifname, IFUP, value, "255.255.255.255", getenv("IPREMOTE"));
		//nvram_set(strcat_r(prefix, "ipaddr", tmp), value);
		//nvram_set(strcat_r(prefix, "netmask", tmp), "255.255.255.255");
		tcapi_set(VPNC_NODE, "ipaddr_x", value);
		tcapi_set(VPNC_NODE, "netmask_x", "255.255.255.255");
	}

	if ((value = getenv("IPREMOTE"))) {
		//nvram_set(strcat_r(prefix, "gateway", tmp), value);
		tcapi_set(VPNC_NODE, "gateway_x", value);
	}

	strcpy(buf, "");
	if ((value = getenv("DNS1")))
		sprintf(buf, "%s", value);
	if ((value = getenv("DNS2")))
		sprintf(buf + strlen(buf), "%s%s", strlen(buf) ? " " : "", value);

	/* empty DNS means they either were not requested or peer refused to send them.
	 * lift up underlying xdns value instead, keeping "dns" filled */
	//if (strlen(buf) == 0)
	//	sprintf(buf, "%s", nvram_safe_get(strcat_r(prefix, "xdns", tmp)));

	//nvram_set(strcat_r(prefix, "dns", tmp), buf);
	tcapi_set(VPNC_NODE, "dns_x", buf);

	vpnc_up(vpnc_ifname);

	_dprintf("%s:: done\n", __FUNCTION__);
	return 0;
}

void vpnc_del_firewall_rule(char *vpnc_ifname)
{

	//char tmp[100], prefix[] = "vpnc_", wan_prefix[] = "wanXXXXXXXXXX_";
	//char *vpnc_ifname = nvram_safe_get(strcat_r(prefix, "pppoe_ifname", tmp));
	//char *wan_proto = NULL;
	//char lan_if[IFNAMSIZ+1];

	//strcpy(lan_if, nvram_safe_get("lan_ifname"));
#if 0
	snprintf(wan_prefix, sizeof(wan_prefix), "wan%d_", wan_primary_ifunit());
	wan_proto = nvram_safe_get(strcat_r(wan_prefix, "proto", tmp));
	if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static"))
		//eval("iptables", "-D", "FORWARD", "-p", "tcp", "--syn", "-j", "TCPMSS", "--clamp-mss-to-pmtu");
		eval("iptables", "-D", "FORWARD", "-p", "tcp", "--tcp-flags", "SYN,RST", "SYN", "-j", "TCPMSS", "--clamp-mss-to-pmtu");
#ifdef RTCONFIG_BCMARM
	else
		eval("iptables", "-t", "mangle", "-D", "FORWARD", "-p", "tcp", 
			"-m", "state", "--state", "NEW","-j", "MARK", "--set-mark", "0x01");
#endif

	eval("iptables", "-D", "FORWARD", "-o", vpnc_ifname, "!", "-i", lan_if, "-j", "DROP");
	eval("iptables", "-t", "nat", "-D", "PREROUTING", "-d", 
		nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)), "-j", "VSERVER");
	eval("iptables", "-t", "nat", "-D", "POSTROUTING", "-o", 
		vpnc_ifname, "!", "-s", nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)), "-j", "MASQUERADE");
#endif
	eval("iptables", "-D", "FORWARD", "-o", vpnc_ifname, "-p", "tcp", "--tcp-flags", "SYN,RST", "SYN", "-j", "TCPMSS", "--clamp-mss-to-pmtu");
	eval("iptables", "-D", "FORWARD", "-i", vpnc_ifname, "-j", "FORWARD_WAN");
	eval("iptables", "-t", "nat", "-D", "PREROUTING", "-i", vpnc_ifname,"-j", "VSERVER");
	eval("iptables", "-t", "nat", "-D", "POSTROUTING", "-o", vpnc_ifname, "-j", "MASQUERADE");
}

void
vpnc_down(char *vpnc_ifname)
{
	char tmp[100], wan_prefix[] = "WanXXXXXXXXXX_";
	char wan_ifname[16], wan_proto[16], wan_gateway[16];
	int wan_pvcunit = wan_primary_ifunit();

	snprintf(wan_prefix, sizeof(wan_prefix), "wan%d_", wan_pvcunit);
	snprintf(wan_proto, sizeof(wan_proto), "%s", nvram_safe_get(strcat_r(wan_prefix, "proto", tmp)));

	if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static"))
		snprintf(wan_ifname, sizeof(wan_ifname), "%s", nvram_safe_get(strcat_r(wan_prefix, "ifname", tmp)));
	else
		snprintf(wan_ifname, sizeof(wan_ifname), "%s", nvram_safe_get(strcat_r(wan_prefix, "pppoe_ifname", tmp)));

#if 0
	/* Reset default gateway route */
	if (!strcmp(wan_proto, "dhcp") || !strcmp(wan_proto, "static")) {
		route_del(wan_ifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
		route_add(wan_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
	}
	else if (!strcmp(wan_proto, "pppoe") || !strcmp(wan_proto, "pptp") || !strcmp(wan_proto, "l2tp"))
	{
		char *wan_xgateway = nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp));

		route_del(wan_ifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");
		route_add(wan_ifname, 0, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0");

		if (strlen(wan_xgateway) > 0 && strcmp(wan_xgateway, "0.0.0.0")) {
			char *wan_xifname = nvram_safe_get(strcat_r(wan_prefix, "ifname", tmp));

			route_del(wan_xifname, 3, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp)), "0.0.0.0");
			route_add(wan_xifname, 2, "0.0.0.0", nvram_safe_get(strcat_r(wan_prefix, "xgateway", tmp)), "0.0.0.0");	
		}

		/* Delete route to pptp/l2tp's server */
		if (nvram_get_int(strcat_r(prefix, "dut_disc", tmp)) && strcmp(wan_proto, "pppoe"))
			route_del(wan_ifname, 0, nvram_safe_get(strcat_r(wan_prefix, "gateway", tmp)), "0.0.0.0", "255.255.255.255");
	}
#endif
	/* Reset default gateway route */
	tcapi_get(WANDUCK_DATA, strcat_r(wan_prefix, "gateway", tmp), wan_gateway);
	eval("route", "del", "default", "gw", wan_gateway, "metric", "1", "dev", wan_ifname);
	eval("route", "add", "default", "gw", wan_gateway, "dev", wan_ifname);

	/* Delete firewall rules for VPN client */
	vpnc_del_firewall_rule(vpnc_ifname);
}

/*
 * Called when link goes down
 */
int
vpnc_ipdown_main(int argc, char **argv)
{
	char *vpnc_ifname = safe_getenv("IFNAME");
	char *vpnc_linkname = safe_getenv("LINKNAME");
	char tmp[100];//, prefix[] = "vpnc_";
	int unit;

	_dprintf("%s():: %s\n", __FUNCTION__, argv[0]);

	/* Get unit from LINKNAME: vpn[UNIT] */
	if ((unit = vpnc_ppp_linkunit(vpnc_linkname)) < 0)
		return 0;

	_dprintf("%s: unit=%d ifname=%s\n", __FUNCTION__, unit, vpnc_ifname);

	/* override wan_state to get real reason */
	update_vpnc_state(WAN_STATE_STOPPED, vpnc_pppstatus());

	vpnc_down(vpnc_ifname);

	start_dnsmasq();

	unlink(strcat_r("/tmp/ppp/link.", vpnc_ifname, tmp));

	_dprintf("%s:: done\n", __FUNCTION__);

	return 0;
}

/*
 * Called when interface comes up
 */
int
vpnc_ippreup_main(int argc, char **argv)
{
	char *vpnc_ifname = safe_getenv("IFNAME");
	char *vpnc_linkname = safe_getenv("LINKNAME");
	//char tmp[100], prefix[] = "vpnc_";
	int unit;

	_dprintf("%s():: %s\n", __FUNCTION__, argv[0]);

	/* Get unit from LINKNAME: vpn[UNIT] */
	if ((unit = vpnc_ppp_linkunit(vpnc_linkname)) < 0)
		return 0;

	_dprintf("%s: unit=%d ifname=%s\n", __FUNCTION__, unit, vpnc_ifname);
#if 0
	/* Set vpnc_pppoe_ifname to real interface name */
	nvram_set(strcat_r(prefix, "pppoe_ifname", tmp), vpnc_ifname);
#endif
	_dprintf("%s:: done\n", __FUNCTION__);
	return 0;
}

/*
 * Called when link closing with auth fail
 */
int
vpnc_authfail_main(int argc, char **argv)
{
	char *vpnc_ifname = safe_getenv("IFNAME");
	char *vpnc_linkname = safe_getenv("LINKNAME");
	//char prefix[] = "vpnc_";
	int unit;

	_dprintf("%s():: %s\n", __FUNCTION__, argv[0]);

	/* Get unit from LINKNAME: ppp[UNIT] */
	if ((unit = vpnc_ppp_linkunit(vpnc_linkname)) < 0)
		return 0;

	_dprintf("%s: unit=%d ifname=%s\n", __FUNCTION__, unit, vpnc_ifname);

	/* override vpnc_state */
	update_vpnc_state(WAN_STATE_STOPPED, WAN_STOPPED_REASON_PPP_AUTH_FAIL);

	_dprintf("%s:: done\n", __FUNCTION__);
	return 0;
}

#ifdef TCSUPPORT_IPV6
int vpnc_ip6up_main(int argc, char **argv)
{
	char *vpnc_ifname = safe_getenv("IFNAME");
#if 0
	if (!wan_ifname || strlen(wan_ifname) <= 0)
		return 0;

        switch (get_ipv6_service()) {
                case IPV6_NATIVE:
                case IPV6_NATIVE_DHCP:
			wait_ppp_count = 10;
			while ((!is_intf_up(wan_ifname) || !getifaddr(wan_ifname, AF_INET6, 0))
				&& (wait_ppp_count-- > 0))
				sleep(1);
			break;
		default:
			wait_ppp_count = 0;
			break;
	}

	if (wait_ppp_count != -2)
	{
		wan6_up(wan_ifname);	
		start_firewall(0, 0);
	}
#endif

	return 0;
}

int vpnc_ip6down_main(int argc, char **argv)
{
	char *vpnc_ifname = safe_getenv("IFNAME");
#if 0
	wait_ppp_count = -2;

	wan6_down(wan_ifname);
#endif
	return 0;
}
#endif  // IPV6
