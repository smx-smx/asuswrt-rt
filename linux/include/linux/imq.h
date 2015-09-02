#ifndef _IMQ_H
#define _IMQ_H

#define IMQ_MAX_DEVS   16

//#define IMQ_F_IFMASK   0x7f
#define IMQ_F_IFMASK   0x0f
#define IMQ_F_ENQUEUE  0x80
#define QOS_DEFAULT_MARK 0x8
/* no queue marked packets to default queue */
#define QOS_FILTER_MARK 0xf0
#define QOS_WANIF_MARK	0xff000
#define QOS_DOT1P_MARK	0xf00
#define QOS_DSCP_MARK  0x3f00000
#define QOS_PRIORITY_DEFAULT 0x80
extern int imq_nf_queue(struct sk_buff *skb, struct nf_info *info, void *data); 

#endif /* _IMQ_H */
