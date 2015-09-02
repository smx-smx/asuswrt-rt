/*
 *  ebt_mark
 *
 *	Authors:
 *	Bart De Schuymer <bdschuym@pandora.be>
 *
 *  July, 2002
 *
 */

/* The mark target can be used in any chain,
 * I believe adding a mangle table just for marking is total overkill.
 * Marking a frame doesn't really change anything in the frame anyway.
 */

#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_mark_t.h>
#include <linux/module.h>

static int ebt_target_mark(struct sk_buff **pskb, unsigned int hooknr,
   const struct net_device *in, const struct net_device *out,
   const void *data, unsigned int datalen)
{
	struct ebt_mark_t_info *info = (struct ebt_mark_t_info *)data;
	int action = info->target & -16;

	if (action == MARK_SET_VALUE)
		(*pskb)->mark = info->mark;
	else if (action == MARK_OR_VALUE)
		(*pskb)->mark |= info->mark;
	else if (action == MARK_AND_VALUE)
		(*pskb)->mark &= info->mark;
	else
		(*pskb)->mark ^= info->mark;

#if 0  /*Rodney_20090724*/
	/* if this is an 8021Q frame, see if we need to do 8021p priority or vlan id remarking*/
	if (((*pskb)->protocol == __constant_htons(ETH_P_8021Q)) && (((*pskb)->mark & EBT_VLAN_REMARKING))){
		struct vlan_hdr _frame, *fp;
		unsigned short TCI;

		fp = skb_header_pointer(*pskb, 0, sizeof(_frame), &_frame);
		if (fp == NULL)
			return NF_DROP;

		TCI = ntohs(fp->h_vlan_TCI);
    	TCI = (TCI & 0x1fff) | ((((*pskb)->mark >> 18) & 0x7) << 13);
    	fp->h_vlan_TCI = htons(TCI);
	}
#endif
	return info->target | ~EBT_VERDICT_BITS;
}

static int ebt_target_mark_check(const char *tablename, unsigned int hookmask,
   const struct ebt_entry *e, void *data, unsigned int datalen)
{
	struct ebt_mark_t_info *info = (struct ebt_mark_t_info *)data;
	int tmp;

	if (datalen != EBT_ALIGN(sizeof(struct ebt_mark_t_info)))
		return -EINVAL;
	tmp = info->target | ~EBT_VERDICT_BITS;
	if (BASE_CHAIN && tmp == EBT_RETURN)
		return -EINVAL;
	CLEAR_BASE_CHAIN_BIT;
	if (tmp < -NUM_STANDARD_TARGETS || tmp >= 0)
		return -EINVAL;
	tmp = info->target & ~EBT_VERDICT_BITS;
	if (tmp != MARK_SET_VALUE && tmp != MARK_OR_VALUE &&
	    tmp != MARK_AND_VALUE && tmp != MARK_XOR_VALUE)
		return -EINVAL;
	return 0;
}

static struct ebt_target mark_target =
{
	.name		= EBT_MARK_TARGET,
	.target		= ebt_target_mark,
	.check		= ebt_target_mark_check,
	.me		= THIS_MODULE,
};

static int __init ebt_mark_init(void)
{
	return ebt_register_target(&mark_target);
}

static void __exit ebt_mark_fini(void)
{
	ebt_unregister_target(&mark_target);
}

module_init(ebt_mark_init);
module_exit(ebt_mark_fini);
MODULE_LICENSE("GPL");
