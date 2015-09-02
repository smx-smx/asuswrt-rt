/*
 * SPIFLASH support for TC3162
 */

/*
 * SPI Flash Memory support header file.
 *
 * Copyright (c) 2005, Atheros Communications Inc.
 * Copyright (C) 2006 FON Technology, SL.
 * Copyright (C) 2006 Imre Kaloz <kaloz@openwrt.org>
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __SPI_TC3162_H
#define __SPI_TC3162_H

#define STM_PAGE_SIZE           256

#define SPI_WRITE_ENABLE    	0
#define SPI_WRITE_DISABLE   	1
#define SPI_RD_STATUS       	2
#define SPI_WR_STATUS       	3
#define SPI_RD_DATA         	4
#define SPI_FAST_RD_DATA    	5
#define SPI_PAGE_PROGRAM    	6
#define SPI_SECTOR_ERASE    	7
#define SPI_BULK_ERASE      	8
#define SPI_DEEP_PWRDOWN    	9
#define SPI_RD_SIG          	10
#define SPI_RD_ID	          	11
#define SPI_MAX_OPCODES     	12

#define SFI_WRITE_BUFFER_SIZE   4
#define SFI_FLASH_ADDR_MASK     0x00ffffff

/*
 * ST Microelectronics Opcodes for Serial Flash
 */

#define STM_OP_WR_ENABLE       	0x06     	/* Write Enable */
#define STM_OP_WR_DISABLE      	0x04     	/* Write Disable */
#define STM_OP_RD_STATUS       	0x05     	/* Read Status */
#define STM_OP_WR_STATUS       	0x01     	/* Write Status */
#define STM_OP_RD_DATA         	0x03     	/* Read Data */
#define STM_OP_FAST_RD_DATA    	0x0b     	/* Fast Read Data */
#define STM_OP_PAGE_PGRM       	0x02     	/* Page Program */
#define STM_OP_SECTOR_ERASE    	0xd8     	/* Sector Erase */
#define STM_OP_BULK_ERASE      	0xc7     	/* Bulk Erase */
#define STM_OP_DEEP_PWRDOWN    	0xb9     	/* Deep Power-Down Mode */
#define STM_OP_RD_SIG          	0xab     	/* Read Electronic Signature */
#define STM_OP_RD_ID          	0x9f     	/* Read JEDEC ID */

#define STM_STATUS_WIP       	0x01       	/* Write-In-Progress */
#define STM_STATUS_WEL       	0x02       	/* Write Enable Latch */
#define STM_STATUS_BP0       	0x04       	/* Block Protect 0 */
#define STM_STATUS_BP1       	0x08       	/* Block Protect 1 */
#define STM_STATUS_BP2       	0x10       	/* Block Protect 2 */
#define STM_STATUS_SRWD      	0x80       	/* Status Register Write Disable */

#define SPI_FLASH_CTL           0x00
#define SPI_FLASH_OPCODE        0x04
#define SPI_FLASH_DATA          0x08
#define SPI_FLASH_MM		    0x28

#define SPI_CTL_START           0x00000100
#define SPI_CTL_BUSY            0x00010000
#define SPI_CTL_TXCNT_MASK      0x0000000f
#define SPI_CTL_RXCNT_MASK      0x000000f0
#define SPI_CTL_TX_RX_CNT_MASK  0x000000ff
#define SPI_CTL_SIZE_MASK       0x00060000

#define SPI_CTL_CLK_SEL_MASK    0x03000000
#define SPI_OPCODE_MASK         0x000000ff

#define SPI_STATUS_WIP			STM_STATUS_WIP

#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
extern int offset;
#endif


#endif
