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
 * Network services
 *
 * Copyright 2004, ASUSTeK Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 */

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include "libtcapi.h"
#include "tcapi.h"
#include "rc.h"
#include "network_utility.h"

#ifdef RTCONFIG_USB_MODEM
#include "usb_info.h"
#endif

#define WAN0_ROUTE_TABLE 100
#define WAN1_ROUTE_TABLE 200

#define TEMP_ROUTE_FILE "/tmp/route_main"

int copy_routes(int table){
	char cmd[2048];
	char *route_buf, *follow_info, line[1024];
	int len;

	if(table <= 0){
		snprintf(cmd, 2048, "ip route flush table %d", WAN0_ROUTE_TABLE);
		system(cmd);

		snprintf(cmd, 2048, "ip route flush table %d", WAN1_ROUTE_TABLE);
		system(cmd);

		return 0;
	}

	snprintf(cmd, 2048, "ip route list table main > %s", TEMP_ROUTE_FILE);
	system(cmd);

	route_buf = read_whole_file(TEMP_ROUTE_FILE);
	if(route_buf == NULL || strlen(route_buf) <= 0)
		return -1;

	follow_info = route_buf;
	while(get_line_from_buffer(follow_info, line, 1024) != NULL && strncmp(line, "default", 7) != 0){
		follow_info += strlen(line);

		len = strlen(line);
		line[len-2] = 0;

		snprintf(cmd, 2048, "ip route add table %d %s", table, line);
		system(cmd);
	}

	free(route_buf);

	return 0;
}

/*
 * the priority of routing rules:
 * pref 100: user's routes.
 * pref 200: from wan's ip, from wan's DNS.
 * pref 300: ISP's routes.
 * pref 400: to wan's gateway, to wan's DNS.
 */
int add_multi_routes(void)
{
	int unit;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	char wan_proto[32], wan_if[32], wan_ip[32], wan_gate[32];
	char cmd[2048];
#ifdef RTCONFIG_DUALWAN
	int gate_num = 0, wan_weight, table;
	char cmd2[2048], *ptr;
	char wan_dns[1024];
	char wan_multi_if[WAN_UNIT_MAX][32], wan_multi_gate[WAN_UNIT_MAX][32];
	char word[64], *next;
	char wan_isp[32];
	char *nv, *nvp, *b;
#endif
#if !ASUSWRT	//make sure add wan_primary first
	int i;
	int wan_primary = wan_primary_ifunit();
	int wan_secondary = wan_secondary_ifunit();
	int wan_pool[] = {wan_primary, wan_secondary, -1};	//wan_secondary=-1 if dualwan disable
#endif

	// clean the rules of routing table and re-build them then.
	system("ip rule flush");
	system("ip rule add from all lookup main pref 32766");
	system("ip rule add from all lookup default pref 32767");

	// clean multi route tables and re-build them then.
	copy_routes(0);

#ifdef RTCONFIG_DUALWAN
	if(nvram_match("wans_mode", "lb")){
		gate_num = 0;
#if ASUSWRT
		for(unit = WAN_UNIT_FIRST; unit < WAN_UNIT_MAX; ++unit) // Multipath
#else
		for(i = 0; unit = wan_pool[i], unit != -1; i++)
#endif
		{
			// when wan_down().
			if(!is_wan_connect(unit))
				continue;

			snprintf(prefix, sizeof(prefix), "wan%d_", unit);
			strncpy(wan_ip, nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)), 32);
			strncpy(wan_gate, nvram_safe_get(strcat_r(prefix, "gateway", tmp)), 32);

			if(strlen(wan_gate) <= 0 || !strcmp(wan_gate, "0.0.0.0"))
				continue;

			if(strlen(wan_ip) <= 0 || !strcmp(wan_ip, "0.0.0.0"))
				continue;

			++gate_num;
		}
	}

	memset(wan_multi_if, 0, sizeof(char)*WAN_UNIT_MAX*32);
	memset(wan_multi_gate, 0, sizeof(char)*WAN_UNIT_MAX*32);
#endif

#if ASUSWRT
	for(unit = WAN_UNIT_FIRST; unit < WAN_UNIT_MAX; ++unit) // Multipath
#else
	for(i = 0; unit = wan_pool[i], unit != -1; i++)
#endif
	{
		if(unit != wan_primary
#ifdef RTCONFIG_DUALWAN
				&& !nvram_match("wans_mode", "lb")
#endif
				)
			continue;

		// when wan_down().
		if(!is_wan_connect(unit))
			continue;

		snprintf(prefix, sizeof(prefix), "wan%d_", unit);
		strncpy(wan_proto, nvram_safe_get(strcat_r(prefix, "proto", tmp)), 32);
		strncpy(wan_if, get_wan_ifname(unit), 32);
		strncpy(wan_ip, nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)), 32);
		strncpy(wan_gate, nvram_safe_get(strcat_r(prefix, "gateway", tmp)), 32);

		if(strlen(wan_gate) <= 0 || !strcmp(wan_gate, "0.0.0.0"))
			continue;

#ifdef RTCONFIG_DUALWAN
		if(nvram_match("wans_mode", "lb") && gate_num > 1){
			if(strlen(wan_ip) <= 0 || !strcmp(wan_ip, "0.0.0.0"))
				continue;

			if(unit == wan_secondary)
				table = WAN1_ROUTE_TABLE;
			else if(unit == wan_primary)
				table = WAN0_ROUTE_TABLE;
			else
				continue;

			// set the rules of wan[X]'s ip and gateway for multi routing tables.
			snprintf(cmd2, 2048, "ip rule del pref 200 from %s table %d", wan_ip, table);
			system(cmd2);

			snprintf(cmd2, 2048, "ip rule add pref 200 from %s table %d", wan_ip, table);
			system(cmd2);

			snprintf(cmd2, 2048, "ip rule del pref 400 to %s table %d", wan_gate, table);
			system(cmd2);

			snprintf(cmd2, 2048, "ip rule add pref 400 to %s table %d", wan_gate, table);
			system(cmd2);

			// set the routes for multi routing tables.
			copy_routes(table);

			/*
			if(!strcmp(wan_proto, "pptp") || !strcmp(wan_proto, "l2tp")){
				snprintf(cmd2, 2048, "ip route add %s dev %s table %d", wan_gate, wan_if, table);
				system(cmd2);
			}
			*/

			snprintf(cmd2, 2048, "ip route replace default via %s dev %s table %d", wan_gate, wan_if, table);
			system(cmd2);

			/*
			if(!strcmp(wan_proto, "pptp") || !strcmp(wan_proto, "l2tp")){
				snprintf(cmd2, 2048, "ip route del %s dev %s table %d", wan_gate, wan_if, table);
				system(cmd2);
			}
			*/

			strcpy(wan_multi_if[unit], wan_if);
			strcpy(wan_multi_gate[unit], wan_gate);

			// set the static routing rules.
			if(nvram_match("wans_routing_enable", "1")){
				char *rfrom, *rto, *rtable_str;
				int rtable;

				nvp = nv = strdup(nvram_safe_get("wans_routing_rulelist"));
				while(nv && (b = strsep(&nvp, "<")) != NULL){
					if((vstrsep(b, ">", &rfrom, &rto, &rtable_str) != 3))
						continue;

					if(!strcmp(rfrom, "all") && !strcmp(rfrom, rto))
						continue;

					rtable = atoi(rtable_str);

#if ASUSWRT
					if(rtable == WAN_UNIT_FIRST || rtable == WAN0_ROUTE_TABLE)
						rtable = WAN0_ROUTE_TABLE;
					else if(rtable == WAN_UNIT_SECOND || rtable == WAN1_ROUTE_TABLE)
						rtable = WAN1_ROUTE_TABLE;
					else // incorrect table.
						continue;
#else
					if(rtable == 0 || rtable == WAN0_ROUTE_TABLE)
						rtable = WAN0_ROUTE_TABLE;
					else if(rtable == 1 || rtable == WAN1_ROUTE_TABLE)
						rtable = WAN1_ROUTE_TABLE;
					else // incorrect table.
						continue;
#endif

					if(rtable == table){
						snprintf(cmd2, 2048, "ip rule del pref 100 from %s to %s table %d", rfrom, rto, rtable);
						system(cmd2);

						snprintf(cmd2, 2048, "ip rule add pref 100 from %s to %s table %d", rfrom, rto, rtable);
						system(cmd2);
					}
	 			}
				free(nv);
			}

			// ISP's routing rules.
			if(nvram_match(strcat_r(prefix, "routing_isp_enable", tmp), "1")){
				strncpy(wan_isp, nvram_safe_get(strcat_r(prefix, "routing_isp", tmp)), 32);

				FILE *fp;
				char conf_name[64], line[1024];

				snprintf(conf_name, 64, "/rom/etc/static_routes/%s.conf", wan_isp);

				if((fp = fopen(conf_name, "r")) != NULL){
					while(fgets(line, sizeof(line), fp)){
						char *token = strtok(line, "\n");

						snprintf(cmd2, 2048, "ip rule del pref 300 %s table %d", token, table);
						system(cmd2);

						snprintf(cmd2, 2048, "ip rule add pref 300 %s table %d", token, table);
						system(cmd2);
					}
					fclose(fp);
				}
			}
		}
		else
#endif
		{
			/*
			if(!strcmp(wan_proto, "pptp") || !strcmp(wan_proto, "l2tp")){
				snprintf(cmd, 2048, "ip route add %s dev %s", wan_gate, wan_if);
				system(cmd);
			}
			*/

			// set the default gateway.
			snprintf(cmd, 2048, "ip route replace default via %s dev %s", wan_gate, wan_if);
			system(cmd);

			/*
			if(!strcmp(wan_proto, "pptp") || !strcmp(wan_proto, "l2tp")){
				snprintf(cmd, 2048, "ip route del %s dev %s", wan_gate, wan_if);
				system(cmd);
			}
			*/
		}

#ifdef RTCONFIG_DUALWAN
		if(!nvram_match("wans_mode", "lb") || gate_num <= 1)
			break;
#endif
	}

#ifdef RTCONFIG_DUALWAN
	// set the multi default gateway.
	if(nvram_match("wans_mode", "lb") && gate_num > 1){
		memset(cmd, 0, 2048);
#if ASUSWRT
		for(unit = WAN_UNIT_FIRST; unit < WAN_UNIT_MAX; ++unit)
#else
		for(i = 0; unit = wan_pool[i], unit != -1; i++)
#endif
		{
			if(unit == wan_secondary)
				table = WAN1_ROUTE_TABLE;
			else if(unit == wan_primary)
				table = WAN0_ROUTE_TABLE;
			else
				continue;

			snprintf(prefix, sizeof(prefix), "wan%d_", unit);

			// move the gateway via VPN+DHCP from the main routing table to the correct one.
			strcpy(wan_gate, nvram_safe_get(strcat_r(prefix, "xgateway", tmp)));
			if(strlen(wan_gate) > 0 && strcmp(wan_gate, "0.0.0.0") && strcmp(wan_gate, wan_multi_gate[unit])){
				snprintf(cmd2, 2048, "ip route del default via %s dev %s", wan_gate, get_wanx_ifname(unit));
				system(cmd2);

				snprintf(cmd2, 2048, "ip route add default via %s dev %s table %d metric 1", wan_gate, get_wanx_ifname(unit), table);
				system(cmd2);
			}

			// set the routing rules of DNS via VPN+DHCP.
			strncpy(wan_dns, nvram_safe_get(strcat_r(prefix, "xdns", tmp)), 32);
			if(strlen(wan_dns) > 0){
				// set the rules for the DNS servers.
				foreach(word, wan_dns, next) {
					snprintf(cmd2, 2048, "ip rule del pref 200 from %s table %d", word, table);
					system(cmd2);
					snprintf(cmd2, 2048, "ip rule add pref 200 from %s table %d", word, table);
					system(cmd2);

					snprintf(cmd2, 2048, "ip rule del pref 400 to %s table %d", word, table);
					system(cmd2);
					snprintf(cmd2, 2048, "ip rule add pref 400 to %s table %d", word, table);
					system(cmd2);
				}
			}

			// set the routing rules of DNS.
			strncpy(wan_dns, nvram_safe_get(strcat_r(prefix, "dns", tmp)), 32);
			if(strlen(wan_dns) > 0){
				// set the rules for the DNS servers.
				foreach(word, wan_dns, next) {
					snprintf(cmd2, 2048, "ip rule del pref 200 from %s table %d", word, table);
					system(cmd2);
					snprintf(cmd2, 2048, "ip rule add pref 200 from %s table %d", word, table);
					system(cmd2);

					snprintf(cmd2, 2048, "ip rule del pref 400 to %s table %d", word, table);
					system(cmd2);
					snprintf(cmd2, 2048, "ip rule add pref 400 to %s table %d", word, table);
					system(cmd2);
				}
			}

			// set the default gateways with weights in the main routing table.
			nvp = nv = strdup(nvram_safe_get("wans_lb_ratio"));
			int i = 0;
			b = NULL;
			while(nv && (b = strsep(&nvp, ":")) != NULL){
#if ASUSWRT
				if(i == unit)
					break;

#else
				if(i == 0 && unit == wan_primary)
					break;
				else if(i == 1 && unit == wan_secondary)
					break;
#endif
				++i;
			}

			if(!b)
				continue;

			wan_weight = atoi(b);
			if(wan_weight > 0 && strlen(wan_multi_gate[unit]) > 0){
				if(strlen(cmd) == 0)
					strcpy(cmd, "ip route replace default equalize");

				ptr = cmd+strlen(cmd);
				sprintf(ptr, " nexthop via %s dev %s weight %d", wan_multi_gate[unit], wan_multi_if[unit], wan_weight);
			}

			if(nv)
				free(nv);
		}

		if(strlen(cmd) > 0){
			/*
			for(unit = WAN_UNIT_FIRST; unit < WAN_UNIT_MAX; ++unit){
				snprintf(prefix, sizeof(prefix), "wan%d_", unit);
				strncpy(wan_proto, nvram_safe_get(strcat_r(prefix, "proto", tmp)), 32);
				strncpy(wan_if, get_wan_ifname(unit), 32);
				strncpy(wan_gate, nvram_safe_get(strcat_r(prefix, "gateway", tmp)), 32);

				if(!strcmp(wan_proto, "pptp") || !strcmp(wan_proto, "l2tp")){
					snprintf(cmd2, 2048, "ip route add %s dev %s", wan_gate, wan_if);
					system(cmd2);
				}
			}
			*/

			system(cmd);

			/*
			for(unit = WAN_UNIT_FIRST; unit < WAN_UNIT_MAX; ++unit){
				snprintf(prefix, sizeof(prefix), "wan%d_", unit);
				strncpy(wan_proto, nvram_safe_get(strcat_r(prefix, "proto", tmp)), 32);
				strncpy(wan_if, get_wan_ifname(unit), 32);
				strncpy(wan_gate, nvram_safe_get(strcat_r(prefix, "gateway", tmp)), 32);

				if(!strcmp(wan_proto, "pptp") || !strcmp(wan_proto, "l2tp")){
					snprintf(cmd2, 2048, "ip route del %s dev %s", wan_gate, wan_if);
					system(cmd2);
				}
			}
			*/
		}
	}
#endif
	system("ip route flush cache");

	return 0;
}

void stop_igmpproxy()
{
	if (pids("udpxy"))
		killall_tk("udpxy");
	if (pids("igmpproxy"))
		killall_tk("igmpproxy");
}

int
wan_prefix(char *ifname, char *prefix)
{
	int unit;

	if ((unit = wan_ifunit(ifname)) < 0)
		return -1;

	sprintf(prefix, "wan%d_", unit);

	return unit;
}

static const char *qosfn = "/tmp/qos";
void stop_iQos(void)
{
	char cmdbuf[128] = {0};

	sprintf(cmdbuf, "%s stop", qosfn);
	system(cmdbuf);
}

void update_wan_state(char *prefix, int state, int reason)
{
	char attr[32], value[4];

	_dprintf("%s(%s, %d, %d)\n", __FUNCTION__, prefix, state, reason);

	// nvram_set_int(strcat_r(prefix, "state_t", tmp), state);
	strcat_r(prefix, "state_t", attr);
	sprintf(value, "%d", state);
	tcapi_set("Wanduck_Common", attr, value);
	// nvram_set_int(strcat_r(prefix, "sbstate_t", tmp), 0);
	strcat_r(prefix, "sbstate_t", attr);
	tcapi_set("Wanduck_Common", attr, "0");
	// nvram_set_int(strcat_r(prefix, "auxstate_t", tmp), 0);
	strcat_r(prefix, "auxstate_t", attr);
	tcapi_set("Wanduck_Common", attr, "0");

	if (state == WAN_STATE_INITIALIZING)
	{
		/* reset wanX_* variables */
	}
	else if (state == WAN_STATE_STOPPED) {
		// Save Stopped Reason
		// keep ip info if it is stopped from connected
		// nvram_set_int(strcat_r(prefix, "sbstate_t", tmp), reason);
		strcat_r(prefix, "sbstate_t", attr);
		sprintf(value, "%d", reason);
		tcapi_set("Wanduck_Common", attr, value);
	}
	else if(state == WAN_STATE_STOPPING){
		unlink("/tmp/wanstatus.log");
	}
}

#ifdef RTCONFIG_USB_MODEM
void start_wan_if(int unit)
{
#ifdef RTCONFIG_DUALWAN
	int wan_type;
#endif
	char wan_ifname[32] = {0};
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	int s;
	struct ifreq ifr;
	pid_t pid;
	char usb_node[32] = {0}, port_path[8] = {0};
	char nvram_name[32] = {0};
	int i = 0;
#ifdef RTCONFIG_USB_MODEM
	char modem_type[32];
#ifdef RTCONFIG_USB_BECEEM
	unsigned int uvid, upid;
#endif
#endif
	char node[64] = {0}, attr[64] = {0}, value[64] = {0};
	int stop_conn_3g = 0;

	TRACE_PT("unit=%d.\n", unit);
	snprintf(prefix, sizeof(prefix), "wan%d_", unit);

	stop_wan_if(unit);

	if(nvram_match("modem_enable", "0"))
	{
		cprintf("USB Modem is disabled!!\n");
		return;
	}

	system("/bin/ln -sf /sbin/rc /etc/ppp/ip-up");
	system("/bin/ln -sf /sbin/rc /etc/ppp/ip-down");
	system("/bin/ln -sf /sbin/rc /etc/ppp/auth-fail");

	sprintf(node, "Wan_PVC%d", unit);
	sprintf(attr, "Active");
	tcapi_get(node, attr, value);

	if(strcmp(value, "No") == 0) {
		update_wan_state(prefix, WAN_STATE_DISABLED, 0);
		return;
	}

	update_wan_state(prefix, WAN_STATE_INITIALIZING, 0);

#ifdef RTCONFIG_DUALWAN
	wan_type = get_dualwan_by_unit(unit);
#endif

#ifdef RTCONFIG_USB_MODEM
	if (dualwan_unit__usbif(unit)) {
		if(is_usb_modem_ready() != 1){ // find_modem_type.sh would be run by is_usb_modem_ready().
			TRACE_PT("No USB Modem!\n");
			return;
		}

		TRACE_PT("3g begin.\n");
#ifdef RTCONFIG_USB_MODEM_PIN
		if(nvram_match("g3err_pin", "1")){
			TRACE_PT("3g end: PIN error previously!\n");
			update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_PINCODE_ERR);
			return;
		}
#endif

#if ASUSWRT
		update_wan_state(prefix, WAN_STATE_CONNECTING, 0);
#else
		update_wan_state(prefix, WAN_STATE_USBMODEM_CONNECTING, 0);
#endif

		snprintf(modem_type, 32, "%s", nvram_safe_get("usb_modem_act_type"));

		if(!strcmp(modem_type, "tty") || !strcmp(modem_type, "qmi") || !strcmp(modem_type, "mbim") || !strcmp(modem_type, "gobi")){
			if(!strcmp(nvram_safe_get("usb_modem_act_int"), "")){
				if(!strcmp(modem_type, "qmi")){	// e.q. Huawei E398.
					TRACE_PT("Sleep 3 seconds to wait modem nodes.\n");
					sleep(3);
				}
				else{
					TRACE_PT("Sleep 2 seconds to wait modem nodes.\n");
					sleep(2);
				}
			}

			// find the modem node at every start_wan_if() to avoid the incorrct one sometimes.
#if 0
			eval("find_modem_node.sh");
#else
			system("find_modem_node.sh");
#endif
		}

		if(nvram_get_int("usb_modem_act_reset") == 1){
			// need to execute find_modem_xxx.sh again.
			TRACE_PT("3g end: Reseting the modem...\n");
			return;
		}

		/* Stop pppd */
		stop_pppd(unit);

		char dhcp_pid_file[1024] = {0};
		FILE *fp = NULL;

		snprintf(dhcp_pid_file, 1024, "/var/run/udhcpc%d.pid", unit);

		kill_pidfile_s(dhcp_pid_file, SIGUSR2);
		kill_pidfile_s(dhcp_pid_file, SIGTERM);

		sprintf(wan_ifname, "%s", nvram_safe_get(strcat_r(prefix, "ifname", tmp)));
		cprintf("%s: wan_ifname=[%s]\n", __FUNCTION__, wan_ifname);

		/*
			"stop_conn_3g" is for testing/debugging.
		*/
		tcapi_get("USBModem_Entry", "stop_conn_3g", value);
		stop_conn_3g = atoi(value);
		if(stop_conn_3g == 1){
			write_3g_ppp_conf();
		}
		else{
			char *ptr;
			int sim_state;

			system("modem_enable.sh");

#ifdef RTCONFIG_USB_MODEM_PIN
			if(nvram_match("g3err_pin", "1")){
				TRACE_PT("3g end: PIN error!\n");
				update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_PINCODE_ERR);
				return;
			}
#endif

			ptr = nvram_safe_get("usb_modem_act_sim");
			if(strlen(ptr) > 0){
				sim_state = atoi(ptr);
				if(sim_state == 2 || sim_state == 3){
					TRACE_PT("3g end: Need to input PIN or PUK.\n");
					update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_PINCODE_ERR);
					return;
				}
				else if(sim_state != 1){
					TRACE_PT("3g end: SIM isn't ready.\n");
					return;
				}
			}
		}

		if((!strcmp(modem_type, "tty") || !strcmp(modem_type, "mbim"))
				&& write_3g_ppp_conf()
				&& (fp = fopen(PPP_CONF_FOR_3G, "r")) != NULL){
			fclose(fp);

			// run as ppp proto.
			nvram_set(strcat_r(prefix, "proto", tmp), "pppoe");
			snprintf(wan_ifname, sizeof(wan_ifname), PPP_IF, unit);
			nvram_set(strcat_r(prefix, "pppoe_ifname", tmp), wan_ifname);
			tcapi_set("Wan_PVC11", "ISP", "2" );

#ifndef RTCONFIG_DUALWAN
#if 1 /* TODO: tmporary change! remove after WEB UI support */
			nvram_set(strcat_r(prefix, "dhcpenable_x", tmp), "1");
			nvram_set(strcat_r(prefix, "vpndhcp", tmp), "0");
#else /* TODO: tmporary change! remove after WEB UI support */
			nvram_set(strcat_r(prefix, "dhcpenable_x", tmp), "2");
#endif
			nvram_set(strcat_r(prefix, "dnsenable_x", tmp), "1");
#endif

			char *pppd_argv[] = { "/usr/bin/pppd", "call", "3g", "unit", "", NULL};
			char unit_argv[] = "xx";
			sprintf(unit_argv, "%d", unit);
			pppd_argv[4] = unit_argv;

			if(stop_conn_3g != 1)
				_eval(pppd_argv, NULL, 0, NULL);
			else
				TRACE_PT("stop_conn_3g was set.\n");
		}
		// RNDIS, QMI interface: usbX, Beceem interface: usbbcm -> ethX, gct(mad)wimax: wimaxX.
		else{
			sprintf(wan_ifname, "%s", nvram_safe_get(strcat_r(prefix, "ifname", tmp)));

			if(strlen(wan_ifname) <= 0){
#ifdef RTCONFIG_USB_BECEEM
				snprintf(usb_node, 32, "%s", nvram_safe_get("usb_modem_act_path"));
				if(strlen(usb_node) <= 0)
					return;

				if(get_path_by_node(usb_node, port_path, 8) != NULL){
					snprintf(nvram_name, 32, "usb_path%s", port_path);
					TRACE_PT("RNDIS/Beceem: start_wan_if.\n");

					if(!strcmp(nvram_safe_get(nvram_name), "modem")){
						snprintf(nvram_name, 32, "usb_path%s_vid", port_path);
						uvid = strtoul(nvram_safe_get(nvram_name), NULL, 16);
						snprintf(nvram_name, 32, "usb_path%s_pid", port_path);
						upid = strtoul(nvram_safe_get(nvram_name), NULL, 16);

						if(is_samsung_dongle(1, uvid, upid)){
							modprobe("tun");
							sleep(1);

							xstart("madwimax");
						}
						else if(is_gct_dongle(1, uvid, upid)){
							modprobe("tun");
							sleep(1);

							write_gct_conf();

							xstart("gctwimax", "-C", WIMAX_CONF);
						}
					}
				}
#endif

				return;
			}

			strncpy(ifr.ifr_name, wan_ifname, IFNAMSIZ);
			if((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0){
				TRACE_PT("Couldn't open the socket!\n");
				update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_SYSTEM_ERR);
				return;
			}

			if(ioctl(s, SIOCGIFFLAGS, &ifr)){
				close(s);
				TRACE_PT("Couldn't read the flags of %s!\n", wan_ifname);
				update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_SYSTEM_ERR);
				return;
			}

			i = 0;
			while(!(ifr.ifr_flags & IFF_UP) && (i++ < 3)){
				ifconfig(wan_ifname, IFUP, NULL, NULL);
				TRACE_PT("%s: wait %s be up, %d second...!\n", __FUNCTION__, wan_ifname, i);
				sleep(1);

				if(ioctl(s, SIOCGIFFLAGS, &ifr)){
					close(s);
					TRACE_PT("Couldn't read the flags of %s(%d)!\n", wan_ifname, i);
					update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_SYSTEM_ERR);
					return;
				}
			}
			close(s);

			if(!(ifr.ifr_flags & IFF_UP)){
				TRACE_PT("Interface %s couldn't be up!\n", wan_ifname);
				update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_SYSTEM_ERR);
				return;
			}

			// run as dhcp proto.
			nvram_set(strcat_r(prefix, "proto", tmp), "dhcp");
			nvram_set(strcat_r(prefix, "dhcpenable_x", tmp), "1");
			nvram_set(strcat_r(prefix, "dnsenable_x", tmp), "1");

			// Android phone, RNDIS, QMI interface.
			if(!strncmp(wan_ifname, "usb", 3)){
				if(nvram_get_int("stop_conn_3g") != 1)
					start_udhcpc(wan_ifname, unit, &pid);
				else
					TRACE_PT("stop_conn_3g was set.\n");
			}
			// Beceem dongle, ASIX USB to RJ45 converter, Huawei E353.
			else if(!strncmp(wan_ifname, "eth", 3)){
#ifdef RTCONFIG_USB_BECEEM
				write_beceem_conf(wan_ifname);
#endif

				if(stop_conn_3g != 1){
					snprintf(usb_node, 32, "%s", nvram_safe_get("usb_modem_act_path"));
					if(strlen(usb_node) <= 0)
						return;

					if(get_path_by_node(usb_node, port_path, 8) == NULL)
						return;

					snprintf(nvram_name, 32, "usb_path%s_act", port_path);

					if(!strcmp(nvram_safe_get(nvram_name), wan_ifname))
						start_udhcpc(wan_ifname, unit, &pid);

#ifdef RTCONFIG_USB_BECEEM
					if(strlen(nvram_safe_get(nvram_name)) <= 0){
						char buf[128];

						snprintf(buf, 128, "wimaxd -c %s", WIMAX_CONF);
						TRACE_PT("%s: cmd=%s.\n", __FUNCTION__, buf);
						system(buf);
						sleep(3);

						TRACE_PT("%s: cmd=wimaxc search.\n", __FUNCTION__);
						system("wimaxc search");
						TRACE_PT("%s: sleep 10 seconds.\n", __FUNCTION__);
						sleep(10);

						TRACE_PT("%s: cmd=wimaxc connect.\n", __FUNCTION__);
						system("wimaxc connect");
					}
#endif
				}
				else
					TRACE_PT("stop_conn_3g was set.\n");
			}
#ifdef RTCONFIG_USB_BECEEM
			else if(!strncmp(wan_ifname, "wimax", 5)){
				if(nvram_get_int("stop_conn_3g") != 1)
					start_udhcpc(wan_ifname, unit, &pid);
				else
					TRACE_PT("stop_conn_3g was set.\n");
			}
#endif
		}

		TRACE_PT("3g end.\n");
		return;
	}
#endif /* RTCONFIG_USB_MODEM */
}

void
stop_wan_if(int unit)
{
#if defined(RTCONFIG_DSL) && defined(RTCONFIG_DUALWAN)
	int wan_type;
#endif
	char *wan_ifname;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	char *wan_proto, active_proto[32];
	char pid_file[256];
#if defined(RTCONFIG_USB_BECEEM) || defined(RT4GAC55U)
	int i;
#endif
#ifdef RTCONFIG_USB_BECEEM
	unsigned int uvid, upid;
#endif

	snprintf(prefix, sizeof(prefix), "wan%d_", unit);

	update_wan_state(prefix, WAN_STATE_STOPPING, WAN_STOPPED_REASON_NONE);

	/* Backup active wan_proto for later restore, if it have been updated by ui */
	wan_proto = nvram_safe_get(strcat_r(prefix, "proto", tmp));
	strncpy(active_proto, wan_proto, sizeof(active_proto));

	/* Set previous wan_proto as active */
	wan_proto = nvram_safe_get(strcat_r(prefix, "proto_t", tmp));
	if (*wan_proto && strcmp(active_proto, wan_proto) != 0)
	{
		stop_iQos(); // clean all tc rules
		_dprintf("%s %sproto_t=%s\n", __FUNCTION__, prefix, wan_proto);
		nvram_set(strcat_r(prefix, "proto", tmp), wan_proto);
		nvram_unset(strcat_r(prefix, "proto_t", tmp));
	}

	// Handel for each interface
	if(unit == wan_primary_ifunit()){
		killall_tk("stats");
		killall_tk("ntpclient");

#ifdef RTCONFIG_IPV6
		if (nvram_match("ipv6_ifdev", "eth") ||
		    get_ipv6_service() != IPV6_NATIVE_DHCP)
			stop_wan6();
#endif
	}

#ifdef RTCONFIG_VPNC
	/* Stop VPN client */
	stop_vpnc();
#endif

	/* Stop l2tp */
	if (strcmp(wan_proto, "l2tp") == 0) {
		kill_pidfile_tk("/var/run/l2tpd.pid");
		usleep(1000*10000);
	}

	/* Stop pppd */
	stop_pppd(unit);

	snprintf(pid_file, sizeof(pid_file), "/var/run/udhcpc%d.pid", unit);
	kill_pidfile_s(pid_file, SIGUSR2);
	kill_pidfile_s(pid_file, SIGTERM);

	/* Bring down WAN interfaces */
	// Does it have to?
	wan_ifname = nvram_safe_get(strcat_r(prefix, "ifname", tmp));
#ifdef RTCONFIG_USB_MODEM
	if(strncmp(wan_ifname, "/dev/tty", 8))
#endif
	{
		if(strlen(wan_ifname) > 0){
			ifconfig(wan_ifname, IFUP, "0.0.0.0", NULL);
		}
	}

#ifdef RTCONFIG_DSL
#ifdef RTCONFIG_DUALWAN
	wan_type = get_dualwan_by_unit(unit);
	if(wan_type == WANS_DUALWAN_IF_DSL)
#endif
		if (nvram_match("dsl0_proto", "ipoa"))
		{
			stop_ipoa();
		}
#endif

#if defined(RTCONFIG_USB_MODEM)
	if (dualwan_unit__usbif(unit)) {
#if defined(RTCONFIG_USB_BECEEM)
		if(is_usb_modem_ready() == 1){
			if(pids("wimaxd"))
				system("wimaxc disconnect");
		}

		if(pids("wimaxd")){
			system("killall wimaxd");
			system("killall -SIGUSR1 wimaxd");
		}

		uvid = atoi(nvram_safe_get("usb_modem_act_vid"));
		upid = atoi(nvram_safe_get("usb_modem_act_pid"));

		if(is_samsung_dongle(1, uvid, upid)){
			i = 0;
			while(i < 3){
				if(pids("madwimax")){
					killall_tk("madwimax");
					sleep(1);

					++i;
				}
				else
					break;
			}

			modprobe_r("tun");

			nvram_set(strcat_r(prefix, "ifname", tmp), "");
		}
		else if(is_gct_dongle(1, uvid, upid)){
			i = 0;
			while(i < 3){
				if(pids("gctwimax")){
					killall_tk("gctwimax");
					sleep(1);

					++i;
				}
				else
					break;
			}
			unlink(WIMAX_CONF);

			modprobe_r("tun");

			nvram_set(strcat_r(prefix, "ifname", tmp), "");
		}
#endif	/* RTCONFIG_USB_BECEEM */

#if defined(RT4GAC55U)
#if 0
#define SLEEP(s) {int sec = s; while((sec = sleep(sec)) > 0) /* cprintf(" intr(%d)\n", sec)*/ ; }
		for(i = 0; i < 3 && pidof("gobi") > 0; i++)
		{
#define GOBI_PIPE_PATH "/tmp/pipe"
			cprintf("stop gobi %d\n", i);
			f_write(GOBI_PIPE_PATH,  "4\n2\n99\n", 7, FW_APPEND, 0);	//disconnect and terminal gobi process.
			SLEEP(2);
		}
#else
		char *const modem_argv[] = {"modem_stop.sh", NULL};

		_eval(modem_argv, ">>/tmp/usb.log", 0, NULL);
#endif
#endif
	}

	if(dualwan_unit__usbif(unit))
		update_wan_state(prefix, WAN_STATE_INITIALIZING, 0);
	else
#endif // RTCONFIG_USB_MODEM
		update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_NONE);
	
#if 0
	// wait for release finished ?
	if (!g_reboot)
		sleep(2);
#else
	clean_modem_state(0);//Ren
	clean_wanduck_state(unit);//Ren

	sleep(1);
#endif

	/* Restore active wan_proto value */
	_dprintf("%s %sproto=%s\n", __FUNCTION__, prefix, active_proto);
	nvram_set(strcat_r(prefix, "proto", tmp), active_proto);
}
#endif /* RTCONFIG_USB_MODEM */

int do_dns_detect()
{
	char *test_url = "www.google.com www.baidu.com www.yandex.com";
	char word[64], *next;

	foreach(word, test_url, next){
		_dprintf("[%s(%d)] Try %s\n", __FUNCTION__, __LINE__, word);
		if(gethostbyname(word) != NULL){
			_dprintf("[%s(%d)]  %s -> OK.\n", __FUNCTION__, __LINE__, word);
			return 1;
		}
	}

	_dprintf("[%s(%d)]  DNS Detect  -> Fail.\n", __FUNCTION__, __LINE__);
	
	return 0;
}

#if defined(RTCONFIG_APP_PREINSTALLED) || defined(RTCONFIG_APP_NETINSTALLED)
void update_apps_list(void)
{
	if(check_if_dir_exist("/opt/lib/ipkg")) {
		_dprintf("[%s(%d)] update the APP's lists...\n", __FUNCTION__, __LINE__);
		char *apps_argv[] = {"/usr/script/app_update.sh", NULL};
		//system("app_update.sh");
		_eval(apps_argv, NULL, 0, NULL);
	}
}
#endif

#if defined(RTCONFIG_BWDPI)
int restart_dpi_service()
{
	char *argv[7];

	//system("bwdpi service restart");
	argv[0] = "bwdpi";
	argv[1] = "service";
	argv[2] = "restart";
	argv[3] = NULL;
	_eval(argv, NULL, 0, NULL);
	
	return 0;
}
#endif

static int _add_static_route(char *route, const char *wan_ifname)
{
	char *pch, *ptr;
	int i = 0, cidr, flag = 0;
	char dst[20], gw[20], mask[20];

	if(!route || !wan_ifname)
		return -1;

	//parse route
	pch = strtok(route, " ");
	while(pch)
	{
		if(!(i%2))	//destination and mask
		{
			//init all
			memset(dst, 0, sizeof(dst));
			memset(gw, 0, sizeof(gw));
			memset(mask, 0, sizeof(mask));

			ptr = strchr(pch, '/');
			if(ptr)
			{
				*ptr = '\0';
				++ptr;
				cidr = atoi(ptr);
				snprintf(dst, sizeof(dst), "%s", pch);
				if(convert_cidr_to_subnet_mask(cidr, mask, sizeof(mask)))
					flag = 1;
			}
		}
		else		//gateway
		{
			if(flag)
			{
				snprintf(gw, sizeof(gw), "%s", pch);
				//add to routing table
				route_add(wan_ifname, 0, dst, gw, mask);
				flag = 0;
			}
		}
		++i;
		pch = strtok(NULL, " ");
	}
	return 0;
}

static int is_mr_wan(char *wan_ifname, int unit, int subunit)
{
	int ret = 0;

	if( tcapi_match("IPTV_Entry", "ad_mr_enable_x", "1")
		&& unit == tcapi_get_int("IPTV_Entry", "ad_wan_port")
	) {
		ret = 1;
	}
	else if(unit == WAN_UNIT_PTM0
		&& tcapi_match("IPTV_Entry", "vd_mr_enable_x", "1")
#if defined(TCSUPPORT_MULTISERVICE_ON_WAN)
		&& subunit == tcapi_get_int("IPTV_Entry", "vd_wan_port")
#endif
	) {
		ret = 1;
	}
	else if(unit == WAN_UNIT_ETH
		&& tcapi_match("IPTV_Entry", "eth_mr_enable_x", "1")
#if defined(TCSUPPORT_MULTISERVICE_ON_WAN)
		&& subunit == tcapi_get_int("IPTV_Entry", "eth_wan_port")
#endif
	) {
		ret = 1;
	}

	if(unit == 1 && tcapi_match("IPTV_Entry", "auto_detect_bng", "1"))	//ADSL non-default gateway WAN
	{
		if(tcapi_match("IPTV_Entry", "ad_mr_enable_x", "1")	&&	//adsl enable iptv
			!strcmp(wan_ifname, "nas1"))	//pvc1 interface is nas1
		{
			tcapi_set("IPTV_Entry", "auto_detect_bng", "0");
			tcapi_set("IPTV_Entry", "ad_wan_port", "1");
			ret = 1;
		}
	}
#if defined(TCSUPPORT_WAN_PTM) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
	else if(tcapi_match("IPTV_Entry", "auto_detect_bng", "1"))
	{
		if(tcapi_match("IPTV_Entry", "vd_mr_enable_x", "1")	&&	//vdsl enable iptv
			!strcmp(wan_ifname, "nas8_1"))	//pvc8_1 interface is nas8_1
		{
			tcapi_set("IPTV_Entry", "auto_detect_bng", "0");
			tcapi_set("IPTV_Entry", "vd_wan_port", "1");
			ret = 1;
		}
	}
#endif

	return ret;
}

void
wan_up(char *wan_ifname)	// oleg patch, replace
{
	char tmp[100];
	char prefix[] = "wanXXXXXXXXXX_";
	char wanpvc_prefix[] = "WanExt_PVCXXXXX";
	char wan_proto[8] = {0};
	char gateway[16] = {0};
	char dns[64] = {0};
	char staticroute[MAXLEN_TCAPI_MSG] = {0};
	int wan_unit;
	int wan_subunit;
	char word[64], *next;

	_dprintf("%s(%s)\n", __FUNCTION__, wan_ifname);

	if(get_wan_unit_ex(wan_ifname, &wan_unit, &wan_subunit) < 0)
	{
		_dprintf("get wan_unit fail.\n");
		return;
	}

	_dprintf("unit: %d, %d\n", wan_unit, wan_subunit);

	if(wan_subunit > 0)
	{
		snprintf(prefix, sizeof(prefix), "wan%d%d_", wan_unit, wan_subunit);
	}
	else
	{
		snprintf(prefix, sizeof(prefix), "wan%d_", wan_unit);
	}
#ifdef TCSUPPORT_MULTISERVICE_ON_WAN
	if(wan_unit == WAN_UNIT_PTM0 || wan_unit == WAN_UNIT_ETH)
	{
		if(wan_subunit > 0)
			snprintf(wanpvc_prefix, sizeof(wanpvc_prefix), "WanExt_PVC%de%d", wan_unit, wan_subunit);
		else
			snprintf(wanpvc_prefix, sizeof(wanpvc_prefix), "WanExt_PVC%de0", wan_unit);
	}
	else
#endif
	{
		snprintf(wanpvc_prefix, sizeof(wanpvc_prefix), "Wan_PVC%d", wan_unit);
	}
	//_dprintf("%s\n", wanpvc_prefix);

	memset(tmp, 0, sizeof(tmp));
	strncpy(wan_proto, nvram_safe_get(strcat_r(prefix, "proto", tmp)), sizeof(wan_proto)-1);
	strncpy(gateway, nvram_safe_get(strcat_r(prefix, "gateway", tmp)), sizeof(gateway)-1);
	_dprintf("proto:[%s]\ngateway:[%s]\n", wan_proto, gateway);

	if (inet_addr_(gateway) == INADDR_ANY)
		memset(gateway, 0, sizeof(gateway));

	/* Set default route to gateway if specified */
	if (strcmp(wan_proto, "dhcp") == 0 || strcmp(wan_proto, "static") == 0) {
		/* the gateway is in the local network */
		if (strlen(gateway))
			route_add(wan_ifname, 0, gateway, NULL, "255.255.255.255");
	}

	//update dhcp option 121
	if (strcmp(wan_proto, "dhcp") == 0) {
		snprintf(tmp, sizeof(tmp), "%sstaticroutes", prefix);
		if(!tcapi_get("Wanduck_Common", tmp, staticroute))
			_add_static_route(staticroute, wan_ifname);
		memset(staticroute, 0, sizeof(staticroute));
		snprintf(tmp, sizeof(tmp), "%smsstaticroutes", prefix);
		if(!tcapi_get("Wanduck_Common", tmp, staticroute))
			_add_static_route(staticroute, wan_ifname);
	}

	//update dns info
	if (strcmp(wan_proto, "dhcp") == 0 || strcmp(wan_proto, "pppoe") == 0) {
		if(tcapi_match(wanpvc_prefix, "DNS_type", "1"))
		{
			sprintf(dns, "%s", tcapi_get_string(wanpvc_prefix, "Primary_DNS", tmp));
			sprintf(dns + strlen(dns), "%s%s", strlen(dns) ? " " : "", tcapi_get_string(wanpvc_prefix, "Secondary_DNS", tmp));
			tcapi_set(WANDUCK_DATA, strcat_r(prefix, "dns", tmp), dns);
		}
		else
		{
			tcapi_get_string(WANDUCK_DATA, strcat_r(prefix, "dns_rx", tmp), dns);
			tcapi_set(WANDUCK_DATA, strcat_r(prefix, "dns", tmp), dns);
		}
	}

	update_wan_state(prefix, WAN_STATE_CONNECTED, WAN_STOPPED_REASON_NONE);

	//restart dns
	start_dnsmasq();

	//restart firewall
	tcapi_commit("Firewall");

	//restart igmp
	if(is_mr_wan(wan_ifname, wan_unit, wan_subunit))
		tcapi_commit("IPTV");

	// Syria workaround
	if (strcmp(wan_proto, "pppoe") == 0
	 && tcapi_match(wanpvc_prefix, "CONNECTION", "Connect_Keep_Alive")
	 && !tcapi_match(wanpvc_prefix, "DEFAULTROUTE", "Yes")
	) {
		tcapi_set("Vram_Entry", "igmp_sy_wr", "1");
	}

	//re-write routing table
	tcapi_commit("Route");

	/* default route via default gateway */
	add_multi_routes();

	// add wan dns route via wan interface
	tcapi_get(WANDUCK_DATA, strcat_r(prefix, "dns", tmp), dns);
	foreach(word, dns, next){
		_dprintf("[%s(%d)] add route for %s\n", __FUNCTION__, __LINE__, word);
		route_add(wan_ifname, 0, word, gateway, "255.255.255.255");
	}

	if(!tcapi_match(wanpvc_prefix, "DEFAULTROUTE", "Yes")) {
		_dprintf("%s(%s): done.\n", __FUNCTION__, wan_ifname);
		return;
	}

	//restart QoS
	tcapi_commit("QoS");

	//restart upnp?

	//Sync time?
	tcapi_commit("Timezone");

#ifdef RTCONFIG_OPENVPN
	start_ovpn_eas();
#endif
#ifdef RTCONFIG_VPNC
	// check and start vpnc
	if (tcapi_match("VPNC_Entry", "auto_conn", "1")) {
		//tcapi_commit("VPNC");
		eval("killall", "run_vpnc");
		stop_vpnc();
		system("/sbin/run_vpnc &");
	}
#endif
#if defined(RTCONFIG_PPTPD) || defined(RTCONFIG_ACCEL_PPTPD)
	if (tcapi_match("PPTP_Entry", "pptpd_enable", "1")) {
		stop_pptpd();
		start_pptpd();
	}
#endif

#ifdef RTCONFIG_TR069
	tcapi_commit("TR069");
#endif

	//start/stop hw_nat according to setting
	tcapi_commit("Misc");

	if(do_dns_detect()) {
		nvram_set_int("link_internet", 2);
#if defined(RTCONFIG_APP_PREINSTALLED) || defined(RTCONFIG_APP_NETINSTALLED)
		update_apps_list();
#endif
	}
	else {
		nvram_set_int("link_internet", 1);
	}

#if defined(TCSUPPORT_6RD)
	tcapi_commit("ipv6rd");

	/* If in Native Mode, skip restart Radvd/Dhcp6s in here. 
           the Radvd will be call in cc.c */
	if((tcapi_match("ipv6rd_Entry", "Active", "Yes") && 
	   !tcapi_match("ipv6rd_Entry", "tunMode", "NATIVE"))) {
		tcapi_commit("Radvd");
		tcapi_commit("Dhcp6s");
	}
#endif

#if defined(RTCONFIG_BWDPI)
	restart_dpi_service();
#endif

	//restart ddns
	tcapi_commit("Ddns");

	_dprintf("%s(%s): done.\n", __FUNCTION__, wan_ifname);
}

void
wan_down(char *wan_ifname)
{
	int wan_unit, wan_subunit;
	char prefix[] = "wanXXXXXXXXXX_";

	_dprintf("%s(%s)\n", __FUNCTION__, wan_ifname);

	if(get_wan_unit_ex(wan_ifname, &wan_unit, &wan_subunit) < 0)
		return;

	if(wan_subunit > 0)
		snprintf(prefix, sizeof(prefix), "wan%d%d_", wan_unit, wan_subunit);
	else
		snprintf(prefix, sizeof(prefix), "wan%d_", wan_unit);

	update_wan_state(prefix, WAN_STATE_DISCONNECTED, WAN_STOPPED_REASON_NONE);

#ifdef RTCONFIG_DUALWAN
	if(nvram_match("wans_mode", "lb"))
		add_multi_routes();
#endif
}

int
wan_ifunit(char *wan_ifname)
{
	int unit;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";

	if ((unit = ppp_linkunit(wan_ifname)) >= 0) {
		return unit;
	} else {
		for (unit = WAN_UNIT_FIRST; unit < WAN_UNIT_MAX; unit++) {
			snprintf(prefix, sizeof(prefix), "wan%d_", unit);
			if (nvram_match(strcat_r(prefix, "ifname", tmp), wan_ifname) &&
			    (nvram_match(strcat_r(prefix, "proto", tmp), "dhcp") ||
			     nvram_match(strcat_r(prefix, "proto", tmp), "static")))
				return unit;
		}
	}

	return -1;
}

int
found_default_route(int wan_unit)
{
	int i, n, found;
	FILE *f;
	unsigned int dest, mask;
	char buf[256], device[256];
	char *wanif;

	if(wan_unit != wan_primary_ifunit())
		return 1;

	n = 0;
	found = 0;
	mask = 0;
	device[0] = '\0';

	if ((f = fopen("/proc/net/route", "r")))
	{
		while (fgets(buf, sizeof(buf), f) != NULL)
		{
			if (++n == 1 && strncmp(buf, "Iface", 5) == 0)
				continue;

			i = sscanf(buf, "%255s %x %*s %*s %*s %*s %*s %x",
						device, &dest, &mask);

			if (i != 3)
			{
				break;
			}

			if (device[0] != '\0' && dest == 0 && mask == 0)
			{
				wanif = get_wan_ifname(wan_unit);
				if (!strcmp(wanif, device))
				{
					found = 1;
					break;
				}
			}
		}

		fclose(f);

		if (found)
		{
			return 1;
		}
	}

	_dprintf("\nNO default route!!!\n");
	return 0;
}

int find_default_route_ifname(char *ifname, size_t size)
{
	FILE *f = NULL;
	int n = 0;
	int i = 0;
	unsigned int dest = 0;
	unsigned int mask = 0;
	char buf[512] = {0};
	char device[256] = {0};

	f = fopen("/proc/net/route", "r");
	if ( f )
	{
		while (fgets(buf, sizeof(buf), f) != NULL)
		{
			if (++n == 1 && strncmp(buf, "Iface", 5) == 0)
				continue;

			i = sscanf(buf, "%255s %x %*s %*s %*s %*s %*s %x",
						device, &dest, &mask);

			if (i != 3)
			{
				//fprintf(stderr, "junk in buffer");
				break;
			}

			if (device[0] != '\0' && dest == 0 && mask == 0)
			{
				//fprintf(stderr, "default route dev: %s\n", device);
				break;
			}
		}
		fclose(f);
	}

	n = 0;
	n = snprintf(ifname, size, "%s", device);

	return (n > 0);
}
