/************************************************************************
 *
 *	Copyright (C) 2010 Trendchip Technologies, Corp.
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
#ifndef TC_PARTITION_H
#define TC_PARTITION_H
 #ifdef TCSUPPORT_MTD_ENCHANCEMENT
/*
note:		
	1.the read base address of reserve area need to compute by (flash total size -flash erase size * reverse block num) 
		-the base addrass can be obtained by mtd ioctl
		-for example :flash total size is 32 m,flash erase size is 64k,reverse block num is 1,so the read base address of 
		reserve area is 0x3fff0000
	2.reverse block num is according to compile option TCSUPPORT_RESERVEAREA_BLOCK
	TCSUPPORT_RESERVEAREA_BLOCK==1 --------reverse block num is 1
	TCSUPPORT_RESERVEAREA_BLOCK==2 --------reverse block num is 2
	TCSUPPORT_RESERVEAREA_BLOCK==3 --------reverse block num is 3
	TCSUPPORT_RESERVEAREA_BLOCK==4 --------reverse block num is 4
	3.if you modify the size or offset of the certain sector or you add a new sector,you must modify the reserve area table in order to let
	other people be clear.
*/

//********************************
//	operation of  reserve area 	       //
//********************************
#define TC_FLASH_READ_CMD		"/userfs/bin/mtd readflash %s %lu %lu %s"
#define TC_FLASH_WRITE_CMD	"/userfs/bin/mtd writeflash %s %lu %lu %s"

#if !defined(TCSUPPORT_CT) 
//********************************
//	sectors define of reserve area 	//
//********************************
#define RESERVEAREA_NAME "reservearea"
#define RESERVEAREA_ERASE_SIZE 0x10000 //this define should be changed baccording to  flash erase size
#define RESERVEAREA_BLOCK_BASE 0

//#if defined( TCSUPPORT_RESERVEAREA_1_BLOCK)

//#elif defined( TCSUPPORT_RESERVEAREA_2_BLOCK)

//#elif defined( TCSUPPORT_RESERVEAREA_3_BLOCK)

#if (TCSUPPORT_RESERVEAREA_BLOCK==1)
/*
---------------------------------------------------------------------------------
@reserve area table@
|sector		name				cover area				note		
|0			eeprom				0x00000~0x003ff			1k(reserve 1k,no use 256 bytes)
|1			cwmppara			0x00400~0x00600			512 Bytes
|2			not use now			0x00601~0x00fff			(4k-1k-512 Bytes)
|3			mrd					0x01000~0x01fff			4k
|4			rom-t				0x02000~0x0ffff			56k
------------------------------------------------------------------------------------
*/
#define RESERVEAREA_TOTAL_SIZE RESERVEAREA_ERASE_SIZE*1

/*eeprom*/
#define EEPROM_RA_SIZE 0x400
#define EEPROM_RA_OFFSET RESERVEAREA_BLOCK_BASE 

/*device information*/
#ifdef TCSUPPORT_PRODUCTIONLINE
#define PROLINE_CWMPPARA_RA_SIZE 0x180
#define PROLINE_CWMPPARA_RA_OFFSET (EEPROM_RA_OFFSET+EEPROM_RA_SIZE)
#else
#define PROLINE_CWMPPARA_RA_SIZE 0x0
#define PROLINE_CWMPPARA_RA_OFFSET (EEPROM_RA_OFFSET+EEPROM_RA_SIZE)
#endif

#define MRD_RA_SIZE 0x1000
#define MRD_RA_OFFSET (EEPROM_RA_OFFSET + 0x1000) 

#define ROM_T_RA_SIZE 0xe000
#define ROM_T_RA_OFFSET (MRD_RA_OFFSET+MRD_RA_SIZE)

#elif (TCSUPPORT_RESERVEAREA_BLOCK==2)
/*
---------------------------------------------------------------------------------
@reserve area table@
|sector		name				cover area				note
|0			backupromfile 		0~0xffff					64k
|1			syslog				0x10000~0x10000			
|2			eeprom				0x10000~0x103ff			1k(reserve 1k,no use 256 bytes)
|3			cwmppara			0x00400~0x00600			512 Bytes
|4			not use now			0x00601~0x00fff			(4k-1k-512 Bytes)
|5			mrd				0x01000~0x01fff			4k
|6			rom-t				0x02000~0x0ffff			56k
------------------------------------------------------------------------------------
*/
#define RESERVEAREA_TOTAL_SIZE RESERVEAREA_ERASE_SIZE*2
/*backupromfile*/
#define BACKUPROMFILE_RA_SIZE 0x10000
#define BACKUPROMFILE_RA_OFFSET RESERVEAREA_BLOCK_BASE
/*defaultromfile*/
//#define DEFAULTROMFILE_RA_SIZE 0x0
//#define DEFAULTROMFILE_RA_OFFSET (BACKUPROMFILE_RA_OFFSET+BACKUPROMFILE_RA_SIZE)
/*syslog*/
#define SYSLOG_RA_SIZE 0x0
//#define SYSLOG_RA_OFFSET (DEFAULTROMFILE_RA_OFFSET+DEFAULTROMFILE_RA_SIZE)
#define SYSLOG_RA_OFFSET (BACKUPROMFILE_RA_OFFSET+BACKUPROMFILE_RA_SIZE)
/*eeprom*/
#define EEPROM_RA_SIZE 0x400
#define EEPROM_RA_OFFSET (SYSLOG_RA_OFFSET+SYSLOG_RA_SIZE)

/*device information*/
#ifdef TCSUPPORT_PRODUCTIONLINE
#define PROLINE_CWMPPARA_RA_SIZE 0x180
#define PROLINE_CWMPPARA_RA_OFFSET (EEPROM_RA_OFFSET+EEPROM_RA_SIZE)
#else
#define PROLINE_CWMPPARA_RA_SIZE 0x0
#define PROLINE_CWMPPARA_RA_OFFSET (EEPROM_RA_OFFSET+EEPROM_RA_SIZE)
#endif

#define MRD_RA_SIZE 0x1000
#define MRD_RA_OFFSET (EEPROM_RA_OFFSET + 0x1000)

#define ROM_T_RA_SIZE 0xe000
#define ROM_T_RA_OFFSET (MRD_RA_OFFSET+MRD_RA_SIZE)

#elif (TCSUPPORT_RESERVEAREA_BLOCK==3)
/*
---------------------------------------------------------------------------------
@reserve area table@
|sector		name				cover area				note
|0			backupromfile 		0~0xffff					64k
|1			defaultromfile		0x10000~0x1ffff			64k
|2			eeprom			0x20000~0x203ff			1k(reserve 1k,no use 256 bytes)
|3			cwmppara			0x00400~0x00600			384 Bytes
|4			not use now			0x00601~0x00fff			(4k-1k-512 Bytes)
|5			mrd					0x01000~0x01fff			4k
|6			rom-t				0x02000~0x0ffff			56k
------------------------------------------------------------------------------------
*/
#define RESERVEAREA_TOTAL_SIZE RESERVEAREA_ERASE_SIZE*3
/*backupromfile*/
#define BACKUPROMFILE_RA_SIZE 0x10000
#define BACKUPROMFILE_RA_OFFSET RESERVEAREA_BLOCK_BASE
/*defaultromfile*/
#define DEFAULTROMFILE_RA_SIZE 0x10000
#define DEFAULTROMFILE_RA_OFFSET (BACKUPROMFILE_RA_OFFSET+BACKUPROMFILE_RA_SIZE)
/*eeprom*/
#define EEPROM_RA_SIZE 0x400
#define EEPROM_RA_OFFSET (DEFAULTROMFILE_RA_SIZE+DEFAULTROMFILE_RA_OFFSET)

/*device information*/
#ifdef TCSUPPORT_PRODUCTIONLINE
#define PROLINE_CWMPPARA_RA_SIZE 0x180
#define PROLINE_CWMPPARA_RA_OFFSET (EEPROM_RA_OFFSET+EEPROM_RA_SIZE)
#else
#define PROLINE_CWMPPARA_RA_SIZE 0x0
#define PROLINE_CWMPPARA_RA_OFFSET (EEPROM_RA_OFFSET+EEPROM_RA_SIZE)
#endif

#define MRD_RA_SIZE 0x1000
#define MRD_RA_OFFSET (EEPROM_RA_OFFSET + 0x1000)

#define ROM_T_RA_SIZE 0xe000
#define ROM_T_RA_OFFSET (MRD_RA_OFFSET+MRD_RA_SIZE)

#else
//defined( TCSUPPORT_RESERVEAREA_BLOCK == 4)
/*
---------------------------------------------------------------------------------
@reserve area table@
|sector		name				cover area				note
|0			backupromfile 		0~0xffff					64k
|1			defaultromfile			0x10000~0x1ffff			64k
|2			syslog				0x20000~0x2ffff			64k
|3			eeprom				0x30000~0x303ff			1k(reserve 1k,no use 256 bytes)
|4			cwmppara			0x00400~0x00600			512 Bytes
|5			not use now			0x00601~0x00fff			(4k-1k-512 Bytes)
|6			mrd					0x01000~0x01fff			4k
|7			rom-t				0x02000~0x0ffff			56k
------------------------------------------------------------------------------------
*/
#define RESERVEAREA_TOTAL_SIZE RESERVEAREA_ERASE_SIZE*4
#ifdef TCSUPPORT_WLAN_AC
#define MAX_EEPROM_BIN_FILE_SIZE	512
#endif

#if defined(TCSUPPORT_NAND_BADBLOCK_CHECK)	//for RT63368 with NAND

/*syslog*/
#define SYSLOG_RA_SIZE 0x20000
#define SYSLOG_RA_OFFSET RESERVEAREA_BLOCK_BASE

/*backupromfile*/
#define BACKUPROMFILE_RA_SIZE 0x20000
#define BACKUPROMFILE_RA_OFFSET (SYSLOG_RA_OFFSET + SYSLOG_RA_SIZE * 11)

/*eeprom*/
#define EEPROM_RA_SIZE 0x400
#define EEPROM_RA_OFFSET (BACKUPROMFILE_RA_OFFSET + BACKUPROMFILE_RA_SIZE * 15)

#elif defined(TCSUPPORT_NAND_FLASH)	//for all other NAND model

/*backupromfile*/
#define BACKUPROMFILE_RA_SIZE 0x10000
#define BACKUPROMFILE_RA_OFFSET RESERVEAREA_BLOCK_BASE
/*defaultromfile*/
#define DEFAULTROMFILE_RA_SIZE 0x10000
#define DEFAULTROMFILE_RA_OFFSET (BACKUPROMFILE_RA_OFFSET+BACKUPROMFILE_RA_SIZE)
/*syslog*/
#define SYSLOG_RA_SIZE 0x20000
#define SYSLOG_RA_OFFSET (DEFAULTROMFILE_RA_OFFSET+DEFAULTROMFILE_RA_SIZE)
/*eeprom*/
#define EEPROM_RA_SIZE 0x400
#define EEPROM_RA_OFFSET (SYSLOG_RA_OFFSET+SYSLOG_RA_SIZE)

#ifdef TCSUPPORT_WLAN_AC
#define EEPROM_RA_AC_OFFSET (EEPROM_RA_OFFSET+MAX_EEPROM_BIN_FILE_SIZE)
#endif

#else	//for all SPI flash model

/*backupromfile*/
#define BACKUPROMFILE_RA_SIZE 0x10000
#define BACKUPROMFILE_RA_OFFSET RESERVEAREA_BLOCK_BASE
/*defaultromfile*/
#define DEFAULTROMFILE_RA_SIZE 0x10000
#define DEFAULTROMFILE_RA_OFFSET (BACKUPROMFILE_RA_OFFSET+BACKUPROMFILE_RA_SIZE)
/*syslog*/
#define SYSLOG_RA_SIZE 0x10000
#define SYSLOG_RA_OFFSET (DEFAULTROMFILE_RA_OFFSET+DEFAULTROMFILE_RA_SIZE)
/*eeprom*/
#define EEPROM_RA_SIZE 0x400
#define EEPROM_RA_OFFSET (SYSLOG_RA_OFFSET+SYSLOG_RA_SIZE)

#ifdef TCSUPPORT_WLAN_AC
#define EEPROM_RA_AC_OFFSET (EEPROM_RA_OFFSET+MAX_EEPROM_BIN_FILE_SIZE)
#endif

#endif

/*device information*/
#ifdef TCSUPPORT_PRODUCTIONLINE
#define PROLINE_CWMPPARA_RA_SIZE 0x180
#define PROLINE_CWMPPARA_RA_OFFSET (EEPROM_RA_OFFSET+EEPROM_RA_SIZE)
#else
#define PROLINE_CWMPPARA_RA_SIZE 0x0
#define PROLINE_CWMPPARA_RA_OFFSET (EEPROM_RA_OFFSET+EEPROM_RA_SIZE)
#endif

#define MRD_RA_SIZE 0x1000
#define MRD_RA_OFFSET (EEPROM_RA_OFFSET + 0x1000)

#define ROM_T_RA_SIZE 0xe000
#define ROM_T_RA_OFFSET (MRD_RA_OFFSET+MRD_RA_SIZE)
#endif
//Andy Chiu, 2015/05/05, reserve 128 for PL data
#define PROLINE_PLDATA_RA_SIZE 0x80
#define PROLINE_PLDATA_RA_OFFSET (PROLINE_CWMPPARA_RA_OFFSET + 0x180)
#endif
#endif
#endif
