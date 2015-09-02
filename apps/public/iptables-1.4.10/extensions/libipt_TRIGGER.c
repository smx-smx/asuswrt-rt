/* Port-triggering target. 
 *
 * Copyright (C) 2003, CyberTAN Corporation
 * All Rights Reserved.
 */

/* Shared library add-on to iptables to add port-trigger support. */

#include <stdbool.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <xtables.h>
#include <iptables.h> /* get_kernel_version */
#include <linux/netfilter_ipv4/ip_tables.h>
#include <net/netfilter/nf_nat.h>
#include <linux/netfilter_ipv4/ipt_TRIGGER.h>
/* Function which prints out usage message. */
static void
TRIGGER_help(void)
{
	printf(
"TRIGGER v%s options:\n"
" --trigger-type (dnat|in|out)\n"
"				Trigger type\n"
" --trigger-proto proto\n"
"				Trigger protocol\n"
" --trigger-match port[-port]\n"
"				Trigger destination port range\n"
" --trigger-relate port[-port]\n"
"				Port range to map related destination port range to.\n\n",
IPTABLES_VERSION);
}

static struct option TRIGGER_opts[] = {
		{.name = "trigger-type", .has_arg = true, .val = '1'},
		{.name = "trigger-proto", .has_arg = true, .val = '2'},
		{.name = "trigger-match", .has_arg = true, .val = '3'},
		{.name = "trigger-relate", .has_arg = true, .val = '4'},
		XT_GETOPT_TABLEEND,
};

/* Initialize the target. */
static void
TRIGGER_init(struct xt_entry_target *t)
{
	/* Can't cache this */
//	*nfcache |= NFC_UNKNOWN;
}

/* Parses ports */
static void
parse_ports(const char *arg, u_int16_t *ports)
{
	const char *dash=NULL;
	int port=0;

	port = atoi(arg);
	if (port == 0 || port > 65535)
		xtables_error(PARAMETER_PROBLEM, "Port range `%s' invalid\n", arg);

	dash = strchr(arg, '-');
	if (!dash)
		ports[0] = ports[1] = port;
	else {
		int maxport;

		maxport = atoi(dash + 1);
		if (maxport == 0 || maxport > 65535)
			xtables_error(PARAMETER_PROBLEM,
				   "Port range `%s' invalid\n", dash+1);
		if (maxport < port)
			xtables_error(PARAMETER_PROBLEM,
				   "Port range `%s' invalid\n", arg);
		ports[0] = port;
		ports[1] = maxport;
	}
}


/* Function which parses command options; returns true if it
   ate an option */
static int
TRIGGER_parse(int c, char **argv, int invert, unsigned int *flags,
      const struct ipt_entry *entry,
      struct xt_entry_target **target)
{
	struct ipt_trigger_info *info = (struct ipt_trigger_info *)(*target)->data;

	switch (c) {
	case '1':
		if (!strcasecmp(optarg, "dnat"))
			info->type = IPT_TRIGGER_DNAT;
		else if (!strcasecmp(optarg, "in"))
			info->type = IPT_TRIGGER_IN;
		else if (!strcasecmp(optarg, "out"))
			info->type = IPT_TRIGGER_OUT;
		else
			xtables_error(PARAMETER_PROBLEM,
				   "unknown type `%s' specified", optarg);
		return 1;

	case '2':
		if (!strcasecmp(optarg, "tcp"))
			info->proto = IPPROTO_TCP;
		else if (!strcasecmp(optarg, "udp"))
			info->proto = IPPROTO_UDP;
		else if (!strcasecmp(optarg, "all"))
			info->proto = 0;
		else
			xtables_error(PARAMETER_PROBLEM,
				   "unknown protocol `%s' specified", optarg);
		return 1;

	case '3':
		if (xtables_check_inverse(optarg, &invert, &optind, 0,argv))
			xtables_error(PARAMETER_PROBLEM,
				   "Unexpected `!' after --trigger-match");

		parse_ports(optarg, info->ports.mport);
		return 1;

	case '4':
		if (xtables_check_inverse(optarg, &invert, &optind, 0,argv))
			xtables_error(PARAMETER_PROBLEM,
				   "Unexpected `!' after --trigger-relate");

		parse_ports(optarg, info->ports.rport);
		*flags |= IP_NAT_RANGE_PROTO_SPECIFIED;
		return 1;

	default:
		return 0;
	}
}

/* Final check; don't care. */
static void TRIGGER_check(unsigned int flags)
{
}

/* Prints out the targinfo. */
static void
TRIGGER_print(const void *ip, const struct xt_entry_target *target,
                       int numeric)
{
	struct ipt_trigger_info *info = (struct ipt_trigger_info *)target->data;

	printf("TRIGGER ");
	if (info->type == IPT_TRIGGER_DNAT)
		printf("type:dnat ");
	else if (info->type == IPT_TRIGGER_IN)
		printf("type:in ");
	else if (info->type == IPT_TRIGGER_OUT)
		printf("type:out ");

	if (info->proto == IPPROTO_TCP)
		printf("tcp ");
	else if (info->proto == IPPROTO_UDP)
		printf("udp ");

	printf("match:%hu", info->ports.mport[0]);
	if (info->ports.mport[1] > info->ports.mport[0])
		printf("-%hu", info->ports.mport[1]);
	printf(" ");

	printf("relate:%hu", info->ports.rport[0]);
	if (info->ports.rport[1] > info->ports.rport[0])
		printf("-%hu", info->ports.rport[1]);
	printf(" ");
}

/* Saves the union ipt_targinfo in parsable form to stdout. */
static void
TRIGGER_save(const void *ip, const struct xt_entry_target *target)
{
	struct ipt_trigger_info *info = (struct ipt_trigger_info *)target->data;

	printf("--trigger-proto ");
	if (info->proto == IPPROTO_TCP)
		printf("tcp ");
	else if (info->proto == IPPROTO_UDP)
		printf("udp ");
	printf("--trigger-match %hu-%hu ", info->ports.mport[0], info->ports.mport[1]);
	printf("--trigger-relate %hu-%hu ", info->ports.rport[0], info->ports.rport[1]);
}
/* mark by tangping for new iptables
struct iptables_target trigger = { 
    .next               = NULL,
    .name               = "TRIGGER",
    .version            = IPTABLES_VERSION,
    .size               = IPT_ALIGN(sizeof(struct ipt_trigger_info)),
    .userspacesize      = IPT_ALIGN(sizeof(struct ipt_trigger_info)),
    .help             = &help,
    .init               = &init,
    .parse              = &parse, 
    .final_check        = &final_check, 
    .print              = &print, 
    .save               = &save, 
    .extra_opts         = opts 
};
*/
struct xtables_target trigger = { 
    .name               = "TRIGGER",
    .version            = XTABLES_VERSION,
    .family		= NFPROTO_IPV4,
    .size               = XT_ALIGN(sizeof(struct ipt_trigger_info)),
    .userspacesize      = XT_ALIGN(sizeof(struct ipt_trigger_info)),
    .help             = TRIGGER_help,
    .init               = TRIGGER_init,
    .parse              = TRIGGER_parse, 
    .final_check        = TRIGGER_check, 
    .print              = TRIGGER_print, 
    .save               = TRIGGER_save, 
    .extra_opts         = TRIGGER_opts 
};

void _init(void)
{
	xtables_register_target(&trigger);
}
