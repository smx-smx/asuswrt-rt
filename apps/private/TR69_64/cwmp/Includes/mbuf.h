
#ifndef _MBUF_H
#define _MBUF_H

#define MBUF_NET_SZIE 2048
typedef	struct qlink_s {
 	struct qlink_s *qnext;		/* point to next queue buffer	*/
 	struct qlink_s *qprev;		/* point to prev queue buffer	*/
} qlink_t;

typedef	struct mbuf {
	qlink_t qlink;
//	struct  mbuf *next;	/* Links mbufs belonging to single packets */
	/* struct mbuf *fnext;  */	/* Links packets within fair queues	*/

//	uint16  allocated;	/* # bytes in buffer, including head & foot */
	uint16  size;		/* # bytes in data area			*/
	uint16  cnt;		/* # bytes used from size		*/

	uint8	*data_sp;	/* data start pointer			*/
	uint8	*data;			/* Active working pointer		*/
	
} mbuf_t;


void *os_malloc(size_t size);
void os_free(void *ptr);
char *os_str_realloc(char *buf,unsigned long newsize,unsigned long oldsize);
mbuf_t *alloc_cache_mbuf2k(void);
void free_p(mbuf_t *bp);
int spSave(void);
int spReadL(void);
#endif

