/*
 *	Handle incoming frames
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	$Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/linux/net/bridge/br_input.c#2 $
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/netfilter_bridge.h>
#include "br_private.h"
#if !defined(TCSUPPORT_CT) 
#ifdef CONFIG_PORT_BINDING
extern int (*portbind_sw_hook)(void);
extern int (*portbind_check_hook)(char *inIf, char *outIf);
#endif
#endif

/* Bridge group multicast address 802.1d (pg 51). */
const u8 br_group_address[ETH_ALEN] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x00 };
#ifdef CONFIG_IGMP_SNOOPING
MODULE_LICENSE("GPL");
int (*br_mc_forward_hook)(struct net_bridge *br, struct sk_buff *skb, int clone);
EXPORT_SYMBOL_GPL(br_mc_forward_hook);
#endif
#ifdef CONFIG_MLD_SNOOPING
int (*br_mldsnooping_forward_hook)(struct sk_buff *skb, struct net_bridge *br, unsigned char *dest,int clone);
EXPORT_SYMBOL(br_mldsnooping_forward_hook);
#endif

#ifdef CONFIG_MIPS_TC3262
__IMEM
#endif
static void br_pass_frame_up(struct net_bridge *br, struct sk_buff *skb)
{
	struct net_device *indev;

	br->statistics.rx_packets++;
	br->statistics.rx_bytes += skb->len;

	indev = skb->dev;
	skb->dev = br->dev;

	NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_IN, skb, indev, NULL,
		netif_receive_skb);
}

#if !defined(TCSUPPORT_CT) 
/* note: already called with rcu_read_lock (preempt_disabled) */
__IMEM int br_handle_frame_finish(struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	struct net_bridge_port *p = rcu_dereference(skb->dev->br_port);
	struct net_bridge *br;
	struct net_bridge_fdb_entry *dst;
	int passedup = 0;
#if defined(CONFIG_MLD_SNOOPING) || defined(CONFIG_IGMP_SNOOPING)
	int ret = 0;
#endif
#ifdef CONFIG_IGMP_SNOOPING
	typeof(br_mc_forward_hook) br_mc_forward_igmpsnoop;
#endif
	#ifdef CONFIG_MLD_SNOOPING
	typeof(br_mldsnooping_forward_hook) br_mldsnooping_forward;
	#endif

	if (!p || p->state == BR_STATE_DISABLED)
		goto drop;

	/* insert into forwarding database after filtering to avoid spoofing */
	br = p->br;
#if defined(TCSUPPORT_HWNAT)
	br_fdb_update(br, p, eth_hdr(skb)->h_source, skb);
#else
	br_fdb_update(br, p, eth_hdr(skb)->h_source);
#endif

	if (p->state == BR_STATE_LEARNING)
		goto drop;

	if (br->dev->flags & IFF_PROMISC) {
		struct sk_buff *skb2;

		skb2 = skb_clone(skb, GFP_ATOMIC);
		if (skb2 != NULL) {
			passedup = 1;
			br_pass_frame_up(br, skb2);
		}
	}

	if (is_multicast_ether_addr(dest)) {
		br->statistics.multicast++;
#if defined(CONFIG_MLD_SNOOPING) || defined(CONFIG_IGMP_SNOOPING)	
		switch(ntohs(skb->protocol)){
		#ifdef CONFIG_IGMP_SNOOPING
			case ETH_P_IP:	/*IGMP Snooping*/
				br_mc_forward_igmpsnoop = rcu_dereference(br_mc_forward_hook);
				if(br_mc_forward_igmpsnoop)
					ret = br_mc_forward_igmpsnoop(br, skb, !passedup);	
				break;
		#endif
		#ifdef CONFIG_MLD_SNOOPING
			case ETH_P_IPV6: /*MLD Snooping*/
				br_mldsnooping_forward = rcu_dereference(br_mldsnooping_forward_hook);	
				if(br_mldsnooping_forward)
				{
					ret = br_mldsnooping_forward(skb, br, dest,!passedup);	
				}			
				break;
		#endif
			default:
				ret = 0;
				break;	
		}		
		
		if(!ret){
		#ifdef CONFIG_PORT_BINDING
		#if defined(TCSUPPORT_FTP_THROUGHPUT)
			if (portbind_sw_hook) {
		#else
			if (portbind_sw_hook && (portbind_sw_hook() == 1)) {
		#endif
				br_flood_pb_forward(br, p, skb, !passedup);
			}
			else {
				br_flood_forward(br, skb, !passedup);
			}
		#else
			br_flood_forward(br, skb, !passedup);
		#endif
		}
#else
	#ifdef CONFIG_PORT_BINDING
		#if defined(TCSUPPORT_FTP_THROUGHPUT)
			if (portbind_sw_hook) {
		#else
			if (portbind_sw_hook && (portbind_sw_hook() == 1)) {
		#endif
			br_flood_pb_forward(br, p, skb, !passedup);
		}
		else {
			br_flood_forward(br, skb, !passedup);
		}
	#else
		br_flood_forward(br, skb, !passedup);
	#endif

#endif
		if (!passedup)
			br_pass_frame_up(br, skb);
		goto out;
	}

#ifdef CONFIG_PORT_BINDING
	#if defined(TCSUPPORT_FTP_THROUGHPUT)
		if (portbind_sw_hook) {
	#else
		if (portbind_sw_hook && (portbind_sw_hook() == 1)) {
	#endif
		//printk("In port is %s\n", p->dev->name);
		dst = __br_fdb_pb_get(br, p, dest);
	}
	else {
		dst = __br_fdb_get(br, dest);
	}
#else
	dst = __br_fdb_get(br, dest);
#endif
	if (dst != NULL && dst->is_local) {
		if (!passedup)
			br_pass_frame_up(br, skb);
		else
			kfree_skb(skb);
		goto out;
	}

	if (dst != NULL) {
		br_forward(dst->dst, skb);
		goto out;
	}

#ifdef CONFIG_PORT_BINDING
	#if defined(TCSUPPORT_FTP_THROUGHPUT)
		if (portbind_sw_hook) {
	#else
		if (portbind_sw_hook && (portbind_sw_hook() == 1)) {
	#endif
		br_flood_pb_forward(br, p, skb, 0);
	}
	else {
		br_flood_forward(br, skb, 0);
	}
#else
	br_flood_forward(br, skb, 0);
#endif

out:
	return 0;
drop:
	kfree_skb(skb);
	goto out;
}

#endif
/* note: already called with rcu_read_lock (preempt_disabled) */
static int br_handle_local_finish(struct sk_buff *skb)
{
	struct net_bridge_port *p = rcu_dereference(skb->dev->br_port);

	if (p && p->state != BR_STATE_DISABLED)
#if defined(TCSUPPORT_HWNAT)
		br_fdb_update(p->br, p, eth_hdr(skb)->h_source, skb);
#else
		br_fdb_update(p->br, p, eth_hdr(skb)->h_source);
#endif

	return 0;	 /* process further */
}

/* Does address match the link local multicast address.
 * 01:80:c2:00:00:0X
 */
static inline int is_link_local(const unsigned char *dest)
{
	const u16 *a = (const u16 *) dest;
	static const u16 *const b = (const u16 *const ) br_group_address;
	static const u16 m = __constant_cpu_to_be16(0xfff0);

	return ((a[0] ^ b[0]) | (a[1] ^ b[1]) | ((a[2] ^ b[2]) & m)) == 0;
}

/*
 * Called via br_handle_frame_hook.
 * Return NULL if skb is handled
 * note: already called with rcu_read_lock (preempt_disabled)
 */
__IMEM struct sk_buff *br_handle_frame(struct net_bridge_port *p, struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	int (*rhook)(struct sk_buff **pskb);

	if (!is_valid_ether_addr(eth_hdr(skb)->h_source))
		goto drop;

	if (unlikely(is_link_local(dest))) {
		/* Pause frames shouldn't be passed up by driver anyway */
		if (skb->protocol == htons(ETH_P_PAUSE))
			goto drop;

		/* Process STP BPDU's through normal netif_receive_skb() path */
		if (p->br->stp_enabled != BR_NO_STP) {
			if (NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_IN, skb, skb->dev,
				    NULL, br_handle_local_finish))
				return NULL;
			else
				return skb;
		}
	}

	switch (p->state) {
	case BR_STATE_FORWARDING:
		
#if 1 //defined(CONFIG_PORT_BINDING) || defined(TCSUPPORT_REDIRECT_WITH_PORTMASK)
		//printk("%s:mark lan interface, dev name is %s\n", __FUNCTION__, skb->dev->name);
#if 1 //defined(TCSUPPORT_E8B) || defined(TCSUPPORT_REDIRECT_WITH_PORTMASK)
		/*_____________________________________________
		** remark packet from different lan interfac,  
		** use the highest 4 bits.
		**
		** eth0	  	0x10000000
		** eth0.1 	0x10000000
		** eth0.2 	0x20000000
		** eth0.3 	0x30000000
		** eth0.4 	0x40000000
		** ra0	  	0x50000000
		** ra1    	0x60000000
		** ra2    	0x70000000
		** ra3    	0x80000000
		** usb0   	0x90000000
		**_________________________________________
		*/
#if 0
#ifdef TCSUPPORT_TC2031
		/* for signal port */
		if (!strcmp(skb->dev->name, "eth0")) {
			skb->mark |= 0x10000000;
		}
#else
		/* for four ports */
		if (!strcmp(skb->dev->name, "eth0.1")) {
			skb->mark |= 0x10000000;
		}
		else if (!strcmp(skb->dev->name, "eth0.2")) {
			skb->mark |= 0x20000000;
		}
		else if (!strcmp(skb->dev->name, "eth0.3")) {
			skb->mark |= 0x30000000;
		}
		else if (!strcmp(skb->dev->name, "eth0.4")) {
			skb->mark |= 0x40000000;
		}
#endif
		else if (!strcmp(skb->dev->name, "ra0")) {
			skb->mark |= 0x50000000;
		}
		else if (!strcmp(skb->dev->name, "ra1")) {
			skb->mark |= 0x60000000;
		}
		else if (!strcmp(skb->dev->name, "ra2")) {
			skb->mark |= 0x70000000;
		}
		else if (!strcmp(skb->dev->name, "ra3")) {
			skb->mark |= 0x80000000;
		}
		else if (!strcmp(skb->dev->name, "usb0")) {
			skb->mark |= 0x90000000;
		}
#endif	
#define		WLAN_DEV_OFFSET 		5
#define		USB_DEV_OFFSET		9
#define		DEV_OFFSET			28
		switch (skb->dev->name[0]) {
			case 'e':
		#ifdef TCSUPPORT_TC2031
				/* device name format must be eth0 */
				skb->mark |= 0x10000000;
		#else
				/* device name format must be eth0.x */
				if (skb->dev->name[4] == '.')
					skb->mark |= (skb->dev->name[5] - '0') << DEV_OFFSET;
		#endif
				break;
			case 'r':
				/* device name must be rax */
				skb->mark |= ((skb->dev->name[2] - '0') + WLAN_DEV_OFFSET) << DEV_OFFSET;
				break;
			case 'u':
				/* device name must be usbx */
				skb->mark |= ((skb->dev->name[3] - '0') + USB_DEV_OFFSET) << DEV_OFFSET;
				break;
			default:
				break;
		}
#endif
		
#endif	
		rhook = rcu_dereference(br_should_route_hook);
		if (rhook != NULL) {
			if (rhook(&skb))
				return skb;
			dest = eth_hdr(skb)->h_dest;
		}
		/* fall through */
	case BR_STATE_LEARNING:
		if (!compare_ether_addr(p->br->dev->dev_addr, dest))
			skb->pkt_type = PACKET_HOST;

		NF_HOOK(PF_BRIDGE, NF_BR_PRE_ROUTING, skb, skb->dev, NULL,
			br_handle_frame_finish);
		break;
	default:
drop:
		kfree_skb(skb);
	}
	return NULL;
}
