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
#ifndef _TCAPI_PROC_H
#define _TCAPI_PROC_H

#define MAX_SHOW_BUF	8192 
#define MAX_ATTR_BUF	640

#define WAN_IPTV	"Wan_IPTV"
#define WAN_IPTV_ID	1

int 
isVaildNode(char name[][MAX_NODE_NAME]);

void 
tcapi_set_req(mxml_node_t* tree,tcapi_msg_t *msg);

void
tcapi_unset_req(mxml_node_t* tree, tcapi_msg_t *msg);

void 
tcapi_get_req(mxml_node_t* tree, tcapi_msg_t *msg);

void 
tcapi_show_req(mxml_node_t* tree,tcapi_msg_t *msg);

void 
tcapi_commit_req(mxml_node_t* tree, tcapi_msg_t *msg);

void 
tcapi_save_req(mxml_node_t* tree, tcapi_msg_t *msg);

void 
tcapi_read_req(mxml_node_t* tree, tcapi_msg_t *msg);

extern void tcapi_readAll_req(tcapi_msg_t *msg);

void
tcapi_reset_req(mxml_node_t* tree, tcapi_msg_t *msg);

void 
tcapi_staticGet_req(mxml_node_t* tree, tcapi_msg_t *msg);
#if (defined(TCSUPPORT_WAN_ETHER) || defined(TCSUPPORT_WAN_PTM)) && defined(TCSUPPORT_MULTISERVICE_ON_WAN)
int transfer_to_exten(mxml_node_t *top, char ori_name[][MAX_NODE_NAME], char merge_flag, int merge_num);
int transfer_to_normal(mxml_node_t *top, char *ori_name, char new_name[][MAX_NODE_NAME]);
#endif
#ifdef TCSUPPORT_GETNODE_ENHANCE
int checkNodeCacheList(char * nodeName);
int isInExceptionList(char * nodeName);
#endif
#endif
