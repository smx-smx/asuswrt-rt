#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include "dns_list.h"

/*****************************************************************************/
dns_request_t *dns_list_add(dns_request_t *list, dns_request_t *m)
{
  dns_request_t *new_node;

  new_node = (dns_request_t *)malloc( sizeof(dns_request_t) * 10 );
  if( !new_node) return list;

  debug( "Addr: %x\n", new_node);

  memcpy( new_node, m, sizeof (*m) );

  new_node->next = list;
  return new_node;
}
/*****************************************************************************/
dns_request_t *dns_list_find_by_id(dns_request_t *list, dns_request_t *m)
{
  dns_list_print( list );
  
  while( list != NULL){
    debug("1. Name: %s  ....  %d  --- 2. %d\n", list->message.question[0].name,
	  list->message.header.id, m->message.header.id );
    if( list->message.header.id == m->message.header.id )
      return list;
    list = list->next;
  }
  return NULL;
}
/*****************************************************************************/
dns_request_t *dns_list_remove(dns_request_t *list, dns_request_t *m )
{
  dns_request_t *prev, *retval;

  if( !m )return list;

  prev = list;
  while (prev){
    if( prev->next == m)break;
    prev = prev->next;
  }

  if( !prev ){ //must be at start of list
    retval = m->next;
    free( m );
  }else{
    prev->next = m->next;
    retval = list;
    free( m );
  }
  return retval;
}
/*****************************************************************************/
void dns_list_print(dns_request_t *list)
{
  debug("Dumping list:\n");

  while(list){
    debug("    ID: %d ... Name: %s ---- IP: %s\n", list->message.header.id,
	  list->cname, list->ip );
    list = list->next;
  }

}




