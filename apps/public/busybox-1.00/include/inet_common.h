/*
 * stolen from net-tools-1.59 and stripped down for busybox by
 *                      Erik Andersen <andersen@codepoet.org>
 *
 * Heavily modified by Manuel Novoa III       Mar 12, 2001
 *
 * Version:     $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/busybox-1.00/include/inet_common.h#1 $
 *
 */

#include <features.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


extern const char bb_INET_default[];    /* = "default" */

/* hostfirst!=0 If we expect this to be a hostname,
   try hostname database first
 */
extern int INET_resolve(const char *name, struct sockaddr_in *s_in, int hostfirst);


/* numeric: & 0x8000: default instead of *,
 *          & 0x4000: host instead of net,
 *          & 0x0fff: don't resolve
 */
extern int INET_rresolve(char *name, size_t len, struct sockaddr_in *s_in,
			 int numeric, unsigned int netmask);

extern int INET6_resolve(const char *name, struct sockaddr_in6 *sin6);
extern int INET6_rresolve(char *name, size_t len, struct sockaddr_in6 *sin6, int numeric);
