/*
 *	Routines having to do with the 'struct sk_buff' memory handlers.
 *
 *	Authors:	Alan Cox <iiitac@pyr.swan.ac.uk>
 *			Florian La Roche <rzsfl@rz.uni-sb.de>
 *
 *	Version:	$Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/linux/net/core/skbuff.c#5 $
 *
 *	Fixes:
 *		Alan Cox	:	Fixed the worst of the load
 *					balancer bugs.
 *		Dave Platt	:	Interrupt stacking fix.
 *	Richard Kooijman	:	Timestamp fixes.
 *		Alan Cox	:	Changed buffer format.
 *		Alan Cox	:	destructor hook for AF_UNIX etc.
 *		Linus Torvalds	:	Better skb_clone.
 *		Alan Cox	:	Added skb_copy.
 *		Alan Cox	:	Added all the changed routines Linus
 *					only put in the headers
 *		Ray VanTassle	:	Fixed --skb->lock in free
 *		Alan Cox	:	skb_copy copy arp field
 *		Andi Kleen	:	slabified it.
 *		Robert Olsson	:	Removed skb_head_pool
 *
 *	NOTE:
 *		The __skb_ routines should be called with interrupts
 *	disabled, or you better be *real* sure that the operation is atomic
 *	with respect to whatever list is being frobbed (e.g. via lock_sock()
 *	or via disabling bottom half handlers, etc).
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

/*
 *	The functions in this file will not compile correctly with gcc 2.4.x
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/slab.h>
#include <linux/netdevice.h>
#ifdef CONFIG_NET_CLS_ACT
#include <net/pkt_sched.h>
#endif
#include <linux/string.h>
#include <linux/skbuff.h>
#include <linux/cache.h>
#include <linux/rtnetlink.h>
#include <linux/init.h>
#include <linux/scatterlist.h>

#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
#include <linux/proc_fs.h>
#endif

#include <net/protocol.h>
#include <net/dst.h>
#include <net/sock.h>
#include <net/checksum.h>
#include <net/xfrm.h>

#include <asm/uaccess.h>
#include <asm/system.h>

#include "kmap_skb.h"
#if defined(TCSUPPORT_HWNAT)		
#include <linux/pktflow.h>	
#endif			
#ifdef TCSUPPORT_RA_HWNAT
#include <linux/foe_hook.h>
#endif

#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
//only one skbmgr pool for every CPU. shnwind 20101215.
#define SKBMGR_SINGLE_QUEUE 

atomic_t skbmgr_alloc_no;
atomic_t skbmgr_4k_alloc_no;
#ifdef SKBMGR_SINGLE_QUEUE
static DEFINE_SPINLOCK(skbmgr_lock);
static DEFINE_SPINLOCK(skbmgr_4k_lock);
#endif
#define SKBMGR_INDICATION         			1
#define SKBMGR_4K_INDICATION               	2
#endif

__DMEM static struct kmem_cache *skbuff_head_cache __read_mostly;
__DMEM static struct kmem_cache *skbuff_fclone_cache __read_mostly;

#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
atomic_t g_used_skb_num;
int g_max_skb_num = 1280;
EXPORT_SYMBOL(g_used_skb_num);

int peak_skb_num = 0;
#endif

/*
 *	Keep out-of-line to prevent kernel bloat.
 *	__builtin_return_address is not used because it is not always
 *	reliable.
 */

/**
 *	skb_over_panic	- 	private function
 *	@skb: buffer
 *	@sz: size
 *	@here: address
 *
 *	Out of line support code for skb_put(). Not user callable.
 */
void skb_over_panic(struct sk_buff *skb, int sz, void *here)
{
	printk(KERN_EMERG "skb_over_panic: text:%p len:%d put:%d head:%p "
			  "data:%p tail:%#lx end:%#lx dev:%s\n",
	       here, skb->len, sz, skb->head, skb->data,
	       (unsigned long)skb->tail, (unsigned long)skb->end,
	       skb->dev ? skb->dev->name : "<NULL>");
	BUG();
}

/**
 *	skb_under_panic	- 	private function
 *	@skb: buffer
 *	@sz: size
 *	@here: address
 *
 *	Out of line support code for skb_push(). Not user callable.
 */

void skb_under_panic(struct sk_buff *skb, int sz, void *here)
{
	printk(KERN_EMERG "skb_under_panic: text:%p len:%d put:%d head:%p "
			  "data:%p tail:%#lx end:%#lx dev:%s\n",
	       here, skb->len, sz, skb->head, skb->data,
	       (unsigned long)skb->tail, (unsigned long)skb->end,
	       skb->dev ? skb->dev->name : "<NULL>");
	BUG();
}

void skb_truesize_bug(struct sk_buff *skb)
{
	printk(KERN_ERR "SKB BUG: Invalid truesize (%u) "
	       "len=%u, sizeof(sk_buff)=%Zd\n",
	       skb->truesize, skb->len, sizeof(struct sk_buff));
}
EXPORT_SYMBOL(skb_truesize_bug);

/* 	Allocate a new skbuff. We do this ourselves so we can fill in a few
 *	'private' fields and also do memory statistics to find all the
 *	[BEEP] leaks.
 *
 */

/**
 *	__alloc_skb	-	allocate a network buffer
 *	@size: size to allocate
 *	@gfp_mask: allocation mask
 *	@fclone: allocate from fclone cache instead of head cache
 *		and allocate a cloned (child) skb
 *	@node: numa node to allocate memory on
 *
 *	Allocate a new &sk_buff. The returned buffer has no headroom and a
 *	tail room of size bytes. The object has a reference count of one.
 *	The return is the buffer. On a failure the return is %NULL.
 *
 *	Buffers may only be allocated from interrupts using a @gfp_mask of
 *	%GFP_ATOMIC.
 */
#ifdef CONFIG_MIPS_TC3262
__IMEM
#endif
struct sk_buff *__alloc_skb(unsigned int size, gfp_t gfp_mask,
			    int fclone, int node)
{
	struct kmem_cache *cache;
	struct skb_shared_info *shinfo;
	struct sk_buff *skb;
	u8 *data;

	cache = fclone ? skbuff_fclone_cache : skbuff_head_cache;

	/* Get the HEAD */
	skb = kmem_cache_alloc_node(cache, gfp_mask & ~__GFP_DMA, node);
	if (!skb)
		goto out;
#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)	
	/*if number of skb reach the max number,go to nodata*/
	if(atomic_read(&g_used_skb_num) > g_max_skb_num){
			goto nodata;
	}
#endif

	size = SKB_DATA_ALIGN(size);
	data = kmalloc_node_track_caller(size + sizeof(struct skb_shared_info),
			gfp_mask, node);
	if (!data)
		goto nodata;

#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
	/*alloc successfully*/
	atomic_inc(&g_used_skb_num);
	if(peak_skb_num < atomic_read(&g_used_skb_num))
	{
		peak_skb_num = atomic_read(&g_used_skb_num);
	}
#endif
	/*
	 * See comment in sk_buff definition, just before the 'tail' member
	 */
	memset(skb, 0, offsetof(struct sk_buff, tail));
	skb->truesize = size + sizeof(struct sk_buff);
	atomic_set(&skb->users, 1);
	skb->head = data;
	skb->data = data;
	skb_reset_tail_pointer(skb);
	skb->end = skb->tail + size;
	/* make sure we initialize shinfo sequentially */
	shinfo = skb_shinfo(skb);
	atomic_set(&shinfo->dataref, 1);
	shinfo->nr_frags  = 0;
	shinfo->gso_size = 0;
	shinfo->gso_segs = 0;
	shinfo->gso_type = 0;
	shinfo->ip6_frag_id = 0;
	shinfo->frag_list = NULL;

	if (fclone) {
		struct sk_buff *child = skb + 1;
		atomic_t *fclone_ref = (atomic_t *) (child + 1);

		skb->fclone = SKB_FCLONE_ORIG;
		atomic_set(fclone_ref, 1);

		child->fclone = SKB_FCLONE_UNAVAILABLE;
	}
out:
	return skb;
nodata:
	kmem_cache_free(cache, skb);
	skb = NULL;
	goto out;
}

/**
 *	__netdev_alloc_skb - allocate an skbuff for rx on a specific device
 *	@dev: network device to receive on
 *	@length: length to allocate
 *	@gfp_mask: get_free_pages mask, passed to alloc_skb
 *
 *	Allocate a new &sk_buff and assign it a usage count of one. The
 *	buffer has unspecified headroom built in. Users should allocate
 *	the headroom they think they need without accounting for the
 *	built in space. The built in space is used for optimisations.
 *
 *	%NULL is returned if there is no free memory.
 */
struct sk_buff *__netdev_alloc_skb(struct net_device *dev,
		unsigned int length, gfp_t gfp_mask)
{
	int node = dev->dev.parent ? dev_to_node(dev->dev.parent) : -1;
	struct sk_buff *skb;

	skb = __alloc_skb(length + NET_SKB_PAD, gfp_mask, 0, node);
	if (likely(skb)) {
		skb_reserve(skb, NET_SKB_PAD);
		skb->dev = dev;
	}
	return skb;
}

static void skb_drop_list(struct sk_buff **listp)
{
	struct sk_buff *list = *listp;

	*listp = NULL;

	do {
		struct sk_buff *this = list;
		list = list->next;
		kfree_skb(this);
	} while (list);
}

static inline void skb_drop_fraglist(struct sk_buff *skb)
{
	skb_drop_list(&skb_shinfo(skb)->frag_list);
}

static void skb_clone_fraglist(struct sk_buff *skb)
{
	struct sk_buff *list;

	for (list = skb_shinfo(skb)->frag_list; list; list = list->next)
		skb_get(list);
}

#ifdef CONFIG_MIPS_TC3262
__IMEM
#endif
static void skb_release_data(struct sk_buff *skb)
{
	if (!skb->cloned ||
	    !atomic_sub_return(skb->nohdr ? (1 << SKB_DATAREF_SHIFT) + 1 : 1,
			       &skb_shinfo(skb)->dataref)) {
		if (skb_shinfo(skb)->nr_frags) {
			int i;
			for (i = 0; i < skb_shinfo(skb)->nr_frags; i++)
				put_page(skb_shinfo(skb)->frags[i].page);
		}

		if (skb_shinfo(skb)->frag_list)
			skb_drop_fraglist(skb);

		kfree(skb->head);
#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
		/*sub used skb number*/
		atomic_dec(&g_used_skb_num);
#endif
	}
}

/*
 *	Free an skbuff by memory without cleaning the state.
 */
#ifdef CONFIG_MIPS_TC3262
__IMEM
#endif
void kfree_skbmem(struct sk_buff *skb)
{
	struct sk_buff *other;
	atomic_t *fclone_ref;

#if defined(TCSUPPORT_HWNAT)		
	pktflow_free(skb);		
#endif
#ifdef TCSUPPORT_RA_HWNAT
	if (ra_sw_nat_hook_free)
		ra_sw_nat_hook_free(skb);
#endif

	skb_release_data(skb);
	switch (skb->fclone) {
	case SKB_FCLONE_UNAVAILABLE:
		kmem_cache_free(skbuff_head_cache, skb);
		break;

	case SKB_FCLONE_ORIG:
		fclone_ref = (atomic_t *) (skb + 2);
		if (atomic_dec_and_test(fclone_ref))
			kmem_cache_free(skbuff_fclone_cache, skb);
		break;

	case SKB_FCLONE_CLONE:
		fclone_ref = (atomic_t *) (skb + 1);
		other = skb - 1;

		/* The clone portion is available for
		 * fast-cloning again.
		 */
		skb->fclone = SKB_FCLONE_UNAVAILABLE;

		if (atomic_dec_and_test(fclone_ref))
			kmem_cache_free(skbuff_fclone_cache, other);
		break;
	}
}

/**
 *	__kfree_skb - private function
 *	@skb: buffer
 *
 *	Free an sk_buff. Release anything attached to the buffer.
 *	Clean the state. This is an internal helper function. Users should
 *	always call kfree_skb
 */

#ifdef CONFIG_MIPS_TC3262
__IMEM
#endif
void __kfree_skb(struct sk_buff *skb)
{
	dst_release(skb->dst);
#ifdef CONFIG_XFRM
	secpath_put(skb->sp);
#endif
	if (skb->destructor) {
		WARN_ON(in_irq());
		skb->destructor(skb);
	}
#if defined(CONFIG_NF_CONNTRACK) || defined(CONFIG_NF_CONNTRACK_MODULE)
	nf_conntrack_put(skb->nfct);
	nf_conntrack_put_reasm(skb->nfct_reasm);
#endif
#ifdef CONFIG_BRIDGE_NETFILTER
	nf_bridge_put(skb->nf_bridge);
#endif
/* XXX: IS this still necessary? - JHS */
#ifdef CONFIG_NET_SCHED
	skb->tc_index = 0;
#ifdef CONFIG_NET_CLS_ACT
	skb->tc_verd = 0;
#endif
#endif
#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
	if (skb->skb_recycling_ind == SKBMGR_INDICATION) {
		atomic_dec(&skbmgr_alloc_no);
		skb->skb_recycling_ind = 0;
	}else if(skb->skb_recycling_ind == SKBMGR_4K_INDICATION){
		atomic_dec(&skbmgr_4k_alloc_no);
		skb->skb_recycling_ind = 0;
	}
	if (skb->skb_recycling_callback) {
		if ((*skb->skb_recycling_callback)(skb))
			return;
	} 
	skb->skb_recycling_callback = NULL;
#endif

	kfree_skbmem(skb);
}

/**
 *	kfree_skb - free an sk_buff
 *	@skb: buffer to free
 *
 *	Drop a reference to the buffer and free it if the usage count has
 *	hit zero.
 */
__IMEM void kfree_skb(struct sk_buff *skb)
{
	if (unlikely(!skb))
		return;
	if (likely(atomic_read(&skb->users) == 1))
		smp_rmb();
	else if (likely(!atomic_dec_and_test(&skb->users)))
		return;
	__kfree_skb(skb);
}

/**
 *	skb_clone	-	duplicate an sk_buff
 *	@skb: buffer to clone
 *	@gfp_mask: allocation priority
 *
 *	Duplicate an &sk_buff. The new one is not owned by a socket. Both
 *	copies share the same packet data but not structure. The new
 *	buffer has a reference count of 1. If the allocation fails the
 *	function returns %NULL otherwise the new buffer is returned.
 *
 *	If this function is called from an interrupt gfp_mask() must be
 *	%GFP_ATOMIC.
 */

struct sk_buff *skb_clone(struct sk_buff *skb, gfp_t gfp_mask)
{
	struct sk_buff *n;

#if defined(TCSUPPORT_HWNAT)		
	int skip_pktflow = gfp_mask & GFP_SKIP_PKTFLOW;
	gfp_mask &= ~GFP_SKIP_PKTFLOW;
#endif

	n = skb + 1;
	if (skb->fclone == SKB_FCLONE_ORIG &&
	    n->fclone == SKB_FCLONE_UNAVAILABLE) {
		atomic_t *fclone_ref = (atomic_t *) (n + 1);
		n->fclone = SKB_FCLONE_CLONE;
		atomic_inc(fclone_ref);
	} else {
		n = kmem_cache_alloc(skbuff_head_cache, gfp_mask);
		if (!n)
			return NULL;
		n->fclone = SKB_FCLONE_UNAVAILABLE;
	}

#define C(x) n->x = skb->x

	n->next = n->prev = NULL;
	n->sk = NULL;
	C(tstamp);
	C(dev);
	C(transport_header);
	C(network_header);
	C(mac_header);
	C(dst);
	dst_clone(skb->dst);
	C(sp);
#ifdef CONFIG_INET
	secpath_get(skb->sp);
#endif
	memcpy(n->cb, skb->cb, sizeof(skb->cb));
	C(len);
	C(data_len);
	C(mac_len);
	C(csum);
	C(local_df);
	n->cloned = 1;
	n->hdr_len = skb->nohdr ? skb_headroom(skb) : skb->hdr_len;
	n->nohdr = 0;
	C(pkt_type);
	C(ip_summed);
	C(priority);
#if defined(CONFIG_IP_VS) || defined(CONFIG_IP_VS_MODULE)
	C(ipvs_property);
#endif
	C(protocol);
	n->destructor = NULL;
#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
	n->skb_recycling_callback = NULL;
	n->skb_recycling_ind = 0;
	skb->skb_recycling_callback = NULL;
#endif
	C(mark);
#if defined(TCSUPPORT_HWNAT)		
	if (skip_pktflow)
		n->pktflow_p = NULL;
	else
	pktflow_xfer(n, skb);	
#endif
#ifdef TCSUPPORT_RA_HWNAT
	if (ra_sw_nat_hook_xfer)
		ra_sw_nat_hook_xfer(n, skb);
#endif
	__nf_copy(n, skb);
#ifdef CONFIG_NET_SCHED
	C(tc_index);
#ifdef CONFIG_NET_CLS_ACT
	n->tc_verd = SET_TC_VERD(skb->tc_verd,0);
	n->tc_verd = CLR_TC_OK2MUNGE(n->tc_verd);
	n->tc_verd = CLR_TC_MUNGED(n->tc_verd);
	C(iif);
#endif
#endif
#if defined(TCSUPPORT_BRIDGE_FASTPATH)
	C(sc_mac_learned);
#endif
	
	skb_copy_secmark(n, skb);
	C(truesize);
	atomic_set(&n->users, 1);
	C(head);
	C(data);
	C(tail);
	C(end);

	atomic_inc(&(skb_shinfo(skb)->dataref));
	skb->cloned = 1;

	return n;
}

static void copy_skb_header(struct sk_buff *new, const struct sk_buff *old)
{
#ifndef NET_SKBUFF_DATA_USES_OFFSET
	/*
	 *	Shift between the two data areas in bytes
	 */
	unsigned long offset = new->data - old->data;
#endif
	new->sk		= NULL;
	new->dev	= old->dev;
	new->priority	= old->priority;
	new->protocol	= old->protocol;
	new->dst	= dst_clone(old->dst);
#ifdef CONFIG_INET
	new->sp		= secpath_get(old->sp);
#endif
	new->transport_header = old->transport_header;
	new->network_header   = old->network_header;
	new->mac_header	      = old->mac_header;
#ifndef NET_SKBUFF_DATA_USES_OFFSET
	/* {transport,network,mac}_header are relative to skb->head */
	new->transport_header += offset;
	new->network_header   += offset;
	new->mac_header	      += offset;
#endif
	memcpy(new->cb, old->cb, sizeof(old->cb));
	new->local_df	= old->local_df;
	new->fclone	= SKB_FCLONE_UNAVAILABLE;
	new->pkt_type	= old->pkt_type;
	new->tstamp	= old->tstamp;
	new->destructor = NULL;
#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
	new->skb_recycling_callback = NULL;
	new->skb_recycling_ind = 0;
#endif
	new->mark	= old->mark;
#if defined(TCSUPPORT_HWNAT)		
	pktflow_xfer(new, old);	
#endif
#ifdef TCSUPPORT_RA_HWNAT
	if (ra_sw_nat_hook_xfer)
		ra_sw_nat_hook_xfer(new, old);
#endif
	__nf_copy(new, old);
#if defined(CONFIG_IP_VS) || defined(CONFIG_IP_VS_MODULE)
	new->ipvs_property = old->ipvs_property;
#endif
#ifdef CONFIG_NET_SCHED
#ifdef CONFIG_NET_CLS_ACT
	new->tc_verd = old->tc_verd;
#endif
	new->tc_index	= old->tc_index;
#endif

#if defined(TCSUPPORT_BRIDGE_FASTPATH)
	new->sc_mac_learned = old->sc_mac_learned;
#endif
	
	skb_copy_secmark(new, old);
	atomic_set(&new->users, 1);
	skb_shinfo(new)->gso_size = skb_shinfo(old)->gso_size;
	skb_shinfo(new)->gso_segs = skb_shinfo(old)->gso_segs;
	skb_shinfo(new)->gso_type = skb_shinfo(old)->gso_type;
}

/**
 *	skb_copy	-	create private copy of an sk_buff
 *	@skb: buffer to copy
 *	@gfp_mask: allocation priority
 *
 *	Make a copy of both an &sk_buff and its data. This is used when the
 *	caller wishes to modify the data and needs a private copy of the
 *	data to alter. Returns %NULL on failure or the pointer to the buffer
 *	on success. The returned buffer has a reference count of 1.
 *
 *	As by-product this function converts non-linear &sk_buff to linear
 *	one, so that &sk_buff becomes completely private and caller is allowed
 *	to modify all the data of returned buffer. This means that this
 *	function is not recommended for use in circumstances when only
 *	header is going to be modified. Use pskb_copy() instead.
 */

struct sk_buff *skb_copy(const struct sk_buff *skb, gfp_t gfp_mask)
{
	int headerlen = skb->data - skb->head;
	/*
	 *	Allocate the copy buffer
	 */
	struct sk_buff *n;
#ifdef NET_SKBUFF_DATA_USES_OFFSET
	n = alloc_skb(skb->end + skb->data_len, gfp_mask);
#else
	n = alloc_skb(skb->end - skb->head + skb->data_len, gfp_mask);
#endif
	if (!n)
		return NULL;

	/* Set the data pointer */
	skb_reserve(n, headerlen);
	/* Set the tail pointer and length */
	skb_put(n, skb->len);
	n->csum	     = skb->csum;
	n->ip_summed = skb->ip_summed;

	if (skb_copy_bits(skb, -headerlen, n->head, headerlen + skb->len))
		BUG();

	copy_skb_header(n, skb);
	return n;
}


/**
 *	pskb_copy	-	create copy of an sk_buff with private head.
 *	@skb: buffer to copy
 *	@gfp_mask: allocation priority
 *
 *	Make a copy of both an &sk_buff and part of its data, located
 *	in header. Fragmented data remain shared. This is used when
 *	the caller wishes to modify only header of &sk_buff and needs
 *	private copy of the header to alter. Returns %NULL on failure
 *	or the pointer to the buffer on success.
 *	The returned buffer has a reference count of 1.
 */

struct sk_buff *pskb_copy(struct sk_buff *skb, gfp_t gfp_mask)
{
	/*
	 *	Allocate the copy buffer
	 */
	struct sk_buff *n;
#ifdef NET_SKBUFF_DATA_USES_OFFSET
	n = alloc_skb(skb->end, gfp_mask);
#else
	n = alloc_skb(skb->end - skb->head, gfp_mask);
#endif
	if (!n)
		goto out;

	/* Set the data pointer */
	skb_reserve(n, skb->data - skb->head);
	/* Set the tail pointer and length */
	skb_put(n, skb_headlen(skb));
	/* Copy the bytes */
	skb_copy_from_linear_data(skb, n->data, n->len);
	n->csum	     = skb->csum;
	n->ip_summed = skb->ip_summed;

	n->truesize += skb->data_len;
	n->data_len  = skb->data_len;
	n->len	     = skb->len;

	if (skb_shinfo(skb)->nr_frags) {
		int i;

		for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
			skb_shinfo(n)->frags[i] = skb_shinfo(skb)->frags[i];
			get_page(skb_shinfo(n)->frags[i].page);
		}
		skb_shinfo(n)->nr_frags = i;
	}

	if (skb_shinfo(skb)->frag_list) {
		skb_shinfo(n)->frag_list = skb_shinfo(skb)->frag_list;
		skb_clone_fraglist(n);
	}

	copy_skb_header(n, skb);
out:
	return n;
}

/**
 *	pskb_expand_head - reallocate header of &sk_buff
 *	@skb: buffer to reallocate
 *	@nhead: room to add at head
 *	@ntail: room to add at tail
 *	@gfp_mask: allocation priority
 *
 *	Expands (or creates identical copy, if &nhead and &ntail are zero)
 *	header of skb. &sk_buff itself is not changed. &sk_buff MUST have
 *	reference count of 1. Returns zero in the case of success or error,
 *	if expansion failed. In the last case, &sk_buff is not changed.
 *
 *	All the pointers pointing into skb header may change and must be
 *	reloaded after call to this function.
 */

int pskb_expand_head(struct sk_buff *skb, int nhead, int ntail,
		     gfp_t gfp_mask)
{
	int i;
	u8 *data;
#ifdef NET_SKBUFF_DATA_USES_OFFSET
	int size = nhead + skb->end + ntail;
#else
	int size = nhead + (skb->end - skb->head) + ntail;
#endif
	long off;

	if (skb_shared(skb))
		BUG();
#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
	/*the skb num reach the max number,go to nodata*/
	if(atomic_read(&g_used_skb_num) > g_max_skb_num)
		goto nodata;
#endif

	size = SKB_DATA_ALIGN(size);

	data = kmalloc(size + sizeof(struct skb_shared_info), gfp_mask);
	if (!data)
		goto nodata;

#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
	/*add skb num*/
	atomic_inc(&g_used_skb_num);
#endif
	/* Copy only real data... and, alas, header. This should be
	 * optimized for the cases when header is void. */
#ifdef NET_SKBUFF_DATA_USES_OFFSET
	memcpy(data + nhead, skb->head, skb->tail);
#else
	memcpy(data + nhead, skb->head, skb->tail - skb->head);
#endif
	memcpy(data + size, skb_end_pointer(skb),
	       sizeof(struct skb_shared_info));

	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++)
		get_page(skb_shinfo(skb)->frags[i].page);

	if (skb_shinfo(skb)->frag_list)
		skb_clone_fraglist(skb);

#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
	skb->skb_recycling_callback = NULL;
#endif
	skb_release_data(skb);

	off = (data + nhead) - skb->head;

	skb->head     = data;
	skb->data    += off;
#ifdef NET_SKBUFF_DATA_USES_OFFSET
	skb->end      = size;
	off           = nhead;
#else
	skb->end      = skb->head + size;
#endif
	/* {transport,network,mac}_header and tail are relative to skb->head */
	skb->tail	      += off;
	skb->transport_header += off;
	skb->network_header   += off;
	skb->mac_header	      += off;
	skb->cloned   = 0;
	skb->hdr_len  = 0;
	skb->nohdr    = 0;
	atomic_set(&skb_shinfo(skb)->dataref, 1);
	return 0;

nodata:
	return -ENOMEM;
}

/* Make private copy of skb with writable head and some headroom */

struct sk_buff *skb_realloc_headroom(struct sk_buff *skb, unsigned int headroom)
{
	struct sk_buff *skb2;
	int delta = headroom - skb_headroom(skb);

	if (delta <= 0)
		skb2 = pskb_copy(skb, GFP_ATOMIC);
	else {
		skb2 = skb_clone(skb, GFP_ATOMIC);
		if (skb2 && pskb_expand_head(skb2, SKB_DATA_ALIGN(delta), 0,
					     GFP_ATOMIC)) {
			kfree_skb(skb2);
			skb2 = NULL;
		}
	}
	return skb2;
}


/**
 *	skb_copy_expand	-	copy and expand sk_buff
 *	@skb: buffer to copy
 *	@newheadroom: new free bytes at head
 *	@newtailroom: new free bytes at tail
 *	@gfp_mask: allocation priority
 *
 *	Make a copy of both an &sk_buff and its data and while doing so
 *	allocate additional space.
 *
 *	This is used when the caller wishes to modify the data and needs a
 *	private copy of the data to alter as well as more space for new fields.
 *	Returns %NULL on failure or the pointer to the buffer
 *	on success. The returned buffer has a reference count of 1.
 *
 *	You must pass %GFP_ATOMIC as the allocation priority if this function
 *	is called from an interrupt.
 *
 *	BUG ALERT: ip_summed is not copied. Why does this work? Is it used
 *	only by netfilter in the cases when checksum is recalculated? --ANK
 */
struct sk_buff *skb_copy_expand(const struct sk_buff *skb,
				int newheadroom, int newtailroom,
				gfp_t gfp_mask)
{
	/*
	 *	Allocate the copy buffer
	 */
	struct sk_buff *n = alloc_skb(newheadroom + skb->len + newtailroom,
				      gfp_mask);
	int oldheadroom = skb_headroom(skb);
	int head_copy_len, head_copy_off;
	int off = 0;

	if (!n)
		return NULL;

	skb_reserve(n, newheadroom);

	/* Set the tail pointer and length */
	skb_put(n, skb->len);

	head_copy_len = oldheadroom;
	head_copy_off = 0;
	if (newheadroom <= head_copy_len)
		head_copy_len = newheadroom;
	else
		head_copy_off = newheadroom - head_copy_len;

	/* Copy the linear header and data. */
	if (skb_copy_bits(skb, -head_copy_len, n->head + head_copy_off,
			  skb->len + head_copy_len))
		BUG();

	copy_skb_header(n, skb);

#ifdef NET_SKBUFF_DATA_USES_OFFSET
	off                  = newheadroom - oldheadroom;
#endif
	n->transport_header += off;
	n->network_header   += off;
	n->mac_header	    += off;

	return n;
}

/**
 *	skb_pad			-	zero pad the tail of an skb
 *	@skb: buffer to pad
 *	@pad: space to pad
 *
 *	Ensure that a buffer is followed by a padding area that is zero
 *	filled. Used by network drivers which may DMA or transfer data
 *	beyond the buffer end onto the wire.
 *
 *	May return error in out of memory cases. The skb is freed on error.
 */

int skb_pad(struct sk_buff *skb, int pad)
{
	int err;
	int ntail;

	/* If the skbuff is non linear tailroom is always zero.. */
	if (!skb_cloned(skb) && skb_tailroom(skb) >= pad) {
		memset(skb->data+skb->len, 0, pad);
		return 0;
	}

	ntail = skb->data_len + pad - (skb->end - skb->tail);
	if (likely(skb_cloned(skb) || ntail > 0)) {
		err = pskb_expand_head(skb, 0, ntail, GFP_ATOMIC);
		if (unlikely(err))
			goto free_skb;
	}

	/* FIXME: The use of this function with non-linear skb's really needs
	 * to be audited.
	 */
	err = skb_linearize(skb);
	if (unlikely(err))
		goto free_skb;

	memset(skb->data + skb->len, 0, pad);
	return 0;

free_skb:
	kfree_skb(skb);
	return err;
}

/* Trims skb to length len. It can change skb pointers.
 */

int ___pskb_trim(struct sk_buff *skb, unsigned int len)
{
	struct sk_buff **fragp;
	struct sk_buff *frag;
	int offset = skb_headlen(skb);
	int nfrags = skb_shinfo(skb)->nr_frags;
	int i;
	int err;

	if (skb_cloned(skb) &&
	    unlikely((err = pskb_expand_head(skb, 0, 0, GFP_ATOMIC))))
		return err;

	i = 0;
	if (offset >= len)
		goto drop_pages;

	for (; i < nfrags; i++) {
		int end = offset + skb_shinfo(skb)->frags[i].size;

		if (end < len) {
			offset = end;
			continue;
		}

		skb_shinfo(skb)->frags[i++].size = len - offset;

drop_pages:
		skb_shinfo(skb)->nr_frags = i;

		for (; i < nfrags; i++)
			put_page(skb_shinfo(skb)->frags[i].page);

		if (skb_shinfo(skb)->frag_list)
			skb_drop_fraglist(skb);
		goto done;
	}

	for (fragp = &skb_shinfo(skb)->frag_list; (frag = *fragp);
	     fragp = &frag->next) {
		int end = offset + frag->len;

		if (skb_shared(frag)) {
			struct sk_buff *nfrag;

			nfrag = skb_clone(frag, GFP_ATOMIC);
			if (unlikely(!nfrag))
				return -ENOMEM;

			nfrag->next = frag->next;
			kfree_skb(frag);
			frag = nfrag;
			*fragp = frag;
		}

		if (end < len) {
			offset = end;
			continue;
		}

		if (end > len &&
		    unlikely((err = pskb_trim(frag, len - offset))))
			return err;

		if (frag->next)
			skb_drop_list(&frag->next);
		break;
	}

done:
	if (len > skb_headlen(skb)) {
		skb->data_len -= skb->len - len;
		skb->len       = len;
	} else {
		skb->len       = len;
		skb->data_len  = 0;
		skb_set_tail_pointer(skb, len);
	}

	return 0;
}

/**
 *	__pskb_pull_tail - advance tail of skb header
 *	@skb: buffer to reallocate
 *	@delta: number of bytes to advance tail
 *
 *	The function makes a sense only on a fragmented &sk_buff,
 *	it expands header moving its tail forward and copying necessary
 *	data from fragmented part.
 *
 *	&sk_buff MUST have reference count of 1.
 *
 *	Returns %NULL (and &sk_buff does not change) if pull failed
 *	or value of new tail of skb in the case of success.
 *
 *	All the pointers pointing into skb header may change and must be
 *	reloaded after call to this function.
 */

/* Moves tail of skb head forward, copying data from fragmented part,
 * when it is necessary.
 * 1. It may fail due to malloc failure.
 * 2. It may change skb pointers.
 *
 * It is pretty complicated. Luckily, it is called only in exceptional cases.
 */
unsigned char *__pskb_pull_tail(struct sk_buff *skb, int delta)
{
	/* If skb has not enough free space at tail, get new one
	 * plus 128 bytes for future expansions. If we have enough
	 * room at tail, reallocate without expansion only if skb is cloned.
	 */
	int i, k, eat = (skb->tail + delta) - skb->end;

	if (eat > 0 || skb_cloned(skb)) {
		if (pskb_expand_head(skb, 0, eat > 0 ? eat + 128 : 0,
				     GFP_ATOMIC))
			return NULL;
	}

	if (skb_copy_bits(skb, skb_headlen(skb), skb_tail_pointer(skb), delta))
		BUG();

	/* Optimization: no fragments, no reasons to preestimate
	 * size of pulled pages. Superb.
	 */
	if (!skb_shinfo(skb)->frag_list)
		goto pull_pages;

	/* Estimate size of pulled pages. */
	eat = delta;
	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		if (skb_shinfo(skb)->frags[i].size >= eat)
			goto pull_pages;
		eat -= skb_shinfo(skb)->frags[i].size;
	}

	/* If we need update frag list, we are in troubles.
	 * Certainly, it possible to add an offset to skb data,
	 * but taking into account that pulling is expected to
	 * be very rare operation, it is worth to fight against
	 * further bloating skb head and crucify ourselves here instead.
	 * Pure masohism, indeed. 8)8)
	 */
	if (eat) {
		struct sk_buff *list = skb_shinfo(skb)->frag_list;
		struct sk_buff *clone = NULL;
		struct sk_buff *insp = NULL;

		do {
			BUG_ON(!list);

			if (list->len <= eat) {
				/* Eaten as whole. */
				eat -= list->len;
				list = list->next;
				insp = list;
			} else {
				/* Eaten partially. */

				if (skb_shared(list)) {
					/* Sucks! We need to fork list. :-( */
					clone = skb_clone(list, GFP_ATOMIC);
					if (!clone)
						return NULL;
					insp = list->next;
					list = clone;
				} else {
					/* This may be pulled without
					 * problems. */
					insp = list;
				}
				if (!pskb_pull(list, eat)) {
					if (clone)
						kfree_skb(clone);
					return NULL;
				}
				break;
			}
		} while (eat);

		/* Free pulled out fragments. */
		while ((list = skb_shinfo(skb)->frag_list) != insp) {
			skb_shinfo(skb)->frag_list = list->next;
			kfree_skb(list);
		}
		/* And insert new clone at head. */
		if (clone) {
			clone->next = list;
			skb_shinfo(skb)->frag_list = clone;
		}
	}
	/* Success! Now we may commit changes to skb data. */

pull_pages:
	eat = delta;
	k = 0;
	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		if (skb_shinfo(skb)->frags[i].size <= eat) {
			put_page(skb_shinfo(skb)->frags[i].page);
			eat -= skb_shinfo(skb)->frags[i].size;
		} else {
			skb_shinfo(skb)->frags[k] = skb_shinfo(skb)->frags[i];
			if (eat) {
				skb_shinfo(skb)->frags[k].page_offset += eat;
				skb_shinfo(skb)->frags[k].size -= eat;
				eat = 0;
			}
			k++;
		}
	}
	skb_shinfo(skb)->nr_frags = k;

	skb->tail     += delta;
	skb->data_len -= delta;

	return skb_tail_pointer(skb);
}

/* Copy some data bits from skb to kernel buffer. */

int skb_copy_bits(const struct sk_buff *skb, int offset, void *to, int len)
{
	int i, copy;
	int start = skb_headlen(skb);

	if (offset > (int)skb->len - len)
		goto fault;

	/* Copy header. */
	if ((copy = start - offset) > 0) {
		if (copy > len)
			copy = len;
		skb_copy_from_linear_data_offset(skb, offset, to, copy);
		if ((len -= copy) == 0)
			return 0;
		offset += copy;
		to     += copy;
	}

	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		int end;

		BUG_TRAP(start <= offset + len);

		end = start + skb_shinfo(skb)->frags[i].size;
		if ((copy = end - offset) > 0) {
			u8 *vaddr;

			if (copy > len)
				copy = len;

			vaddr = kmap_skb_frag(&skb_shinfo(skb)->frags[i]);
			memcpy(to,
			       vaddr + skb_shinfo(skb)->frags[i].page_offset+
			       offset - start, copy);
			kunmap_skb_frag(vaddr);

			if ((len -= copy) == 0)
				return 0;
			offset += copy;
			to     += copy;
		}
		start = end;
	}

	if (skb_shinfo(skb)->frag_list) {
		struct sk_buff *list = skb_shinfo(skb)->frag_list;

		for (; list; list = list->next) {
			int end;

			BUG_TRAP(start <= offset + len);

			end = start + list->len;
			if ((copy = end - offset) > 0) {
				if (copy > len)
					copy = len;
				if (skb_copy_bits(list, offset - start,
						  to, copy))
					goto fault;
				if ((len -= copy) == 0)
					return 0;
				offset += copy;
				to     += copy;
			}
			start = end;
		}
	}
	if (!len)
		return 0;

fault:
	return -EFAULT;
}

/**
 *	skb_store_bits - store bits from kernel buffer to skb
 *	@skb: destination buffer
 *	@offset: offset in destination
 *	@from: source buffer
 *	@len: number of bytes to copy
 *
 *	Copy the specified number of bytes from the source buffer to the
 *	destination skb.  This function handles all the messy bits of
 *	traversing fragment lists and such.
 */

int skb_store_bits(struct sk_buff *skb, int offset, const void *from, int len)
{
	int i, copy;
	int start = skb_headlen(skb);

	if (offset > (int)skb->len - len)
		goto fault;

	if ((copy = start - offset) > 0) {
		if (copy > len)
			copy = len;
		skb_copy_to_linear_data_offset(skb, offset, from, copy);
		if ((len -= copy) == 0)
			return 0;
		offset += copy;
		from += copy;
	}

	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		skb_frag_t *frag = &skb_shinfo(skb)->frags[i];
		int end;

		BUG_TRAP(start <= offset + len);

		end = start + frag->size;
		if ((copy = end - offset) > 0) {
			u8 *vaddr;

			if (copy > len)
				copy = len;

			vaddr = kmap_skb_frag(frag);
			memcpy(vaddr + frag->page_offset + offset - start,
			       from, copy);
			kunmap_skb_frag(vaddr);

			if ((len -= copy) == 0)
				return 0;
			offset += copy;
			from += copy;
		}
		start = end;
	}

	if (skb_shinfo(skb)->frag_list) {
		struct sk_buff *list = skb_shinfo(skb)->frag_list;

		for (; list; list = list->next) {
			int end;

			BUG_TRAP(start <= offset + len);

			end = start + list->len;
			if ((copy = end - offset) > 0) {
				if (copy > len)
					copy = len;
				if (skb_store_bits(list, offset - start,
						   from, copy))
					goto fault;
				if ((len -= copy) == 0)
					return 0;
				offset += copy;
				from += copy;
			}
			start = end;
		}
	}
	if (!len)
		return 0;

fault:
	return -EFAULT;
}

EXPORT_SYMBOL(skb_store_bits);

/* Checksum skb data. */

__wsum skb_checksum(const struct sk_buff *skb, int offset,
			  int len, __wsum csum)
{
	int start = skb_headlen(skb);
	int i, copy = start - offset;
	int pos = 0;

	/* Checksum header. */
	if (copy > 0) {
		if (copy > len)
			copy = len;
		csum = csum_partial(skb->data + offset, copy, csum);
		if ((len -= copy) == 0)
			return csum;
		offset += copy;
		pos	= copy;
	}

	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		int end;

		BUG_TRAP(start <= offset + len);

		end = start + skb_shinfo(skb)->frags[i].size;
		if ((copy = end - offset) > 0) {
			__wsum csum2;
			u8 *vaddr;
			skb_frag_t *frag = &skb_shinfo(skb)->frags[i];

			if (copy > len)
				copy = len;
			vaddr = kmap_skb_frag(frag);
			csum2 = csum_partial(vaddr + frag->page_offset +
					     offset - start, copy, 0);
			kunmap_skb_frag(vaddr);
			csum = csum_block_add(csum, csum2, pos);
			if (!(len -= copy))
				return csum;
			offset += copy;
			pos    += copy;
		}
		start = end;
	}

	if (skb_shinfo(skb)->frag_list) {
		struct sk_buff *list = skb_shinfo(skb)->frag_list;

		for (; list; list = list->next) {
			int end;

			BUG_TRAP(start <= offset + len);

			end = start + list->len;
			if ((copy = end - offset) > 0) {
				__wsum csum2;
				if (copy > len)
					copy = len;
				csum2 = skb_checksum(list, offset - start,
						     copy, 0);
				csum = csum_block_add(csum, csum2, pos);
				if ((len -= copy) == 0)
					return csum;
				offset += copy;
				pos    += copy;
			}
			start = end;
		}
	}
	BUG_ON(len);

	return csum;
}

/* Both of above in one bottle. */

__wsum skb_copy_and_csum_bits(const struct sk_buff *skb, int offset,
				    u8 *to, int len, __wsum csum)
{
	int start = skb_headlen(skb);
	int i, copy = start - offset;
	int pos = 0;

	/* Copy header. */
	if (copy > 0) {
		if (copy > len)
			copy = len;
		csum = csum_partial_copy_nocheck(skb->data + offset, to,
						 copy, csum);
		if ((len -= copy) == 0)
			return csum;
		offset += copy;
		to     += copy;
		pos	= copy;
	}

	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		int end;

		BUG_TRAP(start <= offset + len);

		end = start + skb_shinfo(skb)->frags[i].size;
		if ((copy = end - offset) > 0) {
			__wsum csum2;
			u8 *vaddr;
			skb_frag_t *frag = &skb_shinfo(skb)->frags[i];

			if (copy > len)
				copy = len;
			vaddr = kmap_skb_frag(frag);
			csum2 = csum_partial_copy_nocheck(vaddr +
							  frag->page_offset +
							  offset - start, to,
							  copy, 0);
			kunmap_skb_frag(vaddr);
			csum = csum_block_add(csum, csum2, pos);
			if (!(len -= copy))
				return csum;
			offset += copy;
			to     += copy;
			pos    += copy;
		}
		start = end;
	}

	if (skb_shinfo(skb)->frag_list) {
		struct sk_buff *list = skb_shinfo(skb)->frag_list;

		for (; list; list = list->next) {
			__wsum csum2;
			int end;

			BUG_TRAP(start <= offset + len);

			end = start + list->len;
			if ((copy = end - offset) > 0) {
				if (copy > len)
					copy = len;
				csum2 = skb_copy_and_csum_bits(list,
							       offset - start,
							       to, copy, 0);
				csum = csum_block_add(csum, csum2, pos);
				if ((len -= copy) == 0)
					return csum;
				offset += copy;
				to     += copy;
				pos    += copy;
			}
			start = end;
		}
	}
	BUG_ON(len);
	return csum;
}

void skb_copy_and_csum_dev(const struct sk_buff *skb, u8 *to)
{
	__wsum csum;
	long csstart;

	if (skb->ip_summed == CHECKSUM_PARTIAL)
		csstart = skb->csum_start - skb_headroom(skb);
	else
		csstart = skb_headlen(skb);

	BUG_ON(csstart > skb_headlen(skb));

	skb_copy_from_linear_data(skb, to, csstart);

	csum = 0;
	if (csstart != skb->len)
		csum = skb_copy_and_csum_bits(skb, csstart, to + csstart,
					      skb->len - csstart, 0);

	if (skb->ip_summed == CHECKSUM_PARTIAL) {
		long csstuff = csstart + skb->csum_offset;

		*((__sum16 *)(to + csstuff)) = csum_fold(csum);
	}
}

/**
 *	skb_dequeue - remove from the head of the queue
 *	@list: list to dequeue from
 *
 *	Remove the head of the list. The list lock is taken so the function
 *	may be used safely with other locking list functions. The head item is
 *	returned or %NULL if the list is empty.
 */

struct sk_buff *skb_dequeue(struct sk_buff_head *list)
{
	unsigned long flags;
	struct sk_buff *result;

	spin_lock_irqsave(&list->lock, flags);
	result = __skb_dequeue(list);
	spin_unlock_irqrestore(&list->lock, flags);
	return result;
}

/**
 *	skb_dequeue_tail - remove from the tail of the queue
 *	@list: list to dequeue from
 *
 *	Remove the tail of the list. The list lock is taken so the function
 *	may be used safely with other locking list functions. The tail item is
 *	returned or %NULL if the list is empty.
 */
struct sk_buff *skb_dequeue_tail(struct sk_buff_head *list)
{
	unsigned long flags;
	struct sk_buff *result;

	spin_lock_irqsave(&list->lock, flags);
	result = __skb_dequeue_tail(list);
	spin_unlock_irqrestore(&list->lock, flags);
	return result;
}

/**
 *	skb_queue_purge - empty a list
 *	@list: list to empty
 *
 *	Delete all buffers on an &sk_buff list. Each buffer is removed from
 *	the list and one reference dropped. This function takes the list
 *	lock and is atomic with respect to other list locking functions.
 */
void skb_queue_purge(struct sk_buff_head *list)
{
	struct sk_buff *skb;
	while ((skb = skb_dequeue(list)) != NULL)
		kfree_skb(skb);
}

/**
 *	skb_queue_head - queue a buffer at the list head
 *	@list: list to use
 *	@newsk: buffer to queue
 *
 *	Queue a buffer at the start of the list. This function takes the
 *	list lock and can be used safely with other locking &sk_buff functions
 *	safely.
 *
 *	A buffer cannot be placed on two lists at the same time.
 */
void skb_queue_head(struct sk_buff_head *list, struct sk_buff *newsk)
{
	unsigned long flags;

	spin_lock_irqsave(&list->lock, flags);
	__skb_queue_head(list, newsk);
	spin_unlock_irqrestore(&list->lock, flags);
}

/**
 *	skb_queue_tail - queue a buffer at the list tail
 *	@list: list to use
 *	@newsk: buffer to queue
 *
 *	Queue a buffer at the tail of the list. This function takes the
 *	list lock and can be used safely with other locking &sk_buff functions
 *	safely.
 *
 *	A buffer cannot be placed on two lists at the same time.
 */
void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk)
{
	unsigned long flags;

	spin_lock_irqsave(&list->lock, flags);
	__skb_queue_tail(list, newsk);
	spin_unlock_irqrestore(&list->lock, flags);
}

/**
 *	skb_unlink	-	remove a buffer from a list
 *	@skb: buffer to remove
 *	@list: list to use
 *
 *	Remove a packet from a list. The list locks are taken and this
 *	function is atomic with respect to other list locked calls
 *
 *	You must know what list the SKB is on.
 */
void skb_unlink(struct sk_buff *skb, struct sk_buff_head *list)
{
	unsigned long flags;

	spin_lock_irqsave(&list->lock, flags);
	__skb_unlink(skb, list);
	spin_unlock_irqrestore(&list->lock, flags);
}

/**
 *	skb_append	-	append a buffer
 *	@old: buffer to insert after
 *	@newsk: buffer to insert
 *	@list: list to use
 *
 *	Place a packet after a given packet in a list. The list locks are taken
 *	and this function is atomic with respect to other list locked calls.
 *	A buffer cannot be placed on two lists at the same time.
 */
void skb_append(struct sk_buff *old, struct sk_buff *newsk, struct sk_buff_head *list)
{
	unsigned long flags;

	spin_lock_irqsave(&list->lock, flags);
	__skb_append(old, newsk, list);
	spin_unlock_irqrestore(&list->lock, flags);
}


/**
 *	skb_insert	-	insert a buffer
 *	@old: buffer to insert before
 *	@newsk: buffer to insert
 *	@list: list to use
 *
 *	Place a packet before a given packet in a list. The list locks are
 * 	taken and this function is atomic with respect to other list locked
 *	calls.
 *
 *	A buffer cannot be placed on two lists at the same time.
 */
void skb_insert(struct sk_buff *old, struct sk_buff *newsk, struct sk_buff_head *list)
{
	unsigned long flags;

	spin_lock_irqsave(&list->lock, flags);
	__skb_insert(newsk, old->prev, old, list);
	spin_unlock_irqrestore(&list->lock, flags);
}

static inline void skb_split_inside_header(struct sk_buff *skb,
					   struct sk_buff* skb1,
					   const u32 len, const int pos)
{
	int i;

	skb_copy_from_linear_data_offset(skb, len, skb_put(skb1, pos - len),
					 pos - len);
	/* And move data appendix as is. */
	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++)
		skb_shinfo(skb1)->frags[i] = skb_shinfo(skb)->frags[i];

	skb_shinfo(skb1)->nr_frags = skb_shinfo(skb)->nr_frags;
	skb_shinfo(skb)->nr_frags  = 0;
	skb1->data_len		   = skb->data_len;
	skb1->len		   += skb1->data_len;
	skb->data_len		   = 0;
	skb->len		   = len;
	skb_set_tail_pointer(skb, len);
}

static inline void skb_split_no_header(struct sk_buff *skb,
				       struct sk_buff* skb1,
				       const u32 len, int pos)
{
	int i, k = 0;
	const int nfrags = skb_shinfo(skb)->nr_frags;

	skb_shinfo(skb)->nr_frags = 0;
	skb1->len		  = skb1->data_len = skb->len - len;
	skb->len		  = len;
	skb->data_len		  = len - pos;

	for (i = 0; i < nfrags; i++) {
		int size = skb_shinfo(skb)->frags[i].size;

		if (pos + size > len) {
			skb_shinfo(skb1)->frags[k] = skb_shinfo(skb)->frags[i];

			if (pos < len) {
				/* Split frag.
				 * We have two variants in this case:
				 * 1. Move all the frag to the second
				 *    part, if it is possible. F.e.
				 *    this approach is mandatory for TUX,
				 *    where splitting is expensive.
				 * 2. Split is accurately. We make this.
				 */
				get_page(skb_shinfo(skb)->frags[i].page);
				skb_shinfo(skb1)->frags[0].page_offset += len - pos;
				skb_shinfo(skb1)->frags[0].size -= len - pos;
				skb_shinfo(skb)->frags[i].size	= len - pos;
				skb_shinfo(skb)->nr_frags++;
			}
			k++;
		} else
			skb_shinfo(skb)->nr_frags++;
		pos += size;
	}
	skb_shinfo(skb1)->nr_frags = k;
}

/**
 * skb_split - Split fragmented skb to two parts at length len.
 * @skb: the buffer to split
 * @skb1: the buffer to receive the second part
 * @len: new length for skb
 */
void skb_split(struct sk_buff *skb, struct sk_buff *skb1, const u32 len)
{
	int pos = skb_headlen(skb);

	if (len < pos)	/* Split line is inside header. */
		skb_split_inside_header(skb, skb1, len, pos);
	else		/* Second chunk has no header, nothing to copy. */
		skb_split_no_header(skb, skb1, len, pos);
}

/**
 * skb_prepare_seq_read - Prepare a sequential read of skb data
 * @skb: the buffer to read
 * @from: lower offset of data to be read
 * @to: upper offset of data to be read
 * @st: state variable
 *
 * Initializes the specified state variable. Must be called before
 * invoking skb_seq_read() for the first time.
 */
void skb_prepare_seq_read(struct sk_buff *skb, unsigned int from,
			  unsigned int to, struct skb_seq_state *st)
{
	st->lower_offset = from;
	st->upper_offset = to;
	st->root_skb = st->cur_skb = skb;
	st->frag_idx = st->stepped_offset = 0;
	st->frag_data = NULL;
}

/**
 * skb_seq_read - Sequentially read skb data
 * @consumed: number of bytes consumed by the caller so far
 * @data: destination pointer for data to be returned
 * @st: state variable
 *
 * Reads a block of skb data at &consumed relative to the
 * lower offset specified to skb_prepare_seq_read(). Assigns
 * the head of the data block to &data and returns the length
 * of the block or 0 if the end of the skb data or the upper
 * offset has been reached.
 *
 * The caller is not required to consume all of the data
 * returned, i.e. &consumed is typically set to the number
 * of bytes already consumed and the next call to
 * skb_seq_read() will return the remaining part of the block.
 *
 * Note: The size of each block of data returned can be arbitary,
 *       this limitation is the cost for zerocopy seqeuental
 *       reads of potentially non linear data.
 *
 * Note: Fragment lists within fragments are not implemented
 *       at the moment, state->root_skb could be replaced with
 *       a stack for this purpose.
 */
unsigned int skb_seq_read(unsigned int consumed, const u8 **data,
			  struct skb_seq_state *st)
{
	unsigned int block_limit, abs_offset = consumed + st->lower_offset;
	skb_frag_t *frag;

	if (unlikely(abs_offset >= st->upper_offset))
		return 0;

next_skb:
	block_limit = skb_headlen(st->cur_skb);

	if (abs_offset < block_limit) {
		*data = st->cur_skb->data + abs_offset;
		return block_limit - abs_offset;
	}

	if (st->frag_idx == 0 && !st->frag_data)
		st->stepped_offset += skb_headlen(st->cur_skb);

	while (st->frag_idx < skb_shinfo(st->cur_skb)->nr_frags) {
		frag = &skb_shinfo(st->cur_skb)->frags[st->frag_idx];
		block_limit = frag->size + st->stepped_offset;

		if (abs_offset < block_limit) {
			if (!st->frag_data)
				st->frag_data = kmap_skb_frag(frag);

			*data = (u8 *) st->frag_data + frag->page_offset +
				(abs_offset - st->stepped_offset);

			return block_limit - abs_offset;
		}

		if (st->frag_data) {
			kunmap_skb_frag(st->frag_data);
			st->frag_data = NULL;
		}

		st->frag_idx++;
		st->stepped_offset += frag->size;
	}

	if (st->frag_data) {
		kunmap_skb_frag(st->frag_data);
		st->frag_data = NULL;
	}

	if (st->cur_skb->next) {
		st->cur_skb = st->cur_skb->next;
		st->frag_idx = 0;
		goto next_skb;
	} else if (st->root_skb == st->cur_skb &&
		   skb_shinfo(st->root_skb)->frag_list) {
		st->cur_skb = skb_shinfo(st->root_skb)->frag_list;
		goto next_skb;
	}

	return 0;
}

/**
 * skb_abort_seq_read - Abort a sequential read of skb data
 * @st: state variable
 *
 * Must be called if skb_seq_read() was not called until it
 * returned 0.
 */
void skb_abort_seq_read(struct skb_seq_state *st)
{
	if (st->frag_data)
		kunmap_skb_frag(st->frag_data);
}

#define TS_SKB_CB(state)	((struct skb_seq_state *) &((state)->cb))

static unsigned int skb_ts_get_next_block(unsigned int offset, const u8 **text,
					  struct ts_config *conf,
					  struct ts_state *state)
{
	return skb_seq_read(offset, text, TS_SKB_CB(state));
}

static void skb_ts_finish(struct ts_config *conf, struct ts_state *state)
{
	skb_abort_seq_read(TS_SKB_CB(state));
}

/**
 * skb_find_text - Find a text pattern in skb data
 * @skb: the buffer to look in
 * @from: search offset
 * @to: search limit
 * @config: textsearch configuration
 * @state: uninitialized textsearch state variable
 *
 * Finds a pattern in the skb data according to the specified
 * textsearch configuration. Use textsearch_next() to retrieve
 * subsequent occurrences of the pattern. Returns the offset
 * to the first occurrence or UINT_MAX if no match was found.
 */
unsigned int skb_find_text(struct sk_buff *skb, unsigned int from,
			   unsigned int to, struct ts_config *config,
			   struct ts_state *state)
{
	unsigned int ret;

	config->get_next_block = skb_ts_get_next_block;
	config->finish = skb_ts_finish;

	skb_prepare_seq_read(skb, from, to, TS_SKB_CB(state));

	ret = textsearch_find(config, state);
	return (ret <= to - from ? ret : UINT_MAX);
}

/**
 * skb_append_datato_frags: - append the user data to a skb
 * @sk: sock  structure
 * @skb: skb structure to be appened with user data.
 * @getfrag: call back function to be used for getting the user data
 * @from: pointer to user message iov
 * @length: length of the iov message
 *
 * Description: This procedure append the user data in the fragment part
 * of the skb if any page alloc fails user this procedure returns  -ENOMEM
 */
int skb_append_datato_frags(struct sock *sk, struct sk_buff *skb,
			int (*getfrag)(void *from, char *to, int offset,
					int len, int odd, struct sk_buff *skb),
			void *from, int length)
{
	int frg_cnt = 0;
	skb_frag_t *frag = NULL;
	struct page *page = NULL;
	int copy, left;
	int offset = 0;
	int ret;

	do {
		/* Return error if we don't have space for new frag */
		frg_cnt = skb_shinfo(skb)->nr_frags;
		if (frg_cnt >= MAX_SKB_FRAGS)
			return -EFAULT;

		/* allocate a new page for next frag */
		page = alloc_pages(sk->sk_allocation, 0);

		/* If alloc_page fails just return failure and caller will
		 * free previous allocated pages by doing kfree_skb()
		 */
		if (page == NULL)
			return -ENOMEM;

		/* initialize the next frag */
		sk->sk_sndmsg_page = page;
		sk->sk_sndmsg_off = 0;
		skb_fill_page_desc(skb, frg_cnt, page, 0, 0);
		skb->truesize += PAGE_SIZE;
		atomic_add(PAGE_SIZE, &sk->sk_wmem_alloc);

		/* get the new initialized frag */
		frg_cnt = skb_shinfo(skb)->nr_frags;
		frag = &skb_shinfo(skb)->frags[frg_cnt - 1];

		/* copy the user data to page */
		left = PAGE_SIZE - frag->page_offset;
		copy = (length > left)? left : length;

		ret = getfrag(from, (page_address(frag->page) +
			    frag->page_offset + frag->size),
			    offset, copy, 0, skb);
		if (ret < 0)
			return -EFAULT;

		/* copy was successful so update the size parameters */
		sk->sk_sndmsg_off += copy;
		frag->size += copy;
		skb->len += copy;
		skb->data_len += copy;
		offset += copy;
		length -= copy;

	} while (length > 0);

	return 0;
}

/**
 *	skb_pull_rcsum - pull skb and update receive checksum
 *	@skb: buffer to update
 *	@start: start of data before pull
 *	@len: length of data pulled
 *
 *	This function performs an skb_pull on the packet and updates
 *	update the CHECKSUM_COMPLETE checksum.  It should be used on
 *	receive path processing instead of skb_pull unless you know
 *	that the checksum difference is zero (e.g., a valid IP header)
 *	or you are setting ip_summed to CHECKSUM_NONE.
 */
unsigned char *skb_pull_rcsum(struct sk_buff *skb, unsigned int len)
{
	BUG_ON(len > skb->len);
	skb->len -= len;
	BUG_ON(skb->len < skb->data_len);
	skb_postpull_rcsum(skb, skb->data, len);
	return skb->data += len;
}

EXPORT_SYMBOL_GPL(skb_pull_rcsum);

/**
 *	skb_segment - Perform protocol segmentation on skb.
 *	@skb: buffer to segment
 *	@features: features for the output path (see dev->features)
 *
 *	This function performs segmentation on the given skb.  It returns
 *	the segment at the given position.  It returns NULL if there are
 *	no more segments to generate, or when an error is encountered.
 */
struct sk_buff *skb_segment(struct sk_buff *skb, int features)
{
	struct sk_buff *segs = NULL;
	struct sk_buff *tail = NULL;
	unsigned int mss = skb_shinfo(skb)->gso_size;
	unsigned int doffset = skb->data - skb_mac_header(skb);
	unsigned int offset = doffset;
	unsigned int headroom;
	unsigned int len;
	int sg = features & NETIF_F_SG;
	int nfrags = skb_shinfo(skb)->nr_frags;
	int err = -ENOMEM;
	int i = 0;
	int pos;

	__skb_push(skb, doffset);
	headroom = skb_headroom(skb);
	pos = skb_headlen(skb);

	do {
		struct sk_buff *nskb;
		skb_frag_t *frag;
		int hsize;
		int k;
		int size;

		len = skb->len - offset;
		if (len > mss)
			len = mss;

		hsize = skb_headlen(skb) - offset;
		if (hsize < 0)
			hsize = 0;
		if (hsize > len || !sg)
			hsize = len;

		nskb = alloc_skb(hsize + doffset + headroom, GFP_ATOMIC);
		if (unlikely(!nskb))
			goto err;

		if (segs)
			tail->next = nskb;
		else
			segs = nskb;
		tail = nskb;

		nskb->dev = skb->dev;
		nskb->priority = skb->priority;
		nskb->protocol = skb->protocol;
		nskb->dst = dst_clone(skb->dst);
		memcpy(nskb->cb, skb->cb, sizeof(skb->cb));
		nskb->pkt_type = skb->pkt_type;
		nskb->mac_len = skb->mac_len;

		skb_reserve(nskb, headroom);
		skb_reset_mac_header(nskb);
		skb_set_network_header(nskb, skb->mac_len);
		nskb->transport_header = (nskb->network_header +
					  skb_network_header_len(skb));
		skb_copy_from_linear_data(skb, skb_put(nskb, doffset),
					  doffset);
		if (!sg) {
			nskb->csum = skb_copy_and_csum_bits(skb, offset,
							    skb_put(nskb, len),
							    len, 0);
			continue;
		}

		frag = skb_shinfo(nskb)->frags;
		k = 0;

		nskb->ip_summed = CHECKSUM_PARTIAL;
		nskb->csum = skb->csum;
		skb_copy_from_linear_data_offset(skb, offset,
						 skb_put(nskb, hsize), hsize);

		while (pos < offset + len) {
			BUG_ON(i >= nfrags);

			*frag = skb_shinfo(skb)->frags[i];
			get_page(frag->page);
			size = frag->size;

			if (pos < offset) {
				frag->page_offset += offset - pos;
				frag->size -= offset - pos;
			}

			k++;

			if (pos + size <= offset + len) {
				i++;
				pos += size;
			} else {
				frag->size -= pos + size - (offset + len);
				break;
			}

			frag++;
		}

		skb_shinfo(nskb)->nr_frags = k;
		nskb->data_len = len - hsize;
		nskb->len += nskb->data_len;
		nskb->truesize += nskb->data_len;
	} while ((offset += len) < skb->len);

	return segs;

err:
	while ((skb = segs)) {
		segs = skb->next;
		kfree_skb(skb);
	}
	return ERR_PTR(err);
}

EXPORT_SYMBOL_GPL(skb_segment);

#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)

#define SKBMGR_RX_BUF_LEN 				SKB_WITH_OVERHEAD(2048)
#define SKBMGR_DEF_HOT_LIST_LEN			512

__DMEM int skbmgr_limit = 1280;
__DMEM int skbmgr_max_alloc_no = 0;
__DMEM int skbmgr_alloc_fail = 0;

__DMEM int skbmgr_hot_list_len = SKBMGR_DEF_HOT_LIST_LEN;
__DMEM int skbmgr_max_list_len = 0;
EXPORT_SYMBOL(skbmgr_max_alloc_no);
#ifdef SKBMGR_SINGLE_QUEUE
__DMEM union {
	struct sk_buff_head     list;
	char                    pad[SMP_CACHE_BYTES];
} skbmgr_pool[1];
#else
__DMEM union {
	struct sk_buff_head     list;
	char                    pad[SMP_CACHE_BYTES];
} skbmgr_pool[NR_CPUS];
#endif

__IMEM struct sk_buff *skbmgr_alloc_skb2k(void)
{
	struct sk_buff_head *list;
	struct sk_buff *skb;
	int alloc_no;
	unsigned long flags;
	
#ifdef SKBMGR_SINGLE_QUEUE	
	list = &skbmgr_pool[0].list;
#else
	list = &skbmgr_pool[smp_processor_id()].list;
#endif

	if (skb_queue_len(list)) {
		unsigned int size;
		struct skb_shared_info *shinfo;
		u8 *data;

#ifdef SKBMGR_SINGLE_QUEUE
		spin_lock_irqsave(&skbmgr_lock, flags);		
		skb = __skb_dequeue(list);
		spin_unlock_irqrestore(&skbmgr_lock, flags);	
#else
 		local_irq_save(flags);
		skb = __skb_dequeue(list);
		local_irq_restore(flags);
#endif		

		if (unlikely(skb == NULL))
			goto try_normal;

		size = skb->truesize - sizeof(struct sk_buff);
		data = skb->head;

		/*
		 * See comment in sk_buff definition, just before the 'tail' member
		 */
		memset(skb, 0, offsetof(struct sk_buff, tail));
		skb->truesize = size + sizeof(struct sk_buff);
		atomic_set(&skb->users, 1);
		skb->head = data;
		skb->data = data;
		skb_reset_tail_pointer(skb);
		skb->end = skb->tail + size;
		/* make sure we initialize shinfo sequentially */
		shinfo = skb_shinfo(skb);
		atomic_set(&shinfo->dataref, 1);
		shinfo->nr_frags  = 0;
		shinfo->gso_size = 0;
		shinfo->gso_segs = 0;
		shinfo->gso_type = 0;
		shinfo->ip6_frag_id = 0;
		shinfo->frag_list = NULL;

		skb->skb_recycling_callback = skbmgr_recycling_callback;
		skb->skb_recycling_ind = SKBMGR_INDICATION;

		atomic_inc(&skbmgr_alloc_no);
		alloc_no = atomic_read(&skbmgr_alloc_no);
		if (alloc_no > skbmgr_max_alloc_no)
			skbmgr_max_alloc_no = alloc_no;

		return skb;
	}

try_normal:
	if ((skbmgr_limit == 0) || (atomic_read(&skbmgr_alloc_no) < skbmgr_limit)) {
#if !defined(TCSUPPORT_CT) 
#if defined(TCSUPPORT_MEMORY_CONTROL)
	#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
		skb = alloc_skb(SKBMGR_RX_BUF_LEN, GFP_ATOMIC|__GFP_NOWARN | __GFP_TCMC);/*jlliu*/
	#endif
#else
		skb = alloc_skb(SKBMGR_RX_BUF_LEN, GFP_ATOMIC|__GFP_NOWARN);
#endif
#endif

		if (likely(skb)) {
			skb->skb_recycling_callback = skbmgr_recycling_callback;
			skb->skb_recycling_ind = SKBMGR_INDICATION;

			atomic_inc(&skbmgr_alloc_no);
			alloc_no = atomic_read(&skbmgr_alloc_no);
			if (alloc_no > skbmgr_max_alloc_no)
				skbmgr_max_alloc_no = alloc_no;
		} else {
			skbmgr_alloc_fail++;
		}
	} else {
		skb = NULL;
		skbmgr_alloc_fail++;
	}
	return skb;
}

EXPORT_SYMBOL(skbmgr_alloc_skb2k);

__IMEM int skbmgr_recycling_callback(struct sk_buff *skb)
{
	struct sk_buff_head *list;
	unsigned long flags;

#ifdef SKBMGR_SINGLE_QUEUE
	list = &skbmgr_pool[0].list;	
#else
	list = &skbmgr_pool[smp_processor_id()].list;
#endif	

	if (skb_queue_len(list) < skbmgr_hot_list_len) {
		if ((skb->truesize - sizeof(struct sk_buff) != SKBMGR_RX_BUF_LEN) ||
			(skb_shinfo(skb)->nr_frags) ||
			(skb_shinfo(skb)->frag_list)) {
			return 0;
		}

#if defined(TCSUPPORT_HWNAT)		
		pktflow_free(skb);		
#endif
#ifdef TCSUPPORT_RA_HWNAT
		if (ra_sw_nat_hook_free)
			ra_sw_nat_hook_free(skb);
#endif

		if (skb_queue_len(list) > skbmgr_max_list_len)
			skbmgr_max_list_len = skb_queue_len(list) + 1;

#ifdef SKBMGR_SINGLE_QUEUE
		spin_lock_irqsave(&skbmgr_lock, flags);
		__skb_queue_head(list, skb);
		spin_unlock_irqrestore(&skbmgr_lock, flags);
#else
		local_irq_save(flags);
		__skb_queue_head(list, skb);
		local_irq_restore(flags);
#endif		

		return 1;
	}

	return 0;
}

EXPORT_SYMBOL(skbmgr_recycling_callback);
//use for wifi driver, size 3840 is accord to wifi driver RX_BUFFER_AGGRESIZE.
#define SKBMGR_4K_RX_BUF_LEN 			SKB_WITH_OVERHEAD(4096)
#define SKBMGR_4K_DEF_HOT_LIST_LEN		128

#ifdef MT7592
int skbmgr_4k_limit = 1024;
#else
int skbmgr_4k_limit = 512;
#endif

int skbmgr_4k_max_alloc_no = 0;
int skbmgr_4k_alloc_fail = 0;
int skbmgr_4k_hot_list_len = SKBMGR_4K_DEF_HOT_LIST_LEN;
int skbmgr_4k_max_list_len = 0;
EXPORT_SYMBOL(skbmgr_4k_max_alloc_no);
#ifdef SKBMGR_SINGLE_QUEUE
union {
	struct sk_buff_head     list;
	char                    pad[SMP_CACHE_BYTES];
} skbmgr_4k_pool[1];
#else
union {
	struct sk_buff_head     list;
	char                    pad[SMP_CACHE_BYTES];
} skbmgr_4k_pool[NR_CPUS];
#endif

#ifdef CONFIG_MIPS_TC3162U
__IMEM
#endif
struct sk_buff *skbmgr_alloc_skb4k(void)
{
	struct sk_buff_head *list;
	struct sk_buff *skb;
	int alloc_no;
	unsigned long flags;
	
#ifdef SKBMGR_SINGLE_QUEUE	
	list = &skbmgr_4k_pool[0].list;
#else
	list = &skbmgr_4k_pool[smp_processor_id()].list;
#endif
	
	if (skb_queue_len(list)) {
		unsigned int size;
		struct skb_shared_info *shinfo;
		u8 *data;

#ifdef SKBMGR_SINGLE_QUEUE
		spin_lock_irqsave(&skbmgr_4k_lock, flags);		
		skb = __skb_dequeue(list);
		spin_unlock_irqrestore(&skbmgr_4k_lock, flags);	
#else
 		local_irq_save(flags);
		skb = __skb_dequeue(list);
		local_irq_restore(flags);
#endif		

		if (unlikely(skb == NULL))
			goto try_normal;
		
		size = skb->truesize - sizeof(struct sk_buff);
		data = skb->head;

		/*
		 * See comment in sk_buff definition, just before the 'tail' member
		 */
		memset(skb, 0, offsetof(struct sk_buff, tail));
		skb->truesize = size + sizeof(struct sk_buff);
		atomic_set(&skb->users, 1);
		skb->head = data;
		skb->data = data;
		skb_reset_tail_pointer(skb);
		skb->end = skb->tail + size;
		/* make sure we initialize shinfo sequentially */
		shinfo = skb_shinfo(skb);
		atomic_set(&shinfo->dataref, 1);
		shinfo->nr_frags  = 0;
		shinfo->gso_size = 0;
		shinfo->gso_segs = 0;
		shinfo->gso_type = 0;
		shinfo->ip6_frag_id = 0;
		shinfo->frag_list = NULL;

		skb->skb_recycling_callback = skbmgr_4k_recycling_callback;
		skb->skb_recycling_ind = SKBMGR_4K_INDICATION;

		atomic_inc(&skbmgr_4k_alloc_no);
		alloc_no = atomic_read(&skbmgr_4k_alloc_no);
		if (alloc_no > skbmgr_4k_max_alloc_no)
			skbmgr_4k_max_alloc_no = alloc_no;

		return skb;
	}

try_normal:
	if ((atomic_read(&skbmgr_4k_alloc_no) < skbmgr_4k_limit)) {
		skb = alloc_skb(SKBMGR_4K_RX_BUF_LEN , GFP_ATOMIC|__GFP_NOWARN);
		if (likely(skb)) {
			skb->skb_recycling_callback = skbmgr_4k_recycling_callback;
			skb->skb_recycling_ind = SKBMGR_4K_INDICATION;
			atomic_inc(&skbmgr_4k_alloc_no);
			alloc_no = atomic_read(&skbmgr_4k_alloc_no);
			if (alloc_no > skbmgr_4k_max_alloc_no)
				skbmgr_4k_max_alloc_no = alloc_no;
		} else {
			skbmgr_4k_alloc_fail++;
		}
	} else {
		skb = NULL;
		skbmgr_4k_alloc_fail++;
	}
	return skb;
}

EXPORT_SYMBOL(skbmgr_alloc_skb4k);
#ifdef CONFIG_MIPS_TC3162U
__IMEM
#endif
int skbmgr_4k_recycling_callback(struct sk_buff *skb)
{
	struct sk_buff_head *list;
	unsigned long flags;

#ifdef SKBMGR_SINGLE_QUEUE
	list = &skbmgr_4k_pool[0].list;	
#else
	list = &skbmgr_4k_pool[smp_processor_id()].list;
#endif

	if (skb_queue_len(list) < skbmgr_4k_hot_list_len) {
		if ((skb->truesize - sizeof(struct sk_buff) != SKBMGR_4K_RX_BUF_LEN) ||
			(skb_shinfo(skb)->nr_frags) ||
			(skb_shinfo(skb)->frag_list)) {
			return 0;
		}

#if defined(TCSUPPORT_HWNAT)		
		pktflow_free(skb);		
#endif
#ifdef TCSUPPORT_RA_HWNAT
		if (ra_sw_nat_hook_free)
			ra_sw_nat_hook_free(skb);
#endif

		if (skb_queue_len(list) > skbmgr_4k_max_list_len)
			skbmgr_4k_max_list_len = skb_queue_len(list) + 1;

#ifdef SKBMGR_SINGLE_QUEUE
		spin_lock_irqsave(&skbmgr_4k_lock, flags);
		__skb_queue_head(list, skb);
		spin_unlock_irqrestore(&skbmgr_4k_lock, flags);
#else
		local_irq_save(flags);
		__skb_queue_head(list, skb);
		local_irq_restore(flags);
#endif		

		return 1;
	}

	return 0;
}

EXPORT_SYMBOL(skbmgr_4k_recycling_callback);
#if defined(CONFIG_MIPS_TC3262)

#define SKBMGR_SG_RX_BUF_LEN 			SKB_DATA_ALIGN(128+NET_SKB_PAD)

__DMEM int skbmgr_sg_max_list_len = 0;

__DMEM union {
	struct sk_buff_head     list;
	char                    pad[SMP_CACHE_BYTES];
} skbmgr_sg_pool[NR_CPUS];

struct sk_buff *skbmgr_alloc_skb128(void)
{
	struct sk_buff_head *list = &skbmgr_sg_pool[smp_processor_id()].list;
	struct sk_buff *skb;

	if (skb_queue_len(list)) {
		unsigned long flags;
		unsigned int size;
		struct skb_shared_info *shinfo;
		u8 *data;

 		local_irq_save(flags);
		skb = __skb_dequeue(list);
		local_irq_restore(flags);

		if (unlikely(skb == NULL))
			goto try_normal;

		size = skb->truesize - sizeof(struct sk_buff);
		data = skb->head;

		/*
		 * See comment in sk_buff definition, just before the 'tail' member
		 */
		memset(skb, 0, offsetof(struct sk_buff, tail));
		skb->truesize = size + sizeof(struct sk_buff);
		atomic_set(&skb->users, 1);
		skb->head = data;
		skb->data = data;
		skb_reset_tail_pointer(skb);
		skb->end = skb->tail + size;
		/* make sure we initialize shinfo sequentially */
		shinfo = skb_shinfo(skb);
		atomic_set(&shinfo->dataref, 1);
		shinfo->nr_frags  = 0;
		shinfo->gso_size = 0;
		shinfo->gso_segs = 0;
		shinfo->gso_type = 0;
		shinfo->ip6_frag_id = 0;
		shinfo->frag_list = NULL;

		skb->skb_recycling_callback = skbmgr_sg_recycling_callback;

		return skb;
	}

try_normal:
	skb = alloc_skb(SKBMGR_SG_RX_BUF_LEN, GFP_ATOMIC|__GFP_NOWARN);
	if (likely(skb)) 
		skb->skb_recycling_callback = skbmgr_sg_recycling_callback;
	return skb;
}

EXPORT_SYMBOL(skbmgr_alloc_skb128);

int skbmgr_sg_recycling_callback(struct sk_buff *skb)
{
	struct sk_buff_head *list = &skbmgr_sg_pool[smp_processor_id()].list;

	if (skb_queue_len(list) < skbmgr_hot_list_len) {
		unsigned long flags;

		if ((skb->truesize - sizeof(struct sk_buff) != SKBMGR_SG_RX_BUF_LEN) ||
			(skb_shinfo(skb)->nr_frags) ||
			(skb_shinfo(skb)->frag_list)) {
			return 0;
		}
		
#if defined(TCSUPPORT_HWNAT)		
		pktflow_free(skb);		
#endif
#ifdef TCSUPPORT_RA_HWNAT
		if (ra_sw_nat_hook_free)
			ra_sw_nat_hook_free(skb);
#endif

		if (skb_queue_len(list) > skbmgr_sg_max_list_len)
			skbmgr_sg_max_list_len = skb_queue_len(list) + 1;

		local_irq_save(flags);
		__skb_queue_head(list, skb);
		local_irq_restore(flags);

		return 1;
	}

	return 0;
}

EXPORT_SYMBOL(skbmgr_sg_recycling_callback);

#endif

void skbmgr_free_all_skbs(void)
{
	struct sk_buff_head *list;
	struct sk_buff *skb;
	int i;

#ifdef SKBMGR_SINGLE_QUEUE
	list = &skbmgr_pool[0].list;
	while ((skb = skb_dequeue(list)) != NULL) {
		skb->skb_recycling_callback = NULL;
		if (skb->skb_recycling_ind == SKBMGR_INDICATION)
			atomic_dec(&skbmgr_alloc_no);
		skb->skb_recycling_ind = 0;
		kfree_skbmem(skb);
	}
#else
	for (i=0; i<NR_CPUS; i++) {
		list = &skbmgr_pool[i].list;
		while ((skb = skb_dequeue(list)) != NULL) {
			skb->skb_recycling_callback = NULL;
			if (skb->skb_recycling_ind == SKBMGR_INDICATION)
				atomic_dec(&skbmgr_alloc_no);
			skb->skb_recycling_ind = 0;
			kfree_skbmem(skb);
		}
	}
#endif
#if defined(CONFIG_MIPS_TC3262)
	for (i=0; i<NR_CPUS; i++) {
		list = &skbmgr_sg_pool[i].list;
		while ((skb = skb_dequeue(list)) != NULL) {
			skb->skb_recycling_callback = NULL;
			kfree_skbmem(skb);
		}
	}
#endif
}

#ifdef CONFIG_PROC_FS

static int hot_list_len_read(char *page, char **start, off_t offset,
			    int count, int *eof, void *data)
{
	char *out = page;
	int len;

	out += sprintf(out, "skbmgr_hot_list_len %d skbmgr_4k_hot_list_len %d\n", skbmgr_hot_list_len,skbmgr_4k_hot_list_len);

	len = out - page;
	len -= offset;
	if (len < count) {
		*eof = 1;
		if (len <= 0)
			return 0;
	} else
		len = count;

	*start = page + offset;
	return len;
}

static int hot_list_len_write(struct file *file, const char __user * buffer,
			     unsigned long count, void *data)
{
	char buf[64];
	int val;

	if (count > 64)
		return -EINVAL;

	if (copy_from_user(buf, buffer, count))
		return -EFAULT;

	val = simple_strtoul(buf, NULL, 10);

	skbmgr_hot_list_len = val;
	if (skbmgr_hot_list_len == 0) {
		skbmgr_free_all_skbs();
		skbmgr_max_list_len = 0;
#if defined(CONFIG_MIPS_TC3262)
		skbmgr_sg_max_list_len = 0;
#endif
	}

	return count;
}

#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
/*add proc function,user can change max_skb_num value */
static int driver_max_skb_read(char *page, char **start, off_t offset,
		int count, int *eof, void *data)
{
	char *out = page;
	int len;

	out += sprintf(out, "%d (%d,%d)\n", g_max_skb_num, atomic_read(&g_used_skb_num), peak_skb_num);

	len = out - page;
	len -= offset;
	if (len < count) {
		*eof = 1;
		if (len <= 0)
			return 0;
	} else
		len = count;

	*start = page + offset;
	return len;
}

static int driver_max_skb_write(struct file *file, const char __user * buffer,
			unsigned long count, void *data)
{
	char buf[64];
	int val;

	if (count > 64)
		return -EINVAL;

	if (copy_from_user(buf, buffer, count))
		return -EFAULT;

	val = simple_strtoul(buf, NULL, 10);

	g_max_skb_num = val;

	return count;
}
/*add end*/
#endif

static int limit_4k_read(char *page, char **start, off_t offset,
			int count, int *eof, void *data)
{
	char *out = page;
	int len;

	out += sprintf(out, "%d\n", skbmgr_4k_limit);

	len = out - page;
	len -= offset;
	if (len < count) {
		*eof = 1;
		if (len <= 0)
			return 0;
	} else
		len = count;

	*start = page + offset;
	return len;
}

static int limit_4k_write(struct file *file, const char __user * buffer,
			     unsigned long count, void *data)
{
	char buf[64];
	int val;

	if (count > 64)
		return -EINVAL;

	if (copy_from_user(buf, buffer, count))
		return -EFAULT;

	val = simple_strtoul(buf, NULL, 10);

	skbmgr_4k_limit = val;
	return count;
}

static int limit_read(char *page, char **start, off_t offset,
			int count, int *eof, void *data)
{
	char *out = page;
	int len;

	out += sprintf(out, "%d\n", skbmgr_limit);

	len = out - page;
	len -= offset;
	if (len < count) {
		*eof = 1;
		if (len <= 0)
			return 0;
	} else
		len = count;

	*start = page + offset;
	return len;
}

static int limit_write(struct file *file, const char __user * buffer,
			     unsigned long count, void *data)
{
	char buf[64];
	int val;

	if (count > 64)
		return -EINVAL;

	if (copy_from_user(buf, buffer, count))
		return -EFAULT;

	val = simple_strtoul(buf, NULL, 10);

	skbmgr_limit = val;
	if (skbmgr_hot_list_len == 0) {
		skbmgr_free_all_skbs();
		skbmgr_max_list_len = 0;
#if defined(CONFIG_MIPS_TC3262)
		skbmgr_sg_max_list_len = 0;
#endif
	}

	return count;
}

static int skbmgr_info_read(char *page, char **start, off_t offset,
			    int count, int *eof, void *data)
{
	char *out = page;
	int len;
	struct sk_buff_head *list;
	int i;

	out += sprintf(out, "skbmgr_limit = %d\n", skbmgr_limit);
	out += sprintf(out, "skbmgr_max_alloc_no = %d\n", skbmgr_max_alloc_no);
	out += sprintf(out, "skbmgr_alloc_fail = %d\n", skbmgr_alloc_fail);
	out += sprintf(out, "skbmgr_alloc_no = %d\n", atomic_read(&skbmgr_alloc_no));
	out += sprintf(out, "skbmgr_max_list_len = %d\n", skbmgr_max_list_len);
	out += sprintf(out, "skbmgr_4k_limit = %d\n", skbmgr_4k_limit);
	out += sprintf(out, "skbmgr_4k_max_alloc_no = %d\n", skbmgr_4k_max_alloc_no);
	out += sprintf(out, "skbmgr_4k_alloc_fail = %d\n", skbmgr_4k_alloc_fail);
	out += sprintf(out, "skbmgr_4k_alloc_no = %d\n", atomic_read(&skbmgr_4k_alloc_no));
	out += sprintf(out, "skbmgr_4k_max_list_len = %d\n", skbmgr_4k_max_list_len);
#if defined(CONFIG_MIPS_TC3262)
	out += sprintf(out, "skbmgr_sg_max_list_len = %d\n", skbmgr_sg_max_list_len);
#endif

#ifdef SKBMGR_SINGLE_QUEUE
	list = &skbmgr_pool[0].list;
	out += sprintf(out, "skbmgr_queue_len CPU0 = %d\n", skb_queue_len(list));
#else
	for (i=0; i<NR_CPUS; i++) {
		list = &skbmgr_pool[i].list;
		out += sprintf(out, "skbmgr_queue_len CPU%d = %d\n", i, skb_queue_len(list));
	}	
#endif
#ifdef SKBMGR_SINGLE_QUEUE
	list = &skbmgr_4k_pool[0].list;
	out += sprintf(out, "skbmgr_4k_queue_len CPU0 = %d\n", skb_queue_len(list));
#else
	for (i=0; i<NR_CPUS; i++) {
		list = &skbmgr_4k_pool[i].list;
		out += sprintf(out, "skbmgr_4k_queue_len CPU%d = %d\n", i, skb_queue_len(list));
	}	
#endif

#if defined(CONFIG_MIPS_TC3262)
	for (i=0; i<NR_CPUS; i++) {
		list = &skbmgr_sg_pool[i].list;
		out += sprintf(out, "skbmgr_sg_queue_len CPU%d = %d\n", i, skb_queue_len(list));
	}	
#endif

	len = out - page;
	len -= offset;
	if (len < count) {
		*eof = 1;
		if (len <= 0)
			return 0;
	} else
		len = count;

	*start = page + offset;
	return len;
}

#if defined(TCSUPPORT_MEMORY_CONTROL) || defined(TCSUPPORT_CT)
int auto_clear_cache_flag = 0;
int auto_kill_process_flag = 0;
static int auto_clear_cache_read(char *page, char **start, off_t offset,
			    int count, int *eof, void *data)
{
	char *out = page;
	int len;

	out += sprintf(out, "%d\n", auto_clear_cache_flag);

	len = out - page;
	len -= offset;
	if (len < count) {
		*eof = 1;
		if (len <= 0)
			return 0;
	} else
		len = count;

	*start = page + offset;
	return len;
}

static int auto_clear_cache_write(struct file *file, const char __user * buffer,
			     unsigned long count, void *data)
{
	char buf[64];
	int val;

	if (count > 64)
		return -EINVAL;

	if (copy_from_user(buf, buffer, count))
		return -EFAULT;

	val = simple_strtoul(buf, NULL, 10);

	auto_clear_cache_flag = val;

	return count;
}

static int auto_kill_process_read(char *page, char **start, off_t offset,
			    int count, int *eof, void *data)
{
	char *out = page;
	int len;

	out += sprintf(out, "%d\n", auto_kill_process_flag);

	len = out - page;
	len -= offset;
	if (len < count) {
		*eof = 1;
		if (len <= 0)
			return 0;
	} else
		len = count;

	*start = page + offset;
	return len;
}

static int auto_kill_process_write(struct file *file, const char __user * buffer,
			     unsigned long count, void *data)
{
	char buf[64];
	int val;

	if (count > 64)
		return -EINVAL;

	if (copy_from_user(buf, buffer, count))
		return -EFAULT;

	val = simple_strtoul(buf, NULL, 10);

	auto_kill_process_flag = val;

	return count;
}
#endif

static int register_proc_skbmgr(void)
{
	struct proc_dir_entry *p;

	p = create_proc_entry("skbmgr_hot_list_len", 0644, proc_net);
	if (!p) 
		return 0;

	p->owner = THIS_MODULE;
	p->read_proc = hot_list_len_read;
	p->write_proc = hot_list_len_write;

	p = create_proc_read_entry("skbmgr_info", 0, proc_net, skbmgr_info_read, NULL);
	if (!p) 
		return 0;

	p = create_proc_entry("skbmgr_limit", 0644, proc_net);
	if (!p) 
		return 0;

	p->owner = THIS_MODULE;
	p->read_proc = limit_read;
	p->write_proc = limit_write;

	p = create_proc_entry("skbmgr_4k_limit", 0644, proc_net);
	if (!p) 
		return 0;

	p->owner = THIS_MODULE;
	p->read_proc = limit_4k_read;
	p->write_proc = limit_4k_write;

#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)	
	p = create_proc_entry("skbmgr_driver_max_skb", 0644, proc_net);
	if (!p) 
		return 0;

	p->owner = THIS_MODULE;
	p->read_proc = driver_max_skb_read;
	p->write_proc = driver_max_skb_write;
#endif

#if defined(TCSUPPORT_MEMORY_CONTROL) || defined(TCSUPPORT_CT)
	p = create_proc_entry("auto_clear_cache", 0644, proc_net);
	if (!p) 
		return 0;

	p->read_proc = auto_clear_cache_read;
	p->write_proc = auto_clear_cache_write;

	p = create_proc_entry("auto_kill_process", 0644, proc_net);
	if (!p) 
		return 0;

	p->read_proc = auto_kill_process_read;
	p->write_proc = auto_kill_process_write;
#endif
	return 1;
}

static void unregister_proc_skbmgr(void)
{
	remove_proc_entry("skbmgr_hot_list_len", proc_net);
	remove_proc_entry("skbmgr_info", proc_net);
	remove_proc_entry("skbmgr_limit", proc_net);
#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)	
	remove_proc_entry("skbmgr_driver_max_skb", proc_net);
#endif
#if defined(TCSUPPORT_MEMORY_CONTROL) || defined(TCSUPPORT_CT)
	remove_proc_entry("auto_clear_cache", proc_net);
	remove_proc_entry("auto_kill_process", proc_net);
#endif
}

#endif

#endif

void __init skb_init(void)
{
#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
	int i;

	atomic_set(&skbmgr_alloc_no, 0);
#if defined(CONFIG_CPU_TC3162) || defined(CONFIG_MIPS_TC3262)
	atomic_set(&g_used_skb_num, 0);
#endif
#ifdef SKBMGR_SINGLE_QUEUE
	skb_queue_head_init(&skbmgr_pool[0].list);
#else
	for (i=0; i<NR_CPUS; i++) {
		skb_queue_head_init(&skbmgr_pool[i].list);
	}	
#endif	
#ifdef SKBMGR_SINGLE_QUEUE
	skb_queue_head_init(&skbmgr_4k_pool[0].list);
#else
	for (i=0; i<NR_CPUS; i++) {
		skb_queue_head_init(&skbmgr_4k_pool[i].list);
	}	
#endif	
	
#if defined(CONFIG_MIPS_TC3262)
	for (i=0; i<NR_CPUS; i++) {
		skb_queue_head_init(&skbmgr_sg_pool[i].list);
	}
#endif

#ifdef CONFIG_PROC_FS
	register_proc_skbmgr();
#endif
#endif

	skbuff_head_cache = kmem_cache_create("skbuff_head_cache",
					      sizeof(struct sk_buff),
					      0,
					      SLAB_HWCACHE_ALIGN|SLAB_PANIC,
					      NULL, NULL);
	skbuff_fclone_cache = kmem_cache_create("skbuff_fclone_cache",
						(2*sizeof(struct sk_buff)) +
						sizeof(atomic_t),
						0,
						SLAB_HWCACHE_ALIGN|SLAB_PANIC,
						NULL, NULL);
}

/**
 *	skb_to_sgvec - Fill a scatter-gather list from a socket buffer
 *	@skb: Socket buffer containing the buffers to be mapped
 *	@sg: The scatter-gather list to map into
 *	@offset: The offset into the buffer's contents to start mapping
 *	@len: Length of buffer space to be mapped
 *
 *	Fill the specified scatter-gather list with mappings/pointers into a
 *	region of the buffer space attached to a socket buffer.
 */
int
skb_to_sgvec(struct sk_buff *skb, struct scatterlist *sg, int offset, int len)
{
	int start = skb_headlen(skb);
	int i, copy = start - offset;
	int elt = 0;

	if (copy > 0) {
		if (copy > len)
			copy = len;
		sg[elt].page = virt_to_page(skb->data + offset);
		sg[elt].offset = (unsigned long)(skb->data + offset) % PAGE_SIZE;
		sg[elt].length = copy;
		elt++;
		if ((len -= copy) == 0)
			return elt;
		offset += copy;
	}

	for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
		int end;

		BUG_TRAP(start <= offset + len);

		end = start + skb_shinfo(skb)->frags[i].size;
		if ((copy = end - offset) > 0) {
			skb_frag_t *frag = &skb_shinfo(skb)->frags[i];

			if (copy > len)
				copy = len;
			sg[elt].page = frag->page;
			sg[elt].offset = frag->page_offset+offset-start;
			sg[elt].length = copy;
			elt++;
			if (!(len -= copy))
				return elt;
			offset += copy;
		}
		start = end;
	}

	if (skb_shinfo(skb)->frag_list) {
		struct sk_buff *list = skb_shinfo(skb)->frag_list;

		for (; list; list = list->next) {
			int end;

			BUG_TRAP(start <= offset + len);

			end = start + list->len;
			if ((copy = end - offset) > 0) {
				if (copy > len)
					copy = len;
				elt += skb_to_sgvec(list, sg+elt, offset - start, copy);
				if ((len -= copy) == 0)
					return elt;
				offset += copy;
			}
			start = end;
		}
	}
	BUG_ON(len);
	return elt;
}

/**
 *	skb_cow_data - Check that a socket buffer's data buffers are writable
 *	@skb: The socket buffer to check.
 *	@tailbits: Amount of trailing space to be added
 *	@trailer: Returned pointer to the skb where the @tailbits space begins
 *
 *	Make sure that the data buffers attached to a socket buffer are
 *	writable. If they are not, private copies are made of the data buffers
 *	and the socket buffer is set to use these instead.
 *
 *	If @tailbits is given, make sure that there is space to write @tailbits
 *	bytes of data beyond current end of socket buffer.  @trailer will be
 *	set to point to the skb in which this space begins.
 *
 *	The number of scatterlist elements required to completely map the
 *	COW'd and extended socket buffer will be returned.
 */
int skb_cow_data(struct sk_buff *skb, int tailbits, struct sk_buff **trailer)
{
	int copyflag;
	int elt;
	struct sk_buff *skb1, **skb_p;

	/* If skb is cloned or its head is paged, reallocate
	 * head pulling out all the pages (pages are considered not writable
	 * at the moment even if they are anonymous).
	 */
	if ((skb_cloned(skb) || skb_shinfo(skb)->nr_frags) &&
	    __pskb_pull_tail(skb, skb_pagelen(skb)-skb_headlen(skb)) == NULL)
		return -ENOMEM;

	/* Easy case. Most of packets will go this way. */
	if (!skb_shinfo(skb)->frag_list) {
		/* A little of trouble, not enough of space for trailer.
		 * This should not happen, when stack is tuned to generate
		 * good frames. OK, on miss we reallocate and reserve even more
		 * space, 128 bytes is fair. */

		if (skb_tailroom(skb) < tailbits &&
		    pskb_expand_head(skb, 0, tailbits-skb_tailroom(skb)+128, GFP_ATOMIC))
			return -ENOMEM;

		/* Voila! */
		*trailer = skb;
		return 1;
	}

	/* Misery. We are in troubles, going to mincer fragments... */

	elt = 1;
	skb_p = &skb_shinfo(skb)->frag_list;
	copyflag = 0;

	while ((skb1 = *skb_p) != NULL) {
		int ntail = 0;

		/* The fragment is partially pulled by someone,
		 * this can happen on input. Copy it and everything
		 * after it. */

		if (skb_shared(skb1))
			copyflag = 1;

		/* If the skb is the last, worry about trailer. */

		if (skb1->next == NULL && tailbits) {
			if (skb_shinfo(skb1)->nr_frags ||
			    skb_shinfo(skb1)->frag_list ||
			    skb_tailroom(skb1) < tailbits)
				ntail = tailbits + 128;
		}

		if (copyflag ||
		    skb_cloned(skb1) ||
		    ntail ||
		    skb_shinfo(skb1)->nr_frags ||
		    skb_shinfo(skb1)->frag_list) {
			struct sk_buff *skb2;

			/* Fuck, we are miserable poor guys... */
			if (ntail == 0)
				skb2 = skb_copy(skb1, GFP_ATOMIC);
			else
				skb2 = skb_copy_expand(skb1,
						       skb_headroom(skb1),
						       ntail,
						       GFP_ATOMIC);
			if (unlikely(skb2 == NULL))
				return -ENOMEM;

			if (skb1->sk)
				skb_set_owner_w(skb2, skb1->sk);

			/* Looking around. Are we still alive?
			 * OK, link new skb, drop old one */

			skb2->next = skb1->next;
			*skb_p = skb2;
			kfree_skb(skb1);
			skb1 = skb2;
		}
		elt++;
		*trailer = skb1;
		skb_p = &skb1->next;
	}

	return elt;
}

EXPORT_SYMBOL(___pskb_trim);
EXPORT_SYMBOL(__kfree_skb);
EXPORT_SYMBOL(kfree_skb);
EXPORT_SYMBOL(__pskb_pull_tail);
EXPORT_SYMBOL(__alloc_skb);
EXPORT_SYMBOL(__netdev_alloc_skb);
EXPORT_SYMBOL(pskb_copy);
EXPORT_SYMBOL(pskb_expand_head);
EXPORT_SYMBOL(skb_checksum);
EXPORT_SYMBOL(skb_clone);
EXPORT_SYMBOL(skb_copy);
EXPORT_SYMBOL(skb_copy_and_csum_bits);
EXPORT_SYMBOL(skb_copy_and_csum_dev);
EXPORT_SYMBOL(skb_copy_bits);
EXPORT_SYMBOL(skb_copy_expand);
EXPORT_SYMBOL(skb_over_panic);
EXPORT_SYMBOL(skb_pad);
EXPORT_SYMBOL(skb_realloc_headroom);
EXPORT_SYMBOL(skb_under_panic);
EXPORT_SYMBOL(skb_dequeue);
EXPORT_SYMBOL(skb_dequeue_tail);
EXPORT_SYMBOL(skb_insert);
EXPORT_SYMBOL(skb_queue_purge);
EXPORT_SYMBOL(skb_queue_head);
EXPORT_SYMBOL(skb_queue_tail);
EXPORT_SYMBOL(skb_unlink);
EXPORT_SYMBOL(skb_append);
EXPORT_SYMBOL(skb_split);
EXPORT_SYMBOL(skb_prepare_seq_read);
EXPORT_SYMBOL(skb_seq_read);
EXPORT_SYMBOL(skb_abort_seq_read);
EXPORT_SYMBOL(skb_find_text);
EXPORT_SYMBOL(skb_append_datato_frags);

EXPORT_SYMBOL_GPL(skb_to_sgvec);
EXPORT_SYMBOL_GPL(skb_cow_data);
