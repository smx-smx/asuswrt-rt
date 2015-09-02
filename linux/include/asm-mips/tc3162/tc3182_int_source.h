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

enum
interrupt_source
	{
		DUMMY_INT,
		UART_INT,		//0 	IPL10
		PTM_B0_INT,		//1
		SI_SWINT1_INT0,	//2
		SI_SWINT1_INT1,	//3
		TIMER0_INT, 	//4 	IPL1
		TIMER1_INT, 	//5 	IPL5
		TIMER2_INT, 	//6 	IPL6
		SI_SWINT_INT0, 	//7
		SI_SWINT_INT1, 	//8
		TIMER5_INT, 	//9 	IPL9
		GPIO_INT,		//10	IPL11
		RESERVED1,		//11	IPL20
		SI_PC1_INT, 	//12
		SI_PC_INT, 		//13
		APB_DMA0_INT,	//14	IPL12
		MAC1_INT,		//15	IPL13
		HSUART_INT,		//16	IPL23
#if !defined(CONFIG_MIPS_RT63365)
		RESERVED2,		//17
#else
		IRQ_RT3XXX_USB,	//17	IPL24
#endif
		DYINGGASP_INT,	//18	IPL25
		DMT_INT,		//19	IPL26
		USB20_INT,		//20
		MAC_INT,		//21	IPL3
		SAR_INT,		//22	IPL2
		USB11_INT,
		PCIE_A_INT,		//24
		PCIE_SERR_INT,		//25
		PTM_B1_INT,		//26	IPL15
		XSLV1_INT,		//27	IPL16
		USB_INT,		//28	IPL17
		SI_TIMER1_INT,	//29
		SI_TIMER_INT,	//30
		SWR_INT, 		//31	IPL4
		BUS_TOUT_INT 	//32	
	};

#endif /* _INT_SOURCE_H_ */
