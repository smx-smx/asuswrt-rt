#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <asm/unaligned.h>

#ifndef DPROXY_H
#define DPROXY_H

#define PORT 53
#define NAME_SIZE 256
#define MAX_PACKET_SIZE 512
#define BUF_SIZE NAME_SIZE

typedef u_int16_t uint16;
typedef u_int32_t uint32;

#include "dns.h"

#ifndef DNS_TIMEOUT
#define DNS_TIMEOUT 240
#endif
#ifndef DNS_SERVER_TIMEOUT
#define DNS_SERVER_TIMEOUT 2
#endif
#ifndef MAX_NAME_SERVER
#define MAX_NAME_SERVER 5
#endif

#ifndef NAME_SERVER_DEFAULT
#define NAME_SERVER_DEFAULT "0.0.0.0"
#endif
#ifndef CONFIG_FILE_DEFAULT
#define CONFIG_FILE_DEFAULT "/etc/dproxy.conf"
#endif
#ifndef DENY_FILE_DEFAULT
#define DENY_FILE_DEFAULT "/etc/dproxy.deny"
#endif
#ifndef CACHE_FILE_DEFAULT
#define CACHE_FILE_DEFAULT "/var/cache/dproxy.cache"
#endif
#ifndef HOSTS_FILE_DEFAULT
#define HOSTS_FILE_DEFAULT "/etc/hosts"
#endif
#ifndef PURGE_TIME_DEFAULT
//#define PURGE_TIME_DEFAULT 48 * 60 * 60  Original
#define PURGE_TIME_DEFAULT 5 * 60
#endif
#ifndef PPP_DEV_DEFAULT
#define PPP_DEV_DEFAULT "/var/run/ppp0.pid"
#endif
#ifndef DHCP_LEASES_DEFAULT
#define DHCP_LEASES_DEFAULT "/var/lib/misc/udhcpd.leases"
#endif
#ifndef PPP_DETECT_DEFAULT
#define PPP_DETECT_DEFAULT 0
#endif
#ifndef DEBUG_FILE_DEFAULT
#define DEBUG_FILE_DEFAULT "/var/log/dproxy.debug.log"
#endif

void debug_perror( char * msg );
void debug(char *fmt, ...);

#endif
