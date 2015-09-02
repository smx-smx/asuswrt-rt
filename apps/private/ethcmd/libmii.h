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
#ifndef _LIBMII_H
#define _LIBMII_H

#include <net/if.h>
#include <linux/types.h>

#define IP175C		0x1
#define ADM6996M	0x2
#define TC2206		0x3
#define SWIC_RT63365	0x4
#define RT62806		0x5
#define MT7530		0x6
#define UNKNOW_SWIC	0xff


#if defined(TCSUPPORT_2PORTS) || defined(TCSUPPORT_CT_2PWIFI)
#define SWIC_PORTS_NUM	2
#else
#if defined(TCSUPPORT_CPU_RT63365) || defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520)
#define SWIC_PORTS_NUM	6
#else
#define SWIC_PORTS_NUM	4
#endif
#endif
#define SINGLE_PORT		1

#ifdef TCSUPPORT_CPU_MT7510
#undef DEFAULT_USE_EXT_SWIC
#endif
#if defined(TCSUPPORT_CPU_MT7520) || defined(MT7530_SUPPORT)
#define DEFAULT_USE_EXT_SWIC
#endif
#if defined(TCSUPPORT_CPU_RT65168) || defined(TCSUPPORT_CPU_TC3182)	// for early MT7530 FPGA verification only
#if !defined(TCSUPPORT_CT) 
#define DEFAULT_USE_EXT_SWIC
#endif
#endif

#if defined(TCSUPPORT_CPU_MT7510) || defined(TCSUPPORT_CPU_MT7520)
#define JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL
//#ifdef JUDGE_SWITCH_SCENARIO_BY_751020_SUBMODEL

#define _isMT751020 	(((np_hw_id&0xffff0000))==0x00050000)
#define _isMT751x	(((mt751020_submodel_id&0x3)==0x0) && _isMT751020)
#define _isENP_MOD	(((efuse_extreme_mode&0x8)==0x8)?(((efuse_extreme_mode&0x20)==0x20)?1:0):(((efuse_extreme_mode&0x2)==0x2)?1:0))
#define _isENS_MOD	(((efuse_extreme_mode&0x8)==0x8)?((efuse_extreme_mode&0x40)==0x40):((efuse_extreme_mode&0x4)==0x4))
#define _isMT7520	(((mt751020_submodel_id&0x3)==0x2) && _isMT751020 && (_isENP_MOD))
#define _isMT7520G	(((mt751020_submodel_id&0x3)==0x3) && _isMT751020 && (_isENP_MOD))
#define _isMT7525	(((mt751020_submodel_id&0x3)==0x2) && _isMT751020 && (!_isENP_MOD))
#define _isMT7525G	(((mt751020_submodel_id&0x3)==0x3) && _isMT751020 && (!_isENP_MOD))
#define _isMT7520S	(((mt751020_submodel_id&0x3)==0x2) && _isMT751020 && (_isENP_MOD) && _isENS_MOD)


#define _ExistExtMT7530		(_isMT7520G || _isMT7525G)

#if defined(MT7530_SUPPORT)
#define _DefaultUseExtMT7530	(1)
#else
#define _DefaultUseExtMT7530	(_isMT7520G || _isMT7525G)
#endif

#define _LanPortIntMT7530	(_isMT751x || _isMT7520 || _isMT7525)
#define _LanPortExtMT7530	(_isMT7520G || _isMT7525G)

#else
#define _isMT751020 	(0)
#define _isMT751x	    (0)
#define _isENP_MOD	    (0)
#define _isENS_MOD	    (0)
#define _isMT7520	    (0)
#define _isMT7520G	    (0)
#define _isMT7525	    (0)
#define _isMT7525G	    (0)
#define _isMT7520S	    (0)
#define _ExistExtMT7530	(0)
#define _DefaultUseExtMT7530	(0)
#define _LanPortIntMT7530	(0)
#define _LanPortExtMT7530	(0)

#endif

#define EXTEND_SETVID_PARAM	//MTK20120903_EXTEND_SETVID_PARAM

typedef u_int32_t u32;
typedef u_int16_t u16;
typedef u_int8_t u8;

#define ETHER_VLAN_SAME_VID_IDX	(1<<0)
#define ETHER_8021Q_VLAN	(1<<0)

/* This data structure is used for all the MII ioctl's */
struct mii_data {
      u16   phy_id;
      u16   reg_num;
      u16   val_in;
      u16   val_out;
};

#if (1)	// move to tc3162.h
/* ioctl commands */
#define RAETH_GSW_REG_READ		0x89F1
#define RAETH_GSW_REG_WRITE		0x89F2
#define RAETH_REG_READ			0x89F3
#define RAETH_REG_WRITE			0x89F4
#define RAETH_GSW_PHY_READ		0x89F5
#define RAETH_GSW_PHY_WRITE		0x89F6

#define RAETH_GSWEXT_REG_READ		0x89F7 //MTK120625 ///YM
#define RAETH_GSWEXT_REG_WRITE		0x89F8 //MTK120625 ///YM
#define RAETH_GSWEXT_PHY_READ		0x89F9 //MTK120625 ///YM
#define RAETH_GSWEXT_PHY_WRITE		0x89FA //MTK120625 ///YM

#ifdef TCSUPPORT_MT7530_SWITCH_API
#define RAETH_GSW_CTLAPI		0x89FB
#endif

#define RAETH_GSW_PHY_BP_READ	0x89FE
#define RAETH_GSW_PHY_BP_WRITE	0x89FF


#endif


typedef struct _gsw_reg {
	u32 off;
	u32 val;
} gsw_reg;

typedef struct swic_api{
	int (*hw_igmpsnoop)(u8 type);
	int (*hw_fastleave)(u8 type);
	int (*spTag)(u8 type);
	int (*vlan_active)(u8 type);
	int (*vlan_pvid)(u8 port_id, u16 pvid);
	int (*vlan_get_pvid)(u8 port_id);
	int (*vlan_pvid_disp)(u8 port_id);
	#ifdef EXTEND_SETVID_PARAM
	int (*vlan_vid)(u8 index, u8 type, u16 vid, u8 portMap, u8 tagPortMap, u8 ivl_en, u8 fid, u16 stag);
	#else
		#if defined(SQA_VERIFY)
	int (*sqa_vlan_vid)(u8 index, u8 type, u16 vid, u8 portMap, u8 tagPortMap, u8 ivl_en, u8 fid, u16 stag);
		#endif
	int (*vlan_vid)(u8 index, u8 type, u16 vid, u8 portMap, u8 tagPortMap);
	#endif
	int (*vlan_get_vtbl)(u16 index);	// MT7530 need 16bit index (4K)
	int (*vlan_disp)(void);
	int (*vlan_getCap)(void);
}swic_api_t;

struct vender{
	u8 phy_id1;
	u16 reg1;
	u32 id1;
	u8 phy_id2;
	u16 reg2;
	u32 id2;
	char* name;
	u16 swic_id;
	void (*api_init)(swic_api_t* a);
};

#define 	PORT_REVERSE    (1<<0) /* port reverse */
#define		STAG_TO_VTAG	(1<<1) /* special tag to vlan tag */

struct swconfig{
	int skfd;
	struct ifreq ifr;
	u8 swic_id;
	//u8 reverse_flag;
	u8 flag;
};

typedef struct{
	u16 max_port_no;
	u16 max_vid;
	u32 flags;
}etherChannVlanCap_t;

/*Define Function Prototype*/
int mdio_read(int skfd, struct ifreq ifr, u32 phy_id, u32 reg_num);
void mdio_write(int skfd, struct ifreq ifr, u32 phy_id, u32 reg_num, u32 value);
int gsw_reg_read(int skfd, struct ifreq ifr, u32 offset);
void gsw_reg_write(int skfd, struct ifreq ifr, u32 offset, u32 value);
int mdio_read2(int skfd, struct ifreq ifr, u32 ext_switch, u32 phy_id, u32 reg_num);
void mdio_write2(int skfd, struct ifreq ifr, u32 ext_switch, u32 phy_id, u32 reg_num, u32 value);
int gsw_reg_read2(int skfd, struct ifreq ifr, u32 ext_switch, u32 offset); //MTK120625 ///YM
void gsw_reg_write2(int skfd, struct ifreq ifr, u32 ext_switch, u32 offset, u32 value); //MTK120625 ///YM

//#ifdef MT7530_PHY_VERIFY
int mdio_read45(int skfd, struct ifreq ifr, u32 portAddr, u32 devAddr, u32 regAddr);
void mdio_write45(int skfd, struct ifreq ifr, u32 portAddr, u32 devAddr, u32 regAddr, u32 miiData);
//#endif

void searchSwic(int skfd, struct ifreq ifr);
int init_swconfig(void);
struct swconfig* getSwcfg(void);
void showSwicName(void);

int get_wan_port(void);
#endif
