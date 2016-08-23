//******************************************************************************
//
// Copyright (C) 2003 TrendChip Technologies Corp.
//
// Module name
// -----------
// TCIfSetQuery.h
//
// Abstract
// --------
// This file contains the set and query id definition with ZyNOS
//              
// Modification History
// --------------------
// Date   : 2003/5/27 Jason
// Rev.   : 001
// Modify : 
//
//*****************************************************************************
// $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/linux/include/asm-mips/tc3162/TCIfSetQuery_os.h#2 $
// $Log: TCIfSetQuery_os.h,v $
// Revision 1.3  2011/07/22 10:24:28  treychen_hc
// bug#10858--Trey
//
// Revision 1.2  2011/06/02 11:35:51  lino
// add RT65168 support
//
// Revision 1.1.1.1  2010/04/09 09:39:21  feiyan
// New TC Linux Make Flow Trunk
//
// Revision 1.2  2010/02/03 10:14:31  yuren_nj
// [Enhancement]#4788 Add tr69 parameters.
//
// Revision 1.1.1.1  2009/12/17 01:42:47  josephxu
// 20091217, from Hinchu ,with VoIP
//
// Revision 1.1.1.1  2007/04/12 09:42:03  ian
// TCLinuxTurnkey2007
//
// Revision 1.2  2006/07/06 04:12:45  lino
// add kernel module support
//
// Revision 1.1.1.1  2005/11/02 05:45:38  lino
// no message
//
// Revision 1.2  2005/08/19 14:36:59  jasonlin
// Merge Huawei's code into new main trunk
//
// Revision 1.1.1.1  2005/03/30 14:04:23  jasonlin
// Import Linos source code
//
// Revision 1.8  2005/03/02 06:57:12  jasonlin
// add extra query function ID for CHINA_NM
//
// Revision 1.7  2004/11/10 06:42:04  jasonlin
// Add CI command "wan adsl opencmf adsl2|adsl2plus"
//
// Revision 1.5  2004/11/10 05:40:52  wheellenni
// wheellenni_931110
// compatible for the previous firmware
//
// Revision 1.4  2004/11/09 08:51:15  wheellenni
// 931109_wheellenni
// Add adsl2/adsl2plus to CI command "wan adsl opencmd xxxx"
// "adsl2" only support G.992.3	and "adsl2plus" support G.992.5
//
// Revision 1.3  2004/10/04 12:46:58  bright
// bright_931004
//
// Revision 1.2  2004/09/14 08:50:21  bright
// bright_930914
//
// Revision 1.1.1.1  2004/08/17 07:06:46  jeffrey
// Import framework code from V3.0.0.
//
// Revision 1.1.1.1  2003/08/22 06:19:18  jasonlin
// no message
//
//*****************************************************************************

#ifndef _TCIFSETQUERY_H
#define _TCIFSETQUERY_H

// ADSL Query ID list   Jason_920527
#define ADSL_QUERY_MODESET          0x0000              
#define ADSL_QUERY_MODE             0x0001
#define ADSL_QUERY_STATUS           0x0002
#define ADSL_QUERY_SW_VER           0x0003
#define ADSL_QUERY_VENDOR_ID        0x0004
#define ADSL_QUERY_NEAR_OP_DATA     0x0005
#define ADSL_QUERY_FAR_OP_DATA      0x0006
#define ADSL_QUERY_CH_OP_DATA       0x0007
#define ADSL_QUERY_DEFECT_CNT       0x0008
#define ADSL_QUERY_DEFECT           0x0009
#define ADSL_QUERY_NEAR_ITUID       0x000a
#define ADSL_QUERY_FAR_ITUID        0x000b
#define ADSL_QUERY_WATCHDOG         0x000c
#define ADSL_QUERY_AUTOLINK         0x000d
#define ADSL_QUERY_CELL_CNT         0x000e
#define ADSL_QUERY_TX_GAIN          0x000f
#define ADSL_QUERY_TX_FILTER_TYPE   0x0010
#define ADSL_QUERY_TX_MEDLEY_TONE   0x0011
#define ADSL_QUERY_SNR_OFFSET       0x0012
#define ADSL_QUERY_MIN_SNR_M        0x0013
#define ADSL_QUERY_FW_VER           0x0014
#define ADSL_QUERY_FW_REV           0x0015
#define ADSL_QUERY_TDM_FDM_VER      0x0016
#define ADSL_QUERY_RTS_LOG_SIZE_STR 0x0017
#define ADSL_QUERY_RTS_ADDR_STR     0x0018
#define ADSL_QUERY_RTS_LOG_SIZE     0x0019
#define ADSL_QUERY_HW_VER           0x001a
#define ADSL_QUERY_DGASP_CNTR		0x001b
#define ADSL_QUERY_SHOW_DIAG		0x001c
//#define ADSL_QUERY_CUSTOMER 		0x001d //bright_930914_1
#define ADSL_QUERY_BUILD_VERSION	0x001d //bright_930914_1
#define ADSL_QUERY_ANNEX            0x001e	//julia_070424
#define ADSL_QUERY_TR69_WLAN_DSL_INTERFACE_Config  0x001f	//jf_070522
#define ADSL_QUERY_PMS_PARAM        0x0020	//cheng_070727
#define ADSL_QUERY_TR69_WLAN_DSL_DIAGNOSTIC  0x0021	//dyma_071017
#define ADSL_QUERY_SHOW_STATE_PARAM        0x0022	//jmxu_071219
#define ADSL_QUERY_LINERATE      0x0023   //haotang_080416
#define ADSL_QUERY_TR098_WAN_DSL_INTERFACE_CONFIG 0x0024    //rmzha_080527
#define ADSL_QUERY_PM_STATE 0x0025  //Roger_090206
#define ADSL_QUERY_BITSWAP_ONOFF 0x0026//Roger_090206

#define ADSL_QUERY_LAST_DROP_REASON 0x002a    //whliu_090924

#ifdef DADI				//Julia_051117
#define ADSL_QUERY_ATTAIN_RATE      0x0027	//Julia_051117
#endif
#define ADSL_QUERY_CELL_CNT1         0x002b      //zzma_091020
#define ADSL_QUERY_RX_BEAR_TPSTC_TYPE         0x002c


#if 1 //defined(TCSUPPORT_CWMP_VDSL)
#define VDSL_QUERY_TR098_DSL_INTERFACE_CONFIG 0x2001
#endif

#define TPSTC_DISABLE      			0x00
#define TPSTC_ATM_TC       			0x01
#define TPSTC_PTM_TC_64_65B       	0x02
#define TPSTC_PTM_TC_HDLC       	0x03

#define ADSL_QUERY_SRA_ONOFF 0x0028 //Roger_090206
#define ADSL_QUERY_PM_ONOFF 0x0029 //Roger_090206

#define ADSL_SET_MODE_A43           0x1000
#define ADSL_SET_MODE_LITE          0x1001
#define ADSL_SET_MODE_MULTIMODE     0x1002
#define ADSL_SET_MODE_ANSI          0x1003
#define ADSL_SET_MODE_ADSL2         0x1004   // wheellenni_931110
#define ADSL_SET_MODE_ADSL2PLUS     0x1005   // wheellenni_931110
#define ADSL_SET_MODE_AUTO_SENSE_GDMT_FIRST   0x1006	//yyfeng_050719
#define ADSL_SET_MODE_AUTO_SENSE_T1413_FIRST   0x1007	//yyfeng_050719
#define ADSL_SET_MODE_GDMT_OR_LITE     0x1008	//yyfeng_050719
#define ADSL_SET_MODE_AUTO_SENSE_T1413_OFF     0x1009	//yyfeng_070210
#define ADSL_SET_MODE_AUTO_SENSE_ADSL2_OFF     0x100a	//yyfeng_070210
#define ADSL_SET_MODE_ADSL1_MULTI     0x100b	//yyfeng_070210
#define ADSL_SET_MODE_ADSL2_MULTI     0x100c	//yyfeng_070210
#define ADSL_SET_MODE_ADSL2PLUS_T1413 0x100d	// Ryan_961128
#define ADSL_SET_MODE_VDSL2         0x100e
#if defined(TCSUPPORT_CWMP_VDSL)
#define ADSL_SET_MODE_ADSL2PLUS_MULTI	0x100f
#endif
#define ADSL_SET_R_VENDOR_ID        0x1010
#define ADSL_SET_TX_GAIN            0x1011
#define ADSL_SET_TX_FILTER_TYPE     0x1012
#define ADSL_SET_TX_MEDLEY_TONE     0x1013
#define ADSL_SET_SNR_OFFSET         0x1014
#define ADSL_SET_MIN_SNR_M          0x1015
#define ADSL_SET_RTS_LOG_STOP       0x0016
#define ADSL_SET_RTS_LOG_RESUME     0x0017
#define ADSL_SET_RTS_LOG_FREE       0x0018
#define ADSL_SET_R_SOFT_RESET       0x0019
#define ADSL_SET_DMT_CLOSE          0x001a
#define ADSL_SET_INT_MASK0          0x001b
#define ADSL_SET_DGASP_CNTR_ZERO	0x001c		// Gilb_920901_1
#define ADSL_SET_CONSOLE_DATA		0x001d		// Jason_930315
#define ADSL_SET_TRELLIS                               0x1100	//yyfeng_050719
#define ADSL_SET_BITSWAP                              0x1101	//yyfeng_050719
#define ADSL_SET_PRINT                              0x1102	//jmxu_071219
#define ADSL_SET_SRA           0X1103  //Roger_090206
#define ADSL_SET_PM            0X1104 //Roger_090206
#define ADSL_SET_LDM          0X1105  //Roger_090206
/*//yyfeng_060510_1
#define ADSL_SET_ANNEX_M                                  0x1102          //yyfeng_060424
#define ADSL_SET_ANNEX                                       0x1103          //yyfeng_060425
*/

//julia_070424
#define         ME_CMD_ADSL_ANNEXA                    0x21
#define         ME_CMD_ADSL_ANNEXB                    0x22
#define         ME_CMD_ADSL_ANNEXI                    0x23
#define         ME_CMD_ADSL_ANNEXJ                    0x24
#define         ME_CMD_ADSL_ANNEXM                    0x25
#define         ME_CMD_ADSL_ANNEXL  				  0x26
//julia_070424

#define ADSL_MODEM_STATE_DOWN       0x00
#define ADSL_MODEM_STATE_WAIT_INIT  0x08
#define ADSL_MODEM_STATE_INIT       0x10
#define ADSL_MODEM_STATE_UP         0x20

#define         ME_CMD_ADSL_SELFTEST1               0x01
#define         ME_CMD_ADSL_SELFTEST2               0x02
#define         ME_CMD_ADSL_OPEN                    0x03
#define         ME_CMD_ADSL_CLOSE                   0x04
/*
#define         ME_CMD_ADSL_ANSI                    0x05
#define         ME_CMD_ADSL_OPEN_GDMT  				0x06
#define 		ME_CMD_ADSL_OPEN_GLITE 				0x07
#define 		ME_CMD_ADSL_OPEN_MULTIMODE			0x08
//xzwang 041103
#define          ME_CMD_ADSL_OPEN_GDMT_GLITE              0x0B
*/
//Jason_930706
#define    		ME_CMD_ADSL2						0x09
#define    		ME_CMD_ADSL2PLUS					0x0A
#define    		ME_CMD_VDSL2						0x0B

//xzwang
#define         ME_CMD_ADSL_ANSI                    0x05
#define         ME_CMD_ADSL_OPEN_GDMT  				0x06
#define 		ME_CMD_ADSL_OPEN_GLITE 				0x07
#define 		ME_CMD_ADSL_OPEN_MULTIMODE		       0x08
#define         ME_CMD_ADSL_GDMT_OR_LITE                     0x91 //yyfeng_041015_14
#define         ME_CMD_ADSL_AUTO_GDMT_FIRST              0x92
#define         ME_CMD_ADSL_AUTO_T1413_FIRST             0x93

#if 1//def CMD_ADSL_SNRM
#define ADSL_QUERY_SNR		0x0034
#define ADSL_QUERY_SNRM	0x0035
#define ADSL_QUERY_USBIT	0x0037 //VDSL only.
#define ADSL_QUERY_DSBIT	0x0038 //VDSL only.

#define ADSL_SET_HIGH_PERFORMANCE       0x1109

#define CTRLE_MODEM_NOT_UP 0
#define CTRLE_READ_DATA_OK 1
#define CTRLE_QUERY_FAIL   2
#endif

void TCIfQuery( unsigned short query_id, void *result1, void *result2 );
void TCIfSet( unsigned short set_id, void *value1, void *value2 );

typedef struct {
	void (*query)(unsigned short query_id, void *result1, void *result2);
	void (*set)(unsigned short set_id, void *value1, void *value2);

	void (*rts_rcv)(struct sk_buff *skb);

	int	(*rts_cmd)(int argc,char *argv[],void *p);
	int	(*dmt_cmd)(int argc,char *argv[],void *p);
	int	(*dmt2_cmd)(int argc,char *argv[],void *p);
	int	(*hw_cmd)(int argc,char *argv[],void *p);
	int	(*sw_cmd)(int argc,char *argv[],void *p);
	int	(*ghs_cmd)(int argc,char *argv[],void *p);
	int	(*tcif_cmd)(int argc,char *argv[],void *p);
} adsldev_ops;

extern adsldev_ops *adsl_dev_ops;
void adsl_dev_ops_register(adsldev_ops *ops);
void adsl_dev_ops_deregister(void);

#ifdef CONFIG_RALINK_VDSL
typedef struct {
	int	(*vdsl2_cmd)(int argc,char *argv[],void *p);
} vdsldev_ops;

extern vdsldev_ops *vdsl_dev_ops;
void vdsl_dev_ops_register(vdsldev_ops *ops);
void vdsl_dev_ops_deregister(void);
#endif

#endif
