/*
 * files.c -- DHCP server file manipulation *
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <netdb.h>

#include <netinet/ether.h>
#include "static_leases.h"

#include "dhcpd.h"
#include "files.h"
#include "options.h"
#include "common.h"
#include "../../../lib/libtcapi.h"

//record total lease number and decide if send cwmp msg
int leaseNumber;
/*
 * Domain names may have 254 chars, and string options can be 254
 * chars long. However, 80 bytes will be enough for most, and won't
 * hog up memory. If you have a special application, change it
 */
#define READ_CONFIG_BUF_SIZE 80

/* on these functions, make sure you datatype matches */
static int read_ip(const char *line, void *arg)
{
	struct in_addr *addr = arg;
	struct hostent *host;
	int retval = 1;

	if (!inet_aton(line, addr)) {
		if ((host = gethostbyname(line)))
			addr->s_addr = *((unsigned long *) host->h_addr_list[0]);
		else retval = 0;
	}
	return retval;
}

static int read_mac(const char *line, void *arg)
{
	uint8_t *mac_bytes = arg;
	struct ether_addr *temp_ether_addr;
	int retval = 1;

	temp_ether_addr = ether_aton(line);

	if(temp_ether_addr == NULL)
		retval = 0;
	else
		memcpy(mac_bytes, temp_ether_addr, 6);

	return retval;
}


static int read_str(const char *line, void *arg)
{
	char **dest = arg;

	if (*dest) free(*dest);
	*dest = strdup(line);

	return 1;
}


static int read_u32(const char *line, void *arg)
{
	uint32_t *dest = arg;
	char *endptr;
	*dest = strtoul(line, &endptr, 0);
	return endptr[0] == '\0';
}


static int read_yn(const char *line, void *arg)
{
	char *dest = arg;
	int retval = 1;

	if (!strcasecmp("yes", line))
		*dest = 1;
	else if (!strcasecmp("no", line))
		*dest = 0;
	else retval = 0;

	return retval;
}


/* read a dhcp option and add it to opt_list */
static int read_opt(const char *const_line, void *arg)
{
	struct option_set **opt_list = arg;
	char *opt, *val, *endptr;
	struct dhcp_option *option;
	int retval = 0, length;
	char buffer[8];
	char *line;
	uint16_t *result_u16 = (uint16_t *) buffer;
	uint32_t *result_u32 = (uint32_t *) buffer;

	/* Cheat, the only const line we'll actually get is "" */
	line = (char *) const_line;
	if (!(opt = strtok(line, " \t="))) return 0;

	for (option = dhcp_options; option->code; option++)
		if (!strcasecmp(option->name, opt))
			break;

	if (!option->code) return 0;

	do {
		if (!(val = strtok(NULL, ", \t"))) break;
		length = option_lengths[option->flags & TYPE_MASK];
		retval = 0;
		opt = buffer; /* new meaning for variable opt */
		switch (option->flags & TYPE_MASK) {
		case OPTION_IP:
			retval = read_ip(val, buffer);
			break;
		case OPTION_IP_PAIR:
			retval = read_ip(val, buffer);
			if (!(val = strtok(NULL, ", \t/-"))) retval = 0;
			if (retval) retval = read_ip(val, buffer + 4);
			break;
		case OPTION_STRING:
			length = strlen(val);
			if (length > 0) {
				if (length > 254) length = 254;
				opt = val;
				retval = 1;
			}
			break;
		case OPTION_BOOLEAN:
			retval = read_yn(val, buffer);
			break;
		case OPTION_U8:
			buffer[0] = strtoul(val, &endptr, 0);
			retval = (endptr[0] == '\0');
			break;
		case OPTION_U16:
			*result_u16 = htons(strtoul(val, &endptr, 0));
			retval = (endptr[0] == '\0');
			break;
		case OPTION_S16:
			*result_u16 = htons(strtol(val, &endptr, 0));
			retval = (endptr[0] == '\0');
			break;
		case OPTION_U32:
			*result_u32 = htonl(strtoul(val, &endptr, 0));
			retval = (endptr[0] == '\0');
			break;
		case OPTION_S32:
			*result_u32 = htonl(strtol(val, &endptr, 0));
			retval = (endptr[0] == '\0');
			break;
		default:
			break;
		}
		if (retval)
			attach_option(opt_list, option, opt, length);
	} while (retval && option->flags & OPTION_LIST);
	return retval;
}

static int read_staticlease(const char *const_line, void *arg)
{

	char *line;
	char *mac_string;
	char *ip_string;
	uint8_t *mac_bytes;
	uint32_t *ip;


	/* Allocate memory for addresses */
	mac_bytes = xmalloc(sizeof(unsigned char) * 8);
	ip = xmalloc(sizeof(uint32_t));

	/* Read mac */
	line = (char *) const_line;
	mac_string = strtok(line, " \t");
	read_mac(mac_string, mac_bytes);

	/* Read ip */
	ip_string = strtok(NULL, " \t");
	read_ip(ip_string, ip);

	addStaticLease(arg, mac_bytes, ip);

#ifdef UDHCP_DEBUG
	printStaticLeases(arg);
#endif

	return 1;

}


static const struct config_keyword keywords[] = {
	/* keyword	handler   variable address		default */
	{"start",	read_ip,  &(dhcp_config.start),	"192.168.0.20"},
	{"end",		read_ip,  &(dhcp_config.end),		"192.168.0.254"},
	{"interface",	read_str, &(dhcp_config.interface),	"eth0"},
	{"option",	read_opt, &(dhcp_config.options),	""},
	{"opt",		read_opt, &(dhcp_config.options),	""},
	{"max_leases",	read_u32, &(dhcp_config.max_leases),	"254"},
	{"remaining",	read_yn,  &(dhcp_config.remaining),	"yes"},
	{"auto_time",	read_u32, &(dhcp_config.auto_time),	"7200"},
	{"decline_time",read_u32, &(dhcp_config.decline_time),"3600"},
	{"conflict_time",read_u32,&(dhcp_config.conflict_time),"3600"},
	{"offer_time",	read_u32, &(dhcp_config.offer_time),	"60"},
	{"min_lease",	read_u32, &(dhcp_config.min_lease),	"60"},
	{"lease_file",	read_str, &(dhcp_config.lease_file),	LEASES_FILE},
	{"pidfile",	read_str, &(dhcp_config.pidfile),	"/var/run/udhcpd.pid"},
	{"notify_file", read_str, &(dhcp_config.notify_file),	""},
	{"siaddr",	read_ip,  &(dhcp_config.siaddr),	"0.0.0.0"},
	{"sname",	read_str, &(dhcp_config.sname),	""},
	{"boot_file",	read_str, &(dhcp_config.boot_file),	""},
	{"static_lease",read_staticlease, &(dhcp_config.static_leases),	""},
	/*ADDME: static lease */
	{"",		NULL, 	  NULL,				""}
};

#ifdef DHCP_PROFILE
static const struct config_keyword option_keywords[] = {
	/* keyword	handler   variable address		default */
	{"start",	read_ip,  &(option60_config.start),	"192.168.0.20"},
	{"end",		read_ip,  &(option60_config.end),		"192.168.0.254"},
	{"interface",	read_str, &(option60_config.interface),	"eth0"},
	{"option",	read_opt, &(option60_config.options),	""},
	{"opt",		read_opt, &(option60_config.options),	""},
	{"max_leases",	read_u32, &(option60_config.max_leases),	"5"},
	{"remaining",	read_yn,  &(option60_config.remaining),	"yes"},
	{"auto_time",	read_u32, &(option60_config.auto_time),	"7200"},
	{"decline_time",read_u32, &(option60_config.decline_time),"3600"},
	{"conflict_time",read_u32,&(option60_config.conflict_time),"3600"},
	{"offer_time",	read_u32, &(option60_config.offer_time),	"60"},
	{"min_lease",	read_u32, &(option60_config.min_lease),	"60"},
	{"lease_file",	read_str, &(option60_config.lease_file),	LEASES_FILE},
	{"pidfile",	read_str, &(option60_config.pidfile),	"/var/run/udhcpd.pid"},
	{"notify_file", read_str, &(option60_config.notify_file),	""},
	{"siaddr",	read_ip,  &(option60_config.siaddr),	"0.0.0.0"},
	{"sname",	read_str, &(option60_config.sname),	""},
	{"boot_file",	read_str, &(option60_config.boot_file),	""},
	{"static_lease",read_staticlease, &(option60_config.static_leases),	""},

	{"enable",read_yn, &(gOption60_Param.enable),	"1"},
	{"vendorClassID",read_str, &(gOption60_Param.dhcp_opt60_vid), "MSFT 5.0"},

	{"opt240Enable",read_yn, &(gOption240_Param.enable),	"1"},
	{"opt240Value",read_str, &(gOption240_Param.dhcp_opt240_value), "This is option 240"},
	/*ADDME: static lease */
	{"",		NULL, 	  NULL,				""}
};
#endif

int read_config(const char *file)
{
	FILE *in;
	char buffer[READ_CONFIG_BUF_SIZE], *token, *line;
#ifdef UDHCP_DEBUG
	char orig[READ_CONFIG_BUF_SIZE];
#endif
	int i, lm = 0;

	static const  struct config_keyword *pKeywords = NULL;
	
	if( file == NULL )
		return -1;
	
	if(strcmp(file, DHCPD_CONF_FILE) == 0)
		pKeywords = keywords;
#ifdef DHCP_PROFILE
	else if(strcmp(file, DHCPD_OPTION_CONF_FILE) == 0)
		pKeywords = option_keywords;

#endif
	else
		return -1;

	for (i = 0; pKeywords[i].keyword[0]; i++)
		if (pKeywords[i].def[0])
			pKeywords[i].handler(pKeywords[i].def, pKeywords[i].var);


	if (!(in = fopen(file, "r"))) {
		LOG(LOG_ERR, "unable to open config file: %s", file);
		return 0;
	}

	while (fgets(buffer, READ_CONFIG_BUF_SIZE, in)) {
		lm++;
		if (strchr(buffer, '\n')) *(strchr(buffer, '\n')) = '\0';
#ifdef UDHCP_DEBUG
		strcpy(orig, buffer);
#endif
		if (strchr(buffer, '#')) *(strchr(buffer, '#')) = '\0';

		if (!(token = strtok(buffer, " \t"))) continue;
		if (!(line = strtok(NULL, ""))) continue;

		/* eat leading whitespace */
		line = line + strspn(line, " \t=");
		/* eat trailing whitespace */
		for (i = strlen(line); i > 0 && isspace(line[i - 1]); i--);
		line[i] = '\0';

		for (i = 0; pKeywords[i].keyword[0]; i++)
			if (!strcasecmp(token, pKeywords[i].keyword))
				if (!pKeywords[i].handler(line, pKeywords[i].var)) {
					LOG(LOG_ERR, "Failure parsing line %d of %s", lm, file);
					DEBUG(LOG_ERR, "unable to parse '%s'", orig);
					/* reset back to the default value */
					pKeywords[i].handler(pKeywords[i].def, pKeywords[i].var);
				}
	}
	fclose(in);
	return 1;
}


void write_leases(void)
{
#if !defined(TCSUPPORT_CT) 
	/* use write_leases_enhance replace write_leases, in order to show human-readable leases */
	write_leases_enhance(NULL);
#endif
}

void write_leases_enhance(struct dhcpOfferedAddr *lease)
{
	FILE *fp;
	unsigned int i;
	char buf[255] = {0};
	unsigned long tmp_time, expires;
	char mac_addr[17];
	char ip_addr[16];
	struct in_addr addr;
	int currentLeaseNumer = 0;

	if (!(fp = fopen(LEASE_PATH, "w"))) {
		LOG(LOG_ERR, "Unable to open %s for writing", LEASE_PATH);
		return;
	}

	for (i = 0; i < gTotalMaxLeases; i++) {
		
		if((lease!=NULL) && !memcmp(leases[i].chaddr,lease->chaddr,16)){
			leases[i].expires = lease->expires;
		}

		if (leases[i].yiaddr != 0) {
			/* screw with the time in the struct, for easier writing */
			tmp_time = leases[i].expires;
			
			if (server_config.remaining) {
				if (lease_expired(&(leases[i]))){
					leases[i].expires = 0;
					continue;
				}
				//else leases[i].expires -= curr;
			} /* else stick with the time we got */
			
			sprintf(mac_addr,"%02X:%02X:%02X:%02X:%02X:%02X",leases[i].chaddr[0],leases[i].chaddr[1],leases[i].chaddr[2],leases[i].chaddr[3],leases[i].chaddr[4],leases[i].chaddr[5]);
			
			addr.s_addr = leases[i].yiaddr;
			sprintf(ip_addr,"%s", inet_ntoa(addr));
			expires = leases[i].expires;			
					
			sprintf(buf, "%s %s %ld %s\n", mac_addr, ip_addr, expires, leases[i].hostname);
			
			fwrite(buf, sizeof(char), strlen(buf), fp);
			currentLeaseNumer++;

			/* Then restore it when done. */
			leases[i].expires = tmp_time;
		}
	}
	fclose(fp);

	if(leaseNumber != currentLeaseNumer){
		LOG(LOG_INFO, "\n i will reinit host.");
		leaseNumber = currentLeaseNumer;
		#ifdef CWMP
		sendCwmpMsg(3); //HOST_REINIT
		#endif
	}
}


void read_leases(const char *file)
{
#if !defined(TCSUPPORT_CT) 
	FILE *fp;
	unsigned int i = 0, j = 0;
	struct dhcpOfferedAddr lease;
	struct  timespec  curr;
	char buf[80];
	char ip_buf[20];
	int mac[6];
	
	memset(&lease,0,sizeof(struct dhcpOfferedAddr));
	/* suppose that dhcp server restarted, it need read leases from a file that record
	 * the leases. In dhcpd config file, the file is /etc/udhcpd.lease;
	 * but dhcp server actually write the record to /etc/udhcp_lease;
	 * so we need copy /etc/udhcp_lease to udhcpd.lease
	 * Why don't we read /etc/udhcp_lease directly? because someone may write udhcp_lease when 
	 * we are reading this file, please refer to function add_lease()
	 * Leto.liu@20110316
	 * */
	if((fp = fopen(LEASE_PATH, "r")) != NULL){
		fclose(fp);
		snprintf(buf,sizeof(buf), "/bin/cp %s %s", LEASE_PATH, file);
		system(buf);
	}
	
	if (!(fp = fopen(file, "r"))) {
		LOG(LOG_ERR, "Unable to open %s for reading", file);
		return;
	}

	while (i < gTotalMaxLeases && fgets(buf, sizeof(buf)-1, fp)) {
		sscanf(buf,"%02X:%02X:%02X:%02X:%02X:%02X %s %u %s", &(mac[0]),&(mac[1]),&(mac[2]),&(mac[3]),&(mac[4]),&(mac[5]), ip_buf, &(lease.expires), lease.hostname);
		for(j=0;j<6;j++){
			lease.chaddr[j] = (uint8_t)mac[j];
		}
		inet_aton(ip_buf, &(lease.yiaddr));
		/* ADDME: is it a static lease */
#ifdef DHCP_PROFILE
		if ((lease.yiaddr >= dhcp_config.start && lease.yiaddr <= dhcp_config.end)
		|| (lease.yiaddr >= option60_config.start && lease.yiaddr <= option60_config.end))

#else
		if (lease.yiaddr >= dhcp_config.start && lease.yiaddr <= dhcp_config.end)
#endif
 		{
			lease.expires = ntohl(lease.expires);
			
			/* match witch write_leases_enhance
			 * it directly plus current time no matter how dhcp_config.remaining is*/
			//if (!dhcp_config.remaining){
				clock_gettime(CLOCK_MONOTONIC, &curr);
				lease.expires -= curr.tv_sec;
				//lease.expires -= time(0);
			//}
			if (!(add_lease(lease.chaddr, lease.yiaddr, lease.expires, lease.hostname))) {
				LOG(LOG_WARNING, "Too many leases while loading %s\n", file);
				break;
			}
			i++;
		}
	}
	DEBUG(LOG_INFO, "Read %d leases", i);
	fclose(fp);
#endif
}

void sendCwmpMsg(int msgType)
{
/* we will send 3 kinds of msg
  * 2: VALUE_CHANGED
  * 3: HOST_REINIT
  * 4: DEVICE_REINIT
  */
	int ret;
	long type = 1;
	int flag = 0;
	
	cwmp_msg_t message;
	memset(&message,0,sizeof(cwmp_msg_t));
	message.cwmptype = msgType;
	ret = sendmegq(type,&message,flag);
	if(ret < 0)
	{
		tcdbg_printf("\r\nsend message error!!");
		return;
	}
	return;
}
