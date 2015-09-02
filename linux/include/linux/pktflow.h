#ifndef __PKTFLOW_H
#define __PKTFLOW_H

#include <linux/types.h>

#define PKTFLOW_ENCAP_MAX              6       /* Maximum number of L2 encaps */
#define PKTFLOW_HDRSZ_MAX              32      /* Maximum size of L2 encaps */

#define PKTFLOW_EXTEND_SIZE            32      /* Number of Pktflow_t per extension */
#define PKTFLOW_EXTEND_MAX             32      /* Maximum extensions allowed */
#define PKTFLOW_HP_EXTEND_MAX		   32       /* Maximum extensions allowed */

#define PKTFLOW_HP					   (1<<15) 

/* Forward declarations */
struct sk_buff;
struct net_device;
struct nf_conn;

struct pktflow_stats {
	unsigned long	rx_packets;		/* total packets received	*/
	unsigned long	tx_packets;		/* total packets transmitted	*/
	unsigned long	rx_bytes;		/* total bytes received 	*/
	unsigned long	tx_bytes;		/* total bytes transmitted	*/
	unsigned long	rx_dropped;		/* no space in linux buffers	*/
	unsigned long	tx_dropped;		/* no space available in linux	*/

	unsigned long	tx_packets_q0;	/* packets transmitted to q0	*/
	unsigned long	tx_packets_q1;	/* packets transmitted to q1	*/
	unsigned long	tx_packets_q2;	/* packets transmitted to q2	*/
	unsigned long	tx_packets_q3;	/* packets transmitted to q3	*/
};

/*
 *------------------------------------------------------------------------------
 * Layer 2 encapsulations logged.
 * Implementation constraint: max 8 proto types.
 *------------------------------------------------------------------------------
 */
typedef enum {
	ETH_802x,
   	TC2206_STAG,
   	OUT_VLAN_8021Q,
   	VLAN_8021Q,
   	PPPoE_2516,
	ETH_IPV4,
	ETH_IPV6,
   	PROTO_MAX
} PktflowEncap_t;

typedef struct ip6_addr {
	union 
	{
		__u8		u6_addr8[16];
		__be16		u6_addr16[8];
		__be32		u6_addr32[4];
	} in6_u;
} ip6_addr_t;

typedef struct {
    uint8_t             itf;        
    uint8_t             count;          /* Number of L2 encapsulations */
    uint8_t             multicast;        
    union {
        struct {
            uint8_t   	ETH_IPV6  		: 1;
            uint8_t 	ETH_IPV4  		: 1;
            uint8_t    	PPPoE_2516  	: 1;
            uint8_t  	VLAN_8021Q  	: 1;    
            uint8_t    	OUT_VLAN_8021Q  : 1;    
            uint8_t    	TC2206_STAG 	: 1;
            uint8_t    	ETH_802x    	: 1;
        }               bmap;
        uint8_t         hdrs;
    };
} __attribute__((packed)) PktflowInfo_t;

/*
 *------------------------------------------------------------------------------
 * Buffer to log IP Tuple.
 * Packed: 1 16byte cacheline.
 *------------------------------------------------------------------------------
 */
struct pktflowTuple_t {
	union {
		struct {
			uint32_t        saddr;          /* IP header saddr */
			uint32_t        daddr;          /* IP header daddr */

			union {
				struct {
					uint16_t    source;     /* L4 source port */
					uint16_t    dest;       /* L4 dest port */
				}           port;
				uint32_t    ports;
			};
		} ip4_u;

		struct {
			ip6_addr_t 		daddr;          /* IP header daddr */
		} ip6_u;
	};
} __attribute__((packed)) ____cacheline_aligned;
typedef struct pktflowTuple_t PktflowTuple_t;

/*
 *------------------------------------------------------------------------------
 * Buffer to log Layer 2 and IP Tuple headers.
 * Packed: 4 16byte cachelines
 *------------------------------------------------------------------------------
 */
struct pktflowHeader_t {

    PktflowTuple_t    	tuple;          /* L3+L4 IP Tuple log */

    union {
        struct net_device *dev_p;
        struct nf_conn    *ct_p;
    };

    union {
    	PktflowInfo_t  	info;
	    uint32_t        word;           /* channel, count, rfc2684, bmap */
    };

    union {
        uint8_t         reserved;
        uint8_t         nf_dir;
    };
    uint8_t             length;         /* L2 header total length */
    PktflowEncap_t   	encap[ PKTFLOW_ENCAP_MAX ];    /* All L2 header types */

    uint8_t             l2hdr[ PKTFLOW_HDRSZ_MAX ];    /* Data of all L2 headers */

} __attribute__((packed)) ____cacheline_aligned;

typedef struct pktflowHeader_t PktflowHeader_t;           /* L2 and L3+4 tuple */

/*
 *------------------------------------------------------------------------------
 * Buffer log structure.
 * Packed: 9 16 byte cachelines, 38bytes per pktflow.
 *------------------------------------------------------------------------------
 */
struct pktflow_t {
	struct list_head  	list;         	/* hashed list heads */

    union {
        void            *void_p;
        struct pktflow_t   *pktflow_p;       /* Free list of Pktflow_t */
        struct sk_buff  *skb;        /* Associated sk_buff */
    };

	uint16_t			flags;
	uint16_t			hit_count;

    uint8_t             hash;           /* Hash of Rx IP tuple */
    uint8_t             protocol;       /* IP protocol */
    uint16_t            nfct_events;    /* netfilter events */

	struct net_bridge_fdb_entry *fdb_src;

    PktflowHeader_t    	rx;             /* Receive path headers */
    PktflowHeader_t    	tx;             /* Transmit path headers */

	/* counter and timer */
	atomic_t				refcnt;		/* reference count */
	struct timer_list		timer;		/* Expiration timer */
	volatile unsigned long	timeout;	/* timeout */
} __attribute__((packed)) ____cacheline_aligned;
typedef struct pktflow_t Pktflow_t;


extern int (*pktflow_rx_hook)(struct sk_buff *skb, int itf, unsigned int *dbg_info);
extern int (*pktflow_tx_hook)(struct sk_buff *skb, int itf);
extern void (*pktflow_nfct_close_hook)(struct nf_conn *nf);
extern void (*pktflow_fdb_delete_hook)(struct net_bridge_fdb_entry *f);
extern void (*pktflow_get_stats_hook)(struct pktflow_stats *stats, int itf);
extern void (*pktflow_clear_stats_hook)(int itf);
extern void (*pktflow_update_ipv4_mc_hook)(__be32 group, unsigned int portmap);
extern void (*pktflow_update_ip6_mc_hook)(struct in6_addr group, unsigned int portmap);

extern void pktflow_register(int (*pktflow_rx)(struct sk_buff *skb, int itf, unsigned int *dbg_info), 
						int (*pktflow_tx)(struct sk_buff *skb, int itf), 
						void (*pktflow_nfct_close)(struct nf_conn *nf),
						void (*pktflow_fdb_delete)(struct net_bridge_fdb_entry *f),
						void (*pktflow_get_stats)(struct pktflow_stats *stats, int itf),
						void (*pktflow_clear_stats)(int itf),
						void (*pktflow_update_ipv4_mc)(__be32 group, unsigned int portmap),
						void (*pktflow_update_ip6_mc)(struct in6_addr group, unsigned int portmap));
extern void pktflow_unregister(void);


extern Pktflow_t *pktflow_get(void);
extern Pktflow_t *pktflow_hp_get(void);
extern void pktflow_put(Pktflow_t *pktflow_p);
extern void pktflow_free(struct sk_buff *skb);
extern void pktflow_xfer(struct sk_buff *skb, const struct sk_buff *prev_p);
extern void pktflow_nfct(struct sk_buff *skb, struct nf_conn *nfct_p);
extern void pktflow_refresh_nfct(struct nf_conn *ct_p, uint32_t jiffies);

typedef void (*pktflow_refresh_t)(struct nf_conn *nfct, uint32_t ctinfo,
                               struct sk_buff *skb,
                               uint32_t jiffies, int do_acct);
extern pktflow_refresh_t pktflow_refresh_fn;

#endif 

