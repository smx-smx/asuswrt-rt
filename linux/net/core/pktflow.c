#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/slab.h>
#include <linux/pktflow.h>
#include <linux/if_ether.h>
#include <linux/if_pppox.h>
#include <linux/if_vlan.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <net/ip.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_helper.h>
#if defined(CONFIG_BRIDGE) || defined(CONFIG_BRIDGE_MODULE)
#include <linux/if_bridge.h>
#include "../net/bridge/br_private.h"
#endif

static DEFINE_SPINLOCK(pktflow_lock);

struct sk_buff *nfskb_p = NULL;

pktflow_refresh_t pktflow_refresh_fn = NULL;

int (*pktflow_rx_hook)(struct sk_buff *skb, int itf, unsigned int *dbg_info) = NULL;
int (*pktflow_tx_hook)(struct sk_buff *skb, int itf) = NULL;
void (*pktflow_nfct_close_hook)(struct nf_conn *nf) = NULL;
void (*pktflow_fdb_delete_hook)(struct net_bridge_fdb_entry *f) = NULL;
void (*pktflow_get_stats_hook)(struct pktflow_stats *stats, int itf) = NULL;
void (*pktflow_clear_stats_hook)(int itf) = NULL;
void (*pktflow_update_ipv4_mc_hook)(__be32 group, unsigned int portmap) = NULL;
void (*pktflow_update_ip6_mc_hook)(struct in6_addr group, unsigned int portmap) = NULL;

EXPORT_SYMBOL(pktflow_refresh_fn);
EXPORT_SYMBOL(pktflow_rx_hook);
EXPORT_SYMBOL(pktflow_tx_hook);
EXPORT_SYMBOL(pktflow_nfct_close_hook);
EXPORT_SYMBOL(pktflow_fdb_delete_hook);
EXPORT_SYMBOL(pktflow_get_stats_hook);
EXPORT_SYMBOL(pktflow_clear_stats_hook);
EXPORT_SYMBOL(pktflow_update_ipv4_mc_hook);
EXPORT_SYMBOL(pktflow_update_ip6_mc_hook);

unsigned int pktflow_debug = 0;
EXPORT_SYMBOL(pktflow_debug);

static Pktflow_t *pktflow_list_gp = NULL;
static Pktflow_t *pktflow_hp_list_gp = NULL;

static int pktflow_cnt_extends = 0;  
static int pktflow_cnt_free = 0;    
static int pktflow_cnt_used = 0;   
static int pktflow_cnt_hwm  = 0;  
static int pktflow_cnt_fails = 0;

static int pktflow_hp_cnt_extends = 0;  
static int pktflow_hp_cnt_free = 0;    
static int pktflow_hp_cnt_used = 0;   
static int pktflow_hp_cnt_hwm  = 0;  
static int pktflow_hp_cnt_fails = 0;

static uint32_t pktflow_extend(uint32_t num)
{
    int i;
    Pktflow_t *list_p;

    list_p = (Pktflow_t *) kmalloc(num * sizeof(Pktflow_t), GFP_ATOMIC);
    if (list_p == NULL) 
        return 0;

    pktflow_cnt_extends++;

    for (i = 0; i < num; i++)
        list_p[i].pktflow_p = &list_p[i+1];

    list_p[num-1].pktflow_p = pktflow_list_gp; 
    pktflow_list_gp = list_p;  
    pktflow_cnt_free += num; 

    return num;
}

static uint32_t pktflow_hp_extend(uint32_t num)
{
    int i;
    Pktflow_t *list_p;

    list_p = (Pktflow_t *) kmalloc(num * sizeof(Pktflow_t), GFP_ATOMIC);
    if (list_p == NULL) 
        return 0;

    pktflow_hp_cnt_extends++;

    for (i = 0; i < num; i++)
        list_p[i].pktflow_p = &list_p[i+1];

    list_p[num-1].pktflow_p = pktflow_hp_list_gp; 
    pktflow_hp_list_gp = list_p;  
    pktflow_hp_cnt_free += num; 

    return num;
}

static inline void pktflow_clr(Pktflow_t *pktflow_p)
{
    memset((void*)pktflow_p, 0, sizeof(Pktflow_t)); 

    /* clear rfc2684, count, bmap, and itf */
    pktflow_p->rx.word = 0;
    pktflow_p->tx.word = 0;

    pktflow_p->fdb_src = NULL;

    pktflow_p->nfct_events = 0;
    pktflow_p->rx.ct_p = (struct nf_conn*)NULL;
}

Pktflow_t *pktflow_get(void)
{
    Pktflow_t *pktflow_p;
    unsigned long flags;

    spin_lock_irqsave(&pktflow_lock, flags);
    if (pktflow_list_gp == NULL) {
        if ((pktflow_cnt_extends >= PKTFLOW_EXTEND_MAX)  
          		|| (pktflow_extend(PKTFLOW_EXTEND_SIZE) != PKTFLOW_EXTEND_SIZE)) {
			pktflow_cnt_fails++;
    		spin_unlock_irqrestore(&pktflow_lock, flags);
            return NULL;
        }
    }

	pktflow_cnt_free--;
   	pktflow_cnt_used++;
   	if (pktflow_cnt_used > pktflow_cnt_hwm)
   		pktflow_cnt_hwm = pktflow_cnt_used;

    pktflow_p = pktflow_list_gp;
    pktflow_list_gp = pktflow_list_gp->pktflow_p;
    pktflow_clr(pktflow_p);     
    spin_unlock_irqrestore(&pktflow_lock, flags);

    return pktflow_p;
}
EXPORT_SYMBOL(pktflow_get);

Pktflow_t *pktflow_hp_get(void)
{
    Pktflow_t *pktflow_p;
    unsigned long flags;

    spin_lock_irqsave(&pktflow_lock, flags);
    if (pktflow_hp_list_gp == NULL) {
        if ((pktflow_hp_cnt_extends >= PKTFLOW_HP_EXTEND_MAX)  
          		|| (pktflow_hp_extend(PKTFLOW_EXTEND_SIZE) != PKTFLOW_EXTEND_SIZE)) {
			pktflow_hp_cnt_fails++;
    		spin_unlock_irqrestore(&pktflow_lock, flags);
            return NULL;
        }
    }

	pktflow_hp_cnt_free--;
   	pktflow_hp_cnt_used++;
   	if (pktflow_hp_cnt_used > pktflow_hp_cnt_hwm)
   		pktflow_hp_cnt_hwm = pktflow_hp_cnt_used;

    pktflow_p = pktflow_hp_list_gp;
    pktflow_hp_list_gp = pktflow_hp_list_gp->pktflow_p;
    pktflow_clr(pktflow_p);     
	pktflow_p->flags |= PKTFLOW_HP;
    spin_unlock_irqrestore(&pktflow_lock, flags);

    return pktflow_p;
}
EXPORT_SYMBOL(pktflow_hp_get);

void pktflow_put(Pktflow_t *pktflow_p)
{
    unsigned long flags;

	if (pktflow_p->flags & PKTFLOW_HP) {
		spin_lock_irqsave(&pktflow_lock, flags);
		pktflow_hp_cnt_used--; 
		pktflow_hp_cnt_free++; 
		pktflow_clr(pktflow_p);
		pktflow_p->pktflow_p = pktflow_hp_list_gp;
		pktflow_hp_list_gp = pktflow_p;  
		spin_unlock_irqrestore(&pktflow_lock, flags);
	} else {
    spin_lock_irqsave(&pktflow_lock, flags);
    pktflow_cnt_used--; 
	pktflow_cnt_free++; 
    pktflow_clr(pktflow_p);
    pktflow_p->pktflow_p = pktflow_list_gp;
    pktflow_list_gp = pktflow_p;  
    spin_unlock_irqrestore(&pktflow_lock, flags);
}
}
EXPORT_SYMBOL(pktflow_put);

inline Pktflow_t *_pktflow_null(struct sk_buff *skb)
{
    Pktflow_t *pktflow_p;
	
    pktflow_p = skb->pktflow_p;
    skb->pktflow_p = NULL;
    return pktflow_p;
}

Pktflow_t *pktflow_null(struct sk_buff *skb)
{
    return _pktflow_null(skb);
}
EXPORT_SYMBOL(pktflow_null);

void pktflow_free(struct sk_buff *skb)
{
    Pktflow_t *pktflow_p;

    pktflow_p = _pktflow_null(skb);   
    if (likely(pktflow_p)) {
        pktflow_put(pktflow_p);    
		if (pktflow_debug & 0x2)
			dump_stack();
	}
}
EXPORT_SYMBOL(pktflow_free);

void pktflow_xfer(struct sk_buff *skb, const struct sk_buff *prev_p)
{
    Pktflow_t *pktflow_p;
    struct sk_buff *mod_prev_p;

    mod_prev_p = (struct sk_buff *) prev_p; 
    pktflow_p = _pktflow_null(mod_prev_p);
    skb->pktflow_p = pktflow_p;

    if (unlikely(pktflow_p)) {
        pktflow_p->skb = skb;
		if (pktflow_debug & 0x1)
			dump_stack();
	}
}
EXPORT_SYMBOL(pktflow_xfer);

void pktflow_nfct(struct sk_buff *skb, struct nf_conn *ct_p)
{
    enum ip_conntrack_info ctinfo;
    struct nf_conn *ct;
    
    if (unlikely(skb->pktflow_p == NULL))
        return;

    skb->pktflow_p->rx.ct_p = ct_p;                
    skb->pktflow_p->nfct_events = IPCT_PKTFLOW;     

    ct = nf_ct_get(skb, &ctinfo);
    skb->pktflow_p->rx.nf_dir = CTINFO2DIR(ctinfo);  
}
EXPORT_SYMBOL(pktflow_nfct);

void pktflow_fdb(struct sk_buff *skb, struct net_bridge_fdb_entry *fdb)
{
    if (unlikely(skb->pktflow_p == NULL))
        return;

    skb->pktflow_p->fdb_src = fdb;                
}
EXPORT_SYMBOL(pktflow_fdb);

void pktflow_refresh_nfct(struct nf_conn *ct_p, uint32_t jiffies)
{
    if (pktflow_refresh_fn && ct_p) {
        nfskb_p->nfct = (struct nf_conntrack *) ct_p;
        (*pktflow_refresh_fn)(ct_p, 0, nfskb_p, jiffies, 0);
    }
}
EXPORT_SYMBOL(pktflow_refresh_nfct);

void pktflow_register(int (*pktflow_rx)(struct sk_buff *skb, int itf, unsigned int *dbg_info), 
						int (*pktflow_tx)(struct sk_buff *skb, int itf), 
						void (*pktflow_nfct_close)(struct nf_conn *nf),
						void (*pktflow_fdb_delete)(struct net_bridge_fdb_entry *f),
						void (*pktflow_get_stats)(struct pktflow_stats *stats, int itf),
						void (*pktflow_clear_stats)(int itf),
						void (*pktflow_update_ipv4_mc)(__be32 group, unsigned int portmap),
						void (*pktflow_update_ip6_mc)(struct in6_addr group, unsigned int portmap))
{
    pktflow_rx_hook = pktflow_rx;   
    pktflow_tx_hook = pktflow_tx;  
    pktflow_nfct_close_hook = pktflow_nfct_close; 
    pktflow_fdb_delete_hook = pktflow_fdb_delete; 
    pktflow_get_stats_hook = pktflow_get_stats;  
    pktflow_clear_stats_hook = pktflow_clear_stats;  
    pktflow_update_ipv4_mc_hook = pktflow_update_ipv4_mc;  
    pktflow_update_ip6_mc_hook = pktflow_update_ip6_mc;  
}
EXPORT_SYMBOL(pktflow_register);

void pktflow_unregister(void)
{
    pktflow_rx_hook = NULL;   
    pktflow_tx_hook = NULL;  
    pktflow_nfct_close_hook = NULL; 
    pktflow_fdb_delete_hook = NULL; 
    pktflow_get_stats_hook = NULL;  
    pktflow_clear_stats_hook = NULL;  
    pktflow_update_ipv4_mc_hook = NULL;  
    pktflow_update_ip6_mc_hook = NULL;  
}
EXPORT_SYMBOL(pktflow_unregister);

static int pktflow_seq_show(struct seq_file *seq, void *v)
{
	seq_printf(seq, "extends = %d\n", pktflow_cnt_extends);
	seq_printf(seq, "free    = %d\n", pktflow_cnt_free);
	seq_printf(seq, "used    = %d\n", pktflow_cnt_used);
	seq_printf(seq, "hwm     = %d\n", pktflow_cnt_hwm);
	seq_printf(seq, "fails   = %d\n", pktflow_cnt_fails);
	seq_printf(seq, "hp extends = %d\n", pktflow_hp_cnt_extends);
	seq_printf(seq, "hp free    = %d\n", pktflow_hp_cnt_free);
	seq_printf(seq, "hp used    = %d\n", pktflow_hp_cnt_used);
	seq_printf(seq, "hp hwm     = %d\n", pktflow_hp_cnt_hwm);
	seq_printf(seq, "hp fails   = %d\n", pktflow_hp_cnt_fails);
	return 0;
}

static int pktflow_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, pktflow_seq_show, PDE(inode)->data);
}

static const struct file_operations pktflow_seq_fops = {
	.owner	 = THIS_MODULE,
	.open    = pktflow_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release,
};

static __init int register_proc_pktflow(void)
{
	int rc = -ENOMEM;

	if (!proc_net_fops_create("pktflow", S_IRUGO, &pktflow_seq_fops))
		goto out;

	rc = 0;
out:
	return rc;
}

static int __init pktflow_init(void)
{
    printk("pktflow 1.0 initialized\n");

    nfskb_p = alloc_skb(0, GFP_ATOMIC);

	register_proc_pktflow();

    return 0;
}

subsys_initcall(pktflow_init);

