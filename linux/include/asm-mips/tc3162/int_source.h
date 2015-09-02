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

#ifndef _INT_SOURCE_H_
#define _INT_SOURCE_H_

#if !defined(TCSUPPORT_CT) 
enum
interrupt_source
	{
		UART_INT,		//0 	IPL10
		RTC_ALARM_INT,	//1 	IPL29
		RTC_TICK_INT,	//2 	IPL31
		RESERVED0,		//3 	IPL30
		TIMER0_INT, 	//4 	IPL1
		TIMER1_INT, 	//5 	IPL5
		TIMER2_INT, 	//6 	IPL6
		TIMER3_INT, 	//7 	IPL7
		TIMER4_INT, 	//8 	IPL8
		TIMER5_INT, 	//9 	IPL9
		GPIO_INT,		//10	IPL11
		PCIE_A_INT,		//11	IPL20
		PCIE_SERR_INT,	//12	IPL21
		RESERVED3,		//13	IPL22
		APB_DMA0_INT,	//14	IPL12
		APB_DMA1_INT,	//15	IPL13
		HSUART_INT,		//16	IPL23
		RESERVED5,		//17	IPL24
		DYINGGASP_INT,	//18	IPL25
		DMT_INT,		//19	IPL26
		ARBITER_ERR_INT,//20	IPL0
		MAC_INT,		//21	IPL3
		SAR_INT,		//22	IPL2
		USB_INT,		//23	IPL14
		PCI_A_INT,		//24
		PCI_B_INT,		//25
//		  RESERVED8,	  //24	  IPL27
//		  RESERVED9,	  //25	  IPL28
		XSLV0_INT,		//26	IPL15
		XSLV1_INT,		//27	IPL16
		XSLV2_INT,		//28	IPL17
		XAPB0_INT,		//29	IPL18
		XAPB1_INT,		//30	IPL19
		SWR_INT 		//31	IPL4
	};

#endif
#endif /* _INT_SOURCE_H_ */
