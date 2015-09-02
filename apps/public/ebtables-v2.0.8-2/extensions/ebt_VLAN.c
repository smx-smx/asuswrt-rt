/* ebt_vlan_t
 *
 * Authors:
 *
 */
#if 1  /*Rodney_20090724*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "../include/ebtables_u.h"
#include <linux/netfilter_bridge/ebt_vlan_t.h>

#define VLAN_T_ID    '4'
#define VLAN_T_PRIO  '5'

#define OPT_VLAN_T_ID     0x01
#define OPT_VLAN_T_PRIO   0x02
#define OPT_VLAN_T_FLAGS	(OPT_VLAN_T_ID | OPT_VLAN_T_PRIO)

static struct option opts[] =
{
	{ "vlan_t-id"    , required_argument, 0, VLAN_T_ID },
	{ "vlan_t-prio"  , required_argument, 0, VLAN_T_PRIO },
	{ 0 }
};

static void print_help()
{
	printf(
	"VLAN target options:\n"
	" --vlan_t-id id         : Set vlan id value\n"
	" --vlan_t-prio prio     : Set vlan 802.1p value\n");
}

static void init(struct ebt_entry_target *target)
{
	struct ebt_vlan_t_info *vlaninfo = (struct ebt_vlan_t_info *)target->data;

	vlaninfo->target = EBT_CONTINUE;
}

static int parse(int c, char **argv, int argc,
   const struct ebt_u_entry *entry, unsigned int *flags,
   struct ebt_entry_target **target)
{
	struct ebt_vlan_t_info *vlaninfo = (struct ebt_vlan_t_info *)(*target)->data;
	char *end;
	struct ebt_vlan_t_info local;

	switch (c) {
		case VLAN_T_ID:
			ebt_check_option2(flags, OPT_VLAN_T_ID);
			local.id = strtoul(optarg, &end, 10);
			if (local.id > 4094 || *end != '\0')
				ebt_print_error2("Invalid vlan id range ('%s')", optarg);
			vlaninfo->id = local.id;
			vlaninfo->bitmask |= EBT_VLAN_ID;
			break;
		case VLAN_T_PRIO:
			ebt_check_option2(flags, OPT_VLAN_T_PRIO);
			local.prio = strtoul(optarg, &end, 10);
			if (local.prio >= 8 || *end != '\0')
				ebt_print_error2("Invalid vlan priority range ('%s')", optarg);
			vlaninfo->prio = local.prio;
			vlaninfo->bitmask |= EBT_VLAN_PRIO;
			break;
	 default:
		return 0;
	}
	return 1;
}

static void final_check(const struct ebt_u_entry *entry,
   const struct ebt_entry_target *target, const char *name,
   unsigned int hookmask, unsigned int time)
{

}

static void print(const struct ebt_u_entry *entry,
   const struct ebt_entry_target *target)
{
	struct ebt_vlan_t_info *vlaninfo = (struct ebt_vlan_t_info *)target->data;

	if (vlaninfo->bitmask & EBT_VLAN_ID)
		printf("--vlan_t-id %d", vlaninfo->id);
	if (vlaninfo->bitmask & EBT_VLAN_PRIO)
		printf("--vlan_t-prio %d", vlaninfo->prio);
}

static int compare(const struct ebt_entry_target *t1,
   const struct ebt_entry_target *t2)
{
	struct ebt_vlan_t_info *vlaninfo1 = (struct ebt_vlan_t_info *)t1->data;
	struct ebt_vlan_t_info *vlaninfo2 = (struct ebt_vlan_t_info *)t2->data;

	if (vlaninfo1->bitmask != vlaninfo2->bitmask)
		return 0;
	if ((vlaninfo1->bitmask & EBT_VLAN_ID) && (vlaninfo1->id != vlaninfo2->id))
		return 0;
	if ((vlaninfo1->bitmask & EBT_VLAN_ID) && (vlaninfo1->prio != vlaninfo2->prio))
		return 0;

	return 1;
}

static struct ebt_u_target vlan_t_target =
{
	.name		= EBT_VLAN_TARGET,
	.size		= sizeof(struct ebt_vlan_t_info),
	.help		= print_help,
	.init		= init,
	.parse		= parse,
	.final_check	= final_check,
	.print		= print,
	.compare	= compare,
	.extra_ops	= opts,
};

void _init(void)
{
	ebt_register_target(&vlan_t_target);
}
#endif
