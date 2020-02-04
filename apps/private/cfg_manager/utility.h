/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
 * All Rights Reserved.
 *
 * TrendChip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of TrendChip Technologies, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * TrendChip Technologies, Corp.
 */
/******************************************************************************/
#ifndef _UTILITY_H
#define _UTILITY_H
#include "cfg_manager.h"
#include <arpa/inet.h>

#define LOAD_DEFAULT_CHAIN "/usr/script/filter_forward_start.sh"
#define FLUSH_DEFAULT_CHAIN "/usr/script/filter_forward_stop.sh"

#define PREROUTING_WAN_CMD_FORMAT_WRT "-A PREROUTING -p %s --dport %s:%s -j TRIGGER --trigger-type dnat\n"
#define PREROUTING_RULE_CMD_FORMAT_WRT "-A PREROUTING -i br0 -p %s --dport %s:%s -j TRIGGER --trigger-type out --trigger-proto %s --trigger-match %s-%s --trigger-relate %s-%s\n"
#define FORWARD_WAN_CMD_FORMAT_WRT "-A FORWARD -p %s --dport %s:%s -j TRIGGER --trigger-type in\n"

/*krammer add 2008 11 12*/
#define MAXGET_PROFILE_SIZE 128

#define TOOLS_BUF_LEN 1024

#define IPTABLES_FILTER 0
#define IPTABLES_NAT 1
#define IPTABLES_MANGLE 2

#define wan_prefix(unit, prefix) snprintf(prefix, sizeof(prefix), "wan%d_", unit)

//for Parental Control
/* Copy each token in wordlist delimited by ascii_60 into word */
#define foreach_60(word, wordlist, next) \
	for (next = &wordlist[strspn(wordlist, "<")], \
		strncpy(word, next, sizeof(word)), \
		word[strcspn(word, "<")] = '\0', \
		word[sizeof(word) - 1] = '\0', \
		next = strchr(next, '<'); \
		strlen(word); \
		next = next ? &next[strspn(next, "<")] : "", \
		strncpy(word, next, sizeof(word)), \
		word[strcspn(word, "<")] = '\0', \
		word[sizeof(word) - 1] = '\0', \
		next = strchr(next, '<'))

/* Copy each token in wordlist delimited by ascii_62 into word */
#define foreach_62(word, wordlist, next) \
	for (next = &wordlist[strspn(wordlist, ">")], \
		strncpy(word, next, sizeof(word)), \
		word[strcspn(word, ">")] = '\0', \
		word[sizeof(word) - 1] = '\0', \
		next = strchr(next, '>'); \
		strlen(word); \
		next = next ? &next[strspn(next, ">")] : "", \
		strncpy(word, next, sizeof(word)), \
		word[strcspn(word, ">")] = '\0', \
		word[sizeof(word) - 1] = '\0', \
		next = strchr(next, '>'))

//for Network Services Filter (IP Filter)
/* Copy each token in wordlist delimited by space into word */
#define foreach(word, wordlist, next) \
	for (next = &wordlist[strspn(wordlist, " ")], \
	     strncpy(word, next, sizeof(word)), \
	     word[strcspn(word, " ")] = '\0', \
	     word[sizeof(word) - 1] = '\0', \
	     next = strchr(next, ' '); \
	     strlen(word); \
	     next = next ? &next[strspn(next, " ")] : "", \
	     strncpy(word, next, sizeof(word)), \
	     word[strcspn(word, " ")] = '\0', \
	     word[sizeof(word) - 1] = '\0', \
	     next = strchr(next, ' '))

void
splitName(char *name, char buf[][MAX_NODE_NAME], const char* delim);

void
mergeNodeName(mxml_node_t *top, char name[][MAX_NODE_NAME]);

int
getAttrValue(mxml_node_t *top, char name[][MAX_NODE_NAME], char* attr, char* buf);

int
setAttrValue(mxml_node_t *top, char name[][MAX_NODE_NAME], char *attr, char *value);

void
fileRead(char *path, char *buf, int size);

void
kill_process(char *pid_path);

int
get_profile_str(char *keyname,char *str_return, int size, int type, char *path);

int
check_nat_enable(mxml_node_t *top, int state);

#if 0
int
check_filter_state(mxml_node_t *top, int filter_state);
#endif
unsigned int
check_and_set_filter(unsigned int new_filter_state);
/*rodney_20090420 add*/
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
int check_ip_exist(char *interface, int pvc, int isp, int check_flag, int version, int input_serv);
mxml_node_t * getNodeP(mxml_node_t *top,  char name[][MAX_NODE_NAME]);
int get_entry_serv_number(char *buffer, char *keyword, int *number, int *serv);
#else
int
check_ip_exist(char *interface, int pvc, int isp, int check_flag, int version);
#endif
int isMultiSerPVC(int pvc);

int get_ip_by_interface(char *iface, unsigned int *addr);

/**********20080826 racing add************/
int
acl_get_active_interface(mxml_node_t *top, char interfaces[][16], const char* intf_type);

char*
string_tolower(char *string);

int
check_ip_format(char *ip_arg);

int
acl_check_ip_format(char *ip_arg);

int
acl_check_ip_range(char *ip_begin_arg, char *ip_end_arg);
/**********20080826 racing add************/

/**********20081006 racing add************/
int
check_mask_format(char *mask_arg, char *mask_decimal);

int
check_mac_format(const char *mac_arg);

int
get_interface_by_PVC(mxml_node_t *top, const char* intf_tpye, char *interface);

int
ipfilter_check_active(mxml_node_t *top);
/**********20081006 racing add************/

/*20081022 krammer add*/
#if 0
int
check_l7filter_state(mxml_node_t *top, int l7filter_state);
#endif
void
check_and_set_l7filter(unsigned int new_filter_state);
/*20081022 krammer add*/
#if 0
/*20081228 pork add*/
void trim(char *oriStr,char *retStr);
/*20081228 pork add*/
#endif
/*20081228 pork add*/
void quotHandle(char *oriStr);
void specialCharacterHandle(char *oriStr);
/*20080123 pork add*/
//void
//setNewFilterState(mxml_node_t *top,int (*conditionFunction)(mxml_node_t *top),char vector,char *newFilterState);
int
get_entry_number(char *buffer, char *keyword, int *number);
int doValPut(char *path, char *value);
/*for check_crc32. shnwind add.*/

unsigned long calculate_crc32(int imagefd, long len);
#define CHECKSUM_TEMP_BUF_SIZE 4096

enum if_addr_type{
	inet4 = 0,
	inet6_linklocal,
	inet6_global,
};

int get_if_addr(char *iface, int type, char *re_addr,  int re_size);

#ifdef TCSUPPORT_IPV6_ADVANCE
#define IPV6_NF_PATH "ipv6/netfilter"
enum modules_ref_type{
	IP6_FIREWALL = 0,
	IP6_ACL,
};

struct modules_info{
	char name[32];
	char path[32];
	int is_insert;
	int ref_flag;	//refer to modules_ref_type
};
#endif

#ifdef TCSUPPORT_IPV6_FIREWALL
void insert_ipv6_firewall_modules();
void remove_ipv6_firewall_modules();
#endif


int 
doSystem(const char *format, ...);
mxml_node_t *get_node_top();


/* function used for get ipv4 and ipv6 address by iface name */
#ifndef IFNAMSIZ
#define IFNAMSIZ  16
#endif
#define PROC_IFINET6_PATH   "/proc/net/if_inet6"
#define INET6_ADDRSTRLEN	64
int getifaddr_ipv4(char *ifname, struct in_addr *ifaddr);
int getifaddr_ipv6(char *ifname, struct in6_addr *ifaddr, int type);
int getifaddr(int family, char *ifname, void *ifaddr, int type);

//porting from ASUS WRT by Javier Su
#define MIN_DAY 1
#define MAX_DAY 7
#define MIN_HOUR 0
#define MAX_HOUR 23

typedef struct pc_event pc_event_s;
struct pc_event{
	char e_name[32];
	int start_day;
	int end_day;
	int start_hour;
	int end_hour;
	pc_event_s *next;
};

typedef struct pc pc_s;
struct pc{
	int enabled;
	char device[32];
	char mac[18];
	pc_event_s *events;
	pc_s *next;
};

void config_daytime_string(FILE *fp);

#define	IPT_V4	0x01
#define	IPT_V6	0x02
#define	IPT_ANY_AF	(IPT_V4 | IPT_V6)
#define	IPT_AF_IS_EMPTY(f)	((f & IPT_ANY_AF) == 0)

#ifdef RTCONFIG_IPV6
extern int get_ipv6_service(void);
#define ipv6_enabled()	(get_ipv6_service() != IPV6_DISABLED)
extern const char *ipv6_router_address(struct in6_addr *in6addr);
#else
#define ipv6_enabled()	(0)
#endif

extern int EZgetAttrValue_UpdateType(char *node1, char *node2, char *attribute,char *buf, int buflen);
extern int EZgetAttrValue(char *node1, char *node2, char *attribute,char *buf);
extern int EZsetAttrValue(char *node1, char *node2, char *attribute,char *buf);
extern int EZgetAttrValue2(char *node1, char *node2, char *node3, char *attribute,char *buf);
extern int EZsetAttrValue2(char *node1, char *node2, char *node3, char *attribute,char *buf);
extern int EZmatch(char *node1, char *node2, char *attribute,char *buf, char *matchString);
extern int EZmatch2(char *node1, char *node2, char *node3, char *attribute,char *buf, char *matchString);
extern int EZgetInt(char *node, char *entry, char *attribute);
extern char* EZgetString(char *node, char *entry, char *attribute, char *buffer);
extern int EZget_productid(char *buf, int buflen);

#define vstrsep(buf, sep, args...) _vstrsep(buf, sep, args, NULL)
extern int _vstrsep(char *buf, const char *sep, ...);

/*
 * Concatenate two strings together into a caller supplied buffer
 * @param	s1	first string
 * @param	s2	second string
 * @param	buf	buffer large enough to hold both strings
 * @return	buf
 */
static inline char * strcat_r(const char *s1, const char *s2, char *buf)
{
	strcpy(buf, s1);
	strcat(buf, s2);
	return buf;
}

void start_default_filter( void ); //Ren
int start_firewall(void); //Ren
void route_setting(void);

extern unsigned char get_rand();
extern unsigned long readFileSize( char *filepath );
extern int encryptRomfile(char *src, char *dst, char *productName);
extern int decryptRomfile( char *path, unsigned int *length, unsigned int offset ); //Ren
extern int detect_endianness(void);
extern int encryptBinaryfile(char *src, char *dst, char *productName);
extern int timerTrigger(unsigned int seconds, void (*func)(int signo)); //Ren
extern int timerTrigger_re(unsigned int first_seconds, unsigned int interval, void (*func)(int signo)); //Ren
extern int modifyXDSLscript(void); //Ren
extern unsigned int getMTU(void); //Ren
extern char *getLANIPAddr(char *buffer); //Ren
extern void dbgprintf (const char * format, ...); //Ren
extern int host_addr_info(const char *name, int af, struct sockaddr_storage *buf); //Ren
extern int host_addrtypes(const char *name, int af); //Ren
extern int ipt_addr_compact(const char *s, int af, int strict); //Ren
extern int run_app_script2(const char *pkg_name, const char *pkg_action); //Ren
extern char *trimWS(char *str);
extern char *trimWS_L(char *str);
extern char *trimNL(char *str);
int wan_primary_ifunit(void);
int wan_secondary_ifunit(void);
int copy_node_all_attribute(mxml_node_t* top, char* from, char* to);
int if_down(char* ifname);
void stop_all_wan(void);
typedef enum{
	IFID_BR0 = 0,
	IFID_WL24G,
	IFID_WL5G
}IFID;

enum{
	ENDIAN_UNKNOWN = -1,
	ENDIAN_LITTLE = 0,
	ENDIAN_BIG = 1
};

int getMacAddrWithoutColon(const IFID ifid, char *mac);
int convertMacToRandSeed(const char *mac);
int is_wan_connect(int unit);

#define eval(cmd, args...) ({ \
	char *argv[] = { cmd, ## args, NULL }; \
	_eval(argv, NULL, 0, NULL); \
})

extern int _ifconfig(const char *name, int flags, const char *addr, const char *netmask, const char *dstaddr);
#define ifconfig(name, flags, addr, netmask) _ifconfig(name, flags, addr, netmask, NULL)
#define IFUP (IFF_UP | IFF_RUNNING | IFF_BROADCAST | IFF_MULTICAST)
#define adjustEndian(num) do{num = (((num)>>24) & 0x000000FF) | (((num)<<8) & 0x00FF0000) | (((num)>>8) & 0x0000FF00) | (((num)<<24) & 0xFF000000);}while(0)

extern void reset_switch_phy();
#ifdef ASUS_BUSYBOX_NEW
extern int bin2hex(char *dest, size_t size, const void *src, size_t n);
extern int addOpt2Str(char *dest, size_t size, const unsigned char id, const char *opt_val, const unsigned char len);
#endif


#define MAX_BRIDGE_MEMBER_CNT		32
#define MAX_INTERFACE_NAME_LEN	8

typedef struct BridgeMember
{
	char member[MAX_BRIDGE_MEMBER_CNT][MAX_INTERFACE_NAME_LEN];
}BRIDGE_MEMBER;

extern int getMemberOfBridge(const char *brif, BRIDGE_MEMBER *member);

extern void adjust_hw_nat();
extern char* get_mailserver(char *buffer, size_t buffer_size);
extern char* get_email(char *buffer, size_t buffer_size);
extern char* get_pwd(char *buffer, size_t buffer_size);
extern int get_auth_data(email_auth_data_t* authdata);
extern char *get_wan_primary_ifname(char *ifname);
extern char *get_wan_secondary_ifname(char *ifname);
extern char *get_first_connected_public_wan_ifname(char * ifname);
extern int is_private_subnet(const char * ip);
#endif
