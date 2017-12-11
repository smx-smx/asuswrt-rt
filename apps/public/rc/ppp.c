#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <ctype.h>

#include "bcmnvram.h"
#include "libtcapi.h"
#include "shutils.h"
#include "rc.h"

/*
* parse ifname to retrieve unit #
*/
int
ppp_ifunit(char *ifname)
{
	int unit;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";

	for (unit = WAN_UNIT_FIRST; unit < WAN_UNIT_MAX; unit++) {
		snprintf(prefix, sizeof(prefix), "wan%d_", unit);
		if (nvram_match(strcat_r(prefix, "pppoe_ifname", tmp), ifname))
			return unit;
	}

	return -1;
}

int
ppp_linkunit(char *linkname)
{
	int unit = -1;
	if (strncmp(linkname, "ppp", 3))
		return -1;
	if (!isdigit(linkname[3]))
		return -1;
	unit = atoi(&linkname[3]);
	
	if(unit >= 80 && (unit % 10) == 0)
		return unit/10;
	else
		return unit;
}

/*
 * Called when link comes up
 */
int
ipup_main(int argc, char **argv)
{
	char *wan_ifname = safe_getenv("IFNAME");
	// char *wan_linkname = safe_getenv("LINKNAME");
	char tmp[32] = {0}, prefix[] = "wanXXXXXXXXXX_";
	int unit;
	char *value;
	char buf[256];

	_dprintf("%s(%s)\n", __FUNCTION__, wan_ifname);

	// Get unit from IFNAME: ppp[UNIT]
	if ((unit = ppp_linkunit(wan_ifname)) < 0)
		return 0;

	snprintf(prefix, sizeof(prefix), "wan%d_", unit);

	if ((value = getenv("IPLOCAL"))) {
		tcapi_set(WANDUCK_DATA, strcat_r(prefix, "ipaddr", tmp), value);
		tcapi_set(WANDUCK_DATA, strcat_r(prefix, "netmask", tmp), "255.255.255.255");
	}

	if ((value = getenv("IPREMOTE"))) {
		tcapi_set(WANDUCK_DATA, strcat_r(prefix, "gateway", tmp), value);
	}

	strcpy(buf, "");
	if ((value = getenv("DNS1")))
		sprintf(buf, "%s", value);
	if ((value = getenv("DNS2")))
		sprintf(buf + strlen(buf), "%s%s", strlen(buf) ? " " : "", value);

	if (strlen(buf)) {
		tcapi_set(WANDUCK_DATA, strcat_r(prefix, "dns_rx", tmp), buf);
	}

	wan_up(wan_ifname);

	_dprintf("%s(%d) done.\n", __FUNCTION__, unit);

	return 0;
}

/*
 * Called when link goes down
 */
int
ipdown_main(int argc, char **argv)
{
	char *wan_ifname = safe_getenv("IFNAME");
	// char *wan_linkname = safe_getenv("LINKNAME");
	char prefix[] = "wanXXXXXXXXXX_";
	int unit;

	_dprintf("%s(%s)\n", __FUNCTION__, wan_ifname);

	// Get unit from IFNAME: ppp[UNIT]
	if ((unit = ppp_linkunit(wan_ifname)) < 0)
		return 0;

	snprintf(prefix, sizeof(prefix), "wan%d_", unit);

	update_wan_state(prefix, WAN_STATE_STOPPED, pppstatus());

	_dprintf("%s(%d) done.\n", __FUNCTION__, unit);

	return 0;
}


/*
 * Called when link closing with auth fail
 */
int
authfail_main(int argc, char **argv)
{
	char *wan_ifname = safe_getenv("IFNAME");
	// char *wan_linkname = safe_getenv("LINKNAME");
	char prefix[] = "wanXXXXXXXXXX_";
	int unit;

	// Get unit from IFNAME: ppp[UNIT]
	if ((unit = ppp_linkunit(wan_ifname)) < 0)
		return 0;

	snprintf(prefix, sizeof(prefix), "wan%d_", unit);

	// override wan_state
	update_wan_state(prefix, WAN_STATE_STOPPED, WAN_STOPPED_REASON_PPP_AUTH_FAIL);

	return 0;
}

void USB_restart_dnsmasq(void)
{
	FILE *fp = NULL;
	char tmp[100] = {0};
	char prefix[] = "wanXXXXXXXXXX_";
	int unit = 11; //USB modem
	char result[256] = {0};
	char primary_dns[128] = {0};
	char second_dns[128] = {0};
	char ifx[128] = {0};
	char *p = NULL;

	system("killall -9 dnsmasq");
	fp = fopen("/etc/dnsmasq.conf", "w");
	if(fp == NULL){
		cprintf("Cannot open dnsmasq.conf!!\n");
		return;
	}

	snprintf(prefix, sizeof(prefix), "wan%d_", unit);

	snprintf(result, sizeof(result), "%s", nvram_safe_get(strcat_r(prefix, "dns", tmp)));

	//parse DNS
	if(p=strstr(result," "))
	{
		*p = '\0';
		snprintf(primary_dns, sizeof(primary_dns), "%s", result);
		snprintf(second_dns, sizeof(second_dns), "%s", p+1);
	}
	else
	{
		snprintf(primary_dns, sizeof(primary_dns), "%s", result);
		second_dns[0] = '\0';
	}
	cprintf("primary_dns=[%s], second_dns=[%s]\n", primary_dns, second_dns);

	if(nvram_match(strcat_r(prefix, "proto", tmp), "pppoe"))
	{
		snprintf(ifx, sizeof(ifx), "%s", nvram_safe_get(strcat_r(prefix, "pppoe_ifname", tmp)));
	}
	else
	{
		snprintf(ifx, sizeof(ifx), "%s", nvram_safe_get(strcat_r(prefix, "ifname", tmp)));
	}
	cprintf("ifx=[%s]\n", ifx);

	if(strlen(primary_dns) > 0 )
	{
		fprintf(fp, "server=%s@%s\n", primary_dns, ifx);
	}
	if(strlen(second_dns) > 0 )
	{
		fprintf(fp, "server=%s@%s", second_dns, ifx);
	}
	fclose(fp);
	system("/userfs/bin/dnsmasq");
}
