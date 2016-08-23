/*
 * pptp.c
 *
 * Copyright (C) 2007 Sebastian Gottschall <gottschall@dd-wrt.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Id:
 */

#include <rc.h>
#include <stdlib.h>
#include <shutils.h>
#include <utils.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include "libtcapi.h"
#include "tcapi.h"
#include "tcutils.h"

#define	VPN_DATA	"PPTP_Entry"

void get_broadcast(char *ipaddr, char *netmask)
{
	int ip2[4], mask2[4];
	unsigned char ip[4], mask[4];

	if (!ipaddr || !netmask)
		return;

	sscanf(ipaddr, "%d.%d.%d.%d", &ip2[0], &ip2[1], &ip2[2], &ip2[3]);
	sscanf(netmask, "%d.%d.%d.%d", &mask2[0], &mask2[1], &mask2[2],
			&mask2[3]);
	int i = 0;

	for (i = 0; i < 4; i++) {
		ip[i] = ip2[i];
		mask[i] = mask2[i];
		ip[i] = (ip[i] & mask[i]) | (0xff & ~mask[i]);
	}

	sprintf(ipaddr, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

	//fprintf(stderr, "get_broadcast return %s\n", value);
}

char *pptp_encode(char *str, char *output)
{
	int i;
	int j;

	i=j=0;

	while(*(str+i)) {
		if(*(str+i)==' ' || *(str+i)=='\\' || *(str+i)=='*' || *(str+i)=='\'' || *(str+i)=='"' || *(str+i)=='#') {
			*(output+j++)='\\';
			*(output+j++)=*(str+i);
		}
		else *(output+j++) = *(str+i);
		i++;
	}

	*(output+j)=0;

	return output;
}

void write_chap_secret(char *file)
{
	FILE *fp;
	char nv[MAXLEN_TCAPI_MSG], *nvp, *b;
	char *username, *passwd;
	char namebuf[256], passwdbuf[256];
	int index = 1;
	char listname[32] = {0};

	fp=fopen(file, "w");

	if (fp==NULL) return;

	if( tcapi_get(VPN_DATA, "pptpd_clientlist", nv) == TCAPI_PROCESS_OK) {
		nvp = nv;
		while ((b = strsep(&nvp, "<")) != NULL) {
			if((vstrsep(b, ">", &username, &passwd)!=2)) continue;
			if(strlen(username)==0||strlen(passwd)==0) continue;
			fprintf(fp, "%s * %s *\n", pptp_encode(username, namebuf), pptp_encode(passwd, passwdbuf));
		}
	}

	for(index = 1; index < 16; index++)
	{
		memset(nv, 0, sizeof(nv));
		sprintf(listname, "pptpd_clientlist%d", index);
		if( tcapi_get(VPN_DATA, listname, nv) == TCAPI_PROCESS_OK) {
			nvp = nv;
			while ((b = strsep(&nvp, "<")) != NULL) {
				if((vstrsep(b, ">", &username, &passwd)!=2)) continue;
				if(strlen(username)==0||strlen(passwd)==0) continue;
				fprintf(fp, "%s * %s *\n", pptp_encode(username, namebuf), pptp_encode(passwd, passwdbuf));
			}
		}
	}
	fclose(fp);
}

void start_pptpd(void)
{
	int ret = 0, manual_dns = 0, pptpd_opt = 0;
	FILE *fp;
	char buf[MAXLEN_TCAPI_MSG];
	char lan_ipaddr[16] = {0};

	int pid = getpid();
	_dprintf("start_pptpd: getpid= %d\n", pid);

	if (!tcapi_match(VPN_DATA, "pptpd_enable", "1")) {
		return;
	}

	// cprintf("stop vpn modules\n");
	// stop_vpn_modules ();

	// Create directory for use by pptpd daemon and its supporting files
	mkdir("/tmp/pptpd", 0744);

	// Create options file that will be unique to pptpd to avoid interference
	// with pppoe and pptp
	fp = fopen("/tmp/pptpd/options.pptpd", "w");
	fprintf(fp, "logfile /var/log/pptpd-pppd.log\n");
	//fprintf(fp, "debug dump logfd 2 nodetach\n");
	if (tcapi_match(VPN_DATA, "pptpd_radius", "1"))
		fprintf(fp, "plugin radius.so\nplugin radattr.so\n"
			"radius-config-file /tmp/pptpd/radius/radiusclient.conf\n");

	fprintf(fp, "lock\n"
		"name *\n"
		"proxyarp\n"
//		"ipcp-accept-local\n"
//		"ipcp-accept-remote\n"
		"lcp-echo-failure 10\n"
		"lcp-echo-interval 6\n"
		"deflate 0\n" "auth\n" "-chap\n"
		"nomppe-stateful\n");

	pptpd_opt = tcapi_get_int(VPN_DATA, "pptpd_chap");
	fprintf(fp, "%smschap\n", (pptpd_opt == 0 || pptpd_opt & 1) ? "+" : "-");
	fprintf(fp, "%smschap-v2\n", (pptpd_opt == 0 || pptpd_opt & 2) ? "+" : "-");

	pptpd_opt = tcapi_get_int(VPN_DATA, "pptpd_mppe");
	if (pptpd_opt == 0)
		pptpd_opt = 1 | 4 | 8;
	if (pptpd_opt & (1 | 2 | 4)) {
		fprintf(fp, "%s", (pptpd_opt & 8) ? "" : "require-mppe\n");
  		fprintf(fp, "%smppe-128\n", (pptpd_opt & 1) ? "require-" : "no");
  		//fprintf(fp, "%smppe-56\n", (pptpd_opt & 2) ? "require-" : "no");
  		fprintf(fp, "%smppe-40\n", (pptpd_opt & 4) ? "require-" : "no");
	} else
  		fprintf(fp, "nomppe nomppc\n");

	fprintf(fp, "ms-ignore-domain\n"
		"chap-secrets /tmp/pptpd/chap-secrets\n"
		"ip-up-script /tmp/pptpd/ip-up\n"
		"ip-down-script /tmp/pptpd/ip-down\n"
		"mtu %d\n" "mru %d\n",
		tcapi_get_int(VPN_DATA, "pptpd_mtu"),
		tcapi_get_int(VPN_DATA, "pptpd_mru"));

	tcapi_get("Lan_Entry0", "IP", lan_ipaddr);

	//WINS Server
	int wins_count = 0;
	if (tcapi_match(VPN_DATA, "pptpd_ms_network", "1"))
		wins_count += fprintf(fp, "ms-wins %s\n", lan_ipaddr) > 0 ? 1 : 0;

	memset(buf, 0, sizeof(buf));
	tcapi_get(VPN_DATA, "pptpd_wins1", buf);
	if(strlen(buf)) {
		wins_count += fprintf(fp,"ms-wins %s\n", buf) > 0 ? 1 : 0;
	}
	memset(buf, 0, sizeof(buf));
	tcapi_get(VPN_DATA, "pptpd_wins2", buf);
	if(strlen(buf) && (wins_count < 2)) {
		wins_count += fprintf(fp,"ms-wins %s\n", buf) > 0 ? 1 : 0;
	}

	//DNS Server
	memset(buf, 0, sizeof(buf));
	tcapi_get(VPN_DATA, "pptpd_dns1", buf);
	if(strlen(buf)) {
		fprintf(fp, "ms-dns %s\n", buf);
		manual_dns=1;
	}
	memset(buf, 0, sizeof(buf));
	tcapi_get(VPN_DATA, "pptpd_dns2", buf);
	if(strlen(buf)) {
		fprintf(fp, "ms-dns %s\n", buf);
		manual_dns=1;
	}
	if(!manual_dns && strcmp(lan_ipaddr, ""))
		fprintf(fp, "ms-dns %s\n", lan_ipaddr);

	// force ppp interface starting from 20
	fprintf(fp, "minunit 20\n");

	// Following is all crude and need to be revisited once testing confirms
	// that it does work
	// Should be enough for testing..
	if (tcapi_match(VPN_DATA, "pptpd_radius", "1")) {
		char pptpd_radserver[128] = {0};
		char pptpd_radpass[128] = {0};
		char pptpd_radport[128] = {0};
		char pptpd_acctport[128] = {0};

		if (tcapi_get(VPN_DATA, "pptpd_radserver", pptpd_radserver) == TCAPI_PROCESS_OK
			&& tcapi_get(VPN_DATA, "pptpd_radpass", pptpd_radpass) == TCAPI_PROCESS_OK) {

			fclose(fp);

			mkdir("/tmp/pptpd/radius", 0744);

			if (tcapi_get(VPN_DATA, "pptpd_radport", pptpd_radport) != TCAPI_PROCESS_OK)
				strcpy(pptpd_radport, "radius");

			fp = fopen("/tmp/pptpd/radius/radiusclient.conf", "w");
			fprintf(fp, "auth_order radius\n"
				"login_tries 4\n"
				"login_timeout 60\n"
				"radius_timeout 10\n"
				"nologin /etc/nologin\n"
				"servers /tmp/pptpd/radius/servers\n"
				"dictionary /etc/dictionary\n"
				"seqfile /var/run/radius.seq\n"
				"mapfile /etc/port-id-map\n"
				"radius_retries 3\n"
				"authserver %s:%s\n",
				pptpd_radserver, pptpd_radport);

			if (tcapi_get(VPN_DATA, "pptpd_acctport", pptpd_acctport) != TCAPI_PROCESS_OK)
				strcpy(pptpd_acctport, "radacct");

			fprintf(fp, "acctserver %s:%s\n", pptpd_radserver, pptpd_acctport);
			fclose(fp);

			fp = fopen("/tmp/pptpd/radius/servers", "w");
			fprintf(fp, "%s\t%s\n", pptpd_radserver, pptpd_radpass);
			fclose(fp);

		} else
			fclose(fp);
	} else
		fclose(fp);

	// Create pptpd.conf options file for pptpd daemon
	fp = fopen("/tmp/pptpd/pptpd.conf", "w");

	memset(buf, 0, sizeof(buf));
	tcapi_get(VPN_DATA, "pptpd_clients", buf);
	fprintf(fp, "localip %s\n"
		"remoteip %s\n", lan_ipaddr, buf);

	memset(buf, 0, sizeof(buf));
	tcapi_get(VPN_DATA, "pptpd_broadcast", buf);
	if ( strcmp(buf, "") && strcmp(buf,"disable") && strcmp(buf,"0")) {
		fprintf(fp, "bcrelay %s,%s\n",
			"br0", "ppp2[0-9].*");
	}
	fclose(fp);

	// Create ip-up and ip-down scripts that are unique to pptpd to avoid
	// interference with pppoe and pptp
	/*
	 * adjust for tunneling overhead (mtu - 40 byte IP - 108 byte tunnel
	 * overhead)
	 */
	//if (nvram_match("mtu_enable", "1"))
		//mss = atoi(nvram_safe_get("wan_mtu")) - 40 - 108;
	//else
		//mss = 1500 - 40 - 108;
	char bcast[32];

	strcpy(bcast, lan_ipaddr);
	memset(buf, 0, sizeof(buf));
	get_broadcast(bcast, tcapi_get_string("Lan_Entry0", "netmask", buf));

	memset(buf, 0, sizeof(buf));
	tcapi_get(VPN_DATA, "pptpd_ipup_script", buf);
	fp = fopen("/tmp/pptpd/ip-up", "w");
	fprintf(fp, "#!/bin/sh\n" "startservice set_routes\n"	// reinitialize
		"echo $PPPD_PID $1 $5 $6 $PEERNAME >> /tmp/pptp_connected\n"
		"iptables -I FORWARD -i $1 -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu\n"
		"iptables -I INPUT -i $1 -j ACCEPT\n" "iptables -I FORWARD -i $1 -j ACCEPT\n"
		"iptables -t nat -I PREROUTING -i $1 -p udp -m udp --sport 9 -j DNAT --to-destination %s "	// rule for wake on lan over pptp tunnel
		"%s\n", bcast, buf);

	//Add static route for vpn client
	char nv[MAXLEN_TCAPI_MSG], *nvp, *b;
	char *pptpd_client, *vpn_network, *vpn_netmask;
	int i;
	char listname[MAXLEN_ATTR_NAME] = {0};

	for(i = 0; i < 16; i++)
	{
		memset(nv, 0, sizeof(nv));
		if(i)
			sprintf(listname, "pptpd_sr_rulelist%d", i);
		else
			sprintf(listname, "pptpd_sr_rulelist");
		if( tcapi_get(VPN_DATA, listname, nv) == TCAPI_PROCESS_OK) {
			nvp = nv;
			while ((b = strsep(&nvp, "<")) != NULL) {
				if((vstrsep(b, ">", &pptpd_client, &vpn_network, &vpn_netmask)!=3)) continue;
				if(strlen(pptpd_client)==0||strlen(vpn_network)==0||strlen(vpn_netmask)==0) continue;
				fprintf(fp, "if[ \"$PEERNAME\" == \"%s\" ] then;\n", pptpd_client);
				fprintf(fp, "route del -net %s netmask %s\n", vpn_network, vpn_netmask);
				fprintf(fp, "route add -net %s netmask %s dev $1\n", vpn_network, vpn_netmask);
				fprintf(fp, "fi\n");
			}
		}
	}

	fclose(fp);
	memset(buf, 0, sizeof(buf));
	tcapi_get(VPN_DATA, "pptpd_ipdown_script", buf);
	fp = fopen("/tmp/pptpd/ip-down", "w");
	fprintf(fp, "#!/bin/sh\n" "grep -v $1  /tmp/pptp_connected > /tmp/pptp_connected.new\n"
		"mv /tmp/pptp_connected.new /tmp/pptp_connected\n"
		"iptables -D FORWARD -i $1 -p tcp --tcp-flags SYN,RST SYN -j TCPMSS --clamp-mss-to-pmtu\n"
		"iptables -D INPUT -i $1 -j ACCEPT\n"
		"iptables -D FORWARD -i $1 -j ACCEPT\n"
		"iptables -t nat -D PREROUTING -i $1 -p udp -m udp --sport 9 -j DNAT --to-destination %s "	// rule for wake on lan over pptp tunnel
		"%s\n", bcast, buf);
	fclose(fp);
	chmod("/tmp/pptpd/ip-up", 0744);
	chmod("/tmp/pptpd/ip-down", 0744);

	// Exctract chap-secrets from nvram
	write_chap_secret("/tmp/pptpd/chap-secrets");

	chmod("/tmp/pptpd/chap-secrets", 0600);

	// Execute pptpd daemon
	ret =
	    eval("pptpd", "-c", "/tmp/pptpd/pptpd.conf", "-o",
		 "/tmp/pptpd/options.pptpd");

	_dprintf("start_pptpd: ret= %d\n", ret);
	//dd_syslog(LOG_INFO, "pptpd : pptp daemon successfully started\n");
	return;
}

void stop_pptpd(void)
{
	//if (getpid() != 1) {
		//notify_rc("stop_pptpd");
	//}

	killall_tk("pptpd");
	killall_tk("bcrelay");
	return;
}
