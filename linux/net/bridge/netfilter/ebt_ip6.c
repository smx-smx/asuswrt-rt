/*
 *  ebt_ip6
 *
 *	Authors:
 *	Manohar Castelino <manohar.r.castelino@intel.com>
 *	Kuo-Lang Tseng <kuo-lang.tseng@intel.com>
 *	Jan Engelhardt <jengelh@computergmbh.de>
 *
 * Summary:
 * This is just a modification of the IPv4 code written by
 * Bart De Schuymer <bdschuym@pandora.be>
 * with the changes required to support IPv6
 *
 *  Jan, 2008
 */
#include <linux/ipv6.h>
#include <net/ipv6.h>
#include <linux/in.h>
#include <linux/module.h>

#include <net/dsfield.h>
#if 0
#include <linux/netfilter/x_tables.h>
#endif

#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_ip6.h>

struct tcpudphdr {
	__be16 src;
	__be16 dst;
};
#if 1  /*Rodney_20090724*/
static inline __be16 vlan_proto(const struct sk_buff *skb)
{
	return vlan_eth_hdr(skb)->h_vlan_encapsulated_proto;
}
static inline __be16 pppoe_proto(const struct sk_buff *skb)
{
	return *((__be16 *)(skb_mac_header(skb) + ETH_HLEN +
			    sizeof(struct pppoe_hdr)));
}
#endif

/********************************add for compile error*******************/
/*
 * find out if nexthdr is a well-known extension header or a protocol
 */

int ipv6_ext_hdr(u8 nexthdr)
{
	/*
	 * find out if nexthdr is an extension header or a protocol
	 */
	return ( (nexthdr == NEXTHDR_HOP)	||
		 (nexthdr == NEXTHDR_ROUTING)	||
		 (nexthdr == NEXTHDR_FRAGMENT)	||
		 (nexthdr == NEXTHDR_AUTH)	||
		 (nexthdr == NEXTHDR_NONE)	||
		 (nexthdr == NEXTHDR_DEST) );
}

/*
 * Skip any extension headers. This is used by the ICMP module.
 *
 * Note that strictly speaking this conflicts with RFC 2460 4.0:
 * ...The contents and semantics of each extension header determine whether
 * or not to proceed to the next header.  Therefore, extension headers must
 * be processed strictly in the order they appear in the packet; a
 * receiver must not, for example, scan through a packet looking for a
 * particular kind of extension header and process that header prior to
 * processing all preceding ones.
 *
 * We do exactly this. This is a protocol bug. We can't decide after a
 * seeing an unknown discard-with-error flavour TLV option if it's a
 * ICMP error message or not (errors should never be send in reply to
 * ICMP error messages).
 *
 * But I see no other way to do this. This might need to be reexamined
 * when Linux implements ESP (and maybe AUTH) headers.
 * --AK
 *
 * This function parses (probably truncated) exthdr set "hdr".
 * "nexthdrp" initially points to some place,
 * where type of the first header can be found.
 *
 * It skips all well-known exthdrs, and returns pointer to the start
 * of unparsable area i.e. the first header with unknown type.
 * If it is not NULL *nexthdr is updated by type/protocol of this header.
 *
 * NOTES: - if packet terminated with NEXTHDR_NONE it returns NULL.
 *        - it may return pointer pointing beyond end of packet,
 *	    if the last recognized header is truncated in the middle.
 *        - if packet is truncated, so that all parsed headers are skipped,
 *	    it returns NULL.
 *	  - First fragment header is skipped, not-first ones
 *	    are considered as unparsable.
 *	  - ESP is unparsable for now and considered like
 *	    normal payload protocol.
 *	  - Note also special handling of AUTH header. Thanks to IPsec wizards.
 *
 * --ANK (980726)
 */

int ipv6_skip_exthdr(const struct sk_buff *skb, int start, u8 *nexthdrp)
{
	u8 nexthdr = *nexthdrp;

	while (ipv6_ext_hdr(nexthdr)) {
		struct ipv6_opt_hdr _hdr, *hp;
		int hdrlen;

		if (nexthdr == NEXTHDR_NONE)
			return -1;
		hp = skb_header_pointer(skb, start, sizeof(_hdr), &_hdr);
		if (hp == NULL)
			return -1;
		if (nexthdr == NEXTHDR_FRAGMENT) {
			__be16 _frag_off, *fp;
			fp = skb_header_pointer(skb,
						start+offsetof(struct frag_hdr,
							       frag_off),
						sizeof(_frag_off),
						&_frag_off);
			if (fp == NULL)
				return -1;

			if (ntohs(*fp) & ~0x7)
				break;
			hdrlen = 8;
		} else if (nexthdr == NEXTHDR_AUTH)
			hdrlen = (hp->hdrlen+2)<<2;
		else
			hdrlen = ipv6_optlen(hp);

		nexthdr = hp->nexthdr;
		start += hdrlen;
	}

	*nexthdrp = nexthdr;
	return start;
}
/********************************add for compile error*******************/

#if 0
static bool
ebt_ip6_mt(const struct sk_buff *skb, const struct xt_match_param *par)
#else
static int ebt_filter_ip6(const struct sk_buff *skb, const struct net_device *in,
   const struct net_device *out, const void *data,
   unsigned int datalen)
#endif
{
#if 0
	const struct ebt_ip6_info *info = par->matchinfo;
#else
	struct ebt_ip6_info *info = (struct ebt_ip6_info *)data;
#endif
	const struct ipv6hdr *ih6;
	struct ipv6hdr _ip6h;
	const struct tcpudphdr *pptr;
	struct tcpudphdr _ports;
	struct in6_addr tmp_addr;
	int i;

	//printk("==>ebt_filter_ip6().\n");
	
#if 1 
	if(skb->protocol == htons(ETH_P_IPV6))
		ih6 = (struct ipv6hdr *)skb_header_pointer(skb, 0, sizeof(_ip6h), &_ip6h);
	else if((skb->protocol == htons(ETH_P_8021Q)) && (vlan_proto(skb) == htons(ETH_P_IPV6)))
		ih6 = (struct ipv6hdr *)(skb_mac_header(skb) + VLAN_ETH_HLEN);
	else if((skb->protocol == htons(ETH_P_PPP_SES)) && (pppoe_proto(skb) == htons(0x0021)))
		ih6 = (struct ipv6hdr *)(skb_mac_header(skb) + ETH_HLEN +PPPOE_SES_HLEN);
	else
		ih6 = (struct ipv6hdr *)skb_header_pointer(skb, 0, sizeof(_ip6h), &_ip6h);
#else
	ih6 = skb_header_pointer(skb, 0, sizeof(_ip6h), &_ip6h);
#endif
	
	if (ih6 == NULL)
		//return false;
		return EBT_NOMATCH;
#if 0
	if (info->bitmask & EBT_IP6_TCLASS &&
	   FWINV(info->tclass != ipv6_get_dsfield((struct ipv6hdr *)ih6), EBT_IP6_TCLASS))
		//return false;
		return EBT_NOMATCH;
#else
	if (info->bitmask & EBT_IP6_TCLASS) {
		__u8 tc = ipv6_get_dsfield((struct ipv6hdr *)ih6);
	   if (FWINV(tc < info->tclass[0] || 
	   		 tc > info->tclass[1], EBT_IP6_TCLASS))
		//return false;
		return EBT_NOMATCH;
	}
#endif

	for (i = 0; i < 4; i++)
		tmp_addr.in6_u.u6_addr32[i] = ih6->saddr.in6_u.u6_addr32[i] &
			info->smsk.in6_u.u6_addr32[i];
	if (info->bitmask & EBT_IP6_SOURCE &&
		FWINV((ipv6_addr_cmp(&tmp_addr, &info->saddr) != 0),
			EBT_IP6_SOURCE))
		//return false;
		return EBT_NOMATCH;
	for (i = 0; i < 4; i++)
		tmp_addr.in6_u.u6_addr32[i] = ih6->daddr.in6_u.u6_addr32[i] &
			info->dmsk.in6_u.u6_addr32[i];
	if (info->bitmask & EBT_IP6_DEST &&
	   FWINV((ipv6_addr_cmp(&tmp_addr, &info->daddr) != 0), EBT_IP6_DEST))
		//return false;
		return EBT_NOMATCH;
	if (info->bitmask & EBT_IP6_PROTO) {
		uint8_t nexthdr = ih6->nexthdr;
		int offset_ph;

		offset_ph = ipv6_skip_exthdr(skb, sizeof(_ip6h), &nexthdr);
		if (offset_ph == -1)
			//return false;
			return EBT_NOMATCH;
#if 0
		if (FWINV(info->protocol != nexthdr, EBT_IP6_PROTO))
			//return false;
			return EBT_NOMATCH;
#else
		if ( FWINV((info->protocol[0] != nexthdr) && (info->protocol[1] != nexthdr), EBT_IP6_PROTO) )
			return EBT_NOMATCH;
#endif
		if (!(info->bitmask & EBT_IP6_DPORT) &&
		    !(info->bitmask & EBT_IP6_SPORT) 
			)
			//return true;
			return EBT_MATCH;
#if 1  
		if(skb->protocol == htons(ETH_P_IP))
			pptr = (struct tcpudphdr *)skb_header_pointer(skb, offset_ph, sizeof(_ports), &_ports);
		else if((skb->protocol == htons(ETH_P_8021Q)) && (vlan_proto(skb) == htons(ETH_P_IP)))
			pptr = (struct tcpudphdr *)(skb_mac_header(skb) + VLAN_ETH_HLEN + offset_ph);
		else if((skb->protocol == htons(ETH_P_PPP_SES)) && (pppoe_proto(skb) == htons(0x0021)))
			pptr = (struct tcpudphdr *)(skb_mac_header(skb) + ETH_HLEN + PPPOE_SES_HLEN + offset_ph);
		else
			pptr = (struct tcpudphdr *)skb_header_pointer(skb, offset_ph, sizeof(_ports), &_ports);
#else
		pptr = skb_header_pointer(skb, offset_ph, sizeof(_ports),
					  &_ports);
#endif
		if (pptr == NULL)
			//return false;
			return EBT_NOMATCH;
		if (info->bitmask & EBT_IP6_DPORT) {
			u32 dst = ntohs(pptr->dst);
			if (FWINV(dst < info->dport[0] ||
				  dst > info->dport[1], EBT_IP6_DPORT))
				//return false;
				return EBT_NOMATCH;
		}
		if (info->bitmask & EBT_IP6_SPORT) {
			u32 src = ntohs(pptr->src);
			if (FWINV(src < info->sport[0] ||
				  src > info->sport[1], EBT_IP6_SPORT))
			//return false;
			return EBT_NOMATCH;
		}
		//return true;
		return EBT_MATCH;
	}
	//return true;
	return EBT_MATCH;
}
#if 0
static bool ebt_ip6_mt_check(const struct xt_mtchk_param *par)
#else
static int ebt_ip6_check(const char *tablename, unsigned int hookmask,
   const struct ebt_entry *e, void *data, unsigned int datalen)
#endif
{
#if 0
	const struct ebt_entry *e = par->entryinfo;
	struct ebt_ip6_info *info = par->matchinfo;
#else
	struct ebt_ip6_info *info = (struct ebt_ip6_info *)data;
#endif

	
	if (e->ethproto != htons(ETH_P_IPV6) || e->invflags & EBT_IPROTO){
	#if 0
		return false;
	#else
		return -EINVAL;
	#endif
	}
	if (info->bitmask & ~EBT_IP6_MASK || info->invflags & ~EBT_IP6_MASK) {
		return false;
	}
	if (info->bitmask & (EBT_IP6_DPORT | EBT_IP6_SPORT)) {
		if (info->invflags & EBT_IP6_PROTO) {

		#if 0
			return false;
		#else
			return -EINVAL;
		#endif
		}
#if 0
		if (info->protocol != IPPROTO_TCP &&
		    info->protocol != IPPROTO_UDP &&
		    info->protocol != IPPROTO_UDPLITE &&
		    info->protocol != IPPROTO_SCTP &&
		    info->protocol != IPPROTO_DCCP) {
#else
		if (info->protocol[0] != IPPROTO_TCP && info->protocol[1] != IPPROTO_TCP &&
			info->protocol[0] != IPPROTO_UDP && info->protocol[1] != IPPROTO_UDP &&
		    info->protocol[0] != IPPROTO_UDPLITE && info->protocol[1] != IPPROTO_UDPLITE &&
		    info->protocol[0] != IPPROTO_SCTP && info->protocol[1] != IPPROTO_SCTP &&
		    info->protocol[0] != IPPROTO_DCCP && info->protocol[1] != IPPROTO_DCCP ) {
#endif
		#if 0
			return false;
		#else
			return -EINVAL;
		#endif
		}
	}
	if (info->bitmask & EBT_IP6_DPORT && info->dport[0] > info->dport[1]) {
	#if 0
		return false;
	#else
		return -EINVAL;
	#endif
	}
	if (info->bitmask & EBT_IP6_SPORT && info->sport[0] > info->sport[1]) {
	#if 0
		return false;
	#else
		return -EINVAL;
	#endif
	}

#if 1 
	/* support tclass range */
	if (info->bitmask & EBT_IP6_TCLASS && info->tclass[0] > info->tclass[1]) {
	#if 0
		return false;
	#else
		return -EINVAL;
	#endif
	}
#endif
	
#if 0
	return true;
#else
	return 0;
#endif
}

#if 0
static struct xt_match ebt_ip6_mt_reg __read_mostly = {
	.name		= "ip6",
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.match		= ebt_ip6_mt,
	.checkentry	= ebt_ip6_mt_check,
	.matchsize	= XT_ALIGN(sizeof(struct ebt_ip6_info)),
	.me		= THIS_MODULE,
};
#else
static struct ebt_match filter_ip6 =
{
	.name		= EBT_IP6_MATCH,
	.match		= ebt_filter_ip6,
	.check		= ebt_ip6_check,
	.me		= THIS_MODULE,
};
#endif

#if 0
static int __init ebt_ip6_init(void)
{
	return xt_register_match(&ebt_ip6_mt_reg);
}

static void __exit ebt_ip6_fini(void)
{
	xt_unregister_match(&ebt_ip6_mt_reg);
}
#else
static int __init ebt_ip6_init(void)
{
	return ebt_register_match(&filter_ip6);
}

static void __exit ebt_ip6_fini(void)
{
	ebt_unregister_match(&filter_ip6);
}
#endif

module_init(ebt_ip6_init);
module_exit(ebt_ip6_fini);
MODULE_DESCRIPTION("Ebtables: IPv6 protocol packet match");
MODULE_LICENSE("GPL");
