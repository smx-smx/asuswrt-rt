#include "dproxy.h"

/*
 * Add a node to the list pointed to by 'list'. Copy 'm' into that node.
 * RETURNS: pointer to start of list 
 */
dns_request_t *dns_list_add(dns_request_t *list, dns_request_t *m);
/*
 * Scans each node in the list pointed to by 'list' and compare the id field
 * of each node with that of the id fields of 'm'.
 * RETURNS: pointer to the first node that matches else NULL.
 */
dns_request_t *dns_list_find_by_id(dns_request_t *list, dns_request_t *m);
/*
 * Removes and frees the node pointed to by m
 */
dns_request_t *dns_list_remove(dns_request_t *list, dns_request_t *m );
/*
 * Print out the list for debuging purposes
 */
void dns_list_print(dns_request_t *list);
