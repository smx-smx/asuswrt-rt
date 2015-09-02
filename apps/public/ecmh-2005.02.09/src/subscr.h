/**************************************
 ecmh - Easy Cast du Multi Hub
 by Jeroen Massar <jeroen@unfix.org>
***************************************
 $Author: josephxu $
 $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/ecmh-2005.02.09/src/subscr.h#1 $
 $Date: 2010/10/01 $
**************************************/

/* MLDv2 Source Specific Multicast Support */
struct subscrnode
{
	struct in6_addr	ipv6;		/* The address that wants packets matching this S<->G */
	unsigned int	mode;		/* MLD2_* */
	time_t		refreshtime;	/* The time we last received a join for this S<->G on this interface */
};

struct subscrnode *subscr_create(const struct in6_addr *ipv6, int mode);
void subscr_destroy(struct subscrnode *subscrn);
struct subscrnode *subscr_find(const struct list *list, const struct in6_addr *ipv6);
bool subscr_unsub(struct list *list, const struct in6_addr *ipv6);

