
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "../include/ebtables_u.h"
#include <linux/netfilter_bridge/ebt_tc.h>


static struct option opts[] =
{
	{ "set-tc" , required_argument, 0, '1' },
	{ 0 }
};

static void print_help()
{
	printf(
	"tc target options:\n"
	" --set-tc value     : Set tc value\n");
}


static void init(struct ebt_entry_target *target)
{
	struct ebt_tc_info *tcinfo =
	   (struct ebt_tc_info *)target->data;

	tcinfo->target = EBT_CONTINUE;
}

static void
parse_tc(const unsigned char *s, struct ebt_tc_info *tcinfo){
	char *buffer;
	char *end, *cp;

	buffer = strdup(s);
#if 0
	if ((cp = strchr(buffer, '/')) == NULL){
		ftosinfo->ftos = strtol(buffer, &end, 0);
		ftosinfo->mask = 0xff;
	}
	else {
		*cp = '\0';
		cp++;
		ftosinfo->ftos = buffer[0] ? strtol(buffer, &end, 0) : 0;
		if (ebt_errormsg[0] != '\0')
			return;
		ftosinfo->mask = cp[0] ? strtol(cp, &end, 0) : 0xFF;
		if (ebt_errormsg[0] != '\0')
			return;
	}
#else
	tcinfo->tc = strtol(buffer, &end, 0);

	/* add for debug */
	printf( "tcinfo->tc is 0x%x\n", tcinfo->tc );
#endif
	free(buffer);
}

static int parse(int c, char **argv, int argc,
   const struct ebt_u_entry *entry, unsigned int *flags,
   struct ebt_entry_target **target)
{
	struct ebt_tc_info *tcinfo =
	   (struct ebt_tc_info *)(*target)->data;
	char *end;

	switch (c) {
	case '1':
		parse_tc(optarg, tcinfo);
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
	return;
}

static void print(const struct ebt_u_entry *entry,
   const struct ebt_entry_target *target)
{
	struct ebt_tc_info *tcinfo =
	   (struct ebt_tc_info *)target->data;

	printf("--set-tc 0x%x", tcinfo->tc);

}

static int compare(const struct ebt_entry_target *t1,
   const struct ebt_entry_target *t2)
{
	struct ebt_tc_info *tcinfo1 =
	   (struct ebt_tc_info *)t1->data;
	struct ebt_tc_info *tcinfo2 =
	   (struct ebt_tc_info *)t2->data;

	return tcinfo1->tc == tcinfo2->tc;
}

static struct ebt_u_target tc_target =
{
	.name		= EBT_TC_TARGET,
	.size		= sizeof(struct ebt_tc_info),
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
	ebt_register_target(&tc_target);
}
