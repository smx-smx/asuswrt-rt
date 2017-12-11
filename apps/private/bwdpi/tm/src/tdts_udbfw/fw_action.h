/*
 * Copyright 2014 Trend Micro Incorporated
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software without 
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 */

#ifndef __FW_ACTION_H__
#define __FW_ACTION_H__

#define IP_HLEN		20
#define IP6_HLEN	40
#define TCP_HLEN	20

/* Hop-by-Hop or Destination Option header */
typedef struct
{
	unsigned char nexthdr;
	unsigned char hdrlen;

	/* option dependent data */
	unsigned char opts[6];
} ipv6_opt_hdr_t;

typedef struct pseduohdr_type2 {
	unsigned int sip;
	unsigned int dip;
	unsigned char zero;
	unsigned char ip_pro;
	unsigned short len;
	unsigned char data[1000]; //--mit
} pseudohdr_t2;

typedef struct pseduohdr_v6_type2 {
	unsigned int sip[4];
	unsigned int dip[4];
	unsigned int len;
	unsigned char zero[3];
	unsigned char next_hdr;
	unsigned char data[1000]; //--mit
} pseudohdr_v6_t2;

void send_action_pkt(struct sk_buff *skb
		, struct sock *sk
		, void *send
		, char *wan_name
		, char *redir_url, int url_len
		, struct net_device *in_dev
		, struct net_device *out_dev);

int send_redir_page(void *skb_ptr, redir_param_t *redir_param, tdts_net_device_t *dev);
	
#endif
