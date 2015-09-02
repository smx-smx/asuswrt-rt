/**************************************
 ecmh - Easy Cast du Multi Hub
 by Jeroen Massar <jeroen@unfix.org>
***************************************
 $Author: josephxu $
 $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/ecmh-2005.02.09/src/grpint.h#1 $
 $Date: 2010/10/01 $
**************************************/

/* The node used to hold the interfaces which a group joined */
struct grpintnode
{
	unsigned int		ifindex;		/* The interface */
	struct list		*subscriptions;		/* Subscriber list */
};

struct grpintnode *grpint_create(const struct intnode *interface);
void grpint_destroy(struct grpintnode *grpintn);
struct grpintnode *grpint_find(const struct list *list, const struct intnode *interface);
bool grpint_refresh(struct grpintnode *grpintn, const struct in6_addr *ipv6, unsigned int mode);

