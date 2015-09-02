#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ipv6.h>
#include <net/ipv6.h>
#include <linux/in.h>
#include <net/dsfield.h>
#include <linux/if_vlan.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_tc.h>
#ifdef CONFIG_QOS
#include <linux/qos_type.h>
#endif
//#define QOS_WANIF_MARK	0xff000

#if 1
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

static int ebt_target_tc(struct sk_buff **pskb, unsigned int hooknr,
   const struct net_device *in, const struct net_device *out,
   const void *data, unsigned int datalen)
{
	struct ebt_tc_info *tcinfo = (struct ebt_tc_info *)data;
	const struct ipv6hdr *ih6;
	struct ipv6hdr _ip6h;

#ifdef CONFIG_QOS
	__u8 tos;
	int rule_no;
#endif
	
#if 1  
	if((*pskb)->protocol == htons(ETH_P_IPV6))
		ih6 = (struct ipv6hdr *)skb_header_pointer(*pskb, 0, sizeof(_ip6h), &_ip6h);
	else if(((*pskb)->protocol == htons(ETH_P_8021Q)) && (vlan_proto(*pskb) == htons(ETH_P_IPV6)))
		ih6 = (struct ipv6hdr *)(skb_mac_header(*pskb) + VLAN_ETH_HLEN);
	else if(((*pskb)->protocol == htons(ETH_P_PPP_SES)) && (pppoe_proto(*pskb) == htons(0x0021)))
		ih6 = (struct ipv6hdr *)(skb_mac_header(*pskb) + ETH_HLEN +PPPOE_SES_HLEN);
	else
		ih6 = (struct ipv6hdr *)skb_header_pointer(*pskb, 0, sizeof(_ip6h), &_ip6h);
#else
	ih6 = skb_header_pointer(*pskb, 0, sizeof(_ip6h), &_ip6h);

#endif
	
	if ( ih6 == NULL ) 
		return NF_DROP;
	
	if (!skb_make_writable(pskb, sizeof(struct ipv6hdr)))
		return NF_DROP;

#if 0	
	if ( (*pskb)->mark & QOS_WANIF_MARK ) {
		tos = tcinfo->tc;
		//printk("tos is %d\n", tos);
		(*pskb)->mark |= (tos << 20);
		//printk("mark is %x\n", (tos<<20));
		return tcinfo->target | ~EBT_VERDICT_BITS;
	}
#endif
#ifdef CONFIG_QOS
	rule_no = ((*pskb)->mark & QOS_RULE_INDEX_MARK) >> 12;
	if (0 == qostype_chk(EBT_CHK_TYPE, rule_no, NULL, 0)) {
		tos = tcinfo->tc;
		set_tos(rule_no, tos);
		return tcinfo->target | ~EBT_VERDICT_BITS;
	}
	else {
		unset_tos(rule_no);
	}
#endif
	ipv6_change_dsfield((struct ipv6hdr *)ih6, /*0xFF*/0x0, tcinfo->tc);

    return tcinfo->target | ~EBT_VERDICT_BITS;
}

static int ebt_target_tc_check(const char *tablename, unsigned int hookmask,
   const struct ebt_entry *e, void *data, unsigned int datalen)
{
	struct ebt_tc_info *info = (struct ebt_tc_info *)data;

	//printk("==>ebt_target_tc_check(): info->tc is 0x%x\n", info->tc);
	if (datalen != sizeof(struct ebt_tc_info))
		return -EINVAL;
	if (BASE_CHAIN && info->target == EBT_RETURN)
		return -EINVAL;
	CLEAR_BASE_CHAIN_BIT;
	if (INVALID_TARGET)
		return -EINVAL;
	return 0;
}

static struct ebt_target tc_target =
{
        .name           = EBT_TC_TARGET,
        .target         = ebt_target_tc,
        .check          = ebt_target_tc_check,
        .me             = THIS_MODULE,
};

static int __init init(void)
{
	return ebt_register_target(&tc_target);
}

static void __exit fini(void)
{
	ebt_unregister_target(&tc_target);
}

module_init(init);
module_exit(fini);
MODULE_LICENSE("GPL");
