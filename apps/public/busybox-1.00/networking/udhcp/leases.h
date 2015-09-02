/* leases.h */
#ifndef _LEASES_H
#define _LEASES_H

#define HOST_NAME_LEN 32

struct dhcpOfferedAddr {
	uint8_t chaddr[16];
	uint32_t yiaddr;	/* network order */
	uint32_t expires;	/* host order */
	char hostname[HOST_NAME_LEN];      /*host name*/
};

extern uint8_t blank_chaddr[];

void clear_lease(uint8_t *chaddr, uint32_t yiaddr);
struct dhcpOfferedAddr *add_lease(uint8_t *chaddr, uint32_t yiaddr, unsigned long lease, char *hostname);
int lease_expired(struct dhcpOfferedAddr *lease);
struct dhcpOfferedAddr *oldest_expired_lease(void);
struct dhcpOfferedAddr *find_lease_by_chaddr(uint8_t *chaddr);
struct dhcpOfferedAddr *find_lease_by_yiaddr(uint32_t yiaddr);
uint32_t find_address(int check_expired);


#endif
