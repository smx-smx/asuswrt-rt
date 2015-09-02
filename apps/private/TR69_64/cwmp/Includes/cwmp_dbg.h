
/*
** $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/private/TR69_64/cwmp/Includes/cwmp_dbg.h#1 $
*/
/************************************************************************
 *
 *	Copyright (C) 2005 Trendchip Technologies, Corp.
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


#ifndef __CWMP_DBG_H__
#define  __CWMP_DBG_H__

#define DBG_CWMP_L0  0x0   
#define DBG_CWMP_L1  0x01    
#define DBG_CWMP_L2  0x02
#define DBG_CWMP_L3  0x03 //added by hqfeng,2006.07.05

#if 0 //xzwang_060921

#if defined(WLAN_RT2561_SUPPORT)
#define TX_BUFFER_LENGTH 256000
#elif (!defined(IPFILTER) && !defined(VLAN_8021Q)  && !defined(PKT_QOS))
#define TX_BUFFER_LENGTH 128000
#elif defined(IPFILTER) && !defined(VLAN_8021Q)  && !defined(PKT_QOS)
#define TX_BUFFER_LENGTH 96000
#elif !defined(IPFILTER) && defined(VLAN_8021Q)  && !defined(PKT_QOS)
#define TX_BUFFER_LENGTH 72000
#elif !defined(IPFILTER) && !defined(VLAN_8021Q)  && defined(PKT_QOS)
#define TX_BUFFER_LENGTH 84000
#elif  defined(IPFILTER) && defined(VLAN_8021Q)  && defined(PKT_QOS)
#define TX_BUFFER_LENGTH 68000
#else
#define TX_BUFFER_LENGTH 70000
#endif

#else

#if 	CWMP_USING_MBUF_FOR_TXBUF == 0
#define  TX_BUFFER_LENGTH  (getTxBufLenFromMMCT())
#endif

#endif

// yen_20051204
#define CWMP_DBG_PRINT       if(dbg_cwmp >= DBG_CWMP_L1 ) printf
#define CWMP_DBG_PRINT_2   if(dbg_cwmp >= DBG_CWMP_L2 ) printf

extern unsigned char dbg_cwmp;
 
#endif  //  __CWMP_DBG_H__
