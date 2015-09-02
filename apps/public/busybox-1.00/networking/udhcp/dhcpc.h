/* dhcpc.h */
#ifndef _DHCPC_H
#define _DHCPC_H

#define DEFAULT_SCRIPT  "/usr/share/udhcpc/default.script"

/* allow libbb_udhcp.h to redefine DEFAULT_SCRIPT */
#include "libbb_udhcp.h"

#define INIT_SELECTING	0
#define REQUESTING	1
#define BOUND		2
#define RENEWING	3
#define REBINDING	4
#define INIT_REBOOT	5
#define RENEW_REQUESTED 6
#define RELEASED	7
#ifdef DHCP_OPT121
#define OPT_CLASSLESS_STATIC_ROUTE 0x79
#endif
#if defined(TCSUPPORT_TR069_OPT43)
#define	OPT43_DBG
#define	OPT_VENDOR_SPECIFIC_INFOR	0x2B
#define OPT43_SUBOPT_ACS_URL  		1   	/* Vendor Specific Information Sub-option 1*/
#define OPT43_SUBOPT_PRVCODE  		2   	/* Vendor Specific Information Sub-option 2*/
#define CwmpURLMaxLen 				256
#define	CwmpTempStringMaxLen		256
#define	CwmpPrvCodeMaxLen			64
int min(int a, int b);
static void dhcpExtractOption43(uint8_t  *data, int len);
#endif
#ifdef RTCONFIG_TR181
#define OPT_VI_VENDOR_SPECIFIC 0x7D
#endif

struct client_config_t {
	char foreground;		/* Do not fork */
	char quit_after_lease;		/* Quit after obtaining lease */
	char abort_if_no_lease;		/* Abort if no lease */
	char background_if_no_lease;	/* Fork to background if no lease */
	char *interface;		/* The name of the interface to use */
	char *pidfile;			/* Optionally store the process ID */
	char *script;			/* User script to run at dhcp events */
	uint8_t *clientid;		/* Optional client id to use */
	uint8_t *hostname;		/* Optional hostname to use */
	int ifindex;			/* Index number of the interface to use */

	uint8_t arp[6];			/* Our arp address */
};

extern struct client_config_t client_config;


#endif
