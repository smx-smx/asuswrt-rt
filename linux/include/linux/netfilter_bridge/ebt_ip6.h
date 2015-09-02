/*
 *  ebt_ip6
 *
 *	Authors:
 * Kuo-Lang Tseng <kuo-lang.tseng@intel.com>
 * Manohar Castelino <manohar.r.castelino@intel.com>
 *
 *  Jan 11, 2008
 *
 */

#ifndef __LINUX_BRIDGE_EBT_IP6_H
#define __LINUX_BRIDGE_EBT_IP6_H

#define EBT_IP6_SOURCE 0x01
#define EBT_IP6_DEST 0x02
#define EBT_IP6_TCLASS 0x04
#define EBT_IP6_PROTO 0x08
#define EBT_IP6_SPORT 0x10
#define EBT_IP6_DPORT 0x20

#if !defined(TCSUPPORT_CZ_GENERAL) 
#define EBT_IP6_MASK (EBT_IP6_SOURCE | EBT_IP6_DEST | EBT_IP6_TCLASS |\
		      EBT_IP6_PROTO | EBT_IP6_SPORT | EBT_IP6_DPORT)
#endif
#define EBT_IP6_MATCH "ip6"

/* the same values are used for the invflags */
struct ebt_ip6_info
{
	struct in6_addr saddr;
	struct in6_addr daddr;
	struct in6_addr smsk;
	struct in6_addr dmsk;
#if 0
	uint8_t  tclass;
#else
	uint8_t	 tclass[2];
#endif
#if 0
	uint8_t  protocol;
#else
	uint8_t	 protocol[2];
#endif
	uint8_t  bitmask;
	uint8_t  invflags;
	uint16_t sport[2];
	uint16_t dport[2];
};

#endif
