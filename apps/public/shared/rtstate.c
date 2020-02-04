#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bcmnvram.h>
//#include <bcmdevs.h>
#include <shutils.h>
#include <shared.h>
#include <rtstate.h>
#include "tcutils.h"
#include <sys/ioctl.h>

/* keyword for rc_support 	*/
/* ipv6 mssid update parental 	*/

void add_rc_support(char *feature)
{
	char *rcsupport = nvram_safe_get("rc_support");
	char *features;

	if (!(feature && *feature))
		return;

	if (*rcsupport) {
		features = malloc(strlen(rcsupport) + strlen(feature) + 2);
		if (features == NULL) {
			_dprintf("add_rc_support fail\n");
			return;
		}
		sprintf(features, "%s %s", rcsupport, feature);
		nvram_set("rc_support", features);
		free(features);
	} else
		nvram_set("rc_support", feature);
}

/* keyword for ss_support 	*/
#ifdef RTCONFIG_CLOUDSYNC
void add_ss_support(char *feature)
{
	char *sssupport = nvram_safe_get("ss_support");
	char *features;

	if (!(feature && *feature))
		return;

	if (*sssupport) {
		features = malloc(strlen(sssupport) + strlen(feature) + 2);
		if (features == NULL) {
			_dprintf("add_ss_support fail\n");
			return;
		}
		sprintf(features, "%s %s", sssupport, feature);
		nvram_set("ss_support", features);
		free(features);
	} else
		nvram_set("ss_support", feature);
}
#endif

int is_wan_connect(int unit){
	char tmp[100], prefix[]="wanXXXXXX_";
	int wan_state, wan_sbstate, wan_auxstate;

	snprintf(prefix, sizeof(prefix), "wan%d_", unit);

	wan_state = nvram_get_int(strcat_r(prefix, "state_t", tmp));
	wan_sbstate = nvram_get_int(strcat_r(prefix, "sbstate_t", tmp));
	wan_auxstate = nvram_get_int(strcat_r(prefix, "auxstate_t", tmp));

	if(wan_state == 2 && wan_sbstate == 0 &&
			(wan_auxstate == 0 || wan_auxstate == 2)
			)
		return 1;
	else
		return 0;
}

int get_wan_state(int unit)
{
	char tmp[100], prefix[]="wanXXXXXX_";

	snprintf(prefix, sizeof(prefix), "wan%d_", unit);

	return nvram_get_int(strcat_r(prefix, "state_t", tmp));
}

#ifdef RTCONFIG_DUALWAN
#endif


// get wan_unit from device ifname or hw device ifname
#if 0
int get_wan_unit(char *ifname)
{
	char word[256], tmp[100], *next;
	char prefix[32]="wanXXXXXX_";
	int unit, found = 0;

	unit = WAN_UNIT_FIRST;

	foreach (word, nvram_safe_get("wan_ifnames"), next) {
		if(strncmp(ifname, "ppp", 3)==0) {
			snprintf(prefix, sizeof(prefix), "wan%d_", unit);
			if(strcmp(nvram_safe_get(strcat_r(prefix, "pppoe_ifname", tmp)), ifname)==0) {
				found = 1;
			}	
		}
		else if(strcmp(ifname, word)==0) {
			found = 1;
		}
		if(found) break;
		unit ++;
	}

	if(!found) unit = WAN_UNIT_FIRST;
	return unit;
}
#else
int get_wan_unit(char *ifname)
{
	char tmp[100], prefix[32]="wanXXXXXX_";
	int unit = 0;
	int model = get_model();

	if(ifname == NULL)
		return -1;

	for(unit = WAN_UNIT_FIRST; unit < WAN_UNIT_MAX; ++unit){
		snprintf(prefix, sizeof(prefix), "wan%d_", unit);

		if(!strncmp(ifname, "ppp", 3) ){

			if(nvram_match(strcat_r(prefix, "pppoe_ifname", tmp), ifname)) {
				if (model ==  MODEL_RTN65U) {
					if(!nvram_match(strcat_r(prefix, "proto", tmp), "pppoe") || nvram_match(strcat_r(prefix, "is_usb_modem_ready", tmp), "1"))						
						return unit;
				}	
				else if (nvram_match(strcat_r(prefix, "state_t", tmp), "2") && nvram_match(strcat_r(prefix, "auxstate_t", tmp), "0") && nvram_match(strcat_r(prefix, "gw_ifname", tmp), ifname)) 
					return unit;				
			}

				
		}
		else if(nvram_match(strcat_r(prefix, "ifname", tmp), ifname)) {
			
			if (model == MODEL_RTN65U && !nvram_match(strcat_r(prefix, "proto", tmp), "l2tp") && !nvram_match(strcat_r(prefix, "proto", tmp), "pptp"))
					return unit;
			
			if (!nvram_match(strcat_r(prefix, "proto", tmp), "pppoe") && !nvram_match(strcat_r(prefix, "proto", tmp), "l2tp") && !nvram_match(strcat_r(prefix, "proto", tmp), "pptp") && nvram_match(strcat_r(prefix, "gw_ifname", tmp), ifname))
					return unit;						
		}   
	}

	return -1;
}
#endif

int get_wan_unit_ex(const char *ifname, int *unit, int *subunit)
{
	int ret = -1;

	if(!ifname || !unit || !subunit)
		return -1;

	if(!strncmp(ifname, "ppp", 3))
	{
		*unit = atoi(ifname + 3);
		if( *unit >= 80 )
		{
			*subunit = *unit % 10;
			*unit /= 10;
		}
		else
		{
			*subunit = -1;
		}
		ret = 0;
	}
	else if(!strncmp(ifname, "nas", 3))
	{
		if(strchr(ifname, '_'))
		{
			sscanf(ifname+3, "%d_%d", unit, subunit);
		}
		else
		{
			*unit = atoi(ifname + 3);
			*subunit = -1;
		}
		ret = 0;
	}
	else if(!strncmp(ifname, "usb", 3))
	{
		*unit = 11;
		*subunit = -1;
		ret = 0;
	}
	else if(!strncmp(ifname, "eth", 3))
	{
		*unit = 11;
		*subunit = -1;
		ret = 0;
	}

	return ret;
}

// Get physical wan ifname of working connection
char *get_wanx_ifname(int unit)
{
	char attribute[MAXLEN_ATTR_NAME];

	snprintf(attribute, sizeof(attribute), "wan%d_ifname", unit);

	return nvram_safe_get(attribute);
}

// Get wan ifname of working connection
char *get_wan_ifname(int unit)
{
	char *wan_proto, *wan_ifname;
	char tmp[100], prefix[]="wanXXXXXXXXXX_";
	
	snprintf(prefix, sizeof(prefix), "wan%d_", unit);
	wan_proto = nvram_safe_get(strcat_r(prefix, "proto", tmp));

	//_dprintf("wan_proto: %s\n", wan_proto);

#ifdef RTCONFIG_USB_MODEM
	if (dualwan_unit__usbif(unit)) {
		if(!strcmp(wan_proto, "dhcp"))
			wan_ifname = nvram_safe_get(strcat_r(prefix, "ifname", tmp));
		else
			wan_ifname = nvram_safe_get(strcat_r(prefix, "pppoe_ifname", tmp));
	}
	else
#endif
	if(strcmp(wan_proto, "pppoe") == 0 ||
		strcmp(wan_proto, "pptp") == 0 ||
		strcmp(wan_proto, "l2tp") == 0)
		wan_ifname = nvram_safe_get(strcat_r(prefix, "pppoe_ifname", tmp));
	else
		wan_ifname = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	//_dprintf("wan_ifname: %s\n", wan_ifname);

	return wan_ifname;
}

#ifdef RTCONFIG_IPV6

char *get_wan6_ifname(int unit)
{
	char *wan_proto, *wan_ifname;
	char tmp[100], prefix[]="wanXXXXXXXXXX_";
	
	snprintf(prefix, sizeof(prefix), "wan%d_", unit);
	wan_proto = nvram_safe_get(strcat_r(prefix, "proto", tmp));


	if(strcmp(nvram_safe_get("ipv6_ifdev"), "eth")==0) {
		wan_ifname=nvram_safe_get(strcat_r(prefix, "ifname", tmp));
//		dbG("wan6_ifname: %s\n", wan_ifname);
		return wan_ifname;
	}

#ifdef RTCONFIG_USB_MODEM
	if (dualwan_unit__usbif(unit)) {
		if(!strcmp(wan_proto, "dhcp"))
			wan_ifname = nvram_safe_get(strcat_r(prefix, "ifname", tmp));
		else
			wan_ifname = nvram_safe_get(strcat_r(prefix, "pppoe_ifname", tmp));
	}
	else
#endif
	if(strcmp(wan_proto, "pppoe") == 0 ||
		strcmp(wan_proto, "pptp") == 0 ||
		strcmp(wan_proto, "l2tp") == 0)
		wan_ifname = nvram_safe_get(strcat_r(prefix, "pppoe_ifname", tmp));
	else wan_ifname=nvram_safe_get(strcat_r(prefix, "ifname", tmp));

//	dbG("wan6_ifname: %s\n", wan_ifname);
	return wan_ifname;
}

#endif

#if 0
// OR all lan port status
int get_lanports_status(void)
{
	return lanport_status();
}
#endif

extern int wanport_status(int wan_unit);

// OR all wan port status
#if ASUSWRT
int get_wanports_status(int wan_unit)
{
// 1. PHY type, 2. factory owner, 3. model.
#ifdef RTCONFIG_DSL
#ifdef RTCONFIG_DUALWAN
	if(get_dualwan_by_unit(wan_unit) == WANS_DUALWAN_IF_DSL)
#endif
	{
		/* Paul modify 2012/10/18, check both ADSL sync status, and WAN0 state. */
		if (nvram_match("dsltmp_adslsyncsts","up") && nvram_match("wan0_state_t","2")) return 1;
		return 0;
	}
#ifdef RTCONFIG_DUALWAN
	if(get_dualwan_by_unit(wan_unit) == WANS_DUALWAN_IF_LAN)
	{
	#ifdef RTCONFIG_RALINK
		return rtkswitch_wanPort_phyStatus(wan_unit); //Paul modify 2012/12/4
	#else
		return wanport_status(wan_unit);
	#endif
	}
#endif
	// TO CHENI:
	// HOW TO HANDLE USB?	
#else // RJ-45
#ifdef RTCONFIG_RALINK
	return rtkswitch_wanPort_phyStatus(wan_unit);
#else
	return wanport_status(wan_unit);
#endif
#endif
}
#else
int get_wanports_status(int wan_unit)
{
	char buf[256];

	if(wan_unit < WAN_UNIT_ETH) {	//xDSL
		memset(buf, 0, sizeof(buf));
		f_read_string("/proc/tc3162/adsl_stats", buf, sizeof(buf));
		if(strstr(buf, "up")) {
			if(wan_unit < WAN_UNIT_PTM0)	//ADSL
				if(strstr(buf, "PTM"))
					return 0;
				else
					return 1;
			else
				if(strstr(buf, "PTM"))
					return 1;
				else
					return 0;
		}
		else {
			return 0;
		}
	}
	else if(wan_unit == WAN_UNIT_ETH){
		memset(buf, 0, sizeof(buf));
#if defined(TCSUPPORT_MTK_INTERNAL_ETHER_SWITCH) || defined(MT7530_SUPPORT)
		f_read_string("/proc/tc3162/eth1_link_st", buf, sizeof(buf));
		if(strstr(buf, "10")) //10M,100M,1000M
			return 1;
		else //Down
			return 0;
#else
		f_read_string("/proc/tc3162/eth_wan_link_st", buf, sizeof(buf));
		if(strncmp(buf, "0", 1)) //0:down, 1:10M/100M, 2:1000M
			return 1;
		else
			return 0;
#endif
	}
#ifdef TCSUPPORT_WAN_ETHER_LAN
	else if(wan_unit == WAN_UNIT_ETH_LAN){
		int status[4] = {0};
		int wans_lanport;
		static int last_status = 0;
		int cnt = 2;

		memset(buf, 0, sizeof(buf));
		if(tcapi_get("Dualwan_Entry", "wans_lanport", buf))
			return last_status;
		else {
			wans_lanport = atoi(buf);
			while(cnt--) {
				memset(buf, 0, sizeof(buf));
				f_read_string("/proc/tc3162/eth_port_status", buf, sizeof(buf));
				sscanf(buf, "%d %d %d %d", status, status+1, status+2, status+3);
				if(last_status && !status[wans_lanport-1] && cnt) {	//first up->down
					usleep(300000);
					continue;
				}
				else {
					last_status = status[wans_lanport-1];
					return status[wans_lanport-1];
				}
			}
		}
	}
#endif
	else
		return 0;
}
#endif

int get_usb_modem_state(){
	if(!strcmp(nvram_safe_get("modem_running"), "1"))
		return 1;
	else
		return 0;
}

int set_usb_modem_state(const int flag){
	if(flag != 1 && flag != 0)
		return 0;

	if(flag){
		nvram_set("modem_running", "1");
		return 1;
	}
	else{
		nvram_set("modem_running", "0");
		return 0;
	}
}

int
set_wan_primary_ifunit(const int unit)
{
	int wans = 0;

	if (unit < WAN_UNIT_FIRST || unit >= WAN_UNIT_MAX)
		return -1;

#ifdef RTCONFIG_DUALWAN
	wans = get_dualwan_by_pvcunit(unit);
	if(wans == get_dualwan_primary())	//first item in wans_dualwan
	{
		nvram_set_int("wan_primary", unit);
		nvram_set_int("wan_secondary", get_pvcunit_by_dualwan(get_dualwan_secondary()));
	}
	else if(wans == get_dualwan_secondary())	//second item in wans_dualwan
	{
		nvram_set_int("wan_primary", unit);
		nvram_set_int("wan_secondary", get_pvcunit_by_dualwan(get_dualwan_primary()));
	}
	else
	{
		cprintf("%s: %d not internet unit\n", __func__, unit);
	}
#else
	nvram_set_int("wan_primary", unit);
#endif

	return 0;
}

//wan_primary will be the pvc unit of primary wan (current wan in fo mode)
int
wan_primary_ifunit(void)
{
	return nvram_get_int("wan_primary");
}

//wan_secondary will be the pvc unit of secondary wan (other wan in fo mode)
int
wan_secondary_ifunit(void)
{
	return nvram_get_int("wan_secondary");
}


int
get_wan_primary_ifunit(void)
{
	char node[128] = {0};
	char value[32] = {0};
	int i = 0;
	int pool[] = {0, 8, 9, 10, 11, 12};
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
	int j;
#endif

	for( i = 0; i < sizeof(pool)/sizeof(int); i++ )
	{
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
		if(pool[i])
		{
			for( j = 0; j < MAX_SERVICE_NUM; j++ )
			{
				memset(node, 0, sizeof(node));
				memset(value, 0, sizeof(value));
				sprintf(node, "WanExt_PVC%de%d", pool[i], j);
				if(tcapi_match(node, "Active", "Yes"))
				{
					memset(value, 0, sizeof(value));
					if(tcapi_match(node, "DEFAULTROUTE", "Yes"))
						return pool[i];
				}
			}
		}
		else
#endif
		{
			memset(node, 0, sizeof(node));
			memset(value, 0, sizeof(value));
			sprintf(node, "Wan_PVC%d", pool[i]);
			if(tcapi_match(node, "Active", "Yes"))
			{
				memset(value, 0, sizeof(value));
				if(tcapi_match(node, "DEFAULTROUTE", "Yes"))
					return pool[i];
			}
		}
	}
	return 0;
}

char *get_wan_ifname_ex(int unit, char *wan_ifname)
{
	char wan_proto[16];
	char attribute[64];

	if(!wan_ifname)
		return NULL;

	snprintf(attribute, sizeof(attribute), "wan%d_proto", unit);
	if(tcapi_get("Wanduck_Common", attribute, wan_proto) != TCAPI_PROCESS_OK)
		return NULL;

#ifdef RTCONFIG_USB_MODEM
	if (unit == 11) {
		if(!strcmp(wan_proto, "dhcp"))
		{
			snprintf(attribute, sizeof(attribute), "wan%d_ifname", unit);
		}
		else
		{
			snprintf(attribute, sizeof(attribute), "wan%d_pppoe_ifname", unit);
		}
	}
	else
#endif
	if(strcmp(wan_proto, "pppoe") == 0 ||
		strcmp(wan_proto, "pptp") == 0 ||
		strcmp(wan_proto, "l2tp") == 0)
	{
		snprintf(attribute, sizeof(attribute), "wan%d_pppoe_ifname", unit);
	}
	else
	{
		snprintf(attribute, sizeof(attribute), "wan%d_ifname", unit);
	}

	if(tcapi_get("Wanduck_Common", attribute, wan_ifname) != TCAPI_PROCESS_OK)
		return NULL;

	//_dprintf("wan_ifname: %s\n", wan_ifname);

	if(!strlen(wan_ifname))
		return NULL;

	return wan_ifname;
}

char *get_wan_primary_ifname(char *ifname)
{
	char wan_primary[4] = {0};
	int unit = -1;

	if(tcapi_get("Wanduck_Common", "wan_primary", wan_primary) == TCAPI_PROCESS_OK)
	{
		if(strlen(wan_primary))
		{
			unit = atoi(wan_primary);
			return get_wan_ifname_ex(unit, ifname);
		}
	}
	return NULL;
}

char *get_wan_secondary_ifname(char *ifname)
{
	char wan_secondary[4] = {0};
	int unit = -1;

	if(tcapi_get("Wanduck_Common", "wan_secondary", wan_secondary) == TCAPI_PROCESS_OK)
	{
		if(strlen(wan_secondary))
		{
			unit = atoi(wan_secondary);
			return get_wan_ifname_ex(unit, ifname);
		}
	}
	return NULL;
}

char *get_first_connected_public_wan_ifname(char *ifname)
{
	char wan_unit[4] = {0};
	char wan_ip_address[16] = {0};
	char wan_ip_attribute[16] = {0};
	int unit = -1;

	if(tcapi_get("Wanduck_Common", "wan_primary", wan_unit) == TCAPI_PROCESS_OK)
	{
		if(strlen(wan_unit))
		{
			unit = atoi(wan_unit);
			snprintf(wan_ip_attribute, sizeof(wan_ip_attribute), "wan%d_ipaddr", unit);
			if(tcapi_get("Wanduck_Common", wan_ip_attribute, wan_ip_address) == TCAPI_PROCESS_OK)
			{
				if(!is_private_subnet(wan_ip_address))
				{
					return get_wan_ifname_ex(unit, ifname);
				}
			}
		}
	}

	memset(wan_unit, 0, sizeof(wan_unit));
	memset(wan_ip_address, 0, sizeof(wan_ip_address));
	memset(wan_ip_attribute, 0, sizeof(wan_ip_attribute));
	unit = -1;

	if(tcapi_get("Wanduck_Common", "wan_secondary", wan_unit) == TCAPI_PROCESS_OK)
	{
		if(strlen(wan_unit))
		{
			unit = atoi(wan_unit);
			snprintf(wan_ip_attribute, sizeof(wan_ip_attribute), "wan%d_ipaddr", unit);
			if(tcapi_get("Wanduck_Common", wan_ip_attribute, wan_ip_address) == TCAPI_PROCESS_OK)
			{
				if(!is_private_subnet(wan_ip_address))
				{
					return get_wan_ifname_ex(unit, ifname);
				}
			}
		}
	}

	return NULL;
}


int getTransferMode(void)
{
	int activePVC = 0;
	int transferMode = TMode_ADSL;

	activePVC = get_wan_primary_ifunit();

	switch(activePVC)
	{
		/* ATM */
		case 0:
			transferMode = TMode_ADSL;
			break;
		/* PTM */
		case 8:
			/* FALL-THROUGH */
		case 9:
			transferMode = TMode_VDSL;
			break;
		/* Ethernet WAN */
		case 10:
			transferMode = TMode_Ether;
			break;
		case 11:
			transferMode = TMode_USB;
		default:
			transferMode = TMode_ADSL;
			break;
	}

	return transferMode;
}

#ifdef RTCONFIG_DUALWAN
//return pvc unit of first item in wans_dualwan
int
wan_primary_pvcunit(void)
{
	return get_pvcunit_by_dualwan(get_dualwan_primary());
}

//return pvc unit of second item in wans_dualwan
int
wan_secondary_pvcunit(void)
{
	return get_pvcunit_by_dualwan(get_dualwan_secondary());
}
#endif

#ifdef RTCONFIG_MEDIA_SERVER
void
set_invoke_later(int flag)
{
	nvram_set_int("invoke_later", nvram_get_int("invoke_later")|flag);
}

int
get_invoke_later()
{
	return(nvram_get_int("invoke_later"));
}
#endif	/* RTCONFIG_MEDIA_SERVER */

#ifdef RTCONFIG_USB

char xhci_string[32];
char ehci_string[32];
char ohci_string[32];

char *get_usb_xhci_port(int port)
{
        char word[100], *next;
        int i=0;

        strcpy(xhci_string, "xxxxxxxx");

        foreach(word, nvram_safe_get("xhci_ports"), next) {
                if(i==port) {
                        strcpy(xhci_string, word);
                        break;
                }
                i++;
        }
        return xhci_string;
}

char *get_usb_ehci_port(int port)
{
	char word[100], *next;
	int i=0;

	strcpy(ehci_string, "xxxxxxxx");

	foreach(word, nvram_safe_get("ehci_ports"), next) {
		if(i==port) {
			strcpy(ehci_string, word);
			break;
		}		
		i++;
	}
	return ehci_string;
}

char *get_usb_ohci_port(int port)
{
	char word[100], *next;
	int i=0;

	strcpy(ohci_string, "xxxxxxxx");

	foreach(word, nvram_safe_get("ohci_ports"), next) {
		if(i==port) {
			strcpy(ohci_string, word);
			break;
		}		
		i++;
	}
	return ohci_string;
}

int get_usb_port_number(const char *usb_port)
{
	char word[100], *next;
	int port_num, i;

	port_num = 0;
	i = 0;
	foreach(word, nvram_safe_get("xhci_ports"), next){
		++i;
		if(!strcmp(usb_port, word)){
			port_num = i;
			break;
		}
	}

	i = 0;
	if(port_num == 0){
		foreach(word, nvram_safe_get("ehci_ports"), next){
			++i;
			if(!strcmp(usb_port, word)){
				port_num = i;
				break;
			}
		}
	}

	i = 0;
	if(port_num == 0){
		foreach(word, nvram_safe_get("ohci_ports"), next){
			++i;
			if(!strcmp(usb_port, word)){
				port_num = i;
				break;
			}
		}
	}

	return port_num;
}

int get_usb_port_host(const char *usb_port)
{
	char word[100], *next;
	int i;

	i = 0;
	foreach(word, nvram_safe_get("xhci_ports"), next){
		++i;
		if(!strcmp(usb_port, word)){
			return USB_HOST_XHCI;
		}
	}

	i = 0;
	foreach(word, nvram_safe_get("ehci_ports"), next){
		++i;
		if(!strcmp(usb_port, word)){
			return USB_HOST_EHCI;
		}
	}

	i = 0;
	foreach(word, nvram_safe_get("ohci_ports"), next){
		++i;
		if(!strcmp(usb_port, word)){
			return USB_HOST_OHCI;
		}
	}

	return USB_HOST_NONE;
}
#endif

#ifdef RTCONFIG_DUALWAN
void set_wanscap_support(char *feature)
{
	nvram_set("wans_cap", feature);
}

void add_wanscap_support(char *feature)
{
	char features[128];

	strcpy(features, nvram_safe_get("wans_cap"));

	if(strlen(features)==0) nvram_set("wans_cap", feature);
	else {
		sprintf(features, "%s %s", features, feature);
		nvram_set("wans_cap", features);
	}
}

int get_wans_dualwan(void) 
{
	int caps=0;
	char *wancaps = nvram_safe_get("wans_dualwan");

	if(strstr(wancaps, "lan")) caps |= WANSCAP_LAN;
	if(strstr(wancaps, "2g")) caps |= WANSCAP_2G;
	if(strstr(wancaps, "5g")) caps |= WANSCAP_5G;
	if(strstr(wancaps, "usb")) caps |= WANSCAP_USB;
	if(strstr(wancaps, "dsl")) caps |= WANSCAP_DSL;
	if(strstr(wancaps, "wan")) caps |= WANSCAP_WAN;

	return caps;
}

int get_dualwan_by_unit(int unit)
{
	int i;
	char word[80], *next;

	i = 0;
	foreach(word, nvram_safe_get("wans_dualwan"), next) {
		if(i==unit) {
			if (!strcmp(word,"lan")) return WANS_DUALWAN_IF_LAN;
			if (!strcmp(word,"2g")) return WANS_DUALWAN_IF_2G;
			if (!strcmp(word,"5g")) return WANS_DUALWAN_IF_5G;
			if (!strcmp(word,"usb")) return WANS_DUALWAN_IF_USB;
			if (!strcmp(word,"dsl")) return WANS_DUALWAN_IF_DSL;
			if (!strcmp(word,"wan")) return WANS_DUALWAN_IF_WAN;
			return WANS_DUALWAN_IF_NONE;
		}
		i++;
	}

	return WANS_DUALWAN_IF_NONE;
}

// imply: unit 0: primary, unit 1: secondary
int get_dualwan_primary(void)
{
	return get_dualwan_by_unit(0);
}

int get_dualwan_secondary(void)
{
	return get_dualwan_by_unit(1);
}

/*
 * check specific WAN currently runs as primary WAN or secondary WAN
 * Note : it's not related to dual-WAN configuration
*/
int is_running_as_second_wan(int unit)
{
	char *dualwan_mode = NULL;
	char *secondary_wan_id = NULL;
	int sec_wan_id = -1;

	dualwan_mode = nvram_safe_get("wans_mode");
	if (!strcmp(dualwan_mode, "fo") || !strcmp(dualwan_mode, "fb")) {
		secondary_wan_id = nvram_safe_get("wan_secondary");
		sec_wan_id = atoi(secondary_wan_id);
		if (sec_wan_id == unit)
			return 1;
	}

       return 0;
}

#endif

int get_pvcunit_by_dualwan(int wans_dualwan_if)
{
	char adsl_stats[256] = {0};

	switch(wans_dualwan_if) {
		case WANS_DUALWAN_IF_DSL:
			f_read_string("/proc/tc3162/adsl_stats", adsl_stats, sizeof(adsl_stats));
			if(strstr(adsl_stats, "up"))
			{
				if(strstr(adsl_stats, "PTM"))
					return WAN_UNIT_PTM0;
				else
					return WAN_UNIT_ATM;
			}
			else
			{
			#ifdef TCSUPPORT_WAN_PTM
				if(tcapi_match("Wan_Common", "DSLMode", "VDSL"))
				return WAN_UNIT_PTM0;
				else
			#endif
				return WAN_UNIT_ATM;
			}
		case WANS_DUALWAN_IF_WAN:
			return WAN_UNIT_ETH;
		case WANS_DUALWAN_IF_USB:
			return WAN_UNIT_USB;
		case WANS_DUALWAN_IF_LAN:
			return WAN_UNIT_ETH_LAN;
		case WANS_DUALWAN_IF_NONE:
		default:
			return -1;
	}
}

int get_dualwan_by_pvcunit(int pvcunit)
{
	switch(pvcunit) {
		case WAN_UNIT_ATM:
		case WAN_UNIT_PTM0:
			return WANS_DUALWAN_IF_DSL;
		case WAN_UNIT_ETH:
			return WANS_DUALWAN_IF_WAN;
		case WAN_UNIT_USB:
			return WANS_DUALWAN_IF_USB;
		case WAN_UNIT_ETH_LAN:
			return WANS_DUALWAN_IF_LAN;
		default:
			return WANS_DUALWAN_IF_NONE;
	}
}

// no more to use
/*
void set_dualwan_type(char *type)
{
	nvram_set("wans_dualwan", type);
}

void add_dualwan_type(char *type)
{
	char types[128];

	strcpy(types, nvram_safe_get("wans_dualwan"));

	if(strlen(types)==0) nvram_set("wans_dualwan", type);
	else {
		sprintf(types, "%s %s", types, type);
		nvram_set("wans_dualwan", types);
	}
}
*/

void set_lan_phy(char *phy)
{
	nvram_set("lan_ifnames", phy);
}

void add_lan_phy(char *phy)
{
	char phys[128];

	strcpy(phys, nvram_safe_get("lan_ifnames"));

	if(strlen(phys)==0) nvram_set("lan_ifnames", phy);
	else {
		sprintf(phys, "%s %s", phys, phy);
		nvram_set("lan_ifnames", phys);
	}
}

void set_wan_phy(char *phy)
{
	nvram_set("wan_ifnames", phy);
}

void add_wan_phy(char *phy)
{
	char phys[128];

	strcpy(phys, nvram_safe_get("wan_ifnames"));

	if(strlen(phys)==0) nvram_set("wan_ifnames", phy);
	else {
		sprintf(phys, "%s %s", phys, phy);
		nvram_set("wan_ifnames", phys);
	}
}

//TCLINUX
#ifndef WAN_ETHER_LAN_IF
#define WAN_ETHER_LAN_IF "nas12"
#endif

int get_cur_lanwan_port()
{
	char buf[1024];
	char *p;
	int id = -1;

	f_read_string("/proc/net/vlan/config", buf, sizeof(buf));
	p = strstr(buf, WAN_ETHER_LAN_IF);
	if(p) {
		//format: WAN_ETHER_LAN_IF | 1 | eth0
		sscanf(p, "%*s %*c %d", &id);
		return id;
	}
	else {
		return -1;
	}
}

int set_lanwan_if(int from, int to)
{
	char ifname[8] = {0};
	char cmd[64] = {0};
	struct ifreq ifr;
	int fd;

	if(from == to)
		return 0;

	_dprintf("lanwan_if %d => %d\n", from, to);

	fd = socket(PF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		_dprintf("%s: %d %s", __FUNCTION__, __LINE__, strerror(errno));
		return -1;
	}

	//from: WAN_ETHER_LAN_IF => eth0.x
	if(from >= 1 && from <= 4) {
		//set interface name WAN_ETHER_LAN_IF
		strncpy(ifr.ifr_name, WAN_ETHER_LAN_IF, IFNAMSIZ);

		//down
		if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0) {
			_dprintf("%s: %d SIOCGIFFLAGS: %s", __FUNCTION__, __LINE__, strerror(errno));
			close(fd);
			return -1;
		}
		ifr.ifr_flags &= ~IFF_UP;
		if (ioctl(fd, SIOCSIFFLAGS, &ifr) < 0) {
			_dprintf("%s: %d SIOCSIFFLAGS: %s", __FUNCTION__, __LINE__, strerror(errno));
			close(fd);
			return -1;
		}

		//rename to eth0.x
		snprintf(ifname, sizeof(ifname), "%s%d", LAN_PORT_ITF_PREFIX, from);
		strncpy(ifr.ifr_newname, ifname, IFNAMSIZ);
		if (ioctl(fd, SIOCSIFNAME, &ifr) < 0) {
			_dprintf("%s: %d SIOCSIFNAME: %s", __FUNCTION__, __LINE__, strerror(errno));
			close(fd);
			return -1;
		}

		//add to br0
		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "brctl addif br0 %s", ifname);
		system(cmd);
	}

	//to: eth0.x => WAN_ETHER_LAN_IF
	if(to >= 1 && to <= 4) {
		//set interface name eth0.x
		snprintf(ifname, sizeof(ifname), "%s%d", LAN_PORT_ITF_PREFIX, to);
		strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

		//down
		if (ioctl(fd, SIOCGIFFLAGS, &ifr) < 0) {
			_dprintf("%s: %d SIOCGIFFLAGS: %s", __FUNCTION__, __LINE__, strerror(errno));
			close(fd);
			return -1;
		}
		ifr.ifr_flags &= ~IFF_UP;
		if (ioctl(fd, SIOCSIFFLAGS, &ifr) < 0) {
			_dprintf("%s: %d SIOCSIFFLAGS: %s", __FUNCTION__, __LINE__, strerror(errno));
			close(fd);
			return -1;
		}

		//remove from br0
		memset(cmd, 0, sizeof(cmd));
		snprintf(cmd, sizeof(cmd), "brctl delif br0 %s", ifname);
		system(cmd);

		//rename to WAN_ETHER_LAN_IF
		strncpy(ifr.ifr_newname, WAN_ETHER_LAN_IF, IFNAMSIZ);
		if (ioctl(fd, SIOCSIFNAME, &ifr) < 0) {
			_dprintf("%s: %d SIOCSIFNAME: %s", __FUNCTION__, __LINE__, strerror(errno));
			close(fd);
			return -1;
		}
	}

	close(fd);
	return 0;
}

int isMultiSerPVC(int pvc){
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
	if((pvc >= WAN_UNIT_PTM0) && (pvc <= WAN_UNIT_ETH)){
		return 1;
	}
#endif
	return 0;
}
