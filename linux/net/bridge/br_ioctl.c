/*
 *	Ioctl handler
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	$Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/linux/net/bridge/br_ioctl.c#3 $
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/capability.h>
#include <linux/kernel.h>
#include <linux/if_bridge.h>
#include <linux/netdevice.h>
#include <linux/times.h>
#include <asm/uaccess.h>
#include "br_private.h"

#ifdef CONFIG_IGMP_SNOOPING
MODULE_LICENSE("GPL");
void (*br_igmpsnoop_set_enabled_hook)(struct net_bridge *br, unsigned long val);
EXPORT_SYMBOL_GPL(br_igmpsnoop_set_enabled_hook);
int (*br_igmpsnoop_get_enabled_hook)(struct net_bridge *br);
EXPORT_SYMBOL_GPL(br_igmpsnoop_get_enabled_hook);

void (*br_igmpsnoop_set_ageing_time_hook)(struct net_bridge *br, unsigned long val);
EXPORT_SYMBOL_GPL(br_igmpsnoop_set_ageing_time_hook);
unsigned long (*br_igmpsnoop_get_ageing_time_hook)(struct net_bridge *br);
EXPORT_SYMBOL_GPL(br_igmpsnoop_get_ageing_time_hook);

void (*br_igmpsnoop_set_quickleave_hook)(struct net_bridge *br, unsigned long val);
EXPORT_SYMBOL_GPL(br_igmpsnoop_set_quickleave_hook);
int (*br_igmpsnoop_get_quickleave_hook)(struct net_bridge *br);
EXPORT_SYMBOL_GPL(br_igmpsnoop_get_quickleave_hook);

void (*br_igmpsnoop_set_routeportflag_hook)(struct net_bridge *br, unsigned long val);
EXPORT_SYMBOL_GPL(br_igmpsnoop_set_routeportflag_hook);
int (*br_igmpsnoop_get_routeportflag_hook)(struct net_bridge *br);
EXPORT_SYMBOL_GPL(br_igmpsnoop_get_routeportflag_hook);

void (*br_igmpsnoop_set_dbg_hook)(struct net_bridge *br, unsigned long val);
EXPORT_SYMBOL_GPL(br_igmpsnoop_set_dbg_hook);
int (*br_igmpsnoop_get_dbg_hook)(struct net_bridge *br);
EXPORT_SYMBOL_GPL(br_igmpsnoop_get_dbg_hook);

int (*br_mc_fdb_fillbuf_hook)(struct net_bridge *br, void *buf,
		   unsigned long maxnum, unsigned long skip);
EXPORT_SYMBOL_GPL(br_mc_fdb_fillbuf_hook);
#endif

#ifdef CONFIG_MLD_SNOOPING
void (*br_mldsnoop_enable_hook)(int is_enable);
void (*br_mldsnoop_set_age_hook)(struct net_bridge *br, unsigned long age);
void (*br_mldsnoop_show_hook)(void);
EXPORT_SYMBOL(br_mldsnoop_enable_hook);
EXPORT_SYMBOL(br_mldsnoop_set_age_hook);
EXPORT_SYMBOL(br_mldsnoop_show_hook);
#endif


#if defined(TCSUPPORT_HWNAT)
int port_reverse = 0;
EXPORT_SYMBOL(port_reverse);
#endif

/* called with RTNL */
static int get_bridge_ifindices(int *indices, int num)
{
	struct net_device *dev;
	int i = 0;

	for_each_netdev(dev) {
		if (i >= num)
			break;
		if (dev->priv_flags & IFF_EBRIDGE)
			indices[i++] = dev->ifindex;
	}

	return i;
}

/* called with RTNL */
static void get_port_ifindices(struct net_bridge *br, int *ifindices, int num)
{
	struct net_bridge_port *p;

	list_for_each_entry(p, &br->port_list, list) {
		if (p->port_no < num)
			ifindices[p->port_no] = p->dev->ifindex;
	}
}

/*
 * Format up to a page worth of forwarding table entries
 * userbuf -- where to copy result
 * maxnum  -- maximum number of entries desired
 *            (limited to a page for sanity)
 * offset  -- number of records to skip
 */
static int get_fdb_entries(struct net_bridge *br, void __user *userbuf,
			   unsigned long maxnum, unsigned long offset)
{
	int num;
	void *buf;
	size_t size;

	/* Clamp size to PAGE_SIZE, test maxnum to avoid overflow */
	if (maxnum > PAGE_SIZE/sizeof(struct __fdb_entry))
		maxnum = PAGE_SIZE/sizeof(struct __fdb_entry);

	size = maxnum * sizeof(struct __fdb_entry);

	buf = kmalloc(size, GFP_USER);
	if (!buf)
		return -ENOMEM;

	num = br_fdb_fillbuf(br, buf, maxnum, offset);
	if (num > 0) {
		if (copy_to_user(userbuf, buf, num*sizeof(struct __fdb_entry)))
			num = -EFAULT;
	}
	kfree(buf);

	return num;
}

#ifdef CONFIG_IGMP_SNOOPING
/*
 * Format up to a page worth of forwarding table entries
 * userbuf -- where to copy result
 * maxnum  -- maximum number of entries desired
 *            (limited to a page for sanity)
 * offset  -- number of records to skip
 */
static int get_mc_fdb_entries(struct net_bridge *br, void __user *userbuf,
			   unsigned long maxnum, unsigned long offset)
{
	int num = 0;
	void *buf;
	size_t size;
	
	typeof(br_mc_fdb_fillbuf_hook) br_mc_fdb_fillbuf_info;

	/* Clamp size to PAGE_SIZE, test maxnum to avoid overflow */
	if (maxnum > PAGE_SIZE/sizeof(struct __mc_fdb_entry))
		maxnum = PAGE_SIZE/sizeof(struct __mc_fdb_entry);

	size = maxnum * sizeof(struct __mc_fdb_entry);

	buf = kmalloc(size, GFP_USER);
	if (!buf)
		return -ENOMEM;
	
	br_mc_fdb_fillbuf_info = rcu_dereference(br_mc_fdb_fillbuf_hook);
	if(br_mc_fdb_fillbuf_info)
	{
		num = br_mc_fdb_fillbuf_info(br, buf, maxnum, offset);
		if (num > 0) {
			if (copy_to_user(userbuf, buf, num*sizeof(struct __mc_fdb_entry)))
				num = -EFAULT;
		}
	}
	kfree(buf);

	return num;
}
#endif

static int add_del_if(struct net_bridge *br, int ifindex, int isadd)
{
	struct net_device *dev;
	int ret;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	dev = dev_get_by_index(ifindex);
	if (dev == NULL)
		return -EINVAL;

	if (isadd)
		ret = br_add_if(br, dev);
	else
		ret = br_del_if(br, dev);

	dev_put(dev);
	return ret;
}

/*
 * Legacy ioctl's through SIOCDEVPRIVATE
 * This interface is deprecated because it was too difficult to
 * to do the translation for 32/64bit ioctl compatability.
 */
static int old_dev_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	struct net_bridge *br = netdev_priv(dev);
	unsigned long args[4];
#ifdef CONFIG_IGMP_SNOOPING
	typeof(br_igmpsnoop_set_enabled_hook) br_igmpsnoop_set_enabled_info;
	typeof(br_igmpsnoop_get_enabled_hook) br_igmpsnoop_get_enabled_info;
	typeof(br_igmpsnoop_set_ageing_time_hook) br_igmpsnoop_set_ageing_time_info;
	typeof(br_igmpsnoop_get_ageing_time_hook) br_igmpsnoop_get_ageing_time_info;
	typeof(br_igmpsnoop_set_quickleave_hook) br_igmpsnoop_set_quickleave_info;
	typeof(br_igmpsnoop_get_quickleave_hook) br_igmpsnoop_get_quickleave_info;
	typeof(br_igmpsnoop_set_routeportflag_hook) br_igmpsnoop_set_routeportflag_info;
	typeof(br_igmpsnoop_get_routeportflag_hook) br_igmpsnoop_get_routeportflag_info;
	typeof(br_igmpsnoop_set_dbg_hook) br_igmpsnoop_set_dbg_info;
	typeof(br_igmpsnoop_get_dbg_hook) br_igmpsnoop_get_dbg_info;
#endif
	#ifdef CONFIG_MLD_SNOOPING
	typeof(br_mldsnoop_enable_hook) br_mldsnoop_enable;
	typeof(br_mldsnoop_set_age_hook) br_mldsnoop_set_age;
	typeof(br_mldsnoop_show_hook) br_mldsnoop_show;
	#endif

	if (copy_from_user(args, rq->ifr_data, sizeof(args)))
		return -EFAULT;

	switch (args[0]) {
	case BRCTL_ADD_IF:
	case BRCTL_DEL_IF:
		return add_del_if(br, args[1], args[0] == BRCTL_ADD_IF);

	case BRCTL_GET_BRIDGE_INFO:
	{
		struct __bridge_info b;

		memset(&b, 0, sizeof(struct __bridge_info));
		rcu_read_lock();
		memcpy(&b.designated_root, &br->designated_root, 8);
		memcpy(&b.bridge_id, &br->bridge_id, 8);
		b.root_path_cost = br->root_path_cost;
		b.max_age = jiffies_to_clock_t(br->max_age);
		b.hello_time = jiffies_to_clock_t(br->hello_time);
		b.forward_delay = br->forward_delay;
		b.bridge_max_age = br->bridge_max_age;
		b.bridge_hello_time = br->bridge_hello_time;
		b.bridge_forward_delay = jiffies_to_clock_t(br->bridge_forward_delay);
		b.topology_change = br->topology_change;
		b.topology_change_detected = br->topology_change_detected;
		b.root_port = br->root_port;

		b.stp_enabled = (br->stp_enabled != BR_NO_STP);
		b.ageing_time = jiffies_to_clock_t(br->ageing_time);
		b.hello_timer_value = br_timer_value(&br->hello_timer);
		b.tcn_timer_value = br_timer_value(&br->tcn_timer);
		b.topology_change_timer_value = br_timer_value(&br->topology_change_timer);
		b.gc_timer_value = br_timer_value(&br->gc_timer);
	
#ifdef CONFIG_IGMP_SNOOPING
		br_igmpsnoop_get_enabled_info = rcu_dereference(br_igmpsnoop_get_enabled_hook);
		if(br_igmpsnoop_get_enabled_info)
			b.igmpsnoop_enabled = br_igmpsnoop_get_enabled_info(br) ? 1 : 0;
		
		br_igmpsnoop_get_quickleave_info = rcu_dereference(br_igmpsnoop_get_quickleave_hook);
		if(br_igmpsnoop_get_quickleave_info)
			b.igmpsnoop_quickleave = br_igmpsnoop_get_quickleave_info(br) ? 1 : 0;
		br_igmpsnoop_get_routeportflag_info = rcu_dereference(br_igmpsnoop_get_routeportflag_hook);
		if(br_igmpsnoop_get_routeportflag_info)
			b.igmpsnoop_routeportflag = br_igmpsnoop_get_routeportflag_info(br) ? 1 : 0;
		
		br_igmpsnoop_get_dbg_info = rcu_dereference(br_igmpsnoop_get_dbg_hook);
		if(br_igmpsnoop_get_dbg_info)
			b.igmpsnoop_dbg = br_igmpsnoop_get_dbg_info(br) ? 1 : 0;

		br_igmpsnoop_get_ageing_time_info = rcu_dereference(br_igmpsnoop_get_ageing_time_hook);
		if(br_igmpsnoop_get_ageing_time_info)
			b.igmpsnoop_ageing_time = jiffies_to_clock_t(br_igmpsnoop_get_ageing_time_info(br));
#endif
		rcu_read_unlock();

		if (copy_to_user((void __user *)args[1], &b, sizeof(b)))
			return -EFAULT;

		return 0;
	}

	case BRCTL_GET_PORT_LIST:
	{
		int num, *indices;

		num = args[2];
		if (num < 0)
			return -EINVAL;
		if (num == 0)
			num = 256;
		if (num > BR_MAX_PORTS)
			num = BR_MAX_PORTS;

		indices = kcalloc(num, sizeof(int), GFP_KERNEL);
		if (indices == NULL)
			return -ENOMEM;

		get_port_ifindices(br, indices, num);
		if (copy_to_user((void __user *)args[1], indices, num*sizeof(int)))
			num =  -EFAULT;
		kfree(indices);
		return num;
	}

	case BRCTL_SET_BRIDGE_FORWARD_DELAY:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		spin_lock_bh(&br->lock);
		br->bridge_forward_delay = clock_t_to_jiffies(args[1]);
		if (br_is_root_bridge(br))
			br->forward_delay = br->bridge_forward_delay;
		spin_unlock_bh(&br->lock);
		return 0;

	case BRCTL_SET_BRIDGE_HELLO_TIME:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		spin_lock_bh(&br->lock);
		br->bridge_hello_time = clock_t_to_jiffies(args[1]);
		if (br_is_root_bridge(br))
			br->hello_time = br->bridge_hello_time;
		spin_unlock_bh(&br->lock);
		return 0;

	case BRCTL_SET_BRIDGE_MAX_AGE:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		spin_lock_bh(&br->lock);
		br->bridge_max_age = clock_t_to_jiffies(args[1]);
		if (br_is_root_bridge(br))
			br->max_age = br->bridge_max_age;
		spin_unlock_bh(&br->lock);
		return 0;

	case BRCTL_SET_AGEING_TIME:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		br->ageing_time = clock_t_to_jiffies(args[1]);
		return 0;

	case BRCTL_GET_PORT_INFO:
	{
		struct __port_info p;
		struct net_bridge_port *pt;

		rcu_read_lock();
		if ((pt = br_get_port(br, args[2])) == NULL) {
			rcu_read_unlock();
			return -EINVAL;
		}

		memset(&p, 0, sizeof(struct __port_info));
		memcpy(&p.designated_root, &pt->designated_root, 8);
		memcpy(&p.designated_bridge, &pt->designated_bridge, 8);
		p.port_id = pt->port_id;
		p.designated_port = pt->designated_port;
		p.path_cost = pt->path_cost;
		p.designated_cost = pt->designated_cost;
		p.state = pt->state;
		p.top_change_ack = pt->topology_change_ack;
		p.config_pending = pt->config_pending;
		p.message_age_timer_value = br_timer_value(&pt->message_age_timer);
		p.forward_delay_timer_value = br_timer_value(&pt->forward_delay_timer);
		p.hold_timer_value = br_timer_value(&pt->hold_timer);
#ifdef CONFIG_IGMP_SNOOPING
		p.is_router = pt->is_router;
#endif

		rcu_read_unlock();

		if (copy_to_user((void __user *)args[1], &p, sizeof(p)))
			return -EFAULT;

		return 0;
	}

	case BRCTL_SET_BRIDGE_STP_STATE:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		br_stp_set_enabled(br, args[1]);
		return 0;

	case BRCTL_SET_BRIDGE_PRIORITY:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		spin_lock_bh(&br->lock);
		br_stp_set_bridge_priority(br, args[1]);
		spin_unlock_bh(&br->lock);
		return 0;

	case BRCTL_SET_PORT_PRIORITY:
	{
		struct net_bridge_port *p;
		int ret = 0;

		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		if (args[2] >= (1<<(16-BR_PORT_BITS)))
			return -ERANGE;

		spin_lock_bh(&br->lock);
		if ((p = br_get_port(br, args[1])) == NULL)
			ret = -EINVAL;
		else
			br_stp_set_port_priority(p, args[2]);
		spin_unlock_bh(&br->lock);
		return ret;
	}

	case BRCTL_SET_PATH_COST:
	{
		struct net_bridge_port *p;
		int ret = 0;

		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		if ((p = br_get_port(br, args[1])) == NULL)
			ret = -EINVAL;
		else
			br_stp_set_path_cost(p, args[2]);

		return ret;
	}

	case BRCTL_GET_FDB_ENTRIES:
		return get_fdb_entries(br, (void __user *)args[1],
				       args[2], args[3]);
#ifdef CONFIG_IGMP_SNOOPING
	case BRCTL_SET_IGMPSNOOPING_STATE:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		
		br_igmpsnoop_set_enabled_info = rcu_dereference(br_igmpsnoop_set_enabled_hook);
		if(br_igmpsnoop_set_enabled_info)
			br_igmpsnoop_set_enabled_info(br, args[1]);
		return 0;

	case BRCTL_SET_IGMPSNOOPING_AGEING_TIME:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		br_igmpsnoop_set_ageing_time_info = rcu_dereference(br_igmpsnoop_set_ageing_time_hook);
		if(br_igmpsnoop_set_ageing_time_info)
			br_igmpsnoop_set_ageing_time_info(br, clock_t_to_jiffies(args[1]));
		return 0;

	case BRCTL_GET_MC_FDB_ENTRIES:
		return get_mc_fdb_entries(br, (void __user *)args[1],
				       args[2], args[3]);

	case BRCTL_SET_IGMPSNOOPING_QUICKLEAVE:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		
		br_igmpsnoop_set_quickleave_info = rcu_dereference(br_igmpsnoop_set_quickleave_hook);
		if(br_igmpsnoop_set_quickleave_info)
			br_igmpsnoop_set_quickleave_info(br, args[1]);
		return 0;

	case BRCTL_SET_IGMPSNOOPING_ROUTEPORTFLAG:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		
		br_igmpsnoop_set_routeportflag_info = rcu_dereference(br_igmpsnoop_set_routeportflag_hook);
		if(br_igmpsnoop_set_routeportflag_info)
			br_igmpsnoop_set_routeportflag_info(br, args[1]);
		return 0;
		
	case BRCTL_SET_IGMPSNOOPING_DBG:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		br_igmpsnoop_set_dbg_info = rcu_dereference(br_igmpsnoop_set_dbg_hook);
		if(br_igmpsnoop_set_dbg_info)
			br_igmpsnoop_set_dbg_info(br, args[1]);
		return 0;
#endif

#ifdef CONFIG_MLD_SNOOPING				       
	case BRCTL_SET_MLDSNOOPING_STATE:
		br_mldsnoop_enable = rcu_dereference(br_mldsnoop_enable_hook);		
		if(br_mldsnoop_enable)
			br_mldsnoop_enable(args[1]);			
		return 0;		
	
	case BRCTL_SET_MLDSNOOPING_AGE:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		br_mldsnoop_set_age = rcu_dereference(br_mldsnoop_set_age_hook);
		if(br_mldsnoop_set_age)
			br_mldsnoop_set_age(br, clock_t_to_jiffies(args[1]));
		return 0;
		
	case BRCTL_GET_MLDSNOOPING_INFO:
		br_mldsnoop_show = rcu_dereference(br_mldsnoop_show_hook);
		if(br_mldsnoop_show)
				br_mldsnoop_show();			
		return 0;
#endif
	}

	return -EOPNOTSUPP;
}

static int old_deviceless(void __user *uarg)
{
	unsigned long args[3];

	if (copy_from_user(args, uarg, sizeof(args)))
		return -EFAULT;

	switch (args[0]) {
	case BRCTL_GET_VERSION:
		return BRCTL_VERSION;

	case BRCTL_GET_BRIDGES:
	{
		int *indices;
		int ret = 0;

		if (args[2] >= 2048)
			return -ENOMEM;
		indices = kcalloc(args[2], sizeof(int), GFP_KERNEL);
		if (indices == NULL)
			return -ENOMEM;

		args[2] = get_bridge_ifindices(indices, args[2]);

		ret = copy_to_user((void __user *)args[1], indices, args[2]*sizeof(int))
			? -EFAULT : args[2];

		kfree(indices);
		return ret;
	}

	case BRCTL_ADD_BRIDGE:
	case BRCTL_DEL_BRIDGE:
	{
		char buf[IFNAMSIZ];

		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		if (copy_from_user(buf, (void __user *)args[1], IFNAMSIZ))
			return -EFAULT;

		buf[IFNAMSIZ-1] = 0;

		if (args[0] == BRCTL_ADD_BRIDGE)
			return br_add_bridge(buf);

		return br_del_bridge(buf);
	}
	}

	return -EOPNOTSUPP;
}

int br_ioctl_deviceless_stub(unsigned int cmd, void __user *uarg)
{
	switch (cmd) {
	case SIOCGIFBR:
	case SIOCSIFBR:
		return old_deviceless(uarg);

	case SIOCBRADDBR:
	case SIOCBRDELBR:
	{
		char buf[IFNAMSIZ];

		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		if (copy_from_user(buf, uarg, IFNAMSIZ))
			return -EFAULT;

		buf[IFNAMSIZ-1] = 0;
		if (cmd == SIOCBRADDBR)
			return br_add_bridge(buf);

		return br_del_bridge(buf);
	}
	}
	return -EOPNOTSUPP;
}

int br_dev_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	struct net_bridge *br = netdev_priv(dev);

	switch(cmd) {
	case SIOCDEVPRIVATE:
		return old_dev_ioctl(dev, rq, cmd);

	case SIOCBRADDIF:
	case SIOCBRDELIF:
		return add_del_if(br, rq->ifr_ifindex, cmd == SIOCBRADDIF);

	}

	pr_debug("Bridge does not support ioctl 0x%x\n", cmd);
	return -EOPNOTSUPP;
}
