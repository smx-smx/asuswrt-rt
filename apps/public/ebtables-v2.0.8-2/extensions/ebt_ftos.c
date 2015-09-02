/* ebt_ftos
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
#include <linux/netfilter_bridge/ebt_ftos_t.h>

static struct option opts[] =
{
	{ "set-ftos" , required_argument, 0, '1' },
	{ 0 }
};

static void print_help()
{
	printf(
	"ftos target options:\n"
	" --set-ftos value[/mask]     : Set tos value\n");
}

static void init(struct ebt_entry_target *target)
{
	struct ebt_ftos_info *ftosinfo =
	   (struct ebt_ftos_info *)target->data;

	ftosinfo->target = EBT_CONTINUE;
	ftosinfo->mask = 0xff;
}

static void
parse_ftos(const unsigned char *s, struct ebt_ftos_info *ftosinfo){
	char *buffer;
	char *end, *cp;

	buffer = strdup(s);
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
	free(buffer);
}

static int parse(int c, char **argv, int argc,
   const struct ebt_u_entry *entry, unsigned int *flags,
   struct ebt_entry_target **target)
{
	struct ebt_ftos_info *ftosinfo =
	   (struct ebt_ftos_info *)(*target)->data;
	char *end;

	switch (c) {
	case '1':
		parse_ftos(optarg, ftosinfo);
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
	struct ebt_ftos_info *ftosinfo =
	   (struct ebt_ftos_info *)target->data;

	printf("--set-ftos 0x%x/0x%x", ftosinfo->ftos, ftosinfo->mask);

}

static int compare(const struct ebt_entry_target *t1,
   const struct ebt_entry_target *t2)
{
	struct ebt_ftos_info *ftosinfo1 =
	   (struct ebt_ftos_info *)t1->data;
	struct ebt_ftos_info *ftosinfo2 =
	   (struct ebt_ftos_info *)t2->data;

	return ftosinfo1->ftos == ftosinfo2->ftos &&
	ftosinfo1->mask == ftosinfo2->mask;
}

static struct ebt_u_target ftos_target =
{
	.name		= EBT_FTOS_TARGET,
	.size		= sizeof(struct ebt_ftos_info),
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
	ebt_register_target(&ftos_target);
}
#endif
