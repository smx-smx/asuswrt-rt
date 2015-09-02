/* ebt_ip
 *
 * Authors:
 * Bart De Schuymer <bdschuym@pandora.be>
 *
 * Changes:
 *    added ip-sport and ip-dport; parsing of port arguments is
 *    based on code from iptables-1.2.7a
 *    Innominate Security Technologies AG <mhopf@innominate.com>
 *    September, 2002
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <netdb.h>
#include "../include/ebtables_u.h"
#include <linux/netfilter_bridge/ebt_ip.h>

#define IP_SOURCE '1'
#define IP_DEST   '2'
#define IP_myTOS  '3' /* include/bits/in.h seems to already define IP_TOS */
#define IP_PROTO  '4'
#define IP_SPORT  '5'
#define IP_DPORT  '6'
#if 1  /*Rodney_20090724*/
#define IP_myIPP  '7'
#define IP_myDSCP '8'
#endif

static struct option opts[] =
{
	{ "ip-source"           , required_argument, 0, IP_SOURCE },
	{ "ip-src"              , required_argument, 0, IP_SOURCE },
	{ "ip-destination"      , required_argument, 0, IP_DEST   },
	{ "ip-dst"              , required_argument, 0, IP_DEST   },
	{ "ip-tos"              , required_argument, 0, IP_myTOS  },
	{ "ip-protocol"         , required_argument, 0, IP_PROTO  },
	{ "ip-proto"            , required_argument, 0, IP_PROTO  },
	{ "ip-source-port"      , required_argument, 0, IP_SPORT  },
	{ "ip-sport"            , required_argument, 0, IP_SPORT  },
	{ "ip-destination-port" , required_argument, 0, IP_DPORT  },
	{ "ip-dport"            , required_argument, 0, IP_DPORT  },
#if 1 /*Rodney_20090724*/
	{ "ip-ipp"              , required_argument, 0, IP_myIPP  },
	{ "ip-dscp"             , required_argument, 0, IP_myDSCP },
#endif
	{ 0 }
};

static void
parse_ip_range(char *address, uint32_t *addr, uint32_t *msk)
{
	char *buffer;
	char *cp;

	buffer = strdup(address);
	if ( (cp = strchr(buffer, '-')) == NULL ) {
		ebt_parse_ip_address(address, &addr[0], &msk[0]);
		ebt_parse_ip_address(address, &addr[1], &msk[1]);
	}
	else{
		*cp = '\0';
		cp++;
		ebt_parse_ip_address(buffer, &addr[0], &msk[0]);
		ebt_parse_ip_address(cp, &addr[1], &msk[1]);

		if ( (addr[0] > addr[1]) || (msk[0] != msk[1]) ) {
			ebt_print_error("Invalid ip range (min > max)");
		}
	}
	
	free(buffer);
	return;
}

/* put the mask into 4 bytes */
/* transform a protocol and service name into a port number */
static uint16_t parse_port(const char *protocol, const char *name)
{
	struct servent *service;
	char *end;
	int port;

	port = strtol(name, &end, 10);
	if (*end != '\0') {
		if (protocol &&
		    (service = getservbyname(name, protocol)) != NULL)
			return ntohs(service->s_port);
	}
	else if (port >= 0 || port <= 0xFFFF) {
		return port;
	}
	ebt_print_error("Problem with specified %s port '%s'",
			protocol?protocol:"", name);
	return 0;
}

static void
parse_port_range(const char *protocol, const char *portstring, uint16_t *ports)
{
	char *buffer;
	char *cp;

	buffer = strdup(portstring);
	if ((cp = strchr(buffer, ':')) == NULL)
		ports[0] = ports[1] = parse_port(protocol, buffer);
	else {
		*cp = '\0';
		cp++;
		ports[0] = buffer[0] ? parse_port(protocol, buffer) : 0;
		if (ebt_errormsg[0] != '\0')
			return;
		ports[1] = cp[0] ? parse_port(protocol, cp) : 0xFFFF;
		if (ebt_errormsg[0] != '\0')
			return;

		if (ports[0] > ports[1])
			ebt_print_error("Invalid portrange (min > max)");
	}
	free(buffer);
}

static void print_port_range(uint16_t *ports)
{
	if (ports[0] == ports[1])
		printf("%d ", ports[0]);
	else
		printf("%d:%d ", ports[0], ports[1]);
}
#if 1  /*Rodney_20090724*/
static uint8_t parse_ipp(const char *name)
{
	struct servent *service;
	char *end;
	uint8_t ipp;

	ipp = ((uint8_t)strtol(name, &end, 10) << 5);

	if (ipp >= 0 || ipp <= 0xFF) {
		return ipp;
	}
	ebt_print_error("Problem with specified ipp '%s'", name);

	return 0;
}
static void
parse_ipp_range(const char *portstring, uint8_t *ipp)
{
	char *buffer;
	char *cp;

	buffer = strdup(portstring);
	if ((cp = strchr(buffer, ':')) == NULL)
		ipp[0] = ipp[1] = parse_ipp(buffer);
	else {
		*cp = '\0';
		cp++;
		ipp[0] = buffer[0] ? parse_ipp(buffer) : 0;
		if (ebt_errormsg[0] != '\0')
			return;
		ipp[1] = cp[0] ? parse_ipp(cp) : 0xe0;
		if (ebt_errormsg[0] != '\0')
			return;

		if (ipp[0] > ipp[1])
			ebt_print_error("Invalid ipprange (min > max)");
	}
	free(buffer);
}
static void print_ipp_range(uint8_t *ipp)
{
	if (ipp[0] == ipp[1])
		printf("%d ", ipp[0]>>5);
	else
		printf("%d:%d ", ipp[0]>>5, ipp[1]>>5);
}

static uint8_t parse_dscp(const char *name)
{
	struct servent *service;
	char *end;
	uint8_t dscp;

	dscp = ((uint8_t)strtol(name, &end, 10) << 2);

	if (dscp >= 0 || dscp <= 0xFF) {
		return dscp;
	}
	ebt_print_error("Problem with specified dscp '%s'", name);

	return 0;
}
static void parse_dscp_range(const char *portstring, uint8_t *dscp)
{
	char *buffer;
	char *cp;

	buffer = strdup(portstring);
	if ((cp = strchr(buffer, ':')) == NULL)
		dscp[0] = dscp[1] = parse_dscp(buffer);
	else {
		*cp = '\0';
		cp++;
		dscp[0] = buffer[0] ? parse_dscp(buffer) : 0;
		if (ebt_errormsg[0] != '\0')
			return;
		dscp[1] = cp[0] ? parse_dscp(cp) : 0xfc;
		if (ebt_errormsg[0] != '\0')
			return;

		if (dscp[0] > dscp[1])
			ebt_print_error("Invalid dscprange (min > max)");
	}
	free(buffer);
}
static void print_dscp_range(uint8_t *dscp)
{
	if (dscp[0] == dscp[1])
		printf("%d ", dscp[0]>>2);
	else
		printf("%d:%d ", dscp[0]>>2, dscp[1]>>2);
}
#endif
static void print_help()
{
	printf(
"ip options:\n"
#if 0
"--ip-src    		[!] address[/mask]: ip source specification\n"
"--ip-dst    		[!] address[/mask]: ip destination specification\n"
#else
"--ip-src    		[!] address[/mask][-address[/mask]]: ip source specification\n"
"--ip-dst    		[!] address[/mask][-address[/mask]]: ip destination specification\n"
#endif
"--ip-tos    		[!] tos           : ip tos specification\n"
#if 1  /*Rodney_20090724*/
"--ip-ipp    [!] ipp[:ipp]     : ip ip precedence specification\n"
"--ip-dscp   [!] dscp[:dscp]   : ip dscp specification\n"
"--ip-proto  [!] protocol[:protocol]    : ip protocol specification\n"
#else
"--ip-proto  [!] protocol      : ip protocol specification\n"
#endif
"--ip-sport  [!] port[:port]   : tcp/udp source port or port range\n"
"--ip-dport  [!] port[:port]   : tcp/udp destination port or port range\n");
}

static void init(struct ebt_entry_match *match)
{
	struct ebt_ip_info *ipinfo = (struct ebt_ip_info *)match->data;

	ipinfo->invflags = 0;
	ipinfo->bitmask = 0;
}

#define OPT_SOURCE 0x01
#define OPT_DEST   0x02
#define OPT_TOS    0x04
#define OPT_PROTO  0x08
#define OPT_SPORT  0x10
#define OPT_DPORT  0x20
#if 1  /*Rodney_20090724*/
#define OPT_IPP    0x40
#define OPT_DSCP   0x80
#endif
static int parse(int c, char **argv, int argc, const struct ebt_u_entry *entry,
   unsigned int *flags, struct ebt_entry_match **match)
{
	struct ebt_ip_info *ipinfo = (struct ebt_ip_info *)(*match)->data;
#if 1  /*Rodney_20090724*/
	char *end, *end2, *cp;
	long int i, i2;
#else
	char *end;
	long int i;
#endif

	switch (c) {
	case IP_SOURCE:
		ebt_check_option2(flags, OPT_SOURCE);
		ipinfo->bitmask |= EBT_IP_SOURCE;

	case IP_DEST:
		if (c == IP_DEST) {
			ebt_check_option2(flags, OPT_DEST);
			ipinfo->bitmask |= EBT_IP_DEST;
		}
		if (ebt_check_inverse2(optarg)) {
			if (c == IP_SOURCE)
				ipinfo->invflags |= EBT_IP_SOURCE;
			else
				ipinfo->invflags |= EBT_IP_DEST;
		}
		
		if (c == IP_SOURCE) {
			//ebt_parse_ip_address(optarg, &ipinfo->saddr, &ipinfo->smsk);
			parse_ip_range(optarg, ipinfo->saddr, ipinfo->smsk);
		}
		else {
			//ebt_parse_ip_address(optarg, &ipinfo->daddr, &ipinfo->dmsk);
			parse_ip_range(optarg, ipinfo->daddr, ipinfo->dmsk);
		}
		break;

	case IP_SPORT:
	case IP_DPORT:
		if (c == IP_SPORT) {
			ebt_check_option2(flags, OPT_SPORT);
			ipinfo->bitmask |= EBT_IP_SPORT;
			if (ebt_check_inverse2(optarg))
				ipinfo->invflags |= EBT_IP_SPORT;
		} else {
			ebt_check_option2(flags, OPT_DPORT);
			ipinfo->bitmask |= EBT_IP_DPORT;
			if (ebt_check_inverse2(optarg))
				ipinfo->invflags |= EBT_IP_DPORT;
		}
		if (c == IP_SPORT)
			parse_port_range(NULL, optarg, ipinfo->sport);
		else
			parse_port_range(NULL, optarg, ipinfo->dport);
		break;

	case IP_myTOS:
		ebt_check_option2(flags, OPT_TOS);
		if (ebt_check_inverse2(optarg))
			ipinfo->invflags |= EBT_IP_TOS;
		i = strtol(optarg, &end, 16);
		if (i < 0 || i > 255 || *end != '\0')
			ebt_print_error2("Problem with specified IP tos");
		ipinfo->tos = i;
		ipinfo->bitmask |= EBT_IP_TOS;
		break;
#if 1  /*Rodney_20090724*/
	case IP_myIPP:
		ebt_check_option2(flags, OPT_IPP);
		if (ebt_check_inverse2(optarg))
			ipinfo->invflags |= EBT_IP_IPP;
		parse_ipp_range(optarg, ipinfo->ipp);
		ipinfo->bitmask |= EBT_IP_IPP;
		break;

	case IP_myDSCP:
		ebt_check_option2(flags, OPT_DSCP);
		if (ebt_check_inverse2(optarg))
			ipinfo->invflags |= EBT_IP_DSCP;
		parse_dscp_range(optarg, ipinfo->dscp);
		ipinfo->bitmask |= EBT_IP_DSCP;
		break;
#endif

	case IP_PROTO:
		ebt_check_option2(flags, OPT_PROTO);
		if (ebt_check_inverse2(optarg))
			ipinfo->invflags |= EBT_IP_PROTO;
#if 1  /*Rodney_20090724*/
		if((cp = strchr(optarg, ':')) == NULL){
			i = strtoul(optarg, &end, 10);
			if (*end != '\0') {
				struct protoent *pe;

				pe = getprotobyname(optarg);
				if (pe == NULL)
					ebt_print_error("Unknown specified IP protocol - %s", argv[optind - 1]);
				ipinfo->protocol[0] = pe->p_proto;
			} else {
				ipinfo->protocol[0] = (unsigned char) i;
			}
			ipinfo->protocol[1] = ipinfo->protocol[0];
		}
		else{
			*cp = '\0';
			cp++;
			i = strtoul(optarg, &end, 10);
			i2 = strtoul(cp, &end2, 10);
			if((*end != '\0') || (*end2 != '\0')) {
				struct protoent *pe;
				struct protoent *pe2;

				pe = getprotobyname(optarg);
				pe2 = getprotobyname(cp);
				if ((pe == NULL) || (pe2 == NULL))
					ebt_print_error("Unknown specified IP protocol - %s", argv[optind - 1]);
				ipinfo->protocol[0] = pe->p_proto;
				ipinfo->protocol[1] = pe2->p_proto;
			}
			else{
				ipinfo->protocol[0] = (unsigned char) i;
				ipinfo->protocol[1] = (unsigned char) i2;
			}
		}
#else
		i = strtoul(optarg, &end, 10);
		if (*end != '\0') {
			struct protoent *pe;

			pe = getprotobyname(optarg);
			if (pe == NULL)
				ebt_print_error("Unknown specified IP protocol - %s", argv[optind - 1]);
			ipinfo->protocol = pe->p_proto;
		} else {
			ipinfo->protocol = (unsigned char) i;
		}
#endif
		ipinfo->bitmask |= EBT_IP_PROTO;
		break;
	default:
		return 0;
	}
	return 1;
}

static void final_check(const struct ebt_u_entry *entry,
   const struct ebt_entry_match *match, const char *name,
   unsigned int hookmask, unsigned int time)
{
	struct ebt_ip_info *ipinfo = (struct ebt_ip_info *)match->data;

	if (entry->ethproto != ETH_P_IP || entry->invflags & EBT_IPROTO) {
		ebt_print_error("For IP filtering the protocol must be "
		            "specified as IPv4");
	}
#if 1  /*Rodney_20090724*/
	else if (ipinfo->bitmask & (EBT_IP_SPORT|EBT_IP_DPORT) &&
		(!(ipinfo->bitmask & EBT_IP_PROTO) ||
		ipinfo->invflags & EBT_IP_PROTO ||
		(ipinfo->protocol[0]!=IPPROTO_TCP && ipinfo->protocol[1]!=IPPROTO_TCP &&
		 ipinfo->protocol[0]!=IPPROTO_UDP && ipinfo->protocol[1]!=IPPROTO_UDP &&
		 ipinfo->protocol[0]!=IPPROTO_SCTP && ipinfo->protocol[1]!=IPPROTO_SCTP &&
		 ipinfo->protocol[0]!=IPPROTO_DCCP && ipinfo->protocol[1]!=IPPROTO_DCCP)))
#else
	else if (ipinfo->bitmask & (EBT_IP_SPORT|EBT_IP_DPORT) &&
		(!(ipinfo->bitmask & EBT_IP_PROTO) ||
		ipinfo->invflags & EBT_IP_PROTO ||
		(ipinfo->protocol!=IPPROTO_TCP &&
		 ipinfo->protocol!=IPPROTO_UDP &&
		 ipinfo->protocol!=IPPROTO_SCTP &&
		 ipinfo->protocol!=IPPROTO_DCCP)))
#endif
		ebt_print_error("For port filtering the IP protocol must be "
				"either 6 (tcp), 17 (udp), 33 (dccp) or "
				"132 (sctp)");
}

static void print(const struct ebt_u_entry *entry,
   const struct ebt_entry_match *match)
{
	struct ebt_ip_info *ipinfo = (struct ebt_ip_info *)match->data;
	int j;

/*modify by xyzhu_nj_20100413*/
	if (ipinfo->bitmask & EBT_IP_SOURCE) {
		printf("--ip-src ");
		if (ipinfo->invflags & EBT_IP_SOURCE)
			printf("! ");
		if ( ipinfo->saddr[0] == ipinfo->saddr[1] ) {
			for (j = 0; j < 4; j++)
				printf("%d%s",((unsigned char *)&ipinfo->saddr[0])[j],
				   (j == 3) ? "" : ".");
			printf("%s ", ebt_mask_to_dotted(ipinfo->smsk[0]));
		}
		else {
			for (j = 0; j < 4; j++)
				printf("%d%s",((unsigned char *)&ipinfo->saddr[0])[j],
				   (j == 3) ? "" : ".");
			printf("%s ", ebt_mask_to_dotted(ipinfo->smsk[0]));
			printf("-");
			for (j = 0; j < 4; j++)
				printf("%d%s",((unsigned char *)&ipinfo->saddr[1])[j],
				   (j == 3) ? "" : ".");
			printf("%s ", ebt_mask_to_dotted(ipinfo->smsk[1]));
		}
	}
	if (ipinfo->bitmask & EBT_IP_DEST) {
		printf("--ip-dst ");
		if (ipinfo->invflags & EBT_IP_DEST)
			printf("! ");
		if ( ipinfo->daddr[0] == ipinfo->daddr[1] ) {
			for (j = 0; j < 4; j++)
				printf("%d%s", ((unsigned char *)&ipinfo->daddr[0])[j],
				   (j == 3) ? "" : ".");
			printf("%s ", ebt_mask_to_dotted(ipinfo->dmsk[0]));
		}
		else {
			for (j = 0; j < 4; j++)
				printf("%d%s", ((unsigned char *)&ipinfo->daddr[0])[j],
				   (j == 3) ? "" : ".");
			printf("%s ", ebt_mask_to_dotted(ipinfo->dmsk[0]));
			printf("-");
			for (j = 0; j < 4; j++)
				printf("%d%s", ((unsigned char *)&ipinfo->daddr[1])[j],
				   (j == 3) ? "" : ".");
			printf("%s ", ebt_mask_to_dotted(ipinfo->dmsk[1]));
		}
	}
	if (ipinfo->bitmask & EBT_IP_TOS) {
		printf("--ip-tos ");
		if (ipinfo->invflags & EBT_IP_TOS)
			printf("! ");
		printf("0x%02X ", ipinfo->tos);
	}
#if 1  /*Rodney_20090724*/
	if (ipinfo->bitmask & EBT_IP_IPP) {
		printf("--ip-ipp ");
		if (ipinfo->invflags & EBT_IP_IPP)
			printf("! ");
		print_ipp_range(ipinfo->ipp);
	}
	if (ipinfo->bitmask & EBT_IP_DSCP) {
		printf("--ip-dscp ");
		if (ipinfo->invflags & EBT_IP_DSCP)
			printf("! ");
		print_dscp_range(ipinfo->dscp);
	}
#endif
	if (ipinfo->bitmask & EBT_IP_PROTO) {
		struct protoent *pe;
#if 1  /*Rodney_20090724*/
		struct protoent *pe2;
#endif
		printf("--ip-proto ");
		if (ipinfo->invflags & EBT_IP_PROTO)
			printf("! ");
#if 1  /*Rodney_20090724*/
		if(ipinfo->protocol[0] == ipinfo->protocol[1]){
			pe = getprotobynumber(ipinfo->protocol[0]);
			if(pe == NULL)
				printf("%d ", ipinfo->protocol[0]);
			else
				printf("%s ", pe->p_name);
		}
		else{
			pe = getprotobynumber(ipinfo->protocol[0]);
			pe2 = getprotobynumber(ipinfo->protocol[1]);
			if((pe == NULL) || (pe2 == NULL))
				printf("%d:%d ", ipinfo->protocol[0], ipinfo->protocol[1]);
			else
				printf("%s:%s ", pe->p_name, pe2->p_name);
		}
#else
		pe = getprotobynumber(ipinfo->protocol);
		if (pe == NULL) {
			printf("%d ", ipinfo->protocol);
		} else {
			printf("%s ", pe->p_name);
		}
#endif
	}
	if (ipinfo->bitmask & EBT_IP_SPORT) {
		printf("--ip-sport ");
		if (ipinfo->invflags & EBT_IP_SPORT)
			printf("! ");
		print_port_range(ipinfo->sport);
	}
	if (ipinfo->bitmask & EBT_IP_DPORT) {
		printf("--ip-dport ");
		if (ipinfo->invflags & EBT_IP_DPORT)
			printf("! ");
		print_port_range(ipinfo->dport);
	}
}

static int compare(const struct ebt_entry_match *m1,
   const struct ebt_entry_match *m2)
{
	struct ebt_ip_info *ipinfo1 = (struct ebt_ip_info *)m1->data;
	struct ebt_ip_info *ipinfo2 = (struct ebt_ip_info *)m2->data;

	if (ipinfo1->bitmask != ipinfo2->bitmask)
		return 0;
	if (ipinfo1->invflags != ipinfo2->invflags)
		return 0;
#if 0
	if (ipinfo1->bitmask & EBT_IP_SOURCE) {
		if (ipinfo1->saddr != ipinfo2->saddr)
			return 0;
		if (ipinfo1->smsk != ipinfo2->smsk)
			return 0;
	}
	if (ipinfo1->bitmask & EBT_IP_DEST) {
		if (ipinfo1->daddr != ipinfo2->daddr)
			return 0;
		if (ipinfo1->dmsk != ipinfo2->dmsk)
			return 0;
	}
#else
	if ( ipinfo1->bitmask & EBT_IP_SOURCE ) {
		if ( ipinfo1->saddr[0] != ipinfo2->saddr[0] ||
			 ipinfo1->saddr[1] != ipinfo2->saddr[1] ) {
			return 0;
		}
		if ( ipinfo1->smsk[0] != ipinfo2->smsk[0] ||
			 ipinfo1->smsk[0] != ipinfo2->smsk[0] ) {
			return 0;
		}
	}
	if ( ipinfo2->bitmask & EBT_IP_DEST ) {
		if ( ipinfo1->daddr[0] != ipinfo2->daddr[0] ||
			 ipinfo1->daddr[1] != ipinfo2->daddr[1] ) {
			return 0;
		}
		if ( ipinfo1->dmsk[0] != ipinfo2->dmsk[0] ||
			 ipinfo1->dmsk[1] != ipinfo2->dmsk[1] ) {
			return 0;
		}
	}
#endif
	if (ipinfo1->bitmask & EBT_IP_TOS) {
		if (ipinfo1->tos != ipinfo2->tos)
			return 0;
	}
#if 1  /*Rodney_20090724*/
	if (ipinfo1->bitmask & EBT_IP_IPP) {
		if (ipinfo1->ipp[0] != ipinfo2->ipp[0] ||
			ipinfo1->ipp[1] != ipinfo2->ipp[1])
			return 0;
	}
	if (ipinfo1->bitmask & EBT_IP_DSCP) {
		if (ipinfo1->dscp[0] != ipinfo2->dscp[0] ||
			ipinfo1->dscp[1] != ipinfo2->dscp[1])
			return 0;
	}
	if (ipinfo1->bitmask & EBT_IP_PROTO) {
		if (ipinfo1->protocol[0] != ipinfo2->protocol[0] ||
			ipinfo1->protocol[1] != ipinfo2->protocol[1])
			return 0;
	}
#else
	if (ipinfo1->bitmask & EBT_IP_PROTO) {
		if (ipinfo1->protocol != ipinfo2->protocol)
			return 0;
	}
#endif
	if (ipinfo1->bitmask & EBT_IP_SPORT) {
		if (ipinfo1->sport[0] != ipinfo2->sport[0] ||
		   ipinfo1->sport[1] != ipinfo2->sport[1])
			return 0;
	}
	if (ipinfo1->bitmask & EBT_IP_DPORT) {
		if (ipinfo1->dport[0] != ipinfo2->dport[0] ||
		   ipinfo1->dport[1] != ipinfo2->dport[1])
			return 0;
	}
	return 1;
}

static struct ebt_u_match ip_match =
{
	.name		= EBT_IP_MATCH,
	.size		= sizeof(struct ebt_ip_info),
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
	ebt_register_match(&ip_match);
}
