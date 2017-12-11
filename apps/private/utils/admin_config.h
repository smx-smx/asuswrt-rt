#ifndef _CONFIG_ADMIN_H
#define _CONFIG_ADMIN_H
#define HTTPS_Node "Https_Entry"

#define	WEB_MODE_HTTP 1
#define	WEB_MODE_HTTPS 2
#define	WEB_MODE_BOTH 3

typedef struct _WEBConn_conf
{
	int web_mode;
	int https_port;
} WEBConn_conf_t;

int get_WEBConn_config(WEBConn_conf_t *conf);
#endif
