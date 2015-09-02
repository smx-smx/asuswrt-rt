/* ebt_vlan
 *
 * Authors:
 * Bart De Schuymer <bdschuym@pandora.be>
 * Nick Fedchik <nick@fedchik.org.ua>
 *
 * June, 2002
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include "../include/ebtables_u.h"
#include "../include/ethernetdb.h"
#include <linux/netfilter_bridge/ebt_vlan.h>
#include <linux/if_ether.h>

#define NAME_VLAN_ID    "id"
#define NAME_VLAN_PRIO  "prio"
#define NAME_VLAN_ENCAP "encap"

#define VLAN_ID    '1'
#define VLAN_PRIO  '2'
#define VLAN_ENCAP '3'

static struct option opts[] = {
	{"vlan-id"   , required_argument, NULL, VLAN_ID},
	{"vlan-prio" , required_argument, NULL, VLAN_PRIO},
	{"vlan-encap", required_argument, NULL, VLAN_ENCAP},
	{ 0 }
};

/*
 * option inverse flags definition
 */
#define OPT_VLAN_ID     0x01
#define OPT_VLAN_PRIO   0x02
#define OPT_VLAN_ENCAP  0x04
#define OPT_VLAN_FLAGS	(OPT_VLAN_ID | OPT_VLAN_PRIO | OPT_VLAN_ENCAP)

struct ethertypeent *ethent;

#if 1  /*Rodney_20090724*/
static uint16_t parse_id(const char *name)
{
	struct servent *service;
	char *end;
	uint16_t id;

	id = strtol(name, &end, 10);

	if (id >= 0 || id <= 0xFFFF) {
		return id;
	}
	ebt_print_error("Problem with specified vlanID '%s'", name);

	return 0;
}
static void
parse_id_range(const char *portstring, uint16_t *id)
{
	char *buffer;
	char *cp;

	buffer = strdup(portstring);
	if ((cp = strchr(buffer, ':')) == NULL)
		id[0] = id[1] = parse_id(buffer);
	else {
		*cp = '\0';
		cp++;
		id[0] = buffer[0] ? parse_id(buffer) : 0;
		if (ebt_errormsg[0] != '\0')
			return;
		id[1] = cp[0] ? parse_id(cp) : 0xFFFF;
		if (ebt_errormsg[0] != '\0')
			return;

		if (id[0] > id[1])
			ebt_print_error("Invalid VlanID range (min > max)");
	}
	free(buffer);
}
static void print_id_range(uint16_t *id)
{
	if (id[0] == id[1])
		printf("%d ", id[0]);
	else
		printf("%d:%d ", id[0], id[1]);
}

static uint8_t parse_prio(const char *name)
{
	struct servent *service;
	char *end;
	uint8_t prio;

	prio = (uint8_t)strtol(name, &end, 16);

	if (prio >= 0 || prio <= 0x07) {
		return prio;
	}
	ebt_print_error("Problem with specified vlanPRIO '%s'", name);

	return 0;
}
static void
parse_prio_range(const char *portstring, uint8_t *prio)
{
	char *buffer;
	char *cp;

	buffer = strdup(portstring);
	if ((cp = strchr(buffer, ':')) == NULL)
		prio[0] = prio[1] = parse_prio(buffer);
	else {
		*cp = '\0';
		cp++;
		prio[0] = buffer[0] ? parse_prio(buffer) : 0;
		if (ebt_errormsg[0] != '\0')
			return;
		prio[1] = cp[0] ? parse_prio(cp) : 0x07;
		if (ebt_errormsg[0] != '\0')
			return;

		if (prio[0] > prio[1])
			ebt_print_error("Invalid VlanPrio range (min > max)");
	}
	free(buffer);
}
static void print_prio_range(uint8_t *prio)
{
	if (prio[0] == prio[1])
		printf("%d ", prio[0]);
	else
		printf("%d:%d ", prio[0], prio[1]);
}
#endif

static void print_help()
{
	printf(
"vlan options:\n"
#if 1  /*Rodney_20090724*/
"--vlan-id [!] id[:id]       : vlan-tagged frame identifier, 0,1-4096 (integer)\n"
"--vlan-prio [!] prio[:prio]   : Priority-tagged frame's user priority, 0-7 (integer)\n"
#else
"--vlan-id [!] id       : vlan-tagged frame identifier, 0,1-4096 (integer)\n"
"--vlan-prio [!] prio   : Priority-tagged frame's user priority, 0-7 (integer)\n"
#endif
"--vlan-encap [!] encap : Encapsulated frame protocol (hexadecimal or name)\n");
}

static void init(struct ebt_entry_match *match)
{
	struct ebt_vlan_info *vlaninfo = (struct ebt_vlan_info *) match->data;
	vlaninfo->invflags = 0;
	vlaninfo->bitmask = 0;
}

static int parse(int c, char **argv, int argc, const struct ebt_u_entry *entry,
   unsigned int *flags, struct ebt_entry_match **match)
{
	struct ebt_vlan_info *vlaninfo = (struct ebt_vlan_info *) (*match)->data;
	char *end;
	struct ebt_vlan_info local;

	switch (c) {
	case VLAN_ID:
		ebt_check_option2(flags, OPT_VLAN_ID);
		if (ebt_check_inverse2(optarg))
			vlaninfo->invflags |= EBT_VLAN_ID;
#if 1  /*Rodney_20090724*/
		parse_id_range(optarg, vlaninfo->id);
#else
		local.id = strtoul(optarg, &end, 10);
		if (local.id > 4094 || *end != '\0')
			ebt_print_error2("Invalid --vlan-id range ('%s')", optarg);
		vlaninfo->id = local.id;
#endif

		vlaninfo->bitmask |= EBT_VLAN_ID;
		break;
	case VLAN_PRIO:
		ebt_check_option2(flags, OPT_VLAN_PRIO);
		if (ebt_check_inverse2(optarg))
			vlaninfo->invflags |= EBT_VLAN_PRIO;
#if 1  /*Rodney_20090724*/
		parse_prio_range(optarg, vlaninfo->prio);
#else
		local.prio = strtoul(optarg, &end, 10);
		if (local.prio >= 8 || *end != '\0')
			ebt_print_error2("Invalid --vlan-prio range ('%s')", optarg);
		vlaninfo->prio = local.prio;
#endif
		vlaninfo->bitmask |= EBT_VLAN_PRIO;

		break;
	case VLAN_ENCAP:
		ebt_check_option2(flags, OPT_VLAN_ENCAP);
		if (ebt_check_inverse2(optarg))
			vlaninfo->invflags |= EBT_VLAN_ENCAP;
		local.encap = strtoul(optarg, &end, 16);
		if (*end != '\0') {
			ethent = getethertypebyname(optarg);
			if (ethent == NULL)
				ebt_print_error("Unknown --vlan-encap value ('%s')", optarg);
			local.encap = ethent->e_ethertype;
		}
		if (local.encap < ETH_ZLEN)
			ebt_print_error2("Invalid --vlan-encap range ('%s')", optarg);
		vlaninfo->encap = htons(local.encap);
		vlaninfo->bitmask |= EBT_VLAN_ENCAP;
		break;
	default:
		return 0;

	}
	return 1;
}

static void final_check(const struct ebt_u_entry *entry,
   const struct ebt_entry_match *match,
   const char *name, unsigned int hookmask, unsigned int time)
{
	if (entry->ethproto != ETH_P_8021Q || entry->invflags & EBT_IPROTO)
		ebt_print_error("For vlan filtering the protocol must be specified as 802_1Q");

	/* Check if specified vlan-id=0 (priority-tagged frame condition)
	 * when vlan-prio was specified. */
	/* I see no reason why a user should be prohibited to match on a perhaps impossible situation <BDS>
	if (vlaninfo->bitmask & EBT_VLAN_PRIO &&
	    vlaninfo->id && vlaninfo->bitmask & EBT_VLAN_ID)
		ebt_print_error("When setting --vlan-prio the specified --vlan-id must be 0");*/
}

static void print(const struct ebt_u_entry *entry,
   const struct ebt_entry_match *match)
{
	struct ebt_vlan_info *vlaninfo = (struct ebt_vlan_info *) match->data;

	if (vlaninfo->bitmask & EBT_VLAN_ID) {
#if 1  /*Rodney_20090724*/
		printf("--vlan-id ");
		if (vlaninfo->invflags & EBT_VLAN_ID)
			printf("! ");
		print_id_range(vlaninfo->id);
#else
		printf("--vlan-id %s%d ", (vlaninfo->invflags & EBT_VLAN_ID) ? "! " : "", vlaninfo->id);
#endif
	}
	if (vlaninfo->bitmask & EBT_VLAN_PRIO) {
#if 1  /*Rodney_20090724*/
		printf("--vlan-prio ");
		if (vlaninfo->invflags & EBT_VLAN_PRIO)
			printf("! ");
		print_prio_range(vlaninfo->prio);
#else
		printf("--vlan-prio %s%d ", (vlaninfo->invflags & EBT_VLAN_PRIO) ? "! " : "", vlaninfo->prio);
#endif
	}
	if (vlaninfo->bitmask & EBT_VLAN_ENCAP) {
		printf("--vlan-encap %s", (vlaninfo->invflags & EBT_VLAN_ENCAP) ? "! " : "");
		ethent = getethertypebynumber(ntohs(vlaninfo->encap));
		if (ethent != NULL) {
			printf("%s ", ethent->e_name);
		} else {
			printf("%4.4X ", ntohs(vlaninfo->encap));
		}
	}
}

static int compare(const struct ebt_entry_match *vlan1,
   const struct ebt_entry_match *vlan2)
{
	struct ebt_vlan_info *vlaninfo1 = (struct ebt_vlan_info *) vlan1->data;
	struct ebt_vlan_info *vlaninfo2 = (struct ebt_vlan_info *) vlan2->data;

	if (vlaninfo1->bitmask != vlaninfo2->bitmask)
		return 0;
	if (vlaninfo1->invflags != vlaninfo2->invflags)
		return 0;
#if 1  /*Rodney_20090724*/
	if (vlaninfo1->bitmask & EBT_VLAN_ID) {
		if (vlaninfo1->id[0] != vlaninfo2->id[0] ||
			vlaninfo1->id[1] != vlaninfo2->id[1])
			return 0;
	}
	if (vlaninfo1->bitmask & EBT_VLAN_PRIO) {
		if (vlaninfo1->prio[0] != vlaninfo2->prio[0] ||
			vlaninfo1->prio[1] != vlaninfo2->prio[1])
			return 0;
	}
#else
	if (vlaninfo1->bitmask & EBT_VLAN_ID &&
	    vlaninfo1->id != vlaninfo2->id)
		return 0;
	if (vlaninfo1->bitmask & EBT_VLAN_PRIO &&
	    vlaninfo1->prio != vlaninfo2->prio)
		return 0;
#endif
	if (vlaninfo1->bitmask & EBT_VLAN_ENCAP &&
	    vlaninfo1->encap != vlaninfo2->encap)
		return 0;
	return 1;
}

static struct ebt_u_match vlan_match = {
	.name		= EBT_VLAN_MATCH,
	.size		= sizeof(struct ebt_vlan_info),
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
	ebt_register_match(&vlan_match);
}
