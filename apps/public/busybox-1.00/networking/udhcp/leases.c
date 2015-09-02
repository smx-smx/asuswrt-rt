/*
 * leases.c -- tools to manage DHCP leases
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 */

#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "dhcpd.h"
#include "files.h"
#include "options.h"
#include "leases.h"
#include "arpping.h"
#include "common.h"

#include "static_leases.h"


uint8_t blank_chaddr[] = {[0 ... 15] = 0};

/* clear every lease out that chaddr OR yiaddr matches and is nonzero */
void clear_lease(uint8_t *chaddr, uint32_t yiaddr)
{
	unsigned int i, j;

	for (j = 0; j < 16 && !chaddr[j]; j++);

	for (i = 0; i < gTotalMaxLeases; i++) 
		if ((j != 16 && !memcmp(leases[i].chaddr, chaddr, 16)) ||
		    (yiaddr && leases[i].yiaddr == yiaddr)) {
			memset(&(leases[i]), 0, sizeof(struct dhcpOfferedAddr));
		}
}


/* add a lease into the table, clearing out any old ones */
struct dhcpOfferedAddr *add_lease(uint8_t *chaddr, uint32_t yiaddr, unsigned long lease, char *hostname)
{
	struct dhcpOfferedAddr *oldest;
	struct  timespec  curtime;

	/* clean out any old ones */
	clear_lease(chaddr, yiaddr);

	oldest = oldest_expired_lease();

	if (oldest) {
		memcpy(oldest->chaddr, chaddr, 16);
		oldest->yiaddr = yiaddr;
		clock_gettime(CLOCK_MONOTONIC, &curtime);
		oldest->expires =curtime.tv_sec + lease;
		//oldest->expires = time(0) + lease;
		if(hostname){			
			strncpy(oldest->hostname, hostname, HOST_NAME_LEN);
		}		
	}
	/*For web page show,Added by liujilin_20100111*/
	/* it should not write lease here,
	 * because add_lease will be called by SendOffer and read_leases, etc,
	 * remove it to sendACK()
	 * Leto.liu@20110317 */
	//write_leases_enhance();

	return oldest;
}


/* true if a lease has expired */
int lease_expired(struct dhcpOfferedAddr *lease)
{
	struct  timespec  curtime;
	clock_gettime(CLOCK_MONOTONIC, &curtime);
	/* see from code, the DHCP LEASE has been done before the following code,
         * but the curtime maybe same as the lease time because CPU is very fast
         * so "<" must be instead of "<="
         * Shelven.Lu @ 20110715 */
	return (lease->expires <= (unsigned long) curtime.tv_sec);
	//return (lease->expires < (unsigned long) time(0));
}


/* Find the oldest expired lease, NULL if there are no expired leases */
struct dhcpOfferedAddr *oldest_expired_lease(void)
{
	struct dhcpOfferedAddr *oldest = NULL;
	//unsigned long oldest_lease = time(0);
	unsigned int i;
	struct  timespec  curtime;
	unsigned long oldest_lease;

	clock_gettime(CLOCK_MONOTONIC, &curtime);
	oldest_lease =(unsigned long)curtime.tv_sec;
		
	for (i = 0; i < gTotalMaxLeases; i++) 
		if (oldest_lease > leases[i].expires) {
			oldest_lease = leases[i].expires;
			oldest = &(leases[i]);
		}
	return oldest;

}


/* Find the first lease that matches chaddr, NULL if no match */
struct dhcpOfferedAddr *find_lease_by_chaddr(uint8_t *chaddr)
{
	unsigned int i;

	for (i = 0; i < gTotalMaxLeases; i++) 
		if (!memcmp(leases[i].chaddr, chaddr, 16)) return &(leases[i]);

	return NULL;
}


/* Find the first lease that matches yiaddr, NULL is no match */
struct dhcpOfferedAddr *find_lease_by_yiaddr(uint32_t yiaddr)
{
	unsigned int i;

	for (i = 0; i < gTotalMaxLeases; i++) 
		if (leases[i].yiaddr == yiaddr) return &(leases[i]);

	return NULL;
}


/* check is an IP is taken, if it is, add it to the lease table */
static int check_ip(uint32_t addr)
{
	struct in_addr temp;

	if (arpping(addr, server_config.server, server_config.arp, server_config.interface) == 0) {
		temp.s_addr = addr;
		LOG(LOG_INFO, "%s belongs to someone, reserving it for %ld seconds",
			inet_ntoa(temp), server_config.conflict_time);
		add_lease(blank_chaddr, addr, server_config.conflict_time, NULL);
		return 1;
	} else return 0;
}


/* find an assignable address, it check_expired is true, we check all the expired leases as well.
 * Maybe this should try expired leases by age... */
uint32_t find_address(int check_expired)
{
	uint32_t addr, ret;
	struct dhcpOfferedAddr *lease = NULL;

	char lanIp[64] = {0};
	#if defined(ALIAS_IP)
    char lanAliasIP[64] = {0};
	char tempActive[8]={0};
	int Flag = 0;
	#endif
	struct in_addr inIp;

	tcapi_get("Lan_Entry0","IP",lanIp);
	#if defined(ALIAS_IP)
	tcapi_get("LanAlias_Entry0","Active",tempActive);
	if( strcmp(tempActive,"Yes")==0 ){
		tcapi_get("LanAlias_Entry0","IP",lanAliasIP);
		Flag = 1;
	}
	else{
		Flag = 0;
	}
	#endif
	
	addr = ntohl(server_config.start); /* addr is in host order here */
	for (;addr <= ntohl(server_config.end); addr++) {

		/* ie, 192.168.55.0 */
		if (!(addr & 0xFF)) continue;

		/* ie, 192.168.55.255 */
		if ((addr & 0xFF) == 0xFF) continue;
		inIp.s_addr = addr;
		if(strcmp(lanIp,inet_ntoa(inIp)) == 0){
			continue;
		}
		#if defined(ALIAS_IP)
		if(Flag){
			if(strcmp(lanAliasIP,inet_ntoa(inIp)) == 0){
				continue;
			}
		}
		#endif

		/* Only do if it isn't an assigned as a static lease */
		if(!reservedIp(server_config.static_leases, htonl(addr)))
		{

		/* lease is not taken */
		ret = htonl(addr);
		if ((!(lease = find_lease_by_yiaddr(ret)) ||

		     /* or it expired and we are checking for expired leases */
		     (check_expired  && lease_expired(lease))) &&

		     /* and it isn't on the network */
	    	     !check_ip(ret)) {
			return ret;
			break;
		}
	}
	}
	return 0;
}
