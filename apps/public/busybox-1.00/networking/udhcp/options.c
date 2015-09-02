/*
 * options.c -- DHCP server option packet tools
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 */

#include <stdlib.h>
#include <string.h>

#include "dhcpd.h"
#include "files.h"
#include "options.h"
#include "common.h"

/* supported options are easily added here */
struct dhcp_option dhcp_options[] = {
	/* name[10]	flags					code */
	{"subnet",	OPTION_IP | OPTION_REQ,			0x01},
	{"timezone",	OPTION_S32,				0x02},
	{"router",	OPTION_IP | OPTION_LIST | OPTION_REQ,	0x03},
	{"timesvr",	OPTION_IP | OPTION_LIST,		0x04},
	{"namesvr",	OPTION_IP | OPTION_LIST,		0x05},
	{"dns",		OPTION_IP | OPTION_LIST | OPTION_REQ,	0x06},
	{"logsvr",	OPTION_IP | OPTION_LIST,		0x07},
	{"cookiesvr",	OPTION_IP | OPTION_LIST,		0x08},
	{"lprsvr",	OPTION_IP | OPTION_LIST,		0x09},
	{"hostname",	OPTION_STRING | OPTION_REQ,		0x0c},
	{"bootsize",	OPTION_U16,				0x0d},
	{"domain",	OPTION_STRING | OPTION_REQ,		0x0f},
	{"swapsvr",	OPTION_IP,				0x10},
	{"rootpath",	OPTION_STRING,				0x11},
	{"ipttl",	OPTION_U8,				0x17},
	{"mtu",		OPTION_U16,				0x1a},
	{"broadcast",	OPTION_IP | OPTION_REQ,			0x1c},
	{"ntpsrv",	OPTION_IP | OPTION_LIST,		0x2a},
	{"wins",	OPTION_IP | OPTION_LIST,		0x2c},
	{"requestip",	OPTION_IP,				0x32},
	{"lease",	OPTION_U32,				0x33},
	{"dhcptype",	OPTION_U8,				0x35},
	{"serverid",	OPTION_IP,				0x36},
	{"message",	OPTION_STRING,				0x38},
	{"tftp",	OPTION_STRING,				0x42},
	{"bootfile",	OPTION_STRING,				0x43},
#if defined(TCSUPPORT_OPT212)
	{"6rd",		OPTION_6RD | OPTION_REQ,	0xd4},
#endif 
	{"",		0x00,				0x00}
};

/* Lengths of the different option types */
int option_lengths[] = {
	[OPTION_IP] =		4,
	[OPTION_IP_PAIR] =	8,
	[OPTION_BOOLEAN] =	1,
	[OPTION_STRING] =	1,
	[OPTION_U8] =		1,
	[OPTION_U16] =		2,
	[OPTION_S16] =		2,
	[OPTION_U32] =		4,
	[OPTION_S32] =		4,
#if  defined(TCSUPPORT_OPT212)
	[OPTION_6RD] =		22, //use the minimum length here
#endif 
};


/* get an option with bounds checking (warning, not aligned). */
uint8_t *get_option(struct dhcpMessage *packet, int code)
{
	int i, length;
	uint8_t *optionptr;
	int over = 0, done = 0, curr = OPTION_FIELD;

	optionptr = packet->options;
	i = 0;
	length = 308;
	while (!done) {
		if (i >= length) {
			LOG(LOG_WARNING, "bogus packet, option fields too long.");
			return NULL;
		}
		if (optionptr[i + OPT_CODE] == code) {
			if (i + 1 + optionptr[i + OPT_LEN] >= length) {
				LOG(LOG_WARNING, "bogus packet, option fields too long.");
				return NULL;
			}
			return optionptr + i + 2;
		}
		switch (optionptr[i + OPT_CODE]) {
		case DHCP_PADDING:
			i++;
			break;
		case DHCP_OPTION_OVER:
			if (i + 1 + optionptr[i + OPT_LEN] >= length) {
				LOG(LOG_WARNING, "bogus packet, option fields too long.");
				return NULL;
			}
			over = optionptr[i + 3];
			i += optionptr[OPT_LEN] + 2;
			break;
		case DHCP_END:
			if (curr == OPTION_FIELD && over & FILE_FIELD) {
				optionptr = packet->file;
				i = 0;
				length = 128;
				curr = FILE_FIELD;
			} else if (curr == FILE_FIELD && over & SNAME_FIELD) {
				optionptr = packet->sname;
				i = 0;
				length = 64;
				curr = SNAME_FIELD;
			} else done = 1;
			break;
		default:
			i += optionptr[OPT_LEN + i] + 2;
		}
	}
	return NULL;
}

/* get an option length*/
int get_option_len(struct dhcpMessage *packet, int code)
{
	int i, length;
	uint8_t *optionptr;
	int over = 0, done = 0, curr = OPTION_FIELD;

	optionptr = packet->options;
	i = 0;
	length = 308;
	while (!done) {
		if (i >= length) {
			LOG(LOG_WARNING, "bogus packet, option fields too long.");
			return 0;
		}
		if (optionptr[i + OPT_CODE] == code) {
			if (i + 1 + optionptr[i + OPT_LEN] >= length) {
				LOG(LOG_WARNING, "bogus packet, option fields too long.");
				return 0;
			}
			return (int)optionptr[i + OPT_LEN];
		}
		switch (optionptr[i + OPT_CODE]) {
		case DHCP_PADDING:
			i++;
			break;
		case DHCP_OPTION_OVER:
			if (i + 1 + optionptr[i + OPT_LEN] >= length) {
				LOG(LOG_WARNING, "bogus packet, option fields too long.");
				return 0;
			}
			over = optionptr[i + 3];
			i += optionptr[OPT_LEN] + 2;
			break;
		case DHCP_END:
			if (curr == OPTION_FIELD && over & FILE_FIELD) {
				optionptr = packet->file;
				i = 0;
				length = 128;
				curr = FILE_FIELD;
			} else if (curr == FILE_FIELD && over & SNAME_FIELD) {
				optionptr = packet->sname;
				i = 0;
				length = 64;
				curr = SNAME_FIELD;
			} else done = 1;
			break;
		default:
			i += optionptr[OPT_LEN + i] + 2;
		}
	}
	return 0;
}


/* return the position of the 'end' option (no bounds checking) */
int end_option(uint8_t *optionptr)
{
	int i = 0;

	while (optionptr[i] != DHCP_END) {
		if (optionptr[i] == DHCP_PADDING) i++;
		else i += optionptr[i + OPT_LEN] + 2;
	}
	return i;
}


/* add an option string to the options (an option string contains an option code,
 * length, then data) */
int add_option_string(uint8_t *optionptr, uint8_t *string)
{
	int end = end_option(optionptr);

	/* end position + string length + option code/length + end option */
	if (end + string[OPT_LEN] + 2 + 1 >= 308) {
		LOG(LOG_ERR, "Option 0x%02x did not fit into the packet!", string[OPT_CODE]);
		return 0;
	}
	DEBUG(LOG_INFO, "adding option 0x%02x", string[OPT_CODE]);
	memcpy(optionptr + end, string, string[OPT_LEN] + 2);
	optionptr[end + string[OPT_LEN] + 2] = DHCP_END;
	return string[OPT_LEN] + 2;
}


/* add a one to four byte option to a packet */
int add_simple_option(uint8_t *optionptr, uint8_t code, uint32_t data)
{
	char length = 0;
	int i;
	uint8_t option[2 + 4];
	uint8_t *u8;
	uint16_t *u16;
	uint32_t *u32;
	uint32_t aligned;
	u8 = (uint8_t *) &aligned;
	u16 = (uint16_t *) &aligned;
	u32 = &aligned;

	for (i = 0; dhcp_options[i].code; i++)
		if (dhcp_options[i].code == code) {
			length = option_lengths[dhcp_options[i].flags & TYPE_MASK];
		}

	if (!length) {
		DEBUG(LOG_ERR, "Could not add option 0x%02x", code);
		return 0;
	}

	option[OPT_CODE] = code;
	option[OPT_LEN] = length;

	switch (length) {
		case 1: *u8 =  data; break;
		case 2: *u16 = data; break;
		case 4: *u32 = data; break;
	}
	memcpy(option + 2, &aligned, length);
	return add_option_string(optionptr, option);
}


/* find option 'code' in opt_list */
struct option_set *find_option(struct option_set *opt_list, char code)
{
	while (opt_list && opt_list->data[OPT_CODE] < code)
		opt_list = opt_list->next;

	if (opt_list && opt_list->data[OPT_CODE] == code) return opt_list;
	else return NULL;
}


/* add an option to the opt_list */
void attach_option(struct option_set **opt_list, struct dhcp_option *option, char *buffer, int length)
{
	struct option_set *existing, *new, **curr;

	/* add it to an existing option */
	if ((existing = find_option(*opt_list, option->code))) {
		DEBUG(LOG_INFO, "Attaching option %s to existing member of list", option->name);
		if (option->flags & OPTION_LIST) {
			if (existing->data[OPT_LEN] + length <= 255) {
				existing->data = realloc(existing->data,
						existing->data[OPT_LEN] + length + 2);
				memcpy(existing->data + existing->data[OPT_LEN] + 2, buffer, length);
				existing->data[OPT_LEN] += length;
			} /* else, ignore the data, we could put this in a second option in the future */
		} /* else, ignore the new data */
	} else {
		DEBUG(LOG_INFO, "Attaching option %s to list", option->name);

		/* make a new option */
		new = xmalloc(sizeof(struct option_set));
		new->data = xmalloc(length + 2);
		new->data[OPT_CODE] = option->code;
		new->data[OPT_LEN] = length;
		memcpy(new->data + 2, buffer, length);

		curr = opt_list;
		while (*curr && (*curr)->data[OPT_CODE] < option->code)
			curr = &(*curr)->next;

		new->next = *curr;
		*curr = new;
	}
}
#if defined(TCSUPPORT_CT_DHCPC_OPTION125SERVER)
void addVIOptionDebug(uint8_t *optionptr)
{
        uint8_t data[150] = {0};
        char tmp[128] = {0};
        char code[32] = {0};
        unsigned int num;

        tcapi_get("Cwmp_Entry", "option125Code", code);
        tcapi_get("Cwmp_Entry", "option125String", tmp);

        num =  str_to_uint(code, 4);

        if(0 != strcmp(tmp,"") && num != 0xffff)
        {

                /* option code + length(total length)*/
                LOG(LOG_ERR, "we are correct here!\n");
                data[0] = OPT_VENDOR_INFOR;
                data[1] = 4+1+strlen(tmp);

                *((unsigned int *)&data[2]) = num;
                data[6] = strlen(tmp);
                strcpy(&data[7], tmp);

                add_option_string(optionptr, data);
        }
        else
                LOG(LOG_ERR, "we are wrong here!\n");
}
#endif

#if defined (CWMP) && defined(TR111)
void addVIOption(uint8_t *optionptr)
{
	uint8_t oui_len;
	uint8_t sn_len;
	uint8_t pclass_len;
	uint8_t subopt_len;
	uint8_t total_len;
	char oui[8] = "";
	char sn[68] = "";
	char pclass[68] = "";
	uint8_t data[150];
	int i = 0;
	int r_val;
#ifdef RTCONFIG_TR069
	int nm_MAC[7] ;

	memset(oui, 0, sizeof(oui));
	memset(sn, 0, sizeof(sn));
	memset(pclass, 0, sizeof(pclass));
	memset(data, 0, sizeof(data));

	r_val= tcapi_get("Info_Ether", "mac", sn);
	if(r_val < 0) {
		strcpy(oui, "N/A");
		strcpy(sn, "N/A");
	}
	else
	{
       	sscanf(sn, "%02x:%02x:%02x:%02x:%02x:%02x", &nm_MAC[0], &nm_MAC[1], &nm_MAC[2], &nm_MAC[3], &nm_MAC[4], &nm_MAC[5]);
       	memset(oui, 0, sizeof(oui));
       	sprintf(oui, "%02X%02X%02X", nm_MAC[0], nm_MAC[1], nm_MAC[2]);
		memset(sn, 0, sizeof(sn));
		sprintf(sn, "%02X%02X%02X%02X%02X%02X", nm_MAC[0], nm_MAC[1], nm_MAC[2], nm_MAC[3], nm_MAC[4], nm_MAC[5]);
	}

	r_val = tcapi_get("SysInfo_Entry", "ProductName", pclass);
	if(r_val < 0)
		strcpy(pclass, "N/A");

#else
#if/*TCSUPPORT_TTNET*/ defined(TCSUPPORT_TTNET)	
	int nm_MAC[7] ;
#endif/*TCSUPPORT_TTNET*/
	memset(oui, 0, sizeof(oui));
	memset(sn, 0, sizeof(sn));
	memset(pclass, 0, sizeof(pclass));
	memset(data, 0, sizeof(data));
	
	r_val = tcapi_get("Cwmp_Entry", "ManufacturerOUI", oui);
	if(r_val < 0)
		strcpy(oui, "N/A");
	r_val = tcapi_get("SysInfo_Entry", "SerialNum", sn);
	if(r_val < 0)
		strcpy(sn, "N/A");
#ifdef TCSUPPORT_TTNET
	r_val= tcapi_get("Info_Ether", "mac", sn);
	if(r_val < 0)
		strcpy(sn, "N/A");
	else
       {
       	sscanf(sn, "%02x:%02x:%02x:%02x:%02x:%02x", &nm_MAC[0],&nm_MAC[1],&nm_MAC[2],&nm_MAC[3],&nm_MAC[4],&nm_MAC[5]);
		memset(sn, 0, sizeof(sn));
		sprintf(sn, "%02X%02X%02X%02X%02X%02X",  nm_MAC[0] , nm_MAC[1] , nm_MAC[2] , nm_MAC[3] , nm_MAC[4] , nm_MAC[5]);
	}
#endif
	r_val = tcapi_get("Cwmp_Entry", "ProductClass", pclass);
	if(r_val < 0)
		strcpy(pclass, "N/A");
#endif
		
	oui_len = strlen(oui);
	sn_len = strlen(sn);
	pclass_len = strlen(pclass);
	
	/* subopt length = (oui code+oui length+oui lenght) + (sn code+sn length+sn length) + (pclass code+pclass length+pclass length) */
	subopt_len = 6 + oui_len + sn_len + pclass_len;
	total_len = DSL_FORUM_IANA_NUMBER_LENGTH + 1 + subopt_len;
	
	/* option code + length(total length)*/
	data[i++] = OPT_VENDOR_INFOR;
	data[i++] = total_len;

	/*put DSL_FORUM_IANA_NUMBER*/
	data[i++] = DSL_FORUM_IANA_NUMBER>>24;
	data[i++] = DSL_FORUM_IANA_NUMBER>>16;
	data[i++] = DSL_FORUM_IANA_NUMBER>>8;
	data[i++] = DSL_FORUM_IANA_NUMBER;
	
	/* option code + length(suboption length) */
	data[i++] = subopt_len;
	
	/* For oui: option code + length + data */
	data[i++] = 0x4;
	data[i++] = oui_len;
	memcpy(&data[i], oui, oui_len);
	i += oui_len;
	
	/* For sn: option code + length + data */
	data[i++] = 0x5;
	data[i++] = sn_len;
	memcpy(&data[i], sn, sn_len);
	i += sn_len;

	/* For pclass: option code + length + data */
	data[i++] = 0x6;
	data[i++] = pclass_len;
	memcpy(&data[i], pclass, pclass_len);
	 
	add_option_string(optionptr, data);
}

#ifdef RTCONFIG_TR181
void addDeviceVIOption(uint8_t *optionptr)
{
	uint8_t oui_len;
	uint8_t sn_len;
	uint8_t pclass_len;
	uint8_t subopt_len;
	uint8_t total_len;
	char oui[8] = "";
	char sn[68] = "";
	char pclass[68] = "";
	uint8_t data[150];
	int i = 0;
	int r_val;
	int nm_MAC[7] ;

	memset(oui, 0, sizeof(oui));
	memset(sn, 0, sizeof(sn));
	memset(pclass, 0, sizeof(pclass));
	memset(data, 0, sizeof(data));

	r_val= tcapi_get("Info_Ether", "mac", sn);
	if(r_val < 0) {
		strcpy(oui, "N/A");
		strcpy(sn, "N/A");
	}
	else
	{
       	sscanf(sn, "%02x:%02x:%02x:%02x:%02x:%02x", &nm_MAC[0], &nm_MAC[1], &nm_MAC[2], &nm_MAC[3], &nm_MAC[4], &nm_MAC[5]);
       	memset(oui, 0, sizeof(oui));
       	sprintf(oui, "%02X%02X%02X", nm_MAC[0], nm_MAC[1], nm_MAC[2]);
		memset(sn, 0, sizeof(sn));
		sprintf(sn, "%02X%02X%02X%02X%02X%02X", nm_MAC[0], nm_MAC[1], nm_MAC[2], nm_MAC[3], nm_MAC[4], nm_MAC[5]);
	}

	r_val = tcapi_get("SysInfo_Entry", "ProductName", pclass);
	if(r_val < 0)
		strcpy(pclass, "N/A");
		
	oui_len = strlen(oui);
	sn_len = strlen(sn);
	pclass_len = strlen(pclass);
	
	/* subopt length = (oui code+oui length+oui lenght) + (sn code+sn length+sn length) + (pclass code+pclass length+pclass length) */
	subopt_len = 6 + oui_len + sn_len + pclass_len;
	total_len = DSL_FORUM_IANA_NUMBER_LENGTH + 1 + subopt_len;
	
	/* option code + length(total length)*/
	data[i++] = OPT_VENDOR_INFOR;
	data[i++] = total_len;

	/*put DSL_FORUM_IANA_NUMBER*/
	data[i++] = DSL_FORUM_IANA_NUMBER>>24;
	data[i++] = DSL_FORUM_IANA_NUMBER>>16;
	data[i++] = DSL_FORUM_IANA_NUMBER>>8;
	data[i++] = DSL_FORUM_IANA_NUMBER;
	
	/* option code + length(suboption length) */
	data[i++] = subopt_len;
	
	/* For oui: option code + length + data */
	data[i++] = 0x1;
	data[i++] = oui_len;
	memcpy(&data[i], oui, oui_len);
	i += oui_len;
	
	/* For sn: option code + length + data */
	data[i++] = 0x2;
	data[i++] = sn_len;
	memcpy(&data[i], sn, sn_len);
	i += sn_len;

	/* For pclass: option code + length + data */
	data[i++] = 0x3;
	data[i++] = pclass_len;
	memcpy(&data[i], pclass, pclass_len);
	 
	add_option_string(optionptr, data);
}
#endif
#endif

#ifdef DHCP_PROFILE
void addOPT240Option(uint8_t *optionptr)
{
	uint8_t data[150];
	int i = 0;
	char opt240Str[64];

	memset(opt240Str, 0, sizeof(opt240Str));

	sprintf(opt240Str, "%s", gOption240_Param.dhcp_opt240_value);
	/* For option 240: option code + length + data */
	data[OPT_CODE] = 240;
	data[OPT_LEN] = strlen(opt240Str);
	memcpy(&data[OPT_DATA], opt240Str, strlen(opt240Str));

	add_option_string(optionptr, data);
}
#endif
