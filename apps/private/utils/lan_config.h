#ifndef _CONFIG_LAN_H
#define _CONFIG_LAN_H
#define LAN_Node "Lan_Entry"

typedef struct _LAN_conf
{
	char lan_ip[32];
	char lan_netmask[32];
} LAN_conf_t;

int get_LAN_config(LAN_conf_t *conf);
#endif

