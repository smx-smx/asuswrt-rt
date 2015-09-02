/*
 ***************************************************************************
 * Ralink Tech Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2006, Ralink Technology, Inc.
 *
 * All rights reserved. Ralink's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of Ralink Tech. Any attempt
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of Ralink Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    
    hook.c

    Abstract:

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
    Name        Date            Modification logs
    Steven Liu  2006-10-06      Initial version
*/

#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/skbuff.h>

int (*ra_sw_nat_hook_rx) (struct sk_buff * skb) = NULL;
int (*ra_sw_nat_hook_tx) (struct sk_buff * skb, int gmac_no) = NULL;
int (*ra_sw_nat_hook_free) (struct sk_buff * skb) = NULL;
int (*ra_sw_nat_hook_rxinfo) (struct sk_buff * skb, int magic, char *data, int data_length) = NULL;
int (*ra_sw_nat_hook_txq) (struct sk_buff * skb, int txq) = NULL;
int (*ra_sw_nat_hook_magic) (struct sk_buff * skb, int magic) = NULL;
int (*ra_sw_nat_hook_set_magic) (struct sk_buff * skb, int magic) = NULL;
int (*ra_sw_nat_hook_xfer) (struct sk_buff *skb, const struct sk_buff *prev_p) = NULL;

EXPORT_SYMBOL(ra_sw_nat_hook_rx);
EXPORT_SYMBOL(ra_sw_nat_hook_tx);
EXPORT_SYMBOL(ra_sw_nat_hook_free);
EXPORT_SYMBOL(ra_sw_nat_hook_rxinfo);
EXPORT_SYMBOL(ra_sw_nat_hook_txq);
EXPORT_SYMBOL(ra_sw_nat_hook_magic);
EXPORT_SYMBOL(ra_sw_nat_hook_set_magic);
EXPORT_SYMBOL(ra_sw_nat_hook_xfer);

