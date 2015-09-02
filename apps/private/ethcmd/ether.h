/************************************************************************
 *
 *	Copyright (C) 2006 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
#ifndef _ETHER_H
#define _ETHER_H
#include "libmii.h"

#define QUERY	(1<<1)
#define ACTIVED	(1<<0)
#define DEACTIVED	(0)
#define TC2206_IGMPSNOOP		(1<<0)
#define TC2206_IGMPFASTLEAVE	(1<<8)	

etherChannVlanCap_t* getEtherVlanCap(void);

int macTC2206VlanActive(u8 type);
int macTC2206VlanSetPvid(u8 port, u16 pvid);
#if defined(EXTEND_SETVID_PARAM)
int macTC2206VlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap, u8 ivl_en, u8 fid, u16 stag);
#else
int macTC2206VlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap);
#endif
int macTC2206VlanCap(void);
int macTC2206VlanGetPvid(u8 port);
int macTC2206VlanGetVtbl(u16 index);	// MT7530 need 16bit index (4K)
int macTC2206VlanDisp(void);
int macTC2206Igmpsnoop(u8 type);
int macTC2206IgmpFastLeave(u8 type);

int macRT63365VlanActive(u8 type);
int macRT63365VlanSetPvid(u8 port, u16 pvid);
#if defined(EXTEND_SETVID_PARAM) || defined(SQA_VERIFY)
int macRT63365VlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap, u8 ivl_en, u8 fid, u16 stag);
#else
int macRT63365VlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap);
#endif
int macRT63365VlanCap(void);
int macRT63365VlanGetPvid(u8 port);
int macRT63365VlanGetVtbl(u16 index);	// MT7530 need 16bit index (4K)
int macRT63365VlanDisp(void);
int macRT63365Igmpsnoop(u8 type);
int macRT63365IgmpFastLeave(u8 type);


int macRT7530VlanActive(u8 type);

#if defined(EXTEND_SETVID_PARAM) || defined(SQA_VERIFY)
int macMT7530VlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap, u8 ivl_en, u8 fid, u16 stag);
#else
int macMT7530VlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap);
#endif
int macMT7530VlanGetVtbl(u16 index);
int macMT7530VlanDisp(void);
int macMT7530VlanCap(void);

int macIP175CVlanActive(u8 type);
int macIP175CVlanSetPvid(u8 port, u16 pvid);
#if defined(EXTEND_SETVID_PARAM)
int macIP175CVlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap, u8 ivl_en, u8 fid, u16 stag);
#else
int macIP175CVlanSetVid(u8 index, u8 active, u16 vid, u8 portMap, u8 tagPortMap);
#endif
int macIP175CVlanCap(void);
#endif
