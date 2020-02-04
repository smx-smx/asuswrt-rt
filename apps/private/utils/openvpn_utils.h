#ifndef _OPENVPN_UTILS_H
#define _OPENVPN_UTILS_H

int current_addr(in_addr_t addr);
int current_route(in_addr_t network, in_addr_t netmask);

#endif
