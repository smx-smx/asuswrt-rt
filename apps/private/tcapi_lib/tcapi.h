/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
 * All Rights Reserved.
 *
 * TrendChip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of TrendChip Technologies, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * TrendChip Technologies, Corp.
 */
/******************************************************************************/
#ifndef _TCAPI_H
#define _TCAPI_H

#define SOCK_PATH "/tmp/tcapi_sock"
/*TCAPI OP_CMD */
#define TCAPI_SET_CMD			0
#define TCAPI_UNSET_CMD		1
#define TCAPI_GET_CMD			2
#define TCAPI_SHOW_CMD		3
#define TCAPI_COMMIT_CMD		4
#define TCAPI_SAVE_CMD			5
#define TCAPI_READ_CMD			6
//added by xyzhu_nj_20091204
#define TCAPI_READALL_CMD		7
#define TCAPI_STATIC_GET_CMD                   8
#define TCAPI_RESET_CMD			9

/*TCAPI return message*/
#define TCAPI_PROCESS_OK	0
#define TCAPI_NONE_NODE	-1
#define TCAPI_NONE_ATTR		-2
#define TCAPI_CONN_ERR	-3
#define TCAPI_VERIFY_FAIL	-4
#define TCAPI_UNKNOW_ERR -5

#define MAXLEN_NODE_NAME	32
#define MAXLEN_ATTR_NAME	32
#ifdef TCSUPPORT_CT_E8GUI
#define MAXLEN_TCAPI_MSG		1024
#else
#define MAXLEN_TCAPI_MSG		576
#endif

typedef struct
tcapi_msg{
	unsigned int	op_cmd; /*set/Unset/Get/Show/Commit/Save*/
	/*
		0:sucess, -1: None node, -2: None attribution,
		-3:Something wrong with transfer data
	*/
	int	retval;
	char node[MAXLEN_NODE_NAME];
	char attr[MAXLEN_NODE_NAME];
	char value[MAXLEN_TCAPI_MSG];
} tcapi_msg_t;

#endif
