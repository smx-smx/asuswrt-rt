#if 1  /*Rodney_20090724*/

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/checksum.h>
#include <linux/if_vlan.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_ftos_t.h>
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
static int ebt_target_ftos(struct sk_buff **pskb, unsigned int hooknr,
   const struct net_device *in, const struct net_device *out,
   const void *data, unsigned int datalen)
{
	struct ebt_ftos_info *ftosinfo = (struct ebt_ftos_info *)data;
	struct iphdr _iph, *ih;
	__u8 oldtos;
#ifdef CONFIG_QOS
	__u8 tos;
	int rule_no;
#endif

#if 1  /*Rodney_20090724*/
	if((*pskb)->protocol == htons(ETH_P_IP))
		ih = (struct iphdr *)skb_header_pointer(*pskb, 0, sizeof(_iph), &_iph);
	else if(((*pskb)->protocol == htons(ETH_P_8021Q)) && (vlan_proto(*pskb) == htons(ETH_P_IP)))
		ih = (struct iphdr *)(skb_mac_header(*pskb) + VLAN_ETH_HLEN);
    else if(((*pskb)->protocol == htons(ETH_P_PPP_SES)) && (pppoe_proto(*pskb) == htons(0x0021)))
		ih = (struct iphdr *)(skb_mac_header(*pskb) + ETH_HLEN +PPPOE_SES_HLEN);
	else
		ih = (struct iphdr *)skb_header_pointer(*pskb, 0, sizeof(_iph), &_iph);
#else
	ih = (struct iphdr *)skb_header_pointer(*pskb, 0, sizeof(_iph), &_iph);
#endif

	
	if (!skb_make_writable(pskb, sizeof(struct iphdr)))
		return NF_DROP;

    oldtos = ih->tos;
#if 0
	if ( (*pskb)->mark & QOS_WANIF_MARK ) {
		tos = (ih->tos & ~ftosinfo->mask) | (ftosinfo->ftos & ftosinfo->mask);
		(*pskb)->mark |= (tos << 18);
		return ftosinfo->target | ~EBT_VERDICT_BITS;
	}
#endif
#ifdef CONFIG_QOS
	rule_no = ((*pskb)->mark & QOS_RULE_INDEX_MARK) >> 12;
	if (0 == qostype_chk(EBT_CHK_TYPE, rule_no, NULL, 0)) {
		tos = (ih->tos & ~ftosinfo->mask) | (ftosinfo->ftos & ftosinfo->mask);
		set_tos(rule_no, tos);
		return ftosinfo->target | ~EBT_VERDICT_BITS;
	}
	else {
		unset_tos(rule_no);
	}
#endif

    ih->tos = (ih->tos & ~ftosinfo->mask) | (ftosinfo->ftos & ftosinfo->mask);
    nf_csum_replace2(&ih->check, htons(oldtos), htons(ih->tos));

    return ftosinfo->target | ~EBT_VERDICT_BITS;
}

static int ebt_target_ftos_check(const char *tablename, unsigned int hookmask,
   const struct ebt_entry *e, void *data, unsigned int datalen)
{
	struct ebt_ftos_info *info = (struct ebt_ftos_info *)data;

	if (datalen != sizeof(struct ebt_ftos_info))
		return -EINVAL;
	if (BASE_CHAIN && info->target == EBT_RETURN)
		return -EINVAL;
	CLEAR_BASE_CHAIN_BIT;
	if (INVALID_TARGET)
		return -EINVAL;
	return 0;
}

static struct ebt_target ftos_target =
{
        .name           = EBT_FTOS_TARGET,
        .target         = ebt_target_ftos,
        .check          = ebt_target_ftos_check,
        .me             = THIS_MODULE,
};

static int __init init(void)
{
	return ebt_register_target(&ftos_target);
}

static void __exit fini(void)
{
	ebt_unregister_target(&ftos_target);
}

module_init(init);
module_exit(fini);
MODULE_LICENSE("GPL");
#endif
