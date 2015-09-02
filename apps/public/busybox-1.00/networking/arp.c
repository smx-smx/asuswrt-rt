/*
 * arp.c - Manipulate the system ARP cache
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * Author: Fred N. van Kempen, <waltje at uwalt.nl.mugnet.org>
 * Busybox port: Paul van Gool <pvangool at mimotech.com>
 */

#include <sys/ioctl.h>
#include <sys/signal.h>
#include <sys/time.h>

#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/ether.h>
#include <netpacket/packet.h>

#include "busybox.h"
#include "inet_common.h"

#define APPLET_NAME "arp"

#define DFLT_AF "inet"
#define DFLT_HW "ether"

#define _PATH_PROCNET_ARP "/proc/net/arp"

extern struct aftype *get_aftype(const char *name);
extern struct hwtype *get_hwntype(int type);
extern struct hwtype *get_hwtype(const char *name);

int opt_n = 0;                  /* do not resolve addresses     */
int opt_v = 0;                  /* debugging output flag        */
int opt_D = 0;                  /* HW-address is devicename     */
int opt_e = 0;                  /* 0=BSD output, 1=new linux    */
int opt_a = 0;                  /* all entries, substring match */
struct aftype *ap;              /* current address family       */
struct hwtype *hw;              /* current hardware type        */
int sockfd = 0;                 /* active socket descriptor     */
int hw_set = 0;                 /* flag if hw-type was set (-H) */
char device[16] = "";           /* current device               */

/* Split the input string into multiple fields. */
int getargs(char *string, char *arguments[])
{
    int len = strlen(string); 
    char temp[len+1];
    char *sp, *ptr;
    int i, argc;
    char want;

    /*
     * Copy the string into a buffer.  We may have to modify
     * the original string because of all the quoting...
     */
    sp = string;
    i = 0;
    strcpy(temp, string);
    ptr = temp;

    /*
     * Look for delimiters ("); if present whatever
     * they enclose will be considered one argument.
     */
    while (*ptr != '\0' && i < 31) {
	/* Ignore leading whitespace on input string. */
	while (*ptr == ' ' || *ptr == '\t')
	    ptr++;

	/* Set string pointer. */
	arguments[i++] = sp;

	/* Check for any delimiters. */
	if (*ptr == '"' || *ptr == '\'') {
	    /*
	     * Copy the string up to any whitespace OR the next
	     * delimiter. If the delimiter was escaped, skip it
	     * as it if was not there.
	     */
	    want = *ptr++;
	    while (*ptr != '\0') {
		if (*ptr == want && *(ptr - 1) != '\\') {
		    ptr++;
		    break;
		}
		*sp++ = *ptr++;
	    }
	} else {
	    /* Just copy the string up to any whitespace. */
	    while (*ptr != '\0' && *ptr != ' ' && *ptr != '\t')
		*sp++ = *ptr++;
	}
	*sp++ = '\0';

	/* Skip trailing whitespace. */
	if (*ptr != '\0') {
	    while (*ptr == ' ' || *ptr == '\t')
		ptr++;
	}
    }
    argc = i;
    while (i < 32)
	arguments[i++] = (char *) NULL;
    return (argc);
}

/* Delete an entry from the ARP cache. */
static int arp_del(char **args)
{
    char host[128];
    struct arpreq req;
    struct sockaddr sa;
    int flags = 0;
    int err;

    memset((char *) &req, 0, sizeof(req));

    /* Resolve the host name. */
    if (*args == NULL) {
        bb_error_msg("need host name\n");
        return -1;
    }

    safe_strncpy(host, *args, (sizeof host));
    if (ap->input(0, host, &sa) < 0) {
        bb_herror_msg("%s", host);
        return -1;
    }

    /* If a host has more than one address, use the correct one! */
    memcpy((char *) &req.arp_pa, (char *) &sa, sizeof(struct sockaddr));

    if (hw_set)
        req.arp_ha.sa_family = hw->type;

    req.arp_flags = ATF_PERM;
    args++;
    while (*args != NULL) {
        if (opt_v)
            bb_error_msg("args=%s\n", *args);
        if (!strcmp(*args, "pub")) {
            flags |= 1;
            args++;
            continue;
        }
        if (!strcmp(*args, "priv")) {
            flags |= 2;
            args++;
            continue;
        }
        if (!strcmp(*args, "temp")) {
            req.arp_flags &= ~ATF_PERM;
            args++;
            continue;
        }
        if (!strcmp(*args, "trail")) {
            req.arp_flags |= ATF_USETRAILERS;
            args++;
            continue;
        }
        if (!strcmp(*args, "dontpub")) {
#ifdef HAVE_ATF_DONTPUB
            req.arp_flags |= ATF_DONTPUB;
#else
            bb_error_msg("feature ATF_DONTPUB is not supported");
#endif
            args++;
            continue;
        }
        if (!strcmp(*args, "auto")) {
#ifdef HAVE_ATF_MAGIC
            req.arp_flags |= ATF_MAGIC;
#else
	    bb_error_msg("feature ATF_MAGIC is not supported");
#endif
            args++;
            continue;
        }
        if (!strcmp(*args, "dev")) {
            if (*++args == NULL)
                bb_show_usage();
            safe_strncpy(device, *args, sizeof(device));
            args++;
            continue;
        }
        if (!strcmp(*args, "netmask")) {
            if (*++args == NULL)
                bb_show_usage();
            if (strcmp(*args, "255.255.255.255") != 0) {
                strcpy(host, *args);
                if (ap->input(0, host, &sa) < 0) {
                    bb_herror_msg("%s", host);
                    return (-1);
                }
                memcpy((char *) &req.arp_netmask, (char *) &sa,
                       sizeof(struct sockaddr));
                req.arp_flags |= ATF_NETMASK;
            }
            args++;
            continue;
        }
        bb_show_usage();
    }
    if (flags == 0)
        flags = 3;

    strcpy(req.arp_dev, device);

    err = -1;

    /* Call the kernel. */
    if (flags & 2) {
        if (opt_v)
            bb_error_msg("SIOCDARP(nopub)\n");
        if ((err = ioctl(sockfd, SIOCDARP, &req) < 0)) {
            if (errno == ENXIO) {
                if (flags & 1)
                    goto nopub;
                printf("No ARP entry for %s\n", host);
                return -1;
            }
            perror("SIOCDARP(priv)");
            return -1;
        }
    }
    if ((flags & 1) && (err)) {
      nopub:
        req.arp_flags |= ATF_PUBL;
        if (opt_v)
            bb_error_msg("SIOCDARP(pub)\n");
        if (ioctl(sockfd, SIOCDARP, &req) < 0) {
            if (errno == ENXIO) {
                printf("No ARP entry for %s\n", host);
                return -1;
            }
            perror("SIOCDARP(pub)");
            return -1;
        }
    }
    return 0;
}

/* Get the hardware address to a specified interface name */
static int arp_getdevhw(char *ifname, struct sockaddr *sa, struct hwtype *hwt)
{
    struct ifreq ifr;
    struct hwtype *xhw;

    strcpy(ifr.ifr_name, ifname);
    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0) {
        bb_error_msg("cant get HW-Address for `%s': %s.\n",
              ifname, strerror(errno));
        return -1;
    }
    if (hwt && (ifr.ifr_hwaddr.sa_family != hw->type)) {
        bb_error_msg("protocol type mismatch.\n");
        return -1;
    }
    memcpy((char *) sa, (char *) &(ifr.ifr_hwaddr), sizeof(struct sockaddr));

    if (opt_v) {
       if (!(xhw = get_hwntype(ifr.ifr_hwaddr.sa_family)) ||
             (xhw->print == 0))
       {
          xhw = get_hwntype(-1);
       }
       bb_error_msg("device `%s' has HW address %s `%s'.\n",
             ifname, xhw->name, xhw->print((char *)&ifr.ifr_hwaddr.sa_data));
    }
    return 0;
}

/* Set an entry in the ARP cache. */
static int arp_set(char **args)
{
    char host[128];
    struct arpreq req;
    struct sockaddr sa;
    int flags;

    memset((char *) &req, 0, sizeof(req));

    /* Resolve the host name. */
    if (*args == NULL) {
        bb_error_msg("need host name\n");
        return -1;
    }
    safe_strncpy(host, *args++, (sizeof host));
    if (ap->input(0, host, &sa) < 0) {
        bb_herror_msg("%s", host);
        return -1;
    }
    /* If a host has more than one address, use the correct one! */
    memcpy((char *) &req.arp_pa, (char *) &sa, sizeof(struct sockaddr));

    /* Fetch the hardware address. */
    if (*args == NULL) {
        bb_error_msg("need hardware address\n");
        return -1;
    }
    if (opt_D) {
        if (arp_getdevhw(*args++, &req.arp_ha, hw_set ? hw : NULL) < 0)
            return (-1);
    } else {
        if (hw->input(*args++, &req.arp_ha) < 0) {
            bb_error_msg("invalid hardware address\n");
            return -1;
        }
    }

    /* Check out any modifiers. */
    flags = ATF_PERM | ATF_COM;
    while (*args != NULL) {
        if (!strcmp(*args, "temp")) {
            flags &= ~ATF_PERM;
            args++;
            continue;
        }
        if (!strcmp(*args, "pub")) {
            flags |= ATF_PUBL;
            args++;
            continue;
        }
        if (!strcmp(*args, "priv")) {
            flags &= ~ATF_PUBL;
            args++;
            continue;
        }
        if (!strcmp(*args, "trail")) {
            flags |= ATF_USETRAILERS;
            args++;
            continue;
        }
        if (!strcmp(*args, "dontpub")) {
#ifdef HAVE_ATF_DONTPUB
            flags |= ATF_DONTPUB;
#else
	    bb_error_msg("feature ATF_DONTPUB is not supported");
#endif
            args++;
            continue;
        }
        if (!strcmp(*args, "auto")) {
#ifdef HAVE_ATF_MAGIC
            flags |= ATF_MAGIC;
#else
	    bb_error_msg("feature ATF_MAGIC is not supported");
#endif
            args++;
            continue;
        }
        if (!strcmp(*args, "dev")) {
            if (*++args == NULL)
                bb_show_usage();
            safe_strncpy(device, *args, sizeof(device));
            args++;
            continue;
        }
        if (!strcmp(*args, "netmask")) {
            if (*++args == NULL)
                bb_show_usage();
            if (strcmp(*args, "255.255.255.255") != 0) {
                strcpy(host, *args);
                if (ap->input(0, host, &sa) < 0) {
                    bb_herror_msg("%s", host);
                    return (-1);
                }
                memcpy((char *) &req.arp_netmask, (char *) &sa,
                       sizeof(struct sockaddr));
                flags |= ATF_NETMASK;
            }
            args++;
            continue;
        }
        bb_show_usage();
    }

    /* Fill in the remainder of the request. */
    req.arp_flags = flags;

    strcpy(req.arp_dev, device);

    /* Call the kernel. */
    if (opt_v)
        bb_error_msg("SIOCSARP()\n");
    if (ioctl(sockfd, SIOCSARP, &req) < 0) {
        perror("SIOCSARP");
        return -1;
    }
    return 0;
}

/* Process an EtherFile */
static int arp_file(char *name)
{
    char buff[1024];
    char *sp, *args[32];
    int linenr, argc;
    FILE *fp;

    if ((fp = fopen(name, "r")) == NULL) {
        bb_error_msg("cannot open etherfile %s!\n", name);
        return -1;
    }

    /* Read the lines in the file. */
    linenr = 0;
    while (fgets(buff, sizeof(buff), fp) != (char *) NULL) {
        linenr++;
        if (opt_v == 1)
            fprintf(stderr, ">> %s", buff);
        if ((sp = strchr(buff, '\n')) != (char *) NULL)
            *sp = '\0';
        if (buff[0] == '#' || buff[0] == '\0')
            continue;

        argc = getargs(buff, args);
        if (argc < 2) {
            bb_error_msg("format error on line %u of etherfile %s!\n",
                    linenr, name);
            continue;
        }
        if (strchr (args[0], ':') != NULL) {
            /* We have a correct ethers file, switch hw adress and hostname
               for arp */
            char *cp;
            cp = args[1];
            args[1] = args[0];
            args[0] = cp;
        }
        if (arp_set(args) != 0)
            bb_error_msg("cannot set entry on line %u of etherfile %s!\n",
                    linenr, name);
    }

    (void) fclose(fp);

    return 0;
}

/* Print the contents of an ARP request block. */
static void arp_disp_2(char *name, int type, int arp_flags, char *hwa,
      char *mask, char *dev)
{
    static int title = 0;
    struct hwtype *xhw;
    char flags[10];

    xhw = get_hwntype(type);
    if (xhw == NULL)
        xhw = get_hwtype(DFLT_HW);

    if (title++ == 0) {
        printf("Address                  HWtype  HWaddress           Flags Mask            Iface\n");
    }
    /* Setup the flags. */
    flags[0] = '\0';
    if (arp_flags & ATF_COM)
        strcat(flags, "C");
    if (arp_flags & ATF_PERM)
        strcat(flags, "M");
    if (arp_flags & ATF_PUBL)
        strcat(flags, "P");
#ifdef HAVE_ATF_MAGIC
    if (arp_flags & ATF_MAGIC)
        strcat(flags, "A");
#endif
#ifdef HAVE_ATF_DONTPUB
    if (arp_flags & ATF_DONTPUB)
        strcat(flags, "!");
#endif
    if (arp_flags & ATF_USETRAILERS)
        strcat(flags, "T");

    if (!(arp_flags & ATF_NETMASK))
        mask = "";

    printf("%-23.23s  ", name);

    if (!(arp_flags & ATF_COM)) {
        if (arp_flags & ATF_PUBL)
            printf("%-8.8s%-20.20s", "*", "*");
        else
            printf("%-8.8s%-20.20s", "", "(incomplete)");
    } else {
        printf("%-8.8s%-20.20s", xhw->name, hwa);
    }

    printf("%-6.6s%-15.15s %s\n", flags, mask, dev);
}

/* Print the contents of an ARP request block. */
static void arp_disp(char *name, char *ip, int type, int arp_flags,
      char *hwa, char *mask, char *dev)
{
    struct hwtype *xhw;

    xhw = get_hwntype(type);
    if (xhw == NULL)
        xhw = get_hwtype(DFLT_HW);

    printf("%s (%s) at ", name, ip);

    if (!(arp_flags & ATF_COM)) {
        if (arp_flags & ATF_PUBL)
            printf("* ");
        else
            printf("<incomplete> ");
    } else {
        printf("%s [%s] ", hwa, xhw->name);
    }

    if (arp_flags & ATF_NETMASK)
        printf("netmask %s ", mask);

    if (arp_flags & ATF_PERM)
        printf("PERM ");
    if (arp_flags & ATF_PUBL)
        printf("PUP ");
#ifdef HAVE_ATF_MAGIC
    if (arp_flags & ATF_MAGIC)
        printf("AUTO ");
#endif
#ifdef HAVE_ATF_DONTPUB
    if (arp_flags & ATF_DONTPUB)
        printf("DONTPUB ");
#endif
    if (arp_flags & ATF_USETRAILERS)
        printf("TRAIL ");

    printf("on %s\n", dev);
}

/* Display the contents of the ARP cache in the kernel. */
static int arp_show(char *name)
{
    char host[100];
    struct sockaddr sa;
    char ip[100];
    char hwa[100];
    char mask[100];
    char line[200];
    char dev[100];
    int type, flags;
    FILE *fp;
    char *hostname;
    int num, entries = 0, showed = 0;

    host[0] = '\0';

    if (name != NULL) {
        /* Resolve the host name. */
        safe_strncpy(host, name, (sizeof host));
        if (ap->input(0, host, &sa) < 0) {
            bb_herror_msg("%s", host);
            return -1;
        }
        safe_strncpy(host, ap->sprint(&sa, 1), sizeof(host));
    }
    /* Open the PROCps kernel table. */
    if ((fp = fopen(_PATH_PROCNET_ARP, "r")) == NULL) {
        perror(_PATH_PROCNET_ARP);
        return -1;
    }
    /* Bypass header -- read until newline */
    if (fgets(line, sizeof(line), fp) != (char *) NULL) {
        strcpy(mask, "-");
        strcpy(dev, "-");
        /* Read the ARP cache entries. */
        for (; fgets(line, sizeof(line), fp);) {
            num = sscanf(line, "%s 0x%x 0x%x %100s %100s %100s\n",
                         ip, &type, &flags, hwa, mask, dev);
            if (num < 4)
                break;

            entries++;
            /* if the user specified hw-type differs, skip it */
            if (hw_set && (type != hw->type))
                continue;

            /* if the user specified address differs, skip it */
            if (host[0] && strcmp(ip, host))
                continue;

            /* if the user specified device differs, skip it */
            if (device[0] && strcmp(dev, device))
                continue;

            showed++;
            /* This IS ugly but it works -be */
            if (opt_n)
                hostname = "?";
            else {
                if (ap->input(0, ip, &sa) < 0)
                    hostname = ip;
                else
                    hostname = ap->sprint(&sa, opt_n | 0x8000);
                if (strcmp(hostname, ip) == 0)
                    hostname = "?";
            }

            if (opt_e)
                arp_disp_2(hostname[0] == '?' ? ip : hostname, type, flags,
                      hwa, mask, dev);
            else
                arp_disp(hostname, ip, type, flags, hwa, mask, dev);
        }
    }
    if (opt_v)
        printf("Entries: %d\tSkipped: %d\tFound: %d\n",
              entries, entries - showed, showed);

    if (!showed) {
        if (host[0] && !opt_a)
            printf("%s (%s) -- no entry\n", name, host);
        else if (hw_set || host[0] || device[0]) {
            printf("in %d entries no match found.\n", entries);
        }
    }

    (void) fclose(fp);

    return 0;
}

int arp_main(int argc, char **argv)
{
   int what, i;

   /* Initialize variables... */
   if ((hw = get_hwtype(DFLT_HW)) == NULL) {
      bb_error_msg("%s: hardware type not supported!\n", DFLT_HW);
      return -1;
   }
   if ((ap = get_aftype(DFLT_AF)) == NULL) {
      bb_error_msg("%s: address family not supported!\n", DFLT_AF);
      return -1;
   }
   what = 0;

   /* Fetch the command-line arguments. */
   while ((i = getopt(argc, argv, "A:H:adfp:nsei:t:vh?DNV")) != EOF)
      switch (i) {
         case 'a':
            what = 1;
            opt_a = 1;
            break;
         case 'f':
            what = 2;
            break;
         case 'd':
            what = 3;
            break;
         case 's':
            what = 4;
            break;
         case 'e':
            opt_e = 1;
            break;
         case 'n':
            opt_n = 1;
            break;
         case 'D':
            opt_D = 1;
            break;
         case 'v':
            opt_v = 1;
            break;
         case 'A':
         case 'p':
            ap = get_aftype(optarg);
            if (ap == NULL) {
               bb_error_msg("%s: unknown address family.\n", optarg);
               exit(-1);
            }
            break;
         case 'H':
         case 't':
            hw = get_hwtype(optarg);
            if (hw == NULL) {
               bb_error_msg("%s: unknown hardware type.\n", optarg);
               exit(-1);
            }
            hw_set = 1;
            break;
         case 'i':
            safe_strncpy(device, optarg, sizeof(device));
            break;
         case '?':
         case 'h':
         default:
            bb_show_usage();
      }

   if (ap->af != AF_INET) {
      bb_error_msg("%s: kernel only supports 'inet'.\n", ap->name);
      exit(-1);
   }

   /* If not hw type specified get default */
   if (hw_set==0) {
      if ((hw = get_hwtype(DFLT_HW)) == NULL) {
         bb_error_msg("%s: hardware type not supported!\n", DFLT_HW);
         return (-1);
      }
   }

   if (hw->alen <= 0) {
      bb_error_msg("%s: hardware type without ARP support.\n", hw->name);
      exit(-1);
   }
   if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("socket");
      return -1;
   }
   /* Now see what we have to do here... */
   switch (what) {
      case 0:
         opt_e = 1;
         what = arp_show(argv[optind]);
         break;

      case 1:                     /* show an ARP entry in the cache */
         what = arp_show(argv[optind]);
         break;

      case 2:                     /* process an EtherFile */
         what = arp_file(argv[optind] ? argv[optind] : "/etc/ethers");
         break;

      case 3:                     /* delete an ARP entry from the cache */
         what = arp_del(&argv[optind]);
         break;

      case 4:                     /* set an ARP entry in the cache */
         what = arp_set(&argv[optind]);
         break;

      default:
         bb_show_usage();
   }

   return what;
}

