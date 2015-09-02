/* pppoatm.c - pppd plugin to implement PPPoATM protocol.
 *
 * Copyright 2000 Mitchell Blank Jr.
 * Based in part on work from Jens Axboe and Paul Mackerras.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 */
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <asm/types.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <net/if.h>
#include <atm.h>
#include <linux/atmdev.h>
#include <linux/atmppp.h>
#if _linux_
extern int new_style_driver;    /* From sys-linux.c */
#include <linux/ppp_defs.h>
#include <linux/if_ether.h>
#include <linux/if_pppox.h>
#include <linux/if_ppp.h>
#else
#error this module meant for use with linux only at this time
#endif

#include "pppd.h"
#include "fsm.h"
#include "lcp.h"
#include "ipcp.h"
#include "ccp.h"
#include "pathnames.h"


static void pppoatm_process_extra_options __P((void));
static void pppoatm_check_options __P((void));
static int  connect_pppoatm __P((void));
static void disconnect_pppoatm __P((void));
static int pppoatm_establish_ppp __P((int));
static void pppoatm_disestablish_ppp __P((int));
static void pppoatm_do_send_config __P((int, u_int32_t, int, int));
static void pppoatm_recv_config __P((int, u_int32_t, int, int));
static void cleanup_pppoatm __P((void));
static void pppoatm_close_fds __P((void));


char pppd_version[] = VERSION;


static struct pppoatm_globals {
  struct sockaddr_atmpvc pvcaddr;
  int     pppoatm_max_mtu, pppoatm_max_mru;
  int     real_ttyfd;          /* fd for actual serial port */
  bool    llc_encaps;
  bool    vc_encaps;
  char    *qosstr;
  char    *qos_pcr;
  char    *qos_scr;
  char    *qos_mbs;
  struct atm_qos qos;
} glb;

#define _PATH_ETHOPT         _ROOT_PATH "/etc/ppp/options."

static char *bad_options[] = {
    "noaccomp", 
    "-ac",
    "default-asyncmap", 
    "-am",
    "asyncmap", 
    "-as",
    "escape",
    "multilink",
    "receive-all",
#ifndef _DISABLE_SERIAL_
    "crtscts",
    "-crtscts",
    "nocrtscts",
    "cdtrcts",
    "nocdtrcts",
#endif
    "xonxoff",
#ifndef _DISABLE_SERIAL_
    "modem", 
#endif
    "local", 
    "sync",
    "deflate",
    "nodeflate",
    "vj",
    "novj",
    "nobsdcomp",
    "bsdcomp",
    "-bsdcomp",  
    NULL
};

int setdevname_pppoatm(const char *cp);

static option_t pppoatm_options[] = {
	{ "device name", o_wild, (void *) &setdevname_pppoatm,
	  "Serial port device name",
	  OPT_DEVNAM | OPT_PRIVFIX | OPT_NOARG  | OPT_A2STRVAL | OPT_STATIC,
	  devnam},
	{ "llc-encaps", o_bool, &glb.llc_encaps,
	  "use LLC encapsulation for PPPoATM", 1},
	{ "vc-encaps", o_bool, &glb.vc_encaps,
	  "use VC multiplexing for PPPoATM (default)", 1},
	{ "qos", o_string, &glb.qosstr,
	  "set QoS for PPPoATM connection", 1},
	{ "qos_pcr", o_string, &glb.qos_pcr,
	  "set QoS PCR for PPPoATM connection", 1},
	{ "qos_scr", o_string, &glb.qos_scr,
	  "set QoS SCR for PPPoATM connection", 1},
	{ "qos_mbs", o_string, &glb.qos_mbs,
	  "set QoS MBS for PPPoATM connection", 1},  
	{ NULL }
};

static int connect_pppoatm_ses(void)
{
	info("connect_pppoatm_ses...Enter");
	glb.real_ttyfd = socket(AF_ATMPVC, SOCK_DGRAM, 0);
	if (glb.real_ttyfd < 0)
		fatal("failed to create socket: %m");
	info("connect_pppoatm_ses: socket created fd - 0x%x.",glb.real_ttyfd);

	memset(&glb.qos, 0, sizeof glb.qos);
	glb.qos.txtp.traffic_class = glb.qos.rxtp.traffic_class = ATM_UBR;
	/* TODO: support simplified QoS setting */
	if (glb.qosstr != NULL) {
	  if (text2qos(glb.qosstr, &glb.qos, 0))
	    fatal("Can't parse QoS: \"%s\"");

	fprintf(stderr, "glb.qos.rxtp.traffic_class = %d\n", glb.qos.rxtp.traffic_class); 

	  glb.qos.txtp.pcr = glb.qos.rxtp.pcr = atoi(glb.qos_pcr);
	  info("glb.qos.txtp.pcr = glb.qos.rxtp.pcr = %d", glb.qos.txtp.pcr);
	  if ((strcmp(glb.qosstr, "VBR") == 0) || (strcmp(glb.qosstr, "NRTVBR") == 0)) {
	       glb.qos.txtp.scr = glb.qos.rxtp.scr = atoi(glb.qos_scr);
	      info("glb.qos.txtp.scr = glb.qos.rxtp.scr = %d", glb.qos.txtp.scr);
		glb.qos.txtp.mbs = glb.qos.rxtp.mbs = atoi(glb.qos_mbs);
	      info("glb.qos.txtp.mbs = glb.qos.rxtp.mbs = %d", glb.qos.txtp.mbs);
	  }
	}
	glb.qos.txtp.max_sdu = lcp_allowoptions[0].mru + (glb.llc_encaps ? 6 : 2);
	glb.qos.rxtp.max_sdu = lcp_wantoptions[0].mru + (glb.llc_encaps ? 6 : 2);
	glb.qos.aal = ATM_AAL5;
	if (setsockopt(glb.real_ttyfd, SOL_ATM, SO_ATMQOS, &glb.qos, sizeof(glb.qos)) < 0)
		fatal("setsockopt(SO_ATMQOS): %m");
	/* TODO: accept on SVCs... */
	info("connect_pppoatm_ses: setsockopt called.");
	if (connect(glb.real_ttyfd, (struct sockaddr *) &glb.pvcaddr,
	    sizeof(struct sockaddr_atmpvc)))
		fatal("connect(%s): %m", devnam);
	info("connect_pppoatm_ses: connect successful.");
	glb.pppoatm_max_mtu = lcp_allowoptions[0].mru;
	glb.pppoatm_max_mru = lcp_wantoptions[0].mru;
	return glb.real_ttyfd;
}

static void disconnect_pppoatm_ses(void)
{
	pppoatm_close_fds();
}

static int setspeed_pppoatm(const char *cp)
{
	return 0;
}

static void pppoatm_extra_options()
{
    	int ret;
    	char buf[256];
	snprintf(buf, 256, _PATH_ETHOPT "%s",devnam);
	info("Options file - %s.\n", buf);
    	if(!options_from_file(buf, 0, 0, 1))
		exit(EXIT_OPTION_ERROR);
}

static void send_config_pppoatm(int mtu, u_int32_t asyncmap,
	int pcomp, int accomp)
{
	int sock;
	struct ifreq ifr;
	if (mtu > glb.pppoatm_max_mtu)
		error("Couldn't increase MTU to %d", mtu);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
		fatal("Couldn't create IP socket: %m");
	strlcpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
	ifr.ifr_mtu = mtu;
	if (ioctl(sock, SIOCSIFMTU, (caddr_t) &ifr) < 0)
		fatal("ioctl(SIOCSIFMTU): %m");
	(void) close (sock);
}

static void recv_config_pppoatm(int mru, u_int32_t asyncmap,
	int pcomp, int accomp)
{
	if (mru > glb.pppoatm_max_mru)
		error("Couldn't increase MRU to %d", mru);
}

static void set_xaccm_pppoatm(int unit, ext_accm accm)
{
	/* NOTHING */
}

struct channel pppoatm_channel;
/* returns:
 *  -1 if there's a problem with setting the device
 *   0 if we can't parse "cp" as a valid name of a device
 *   1 if "cp" is a reasonable thing to name a device
 * Note that we don't actually open the device at this point
 * We do need to fill in:
 *   devnam: a string representation of the device
 *   devstat: a stat structure of the device.  In this case
 *     we're not opening a device, so we just make sure
 *     to set up S_ISCHR(devstat.st_mode) != 1, so we
 *     don't get confused that we're on stdin.
 */
int (*old_setdevname_hook)(const char* cp) = NULL;
int setdevname_pppoatm(const char *cp)
{
	struct sockaddr_atmpvc addr;
	extern struct stat devstat;
	
	memset(&addr, 0, sizeof addr);
	if (text2atm(cp, (struct sockaddr *) &addr, sizeof(addr),
	    T2A_PVC | T2A_NAME) < 0)
		return 0;

	memcpy(&glb.pvcaddr, &addr, sizeof glb.pvcaddr);
	strlcpy(devnam, cp, sizeof devnam);
	
	info("PPPoATM setdevname_pppoatm - SUCCESS");
	if(the_channel != &pppoatm_channel) {
		the_channel = &pppoatm_channel;	
		
		{
			char **a;
			for (a = bad_options; *a != NULL; a++)
				remove_option(*a);
		}
#ifndef _DISABLE_SERIAL_
		modem = 0;
#endif

		lcp_allowoptions[0].neg_accompression = 0;
		lcp_wantoptions[0].neg_accompression = 0;

		lcp_allowoptions[0].neg_asyncmap = 0;
		lcp_wantoptions[0].neg_asyncmap = 0;

		lcp_allowoptions[0].neg_pcompression = 0;
		lcp_wantoptions[0].neg_pcompression = 0;

		ccp_allowoptions[0].deflate = 0 ;
		ccp_wantoptions[0].deflate = 0 ;

		ipcp_allowoptions[0].neg_vj=0;
		ipcp_wantoptions[0].neg_vj=0;

		ccp_allowoptions[0].bsd_compress = 0;
		ccp_wantoptions[0].bsd_compress = 0;
	}
	return 1;
}

static void set_line_discipline_pppoatm(int fd)
{
	struct atm_backend_ppp be;
	be.backend_num = ATM_BACKEND_PPP;

	info("Setting pppoatm line discipline.");
	if (!glb.llc_encaps) {
		info("vc encaps.\n");
		be.encaps = PPPOATM_ENCAPS_VC;
	}
	else if (!glb.vc_encaps) {
		info("LLC encaps.\n");
		be.encaps = PPPOATM_ENCAPS_LLC;
	}
	else {
		info("Autodetect encaps.\n");
		be.encaps = PPPOATM_ENCAPS_AUTODETECT;
	}
	if (ioctl(fd, ATM_SETBACKEND, &be) < 0)
		fatal("ioctl(ATM_SETBACKEND): %m");
}

static void reset_line_discipline_pppoatm(int fd)
{
	atm_backend_t be = ATM_BACKEND_RAW;
	/* 2.4 doesn't support this yet */
	(void) ioctl(fd, ATM_SETBACKEND, &be);
}

char *get_pppd_version(void)
{
	return ((char *)pppd_version);
}

void pppoatm_plugin_init(void)
{
/*
     	fatal("PPPoA plugin loading...");
*/ 
	
#if _linux_
    	if (!ppp_available() && !new_style_driver)
		fatal("Kernel doesn't support ppp_generic - "
		    "needed for PPPoATM");
#else
	fatal("No PPPoATM support on this OS");
#endif
    	add_options(pppoatm_options);

    	info("PPPoA Plugin Initialized");
}

static void pppoatm_close_fds()
{
  	if (glb.real_ttyfd >= 0) {
    	close(glb.real_ttyfd);
    	glb.real_ttyfd = -1;
  	}
}

static void cleanup_pppoatm()
{
  	pppoatm_close_fds();
}

struct channel pppoatm_channel = {
    options: pppoatm_options,
    process_extra_options: &pppoatm_extra_options,
    check_options: NULL,
    connect: connect_pppoatm_ses,
    disconnect: disconnect_pppoatm_ses,
    establish_ppp: &generic_establish_ppp,
    disestablish_ppp: &generic_disestablish_ppp,
    send_config: &send_config_pppoatm,
    recv_config: &recv_config_pppoatm,
    set_line_discipline_hook: &set_line_discipline_pppoatm,
    reset_line_discipline_hook: &reset_line_discipline_pppoatm,
  	cleanup: &cleanup_pppoatm,
	close: &pppoatm_close_fds
};
