#ifndef RT63165_NAND_H
#define RT63165_NAND_H
#include <linux/mtd/mtd.h>

#if !defined (__UBOOT__)
//#include <asm/rt2880/rt_mmap.h>
#else
//#include <rt_mmap.h>
#define EIO      	5  /* I/O error */
#define EINVAL      22  /* Invalid argument */
#define ENOMEM      12  /* Out of memory */
#define EFAULT		14  /* Out of accessible address space */
#define EBADMSG     74  /* Not a data message */
#define EUCLEAN     117 /* Structure needs cleaning */
#endif

#define ECC_NO_ERR 0
#define ECC_ONE_BIT_ERR -6
#define ECC_DATA_ERR -7
#define ECC_CODE_ERR -8
#define ECC_NFC_CONFLICT -9


/* frankliao deleted 20100916 */
//1#include "gdma.h"

#define ra_inl(addr)  (*(volatile unsigned int *)(addr))
#define ra_outl(addr, value)  (*(volatile unsigned int *)(addr) = (value))
#define ra_aor(addr, a_mask, o_value)  ra_outl(addr, (ra_inl(addr) & (a_mask)) | (o_value))

/* FOR ID frankliao added 20101012 */
#define NAND_CMD1_READID		0x90
#define NAND_CONF_READID		0x410101

/* NAND Reg Verify Type */
#define REG_DEF_CHK 1
#define REG_RW_CHK  2

/* Flash Type */
#define STANDARD_SMALL_FLASH	(0)
#define STANDARD_LARGE_FLASH	(1)

#define NONE					(-1)

/* Manufacturers */
#define MANUFACTURER_ST3A 		0x20
#define MANUFACTURER_ST4A 		0x0020
#define	MANUFACTURER_MIRCON		0x2c
#define MANUFACTURER_ZENTEL		0x92 // to support Zentel , Byron 2012.10.26
#define MANUFACTURER_SAMSUNG  		0xec
#define MANUFACTURER_SPANSION		0x01

/* ST Device ID */
#define ST128W3A				0x73
#define ST512W3A				0x76

/* MICRON Device ID */
#define MT29F2G08AAD			0xda
#define MT29F4G08AAC			0xdc

/* Zentel Device ID */
#define A5U1GA31ATS			0xf1 // to support Zentel, Byron 2012.10.26

/* SAMSUNG Device ID*/
#define K9F1G08U0D         		0xf1

/* SPANSION Device ID*/
#define S34ML01G1			0xf1

/* SIZE BIT*/
#define SIZE_512MiB_BIT			(29)
#define SIZE_256MiB_BIT			(28)
#define SIZE_128MiB_BIT			(27)
#define SIZE_64MiB_BIT			(26)
#define SIZE_16MiB_BIT			(24)
#define SIZE_128KiB_BIT			(17)
#define SIZE_16KiB_BIT			(14)
#define SIZE_4KiB_BIT			(12)
#define SIZE_2KiB_BIT			(11)
#define SIZE_512iB_BIT			(9)
#define SIZE_64iB_BIT			(6)
#define SIZE_16iB_BIT			(4)


/* SIZE BYTE */
#define SIZE_512M_BYTES			(512)
#define SIZE_2K_BYTES			(2048)

#define NFC_BASE						(0xbfbe0000)
#define NFC_CTRL						(NFC_BASE + 0x10)
#define NFC_CONF						(NFC_BASE + 0x14)
#define NFC_CMD1						(NFC_BASE + 0x18)
#define NFC_CMD2						(NFC_BASE + 0x1c)
#define NFC_CMD3						(NFC_BASE + 0x20)
#define NFC_ADDR						(NFC_BASE + 0x24)
#define NFC_DATA						(NFC_BASE + 0x28)
#define NFC_STATUS						(NFC_BASE + 0x30)
#define NFC_INT_EN						(NFC_BASE + 0x34)
#define NFC_INT_ST						(NFC_BASE + 0x38)

#ifdef CONFIG_MIPS_RT63165

#define NFC_ECC							(NFC_BASE + 0x2c)
#define NFC_ADDRII						(NFC_BASE + 0x3c)
#define NFC_ECCII						(NFC_BASE + 0x40)
#define NFC_ECCIII						(NFC_BASE + 0x44)
#define NFC_ECCIV						(NFC_BASE + 0x48)
#define NFC_ECC_STII					(NFC_BASE + 0x4c)
#define NFC_ECC_STIII					(NFC_BASE + 0x50)
#define NFC_CTRLII						(NFC_BASE + 0x54)

#else

#define NFC_CTRLII						(NFC_BASE + 0x3c)
#define NFC_ECC							(NFC_BASE + 0x40)
#define NFC_ECCII						(NFC_BASE + 0x44)
#define NFC_ECCIII						(NFC_BASE + 0x48)
#define NFC_ECCIV						(NFC_BASE + 0x4c)
#define NFC_ECC_ST						(NFC_BASE + 0x50)
#define NFC_ECC_STII					(NFC_BASE + 0x54)
#define NFC_ECC_STIII					(NFC_BASE + 0x58)
#define NFC_ECC_STIV					(NFC_BASE + 0x5c)
#define NFC_ADDRII						(NFC_BASE + 0x60)

#endif

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK

#define TCROMFILE_BLOCK_NUM 13
#define TCBKROMFILE_BLOCK_NUM 13

#ifdef CONFIG_DUAL_IMAGE
#define TCLINUX_BLOCK_NUM  256
#else
#define TCLINUX_BLOCK_NUM  512
#endif

#define RWFS_BLOCK_NUM 464

#define TCSYSLOG_BLOCK_NUM 11
#define TCWIFI_BLOCK_NUM 2

#define TCROMFILE_START 7
#define TCROMFILE_END   (TCROMFILE_START + TCROMFILE_BLOCK_NUM)

#define TCLINUX_BLOCK_START  20
#define TCLINUX_BLOCK_END    (TCLINUX_BLOCK_START + TCLINUX_BLOCK_NUM)

#ifdef CONFIG_DUAL_IMAGE
#define TCLINUX_SLAVE_BLOCK_START  276
#define TCLINUX_SLAVE_BLOCK_END    (TCLINUX_SLAVE_BLOCK_START + TCLINUX_BLOCK_NUM)
#endif

#define RWFS_BLOCK_START 532
#define RWFS_BLOCK_END RWFS_BLOCK_START + RWFS_BLOCK_NUM

#define TCSYSLOG_START 996
#define TCSYSLOG_END (TCSYSLOG_START + TCSYSLOG_BLOCK_NUM)

#define TCBKROMFILE_START 1007
#define TCBKROMFILE_END   (TCBKROMFILE_START + TCBKROMFILE_BLOCK_NUM)

#define TCWIFI_START 1022
#define TCWIFI_END   (TCWIFI_START + TCWIFI_BLOCK_NUM)

#endif


enum _int_stat {
	INT_ST_ND_DONE 	= 1<<0,
	INT_ST_TX_BUF_RDY   = 1<<1,
	INT_ST_RX_BUF_RDY	= 1<<2,
	INT_ST_ECC_ERR		= 1<<3,
	INT_ST_TX_TRAS_ERR	= 1<<4,
	INT_ST_RX_TRAS_ERR	= 1<<5,
	INT_ST_TX_KICK_ERR	= 1<<6,
	INT_ST_RX_KICK_ERR  = 1<<7
};


//#define WORKAROUND_RX_BUF_OV 1

//chip
#define CONFIG_NUMCHIPS 1
/* frankliao marked, rt63165 has support WP */
//#define CONFIG_NOT_SUPPORT_WP 
/* frankliao marked, rt63165 has support RB */
//#define CONFIG_NOT_SUPPORT_RB

#define SMALL_FLASH_ECC_BYTES 	3			//! ecc has 3 bytes
#define SMALL_FLASH_ECC_OFFSET 	5		//! ecc starts from offset 5.

#define LARGE_FLASH_ECC_BYTES 	12		//! ecc has 12 bytes
// frank modify 20110415
#ifdef CONFIG_MIPS_RT63365
#define LARGE_FLASH_ECC_OFFSET 	5		//! ecc starts from offset 5.
#else
#define LARGE_FLASH_ECC_OFFSET 	52		//! ecc starts from offset 52.
#endif

/* Status bits */
#define NAND_STATUS_FAIL		0x01
#define NAND_STATUS_FAIL_N1		0x02
#define NAND_STATUS_TRUE_READY	0x20
#define NAND_STATUS_READY		0x40
#define NAND_STATUS_WP			0x80

typedef enum {
	FL_READY,
	FL_READING,
	FL_WRITING,
	FL_ERASING,
	FL_SYNCING,
	FL_CACHEDPRG,
	FL_PM_SUSPENDED,
} nand_state_t;

/*************************************************************/

typedef enum _ra_flags {
	FLAG_NONE	= 0,
	FLAG_ECC_EN 	= (1<<0),
	FLAG_USE_GDMA 	= (1<<1),
	FLAG_VERIFY 	= (1<<2),
} RA_FLAGS;

#define BBTTAG_BITS		2
#define BBTTAG_BITS_MASK	((1<<BBTTAG_BITS) -1)
enum BBT_TAG {
	BBT_TAG_UNKNOWN = 0, //2'b01
	BBT_TAG_GOOD	= 3, //2'b11
	BBT_TAG_BAD	= 2, //2'b10
	BBT_TAG_RES	= 1, //2'b01
};


struct nand_opcode {
	const int type;
	const int read1;
	const int read2;
	const int readB;
	const int readoob;
	const int pageprog1;
	const int pageprog2;
	const int writeoob;
	const int erase1;
	const int erase2;
	const int status;
	const int reset;
};


struct nand_info {
	const int mfr_id;
	const int dev_id;
	const char *name;
	const int numchips;
	const int chip_shift;
	const int page_shift;
	const int erase_shift;
	const int oob_shift;
	const int badblockpos;
	const int opcode_type;
};


struct ra_nand_chip {

	struct nand_info *flash;

#if !defined (__UBOOT__)
	struct mutex hwcontrol;
	struct mutex *controller;
#endif

	struct nand_ecclayout	*oob;
	struct nand_opcode		*opcode;
	int 	state;
	unsigned int 	buffers_page;
	unsigned char	*buffers; //[CFG_PAGESIZE + CFG_PAGE_OOBSIZE];
	unsigned char 	*readback_buffers;
	unsigned char 	*bbt;
#if defined (WORKAROUND_RX_BUF_OV)
	unsigned int	 sandbox_page;	// steal a page (block) for read ECC verification
#endif

};

//fixme, gdma api 
int nand_dma_sync(void);
void release_dma_buf(void);
int set_gdma_ch(unsigned long dst, 
		unsigned long src, unsigned int len, int burst_size,
		int soft_mode, int src_req_type, int dst_req_type,
		int src_burst_mode, int dst_burst_mode);

#endif
