#ifndef _VPN_H
#define _VPN_H

#include "cfg_manager.h"

#if defined(RTCONFIG_ACCEL_PPTPD) || defined(RTCONFIG_OPENVPN) || defined(RTCONFIG_VPNC)
int vpncontrol_init(void);
#endif

#ifdef RTCONFIG_ACCEL_PPTPD
int pptp_init(void);
int pptp_boot(mxml_node_t *top);
int pptp_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#ifdef RTCONFIG_VPNC
#define VPNC_CONF_PATH "/etc/vpnc.conf"
int vpnc_init(void);
int vpnc_boot(mxml_node_t *top);
int vpnc_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
int vpnc_write(mxml_node_t *top, mxml_node_t *parant);
#endif

#ifdef RTCONFIG_OPENVPN
int openvpn_init(void);
int openvpn_write(mxml_node_t *top, mxml_node_t *parant);
int openvpn_execute(mxml_node_t *top, char name[][MAX_NODE_NAME]);
#endif

#endif
