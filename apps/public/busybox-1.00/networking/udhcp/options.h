/* options.h */
#ifndef _OPTIONS_H
#define _OPTIONS_H

#include "packet.h"

#define TYPE_MASK	0x0F

enum {
	OPTION_IP=1,
	OPTION_IP_PAIR,
	OPTION_STRING,
	OPTION_BOOLEAN,
	OPTION_U8,
	OPTION_U16,
	OPTION_S16,
	OPTION_U32,
	OPTION_S32,
#if defined(TCSUPPORT_OPT212)
	OPTION_6RD,
#endif 
};

#define OPTION_REQ	0x10 /* have the client request this option */
#define OPTION_LIST	0x20 /* There can be a list of 1 or more of these */

struct dhcp_option {
	char name[10];
	char flags;
	uint8_t code;
};

extern struct dhcp_option dhcp_options[];
extern int option_lengths[];

uint8_t *get_option(struct dhcpMessage *packet, int code);
int get_option_len(struct dhcpMessage *packet, int code);
int end_option(uint8_t *optionptr);
int add_option_string(uint8_t *optionptr, uint8_t *string);
int add_simple_option(uint8_t *optionptr, uint8_t code, uint32_t data);
struct option_set *find_option(struct option_set *opt_list, char code);
void attach_option(struct option_set **opt_list, struct dhcp_option *option, char *buffer, int length);

#if defined (CWMP) && defined(TR111)
#define DSL_FORUM_IANA_NUMBER	3561
#define DSL_FORUM_IANA_NUMBER_LENGTH	4

#ifdef TR111_TEST
#define DSL_FORUM_IANA_NUMBER_TEST	59661
#define DSL_FORUM_IANA_NUMBER_LENGTH_TEST	2
#endif
void addVIOption(uint8_t *optionptr);
#endif

#if defined(TCSUPPORT_CT_DHCPC_OPTION125SERVER)
void addVIOptionDebug(uint8_t *optionptr);
#endif

#ifdef DHCP_PROFILE
void addOPT240Option(uint8_t *optionptr);
#endif
#endif
