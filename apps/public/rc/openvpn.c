/*

	Copyright (C) 2008-2010 Keith Moyer, tomatovpn@keithmoyer.com

	No part of this file may be used without permission.

*/

#include "rc.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <misc.h>

// Line number as text string
#define __LINE_T__ __LINE_T_(__LINE__)
#define __LINE_T_(x) __LINE_T(x)
#define __LINE_T(x) # x

#define OPENVPN_COMMON_NODE	"OpenVPN_Common"

#define VPN_LOG_ERROR -1
#define VPN_LOG_NOTE 0
#define VPN_LOG_INFO 1
#define VPN_LOG_EXTRA 2
#define vpnlog(level,x...) if(tcapi_get_int(OPENVPN_COMMON_NODE,"vpn_debug")>=level) syslog(LOG_INFO, #level ": " __LINE_T__ ": " x)

#define BUF_SIZE 4096
#define IF_SIZE 8

#define PUSH_LAN_METRIC 500

static int ovpn_waitfor(const char *name)
{
	int pid, n = 5;

	killall_tk(name);
	while ( (pid = pidof(name)) > 0 && (n-- > 0) )
	{
		// Reap the zombie if it has terminated
		waitpid(pid, NULL, WNOHANG);
		sleep(1);
	}
	return (pid >= 0);
}

void start_vpnclient(int clientNum)
{
	FILE *fp;
	char iface[IF_SIZE];
	char node[MAXLEN_NODE_NAME] = {0};
	char buffer[BUF_SIZE];
	char buffer2[4000];
	char *argv[6];
	int argc = 0;
	enum { TLS, SECRET, CUSTOM } cryptMode = CUSTOM;
	enum { TAP, TUN } ifType = TUN;
	enum { BRIDGE, NAT, NONE } routeMode = NONE;
	int nvi, ip[4], nm[4];
	long int nvl;
	int pid;
	int userauth, useronly;

	sprintf(&buffer[0], "start_vpnclient%d", clientNum);

	vpnlog(VPN_LOG_INFO,"VPN GUI client backend starting...");

	if ( (pid = pidof(&buffer[6])) > 0 )
	{
		vpnlog(VPN_LOG_NOTE, "VPN Client %d already running...", clientNum);
		vpnlog(VPN_LOG_INFO,"PID: %d", pid);
		return;
	}

	if(clientNum)
		sprintf(node, "OpenVPN_Entry%d", clientNum+CLIENT_IF_START);
	else
		sprintf(node, "OpenVPN_Entry11");

	//initial status
	tcapi_set(node, "state", "1");
	tcapi_set(node, "errno", "0");

	// Determine interface
	tcapi_get(node, "iface", &buffer[0]);
	if ( find_word(&buffer[0], "tap") )
		ifType = TAP;
	else if ( find_word(&buffer[0], "tun") )
		ifType = TUN;
	else
	{
		vpnlog(VPN_LOG_ERROR, "Invalid interface type, %.3s", &buffer[0]);
		return;
	}

	// Build interface name
	snprintf(&iface[0], IF_SIZE, "%s%d", &buffer[0], clientNum+CLIENT_IF_START);

	// Determine encryption mode
	tcapi_get(node, "crypt", &buffer[0]);
	if ( find_word(&buffer[0], "tls") )
		cryptMode = TLS;
	else if ( find_word(&buffer[0], "secret") )
		cryptMode = SECRET;
	else if ( find_word(&buffer[0], "custom") )
		cryptMode = CUSTOM;
	else
	{
		vpnlog(VPN_LOG_ERROR,"Invalid encryption mode, %.6s", &buffer[0]);
		return;
	}

	// Determine if we should bridge the tunnel
	tcapi_get(node, "bridge", &buffer[0]);
	if ( ifType == TAP && atoi(&buffer[0]) == 1 )
		routeMode = BRIDGE;

	// Determine if we should NAT the tunnel
	tcapi_get(node, "nat", &buffer[0]);
	if ( (ifType == TUN || routeMode != BRIDGE) && atoi(&buffer[0]) == 1 )
		routeMode = NAT;

	// Make sure openvpn directory exists
	mkdir("/etc/openvpn", 0700);
	sprintf(&buffer[0], "/etc/openvpn/client%d", clientNum);
	mkdir(&buffer[0], 0700);

	// Make sure symbolic link exists
	sprintf(&buffer[0], "/etc/openvpn/vpnclient%d", clientNum);
	unlink(&buffer[0]);
	if ( symlink("/usr/sbin/openvpn", &buffer[0]) )
	{
		vpnlog(VPN_LOG_ERROR,"Creating symlink failed...");
		stop_vpnclient(clientNum);
		return;
	}

	// Make sure module is loaded
	modprobe("tun");
	f_wait_exists("/dev/net/tun", 5);

	// Create tap/tun interface
	sprintf(&buffer[0], "openvpn --mktun --dev %s", &iface[0]);
	for (argv[argc=0] = strtok(&buffer[0], " "); argv[argc] != NULL; argv[++argc] = strtok(NULL, " "));
	if ( _eval(argv, NULL, 0, NULL) )
	{
		vpnlog(VPN_LOG_ERROR,"Creating tunnel interface %s failed...",&iface[0]);
		stop_vpnclient(clientNum);
		return;
	}

	// Bring interface up (TAP only)
	if( ifType == TAP )
	{
		if ( routeMode == BRIDGE )
		{
			snprintf(&buffer[0], BUF_SIZE, "brctl addif br0 %s", &iface[0]);
			for (argv[argc=0] = strtok(&buffer[0], " "); argv[argc] != NULL; argv[++argc] = strtok(NULL, " "));
			if ( _eval(argv, NULL, 0, NULL) )
			{
				vpnlog(VPN_LOG_ERROR,"Adding tunnel interface to bridge failed...");
				stop_vpnclient(clientNum);
				return;
			}
		}

		snprintf(&buffer[0], BUF_SIZE, "ifconfig %s promisc up", &iface[0]);
		for (argv[argc=0] = strtok(&buffer[0], " "); argv[argc] != NULL; argv[++argc] = strtok(NULL, " "));
		if ( _eval(argv, NULL, 0, NULL) )
		{
			vpnlog(VPN_LOG_ERROR,"Bringing interface up failed...");
			stop_vpnclient(clientNum);
			return;
		}
	}

	tcapi_get(node, "userauth", &buffer[0]);
	userauth = atoi(&buffer[0]);
	tcapi_get(node, "useronly", &buffer[0]);
	useronly = userauth && atoi(&buffer[0]);

	// Build and write config file
	vpnlog(VPN_LOG_EXTRA,"Writing config file");
	sprintf(&buffer[0], "/etc/openvpn/client%d/config.ovpn", clientNum);
	fp = fopen(&buffer[0], "w");
	chmod(&buffer[0], S_IRUSR|S_IWUSR);
	fprintf(fp, "# Automatically generated configuration\n");
	fprintf(fp, "daemon\n");
	if ( cryptMode == TLS )
		fprintf(fp, "client\n");
	fprintf(fp, "dev %s\n", &iface[0]);
	tcapi_get(node, "proto", &buffer[0]);
	fprintf(fp, "proto %s\n", &buffer[0]);
	tcapi_get(node, "addr", &buffer[0]);
	fprintf(fp, "remote %s ", &buffer[0]);
	tcapi_get(node, "port", &buffer[0]);
	fprintf(fp, "%s\n", &buffer[0]);
	if ( cryptMode == SECRET )
	{
		if ( ifType == TUN )
		{
			tcapi_get(node, "local", &buffer[0]);
			fprintf(fp, "ifconfig %s ", &buffer[0]);
			tcapi_get(node, "remote", &buffer[0]);
			fprintf(fp, "%s\n", &buffer[0]);
		}
		else if ( ifType == TAP )
		{
			tcapi_get(node, "local", &buffer[0]);
			fprintf(fp, "ifconfig %s ", &buffer[0]);
			tcapi_get(node, "netmask", &buffer[0]);
			fprintf(fp, "%s\n", &buffer[0]);
		}
	}
	tcapi_get(node, "retry", &buffer[0]);
	if ( (nvi = atoi(&buffer[0])) >= 0 )
		fprintf(fp, "resolv-retry %d\n", nvi);
	else
		fprintf(fp, "resolv-retry infinite\n");

	fprintf(fp, "nobind\n");
	fprintf(fp, "persist-key\n");
	fprintf(fp, "persist-tun\n");
	tcapi_get(node, "comp", &buffer[0]);
	if ( atoi(&buffer[0]) >= 0 )
		fprintf(fp, "comp-lzo %s\n", &buffer[0]);
	tcapi_get(node, "cipher", &buffer[0]);
	if ( !find_word(&buffer[0], "default") )
		fprintf(fp, "cipher %s\n", &buffer[0]);
	tcapi_get(node, "digest", &buffer[0]);
	if ( !find_word(&buffer[0], "default") )
		fprintf(fp, "auth %s\n", &buffer[0]);
	tcapi_get(node, "redirect_gateway", &buffer[0]);
	if ( atoi(&buffer[0]) )
	{
		tcapi_get(node, "gateway", &buffer[0]);
		if ( ifType == TAP && buffer[0] != '\0' )
			fprintf(fp, "route-gateway %s\n", &buffer[0]);
		fprintf(fp, "redirect-gateway def1\n");
	}
	if ( (nvi = tcapi_get_int(node,"loglevel")) >= 0 )
		fprintf(fp, "verb %d\n", nvi);
	else
		fprintf(fp, "verb 3\n");
	if ( cryptMode == TLS )
	{
		tcapi_get(node, "reneg", &buffer[0]);
		if ( (nvl = atol(&buffer[0])) >= 0 )
			fprintf(fp, "reneg-sec %ld\n", nvl);

		tcapi_get(node, "adns", &buffer[0]);
		if ( atoi(&buffer[0]) > 0 )
		{
			sprintf(&buffer[0], "/etc/openvpn/client%d/updown.sh", clientNum);
			symlink("/usr/sbin/updown.sh", &buffer[0]);
			fprintf(fp, "script-security 2\n");
			fprintf(fp, "up updown.sh\n");
			fprintf(fp, "down updown.sh\n");
		}

		tcapi_get(node, "hmac", &buffer[0]);
		nvi = atoi(&buffer[0]);
		sprintf(&buffer[0], "vpn_crt_client%d_static", clientNum);
		if ( !ovpn_crt_is_empty(&buffer[0]) && nvi >= 0 )
		{
			fprintf(fp, "tls-auth static.key");
			if ( nvi < 2 )
				fprintf(fp, " %d", nvi);
			fprintf(fp, "\n");
		}

		sprintf(&buffer[0], "vpn_crt_client%d_ca", clientNum);
		if ( !ovpn_crt_is_empty(&buffer[0]) )
			fprintf(fp, "ca ca.crt\n");
		if (!useronly)
		{
			sprintf(&buffer[0], "vpn_crt_client%d_crt", clientNum);
			if ( !ovpn_crt_is_empty(&buffer[0]) )
				fprintf(fp, "cert client.crt\n");
			sprintf(&buffer[0], "vpn_crt_client%d_key", clientNum);
			if ( !ovpn_crt_is_empty(&buffer[0]) )
				fprintf(fp, "key client.key\n");
		}
		tcapi_get(node, "tlsremote", &buffer[0]);	//DEPRECATED OPTION
		if (atoi(&buffer[0]))
		{
			tcapi_get(node, "common_name", &buffer[0]);
			fprintf(fp, "tls-remote %s\n", &buffer[0]);
		}
		if (userauth)
			fprintf(fp, "auth-user-pass up\n");

		sprintf(&buffer[0], "vpn_crt_client%d_crl", clientNum);
		if ( !ovpn_crt_is_empty(&buffer[0]) )
			fprintf(fp, "crl-verify crl.pem\n");
	}
	else if ( cryptMode == SECRET )
	{
		sprintf(&buffer[0], "vpn_crt_client%d_static", clientNum);
		if ( !ovpn_crt_is_empty(&buffer[0]) )
			fprintf(fp, "secret static.key\n");

	}

	// All other cryptmodes need a default up/down script set
	if ( (cryptMode != TLS) && (check_if_file_exist("/jffs/scripts/openvpn-event")) )
	{
		sprintf(&buffer[0], "/etc/openvpn/client%d/updown.sh", clientNum);
		symlink("/jffs/scripts/openvpn-event", &buffer[0]);
		fprintf(fp, "script-security 2\n");
		fprintf(fp, "up updown.sh\n");
		fprintf(fp, "down updown.sh\n");
	}

	fprintf(fp, "status-version 2\n");
	fprintf(fp, "status status 10\n");
	fprintf(fp, "\n# Custom Configuration\n");
	tcapi_get(node, "custom", &buffer[0]);
	fprintf(fp, "%s", &buffer[0]);
	fclose(fp);
	vpnlog(VPN_LOG_EXTRA,"Done writing config file");

	// Write certification and key files
	vpnlog(VPN_LOG_EXTRA,"Writing certs/keys");
	if ( cryptMode == TLS )
	{
		sprintf(&buffer[0], "vpn_crt_client%d_ca", clientNum);
		if ( !ovpn_crt_is_empty(&buffer[0]) )
		{
			sprintf(&buffer[0], "/etc/openvpn/client%d/ca.crt", clientNum);
			fp = fopen(&buffer[0], "w");
			chmod(&buffer[0], S_IRUSR|S_IWUSR);
			sprintf(&buffer[0], "vpn_crt_client%d_ca", clientNum);
			fprintf(fp, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
			fclose(fp);
		}

		if (!useronly)
		{
			sprintf(&buffer[0], "vpn_crt_client%d_key", clientNum);
			if ( !ovpn_crt_is_empty(&buffer[0]) )
			{
				sprintf(&buffer[0], "/etc/openvpn/client%d/client.key", clientNum);
				fp = fopen(&buffer[0], "w");
				chmod(&buffer[0], S_IRUSR|S_IWUSR);
				sprintf(&buffer[0], "vpn_crt_client%d_key", clientNum);
				fprintf(fp, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
				fclose(fp);
			}

			sprintf(&buffer[0], "vpn_crt_client%d_crt", clientNum);
			if ( !ovpn_crt_is_empty(&buffer[0]) )
			{
				sprintf(&buffer[0], "/etc/openvpn/client%d/client.crt", clientNum);
				fp = fopen(&buffer[0], "w");
				chmod(&buffer[0], S_IRUSR|S_IWUSR);
				sprintf(&buffer[0], "vpn_crt_client%d_crt", clientNum);
				fprintf(fp, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
				fclose(fp);
			}
		}
		if (userauth)
		{
			sprintf(&buffer[0], "/etc/openvpn/client%d/up", clientNum);
			fp = fopen(&buffer[0], "w");
			chmod(&buffer[0], S_IRUSR|S_IWUSR);
			tcapi_get(node, "username", &buffer[0]);
			fprintf(fp, "%s\n", &buffer[0]);
			tcapi_get(node, "password", &buffer[0]);
			fprintf(fp, "%s\n", &buffer[0]);
			fclose(fp);
		}

		sprintf(&buffer[0], "vpn_crt_client%d_crl", clientNum);
		if ( !ovpn_crt_is_empty(&buffer[0]) )
		{
			sprintf(&buffer[0], "/etc/openvpn/client%d/crl.pem", clientNum);
			fp = fopen(&buffer[0], "w");
			chmod(&buffer[0], S_IRUSR|S_IWUSR);
			sprintf(&buffer[0], "vpn_crt_client%d_crl", clientNum);
			fprintf(fp, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
			fclose(fp);
		}
	}
	tcapi_get(node, "hmac", &buffer[0]);
	if ( cryptMode == SECRET || (cryptMode == TLS && atoi(&buffer[0]) >= 0) )
	{
		sprintf(&buffer[0], "vpn_crt_client%d_static", clientNum);
		if ( !ovpn_crt_is_empty(&buffer[0]) )
		{
			sprintf(&buffer[0], "/etc/openvpn/client%d/static.key", clientNum);
			fp = fopen(&buffer[0], "w");
			chmod(&buffer[0], S_IRUSR|S_IWUSR);
			sprintf(&buffer[0], "vpn_crt_client%d_static", clientNum);
			fprintf(fp, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
			fclose(fp);
		}
	}
	vpnlog(VPN_LOG_EXTRA,"Done writing certs/keys");

	// Start the VPN client
	sprintf(&buffer[0], "/etc/openvpn/vpnclient%d --cd /etc/openvpn/client%d --config config.ovpn", clientNum, clientNum);
	vpnlog(VPN_LOG_INFO,"Starting OpenVPN: %s",&buffer[0]);
	for (argv[argc=0] = strtok(&buffer[0], " "); argv[argc] != NULL; argv[++argc] = strtok(NULL, " "));
	if ( _eval(argv, NULL, 0, &pid) )
	{
		vpnlog(VPN_LOG_ERROR,"Starting OpenVPN failed...");
		stop_vpnclient(clientNum);
		return;
	}
	vpnlog(VPN_LOG_EXTRA,"Done starting openvpn");

	// Handle firewall rules if appropriate
	tcapi_get(node, "firewall", &buffer[0]);
	if ( !find_word(&buffer[0], "custom") )
	{
		// Create firewall rules
		vpnlog(VPN_LOG_EXTRA,"Creating firewall rules");
		mkdir("/etc/openvpn/fw", 0700);
		sprintf(&buffer[0], "/etc/openvpn/fw/client%d-fw.sh", clientNum);
		fp = fopen(&buffer[0], "w");
		chmod(&buffer[0], S_IRUSR|S_IWUSR|S_IXUSR);
		fprintf(fp, "#!/bin/sh\n");
		fprintf(fp, "iptables -I INPUT -i %s -j ACCEPT\n", &iface[0]);
		fprintf(fp, "iptables -I FORWARD -i %s -j ACCEPT\n", &iface[0]);
		if ( routeMode == NAT )
		{
			tcapi_get("Lan_Entry0", "IP", &buffer[0]);
			sscanf(&buffer[0], "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
			tcapi_get("Lan_Entry0", "netmask", &buffer[0]);
			sscanf(&buffer[0], "%d.%d.%d.%d", &nm[0], &nm[1], &nm[2], &nm[3]);
			fprintf(fp, "iptables -t nat -I POSTROUTING -s %d.%d.%d.%d/%s -o %s -j MASQUERADE\n",
			        ip[0]&nm[0], ip[1]&nm[1], ip[2]&nm[2], ip[3]&nm[3], &buffer[0], &iface[0]);
		}
		fclose(fp);
		vpnlog(VPN_LOG_EXTRA,"Done creating firewall rules");

		// Run the firewall rules
		vpnlog(VPN_LOG_EXTRA,"Running firewall rules");
		sprintf(&buffer[0], "/etc/openvpn/fw/client%d-fw.sh", clientNum);
		argv[0] = &buffer[0];
		argv[1] = NULL;
		_eval(argv, NULL, 0, NULL);
		vpnlog(VPN_LOG_EXTRA,"Done running firewall rules");
	}

	// Set up cron job
	tcapi_get(node, "poll", &buffer[0]);
	if ( (nvi = atoi(&buffer[0])) > 0 )
	{
		vpnlog(VPN_LOG_EXTRA,"Adding cron job");
		argv[0] = "cru";
		argv[1] = "a";
		sprintf(&buffer[0], "CheckVPNClient%d", clientNum);
		argv[2] = &buffer[0];
		sprintf(&buffer[strlen(&buffer[0])+1], "*/%d * * * * service start_vpnclient%d", nvi, clientNum);
		argv[3] = &buffer[strlen(&buffer[0])+1];
		argv[4] = NULL;
		_eval(argv, NULL, 0, NULL);
		vpnlog(VPN_LOG_EXTRA,"Done adding cron job");
	}

#ifdef LINUX26
//	sprintf(&buffer[0], "vpn_client%d", clientNum);
//	allow_fastnat(buffer, 0);
//	try_enabling_fastnat();
#endif

	vpnlog(VPN_LOG_INFO,"VPN GUI client backend complete.");
}

void stop_vpnclient(int clientNum)
{
	int argc;
	char *argv[7];
	char buffer[BUF_SIZE];

	sprintf(&buffer[0], "stop_vpnclient%d", clientNum);

	vpnlog(VPN_LOG_INFO,"Stopping VPN GUI client backend.");

	// Remove cron job
	vpnlog(VPN_LOG_EXTRA,"Removing cron job");
	argv[0] = "cru";
	argv[1] = "d";
	sprintf(&buffer[0], "CheckVPNClient%d", clientNum);
	argv[2] = &buffer[0];
	argv[3] = NULL;
	_eval(argv, NULL, 0, NULL);
	vpnlog(VPN_LOG_EXTRA,"Done removing cron job");

	// Remove firewall rules
	vpnlog(VPN_LOG_EXTRA,"Removing firewall rules.");
	sprintf(&buffer[0], "/etc/openvpn/fw/client%d-fw.sh", clientNum);
	argv[0] = "sed";
	argv[1] = "-i";
	argv[2] = "s/-A/-D/g;s/-I/-D/g";
	argv[3] = &buffer[0];
	argv[4] = NULL;
	if (!_eval(argv, NULL, 0, NULL))
	{
		argv[0] = &buffer[0];
		argv[1] = NULL;
		_eval(argv, NULL, 0, NULL);
	}
	vpnlog(VPN_LOG_EXTRA,"Done removing firewall rules.");

	// Stop the VPN client
	vpnlog(VPN_LOG_EXTRA,"Stopping OpenVPN client.");
	sprintf(&buffer[0], "vpnclient%d", clientNum);
	if ( !ovpn_waitfor(&buffer[0]) )
		vpnlog(VPN_LOG_EXTRA,"OpenVPN client stopped.");

	// NVRAM setting for device type could have changed, just try to remove both
	vpnlog(VPN_LOG_EXTRA,"Removing VPN device.");
	sprintf(&buffer[0], "openvpn --rmtun --dev tap%d", clientNum+CLIENT_IF_START);
	for (argv[argc=0] = strtok(&buffer[0], " "); argv[argc] != NULL; argv[++argc] = strtok(NULL, " "));
	_eval(argv, NULL, 0, NULL);

	sprintf(&buffer[0], "openvpn --rmtun --dev tun%d", clientNum+CLIENT_IF_START);
	for (argv[argc=0] = strtok(&buffer[0], " "); argv[argc] != NULL; argv[++argc] = strtok(NULL, " "));
	_eval(argv, NULL, 0, NULL);
	vpnlog(VPN_LOG_EXTRA,"VPN device removed.");

	modprobe_r("tun");

	if ( tcapi_get_int(OPENVPN_COMMON_NODE,"vpn_debug") <= VPN_LOG_EXTRA )
	{
		vpnlog(VPN_LOG_EXTRA,"Removing generated files.");
		// Delete all files for this client
		sprintf(&buffer[0], "rm -rf /etc/openvpn/client%d /etc/openvpn/fw/client%d-fw.sh /etc/openvpn/vpnclient%d",clientNum,clientNum,clientNum);
		for (argv[argc=0] = strtok(&buffer[0], " "); argv[argc] != NULL; argv[++argc] = strtok(NULL, " "));
		_eval(argv, NULL, 0, NULL);

		// Attempt to remove directories.  Will fail if not empty
		rmdir("/etc/openvpn/fw");
		rmdir("/etc/openvpn");
		vpnlog(VPN_LOG_EXTRA,"Done removing generated files.");
	}

#ifdef LINUX26
//	sprintf(&buffer[0], "vpn_client%d", clientNum);
//	allow_fastnat(buffer, 1);
//	try_enabling_fastnat();
#endif

	//update status
	if(clientNum)
		sprintf(buffer, "OpenVPN_Entry%d", clientNum+CLIENT_IF_START);
	else
		sprintf(buffer, "OpenVPN_Entry11");
	tcapi_set(buffer, "state", "0");
	tcapi_set(buffer, "errno", "0");

	vpnlog(VPN_LOG_INFO,"VPN GUI client backend stopped.");
}

void start_vpnserver(int serverNum)
{
	FILE *fp, *ccd, *fp_client;
	char iface[IF_SIZE] = {0};
	char node[MAXLEN_NODE_NAME] = {0};
	char buffer[BUF_SIZE] = {0};
	char buffer2[4000] = {0};
	char *argv[6], *chp, *route;
	int argc = 0;
	int c2c = 0;
	enum { TAP, TUN } ifType = TUN;
	enum { TLS, SECRET, CUSTOM } cryptMode = CUSTOM;
	int nvi, ip[4], nm[4];
	long int nvl;
	int pid;
	char nv1[32], nv2[32], nv3[32], fpath[128];
	int valid = 0;
	int useronly = 0;
	int dhkeysize = 0;

	sprintf(&buffer[0], "start_vpnserver%d", serverNum);

	vpnlog(VPN_LOG_INFO,"VPN GUI server backend starting...");

	if ( (pid = pidof(&buffer[6])) > 0 )
	{
		vpnlog(VPN_LOG_NOTE, "%s already running...", &buffer[6]);
		vpnlog(VPN_LOG_INFO,"PID: %d", pid);
		return;
	}

	if(serverNum) {
		sprintf(node, "OpenVPN_Entry%d", serverNum+SERVER_IF_START);
		sprintf(buffer, "%d", serverNum+SERVER_IF_START);
		tcapi_set("WebCurSet_Entry", "openvpn_id", buffer);
	}
	else {
		sprintf(node, "OpenVPN_Entry21");
		tcapi_set("WebCurSet_Entry", "openvpn_id", "21");
	}

	//initial status
	tcapi_set(node, "state", "1");
	tcapi_set(node, "errno", "0");

	// Determine interface type
	tcapi_get(node, "iface", &buffer[0]);
	if ( find_word(&buffer[0], "tap") )
		ifType = TAP;
	else if ( find_word(&buffer[0], "tun") )
		ifType = TUN;
	else
	{
		vpnlog(VPN_LOG_ERROR,"Invalid interface type, %.3s", &buffer[0]);
		return;
	}

	// Build interface name
	snprintf(&iface[0], IF_SIZE, "%s%d", &buffer[0], serverNum+SERVER_IF_START);

	//
	if(is_intf_up(&iface[0]) && ifType == TAP) {
		eval("brctl", "delif", "br0", &iface[0]);
	}

	// Determine encryption mode
	tcapi_get(node, "crypt", &buffer[0]);
	if ( find_word(&buffer[0], "tls") )
		cryptMode = TLS;
	else if ( find_word(&buffer[0], "secret") )
		cryptMode = SECRET;
	else if ( find_word(&buffer[0], "custom") )
		cryptMode = CUSTOM;
	else
	{
		vpnlog(VPN_LOG_ERROR,"Invalid encryption mode, %.6s", &buffer[0]);
		return;
	}

	// Make sure openvpn directory exists
	mkdir("/etc/openvpn", 0700);
	sprintf(&buffer[0], "/etc/openvpn/server%d", serverNum);
	mkdir(&buffer[0], 0700);

	// Make sure symbolic link exists
	sprintf(&buffer[0], "/etc/openvpn/vpnserver%d", serverNum);
	unlink(&buffer[0]);
	if ( symlink("/usr/sbin/openvpn", &buffer[0]) )
	{
		vpnlog(VPN_LOG_ERROR,"Creating symlink failed...");
		stop_vpnserver(serverNum);
		return;
	}

	// Make sure module is loaded
	modprobe("tun");
	f_wait_exists("/dev/net/tun", 5);

	// Create tap/tun interface
	sprintf(&buffer[0], "openvpn --mktun --dev %s", &iface[0]);
	for (argv[argc=0] = strtok(&buffer[0], " "); argv[argc] != NULL; argv[++argc] = strtok(NULL, " "));
	if ( _eval(argv, NULL, 0, NULL) )
	{
		vpnlog(VPN_LOG_ERROR,"Creating tunnel interface %s failed...",&iface[0]);
		stop_vpnserver(serverNum);
		return;
	}

	// Add interface to LAN bridge (TAP only)
	if( ifType == TAP )
	{
		snprintf(&buffer[0], BUF_SIZE, "brctl addif br0 %s", &iface[0]);
		for (argv[argc=0] = strtok(&buffer[0], " "); argv[argc] != NULL; argv[++argc] = strtok(NULL, " "));
		if ( _eval(argv, NULL, 0, NULL) )
		{
			vpnlog(VPN_LOG_ERROR,"Adding tunnel interface to bridge failed...");
			stop_vpnserver(serverNum);
			return;
		}
	}

	// Bring interface up
	sprintf(&buffer[0], "ifconfig %s 0.0.0.0 promisc up", &iface[0]);
	for (argv[argc=0] = strtok(&buffer[0], " "); argv[argc] != NULL; argv[++argc] = strtok(NULL, " "));
	if ( _eval(argv, NULL, 0, NULL) )
	{
		vpnlog(VPN_LOG_ERROR,"Bringing up tunnel interface failed...");
		stop_vpnserver(serverNum);
		return;
	}

	// Build and write config files
	vpnlog(VPN_LOG_EXTRA,"Writing config file");
	sprintf(&buffer[0], "/etc/openvpn/server%d/config.ovpn", serverNum);
	fp = fopen(&buffer[0], "w");
	chmod(&buffer[0], S_IRUSR|S_IWUSR);
	fprintf(fp, "# Automatically generated configuration\n");
	fprintf(fp, "daemon\n");

	sprintf(&buffer[0], "/etc/openvpn/server%d/client.ovpn", serverNum);
	fp_client = fopen(&buffer[0], "w");

	if ( cryptMode == TLS )
	{
		fprintf(fp_client, "client\n");

		if ( ifType == TUN )
		{
			tcapi_get(node, "subnet", &buffer[0]);
			fprintf(fp, "server %s ", &buffer[0]);
			sprintf(&buffer[0], "vpn_server%d_nm", serverNum);
			tcapi_get(node, "netmask", &buffer[0]);
			fprintf(fp, "%s\n", &buffer[0]);

			fprintf(fp_client, "dev tun\n");
		}
		else if ( ifType == TAP )
		{
			fprintf(fp, "server-bridge");
			if ( tcapi_get_int(node, "dhcp") == 0 )
			{
				tcapi_get("Lan_Entry0", "IP", &buffer[0]);
				fprintf(fp, " %s ", &buffer[0]);
				tcapi_get("Lan_Entry0", "netmask", &buffer[0]);
				fprintf(fp, "%s ", &buffer[0]);
				tcapi_get(node, "pool_start", &buffer[0]);
				fprintf(fp, "%s ", &buffer[0]);
				tcapi_get(node, "pool_end", &buffer[0]);
				fprintf(fp, "%s", &buffer[0]);
			}
			else
			{
				fprintf(fp, "\npush \"route 0.0.0.0 255.255.255.255 net_gateway\"");
			}

			fprintf(fp, "\n");

			fprintf(fp_client, "dev tap\n"
				"# Windows needs the TAP-Win32 adapter name\n"
				"# from the Network Connections panel\n"
				"# if you have more than one.  On XP SP2,\n"
				"# you may need to disable the firewall\n"
				"# for the TAP adapter.\n"
				";dev-node MyTap\n"
				);
		}
	}
	else if ( cryptMode == SECRET )
	{
		fprintf(fp_client, "mode p2p\n");

		if ( ifType == TUN )
		{
			tcapi_get(node, "local", &buffer[0]);
			fprintf(fp, "ifconfig %s ", &buffer[0]);
			tcapi_get(node, "remote", &buffer[0]);
			fprintf(fp, "%s\n", &buffer[0]);

			fprintf(fp_client, "dev tun\n");
			tcapi_get(node, "remote", &buffer[0]);
			fprintf(fp_client, "ifconfig %s ", &buffer[0]);
			tcapi_get(node, "local", &buffer[0]);
			fprintf(fp_client, "%s\n", &buffer[0]);
		}
		else if ( ifType == TAP )
		{
			fprintf(fp_client, "dev tap\n");
		}
	}
	fprintf(fp, "dev %s\n", &iface[0]);


	//protocol
	tcapi_get(node, "proto", &buffer[0]);
	fprintf(fp, "proto %s\n", &buffer[0]);
	if(!strcmp(&buffer[0], "udp"))
		fprintf(fp_client, "proto %s\n", &buffer[0]);
	else
		fprintf(fp_client, "proto tcp-client\n");

	//port
	if(tcapi_get_int("Ddns_Entry", "Active"))
	{
		tcapi_get("Ddns_Entry", "MYHOST", &buffer[0]);
		fprintf(fp_client, "remote %s", &buffer[0]);
	}
	else
	{
		sprintf(&buffer[0], "wan%d_ipaddr", tcapi_get_int("Wanduck_Common", "wan_primary"));
		tcapi_get("Wanduck_Common", &buffer[0], &buffer[MAXLEN_NODE_NAME]);
		fprintf(fp_client, "remote %s", &buffer[MAXLEN_NODE_NAME]);
	}
	tcapi_get(node, "port", &buffer[0]);
	fprintf(fp, "port %s\n", &buffer[0]);
	fprintf(fp_client, " %s\n", &buffer[0]);
	fprintf(fp_client, "float\n");

	//cipher
	tcapi_get(node, "cipher", &buffer[0]);
	if ( !find_word(&buffer[0], "default") ) {
		fprintf(fp, "cipher %s\n", &buffer[0]);
		fprintf(fp_client, "cipher %s\n", &buffer[0]);
	}

	//digest
	tcapi_get(node, "digest", &buffer[0]);
	if ( !find_word(&buffer[0], "default") ) {
		fprintf(fp, "auth %s\n", &buffer[0]);
		fprintf(fp_client, "auth %s\n", &buffer[0]);
	}
	
	//compression
	tcapi_get(node, "comp", &buffer[0]);
	if ( atoi(&buffer[0]) >= 0 ) {
		fprintf(fp, "comp-lzo %s\n", &buffer[0]);
		fprintf(fp_client, "comp-lzo %s\n", &buffer[0]);
	}

	fprintf(fp, "keepalive 15 60\n");
	fprintf(fp_client, "keepalive 15 60\n");

	if ( (nvi = tcapi_get_int(node, "loglevel")) >= 0 )
		fprintf(fp, "verb %d\n", nvi);
	else
		fprintf(fp, "verb 3\n");

	if ( cryptMode == TLS )
	{
		//TLS Renegotiation Time
		tcapi_get(node, "reneg", &buffer[0]);
		if ( (nvl = atol(&buffer[0])) >= 0 ) {
			fprintf(fp, "reneg-sec %ld\n", nvl);
			fprintf(fp_client, "reneg-sec %ld\n", nvl);
		}

		tcapi_get(node, "push_lan", &buffer[0]);
		if ( ifType == TUN && atoi(&buffer[0]) )
		{
			tcapi_get("Lan_Entry0", "IP", &buffer[0]);
			sscanf(&buffer[0], "%d.%d.%d.%d", &ip[0], &ip[1], &ip[2], &ip[3]);
			tcapi_get("Lan_Entry0", "netmask", &buffer[0]);
			sscanf(&buffer[0], "%d.%d.%d.%d", &nm[0], &nm[1], &nm[2], &nm[3]);
			fprintf(fp, "push \"route %d.%d.%d.%d %s vpn_gateway %d\"\n",
				ip[0]&nm[0], ip[1]&nm[1], ip[2]&nm[2], ip[3]&nm[3],
				&buffer[0], PUSH_LAN_METRIC);
		}

		tcapi_get(node, "ccd", &buffer[0]);
		if ( atoi(&buffer[0]) )
		{
			fprintf(fp, "client-config-dir ccd\n");

			tcapi_get(node, "c2c", &buffer[0]);
			if ( (c2c = atoi(&buffer[0])) )
				fprintf(fp, "client-to-client\n");

			tcapi_get(node, "ccd_excl", &buffer[0]);
			if ( atoi(&buffer[0]) )
				fprintf(fp, "ccd-exclusive\n");
			else
				fprintf(fp, "duplicate-cn\n");

			sprintf(&buffer[0], "/etc/openvpn/server%d/ccd", serverNum);
			mkdir(&buffer[0], 0700);
			chdir(&buffer[0]);

			tcapi_get_list("vpn_server_ccd_val", &buffer[0], sizeof(buffer));
			chp = strtok(&buffer[0],"<");
			while ( chp != NULL )
			{
				nvi = strlen(chp);

				chp[strcspn(chp,">")] = '\0';
				vpnlog(VPN_LOG_EXTRA,"CCD: enabled: %d", atoi(chp));
				if ( atoi(chp) == 1 )
				{
					nvi -= strlen(chp)+1;
					chp += strlen(chp)+1;

					ccd = NULL;
					route = NULL;
					if ( nvi > 0 )
					{
						chp[strcspn(chp,">")] = '\0';
						vpnlog(VPN_LOG_EXTRA,"CCD: Common name: %s", chp);
						ccd = fopen(chp, "w");
						chmod(chp, S_IRUSR|S_IWUSR);

						nvi -= strlen(chp)+1;
						chp += strlen(chp)+1;
					}
					if ( nvi > 0 && ccd != NULL && strcspn(chp,">") != strlen(chp) )
					{
						chp[strcspn(chp,">")] = ' ';
						chp[strcspn(chp,">")] = '\0';
						route = chp;
						vpnlog(VPN_LOG_EXTRA,"CCD: Route: %s", chp);
						if ( strlen(route) > 1 )
						{
							fprintf(ccd, "iroute %s\n", route);
							fprintf(fp, "route %s\n", route);
						}

						nvi -= strlen(chp)+1;
						chp += strlen(chp)+1;
					}
					if ( ccd != NULL )
						fclose(ccd);
					if ( nvi > 0 && route != NULL )
					{
						chp[strcspn(chp,">")] = '\0';
						vpnlog(VPN_LOG_EXTRA,"CCD: Push: %d", atoi(chp));
						if ( c2c && atoi(chp) == 1 && strlen(route) > 1 )
							fprintf(fp, "push \"route %s\"\n", route);

						nvi -= strlen(chp)+1;
						chp += strlen(chp)+1;
					}

					vpnlog(VPN_LOG_EXTRA,"CCD leftover: %d", nvi+1);
				}
				// Advance to next entry
				chp = strtok(NULL, "<");
			}
			// Copy any user-configured client config files
			sprintf(&buffer[0], "/jffs/configs/openvpn/ccd%d", serverNum);

			if(check_if_dir_exist(&buffer[0]))
			{
				vpnlog(VPN_LOG_EXTRA,"CCD - copying user files");
				sprintf(buffer2, "cp %s/* .", &buffer[0]); /* */
				system(buffer2);
			}

			vpnlog(VPN_LOG_EXTRA,"CCD processing complete");
		}
		else
			fprintf(fp, "duplicate-cn\n");

		tcapi_get(node, "push_dns", &buffer[0]);
		if ( atoi(&buffer[0]) )
		{
			//TODO
			//if ( nvram_safe_get("wan_domain")[0] != '\0' )
				//fprintf(fp, "push \"dhcp-option DOMAIN %s\"\n", nvram_safe_get("wan_domain"));
			tcapi_get("Lan_Entry0", "IP", &buffer[0]);
			fprintf(fp, "push \"dhcp-option DNS %s\"\n", &buffer[0]);
		}

		tcapi_get(node, "redirect_gateway", &buffer[0]);
		if ( atoi(&buffer[0]) )
		{
			if ( ifType == TAP ) {
				tcapi_get("Lan_Entry0", "IP", &buffer[0]);
				fprintf(fp, "push \"route-gateway %s\"\n", &buffer[0]);
			}
			fprintf(fp, "push \"redirect-gateway def1\"\n");
		}

		tcapi_get(node, "hmac", &buffer[0]);
		nvi = atoi(&buffer[0]);
		if ( nvi >= 0 )
		{
			fprintf(fp, "tls-auth static.key");
			if ( nvi < 2 )
				fprintf(fp, " %d", nvi);
			fprintf(fp, "\n");
		}

		tcapi_get(node, "igncrt", &buffer[0]);
		useronly = atoi(&buffer[0]);

		//authentication
		fprintf(fp, "plugin /usr/lib/openvpn-plugin-auth-pam.so openvpn\n");
		fprintf(fp_client, "auth-user-pass\n");

		//ignore client certificate
		if ( useronly ) {
			fprintf(fp, "client-cert-not-required\n");
			fprintf(fp, "username-as-common-name\n");
		}

		fprintf(fp_client, "ns-cert-type server\n");

		fprintf(fp, "ca ca.crt\n");
		fprintf(fp, "dh dh.pem\n");
		fprintf(fp, "cert server.crt\n");
		fprintf(fp, "key server.key\n");
		sprintf(&buffer[0], "vpn_crt_server%d_crl", serverNum);
		if ( !ovpn_crt_is_empty(&buffer[0]) )
			fprintf(fp, "crl-verify crl.pem\n");
	}
	else if ( cryptMode == SECRET )
	{
		fprintf(fp, "secret static.key\n");
	}

	if (check_if_file_exist("/jffs/scripts/openvpn-event"))
	{
		sprintf(&buffer[0], "/etc/openvpn/server%d/updown.sh", serverNum);
		symlink("/jffs/scripts/openvpn-event", &buffer[0]);
		fprintf(fp, "script-security 2\n");
		fprintf(fp, "up updown.sh\n");
		fprintf(fp, "down updown.sh\n");
	}

	fprintf(fp, "status-version 2\n");
	fprintf(fp, "status status 10\n");
	fprintf(fp, "\n# Custom Configuration\n");
	memset(buffer, 0, sizeof(buffer));
	tcapi_get_multiattr(node, "custom", &buffer[0], sizeof(buffer));
	fprintf(fp, "%s", &buffer[0]);
	fclose(fp);

	vpnlog(VPN_LOG_EXTRA,"Done writing server config file");

	// Write certification and key files
	vpnlog(VPN_LOG_EXTRA,"Writing certs/keys");
	if ( cryptMode == TLS )
	{
		//generate certification and key
		sprintf(nv1, "vpn_crt_server%d_ca", serverNum);
		sprintf(nv2, "vpn_crt_server%d_key", serverNum);
		sprintf(nv3, "vpn_crt_server%d_crt", serverNum);
		if( ovpn_crt_is_empty(nv1) || ovpn_crt_is_empty(nv2) || ovpn_crt_is_empty(nv3) ) {
			sprintf(fpath, "/tmp/genvpncert.sh");
			fp = fopen(fpath, "w");
			if(fp) {
				fprintf(fp, "#!/bin/sh\n");
				//fprintf(fp, ". /rom/easy-rsa/vars\n");
				fprintf(fp, "export OPENSSL=\"openssl\"\n");
				fprintf(fp, "export GREP=\"grep\"\n");
				fprintf(fp, "export KEY_CONFIG=\"/rom/easy-rsa/openssl-1.0.0.cnf\"\n");
				fprintf(fp, "export KEY_DIR=\"/etc/openvpn/server%d\"\n", serverNum);
				fprintf(fp, "export KEY_SIZE=1024\n");
				fprintf(fp, "export CA_EXPIRE=3650\n");
				fprintf(fp, "export KEY_EXPIRE=3650\n");
				fprintf(fp, "export KEY_COUNTRY=\"TW\"\n");
				fprintf(fp, "export KEY_PROVINCE=\"TW\"\n");
				fprintf(fp, "export KEY_CITY=\"Taipei\"\n");
				fprintf(fp, "export KEY_ORG=\"ASUS\"\n");
				fprintf(fp, "export KEY_EMAIL=\"me@myhost.mydomain\"\n");
				fprintf(fp, "export KEY_CN=\"%s\"\n", nvram_safe_get("productid"));
				fprintf(fp, "touch /etc/openvpn/server%d/index.txt\n", serverNum);
				fprintf(fp, "echo 01 >/etc/openvpn/server%d/serial\n", serverNum);
				fprintf(fp, "/rom/easy-rsa/pkitool --initca\n");
				fprintf(fp, "/rom/easy-rsa/pkitool --server server\n");

				//undefined common name, default use username-as-common-name
				fprintf(fp, "export KEY_CN=\"\"\n");
				fprintf(fp, "/rom/easy-rsa/pkitool client\n");

				fclose(fp);
				chmod(fpath, 0700);
				eval(fpath);
				unlink(fpath);
			}

			//set certification and key to nvram
			sprintf(fpath, "/etc/openvpn/server%d/ca.key", serverNum);
			fp = fopen(fpath, "r");
			if(fp) {
				sprintf(&buffer[0], "vpn_crt_server%d_ca_key", serverNum);
				set_crt_parsed(&buffer[0], fpath);
				fclose(fp);
			}
			sprintf(fpath, "/etc/openvpn/server%d/ca.crt", serverNum);
			fp = fopen(fpath, "r");
			if(fp) {
				set_crt_parsed(nv1, fpath);
				fclose(fp);
			}
			sprintf(fpath, "/etc/openvpn/server%d/server.key", serverNum);
			fp = fopen(fpath, "r");
			if(fp) {
				set_crt_parsed(nv2, fpath);
				fclose(fp);
			}
			sprintf(fpath, "/etc/openvpn/server%d/server.crt", serverNum);
			fp = fopen(fpath, "r");
			if(fp) {
				set_crt_parsed(nv3, fpath);
				fclose(fp);
			}
			sprintf(fpath, "/etc/openvpn/server%d/client.key", serverNum);
			fp = fopen(fpath, "r");
			if(fp) {
				sprintf(&buffer[0], "vpn_crt_server%d_client_key", serverNum);
				set_crt_parsed(&buffer[0], fpath);
				fclose(fp);
			}
			sprintf(fpath, "/etc/openvpn/server%d/client.crt", serverNum);
			fp = fopen(fpath, "r");
			if(fp) {
				sprintf(&buffer[0], "vpn_crt_server%d_client_crt", serverNum);
				set_crt_parsed(&buffer[0], fpath);
				fclose(fp);
			}
		}
		else {
				sprintf(&buffer[0], "/etc/openvpn/server%d/ca.key", serverNum);
				fp = fopen(&buffer[0], "w");
				chmod(&buffer[0], S_IRUSR|S_IWUSR);
				sprintf(&buffer[0], "vpn_crt_server%d_ca_key", serverNum);
				fprintf(fp, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
				fclose(fp);

				sprintf(&buffer[0], "/etc/openvpn/server%d/ca.crt", serverNum);
				fp = fopen(&buffer[0], "w");
				chmod(&buffer[0], S_IRUSR|S_IWUSR);
				sprintf(&buffer[0], "vpn_crt_server%d_ca", serverNum);
				fprintf(fp, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
				fclose(fp);

				sprintf(&buffer[0], "/etc/openvpn/server%d/server.key", serverNum);
				fp = fopen(&buffer[0], "w");
				chmod(&buffer[0], S_IRUSR|S_IWUSR);
				sprintf(&buffer[0], "vpn_crt_server%d_key", serverNum);
				fprintf(fp, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
				fclose(fp);

				sprintf(&buffer[0], "/etc/openvpn/server%d/server.crt", serverNum);
				fp = fopen(&buffer[0], "w");
				chmod(&buffer[0], S_IRUSR|S_IWUSR);
				sprintf(&buffer[0], "vpn_crt_server%d_crt", serverNum);
				fprintf(fp, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
				fclose(fp);

			sprintf(&buffer[0], "vpn_crt_server%d_crl", serverNum);
			if ( !ovpn_crt_is_empty(&buffer[0]) )
			{
				sprintf(&buffer[0], "/etc/openvpn/server%d/crl.pem", serverNum);
				fp = fopen(&buffer[0], "w");
				chmod(&buffer[0], S_IRUSR|S_IWUSR);
				sprintf(&buffer[0], "vpn_crt_server%d_crl", serverNum);
				fprintf(fp, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
				fclose(fp);
			}
		}

		sprintf(&buffer[0], "vpn_crt_server%d_ca", serverNum);
		fprintf(fp_client, "<ca>\n");
		fprintf(fp_client, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
		if (buffer2[strlen(buffer2)-1] != '\n') fprintf(fp_client, "\n");	// Append newline if missing
		fprintf(fp_client, "</ca>\n");

		//if(!useronly) {
			/*
			 * See if stored client cert was signed with our stored CA.  If not, it means
			 * the CA was changed by the user and the current client crt/key no longer match,
			 * so we should not insert them in the exported client ovpn file.
			 */
			fp = fopen("/tmp/test.crt", "w");
			sprintf(&buffer[0], "vpn_crt_server%d_client_crt", serverNum);
			fprintf(fp, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
			fclose(fp);

			sprintf(&buffer[0], "openssl verify -CAfile /etc/openvpn/server%d/ca.crt /tmp/test.crt > /tmp/output.txt", serverNum);
			system(&buffer[0]);
			f_read_string("/tmp/output.txt", &buffer[0], 64);
			unlink("/tmp/test.crt");

			if (!strncmp(&buffer[0],"/tmp/test.crt: OK",17))
				valid = 1;

			fprintf(fp_client, "<cert>\n");
			sprintf(&buffer[0], "vpn_crt_server%d_client_crt", serverNum);
			if ((valid == 1) && ( !ovpn_crt_is_empty(&buffer[0]) ) ) {
				fprintf(fp_client, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
				if (buffer2[strlen(buffer2)-1] != '\n') fprintf(fp_client, "\n");  // Append newline if missing
			} else {
				fprintf(fp_client, "    paste client certificate data here\n");
			}
			fprintf(fp_client, "</cert>\n");

			fprintf(fp_client, "<key>\n");
			sprintf(&buffer[0], "vpn_crt_server%d_client_key", serverNum);
			if ((valid == 1) && ( !ovpn_crt_is_empty(&buffer[0]) ) ) {
				fprintf(fp_client, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
				if (buffer2[strlen(buffer2)-1] != '\n') fprintf(fp_client, "\n");  // Append newline if missing
			} else {
				fprintf(fp_client, "    paste client key data here\n");
			}
			fprintf(fp_client, "</key>\n");
		//}

		valid = 0;
		sprintf(&buffer[0], "vpn_crt_server%d_dh", serverNum);
		if ( !ovpn_crt_is_empty(&buffer[0]) )
		{
			sprintf(&buffer[0], "/etc/openvpn/server%d/dh.pem", serverNum);
			fp = fopen(&buffer[0], "w");
			chmod(&buffer[0], S_IRUSR|S_IWUSR);
			sprintf(&buffer[0], "vpn_crt_server%d_dh", serverNum);
			fprintf(fp, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
			fclose(fp);
			//verify dh key size
			sprintf(&buffer[0], "openssl dh -in /etc/openvpn/server%d/dh.pem -text |grep \"DH Parameters\" > /tmp/output.txt", serverNum);
			system(&buffer[0]);
			if(f_read_string("/tmp/output.txt", &buffer[0], 64)) {
				if(sscanf(strstr(&buffer[0], "DH Parameters"), "DH Parameters: (%d bit)", &dhkeysize)) {
					//_dprintf("dhkeysize = %d\n", dhkeysize);
					if(dhkeysize >= 768)
						valid = 1;
					else
						logmessage("openvpn", "DH parameter will be replaced since key size %d is too small", dhkeysize);
				}
			}
			unlink("/tmp/output.txt");
		}
		if(valid == 0)
		{	//generate dh param file
			sprintf(fpath, "/etc/openvpn/server%d/dh.pem", serverNum);
			//eval("openssl", "dhparam", "-out", fpath, "1024");
			eval("cp", "/rom/dh2048.pem", fpath);
			fp = fopen(fpath, "r");
			if(fp) {
				sprintf(&buffer[0], "vpn_crt_server%d_dh", serverNum);
				set_crt_parsed(&buffer[0], fpath);
				fclose(fp);
			}
		}
	}
	tcapi_get(node, "hmac", &buffer[0]);
	if ( cryptMode == SECRET || (cryptMode == TLS && atoi(&buffer[0]) >= 0) )
	{
		sprintf(&buffer[0], "vpn_crt_server%d_static", serverNum);
		if ( !ovpn_crt_is_empty(&buffer[0]) )
		{
			sprintf(&buffer[0], "/etc/openvpn/server%d/static.key", serverNum);
			fp = fopen(&buffer[0], "w");
			chmod(&buffer[0], S_IRUSR|S_IWUSR);
			sprintf(&buffer[0], "vpn_crt_server%d_static", serverNum);
			fprintf(fp, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
			fclose(fp);
		}
		else
		{	//generate openvpn static key
			sprintf(fpath, "/etc/openvpn/server%d/static.key", serverNum);
			eval("openvpn", "--genkey", "--secret", fpath);
			fp = fopen(fpath, "r");
			if(fp) {
				set_crt_parsed(&buffer[0], fpath);
				fclose(fp);
			}
		}

		sprintf(&buffer[0], "vpn_crt_server%d_static", serverNum);
		if(cryptMode == TLS)
			fprintf(fp_client, "<tls-auth>\n");
		else if(cryptMode == SECRET)
			fprintf(fp_client, "<secret>\n");
		fprintf(fp_client, "%s", get_parsed_crt(&buffer[0], buffer2, sizeof(buffer2)));
		if (buffer2[strlen(buffer2)-1] != '\n') fprintf(fp_client, "\n");  // Append newline if missing
		if(cryptMode == TLS) {
			fprintf(fp_client, "</tls-auth>\n");
			tcapi_get(node, "hmac", &buffer[0]);
			nvi = atoi(&buffer[0]);
			if(nvi == 1)
				fprintf(fp_client, "key-direction 0\n");
			else if(nvi == 0)
				fprintf(fp_client, "key-direction 1\n");
		}
		else if(cryptMode == SECRET)
			fprintf(fp_client, "</secret>\n");
	}
	vpnlog(VPN_LOG_EXTRA,"Done writing certs/keys");
	//nvram_commit();

	fprintf(fp_client, "resolv-retry infinite\n");
	fprintf(fp_client, "nobind\n");
	fclose(fp_client);
	vpnlog(VPN_LOG_EXTRA,"Done writing client config file");

	sprintf(&buffer[0], "/etc/openvpn/vpnserver%d --cd /etc/openvpn/server%d --config config.ovpn", serverNum, serverNum);
	vpnlog(VPN_LOG_INFO,"Starting OpenVPN: %s",&buffer[0]);
	for (argv[argc=0] = strtok(&buffer[0], " "); argv[argc] != NULL; argv[++argc] = strtok(NULL, " "));
	if ( _eval(argv, NULL, 0, &pid) )
	{
		vpnlog(VPN_LOG_ERROR,"Starting VPN instance failed...");
		stop_vpnserver(serverNum);
		return;
	}
	vpnlog(VPN_LOG_EXTRA,"Done starting openvpn");

	// Handle firewall rules if appropriate
	tcapi_get(node, "firewall", &buffer[0]);
	if ( !find_word(&buffer[0], "custom") )
	{
		// Create firewall rules
		vpnlog(VPN_LOG_EXTRA,"Creating firewall rules");
		mkdir("/etc/openvpn/fw", 0700);
		sprintf(&buffer[0], "/etc/openvpn/fw/server%d-fw.sh", serverNum);
		fp = fopen(&buffer[0], "w");
		chmod(&buffer[0], S_IRUSR|S_IWUSR|S_IXUSR);
		fprintf(fp, "#!/bin/sh\n");
		tcapi_get(node, "proto", &buffer[0]);
		fprintf(fp, "iptables -t nat -I PREROUTING -p %s ", strtok(&buffer[0], "-"));
		tcapi_get(node, "port", &buffer[0]);
		fprintf(fp, "--dport %d -j ACCEPT\n", atoi(&buffer[0]));
		tcapi_get(node, "proto", &buffer[0]);
		fprintf(fp, "iptables -I INPUT -p %s ", strtok(&buffer[0], "-"));
		tcapi_get(node, "port", &buffer[0]);
		fprintf(fp, "--dport %d -j ACCEPT\n", atoi(&buffer[0]));
		tcapi_get(node, "firewall", &buffer[0]);
		if ( !find_word(&buffer[0], "external") )
		{
			fprintf(fp, "iptables -I INPUT -i %s -j ACCEPT\n", &iface[0]);
			fprintf(fp, "iptables -I FORWARD -i %s -j ACCEPT\n", &iface[0]);
		}
		fclose(fp);
		vpnlog(VPN_LOG_EXTRA,"Done creating firewall rules");

		// Run the firewall rules
		vpnlog(VPN_LOG_EXTRA,"Running firewall rules");
		sprintf(&buffer[0], "/etc/openvpn/fw/server%d-fw.sh", serverNum);
		argv[0] = &buffer[0];
		argv[1] = NULL;
		_eval(argv, NULL, 0, NULL);
		vpnlog(VPN_LOG_EXTRA,"Done running firewall rules");
	}

	// Set up cron job
	tcapi_get(node, "poll", &buffer[0]);
	if ( (nvi = atoi(&buffer[0])) > 0 )
	{
		vpnlog(VPN_LOG_EXTRA,"Adding cron job");
		argv[0] = "cru";
		argv[1] = "a";
		sprintf(&buffer[0], "CheckVPNServer%d", serverNum);
		argv[2] = &buffer[0];
		sprintf(&buffer[strlen(&buffer[0])+1], "*/%d * * * * service start_vpnserver%d", nvi, serverNum);
		argv[3] = &buffer[strlen(&buffer[0])+1];
		argv[4] = NULL;
		_eval(argv, NULL, 0, NULL);
		vpnlog(VPN_LOG_EXTRA,"Done adding cron job");
	}

	if ( cryptMode == SECRET || cryptMode == CUSTOM)
	{
		tcapi_set(node, "state", "2");	//running
		tcapi_set(node, "errno", "0");
	}

#ifdef LINUX26
//	sprintf(&buffer[0], "vpn_server%d", serverNum);
//	allow_fastnat(buffer, 0);
//	try_enabling_fastnat();
#endif

	vpnlog(VPN_LOG_INFO,"VPN GUI server backend complete.");
}

void stop_vpnserver(int serverNum)
{
	int argc;
	char *argv[9];
	char buffer[BUF_SIZE];

	sprintf(&buffer[0], "stop_vpnserver%d", serverNum);

	vpnlog(VPN_LOG_INFO,"Stopping VPN GUI server backend.");

	// Remove cron job
	vpnlog(VPN_LOG_EXTRA,"Removing cron job");
	argv[0] = "cru";
	argv[1] = "d";
	sprintf(&buffer[0], "CheckVPNServer%d", serverNum);
	argv[2] = &buffer[0];
	argv[3] = NULL;
	_eval(argv, NULL, 0, NULL);
	vpnlog(VPN_LOG_EXTRA,"Done removing cron job");

	// Remove firewall rules
	vpnlog(VPN_LOG_EXTRA,"Removing firewall rules.");
	sprintf(&buffer[0], "/etc/openvpn/fw/server%d-fw.sh", serverNum);
	argv[0] = "sed";
	argv[1] = "-i";
	argv[2] = "s/-A/-D/g;s/-I/-D/g";
	argv[3] = &buffer[0];
	argv[4] = NULL;
	if (!_eval(argv, NULL, 0, NULL))
	{
		argv[0] = &buffer[0];
		argv[1] = NULL;
		_eval(argv, NULL, 0, NULL);
	}
	vpnlog(VPN_LOG_EXTRA,"Done removing firewall rules.");

	// Stop the VPN server
	vpnlog(VPN_LOG_EXTRA,"Stopping OpenVPN server.");
	sprintf(&buffer[0], "vpnserver%d", serverNum);
	if ( !ovpn_waitfor(&buffer[0]) )
		vpnlog(VPN_LOG_EXTRA,"OpenVPN server stopped.");

	// NVRAM setting for device type could have changed, just try to remove both
	vpnlog(VPN_LOG_EXTRA,"Removing VPN device.");
	sprintf(&buffer[0], "openvpn --rmtun --dev tap%d", serverNum+SERVER_IF_START);
	for (argv[argc=0] = strtok(&buffer[0], " "); argv[argc] != NULL; argv[++argc] = strtok(NULL, " "));
	_eval(argv, NULL, 0, NULL);

	sprintf(&buffer[0], "openvpn --rmtun --dev tun%d", serverNum+SERVER_IF_START);
	for (argv[argc=0] = strtok(&buffer[0], " "); argv[argc] != NULL; argv[++argc] = strtok(NULL, " "));
	_eval(argv, NULL, 0, NULL);
	vpnlog(VPN_LOG_EXTRA,"VPN device removed.");

	modprobe_r("tun");

	if ( tcapi_get_int(OPENVPN_COMMON_NODE,"vpn_debug") <= VPN_LOG_EXTRA )
	{
		vpnlog(VPN_LOG_EXTRA,"Removing generated files.");
		// Delete all files for this server
		sprintf(&buffer[0], "rm -rf /etc/openvpn/server%d /etc/openvpn/fw/server%d-fw.sh /etc/openvpn/vpnserver%d",serverNum,serverNum,serverNum);
		for (argv[argc=0] = strtok(&buffer[0], " "); argv[argc] != NULL; argv[++argc] = strtok(NULL, " "));
		_eval(argv, NULL, 0, NULL);

		// Attempt to remove directories.  Will fail if not empty
		rmdir("/etc/openvpn/fw");
		rmdir("/etc/openvpn");
		vpnlog(VPN_LOG_EXTRA,"Done removing generated files.");
	}

#ifdef LINUX26
//	sprintf(&buffer[0], "vpn_server%d", serverNum);
//	allow_fastnat(buffer, 1);
//	try_enabling_fastnat();
#endif

	//update status
	if(serverNum)
		sprintf(buffer, "OpenVPN_Entry%d", serverNum+SERVER_IF_START);
	else
		sprintf(buffer, "OpenVPN_Entry21");
	tcapi_set(buffer, "state", "0");
	tcapi_set(buffer, "errno", "0");

	vpnlog(VPN_LOG_INFO,"VPN GUI server backend stopped.");
}

void start_vpn_eas()
{
	char buffer[16], *cur;
	int nums[5], i;

	// wait for time sync for a while
	i = 10;
	while (time(0) < 1325376000 && i--) {
		sleep(1);
	}

	// Parse and start servers
	if(tcapi_get_int("VPNControl_Entry", "VPNServer_enable"))
	{
		memset(buffer, 0, sizeof(buffer));
		tcapi_get("OpenVPN_Common", "vpn_serverx_eas", buffer);
		if( strlen(&buffer[0]) != 0 ) vpnlog(VPN_LOG_INFO, "Starting OpenVPN servers (eas): %s", &buffer[0]);
		i = 0;
		for( cur = strtok(&buffer[0],","); cur != NULL && i < 5; cur = strtok(NULL, ",")) { nums[i++] = atoi(cur); }
		if(i < 5) nums[i] = 0;
		for( i = 0; nums[i] > 0 && i < 5; i++ )
		{
			sprintf(&buffer[0], "vpnserver%d", nums[i]);
			if ( pidof(&buffer[0]) > 0 )
			{
				vpnlog(VPN_LOG_INFO, "Stopping OpenVPN server %d (eas)", nums[i]);
				stop_vpnserver(nums[i]);
			}

			vpnlog(VPN_LOG_INFO, "Starting OpenVPN server %d (eas)", nums[i]);
			start_vpnserver(nums[i]);
		}
	}

	// Parse and start clients
	memset(buffer, 0, sizeof(buffer));
	tcapi_get("OpenVPN_Common", "vpn_clientx_eas", buffer);
	if( strlen(&buffer[0]) != 0 ) vpnlog(VPN_LOG_INFO, "Starting clients (eas): %s", &buffer[0]);
	i = 0;
	for( cur = strtok(&buffer[0],","); cur != NULL && i < 5; cur = strtok(NULL, ",")) { nums[i++] = atoi(cur); }
	if(i < 5) nums[i] = 0;
	for( i = 0; nums[i] > 0 && i < 5; i++ )
	{
		sprintf(&buffer[0], "vpnclient%d", nums[i]);
		if ( pidof(&buffer[0]) > 0 )
		{
			vpnlog(VPN_LOG_INFO, "Stopping OpenVPN client %d (eas)", nums[i]);
			stop_vpnclient(nums[i]);
		}

		vpnlog(VPN_LOG_INFO, "Starting OpenVPN client %d (eas)", nums[i]);
		start_vpnclient(nums[i]);
	}
}

void stop_vpn_eas()
{
	char buffer[16], *cur;
	int nums[4], i;

	// Parse and stop servers
	memset(buffer, 0, sizeof(buffer));
	tcapi_get("OpenVPN_Common", "vpn_serverx_eas", buffer);
	if ( strlen(&buffer[0]) != 0 ) vpnlog(VPN_LOG_INFO, "Stopping OpenVPN servers (eas): %s", &buffer[0]);
	i = 0;
	for( cur = strtok(&buffer[0],","); cur != NULL && i < 4; cur = strtok(NULL, ",")) { nums[i++] = atoi(cur); }
	nums[i] = 0;
	for( i = 0; nums[i] > 0; i++ )
	{
		sprintf(&buffer[0], "vpnserver%d", nums[i]);
		if ( pidof(&buffer[0]) > 0 )
		{
			vpnlog(VPN_LOG_INFO, "Stopping OpenVPN server %d (eas)", nums[i]);
			stop_vpnserver(nums[i]);
		}
	}

	// Parse and stop clients
	memset(buffer, 0, sizeof(buffer));
	tcapi_get("OpenVPN_Common", "vpn_clientx_eas", buffer);
	if ( strlen(&buffer[0]) != 0 ) vpnlog(VPN_LOG_INFO, "Stopping OpenVPN clients (eas): %s", &buffer[0]);
	i = 0;
	for( cur = strtok(&buffer[0],","); cur != NULL && i < 4; cur = strtok(NULL, ",")) { nums[i++] = atoi(cur); }
	nums[i] = 0;
	for( i = 0; nums[i] > 0; i++ )
	{
		sprintf(&buffer[0], "vpnclient%d", nums[i]);
		if ( pidof(&buffer[0]) > 0 )
		{
			vpnlog(VPN_LOG_INFO, "Stopping OpenVPN client %d (eas)", nums[i]);
			stop_vpnclient(nums[i]);
		}
	}
}

void run_vpn_firewall_scripts()
{
	DIR *dir;
	struct dirent *file;
	char *fn;
	char *argv[3];

	if ( chdir("/etc/openvpn/fw") )
		return;

	dir = opendir("/etc/openvpn/fw");

	vpnlog(VPN_LOG_EXTRA,"Beginning all firewall scripts...");
	while ( (file = readdir(dir)) != NULL )
	{
		fn = file->d_name;
		if ( fn[0] == '.' )
			continue;
		vpnlog(VPN_LOG_INFO,"Running firewall script: %s", fn);
		argv[0] = "/bin/sh";
		argv[1] = fn;
		argv[2] = NULL;
		_eval(argv, NULL, 0, NULL);
	}
	vpnlog(VPN_LOG_EXTRA,"Done with all firewall scripts...");

	closedir(dir);
}

void write_vpn_dnsmasq_config(FILE* f)
{
	char nv[16];
	char buf[24];
	char *pos, ch;
	int cur;
	DIR *dir;
	struct dirent *file;
	FILE *dnsf;

	strlcpy(&buf[0], nvram_safe_get("vpn_serverx_dns"), sizeof(buf));
	for ( pos = strtok(&buf[0],","); pos != NULL; pos=strtok(NULL, ",") )
	{
		cur = atoi(pos);
		if ( cur )
		{
			vpnlog(VPN_LOG_EXTRA, "Adding server %d interface to dns config", cur);
			snprintf(&nv[0], sizeof(nv), "vpn_server%d_if", cur);
			fprintf(f, "interface=%s%d\n", nvram_safe_get(&nv[0]), SERVER_IF_START+cur);
		}
	}

	if ( (dir = opendir("/etc/openvpn/dns")) != NULL )
	{
		while ( (file = readdir(dir)) != NULL )
		{
			if ( file->d_name[0] == '.' )
				continue;

			if ( sscanf(file->d_name, "client%d.resol%c", &cur, &ch) == 2 )
			{
				vpnlog(VPN_LOG_EXTRA, "Checking ADNS settings for client %d", cur);
				snprintf(&buf[0], sizeof(buf), "vpn_client%d_adns", cur);
				if ( nvram_get_int(&buf[0]) == 2 )
				{
					vpnlog(VPN_LOG_INFO, "Adding strict-order to dnsmasq config for client %d", cur);
					fprintf(f, "strict-order\n");
					break;
				}
			}

			if ( sscanf(file->d_name, "client%d.con%c", &cur, &ch) == 2 )
			{
				if ( (dnsf = fopen(file->d_name, "r")) != NULL )
				{
					vpnlog(VPN_LOG_INFO, "Adding Dnsmasq config from %s", file->d_name);

					while( !feof(dnsf) )
					{
						ch = fgetc(dnsf);
						fputc(ch==EOF?'\n':ch, f);
					}

					fclose(dnsf);
				}
			}
		}
	}
}

int write_vpn_resolv(FILE* f)
{
	DIR *dir;
	struct dirent *file;
	char *fn, ch, num, buf[24];
	FILE *dnsf;
	int exclusive = 0;

	if ( chdir("/etc/openvpn/dns") )
		return 0;

	dir = opendir("/etc/openvpn/dns");

	vpnlog(VPN_LOG_EXTRA, "Adding DNS entries...");
	while ( (file = readdir(dir)) != NULL )
	{
		fn = file->d_name;

		if ( fn[0] == '.' )
			continue;

		if ( sscanf(fn, "client%c.resol%c", &num, &ch) == 2 )
		{
			if ( (dnsf = fopen(fn, "r")) == NULL )
				continue;

			vpnlog(VPN_LOG_INFO,"Adding DNS entries from %s", fn);

			while( !feof(dnsf) )
			{
				ch = fgetc(dnsf);
				fputc(ch==EOF?'\n':ch, f);
			}

			fclose(dnsf);

			snprintf(&buf[0], sizeof(buf), "vpn_client%c_adns", num);
			if ( nvram_get_int(&buf[0]) == 3 )
				exclusive = 1;
		}
	}
	vpnlog(VPN_LOG_EXTRA, "Done with DNS entries...");

	closedir(dir);

	return exclusive;
}

void create_openvpn_passwd()
{
	unsigned char s[512];
	char *p;
	char salt[32];
	FILE *fp;
	openvpn_accnt_info_t account_info;
	int id = 200;
	int i;
	
	strcpy(salt, "$1$");
	f_read("/dev/urandom", s, 6);
	base64_encode(s, salt + 3, 6);
	salt[3 + 8] = 0;
	p = salt;
	while (*p) {
		if (*p == '+') *p = '.';
		++p;
	}

	fp=fopen("/etc/passwd.openvpn", "w");
	if (!fp) return;

	bzero(&account_info, sizeof(account_info));
	get_openvpn_account(&account_info);

	for (i = 0; i <account_info.count; i++)
	{
		p = crypt(account_info.account[i].passwd, salt);
		fprintf(fp, "%s:%s:%d:%d:::\n", account_info.account[i].name, p, id, id);
		id++;
	}

	fclose(fp);
}

static int _is_dup_vpnaccount(char *account_str)
{
	int is_duplicate = 0;
	int i;
	openvpn_accnt_info_t account_info;

	if ((account_str == NULL))
	{
		return 0;
	}

	get_openvpn_account(&account_info);

	for (i=0; i<account_info.count; i++)
	{
		//printf("%s-account(%s)-(%s)\n", __FUNCTION__, account_str, username);
		if (!strcmp(account_str, account_info.account[i].name))
		{
			is_duplicate = 1;
			break;
		}
	}

	return is_duplicate;
}

/*
	work around, OpenVpn server account would duplicate with system and samba
	need to deal with it before a formal account management function is
	implmented, or OpenVpn account won't work..

	Samba account only need an entry, so when account is duplicate with openvpn
	account, replaced with OpenVPN account
*/
void add_openvpn_account(const char *path, const char *fname)
{
	char tmpfile[256];
	char line[512];
	char account[32];
	FILE *f;
	FILE *f_tmp;

	if ((path == NULL) || (fname == NULL))
	{
		return;
	}

	if ((f_exists(path) == 0) || (f_exists(fname) == 0))
	{
		return;
	}

	bzero(tmpfile, sizeof(tmpfile));
	snprintf(tmpfile, sizeof(tmpfile), "%s.tmp", path);
	if ((f = fopen(path, "r")) != NULL)
	{
		if ((f_tmp = fopen(tmpfile, "w+")) != NULL)
		{
			//first line for admin account
			if (fgets(line, sizeof(line), f))
			{
				fputs(line, f_tmp);
			}
			
			while (fgets(line, sizeof(line), f))
			{
				if (sscanf(line, "%31[^:]:%*s", account) == 1)
				{
					//printf("%s-account(%s)\n", __FUNCTION__, account);
					if (_is_dup_vpnaccount(account) == 0)
					{
						fputs(line, f_tmp);
					}
				}
				else
				{
					fputs(line, f_tmp);
				}
			}
		}
		else
		{
			fclose(f);
			return;
		}

		fclose(f);
		fclose(f_tmp);

		unlink(path);
		rename(tmpfile, path);
	}
	else
	{
		return;
	}

	if ((f = fopen(path, "a")) != NULL) 
	{
		fappend(f, fname);
		fclose(f);
	}
}

