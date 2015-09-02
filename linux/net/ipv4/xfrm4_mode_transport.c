/*
 * xfrm4_mode_transport.c - Transport mode encapsulation for IPv4.
 *
 * Copyright (c) 2004-2006 Herbert Xu <herbert@gondor.apana.org.au>
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/stringify.h>
#include <net/dst.h>
#include <net/ip.h>
#include <net/xfrm.h>
#ifdef MTK_CRYPTO_DRIVER
#include <net/esp.h>
#endif

/* Add encapsulation header.
 *
 * The IP header will be moved forward to make space for the encapsulation
 * header.
 *
 * On exit, skb->h will be set to the start of the payload to be processed
 * by x->type->output and skb->nh will be set to the top IP header.
 */
static int xfrm4_transport_output(struct xfrm_state *x, struct sk_buff *skb)
{
	struct iphdr *iph = ip_hdr(skb);
	int ihl = iph->ihl * 4;
#ifdef MTK_CRYPTO_DRIVER
	struct esp_data *esp;
	int header_len;
#endif

	skb->transport_header = skb->network_header + ihl;
#ifdef MTK_CRYPTO_DRIVER
/* 1. Don't need to take care esp header(8 bytes) and iv(8 bytes)
 * because EIP93 will cover it !!
 * 2. Don't change the value of x->props.header_len becaue it
 * will also change the values for dst_mtu and tcp mss, which
 * will cause ftp stress test failure.
 */
	esp = x->data;
	if (!esp)
	{
		printk("%s: esp is NULL\n", __FUNCTION__);
		return -EPERM;
	}
	header_len = (x->props.header_len) - (sizeof(struct ip_esp_hdr) + esp->conf.ivlen);
	if (header_len < 0)
	{
		printk("%s: Wrong value for header_len:%d\n", __FUNCTION__, header_len);
		return -EPERM;
	}

	skb_push(skb, header_len);
#else
	skb_push(skb, x->props.header_len);
#endif

	skb_reset_network_header(skb);
	memmove(skb_network_header(skb), iph, ihl);
	return 0;
}

/* Remove encapsulation header.
 *
 * The IP header will be moved over the top of the encapsulation header.
 *
 * On entry, skb->h shall point to where the IP header should be and skb->nh
 * shall be set to where the IP header currently is.  skb->data shall point
 * to the start of the payload.
 */
static int xfrm4_transport_input(struct xfrm_state *x, struct sk_buff *skb)
{
	int ihl = skb->data - skb_transport_header(skb);

	if (skb->transport_header != skb->network_header) {
		memmove(skb_transport_header(skb),
			skb_network_header(skb), ihl);
		skb->network_header = skb->transport_header;
	}
	ip_hdr(skb)->tot_len = htons(skb->len + ihl);
	skb_reset_transport_header(skb);
	return 0;
}

static struct xfrm_mode xfrm4_transport_mode = {
	.input = xfrm4_transport_input,
	.output = xfrm4_transport_output,
	.owner = THIS_MODULE,
	.encap = XFRM_MODE_TRANSPORT,
};

static int __init xfrm4_transport_init(void)
{
	return xfrm_register_mode(&xfrm4_transport_mode, AF_INET);
}

static void __exit xfrm4_transport_exit(void)
{
	int err;

	err = xfrm_unregister_mode(&xfrm4_transport_mode, AF_INET);
	BUG_ON(err);
}

module_init(xfrm4_transport_init);
module_exit(xfrm4_transport_exit);
MODULE_LICENSE("GPL");
MODULE_ALIAS_XFRM_MODE(AF_INET, XFRM_MODE_TRANSPORT);
