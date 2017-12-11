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

#ifndef __FW_UTIL_H__
#define __FW_UTIL_H__

#define IP6_IPPROTO_HOPOPTS    0x00 /* IPv6 hop-by-hop options */
#define IP6_IPPROTO_ICMP          1 /* ICMP */
#define IP6_IPPROTO_IGMP          2 /* IGMP */
#define IP6_IPPROTO_IPINIP        4 /* IP in/within IP */
#define IP6_IPPROTO_TCP           6 /* TCP */
#define IP6_IPPROTO_UDP          17 /* UDP */
#define IP6_IPPROTO_RDP          27 /* RDP */
#define IP6_IPPROTO_IP6          41 /* IPv6 encapsulation */
#define IP6_IPPROTO_IP6_ROUTING  43 /* IPv6 routing header */
#define IP6_IPPROTO_IP6_FRAGMENT 44 /* IPv6 fragmentation header */
#define IP6_IPPROTO_GRE          47 /* GRE */
#define IP6_IPPROTO_ESP          50 /* ESP */
#define IP6_IPPROTO_AH           51 /* AH */
#define IP6_IPPROTO_IP6_ICMP6    58 /* ICMPv6 */
#define IP6_IPPROTO_IP6_NONXT    59 /* IPv6 no next header */
#define IP6_IPPROTO_IP6_DSTOPTS  60 /* IPv6 destination options */
#define IP6_IPPROTO_SCTP        132 /* SCTP */
#define IP6_IPPROTO_MH          135 /* IPv6 mobility header */

typedef unsigned int socklen_t;

int parse_skb(struct sk_buff *skb);
int inet_pton(int af, const char *src, void *dst);
const char* inet_ntop(int af, const void */*restrict*/ src
	, char */*restrict*/ dst, socklen_t cnt);
unsigned short ip_sum(unsigned short *buf, int len);
bool is_skb_upload(struct sk_buff *skb, const struct net_device *fw_outdev);
void dump_skb(struct sk_buff *skb);

#define IP_FLAG_UF 0x04
#define IP_FLAG_DF 0x02
#define IP_FLAG_MF 0x01
#define IP_OFFMASK 0x1fff

#define IP4_EQUAL(_left, _right) (*((uint32_t *) _left) == *((uint32_t *) _right))
#define IP4_COPY(_dst, _src) (*((uint32_t *) _dst) = *((uint32_t *) _src))
#define IP4_MATCH(_left, _right, _match_len) (memcmp(_left, _right, _match_len) == 0)

#define IP6_EQUAL(_left, _right) (memcmp(_left, _right, 16) == 0)
#define IP6_COPY(_dst, _src) do { memcpy(_dst, _src, 16); } while (0)
#define IP6_MATCH(_left, _right, _match_len) (memcmp(_left, _right, _match_len) == 0)

#define IP_EQUAL(__left, __right, __ip_ver) \
	((__ip_ver == 4) ? IP4_EQUAL(__left, __right) : IP6_EQUAL(__left, __right))

#define IP_COPY(__dst, __src, __ip_ver) \
	do { \
		if (likely(__ip_ver == 4)) \
			IP4_COPY(__dst, __src); \
		else \
			IP6_COPY(__dst, __src); \
	} while(0)

/* memory statistics */
extern atomic_t ford_mem_sta;
extern atomic_t ford_mem_dyn;

#define ADD_STA_MEM(__n)	atomic_add(__n, &ford_mem_sta)
#define SUB_STA_MEM(__n)	atomic_sub(__n, &ford_mem_sta)
#define ADD_DYN_MEM(__n)	atomic_add(__n, &ford_mem_dyn)
#define SUB_DYN_MEM(__n)	atomic_sub(__n, &ford_mem_dyn)

#define KMALLOC_INIT(_size, _flags) 	\
({ 									\
	void *ptr;						\
	if ((ptr = kmalloc(_size, _flags)))	\
	{ 							\
		memset(ptr, 0x00, _size);			\
		ADD_DYN_MEM(_size);			\
		DBG("%s kmalloc %u, ford_mem_sta = %d\n",			\
			__FUNCTION__, _size, atomic_read(&ford_mem_dyn));	\
	}								\
	ptr;							\
})

#define VMALLOC_INIT(_size) 	\
({ 									\
	void *ptr;						\
	if ((ptr = vmalloc(_size)))		\
	{ 							\
		memset(ptr, 0x00, _size);			\
		ADD_STA_MEM(_size);			\
		DBG("%s vmalloc %u, ford_mem_sta = %d\n",			\
			__FUNCTION__, _size, atomic_read(&ford_mem_sta));	\
	}								\
	ptr;							\
})

#define KFREE_INIT(_p, _size) \
do { 				\
	if (likely(_p)) \
	{ 				\
		kfree(_p); 	\
		_p = NULL;	\
		SUB_DYN_MEM(_size);	\
		DBG("%s kfree %u, ford_mem_sta = %d\n",			\
			__FUNCTION__, _size, atomic_read(&ford_mem_dyn));	\
	} 				\
} while (0)

#define VFREE_INIT(_p, _size)	\
do { 					\
	if (likely(_p)) 	\
	{ 					\
		vfree(_p);		\
		_p = NULL; 		\
		SUB_STA_MEM(_size);	\
		DBG("%s vfree %u, ford_mem_sta = %d\n",			\
			__FUNCTION__, _size, atomic_read(&ford_mem_sta));	\
	} 					\
} while (0)

/*!
 * \brief Inner Function, count the entry in list.
 * \param head [in] The list head.
 * \return Number, The number of items.
 */
static inline uint32_t list_count(struct list_head *head)
{
	uint32_t cnt = 0;
	struct list_head *entry;

	if (!list_empty(head))
	{
		list_for_each(entry, head)
		{
			cnt++;
		}
	}
	return cnt;
}

#endif
