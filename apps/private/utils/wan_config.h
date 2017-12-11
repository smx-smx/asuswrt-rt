#ifndef _CONFIG_WAN_H
#define _CONFIG_WAN_H
#define Wanduck_Node "Wanduck_Common"
#define Dualwan_Node "Dualwan_Entry"

typedef enum _Wan_mode
{
	WAN_MODE_SINGLE = 0,
	WAN_MODE_FAILOVER,
	WAN_MODE_FAILBACK,
	WAN_MODE_LOADBALANCE,
} Wan_mode_t;

typedef struct _DNS_ip_info
{
	char ipaddr[32];
} DNS_ip_info_t;

typedef struct _DNS_info
{
	int count;
	DNS_ip_info_t server[2];
} DNS_info_t;

typedef struct _Internet_info
{
	Wan_mode_t wan_mode;
	int wan_primary_id;
	int wan_secondary_id;
	int wan_state;
	int wan_sbstate;
	int wan_auxstate;
	char wan_ifname[32];	//physical name
	char wan_proto[32];
	char wan_linkname[32];	//wan ifname, e.g. ppp0
	char wan_ipaddr[32];
	char wan_netmask[32];
	char wan_gateway[32];
	DNS_info_t dns;
	DNS_info_t dns_rx;
} Internet_info_t;

int get_internet_status(Internet_info_t *info);
#endif
