//#ifdef _MESSAGEHANDLE_H
//#define _MESSAGEHANDLE_H

#include "Global_res.h"
#include "../../../lib/libtcapi.h"
/***************************************
*
*	macro definition
*
***************************************/

#define CONN_REQ 		0 		/*message queue for connection request*/
#define CWMP_CHANGED 	1	 	/*message queue for acs url change*/
#define VALUE_CHANGED 	2  		/*message queue for value change*/
#define HOST_REINIT 	3  	 	/*message queue for host information reinit*/
#define DEVICE_REINIT 	4 		/*message queue for host information reinit*/
#define DIAG_COMPLETE	5		/*message queue for diagnosis complete, then signal inform */
/***************************************
*
*	function declare
*
***************************************/
//int process_mq(char *message);
int process_mq(cwmp_msg_t *message);
int open_msgq();
//int read_msg(int mqid,long type,char *text,int flag);
int read_msg(int mqid,long type,cwmp_msg_t *text,int flag);
//int write_msg(int mqid,long type,char *text,int flag);
int write_msg(int mqid,long type,cwmp_msg_t *text,int flag);

#ifndef THREAD_REDUCE
void *message_handle();
#else
int message_handle();
#endif
int init_msg();






//#endif
