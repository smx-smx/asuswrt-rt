#include "rc.h"
#include <arpa/inet.h>

#define GATEWAY_FILE	"/etc/%s_gateway.conf"
#define RESOLV_FILE	"/etc/resolv_%s.conf"
#define LEASE_FILE	"/tmp/udhcpc%d.lease"

static int
expires(char *wan_ifname, unsigned int in)
{
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	int unit, wan_unit, wan_subunit;
	time_t now;
	FILE *fp;

	if(get_wan_unit_ex(wan_ifname, &wan_unit, &wan_subunit) < 0)
		return -1;
	if(wan_subunit > 0)
		unit = wan_unit*10 + wan_subunit;
	else
		unit = wan_unit;
	snprintf(prefix, sizeof(prefix), "wan%d_", unit);

	//called by udhcpc_expires(), need to set lease
	nvram_set_int(strcat_r(prefix, "lease", tmp), in);

	//DeviceInfo_PVCx still use this file to get information.
	//Remove if DeviceInfo is deprecated
	sprintf(tmp, "/tmp/udhcpc%d.expires.uptime", unit);
	if ((fp = fopen(tmp, "w")) == NULL) {
		perror(tmp);
		return errno;
	}
	fprintf(fp, "%d", (unsigned int) uptime() + in);
	fclose(fp);

	nvram_set_int(strcat_r(prefix, "expires", tmp), (unsigned int) uptime() + in);

	snprintf(tmp, sizeof(tmp), "/tmp/udhcpc%d.expires", unit);
	if ((fp = fopen(tmp, "w")) == NULL) {
		perror(tmp);
		return errno;
	}
	time(&now);
	fprintf(fp, "%d", (unsigned int) now + in);
	fclose(fp);

	return 0;
}

int
udhcpc_expires(int argc, char **argv)
{
	if (argc < 3)
		return EINVAL;
	else {
		return expires(argv[1], strtoul(argv[2], NULL, 10));
	}
}

int
udhcpc_bound(int argc, char **argv)
{
	char lan_ip[16] = {0};
	char lan_subnet[16] = {0};
	int wan_unit;
	char prefix[8] = {0};

	if (argc < 4)
		return EINVAL;

	tcapi_get("Lan_Entry0", "IP", lan_ip);
	tcapi_get("Lan_Entry0", "netmask", lan_subnet);

	if ((inet_network(argv[1]) & inet_network(argv[2])) ==
		(inet_network(lan_ip) & inet_network(lan_subnet))) {
dbg("wan ip = %x!\n", inet_network(argv[1]));
dbg("wan subnet = %x!\n", inet_network(argv[2]));
dbg("lan ip = %x!\n", inet_network(lan_ip));
dbg("lan subnet = %x!\n", inet_network(lan_subnet));
		wan_unit = atoi(argv[3]);
		sprintf(prefix, "wan%d_", wan_unit);
		update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_INVALID_IPADDR);
		return 200;
	}

	return 0;
}

/* 
 * deconfig: This argument is used when udhcpc starts, and when a
 * leases is lost. The script should put the interface in an up, but
 * deconfigured state.
*/
static int
deconfig(int zcip)
{
	char *wan_ifname = safe_getenv("interface");
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	int unit = wan_ifunit(wan_ifname);

	/* Figure out nvram variable name prefix for this i/f */
	if (wan_prefix(wan_ifname, prefix) < 0)
		return -1;
	if ((unit < 0)) {
		logmessage(zcip ? "zcip client" : "dhcp client", "skipping resetting IP address to 0.0.0.0");
	} else
		ifconfig(wan_ifname, IFUP, "0.0.0.0", NULL);

	//DeviceInfo_PVCx still use this file to get information.
	//Remove if DeviceInfo is deprecated
	snprintf(tmp, sizeof(tmp), GATEWAY_FILE, wan_ifname);
	unlink(tmp);

	expires(wan_ifname, 0);
	wan_down(wan_ifname);

	/* Skip physical VPN subinterface */
	if (!(unit < 0))
		update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_DHCP_DECONFIG);

	_dprintf("udhcpc:: %s done\n", __FUNCTION__);
	return 0;
}

/*
 * bound: This argument is used when udhcpc moves from an unbound, to
 * a bound state. All of the paramaters are set in enviromental
 * variables, The script should configure the interface, and set any
 * other relavent parameters (default gateway, dns server, etc).
*/
static int
bound(void)
{
	char *wan_ifname = safe_getenv("interface");
	char *value, *gateway;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	char wanprefix[] = "wanXXXXXXXXXX_";
	char route[sizeof("255.255.255.255/255")];
	int unit, ifunit;
	int changed = 0;
	char wan_ipaddr[16] = {0};
	char wan_netmask[16] = {0};
	char wan_gateway[16] = {0};
	char lan_ipaddr[16] = {0};
	char lan_netmask[16] = {0};
	unsigned int lease = 0;
	FILE* fp;

#ifdef DHCP_ZEROCONF
	killall("zcip", SIGTERM);
#endif

	/* Figure out nvram variable name prefix for this i/f */
	if ((ifunit = wan_prefix(wan_ifname, wanprefix)) < 0)
		return -1;
	if ((unit = wan_ifunit(wan_ifname)) < 0)
		snprintf(prefix, sizeof(prefix), "wan%d_x", ifunit);
	else	snprintf(prefix, sizeof(prefix), "wan%d_", ifunit);

	if ((value = getenv("ip"))) {
		strncpy(wan_ipaddr, trim_r(value), sizeof(wan_ipaddr));
		changed = !nvram_match(strcat_r(prefix, "ipaddr", tmp), wan_ipaddr);
		nvram_set(strcat_r(prefix, "ipaddr", tmp), wan_ipaddr);
		//tmp reserve for firmwall.
		tcapi_set("System_Entry", "CurrentWANIP", wan_ipaddr);
	}
	if ((value = getenv("subnet"))) {
		strncpy(wan_netmask, trim_r(value), sizeof(wan_netmask));
		nvram_set(strcat_r(prefix, "netmask", tmp), wan_netmask);
	}
	if ((gateway = getenv("router"))) {
		strncpy(wan_gateway, trim_r(gateway), sizeof(wan_gateway));
		nvram_set(strcat_r(prefix, "gateway", tmp), wan_gateway);

		//reserve for VPNC
		snprintf(tmp, sizeof(tmp), "Wan_PVC%d", ifunit);
		tcapi_set(tmp, "gateway_x", wan_gateway);

		//DeviceInfo_PVCx still use this file to get information.
		//Remove if DeviceInfo is deprecated
		snprintf(tmp, sizeof(tmp), GATEWAY_FILE, wan_ifname);
		fp=fopen(tmp, "w");
		if(fp) {
			fprintf(fp, "gateway=%s", wan_gateway);
			fclose(fp);
		}
	}

	/* ex: android phone, the gateway is the DNS server. */
	if ((value = getenv("dns") ? : gateway)) {
		nvram_set(strcat_r(prefix, "dns", tmp), trim_r(value));

		//reserve for VPNC
		snprintf(tmp, sizeof(tmp), "Wan_PVC%d", ifunit);
		tcapi_set(tmp, "dns_x", trim_r(value));

		//tmp reserve for dnsmasq
		snprintf(tmp, sizeof(tmp), RESOLV_FILE, wan_ifname);
		fp=fopen(tmp, "w");
		if(fp) {
			fprintf(fp, "nameserver %s", value);
			fclose(fp);
		}
	}
	if ((value = getenv("wins")))
		nvram_set(strcat_r(prefix, "wins", tmp), trim_r(value));
	//if ((value = getenv("hostname")))
	//	sethostname(value, strlen(value) + 1);
	if ((value = getenv("domain")))
		nvram_set(strcat_r(prefix, "domain", tmp), trim_r(value));
	if ((value = getenv("lease"))) {
		lease = atoi(value);
		nvram_set_int(strcat_r(prefix, "lease", tmp), lease);
		expires(wan_ifname, lease);

		//DeviceInfo_PVCx still use this file to get information.
		//Remove if DeviceInfo is deprecated
		snprintf(tmp, sizeof(tmp), LEASE_FILE, ifunit);
		fp=fopen(tmp, "w");
		if(fp) {
			fprintf(fp, "%d", lease);
			fclose(fp);
		}
	}

	/* classful static routes */
	if ((value = getenv("routes")))
		nvram_set(strcat_r(prefix, "routes", tmp), trim_r(value));
	/* ms classless static routes */
	if ((value = getenv("msstaticroutes")))
		nvram_set(strcat_r(prefix, "routes_ms", tmp), trim_r(value));
	/* rfc3442 classless static routes */
	if ((value = getenv("staticroutes")))
		nvram_set(strcat_r(prefix, "routes_rfc", tmp), trim_r(value));

	/* rfc3442 could contain gateway
	 * format: "net/size gateway" */
	if (!gateway) {
		foreach(route, nvram_safe_get(strcat_r(prefix, "routes_rfc", tmp)), value) {
			if (gateway) {
				nvram_set(strcat_r(prefix, "gateway", tmp), route);
				break;
			} else
			if (strcmp(route, "0.0.0.0/0") == 0)
				gateway = route;
		}
	}

#ifdef RTCONFIG_IPV6
	if ((value = getenv("ip6rd")) &&
	    (get_ipv6_service() == IPV6_6RD && nvram_match("ipv6_6rd_dhcp", "1"))) {
		char *ptr, *values[4];
		int i;

		ptr = value = strdup(value);
		for (i = 0; value && i < 4; i++)
			values[i] = strsep(&value, " ");
		if (i == 4) {
			nvram_set(strcat_r(wanprefix, "6rd_ip4size", tmp), values[0]);
			nvram_set(strcat_r(wanprefix, "6rd_prefixlen", tmp), values[1]);
			nvram_set(strcat_r(wanprefix, "6rd_prefix", tmp), values[2]);
			nvram_set(strcat_r(wanprefix, "6rd_router", tmp), values[3]);
		}
		free(ptr);
	}
#endif

	// check if the ipaddr is safe to apply
	// only handle one lan instance so far
	// update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_INVALID_IPADDR)
	tcapi_get("Lan_Entry0", "IP", lan_ipaddr);
	tcapi_get("Lan_Entry0", "netmask", lan_netmask);
	if (inet_equal(wan_ipaddr, wan_netmask, lan_ipaddr, lan_netmask)) {
		update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_INVALID_IPADDR);
		return 0;
	}

	/* Clean nat conntrack for this interface,
	 * but skip physical VPN subinterface for PPTP/L2TP */
	if (changed && !(unit < 0))
		ifconfig(wan_ifname, IFUP, "0.0.0.0", NULL);
	ifconfig(wan_ifname, IFUP, wan_ipaddr, wan_netmask);

	wan_up(wan_ifname);

	logmessage("dhcp client", "bound %s via %s during %d seconds.",
		wan_ipaddr, wan_gateway, lease);

	_dprintf("udhcpc:: %s done\n", __FUNCTION__);
	return 0;
}

/*
 * renew: This argument is used when a DHCP lease is renewed. All of
 * the paramaters are set in enviromental variables. This argument is
 * used when the interface is already configured, so the IP address,
 * will not change, however, the other DHCP paramaters, such as the
 * default gateway, subnet mask, and dns server may change.
 */
static int
renew(void)
{
	char *wan_ifname = safe_getenv("interface");
	char *value, *gateway, *dns;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	char wanprefix[] = "wanXXXXXXXXXX_";
	int unit, ifunit;
	int changed = 0;
	FILE* fp;

#ifdef DHCP_ZEROCONF
	killall("zcip", SIGTERM);
#endif
	/* Figure out nvram variable name prefix for this i/f */
	if ((ifunit = wan_prefix(wan_ifname, wanprefix)) < 0)
		return -1;
	if ((unit = wan_ifunit(wan_ifname)) < 0)
		snprintf(prefix, sizeof(prefix), "wan%d_x", ifunit);
	else	snprintf(prefix, sizeof(prefix), "wan%d_", ifunit);

	if ((value = getenv("subnet")) == NULL ||
	    nvram_invmatch(strcat_r(prefix, "netmask", tmp), trim_r(value)))
		return bound();
	if ((gateway = getenv("router")) == NULL ||
	    nvram_invmatch(strcat_r(prefix, "gateway", tmp), trim_r(gateway)))
		return bound();

	/* ex: android phone, the gateway is the DNS server. */
	if ((dns = getenv("dns") ? : gateway)) {
		changed = !nvram_match(strcat_r(prefix, "dns", tmp), trim_r(dns));
		nvram_set(strcat_r(prefix, "dns", tmp), trim_r(dns));

		//reserve for VPNC
		snprintf(tmp, sizeof(tmp), "Wan_PVC%d", ifunit);
		tcapi_set(tmp, "dns_x", trim_r(value));

		//tmp reserve for dnsmasq
		snprintf(tmp, sizeof(tmp), RESOLV_FILE, wan_ifname);
		fp=fopen(tmp, "w");
		if(fp) {
			fprintf(fp, "nameserver %s", value);
			fclose(fp);
		}
	}
	if ((value = getenv("wins")))
		nvram_set(strcat_r(prefix, "wins", tmp), trim_r(value));
	//if ((value = getenv("hostname")))
	//	sethostname(value, strlen(value) + 1);
	if ((value = getenv("domain")))
		nvram_set(strcat_r(prefix, "domain", tmp), trim_r(value));
	if ((value = getenv("lease"))) {
		unsigned int lease = atoi(value);
		nvram_set_int(strcat_r(prefix, "lease", tmp), lease);
		expires(wan_ifname, lease);

		//DeviceInfo_PVCx still use this file to get information.
		//Remove if DeviceInfo is deprecated
		snprintf(tmp, sizeof(tmp), LEASE_FILE, ifunit);
		fp=fopen(tmp, "w");
		if(fp) {
			fprintf(fp, "%d", lease);
			fclose(fp);
		}
	}

	/* Update actual DNS or delayed for DHCP+PPP */
	if (changed) {
		//restart_dnsmasq(wan_ifname, trim_r(dns));
		start_dnsmasq();
	}

	/* Update connected state and DNS for WEB UI,
	 * but skip physical VPN subinterface */
	if (changed && !(unit < 0))
		update_wan_state(wanprefix, WAN_STATE_CONNECTED, 0);

	_dprintf("udhcpc:: %s done\n", __FUNCTION__);
	return 0;
}

#ifdef DHCP_ZEROCONF
static int
leasefail(void)
{
	char *wan_ifname = safe_getenv("interface");
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	char wanprefix[] = "wanXXXXXXXXXX_";
	int unit, ifunit;

	/* Figure out nvram variable name prefix for this i/f */
	if ((ifunit = wan_prefix(wan_ifname, wanprefix)) < 0)
		return -1;
	if ((unit = wan_ifunit(wan_ifname)) < 0)
		snprintf(prefix, sizeof(prefix), "wan%d_x", ifunit);
	else	snprintf(prefix, sizeof(prefix), "wan%d_", ifunit);

	if ((inet_network(nvram_safe_get(strcat_r(prefix, "ipaddr", tmp))) &
	     inet_network(nvram_safe_get(strcat_r(prefix, "netmask", tmp)))) ==
	     inet_network("169.254.0.0"))
		return 0;

	return start_zcip(wan_ifname);
}
#endif

int
udhcpc_wan(int argc, char **argv)
{
	if(strcmp(argv[1], "leasefail"))
		_dprintf("%s:: %s\n", __FUNCTION__, argv[1]);
	if (!argv[1])
		return EINVAL;
	else if (strstr(argv[1], "deconfig"))
		return deconfig(0);
	else if (strstr(argv[1], "bound"))
		return bound();
	else if (strstr(argv[1], "renew"))
		return renew();
#ifdef DHCP_ZEROCONF
	else if (strstr(argv[1], "leasefail"))
		return leasefail();
#endif
/*	else if (strstr(argv[1], "nak")) */

	return 0;
}

int
start_udhcpc(char *wan_ifname, int unit, pid_t *ppid)
{
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	char pid[sizeof("/var/run/udhcpcXXXXXXXXXX.pid")];
#ifdef RTCONFIG_DSL
	char clientid[sizeof("61:") + (32+32+1)*2];
#endif
	char *value;
	char *dhcp_argv[] = { "udhcpc",
		"-i", wan_ifname,
		"-p", (snprintf(pid, sizeof(pid), "/var/run/udhcpc%d.pid", unit), pid),
		"-s", "/usr/script/udhcpc.sh", //"-s", "/tmp/udhcpc",
		NULL,		/* -t2 */
		NULL,		/* -T5 */
		NULL,		/* -A120 */
		NULL,		/* -b */
		NULL, NULL,	/* -H wan_hostname */
		NULL,		/* -Oroutes */
		NULL,		/* -Ostaticroutes */
		NULL,		/* -Omsstaticroutes */
#ifdef __CONFIG_IPV6__
		NULL,		/* -Oip6rd rfc */
		NULL,		/* -Oip6rd comcast */
#endif
#ifdef RTCONFIG_TMOBILE
		NULL,		/* -Osip */
#endif
#ifdef RTCONFIG_DSL
		NULL, NULL,	/* -x 61:wan_clientid */
#endif
		NULL };
	int index = 7;		/* first NULL */
	int dr_enable;

	/* Use unit */
	snprintf(prefix, sizeof(prefix), "wan%d_", unit);

	#if 0 //we don't support these parameters
	if (nvram_get_int("dhcpc_mode") == 0)
	{
		/* 2 discover packets max (default 3 discover packets) */
		dhcp_argv[index++] = "-t2";
		/* 5 seconds between packets (default 3 seconds) */
		dhcp_argv[index++] = "-T5";
		/* Wait 160 seconds before trying again (default 20 seconds) */
		/* set to 160 to accomodate new timings enforced by Charter cable */
		dhcp_argv[index++] = "-A160";
	}
	#endif

	if (ppid == NULL)
		dhcp_argv[index++] = "-b";

	value = nvram_safe_get(strcat_r(prefix, "hostname", tmp));
	if (*value && is_valid_hostname(value)) {
		dhcp_argv[index++] = "-H";
		dhcp_argv[index++] = value;
	}

	#if 0 //we don't support this parameter
	/* 0: disable, 1: MS routes, 2: RFC routes, 3: Both */
	dr_enable = nvram_get_int("dr_enable_x");
	if (dr_enable != 0) {
		dhcp_argv[index++] = "-O33";		/* routes */
		if (dr_enable & (1 << 0))
			dhcp_argv[index++] = "-O249";	/* "msstaticroutes" */
		if (dr_enable & (1 << 1))
			dhcp_argv[index++] = "-O121";	/* "staticroutes" */
	}
	#endif

#ifdef RTCONFIG_IPV6
	#if 0 //we don't support this parameter
	if (get_ipv6_service() == IPV6_6RD && nvram_match("ipv6_6rd_dhcp", "1")) {
		dhcp_argv[index++] = "-O212";		/* ip6rd rfc */
		dhcp_argv[index++] = "-O150";		/* ip6rd comcast */
	}
	#endif
#endif

#ifdef RTCONFIG_TMOBILE
	#if 0 //we don't support this parameter
	/* request option sip */
	dhcp_argv[index++] = "-O120";
	#endif
#endif

#ifdef RTCONFIG_DSL
	#if 0 //we don't support this parameter
	value = nvram_safe_get(strcat_r(prefix, "clientid", tmp));
	if (*value) {
		char *ptr = clientid;
		ptr += sprintf(ptr, "61:");
		while (*value && (ptr - clientid) < sizeof(clientid) - 2)
			ptr += sprintf(ptr, "%02x", *value++);
		dhcp_argv[index++] = "-x";
		dhcp_argv[index++] = clientid;
	}
	#endif
#endif

	return _eval(dhcp_argv, NULL, 0, ppid);
}
