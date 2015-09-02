

#ifndef _IXML_MEMBUF_H
#define _IXML_MEMBUF_H
//#include "typedefs.h"//added by hqfeng,2006.02.27
#include <stdlib.h>
#include "cwmpXml.h"

#define MINVAL( a, b ) ( (a) < (b) ? (a) : (b) )
#define MAXVAL( a, b ) ( (a) > (b) ? (a) : (b) )

#define	XINLINE inline

#define MEMBUF_DEF_SIZE_INC		8192


typedef struct // ixml_membuf 
{
	char	*buf;
				
	size_t	length;
	size_t	capacity;
	size_t	size_inc;

} ixml_membuf;

//--------------------------------------------------
//////////////// functions /////////////////////////
//--------------------------------------------------
/*
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
*/

void ixml_membuf_init(INOUT ixml_membuf *m);
void ixml_membuf_init_new( INOUT ixml_membuf * m );
void ixml_membuf_destroy(INOUT ixml_membuf *m);
int ixml_membuf_assign(INOUT ixml_membuf *m, IN const void *buf, 
					 IN size_t buf_len );
int ixml_membuf_assign_str(INOUT ixml_membuf *m, IN const char *c_str );
int ixml_membuf_append(INOUT ixml_membuf *m, IN const void *buf);
int ixml_membuf_append_str(INOUT ixml_membuf *m, IN const char *c_str);
int ixml_membuf_insert(INOUT ixml_membuf *m, IN const void* buf, 
					 IN size_t buf_len, int index );

#endif // _IXML_MEMBUF_H
