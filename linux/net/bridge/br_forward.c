/*
 *	Forwarding decision
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	$Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/linux/net/bridge/br_forward.c#2 $
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/if_vlan.h>
#include <linux/netfilter_bridge.h>
#if defined(CONFIG_IMQ) || defined(CONFIG_IMQ_MODULE)
#include <linux/imq.h>
#endif
#include "br_private.h"
#if defined(TCSUPPORT_HWNAT)
#include <linux/pktflow.h>
#endif
#ifdef TCSUPPORT_RA_HWNAT
#include <linux/foe_hook.h>
#endif

#if !defined(TCSUPPORT_CT) 
#ifdef CONFIG_PORT_BINDING
extern int (*portbind_sw_hook)(void);
extern int (*portbind_check_hook)(char *inIf, char *outIf);
#endif
#endif


/* Don't forward packets to originating port or forwarding diasabled */
static inline int should_deliver(const struct net_bridge_port *p,
				 const struct sk_buff *skb)
{
#ifdef CONFIG_IGMP_SNOOPING
	/* block WAN-to-WAN bridging traffic */
	if ((skb->dev->priv_flags & p->dev->priv_flags) & IFF_WAN_DEV)
		return 0;
#endif
	return (skb->dev != p->dev && p->state == BR_STATE_FORWARDING);
}

static inline unsigned packet_length(const struct sk_buff *skb)
{
	return skb->len - (skb->protocol == htons(ETH_P_8021Q) ? VLAN_HLEN : 0);
}

__IMEM int br_dev_queue_push_xmit(struct sk_buff *skb)
{
	/* drop mtu oversized packets except gso */
	if (packet_length(skb) > skb->dev->mtu && !skb_is_gso(skb))
		kfree_skb(skb);
	else {
		/* ip_refrag calls ip_fragment, doesn't copy the MAC header. */
		if (nf_bridge_maybe_copy_header(skb))
			kfree_skb(skb);
		else {
			skb_push(skb, ETH_HLEN);

			dev_queue_xmit(skb);
		}
	}

	return 0;
}

__IMEM int br_forward_finish(struct sk_buff *skb)
{
	return NF_HOOK(PF_BRIDGE, NF_BR_POST_ROUTING, skb, NULL, skb->dev,
		       br_dev_queue_push_xmit);

}

__IMEM static void __br_deliver(const struct net_bridge_port *to, struct sk_buff *skb)
{
	skb->dev = to->dev;
	NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_OUT, skb, NULL, skb->dev,
			br_forward_finish);
}

__IMEM static void __br_forward(const struct net_bridge_port *to, struct sk_buff *skb)
{
	struct net_device *indev;

	indev = skb->dev;
	skb->dev = to->dev;
	skb_forward_csum(skb);

#ifdef CONFIG_QOS
#if defined(CONFIG_IMQ) || defined(CONFIG_IMQ_MODULE)
	skb->mark |= QOS_DEFAULT_MARK;
#endif
#endif
	NF_HOOK(PF_BRIDGE, NF_BR_FORWARD, skb, indev, skb->dev,
			br_forward_finish);
}

/* called with rcu_read_lock */
__IMEM void br_deliver(const struct net_bridge_port *to, struct sk_buff *skb)
{
	if (should_deliver(to, skb)) {
		__br_deliver(to, skb);
		return;
	}

	kfree_skb(skb);
}

/* called with rcu_read_lock */
__IMEM void br_forward(const struct net_bridge_port *to, struct sk_buff *skb)
{
	if (should_deliver(to, skb)) {
		__br_forward(to, skb);
		return;
	}

	kfree_skb(skb);
}
#if !defined(TCSUPPORT_CT) 
#ifdef CONFIG_PORT_BINDING
/* called under bridge lock */
static void br_pb_flood(struct net_bridge *br, struct net_bridge_port *pIN, struct sk_buff *skb, int clone,
	void (*__packet_hook)(const struct net_bridge_port *p,
			      struct sk_buff *skb))
{
	struct net_bridge_port *p;
	struct net_bridge_port *prev;

	struct net_device *indev = NULL;
	struct net_device *outdev = NULL;

#if defined(TCSUPPORT_HWNAT)
	pktflow_free(skb);
#endif
#ifdef TCSUPPORT_RA_HWNAT
	if (ra_sw_nat_hook_free)
		ra_sw_nat_hook_free(skb);
#endif

	if (clone) {
		struct sk_buff *skb2;

		if ((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL) {
			br->statistics.tx_dropped++;
			return;
		}

		skb = skb2;
	}

	prev = NULL;

	list_for_each_entry_rcu(p, &br->port_list, list) {
		if (should_deliver(p, skb)) {
			if (prev != NULL) {
				struct sk_buff *skb2;
			
				indev = outdev = NULL;
				if (pIN) {
					indev = pIN->dev;
				}
				if (prev) {
					outdev = prev->dev;
				}
				//printk("%s:indev->name is %s, outdev->name is %s.\n", __FUNCTION__, indev->name, outdev->name);
				/* in dev and out dev is not in the same group */
				if ( (indev == NULL) || 
					(outdev == NULL) ||
					(portbind_check_hook == NULL) ||
					((portbind_check_hook) && portbind_check_hook(indev->name, outdev->name)) ) {
					/* it has been checked, not check again */
					skb->portbind_mark |= MASK_OUT_DEV;
					
					if ((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL) {
						br->statistics.tx_dropped++;
						kfree_skb(skb);
						return;
					}
					
					__packet_hook(prev, skb2);
				}
			}
			prev = p;
		}
	}
	
	indev = outdev = NULL;
	if (pIN) {
		indev = pIN->dev;
	}
	if (prev) {
		outdev = prev->dev;
	}
	
	if ( (prev != NULL) && 
		((indev == NULL) ||
		(outdev == NULL) ||
		(portbind_check_hook == NULL) || 
		((portbind_check_hook) && (portbind_check_hook(indev->name, outdev->name)))) ) {
		__packet_hook(prev, skb);
		return;
	}

	kfree_skb(skb);
}
#endif
#endif

/* called under bridge lock */
static void br_flood(struct net_bridge *br, struct sk_buff *skb, int clone,
	void (*__packet_hook)(const struct net_bridge_port *p,
			      struct sk_buff *skb))
{
	struct net_bridge_port *p;
	struct net_bridge_port *prev;

#if defined(TCSUPPORT_HWNAT)
	pktflow_free(skb);
#endif
#ifdef TCSUPPORT_RA_HWNAT
	if (ra_sw_nat_hook_free)
		ra_sw_nat_hook_free(skb);
#endif

	if (clone) {
		struct sk_buff *skb2;

		if ((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL) {
			br->statistics.tx_dropped++;
			return;
		}

		skb = skb2;
	}

	prev = NULL;

	list_for_each_entry_rcu(p, &br->port_list, list) {
		if (should_deliver(p, skb)) {
			if (prev != NULL) {
				struct sk_buff *skb2;

				if ((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL) {
					br->statistics.tx_dropped++;
					kfree_skb(skb);
					return;
				}

				__packet_hook(prev, skb2);
			}

			prev = p;
		}
	}

	if (prev != NULL) {
		__packet_hook(prev, skb);
		return;
	}

	kfree_skb(skb);
}


/* called with rcu_read_lock */
void br_flood_deliver(struct net_bridge *br, struct sk_buff *skb, int clone)
{
	br_flood(br, skb, clone, __br_deliver);
}

#if !defined(TCSUPPORT_CT) 
#ifdef CONFIG_PORT_BINDING
void br_flood_pb_forward(struct net_bridge *br, struct net_bridge_port *p, struct sk_buff *skb, int clone)
{
#if defined(TCSUPPORT_BRIDGE_FASTPATH)
	/*MARK sc_mac_learned flag, the flooding packets can't be learned into fastbridge table*/
	skb->sc_mac_learned |=FB_FLOOD_PKT;
#endif
	br_pb_flood(br, p, skb, clone, __br_forward);
}
#endif
#endif

/* called under bridge lock */
void br_flood_forward(struct net_bridge *br, struct sk_buff *skb, int clone)
{
#if defined(TCSUPPORT_BRIDGE_FASTPATH)
	/*MARK sc_mac_learned flag, the flooding packets can't be learned into fastbridge table*/
	skb->sc_mac_learned |=FB_FLOOD_PKT;
#endif
	br_flood(br, skb, clone, __br_forward);
}
#ifdef CONFIG_MLD_SNOOPING
EXPORT_SYMBOL(br_flood_deliver);
EXPORT_SYMBOL(br_flood_forward);
#endif
#if defined(CONFIG_MLD_SNOOPING) || defined(CONFIG_IGMP_SNOOPING)
EXPORT_SYMBOL(br_deliver);
EXPORT_SYMBOL(br_forward);
#endif

