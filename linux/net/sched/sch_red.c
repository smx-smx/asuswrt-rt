/*
 * net/sched/sch_red.c	Random Early Detection queue.
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 * Authors:	Alexey Kuznetsov, <kuznet@ms2.inr.ac.ru>
 *
 * Changes:
 * J Hadi Salim 980914:	computation fixes
 * Alexey Makarenko <makar@phoenix.kharkov.ua> 990814: qave on idle link was calculated incorrectly.
 * J Hadi Salim 980816:  ECN support
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <net/pkt_sched.h>
#include <net/inet_ecn.h>
#include <net/red.h>

#ifdef QOS_RED_WRED  /*Rodney_20091115*/
#include <linux/proc_fs.h>
#include <linux/if_pppox.h>
#endif

//#define QOS_DROP_DUMP 1  /*Rodney_20091115*/

#ifdef QOS_RED_WRED  /*Rodney_20091115*/
#define QOS_QDISC_RED  2
#define QOS_QDISC_WRED 4
#define PRIORITY_IPP   1
#define PRIORITY_DSCP  2
#define PRIORITY_QUEUE 4  /*priority QUEUE*/

#define DSCP_L1     0
#define DSCP_L2    16
#define DSCP_L3    32
#define DSCP_L4    48

#define IPP_L1      0
#define IPP_L2      2
#define IPP_L3      4
#define IPP_L4      6

#define WRED_HIGHEST  3
#define WRED_HIGH     2
#define WRED_MEDIUM   1
#define WRED_LOW      0

static int qos_qdisc_mode = QOS_QDISC_RED;
static int priority_mode = PRIORITY_DSCP; 
#endif

#ifdef QOS_DROP_DUMP  /*Rodney_20091115*/
#define MAX_LOG_PKT  10000
static unsigned short int qos_drop_flag[MAX_LOG_PKT] = {0};
static unsigned short int qos_shift[MAX_LOG_PKT] = {0};
#endif

/*	Parameters, settable by user:
	-----------------------------

	limit		- bytes (must be > qth_max + burst)

	Hard limit on queue length, should be chosen >qth_max
	to allow packet bursts. This parameter does not
	affect the algorithms behaviour and can be chosen
	arbitrarily high (well, less than ram size)
	Really, this limit will never be reached
	if RED works correctly.
 */

struct red_sched_data
{
	u32			limit;		/* HARD maximal queue length */
	unsigned char		flags;
	struct red_parms	parms;
	struct red_stats	stats;
	struct Qdisc		*qdisc;
};

static inline int red_use_ecn(struct red_sched_data *q)
{
	return q->flags & TC_RED_ECN;
}

static inline int red_use_harddrop(struct red_sched_data *q)
{
	return q->flags & TC_RED_HARDDROP;
}

#ifdef QOS_RED_WRED  /*Rodney_20091115*/
static void red_prio_decide(struct sk_buff *skb, struct red_parms *p){
		struct iphdr *iph;

		if(skb->protocol == __constant_htons(ETH_P_IP))
    	    iph = ip_hdr(skb);
		else if(skb->protocol == __constant_htons(ETH_P_PPP_SES))
			iph = (struct iphdr *)(skb_network_header(skb) + PPPOE_SES_HLEN);
		else
			iph = ip_hdr(skb);

		if(priority_mode == PRIORITY_QUEUE){
			unsigned short int mark = (skb->mark & 0x6) >> 1;
			if((skb->mark & 0x1) == 1){  /*match & remarking enable*/
				if(mark == 0)
					p->factor = 6;
				else if(mark == 1)
					p->factor = 4;
				else if(mark == 2)
					p->factor = 3;
				else
					p->factor = 1;
			}
			else
				p->factor = 1;
		}
		else if(priority_mode == PRIORITY_DSCP){
			unsigned short int dscp = (iph->tos >> 2);
			if(dscp > DSCP_L4)
  				p->factor = 6;
			else if(dscp > DSCP_L3)
				p->factor = 4;
			else if(dscp > DSCP_L2)
				p->factor = 3;
			else
				p->factor = 1;
		}
		else{  /*IPP*/
			unsigned short int ipp = (iph->tos >> 5);
			if(ipp >= IPP_L4)
  				p->factor = 6;
			else if(ipp > IPP_L3)
				p->factor = 4;
			else if(ipp > IPP_L2)
				p->factor = 3;
			else
			    p->factor = 1;
		}
}
#endif

static int red_enqueue(struct sk_buff *skb, struct Qdisc* sch)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct Qdisc *child = q->qdisc;
	int ret;
#ifdef QOS_DROP_DUMP  /*Rodney_20091115*/
	static int drop_cnt = 0;
#endif

	q->parms.qavg = red_calc_qavg(&q->parms, child->qstats.backlog);
#ifdef QOS_RED_WRED  /*Rodney_20091115*/
	if(qos_qdisc_mode == QOS_QDISC_WRED)
		red_prio_decide(skb, &q->parms);			
	else
		q->parms.factor = 1;
#endif

	if (red_is_idling(&q->parms))
		red_end_of_idle_period(&q->parms);

	switch (red_action(&q->parms, q->parms.qavg)) {
		case RED_DONT_MARK:
			break;

		case RED_PROB_MARK:
			sch->qstats.overlimits++;
			if (!red_use_ecn(q) || !INET_ECN_set_ce(skb)) {
				q->stats.prob_drop++;
				goto congestion_drop;
			}

			q->stats.prob_mark++;
			break;

		case RED_HARD_MARK:
			sch->qstats.overlimits++;
			if (red_use_harddrop(q) || !red_use_ecn(q) ||
			    !INET_ECN_set_ce(skb)) {
				q->stats.forced_drop++;
				goto congestion_drop;
			}

			q->stats.forced_mark++;
			break;
	}

#ifdef QOS_DROP_DUMP  /*Rodney_20091115*/
	if(drop_cnt >= MAX_LOG_PKT)
		drop_cnt = 0;
	qos_shift[drop_cnt] = q->parms.factor;
	qos_drop_flag[drop_cnt++] = 0;
#endif
	ret = child->enqueue(skb, child);
	if (likely(ret == NET_XMIT_SUCCESS)) {
		sch->bstats.bytes += skb->len;
		sch->bstats.packets++;
		sch->q.qlen++;
	} else {
		q->stats.pdrop++;
		sch->qstats.drops++;
	}
	return ret;

congestion_drop:
#ifdef QOS_DROP_DUMP  /*Rodney_20091115*/
	if(drop_cnt >= MAX_LOG_PKT)
		drop_cnt = 0;
	qos_shift[drop_cnt] = q->parms.factor;
	qos_drop_flag[drop_cnt++] = 1;
#endif
	qdisc_drop(skb, sch);
	return NET_XMIT_CN;
}

static int red_requeue(struct sk_buff *skb, struct Qdisc* sch)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct Qdisc *child = q->qdisc;
	int ret;

	if (red_is_idling(&q->parms))
		red_end_of_idle_period(&q->parms);

	ret = child->ops->requeue(skb, child);
	if (likely(ret == NET_XMIT_SUCCESS)) {
		sch->qstats.requeues++;
		sch->q.qlen++;
	}
	return ret;
}

static struct sk_buff * red_dequeue(struct Qdisc* sch)
{
	struct sk_buff *skb;
	struct red_sched_data *q = qdisc_priv(sch);
	struct Qdisc *child = q->qdisc;

	skb = child->dequeue(child);
	if (skb)
		sch->q.qlen--;
	else if (!red_is_idling(&q->parms))
		red_start_of_idle_period(&q->parms);

	return skb;
}

static unsigned int red_drop(struct Qdisc* sch)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct Qdisc *child = q->qdisc;
	unsigned int len;

	if (child->ops->drop && (len = child->ops->drop(child)) > 0) {
		q->stats.other++;
		sch->qstats.drops++;
		sch->q.qlen--;
		return len;
	}

	if (!red_is_idling(&q->parms))
		red_start_of_idle_period(&q->parms);

	return 0;
}

static void red_reset(struct Qdisc* sch)
{
	struct red_sched_data *q = qdisc_priv(sch);

	qdisc_reset(q->qdisc);
	sch->q.qlen = 0;
	red_restart(&q->parms);
}

static void red_destroy(struct Qdisc *sch)
{
	struct red_sched_data *q = qdisc_priv(sch);
	qdisc_destroy(q->qdisc);
}

static struct Qdisc *red_create_dflt(struct Qdisc *sch, u32 limit)
{
	struct Qdisc *q;
	struct rtattr *rta;
	int ret;

	q = qdisc_create_dflt(sch->dev, &bfifo_qdisc_ops,
			      TC_H_MAKE(sch->handle, 1));
	if (q) {
		rta = kmalloc(RTA_LENGTH(sizeof(struct tc_fifo_qopt)),
			      GFP_KERNEL);
		if (rta) {
			rta->rta_type = RTM_NEWQDISC;
			rta->rta_len = RTA_LENGTH(sizeof(struct tc_fifo_qopt));
			((struct tc_fifo_qopt *)RTA_DATA(rta))->limit = limit;

			ret = q->ops->change(q, rta);
			kfree(rta);

			if (ret == 0)
				return q;
		}
		qdisc_destroy(q);
	}
	return NULL;
}

static int red_change(struct Qdisc *sch, struct rtattr *opt)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct rtattr *tb[TCA_RED_MAX];
	struct tc_red_qopt *ctl;
	struct Qdisc *child = NULL;

	if (opt == NULL || rtattr_parse_nested(tb, TCA_RED_MAX, opt))
		return -EINVAL;

	if (tb[TCA_RED_PARMS-1] == NULL ||
	    RTA_PAYLOAD(tb[TCA_RED_PARMS-1]) < sizeof(*ctl) ||
	    tb[TCA_RED_STAB-1] == NULL ||
	    RTA_PAYLOAD(tb[TCA_RED_STAB-1]) < RED_STAB_SIZE)
		return -EINVAL;

	ctl = RTA_DATA(tb[TCA_RED_PARMS-1]);

	if (ctl->limit > 0) {
		child = red_create_dflt(sch, ctl->limit);
		if (child == NULL)
			return -ENOMEM;
	}

	sch_tree_lock(sch);
	q->flags = ctl->flags;
	q->limit = ctl->limit;
	if (child) {
		qdisc_tree_decrease_qlen(q->qdisc, q->qdisc->q.qlen);
		qdisc_destroy(xchg(&q->qdisc, child));
	}

	red_set_parms(&q->parms, ctl->qth_min, ctl->qth_max, ctl->Wlog,
				 ctl->Plog, ctl->Scell_log,
				 RTA_DATA(tb[TCA_RED_STAB-1]));

	if (skb_queue_empty(&sch->q))
		red_end_of_idle_period(&q->parms);

	sch_tree_unlock(sch);
	return 0;
}

static int red_init(struct Qdisc* sch, struct rtattr *opt)
{
	struct red_sched_data *q = qdisc_priv(sch);

	q->qdisc = &noop_qdisc;
	return red_change(sch, opt);
}

static int red_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct rtattr *opts = NULL;
	struct tc_red_qopt opt = {
		.limit		= q->limit,
		.flags		= q->flags,
		.qth_min	= q->parms.qth_min >> q->parms.Wlog,
		.qth_max	= q->parms.qth_max >> q->parms.Wlog,
		.Wlog		= q->parms.Wlog,
		.Plog		= q->parms.Plog,
		.Scell_log	= q->parms.Scell_log,
	};

	opts = RTA_NEST(skb, TCA_OPTIONS);
	RTA_PUT(skb, TCA_RED_PARMS, sizeof(opt), &opt);
	return RTA_NEST_END(skb, opts);

rtattr_failure:
	return RTA_NEST_CANCEL(skb, opts);
}

static int red_dump_stats(struct Qdisc *sch, struct gnet_dump *d)
{
	struct red_sched_data *q = qdisc_priv(sch);
	struct tc_red_xstats st = {
		.early	= q->stats.prob_drop + q->stats.forced_drop,
		.pdrop	= q->stats.pdrop,
		.other	= q->stats.other,
		.marked	= q->stats.prob_mark + q->stats.forced_mark,
	};

	return gnet_stats_copy_app(d, &st, sizeof(st));
}

static int red_dump_class(struct Qdisc *sch, unsigned long cl,
			  struct sk_buff *skb, struct tcmsg *tcm)
{
	struct red_sched_data *q = qdisc_priv(sch);

	if (cl != 1)
		return -ENOENT;
	tcm->tcm_handle |= TC_H_MIN(1);
	tcm->tcm_info = q->qdisc->handle;
	return 0;
}

static int red_graft(struct Qdisc *sch, unsigned long arg, struct Qdisc *new,
		     struct Qdisc **old)
{
	struct red_sched_data *q = qdisc_priv(sch);

	if (new == NULL)
		new = &noop_qdisc;

	sch_tree_lock(sch);
	*old = xchg(&q->qdisc, new);
	qdisc_tree_decrease_qlen(*old, (*old)->q.qlen);
	qdisc_reset(*old);
	sch_tree_unlock(sch);
	return 0;
}

static struct Qdisc *red_leaf(struct Qdisc *sch, unsigned long arg)
{
	struct red_sched_data *q = qdisc_priv(sch);
	return q->qdisc;
}

static unsigned long red_get(struct Qdisc *sch, u32 classid)
{
	return 1;
}

static void red_put(struct Qdisc *sch, unsigned long arg)
{
	return;
}

static int red_change_class(struct Qdisc *sch, u32 classid, u32 parentid,
			    struct rtattr **tca, unsigned long *arg)
{
	return -ENOSYS;
}

static int red_delete(struct Qdisc *sch, unsigned long cl)
{
	return -ENOSYS;
}

static void red_walk(struct Qdisc *sch, struct qdisc_walker *walker)
{
	if (!walker->stop) {
		if (walker->count >= walker->skip)
			if (walker->fn(sch, 1, walker) < 0) {
				walker->stop = 1;
				return;
			}
		walker->count++;
	}
}

static struct tcf_proto **red_find_tcf(struct Qdisc *sch, unsigned long cl)
{
	return NULL;
}

static struct Qdisc_class_ops red_class_ops = {
	.graft		=	red_graft,
	.leaf		=	red_leaf,
	.get		=	red_get,
	.put		=	red_put,
	.change		=	red_change_class,
	.delete		=	red_delete,
	.walk		=	red_walk,
	.tcf_chain	=	red_find_tcf,
	.dump		=	red_dump_class,
};

static struct Qdisc_ops red_qdisc_ops = {
	.id		=	"red",
	.priv_size	=	sizeof(struct red_sched_data),
	.cl_ops		=	&red_class_ops,
	.enqueue	=	red_enqueue,
	.dequeue	=	red_dequeue,
	.requeue	=	red_requeue,
	.drop		=	red_drop,
	.init		=	red_init,
	.reset		=	red_reset,
	.destroy	=	red_destroy,
	.change		=	red_change,
	.dump		=	red_dump,
	.dump_stats	=	red_dump_stats,
	.owner		=	THIS_MODULE,
};

#ifdef QOS_RED_WRED  /*Rodney_20091115*/
static int sch_qosred_read_proc(char *page, char **start, off_t off,
		int count, int *eof, void *data){
	int index=0;
	off_t begin = 0;

	index += sprintf(page+index, "mode=%d\n", qos_qdisc_mode);
	index += sprintf(page+index, "priority=%d\n", priority_mode);

#ifdef QOS_DROP_DUMP
	int cnt;
	for(cnt=0 ; cnt<MAX_LOG_PKT ; cnt++)
		printk("cnt=%4d flag=%d shift=%d\n",cnt, qos_drop_flag[cnt], qos_shift[cnt]);
#endif

	*eof = 1;

	*start = page + (off - begin);
	index -= (off - begin);
	if (index<0)
		index = 0;
	if (index>count) 
		index = count;
	return index;
}
static int sch_qosred_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data){
	int len, proc_data[5] = {0};
	char get_buf[32];

	/* do a range checking, don't overflow buffers in kernel modules */
	if(count > 32)
		len = 32;
	else
		len = count;
	/* use the copy_from_user function to copy buffer data to our get_buf */
	if(copy_from_user(get_buf, buffer, len))
		return -EFAULT;
	/* zero terminate get_buf */
	get_buf[len]='\0';

	if(sscanf(get_buf, "%d %d %d %d %d", proc_data, (proc_data+1), (proc_data+2), (proc_data+3), (proc_data+4)) != 5)
		return count;

	qos_qdisc_mode = proc_data[0];
	priority_mode = proc_data[1];
	
	return len;
}
#endif

static int __init red_module_init(void)
{
#ifdef QOS_RED_WRED  /*Rodney_20091115*/
	struct proc_dir_entry *sch_red;

	sch_red = create_proc_entry("sch_qosred", 0, NULL);
	sch_red->read_proc = sch_qosred_read_proc;
	sch_red->write_proc = sch_qosred_write_proc;
#endif
	return register_qdisc(&red_qdisc_ops);
}

static void __exit red_module_exit(void)
{
#ifdef QOS_RED_WRED  /*Rodney_20091115*/
	remove_proc_entry("sch_qosred", NULL);
#endif
	unregister_qdisc(&red_qdisc_ops);
}

module_init(red_module_init)
module_exit(red_module_exit)

MODULE_LICENSE("GPL");
