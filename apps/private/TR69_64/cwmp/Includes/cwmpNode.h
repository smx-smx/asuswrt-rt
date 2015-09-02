#ifndef __CWMP_NDOE_H__
#define __CWMP_NDOE_H__


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif 

#define CWMP_E_SUCCESS          0
#define CWMP_E_DELAY            1	
#define CWMP_E_NOT_FOUND	    -507



int
cwmpCmpName( IN char *name,
              IN IXML_Node * node );

int
cwmpFindDeepNode( IN char *names[],
                    IN int num_names,
                    IN IXML_Node * start_node,
                    OUT IXML_Node ** matching_node );

int
cwmpFindNode( IN char *node_name,
               IN IXML_Node * start_node,
               OUT IXML_Node ** matching_node );

#if CWMP_REDUCE_CODE
const char *
cwmpGetNodeValue( IN IXML_Node * node );
#endif

#endif
