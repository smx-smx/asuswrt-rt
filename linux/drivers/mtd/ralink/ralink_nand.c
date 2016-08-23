#if defined (__UBOOT__)
#include <common.h>
#include <malloc.h>
#include <linux/stddef.h>
#include <linux/mtd/compat.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/mtd-abi.h>
#include <linux/mtd/partitions.h>
#include "ralink_nand.h"

#define EIO      	5   /* I/O error */
#define EINVAL      22  /* Invalid argument */
#define ENOMEM      12  /* Out of memory */
#define EFAULT		14  /* Out of accessible address space */

#define __devinit
#define __devexit
#if 0
#define NULL_DEFINED( ... )     do{}while(0)
#define NULL_DEF_RET_1( ... )   (1)
#define NULL_DEF_RET_0( ... )   (0)

#define HZ 1
#define schedule_timeout(a)     udelay(1000000*(a))
#define cond_resched()   	   	NULL_DEF_RET_0()
#endif

#else // !defined (__UBOOT__)
															   
#define DEBUG
#include <linux/device.h>
#undef DEBUG
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/mtd/mtd.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/mtd/partitions.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/sched.h>

#include <asm/byteorder.h>
#include <linux/proc_fs.h>
#include "ralink_nand.h"

#include <linux/mtd/map.h>
#include <asm/tc3162/tc3162.h>
#include "gdma.h"
//#include <linux/mtd/gen_probe.h>

#ifdef TCSUPPORT_NAND_RT63368
#include "bmt.h"
#endif

#endif// !defined (__UBOOT__)


/* frankliao modify 1000 ==> 1000000 */
#define RETRY_NUMBER			1000000
#define NFC_BIG_ENDIAN			0x02
#define NFC_LITTLE_ENDIAN 		0x0

/* frankliao modify 20101215 */
struct mtd_info *ranfc_mtd = NULL;
struct ra_nand_chip *ra = NULL;

//static int skipbbt = 0;
static int ranfc_debug = 0;
static int ranfc_bbt = 1;
static int ranfc_verify = 1;
static int ranfc_flags = 0;
static int ranfc_page = 0;
static int column_addr_cycle = 0;
static int row_addr_cycle = 0;
static int addr_cycle = 0;

#ifdef TCSUPPORT_NAND_RT63368
#define BMT_BAD_BLOCK_INDEX_OFFSET (1)
#define POOL_GOOD_BLOCK_PERCENT 8/100
#define SLAVE_IMAGE_OFFSET 0xf00000
#define SIZE_2KiB_BYTES (2048)
#define SIZE_64iB_BYTES (64)
static int bmt_pool_size = 0;
static bmt_struct *g_bmt = NULL;
static init_bbt_struct *g_bbt = NULL;
extern int nand_logic_size;
#endif

#if !defined (__UBOOT__)
module_param(ranfc_debug, int, 0644);
module_param(ranfc_bbt, int, 0644);
module_param(ranfc_verify, int, 0644);
#endif 

extern unsigned long flash_base;
extern unsigned int (*ranand_read_byte)(unsigned long long);
extern unsigned int (*ranand_read_dword)(unsigned long long);
/* for test */
extern void prom_printf(const char *fmt, ...);

#if 0
#define ra_dbg(args...) do { if (ranfc_debug) printk(args); } while(0)
#else
#define ra_dbg(args...)
#endif

#define CLEAR_INT_STATUS() 	ra_outl(NFC_INT_ST, ra_inl(NFC_INT_ST))
#define NFC_TRANS_DONE() 	(ra_inl(NFC_INT_ST) & INT_ST_ND_DONE)

#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
extern int offset;
#endif
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
static int block_table[1024];
int nand_erase_next_goodblock(struct ra_nand_chip *ra, int block, unsigned long addr_l);
int nand_write_next_goodblock(struct ra_nand_chip *ra, int page_u, int page_l);
int nand_partition_check(int block);
int nand_is_rwfs_partition(struct ra_nand_chip *ra, int *block, int *page, unsigned long *addr);
#endif

#ifdef TCSUPPORT_NAND_RT63368
int calc_bmt_pool_size(struct ra_nand_chip *ra);
#endif

static struct mtd_partition rt63165_test_partitions[] = {
	{									 	/* First partition */
		  name 	     : "NAND Flash",	 	/* Section */
		  size	  	 : 0x0,		 		 	/* Size  */
		  offset     : 0				 	/* Offset from start of flash- location 0x0*/
	},
};


static struct nand_opcode opcode_tables[] = {
	{
		type: STANDARD_SMALL_FLASH,
		read1: 0x00,
		read2: NONE,
		readB: 0x01,
		readoob: 0x50,
		pageprog1: 0x8000,
		pageprog2: 0x10,
		writeoob: 0x8050,
		erase1: 0x60,
		erase2: 0xd0,
		status: 0x70,
		reset: 0xff,
	},
	{
		type: STANDARD_LARGE_FLASH,
		read1: 0x00,
		read2: 0x30,
		readB: NONE,
		readoob: 0x00,
		pageprog1: 0x80,
		pageprog2: 0x10,
		writeoob: 0x80,
		erase1: 0x60,
		erase2: 0xd0,
		status: 0x70,
		reset: 0xff,
	},
};


static struct nand_info flash_tables[] = {
	{
		mfr_id: MANUFACTURER_ST3A,
		dev_id: ST128W3A,
		name: "ST NAND128W3A",
		numchips: (1),
		chip_shift: SIZE_16MiB_BIT,
		page_shift: SIZE_512iB_BIT,
		erase_shift: SIZE_16KiB_BIT,
		oob_shift: SIZE_16iB_BIT,
		badblockpos: (4),		//512 pagesize bad blk offset --> 4
		opcode_type: STANDARD_SMALL_FLASH,
	},
	{
		mfr_id: MANUFACTURER_ST3A,
		dev_id: ST512W3A,
		name: "ST NAND512W3A",
		numchips: (1),
		chip_shift: SIZE_64MiB_BIT,
		page_shift: SIZE_512iB_BIT,
		erase_shift: SIZE_16KiB_BIT,
		oob_shift: SIZE_16iB_BIT,
		badblockpos: (4),		//512 pagesize bad blk offset --> 4
		opcode_type: STANDARD_SMALL_FLASH,
	},
	{
		mfr_id: MANUFACTURER_ZENTEL,
		dev_id: A5U1GA31ATS,
		name:   "ZENTEL NAND1GA31ATS",
		numchips: (1),
		chip_shift: SIZE_128MiB_BIT,
		page_shift: SIZE_2KiB_BIT,
		erase_shift: SIZE_128KiB_BIT,
		oob_shift: SIZE_64iB_BIT,
		badblockpos: (51),
		opcode_type: STANDARD_LARGE_FLASH,

	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S34ML01G1,
		name:   "SPANSION S34ML01G1",
		numchips: (1),
		chip_shift: SIZE_128MiB_BIT,
		page_shift: SIZE_2KiB_BIT,
		erase_shift: SIZE_128KiB_BIT,
		oob_shift: SIZE_64iB_BIT,
		badblockpos: (0),
		opcode_type: STANDARD_LARGE_FLASH,
	},
	{
		mfr_id: MANUFACTURER_MIRCON,
		dev_id: MT29F2G08AAD,
		name:	"MICRON NAND2G08AAD",
		numchips: (1),
		chip_shift: SIZE_256MiB_BIT,
		page_shift: SIZE_2KiB_BIT,
		erase_shift: SIZE_128KiB_BIT,
		oob_shift: SIZE_64iB_BIT,
		badblockpos: (51),
		opcode_type: STANDARD_LARGE_FLASH,
	},
	{
		mfr_id: MANUFACTURER_MIRCON,
		dev_id: MT29F4G08AAC,
		name:	"MICRON NAND4G08AAC",
		numchips: (1),
		chip_shift: SIZE_512MiB_BIT,
		page_shift: SIZE_2KiB_BIT,
		erase_shift: SIZE_128KiB_BIT,
		oob_shift: SIZE_64iB_BIT,
		badblockpos: (51),
		opcode_type: STANDARD_LARGE_FLASH,
	},
	{
		mfr_id: MANUFACTURER_SAMSUNG,
		dev_id: K9F1G08U0D,
		name:   "SAMSUNG K9F1G08U0D",
		numchips: (1),
		chip_shift: SIZE_128MiB_BIT,
		page_shift: SIZE_2KiB_BIT,
		erase_shift: SIZE_128KiB_BIT,
		oob_shift: SIZE_64iB_BIT,
		badblockpos: (0),
		opcode_type: STANDARD_LARGE_FLASH,

	},
};


static struct nand_ecclayout oob_layout_tables[] = {
	/* 512iB page size flash */
	{
		.eccbytes = SMALL_FLASH_ECC_BYTES,
		.eccpos = {SMALL_FLASH_ECC_OFFSET, SMALL_FLASH_ECC_OFFSET+1, SMALL_FLASH_ECC_OFFSET+2},
		.oobfree = {
			 {.offset = 0, .length = 4},
			 {.offset = 8, .length = 8},
			 {.offset = 0, .length = 0}
		 },
		.oobavail = 12,
		// 4th byte is bad-block flag.
	},
	/* 2K page size flash */
	{
		.eccbytes = LARGE_FLASH_ECC_BYTES,
		#ifdef CONFIG_MIPS_RT63365		
		.eccpos = { 
					LARGE_FLASH_ECC_OFFSET,   LARGE_FLASH_ECC_OFFSET+1,  LARGE_FLASH_ECC_OFFSET+2,
					LARGE_FLASH_ECC_OFFSET+16, LARGE_FLASH_ECC_OFFSET+17,  LARGE_FLASH_ECC_OFFSET+18,
					LARGE_FLASH_ECC_OFFSET+32, LARGE_FLASH_ECC_OFFSET+33,  LARGE_FLASH_ECC_OFFSET+34,
					LARGE_FLASH_ECC_OFFSET+48, LARGE_FLASH_ECC_OFFSET+49, LARGE_FLASH_ECC_OFFSET+50
		},
		.oobfree = {
		#ifdef TCSUPPORT_NAND_RT63368
			 {.offset = 4, .length = 1},
		#else
			 {.offset = 0, .length = 5},
		#endif
			 {.offset = 8, .length = 13},
			 {.offset = 24, .length = 13},
			 {.offset = 40, .length = 11},
			 {.offset = 52, .length = 1},
			 {.offset = 56, .length = 8}
		},
		#else	
		.eccpos = { 
					LARGE_FLASH_ECC_OFFSET,   LARGE_FLASH_ECC_OFFSET+1,  LARGE_FLASH_ECC_OFFSET+2,
					LARGE_FLASH_ECC_OFFSET+3, LARGE_FLASH_ECC_OFFSET+4,  LARGE_FLASH_ECC_OFFSET+5,
					LARGE_FLASH_ECC_OFFSET+6, LARGE_FLASH_ECC_OFFSET+7,  LARGE_FLASH_ECC_OFFSET+8,
					LARGE_FLASH_ECC_OFFSET+9, LARGE_FLASH_ECC_OFFSET+10, LARGE_FLASH_ECC_OFFSET+11
		},
		.oobfree = {
			 {.offset = 0, .length = 51},
			 {.offset = 0, .length = 0}
		},
		#endif
		#ifdef TCSUPPORT_NAND_RT63368
		.oobavail = 47,
		#else
		.oobavail = 51,
		#endif
		// 2009th byte is bad-block flag.
	}
};


/*************************************************************
 * nfc functions 
 *************************************************************/
static int nfc_wait_ready(struct ra_nand_chip *ra);

/*
unsigned int 
nfc_addr_translate(struct ra_nand_chip *ra, unsigned int addr, unsigned int *column, unsigned int *row)
{
	unsigned int _col, _row;

	_row = (addr >> ra->flash->page_shift);
	_col = addr & ((1<<ra->flash->page_shift) - CONFIG_SUBPAGE_BIT);
	
	if (column) 
		*column = _col;
	if (row) 
		*row = _row;
	
	return  ((_row) << (column_addr_cycle * 8)) | (_col & ((1<<(column_addr_cycle * 8))-1)); 
}*/


/**
 * reset nand chip
 */
static int 
nfc_chip_reset(struct ra_nand_chip *ra)
{
	int status, cmd1;

//	cmd1 = ra->opcode->reset;
	cmd1 = 0xff;
	// reset nand flash
	ra_outl(NFC_CMD1, cmd1);
//	ra_outl(NFC_ADDR, 0xfffffff);
	ra_outl(NFC_CONF, 0x0101 | (addr_cycle << 16));

	status = nfc_wait_ready(ra);  //erase wait 5us
	if (status & NAND_STATUS_FAIL) {
		printk("%s: fail \n", __func__);
	}
	
	return (int)(status & NAND_STATUS_FAIL);
}


/** 
 * clear NFC and flash chip.
 */
static int 
nfc_all_reset(struct ra_nand_chip *ra)
{
	long retry;

	ra_dbg("%s: \n", __func__);

	// reset controller
	ra_outl(NFC_CTRL, ra_inl(NFC_CTRL) | 0x02); //clear data buffer
	ra_outl(NFC_CTRL, ra_inl(NFC_CTRL) & ~0x02); //clear data buffer

	CLEAR_INT_STATUS();

	retry = RETRY_NUMBER;
	while ((ra_inl(NFC_INT_ST) & 0x02) != 0x02 && retry--);
	if (retry <= 0) {
		printk("nfc_all_reset: clean buffer fail \n");
		return -1;
	}

	retry = RETRY_NUMBER;
	while ((ra_inl(NFC_STATUS) & 0x1) != 0x0 && retry--) { //fixme, controller is busy ?
	//	ndelay(1)
	//	mdelay(1);
	}

	nfc_chip_reset(ra);

	return 0;

}


/** NOTICE: only called by nfc_wait_ready().
 * @return -1, nfc can not get transction done 
 * @return 0, ok.
 */
static int 
_nfc_read_status(struct ra_nand_chip *ra, char *status)
{
	unsigned long cmd1, conf;
	unsigned int endian = 0;
	int int_st, nfc_st;
	long retry;

	cmd1 = 0x70;
//	cmd1 = ra->opcode->status;
	conf = 0x000101 | (1 << 20);

	//fixme, should we check nfc status?
	CLEAR_INT_STATUS();

	ra_outl(NFC_CMD1, cmd1); 	
	ra_outl(NFC_CONF, conf); 

	/* FIXME, 
	 * 1. since we have no wired ready signal, directly 
	 * calling this function is not gurantee to read right status under ready state.
	 * 2. the other side, we can not determine how long to become ready, this timeout retry is nonsense.
	 * 3. SUGGESTION: call nfc_read_status() from nfc_wait_ready(),
	 * that is aware about caller (in sementics) and has snooze plused nfc ND_DONE.
	 */
	retry = RETRY_NUMBER; 
	do {
		nfc_st = ra_inl(NFC_STATUS);
		int_st = ra_inl(NFC_INT_ST);	
//		ndelay(1);
	} while (!(int_st & INT_ST_RX_BUF_RDY) && retry--);
		

	if (!(int_st & INT_ST_RX_BUF_RDY)) {
		printk("nfc_read_status: NFC fail, int_st(%x), nfc:%x, reset nfc and flash. \n", 
		       int_st, nfc_st);
		nfc_all_reset(ra);
		*status = NAND_STATUS_FAIL;
		return -1;
	}
	/* frankliao modify 20101001 */
//	*status = (char)(le32_to_cpu(ra_inl(NFC_DATA)) & 0x0ff);
	// frank modify 20110418
	// rt63365 modify
	#ifdef CONFIG_MIPS_RT63365
	/* frank modify 20110420 for rt63365 big endian*/
	*status = (char)(((ra_inl(NFC_DATA)) & 0xff000000)>>24);
	#else	
	/* frankliao modify 20101001 for rt63165 a*/
	*status = (char)((ra_inl(NFC_DATA)) & 0x0ff);
	#endif

	return 0;
}


/**
 * @return !0, chip protect.
 * @return 0, chip not protected.
 */
static int 
nfc_check_wp(struct ra_nand_chip *ra)
{
	/* Check the WP bit */
#if !defined CONFIG_NOT_SUPPORT_WP
//	printk("NFC_CTRL : %x\n", ra_inl(NFC_CTRL));
//	printk("WP BIT : %d\n", ra_inl(NFC_CTRL) & 0x01);
	return !(ra_inl(NFC_CTRL) & 0x01);
#else
	char result = 0;
	int ret;

	ret = _nfc_read_status(ra, &result);
	//fixme, if ret < 0

	return !(result & NAND_STATUS_WP);
#endif

}


#if !defined CONFIG_NOT_SUPPORT_RB
/*
 * @return !0, chip ready.
 * @return 0, chip busy.
 */
static int 
nfc_device_ready(void)
{
	/* Check the ready  */
	return (ra_inl(NFC_STATUS) & 0x04);
}
#endif

/* frankliao added 20101015 */
static int
nfc_read_id(void)
{
	long retry;
	unsigned int ret_data;

	retry = RETRY_NUMBER;
	CLEAR_INT_STATUS();

	ra_outl(NFC_CMD1, NAND_CMD1_READID); 	
	ra_outl(NFC_CONF, NAND_CONF_READID);

	while (retry > 0) {
		int int_st;
		int_st = ra_inl(NFC_INT_ST);

		if (int_st & INT_ST_RX_BUF_RDY) {
			ret_data = ra_inl(NFC_DATA);
			break;
		} else {
//			mdelay(1);
//			ndelay(1);
			retry--;
		}
	}

//	printk("READID : %x\n", ret_data);

	if (retry <= 0) {
		printk("%s: read id fail \n", __func__);
		return -1;
	} else {
		return ret_data;
	}	

}


/**
 * generic function to get data from flash.
 * @return data length reading from flash.
 */
 static int
_ra_nand_pull_data(unsigned char *buf, int len, int use_gdma)
{
	unsigned char *p = buf;
	long retry;

	// receive data by use_gdma 
	if (use_gdma) { 
		if (_ra_nand_dma_pull((unsigned long)p, len)) {
			printk("%s: fail \n", __func__);
			len = -1; //return error
		}
		return len;
	} 

	//fixme: retry count size?
	retry = RETRY_NUMBER;
	// no gdma
	while (len > 0) {
		int int_st = ra_inl(NFC_INT_ST);
		/* frankliao added 20101004 */
		if (int_st & INT_ST_RX_BUF_RDY) {
			unsigned int ret_data;
			int ret_size;
#if 1 //!fixme, need optimal by endian
			ret_data = ra_inl(NFC_DATA);
			/* frankliao added 20101005 */
			ra_outl(NFC_INT_ST, INT_ST_RX_BUF_RDY); 
				
			ret_size = sizeof(unsigned long);
			ret_size = min(ret_size, len);
			len -= ret_size;

			// rt63365 modify
			#ifdef CONFIG_MIPS_RT63365
			*(unsigned int*)p = ret_data;
			p += ret_size;
			#else
			while (ret_size-- > 0) {
				//nfc is little endian 
				*p++ = ret_data & 0x0ff;
				// frankliao test 20101213
//				wmb();
				ret_data >>= 8; 
			}
			#endif			
#else
			//optimaize
#endif
		} else if (int_st & INT_ST_ND_DONE) {
//			printk("!! done\n");
			break;
		} else {
//			ndelay(1);
			if (retry-- < 0) 
				break;
		}
	}

	return (p-buf);
}


/**
 * generic function to put data into flash.
 * @return data length writing into flash.
 */
static int 
_ra_nand_push_data(unsigned char *buf, int len, int use_gdma)
{
	unsigned char *p = buf;
	long retry;

//	printk("LENGTH = %d\n", len);
	// receive data by use_gdma 
	if (use_gdma) { 
		if (_ra_nand_dma_push((unsigned long)p, len))
			len = 0;		
		return len;
	} 

	// no gdma frankliao modify old value : 528
	retry = RETRY_NUMBER;
	while(len > 0) {
		int int_st = ra_inl(NFC_INT_ST);
		if (int_st & INT_ST_TX_BUF_RDY) {
			unsigned int tx_data = 0;
			int tx_size;

#if 1 //fixme, need optimaize in words	
			tx_size = min(len, (int)sizeof(unsigned long));

			// rt63365 modify
			#ifdef CONFIG_MIPS_RT63365
			tx_data = *(unsigned int*)p;
			p += tx_size;
			#else
			int iter;
			for (iter = 0; iter < tx_size; iter++) {
				/* nfc is little endian */
				tx_data |= ((*p++ & 0xff) << (8*iter));
			}

			#endif

			/* frankliao mark */
			ra_outl(NFC_INT_ST, INT_ST_TX_BUF_RDY);
			ra_outl(NFC_DATA, tx_data);

			len -= tx_size;
#else
			//optimaize
#endif
		}
		
		if (int_st & INT_ST_ND_DONE) {	// code review tag 
			break;
		} else {
			/* frankliao modify 20101006 */
//			ndelay(1);
			if (retry-- < 0) {
				ra_dbg("%s p:%p buf:%p \n", __func__, p, buf);
				break;
			}
		}
	}
	
	return (int)(p-buf);

}


static int 
nfc_select_chip(struct ra_nand_chip *ra, int chipnr)
{
#if (CONFIG_NUMCHIPS == 1)
	if (!(chipnr < CONFIG_NUMCHIPS))
		return -1;
	return 0;
#else
	BUG();
#endif
}


/** @return -1: chip_select fail
 *	    0 : both CE and WP==0 are OK
 * 	    1 : CE OK and WP==1
 */
static int 
nfc_enable_chip(struct ra_nand_chip *ra, loff_t offs, int read_only)
{
	/* frankliao fix 20101223 */
	int chipnr = (int)(offs >> ra->flash->chip_shift);

//	ra_dbg("%s: offs:%llx read_only:%x \n", __func__, offs, read_only);
//	ra_dbg("chipnr:%d\n", chipnr);

	chipnr = nfc_select_chip(ra, chipnr);
	if (chipnr < 0) {
		printk("%s: chip select error, offs(%llx)\n", __func__, offs);
		return -1;
	}

	if (!read_only)
		return nfc_check_wp(ra);
	
	return 0;
}


/** wait nand chip becomeing ready and return queried status.
 * @param snooze: sleep time in ms unit before polling device ready.
 * @return status of nand chip
 * @return NAN_STATUS_FAIL if something unexpected.
 */
static int 
nfc_wait_ready(struct ra_nand_chip *ra)
{
	long retry;
	char status;
	
#if 0	
	// wait nfc idle,
	if (snooze_ms == 0)
		snooze_ms = 1;
	else
		schedule_timeout(snooze_ms * HZ / 1000);
	
	snooze_ms = retry = snooze_ms *1000000 / 100 ; 
#endif
	
	retry = RETRY_NUMBER;
	while (!NFC_TRANS_DONE() && retry--) {
//		if (!cond_resched());
//			ndelay(1);
	}

	if (!NFC_TRANS_DONE()) {
		printk("nfc_wait_ready: no transaction done \n");
		return NAND_STATUS_FAIL;
	}

#if !defined (CONFIG_NOT_SUPPORT_RB)
	//fixme
	retry = RETRY_NUMBER;
	while(!(status = nfc_device_ready()) && retry--) {
//		ndelay(1);
	}

	if (status == 0) {
		printk("nfc_wait_ready: no device ready. \n");	
		return NAND_STATUS_FAIL;
	}

	// frank modify 201104221 for device ready issue (ecc verify occurs
	retry = RETRY_NUMBER;
	while (retry--) {
		_nfc_read_status(ra, &status);
		if (status & NAND_STATUS_READY)
			break;
//		ndelay(1);
	}
	
//	_nfc_read_status(ra, &status);

	return status;

#else

	retry = RETRY_NUMBER;
	while (retry--) {
		_nfc_read_status(ra, &status);
		if (status & NAND_STATUS_READY)
			break;
//		ndelay(1);
	}
	if (retry<0)
		printk("nfc_wait_ready 2: no device ready, status(%x). \n", status);	

	return status;

#endif
}


/**
 * return 0: erase OK
 * return -EIO: fail 
 */
static int 
nfc_erase_block(struct ra_nand_chip *ra, int row_addr)
{
	unsigned long cmd1, cmd2, conf;
	char status;

//	cmd1 = 0x60;
//	cmd2 = 0xd0;
	cmd1 = ra->opcode->erase1;
	cmd2 = ra->opcode->erase2;

	
	if (ra->flash->page_shift == SIZE_512iB_BIT) {
		conf = 0x00513 | ((row_addr_cycle)<<16);
	} else {
		conf = 0x00503 | ((row_addr_cycle)<<16);
	}

	ra_dbg("Erase CMD1:%2lx\n", cmd1);
	ra_dbg("Erase CMD2:%2lx\n", cmd2);
	ra_dbg("Erase BLOCK:%2x\n", row_addr);
	ra_dbg("CONFIG:%5lx\n", conf);

	// set NFC
//	printk("%s: cmd1: %lx, cmd2:%lx bus_addr: %x, conf: %lx \n", 
//	       __func__, cmd1, cmd2, bus_addr, conf);

	//fixme, should we check nfc status?
	CLEAR_INT_STATUS();

	ra_outl(NFC_CMD1, cmd1); 	
	ra_outl(NFC_CMD2, cmd2);
	ra_outl(NFC_ADDR, row_addr);
	ra_outl(NFC_CONF, conf); 

	status = nfc_wait_ready(ra);  //erase wait 3ms 
	if (status & NAND_STATUS_FAIL) {
		printk("%s: fail \n", __func__);
		return -EIO;
	}
	
	return 0;

}


static inline int 
_nfc_write_raw_data(struct ra_nand_chip *ra, int cmd1, int cmd3, unsigned long row_addr, 
					unsigned long column_addr, int conf, unsigned char *buf, int len, int flags)
{
	int ret;

	CLEAR_INT_STATUS();
	ra_dbg("In _nfc_write_raw_data function\n");
	ra_dbg("NFC_CMD1:%x\n", cmd1);
	ra_dbg("NFC_CMD3:%x\n", cmd3);
	ra_dbg("ROW_ADDR:%lx\n", row_addr);
	ra_dbg("COLUMN_ADDR:%lx\n", column_addr);
	ra_dbg("NFC_CONF:%x\n", conf);

	ra_outl(NFC_CMD1, cmd1); 	
	ra_outl(NFC_CMD3, cmd3); 	

	if (ra->flash->page_shift == SIZE_2KiB_BIT) {
		ra_outl(NFC_ADDR, (row_addr << (column_addr_cycle << 3)) | column_addr );
		ra_outl(NFC_ADDRII, ((row_addr>>(32-(column_addr_cycle << 3)))));
	} else {
		ra_outl(NFC_ADDR, (row_addr << (column_addr_cycle << 3)) | column_addr );
	}

#if 0
	if (ra->flash->page_shift == SIZE_512iB_BIT) {
		ra_outl(NFC_ADDR, (unsigned int)bus_addr);
	} else {
		ra_outl(NFC_ADDR, (unsigned int)(bus_addr & 0xffffffff));
		ra_outl(NFC_ADDRII, (unsigned int)((bus_addr>>32) & 0xff));
	}
#endif

	ra_outl(NFC_CONF, conf); 

	ret = _ra_nand_push_data(buf, len, flags & FLAG_USE_GDMA);
	if (ret != len) {
		ra_dbg("%s: ret:%x (%x) \n", __func__, ret, len);
		return NAND_STATUS_FAIL;
	}

//	ret = nfc_wait_ready(1); //write wait 1ms
	/* frankliao modify 20101004 */
	ret = nfc_wait_ready(ra); //write wait 1ms
	if (ret & NAND_STATUS_FAIL) {
		printk("%s: fail \n", __func__);
		return NAND_STATUS_FAIL;
	}

	return 0;
}


static inline int 
_nfc_read_raw_data(struct ra_nand_chip *ra, int cmd1, int cmd2, unsigned long row_addr, 
					unsigned long column_addr, int conf, unsigned char *buf, int len, int flags)
{
	int ret;

	ra_dbg("in _nfc_read_raw_data function\n");
	ra_dbg("NFC_CMD1: %x\n", cmd1);
	ra_dbg("cmd1: %x\n", cmd1);
	ra_dbg("row_addr: %x\n", row_addr);
	ra_dbg("column_addr: %x\n", column_addr);
	ra_dbg("NFC_ADDR:%x\n", (row_addr << (column_addr_cycle << 3)) | column_addr );
	ra_dbg("NFC_ADDRII:%x\n",((row_addr>>(32 - (column_addr_cycle << 3)))));
	ra_dbg("conf: %x\n", conf);
	ra_dbg("len : %x\n", len);

	CLEAR_INT_STATUS();
	ra_outl(NFC_CMD1, cmd1); 	

#if 0
	if (ra->flash->page_shift == SIZE_2KiB_BIT)	{
		ra_outl(NFC_CMD2, cmd2); 	
		ra_outl(NFC_ADDR, (unsigned int)(bus_addr & 0xffffffff)); 			
		ra_outl(NFC_ADDRII, (unsigned int)((bus_addr>>32) & 0xff)); 
	} else {	
		ra_outl(NFC_ADDR, (unsigned int)bus_addr);
	}
#endif

	if (ra->flash->page_shift == SIZE_2KiB_BIT) {
		ra_outl(NFC_CMD2, cmd2);
		ra_outl(NFC_ADDR, (row_addr << (column_addr_cycle << 3)) | column_addr );
		ra_outl(NFC_ADDRII, ((row_addr>>(32 - (column_addr_cycle << 3)))));
	} else {
		ra_outl(NFC_ADDR, (row_addr << (column_addr_cycle << 3)) | column_addr );
	}	

	ra_outl(NFC_CONF, conf); 

	ret = _ra_nand_pull_data(buf, len, flags & FLAG_USE_GDMA);

//	ra_dbg("in _nfc_read_raw_data function\n");

	if (ret != len) {
		ra_dbg("%s: ret:%x (%x) \n", __func__, ret, len);
		return NAND_STATUS_FAIL;
	}

	//FIXME, this section is not necessary
	// frankliao modify
	ret = nfc_wait_ready(ra); //wait ready 
//	ret = nfc_wait_ready(0); //wait ready 
	if (ret & NAND_STATUS_FAIL) {
		printk("%s: fail \n", __func__);
		return NAND_STATUS_FAIL;
	} 
	return 0;
}


/**
 * @return !0: fail
 * @return 0: OK
 */
int 
nfc_read_oob(struct ra_nand_chip *ra, int page, unsigned int offs, unsigned char *buf, int len, int flags)
{
	unsigned int cmd1 = 0, cmd2 = 0, conf = 0;
	unsigned long row_addr, column_addr;
	unsigned int ecc_en;
	int use_gdma;
	int status;

	int pages_perblock = 1<<(ra->flash->erase_shift - ra->flash->page_shift);
	// constrain of nfc read function 

#if defined (WORKAROUND_RX_BUF_OV)
	BUG_ON (len > 60); 	//problem of rx-buffer overrun 
#endif
	BUG_ON (offs >> ra->flash->oob_shift); //page boundry
	BUG_ON ((unsigned int)(((offs + len) >> ra->flash->oob_shift) + page) >
		((page + pages_perblock) & ~(pages_perblock-1))); //block boundry

	use_gdma = flags & FLAG_USE_GDMA;
	ecc_en = flags & FLAG_ECC_EN;
	row_addr = page;
	column_addr = offs & ((1 << (column_addr_cycle<<3)) -1);
//	cmd1 = 0x50;
	if (ra->flash->page_shift == SIZE_512iB_BIT) {
		cmd1 = ra->opcode->readoob;
		conf = 0x000141| ((addr_cycle)<<16) | ((len) << 20); 
	} else {	
		cmd1 = ra->opcode->read1;
		cmd2 = ra->opcode->read2;		
		conf = 0x000511| ((addr_cycle)<<16) | ((len) << 20); 
		column_addr |= (1<<11);
	}	

	if (ecc_en) 
		conf |= (1<<3); 
	if (use_gdma)
		conf |= (1<<2);

//	ra_dbg("%s: cmd1: %x, bus_addr: %llx, conf: %x, len:%x, flag:%x\n", 
//	       __func__, cmd1, bus_addr, conf, len, flags);

	status = _nfc_read_raw_data(ra, cmd1, cmd2, row_addr, column_addr, conf, buf, len, flags);
//	ra_dbg("in read_oob function\n");
//	ra_dbg("after read raw data\n");
//	ra_dbg("buf : %02x\n", buf[0] & 0xff);
	if (status & NAND_STATUS_FAIL) {
		printk("%s: fail\n", __func__);
		return -EIO;
	}

	return 0; 
}


/**
 * @return !0: fail
 * @return 0: OK
 */
int 
nfc_write_oob(struct ra_nand_chip *ra, int page, unsigned int offs, unsigned char *buf, int len, int flags)
{
	unsigned int cmd1 = 0, cmd3=0, conf = 0;
//	unsigned long long bus_addr = 0;
	unsigned long row_addr, column_addr;
	int use_gdma;
	int status;

	int pages_perblock = 1<<(ra->flash->erase_shift - ra->flash->page_shift);
	// constrain of nfc read function 

	BUG_ON (offs >> ra->flash->oob_shift); //page boundry
	BUG_ON ((unsigned int)(((offs + len) >> ra->flash->oob_shift) + page) >
		((page + pages_perblock) & ~(pages_perblock-1))); //block boundry 

	use_gdma = flags & FLAG_USE_GDMA;

	row_addr = page;
	column_addr = offs & ((1<<(column_addr_cycle<<3)) - 1);

//	bus_addr = (((unsigned long long) page << (column_addr_cycle*8)) | 
//				((unsigned long long) offs & ((1<<column_addr_cycle*8) - 1)));
	
	cmd1 = ra->opcode->writeoob;
	cmd3 = ra->opcode->pageprog2;
	
	/* frankliao modify 20100105 */	
	if (ra->flash->page_shift == SIZE_512iB_BIT) {
		conf = 0x001243 | ((addr_cycle)<<16) | ((len) << 20); 
	} else if (ra->flash->page_shift == SIZE_2KiB_BIT) {
		conf = 0x001103 | ((addr_cycle)<<16) | ((len) << 20); 
		column_addr |= (1<<11);
	} else {
		printk("Undefined Write Page Command\n");
		return -EIO;
	}
	
#if 0
	/* frankliao added 20101015 */	
	if (cmd1 <= 0xff && cmd1 >= 0x0)
		conf = 0x001123 | ((addr_cycle)<<16) | ((len) << 20); 
	else if (cmd1 > 0xff && cmd1 <= 0xffff)
		conf = 0x001223 | ((addr_cycle)<<16) | ((len) << 20); 
	else {
		printk("Undefine write oob command\n");
		return -EIO;
	}
#endif

	if (use_gdma)
		conf |= (1<<2);

	// set NFC
	ra_dbg("%s: cmd1: %x, cmd3: %x row_addr: %lx, column_addr: %lx, conf: %x, len:%x\n", 
	       __func__, cmd1, cmd3, row_addr, column_addr, conf, len);

	status = _nfc_write_raw_data(ra, cmd1, cmd3, row_addr, column_addr, conf, buf, len, flags);
	if (status & NAND_STATUS_FAIL) {
		printk("%s: fail \n", __func__);
		return -EIO;
	}

	return 0; 
}


int nfc_read_page(struct ra_nand_chip *ra, unsigned char *buf, int page, int flags);
int nfc_write_page(struct ra_nand_chip *ra, unsigned char *buf, int page, int flags);
	

/*nfc_ecc_err_handler
for 512 byte/page
	return:
		ECC_NO_ERR: no error
		ECC_CODE_ERR: ECC code error
		ECC_DATA_ERR: more than 1 bit error, un-correctable
		ECC_ONE_BIT_ERR: 1 bit correctable error
		ECC_NFC_CONFLICT: software check result conflict with HW check result
*/
int nfc_ecc_err_handler(int page_index , unsigned char *ecc_from_oob,
unsigned char *ecc_from_nfc, unsigned long *error_byte_index, unsigned long *error_bit_index){
	unsigned long old_ecc = 0;
	unsigned long new_ecc = 0;
	unsigned long ecc_rst = 0;
	int ecc_bit_index = 0;
	int ecc_bit1_cnt = 0;
	unsigned long temp = 0;

	memcpy((unsigned char *)&old_ecc + 1 , ecc_from_oob , 3);
	memcpy((unsigned char *)&new_ecc + 1 , ecc_from_nfc , 3);


	ecc_rst = old_ecc^new_ecc;


	if(ecc_rst == 0){//no ecc error
		return ECC_NO_ERR;
	}else{
		for(ecc_bit_index = 0; ecc_bit_index< 24; ecc_bit_index++ ){
			if((ecc_rst&(1<<ecc_bit_index)) != 0){
				ecc_bit1_cnt++;
			}
		}
		printk("\r\n ecc_rst= 0x%08x, ecc_bit1_cnt=%d ", ecc_rst, ecc_bit1_cnt);
		if(ecc_bit1_cnt == 1){//ECC code error
			return ECC_CODE_ERR;
		}else if(ecc_bit1_cnt != 12){//more than 1 bit error, un-correctable
			printk("\r\n more than one bit ECC error \r\n");
			return ECC_DATA_ERR;
		}else if(ecc_bit1_cnt == 12){// 1 bit correctable error, get error bit
			temp = ra_inl(NFC_ECC_ST + page_index*4);
			if(unlikely((temp&0x1) == 0)){
				printk("\r\n ECC result conflict!! \r\n");
				return ECC_NFC_CONFLICT;
			}
			*error_byte_index = ((temp>>6)&0x1ff);
			*error_bit_index = ((temp>>2)&0x7);
			printk("\r\n correctable ECC error   error_byte_index=%d, error_bit_index=%d",
					*error_byte_index, *error_bit_index);
			return ECC_ONE_BIT_ERR;
		}
	}
	return ECC_NO_ERR;
}
#if !defined (WORKAROUND_RX_BUF_OV)	
/**
 * nfc_ecc_verify
 return value:
	0:    data OK or data correct OK
	-1:  data ECC fail
 */
int nfc_ecc_verify(struct ra_nand_chip *ra, unsigned char *buf, int page, int mode)
{
	int ret, i, j, ecc_num;
	unsigned char *p, *e;
	unsigned long err_byte_index = 0;
	unsigned long err_bit_index = 0;
	int ecc_error_code = ECC_DATA_ERR;
	int ecc_ret = -1;

	/* 512 bytes data has a ecc value */
	int ecc_bytes, ecc_offset, ecc[4];
	unsigned char ecc_swap[3] = {0};
	unsigned char correct_byte = 0;
	
//	printk("%s, page:%x mode:%d\n", __func__, page, mode);

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
	if(nand_is_rwfs_partition(ra, NULL, &page, NULL)) return 0;
#endif

	if (mode == FL_WRITING) {
		int len = (1 << ra->flash->page_shift) + (1 << ra->flash->oob_shift);
		int conf; 

		if (ra->flash->page_shift == SIZE_512iB_BIT)
			conf = 0x000141; 
		else
			conf = 0x000511;

		conf |= ((addr_cycle) << 16) | (len << 20);
		conf |= (1<<3); //(ecc_en) 
//		conf |= (1<<2); // (use_gdma)

		p = ra->readback_buffers;

//		ret = nfc_read_page(ra, ra->readback_buffers, page, FLAG_USE_GDMA | FLAG_ECC_EN); 
		ret = nfc_read_page(ra, ra->readback_buffers, page, FLAG_ECC_EN); 
//		ret = nfc_read_page(ra, ra->readback_buffers, page, FLAG_NONE); 
		if (ret == 0) 
			goto ecc_check;
		
		//FIXME, double comfirm
		printk("%s: read back fail, try again \n",__func__);
//		ret = nfc_read_page(ra, ra->readback_buffers, page, FLAG_USE_GDMA | FLAG_ECC_EN); 
		ret = nfc_read_page(ra, ra->readback_buffers, page, FLAG_ECC_EN); 
//		ret = nfc_read_page(ra, ra->readback_buffers, page, FLAG_NONE); 


		if (ret != 0) {
			printk("\t%s: read back fail agian \n",__func__);
			goto bad_block;
		}
	} else if (mode == FL_READING) {
		p = buf;
	} else {
		return -2;
	}

ecc_check:
	/* frankliao modify 20101018 */
	p += (1<<ra->flash->page_shift);
	
	ecc[0] = ra_inl(NFC_ECC); 
	
	if (ecc[0] == 0) {
		// printk("clean page.\n");
		return 0;
	}	

	ecc_bytes = ra->oob->eccbytes;

	ecc_offset = ra->oob->eccpos[0];
	
	/* each ecc register store 3 bytes ecc value */
	if (ecc_bytes == 12) {
		ecc[1] = ra_inl(NFC_ECCII); 
		ecc[2] = ra_inl(NFC_ECCIII); 
		ecc[3] = ra_inl(NFC_ECCIV); 
	}

	ecc_num = ecc_bytes/3;
	for (i=0; i<ecc_num; i++) {
		e = (unsigned char*)&ecc[i];
		ecc_swap[0] = *((unsigned char*)&ecc[i]+3);
		ecc_swap[1] = *((unsigned char*)&ecc[i]+2);
		ecc_swap[2] = *((unsigned char*)&ecc[i]+1);
	#if defined (TCSUPPORT_NAND_BADBLOCK_CHECK) || defined (TCSUPPORT_NAND_RT63368)
		ecc_offset = ra->oob->eccpos[i * 3];
		if(	*(p+ecc_offset) == 0xff
			&& *(p+ecc_offset+1) == 0xff
			&& *(p+ecc_offset+2) == 0xff
		) continue;	//ignore this case. ECC data not be written. Sam 2015/01/30
	#endif
		err_byte_index = 0;
		err_bit_index = 0;
		/* each ecc register store 3 bytes ecc value */
		ecc_ret = 0;
		for (j=0; j<3; j++) {
			#ifdef __LITTLE_ENDIAN
			int eccpos = ecc_offset + j + i*3;
			#else
		#if defined (TCSUPPORT_NAND_BADBLOCK_CHECK) || defined (TCSUPPORT_NAND_RT63368)
			int eccpos = ecc_offset - j + 2;
		#else
			int eccpos = ecc_offset - j + 2 + i*3;
		#endif
			#endif

			if (*(p + eccpos) != *(e + j + 1)) {

			#ifdef __LITTLE_ENDIAN
				printk("%s mode:%s, invalid ecc, page: %x read:%x %x %x, ecc:%x \n",
				       __func__, (mode == FL_READING)?"read":"write", page,	
				       *(p+ecc_offset), *(p+ecc_offset+1), *(p+ecc_offset+2), ecc[i]);
			#else
				printk("%s mode:%s, invalid ecc, page: %x read:%x %x %x, ecc:%x \n",
				       __func__, (mode == FL_READING)?"read":"write", page,	
				       *(p+ecc_offset+2), *(p+ecc_offset+1), *(p+ecc_offset), ecc[i]);
			#endif
				ecc_ret =-1;
				break;
			}
		}
		if(ecc_ret == -1){
			ecc_error_code = nfc_ecc_err_handler(i , p+ecc_offset, ecc_swap, &err_byte_index,
				&err_bit_index );
			if(ecc_error_code != ECC_NO_ERR){
				printk("\r\n ecc_error_code= %d, page=%d ,i=%d\n", ecc_error_code, page, i);
				if(ecc_error_code == ECC_ONE_BIT_ERR){
					//correct the error
					//printk("\r\n  err_byte_index= %d, err_bit_index=%d\n", err_byte_index , err_bit_index);
					correct_byte = buf[err_byte_index + i*512];
					if((correct_byte&(1<<err_bit_index)) != 0){
						correct_byte &= (~(1<<err_bit_index));
					}else{
						correct_byte |= (1<<err_bit_index);
					}
					buf[err_byte_index + i*512] = correct_byte;
					ecc_ret = 0;
					ecc_error_code = ECC_NO_ERR;
					continue;
				}
				return ecc_error_code;
			}
		}
	}

	return 0;

bad_block:
	return -1;
}


#else

void ranfc_dump(void) 
{	
	int i;
	for (i=0; i<11; i++) {
		if (i==6) 
			continue;
		printk("%x: %x \n", NFC_BASE + i*4, ra_inl(NFC_BASE + i*4));
	}
}


/**
 * @return 0, ecc OK or corrected.
 * @return NAND_STATUS_FAIL, ecc fail.   
 */

int 
nfc_ecc_verify(struct ra_nand_chip *ra, unsigned char *buf, int page, int mode)
{
	int ret, i;
	unsigned char *p, *e;
	int ecc;
	
	if (ranfc_verify == 0)
		return 0;

	ra_dbg("%s, page:%x mode:%d\n", __func__, page, mode);

	if (mode == FL_WRITING) { // read back and memcmp
		ret = nfc_read_page(ra, ra->readback_buffers, page, FLAG_USE_GDMA); 
		if (ret != 0) //double comfirm
			ret = nfc_read_page(ra, ra->readback_buffers, page, FLAG_USE_GDMA); 

		if (ret != 0) {
			printk("%s: mode:%x read back fail \n", __func__, mode);
			return -1;
		}
		return memcmp(buf, ra->readback_buffers, 1<<ra->flash->page_shift);
	}
	
	if (mode == FL_READING) { 
#if 0
		if (ra->sandbox_page == 0)
			return 0;

		ret = nfc_write_page(ra, buf, ra->sandbox_page, FLAG_USE_GDMA | FLAG_ECC_EN);
		if (ret != 0) {
			printk("%s, fail write sandbox_page \n", __func__);
			return -1;
		}
#else
		/** @n// code review tag ote: 
		 * The following command is actually not 'write' command to drive NFC to write flash.
		 * However, it can make NFC to calculate ECC, that will be used to compare with original ones.
		 * --YT
		 */
		unsigned int conf = 0x001223 | (addr_cycle<<16) | (0x200 << 20) | (1<<3) | (1<<2); 
		_nfc_write_raw_data(ra, 0xff, 0xff, ra->sandbox_page << ra->flash->page_shift, conf, buf, 0x200, FLAG_USE_GDMA);
#endif

		ecc = ra_inl(NFC_ECC); 
		if (ecc == 0) //clean page.
			return 0;
		e = (unsigned char*)&ecc;
		p = buf + (1 << ra->flash->page_shift);

		ecc_bytes = ra->oob->eccbytes;
		ecc_offset = ra->oob->eccpos[0];
	
		for (i=0; i<ecc_bytes; i++) {

			#ifdef __LITTLE_ENDIAN
			int eccpos = ecc_offset + i;
			#else
			int eccpos = ecc_offset + ecc_bytes - i - 1;
			#endif

			if (*(p + eccpos) != *(e + i + 1)) {

			#ifdef __LITTLE_ENDIAN
				printk("%s mode:%s, invalid ecc, page: %x read:%x %x %x, ecc:%x \n",
				       __func__, (mode == FL_READING)?"read":"write", page,	
				       *(p+ecc_offset), *(p+ecc_offset+1), *(p+ecc_offset+2), ecc);
			#else
				printk("%s mode:%s, invalid ecc, page: %x read:%x %x %x, ecc:%x \n",
				       __func__, (mode == FL_READING)?"read":"write", page,	
				       *(p+ecc_offset+2), *(p+ecc_offset+1), *(p+ecc_offset), ecc);
			#endif
				return -1;
			}
		}
		return 0;
	}
	return -1;
}
#endif


/**
 * @return -EIO, writing size is less than a page 
 * @return 0, OK
 */
int 
nfc_read_page(struct ra_nand_chip *ra, unsigned char *buf, int page, int flags)
{
	unsigned int cmd1 = 0, cmd2 = 0, conf = 0;
//	unsigned long long bus_addr = 0;
	unsigned long column_addr, row_addr;
	unsigned int ecc_en;
	int use_gdma;
	int pagesize, size, offs;
	int status = 0;

	use_gdma = flags & FLAG_USE_GDMA;
	ecc_en = flags & FLAG_ECC_EN;

	page = page & ((1 << ra->flash->chip_shift)-1); // chip boundary
	pagesize = (1 << ra->flash->page_shift);
	// write first, then read frankliao mark

	/* frankliao added 20101029, read size make status reg show ecc error*/
	
	if (ecc_en)
		size = pagesize + (1 <<ra->flash->oob_shift); //add oobsize
	else
		size = pagesize;

	offs = 0;
	
	while (size > 0) {
		int len;
#if defined (WORKAROUND_RX_BUF_OV)
		len = min(60, size);
#else
		len = size;
#endif	

//		bus_addr = (((unsigned long long) page << (column_addr_cycle*8)) | 
//					((unsigned long long) offs & ((1<<column_addr_cycle*8)-1))); 

		row_addr = page;
		column_addr = offs & ((1<<(column_addr_cycle<<3))-1);

		if (ra->flash->page_shift == SIZE_512iB_BIT) {
			/* frankliao added 512 bytes ~ 528 bytes 20101012*/
			
			if (unlikely((offs & ~((1 << ra->flash->page_shift)-1))))
				cmd1 = ra->opcode->readoob;
			/* frankliao added 256 bytes ~ 512 bytes 20101012*/
			else if (offs & ~((1<<(column_addr_cycle<<3))-1))
				cmd1 = ra->opcode->readB;
			else
				cmd1 = ra->opcode->read1;
			conf = 0x000141 | ((addr_cycle)<<16) | (len << 20); 

		} else if (ra->flash->page_shift == SIZE_2KiB_BIT) {
			cmd1 = ra->opcode->read1;
			cmd2 = ra->opcode->read2;
			conf = 0x000511 | ((addr_cycle)<<16) | (len << 20); 
		}
		// frankliao
#if !defined (WORKAROUND_RX_BUF_OV)
		if (ecc_en) 
			conf |= (1<<3); 
#endif
		if (use_gdma)
			conf |= (1<<2);
			
		status = _nfc_read_raw_data(ra, cmd1, cmd2, row_addr, column_addr, conf, buf+offs, len, flags);

		if (status & NAND_STATUS_FAIL) {
			printk("%s: fail \n", __func__);
			return -EIO;
		}

		offs += len;
		size -= len;
	}

	// verify and correct ecc frankliao mark
	if ((flags & (FLAG_VERIFY | FLAG_ECC_EN)) == (FLAG_VERIFY | FLAG_ECC_EN)) {
		status = nfc_ecc_verify(ra, buf, page, FL_READING);	
		if (status != 0) {
			printk("%s: fail, buf:%x, page:%x, flag:%x\n", 
			       __func__, (unsigned int)buf, page, flags);
			return status;
		}
	}
	else {
		// fix,e not yet support
		ra->buffers_page = -1; //cached
	} 

	return 0;
}


/** 
 * @return -EIO, fail to write
 * @return 0, OK
 */
int 
nfc_write_page(struct ra_nand_chip *ra, unsigned char *buf, int page, int flags)
{
	unsigned int cmd1 = 0, cmd3, conf = 0;
//	unsigned long long bus_addr = 0;
	unsigned long row_addr = 0;
	unsigned int ecc_en;
	int use_gdma;
	int pagesize;
	int i;
	char status;
//	uint8_t *oob = buf + (1 << ra->flash->page_shift);

	use_gdma = flags & FLAG_USE_GDMA;
	ecc_en = flags & FLAG_ECC_EN;
//	oob[ra->flash->badblockpos] = 0xff;	//tag as good block.
	ra->buffers_page = -1; 				//cached

	page = page & ((1<<ra->flash->chip_shift)-1); // chip boundary
	pagesize = (1 << ra->flash->page_shift);

	/* frankliao added 20101029 */
	if (ecc_en) {
		// frank added 20110421
		#if !defined(TCSUPPORT_NAND_BADBLOCK_CHECK) && !defined(TCSUPPORT_NAND_RT63368)
		memset(ra->buffers + pagesize, 0xff, (1 << ra->flash->oob_shift));
		#endif
		pagesize = pagesize + (1 << ra->flash->oob_shift);
	}	
	
//	bus_addr = ((unsigned long long) page << (column_addr_cycle*8)); //write_page always write from offset 0.
	row_addr = page;
	
	/* frankliao modify 20101004 */
	cmd1 = ra->opcode->pageprog1;
	cmd3 = ra->opcode->pageprog2;

	/* frankliao modify 20100105 */	
	if (ra->flash->page_shift == SIZE_512iB_BIT) {
		conf = 0x001243 | ((addr_cycle)<<16) | ((pagesize) << 20); 
	} else if (ra->flash->page_shift == SIZE_2KiB_BIT) {
		conf = 0x001103 | ((addr_cycle)<<16) | ((pagesize) << 20); 
	} else {
		printk("Undefined Write Page Command\n");
		return -EIO;
	}

	ra_dbg("in nfc_write_page function\n");
	ra_dbg("CMD1:%02x\n", cmd1);
	ra_dbg("CMD3:%02x\n", cmd3);
	ra_dbg("CONFIG:%06x\n", conf);

	if (ecc_en) 
		conf |= (1<<3); 
	if (use_gdma)
		conf |= (1<<2);

	// set NFC
	ra_dbg("nfc_write_page: cmd1: %x, cmd3: %x, conf: %x, len:%x\n",cmd1, cmd3, conf, pagesize);

	status = _nfc_write_raw_data(ra, cmd1, cmd3, row_addr, 0, conf, buf, pagesize, flags);
	if (status & NAND_STATUS_FAIL) {
		printk("%s: fail \n", __func__);
		return -EIO;
	}
	
	if (flags & FLAG_VERIFY) { // verify and correct ecc
		status = nfc_ecc_verify(ra, buf, page, FL_WRITING);
		// frank added 20110421
		#ifdef radbg 
		for (i=0; i<512; i++) {
			if (ra->readback_buffers[i] != buf[i]) {
				printk("bytes %d error readbuf: %x buf: %x\n", i, ra->readback_buffers[i], buf[i]);
			}
		}
		#endif	
		if (status != 0) {
			printk("%s: ecc_verify fail: ret:%x \n", __func__, status);
			return -EBADMSG;
		}
	}

	ra->buffers_page = page; //cached
	return 0;
}


/*************************************************************
 * nand internal process 
 *************************************************************/

/**
 * nand_release_device - [GENERIC] release chip
 * @mtd:	MTD device structure
 *
 * Deselect, release chip lock and wake up anyone waiting on the device
 */
static void 
nand_release_device(struct ra_nand_chip *ra)
{
	/* De-select the NAND device */
	nfc_select_chip(ra, -1);

	/* Release the controller and the chip */
	ra->state = FL_READY;
#if !defined (__UBOOT__)
	mutex_unlock(ra->controller);
#endif ///
}


/**
 * nand_get_device - [GENERIC] Get chip for selected access
 * @chip:	the nand chip descriptor
 * @mtd:	MTD device structure
 * @new_state:	the state which is requested
 *
 * Get the device and lock it for exclusive access
 */
static int
nand_get_device(struct ra_nand_chip *ra, int new_state)
{
	int ret = 0;

#if !defined (__UBOOT__)
	ret = mutex_lock_interruptible(ra->controller);	// code review tag
#endif ///

	if (!ret) 
		ra->state = new_state;

	return ret;

}


/*************************************************************
 * nand internal process 
 *************************************************************/
int 
nand_bbt_get(struct ra_nand_chip *ra, int block)
{
	int byte, bits;
	bits = block * BBTTAG_BITS;

	byte = bits / 8;
	bits = bits % 8;
	
	return (ra->bbt[byte] >> bits) & BBTTAG_BITS_MASK;
}


int 
nand_bbt_set(struct ra_nand_chip *ra, int block, int tag)
{
	int byte, bits;
	bits = block * BBTTAG_BITS;

	byte = bits / 8;
	bits = bits % 8;
	
	ra->bbt[byte] = (ra->bbt[byte] & ~(BBTTAG_BITS_MASK << bits)) | ((tag & BBTTAG_BITS_MASK) << bits);
		
	return tag;
}


/*s
 * nand_block_checkbad - [GENERIC] Check if a block is marked bad
 * @mtd:	MTD device structure
 * @ofs:	offset from device start
 *
 * Check, if the block is bad. Either by reading the bad block table or
 * calling of the scan function.
 */
int 
nand_block_checkbad(struct ra_nand_chip *ra, loff_t offs
#ifdef TCSUPPORT_NAND_RT63368
, unsigned long bmt_block
#endif
)
{
	int page, block;
	int ret = 4;
	unsigned int tag;

	char *str[]= {"UNK", "RES", "BAD", "GOOD"};

	if (ranfc_bbt == 0)
		return 0;

	// align with chip
	offs = offs & (((loff_t)1<<ra->flash->chip_shift) -1);

	page = offs >> ra->flash->page_shift;
	block = offs >> ra->flash->erase_shift;
#ifdef TCSUPPORT_NAND_RT63368
    if(bmt_block == 0){
#endif
	tag = nand_bbt_get(ra, block);

	if (tag == BBT_TAG_UNKNOWN) {
		ret = nfc_read_oob(ra, page, ra->flash->badblockpos, (char*)&tag, 1, FLAG_NONE);
		/* frankliao added 20101025, 32 - 8 = 24 */
		tag = tag >> 24;		
		if (ret == 0) 
			tag = ((tag & 0xff) == 0xff) ? BBT_TAG_GOOD : BBT_TAG_BAD;
		else
			tag = BBT_TAG_BAD;

		nand_bbt_set(ra, block, tag);
	}

#if 0
	ra_dbg("%s offs: %x , ret: %x, tag:%s\n", 
	       __func__, (int)offs, ret, str[tag]);
#endif

	if (tag != BBT_TAG_GOOD) {
		printk("%s: offs:%llx tag: %s \n", __func__, (loff_t)offs, str[tag]);
		return 1;
	} else {
		return 0;
	}
#ifdef TCSUPPORT_NAND_RT63368
    }
    else{

	ret = nfc_read_oob(ra, page, BMT_BAD_BLOCK_INDEX_OFFSET, (char*)&tag, 1, FLAG_NONE);

	tag = tag >> 24;
	if (ret == 0 && ((tag & 0xff) == 0xff))
		return 0;
	else
		return 1;

    }
#endif
}


/**
 * nand_block_markbad -
 */
int 
nand_block_markbad(struct ra_nand_chip *ra, loff_t offs
#ifdef TCSUPPORT_NAND_RT63368
, unsigned long bmt_block
#endif
)
{
	int page, block;
	int start_page, end_page;
	int ret = 4;
	unsigned int tag;
	char *ecc;

	// align with chip
	ra_dbg("%s offs: %llx \n", __func__, (loff_t)offs);

	offs = offs & (((loff_t)1<<ra->flash->chip_shift) -1);

//	page = offs >> ra->flash->page_shift;
	block = offs >> ra->flash->erase_shift;
	start_page = block * (1<<(ra->flash->erase_shift - ra->flash->page_shift));
	end_page = (block+1) * (1<<(ra->flash->erase_shift - ra->flash->page_shift));
#ifdef TCSUPPORT_NAND_RT63368
    if(bmt_block == 0){
#endif
	tag = nand_bbt_get(ra, block);

	if (tag == BBT_TAG_BAD) {
		printk("%s: mark repeatedly \n", __func__);
		return 0;
	}
#ifdef TCSUPPORT_NAND_RT63368
    }
#endif

	for (page=start_page; page<end_page; page++) {
		// new tag as bad
		tag = BBT_TAG_BAD;
		ret = nfc_read_page(ra, ra->buffers, page, FLAG_ECC_EN);
		if (ret != 0) {
			printk("%s: fail to read bad block tag \n", __func__);
			goto tag_bbt;
		}

#ifdef TCSUPPORT_NAND_RT63368
	if(bmt_block)
		ecc = &ra->buffers[(1<<ra->flash->page_shift) + BMT_BAD_BLOCK_INDEX_OFFSET];
	else
#endif
		ecc = &ra->buffers[(1<<ra->flash->page_shift)+ra->flash->badblockpos];
	
		if (*ecc == (char)0xff) {
			//tag into flash
			*ecc = (char)tag;
	//		ret = nfc_write_page(ra, ra->buffers, page, FLAG_USE_GDMA);
			ret = nfc_write_page(ra, ra->buffers, page, FLAG_ECC_EN);
			if (ret) {
				printk("%s: fail to write bad block tag \n", __func__);	
				break;
			}
		}	
	#if defined(TCSUPPORT_NAND_BADBLOCK_CHECK) || defined(TCSUPPORT_NAND_RT63368)
		break;
	#endif
	}
tag_bbt:
#ifdef TCSUPPORT_NAND_RT63368
	if(bmt_block == 0){
#endif
	//update bbt
	nand_bbt_set(ra, block, tag);

#ifdef TCSUPPORT_NAND_RT63368
	}
#endif
	return 0;
}


#if defined (WORKAROUND_RX_BUF_OV)
/**
 * to find a bad block for ecc verify of read_page
 */
unsigned int 
nand_bbt_find_sandbox(struct ra_nand_chip *ra)
{
	loff_t offs = 0;
	int chipsize = 1 << ra->flash->chip_shift;
	int blocksize = 1 << ra->flash->erase_shift;

	
	while (offs < chipsize) {
		if (nand_block_checkbad(ra, offs)) //scan and verify the unknown tag
			break;
		offs += blocksize;
	}

	if (offs >= chipsize) {
		offs = chipsize - blocksize;
	}

	nand_bbt_set(ra, (loff_t)offs >> ra->flash->erase_shift, BBT_TAG_RES);	 // tag bbt only, instead of update badblockpos of flash.
	return (offs >> ra->flash->page_shift);
}
#endif


/**
 * nand_erase_nand - [Internal] erase block(s)
 * @mtd:	MTD device structure
 * @instr:	erase instruction
 * @allowbbt:	allow erasing the bbt area
 *
 * Erase one ore more blocks
 */
int 
nand_erase_nand(struct ra_nand_chip *ra, struct erase_info *instr)
{
	int page, len, status, ret;
	unsigned long long addr;
	unsigned int blocksize = 1<<ra->flash->erase_shift;

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
	int block;
	int srcblock;
	unsigned long srcaddr;
#endif

#ifdef TCSUPPORT_NAND_RT63368
	int physical_block;
	unsigned long logic_addr;
	unsigned short phy_block_bbt;
#endif

	ra_dbg("%s: start:%llx, len:%x \n", __func__, instr->addr, (unsigned int)instr->len);

#define BLOCK_ALIGNED(a) ((a) & (blocksize - 1))

	if (BLOCK_ALIGNED(instr->addr) || BLOCK_ALIGNED(instr->len)) {
		ra_dbg("%s: erase block not aligned, addr:%llx len:%x\n", __func__, instr->addr, instr->len);
		return -EINVAL;
	}

	instr->fail_addr = -1;

	len = instr->len;
	addr = instr->addr;	    //logic address
	instr->state = MTD_ERASING;
	
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
	srcblock = addr >> ra->flash->erase_shift;
	srcaddr = addr;

	addr += (block_table[srcblock] - srcblock) << ra->flash->erase_shift;

	if(nand_partition_check(srcblock)){
		printk("%s: block %d >> %d address over partition size, erase fail \n", __func__, srcblock, block_table[srcblock]);
		instr->state = MTD_ERASE_FAILED;
		return -EFAULT;
	}
#endif

#ifdef TCSUPPORT_NAND_RT63368
	logic_addr = addr;  //logic address
#endif

	while (len) {

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		block = srcaddr >> ra->flash->erase_shift;
		if(srcblock != block)
		{
			srcblock = block;
			addr = srcaddr + ((block_table[block] - block) << ra->flash->erase_shift);

			if(nand_partition_check(srcblock)){
				printk("%s: block %d >> %d address over partition size, erase fail \n", __func__, srcblock, block_table[srcblock]);
				instr->state = MTD_ERASE_FAILED;
				return -EFAULT;
			}
		}
#endif

#ifdef TCSUPPORT_NAND_RT63368
		physical_block = get_mapping_block_index(logic_addr >> ra->flash->erase_shift, &phy_block_bbt); //physical block
		addr = (physical_block << ra->flash->erase_shift);  //physical address
#endif

//		page = (int)(addr >> ra->flash->page_shift);
		page = (int)(addr >> ra->flash->page_shift);
		ranfc_page = page;
//		block = (int)(addr >> ra->flash->erase_shift);
//		ra_dbg("FrankLiao block %x\n", block);

		/* select device and check wp */
		if (nfc_enable_chip(ra, addr, 0)) {
			printk("%s: nand is write protected \n", __func__);
			printk("NFC_CTRL : %x\n", ra_inl(NFC_CTRL));
			instr->state = MTD_ERASE_FAILED;
			goto erase_exit;
		}

		/*
		 * heck if we have a bad block, we do not erase bad blocks !
		 */
		 // frankliao mark
#if 0		 
		if (nand_block_checkbad(ra, addr)) {
			printk(KERN_WARNING "nand_erase: attempt to erase a "
			       "bad block at 0x%llx\n", addr);
			instr->state = MTD_ERASE_FAILED;
			goto erase_exit;
		}	
#endif
		/*
		 * Invalidate the page cache, if we erase the block which
		 * contains the current cached page
		 */
		 // frankliao mark
		if (BLOCK_ALIGNED(addr) == BLOCK_ALIGNED(ra->buffers_page << ra->flash->page_shift))
			ra->buffers_page = -1;

		status = nfc_erase_block(ra, page);
//		status = nfc_erase_block(ra, block);

		/* See if block erase succeeded */
		if (status) {
	#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		if(nand_is_rwfs_partition(ra, &block, NULL, NULL))
			printk("%s erase rwfs block %d fail\n", __FUNCTION__, block);
		else
			nand_erase_next_goodblock(ra, block, addr);
	#elif defined(TCSUPPORT_NAND_RT63368)
		if (update_bmt(addr,
			UPDATE_ERASE_FAIL, NULL, NULL))
		{
			printk("Erase fail at block, update BMT success\n");
		}
		else
		{
			printk("Erase fail at block, update BMT fail\n");
			return -1;
		}
	#else
			printk("%s: failed erase, block 0x%08x\n", __func__, page);
			instr->state = MTD_ERASE_FAILED;
//			instr->fail_addr = (block << ra->flash->erase_shift);
			instr->fail_addr = (page << ra->flash->page_shift);
			goto erase_exit;
        #endif

		}

		/* Increment page address and decrement length */
		len -= blocksize;
		addr += blocksize;  //physical address

	#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		srcaddr += blocksize;
	#endif

	#ifdef TCSUPPORT_NAND_RT63368
		logic_addr += blocksize;    //logic address
	#endif
	}
	instr->state = MTD_ERASE_DONE;

erase_exit:

	ret = ((instr->state == MTD_ERASE_DONE) ? 0 : -EIO);

#if !defined  (__UBOOT__)
	/* Do call back function */
	if (!ret)
		mtd_erase_callback(instr);
#endif

	if (ret) {
		nand_bbt_set(ra, addr >> ra->flash->erase_shift, BBT_TAG_BAD);
	}

	/* Return more or less happy */
	return ret;
}


static int 
nand_write_oob_buf(struct ra_nand_chip *ra, uint8_t *buf, uint8_t *oob, size_t size, 
			      int mode, int ooboffs) 
{
	size_t oobsize = 1 << ra->flash->oob_shift;
//	uint8_t *buf = ra->buffers + (1<<ra->page_shift);
	int retsize = 0;

	ra_dbg("%s: size:%x, mode:%x, offs:%x  \n", __func__, size, mode, ooboffs);

	switch(mode) {
	case MTD_OOB_PLACE:
	case MTD_OOB_RAW:
		if (ooboffs > oobsize)
			return -1;

#if 0		//* clear buffer */
		if (ooboffs || ooboffs+size < oobsize) 
			memset (ra->buffers + oobsize, 0x0ff, 1<<ra->oob_shift);
#endif

		size = min(size, oobsize - ooboffs);
		memcpy(buf + ooboffs, oob, size);
		retsize = size;
		break;

	case MTD_OOB_AUTO:  
	{
		struct nand_oobfree *free;
		uint32_t woffs = ooboffs;

		if (ooboffs > ra->oob->oobavail) 
			return -1;

		/* OOB AUTO does not clear buffer */
		/* frankliao mark 20101021 */

		for(free = ra->oob->oobfree; free->length && size; free++) {
			int wlen = free->length - woffs;
			int bytes = 0;

			/* Write request not from offset 0 ? */
			if (wlen <= 0) {
				woffs = -wlen;
				continue;
			}
			
			bytes = min_t(size_t, size, wlen);
			memcpy (buf + free->offset + woffs, oob, bytes);
			woffs = 0;
			oob += bytes;
			size -= bytes;
			retsize += bytes;
		}

		buf += oobsize;	
		break;
	}

	default:
		BUG();
	}

	return retsize;
}


static int 
nand_read_oob_buf(struct ra_nand_chip *ra, uint8_t *oob, size_t size, 
			     int mode, int ooboffs) 
{
	size_t oobsize = 1 << ra->flash->oob_shift;
	uint8_t *buf = ra->buffers + (1 << ra->flash->page_shift);
	int retsize=0;

	ra_dbg("%s: size:%x, mode:%x, offs:%x  \n", __func__, size, mode, ooboffs);

	switch(mode) {
	case MTD_OOB_PLACE:
	case MTD_OOB_RAW:
		if (ooboffs > oobsize)
			return -1;

		size = min(size, oobsize - ooboffs);
		memcpy(oob, buf + ooboffs, size);
		return size;

	case MTD_OOB_AUTO: {
		struct nand_oobfree *free;
		uint32_t woffs = ooboffs;

		if (ooboffs > ra->oob->oobavail) 
			return -1;

		size = min(size, ra->oob->oobavail - ooboffs);
		for(free = ra->oob->oobfree; free->length && size; free++) {
			int wlen = free->length - woffs;
			int bytes = 0;

			/* Write request not from offset 0 ? */
			if (wlen <= 0) {
				woffs = -wlen;
				continue;
			}
			
			bytes = min_t(size_t, size, wlen);
			memcpy(oob, buf + free->offset + woffs, bytes);
			woffs = 0;
			oob += bytes;
			size -= bytes;
			retsize += bytes;
		}
		

		return retsize;
	}
	default:
		BUG();
	}
	
	return -1;
}


#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
int nandflash_scan_badblock(void)
{
	int i, j, badblock;
	int addr = 0;
	int blocksize = 1 << ra->flash->erase_shift;
	int totalblock = 1024;

	for(i = 0; i < totalblock; i++)
	{
		block_table[i] = i;
	}

	for(j = 0; j < totalblock; j++)
	{

		if(nand_block_checkbad(ra, addr))
		{
			badblock = addr/blocksize;

			if(badblock >= TCROMFILE_START && badblock < TCROMFILE_END)
			{

				for(i = TCROMFILE_START; i < TCROMFILE_END; i++)
				{
					if(block_table[i] == badblock)
					{
						break;
					}

				}

				for(; i < TCROMFILE_END; i++)
				{
					block_table[i]++;
				}

			}else if(badblock >= TCLINUX_BLOCK_START && badblock < TCLINUX_BLOCK_END){

				for(i = TCLINUX_BLOCK_START; i < TCLINUX_BLOCK_END; i++)
				{
					if(block_table[i] == badblock)
					{
						break;
					}

				}

				for(; i < TCLINUX_BLOCK_END; i++)
				{
					block_table[i]++;
				}
		#ifdef CONFIG_DUAL_IMAGE
			}else if(badblock >= TCLINUX_SLAVE_BLOCK_START && badblock < TCLINUX_SLAVE_BLOCK_END){

				for(i = TCLINUX_SLAVE_BLOCK_START; i < TCLINUX_SLAVE_BLOCK_END; i++)
				{
					if(block_table[i] == badblock)
					{
						break;
					}

				}

				for(; i < TCLINUX_SLAVE_BLOCK_END; i++)
				{
					block_table[i]++;
				}
		#endif
		#if 0	//ignore bad block mechanism
			}else if(badblock >= RWFS_BLOCK_START && badblock < RWFS_BLOCK_END){

				for(i = RWFS_BLOCK_START; i < RWFS_BLOCK_END; i++)
				{
					if(block_table[i] == badblock)
					{
						break;
					}

				}

				for(; i < RWFS_BLOCK_END; i++)
				{
					block_table[i]++;
				}
		#endif
			}else if(badblock >= TCSYSLOG_START && badblock < TCSYSLOG_END){

				for(i = TCSYSLOG_START; i < TCSYSLOG_END; i++)
				{
					if(block_table[i] == badblock)
					{
						break;
					}

				}

				for(; i < TCSYSLOG_END; i++)
				{
					block_table[i]++;
				}

			}else if(badblock >= TCBKROMFILE_START && badblock < TCBKROMFILE_END){

				for(i = TCBKROMFILE_START; i < TCBKROMFILE_END; i++)
				{
					if(block_table[i] == badblock)
					{
						break;
					}

				}

				for(; i < TCBKROMFILE_END; i++)
				{
					block_table[i]++;
				}

			}else if(badblock >= TCWIFI_START && badblock < TCWIFI_END){

				for(i = TCWIFI_START; i < TCWIFI_END; i++)
				{
					if(block_table[i] == badblock)
					{
						break;
					}

				}

				for(; i < TCWIFI_END; i++)
				{
					block_table[i]++;
				}

			}

		}

		addr += blocksize;

	}

    return 0;

}


int nand_partition_check(int block)
{
	int ret = 0;

	if(block >= TCROMFILE_START && block < TCROMFILE_END){
		if(block_table[block] >= TCROMFILE_END){
			ret = -1;
			goto done;
		}

	}else if(block >= TCLINUX_BLOCK_START && block < TCLINUX_BLOCK_END){
		if(block_table[block] >= TCLINUX_BLOCK_END){
			ret = -1;
			goto done;
		}
#ifdef CONFIG_DUAL_IMAGE
	}else if(block >= TCLINUX_SLAVE_BLOCK_START && block < TCLINUX_SLAVE_BLOCK_END){
		if(block_table[block] >= TCLINUX_SLAVE_BLOCK_END){
			ret = -1;
			goto done;
		}
#endif
	}else if(block >= RWFS_BLOCK_START && block < RWFS_BLOCK_END){
		if(block_table[block] >= RWFS_BLOCK_END){
			ret = -1;
			goto done;
		}
	}else if(block >= TCSYSLOG_START && block < TCSYSLOG_END){
		if(block_table[block] >= TCSYSLOG_END){
			ret = -1;
			goto done;
		}

	}else if(block >= TCBKROMFILE_START && block < TCBKROMFILE_END){
		if(block_table[block] >= TCBKROMFILE_END){
			ret = -1;
			goto done;
		}
	}else if(block >= TCWIFI_START && block < TCWIFI_END){
		if(block_table[block] >= TCWIFI_END){
			ret = -1;
			goto done;
		}
	}

done:
	return ret;

}

int nand_is_rwfs_partition(struct ra_nand_chip *ra, int *block, int *page, unsigned long *addr)
{
	int target_block;
	if(block) {
		target_block = *block;
	}
	if(page) {
		target_block = *page >> (ra->flash->erase_shift - ra->flash->page_shift);
	}
	if(addr) {
		target_block = *addr >> ra->flash->erase_shift;
	}

	if(target_block >= RWFS_BLOCK_START && target_block < RWFS_BLOCK_END)
		return 1;
	else
		return 0;
}

int nand_erase_next_goodblock(struct ra_nand_chip *ra, int block, unsigned long addr_l)
{
	unsigned int blocksize = 1 << ra->flash->erase_shift;
	unsigned int pagesize = 1 << ra->flash->page_shift;
	unsigned long offset;
	int page;
	nand_block_markbad(ra, addr_l);

	block++;

	if(nand_partition_check(block))
		return -EFAULT;

	offset = block * blocksize;
	offset += (block_table[block] - block) << ra->flash->erase_shift;

	page = (unsigned long)(offset >> ra->flash->page_shift);

	while(nfc_erase_block(ra, page))
	{

		nand_block_markbad(ra, offset);

		block++;
		if(nand_partition_check(block)) {
			nandflash_scan_badblock();
			return -EFAULT;
		}
		offset = block * blocksize;
		offset += (block_table[block] - block) << ra->flash->erase_shift;

		page = (unsigned long)(offset >> ra->flash->page_shift);

	}

	nandflash_scan_badblock();

	return 0;

}

int nand_write_next_goodblock(struct ra_nand_chip *ra, int page_u, int page_l)
{
	int src_page, npage, nextblk_startaddr, nextblk_writeaddr, ret, block, addr;
	int readstart_page, writestart_page, last_page, readtotal_page, to_page, erase_blk, erase_addr;
	uint8_t *pbuf = NULL;
	uint32_t pagesize = (1 << ra->flash->page_shift);
	uint32_t blocksize = (1 << ra->flash->erase_shift);
	uint8_t page_data[pagesize + (1 << ra->flash->oob_shift)];

	src_page = page_l >> (ra->flash->erase_shift - ra->flash->page_shift);
	src_page = src_page << (ra->flash->erase_shift - ra->flash->page_shift);

	readstart_page = src_page;

	npage = page_l - src_page + 1;
	readtotal_page = npage;

	block = page_u >> (ra->flash->erase_shift - ra->flash->page_shift);
	addr = block << ra->flash->erase_shift;

	block++;
	if(nand_partition_check(block))
		return -EFAULT;
	addr += blocksize;

	nextblk_startaddr = addr + ((block_table[block] - block) << ra->flash->erase_shift);
	nextblk_writeaddr = nextblk_startaddr + (page_l - src_page) * pagesize;

	writestart_page = nextblk_startaddr >> ra->flash->page_shift;

	memcpy(page_data, ra->buffers, pagesize + (1 << ra->flash->oob_shift));

	nfc_erase_block(ra, writestart_page);

	while(readtotal_page > 0)
	{
		if(readtotal_page > 1){

			memset(ra->buffers, 0xff, pagesize + (1 << ra->flash->oob_shift));

			ret = nfc_read_page(ra, ra->buffers, readstart_page, 0);
			if(ret)
			{
				ret = nfc_read_page(ra, ra->buffers, readstart_page, 0);
				if(ret)
					printk("%s: read page fail", __func__);

			}
			to_page = writestart_page;
			pbuf = ra->buffers;
		}
		else{
			to_page = nextblk_writeaddr >> ra->flash->page_shift;
			pbuf = page_data;

		}

		ret = nfc_write_page(ra, pbuf, to_page, FLAG_ECC_EN | FLAG_VERIFY);
		if(ret)
		{
			nfc_erase_block(ra, writestart_page);
			nand_block_markbad(ra, nextblk_startaddr);

			block++;
			if(nand_partition_check(block)) {
				nandflash_scan_badblock();
				return -ENOSPC;
			}
			addr += blocksize;
			nextblk_startaddr = addr + (block_table[block] - block) << ra->flash->erase_shift;
			writestart_page = nextblk_startaddr >> ra->flash->page_shift;
			nextblk_writeaddr = nextblk_startaddr + (page_l - src_page) * pagesize;
			readstart_page = src_page;
			readtotal_page = npage;
			continue;

		}
		writestart_page++;
		readstart_page++;
		readtotal_page--;

	}

	nfc_erase_block(ra, src_page);
	nand_block_markbad(ra, src_page * pagesize);

	nandflash_scan_badblock();

	return to_page;

}

#endif
/**
 * nand_do_write_ops - [Internal] NAND write with ECC
 * @mtd:	MTD device structure
 * @to:		offset to write to
 * @ops:	oob operations description structure
 *
 * NAND write with ECC
 */
static int 
nand_do_write_ops(struct ra_nand_chip *ra, loff_t to,
			     struct mtd_oob_ops *ops)
{
	int page;
	uint32_t datalen = ops->len;
	uint32_t ooblen = ops->ooblen;
	uint8_t *oob = ops->oobbuf;
	uint8_t *data = ops->datbuf;

	int pagesize = (1 << ra->flash->page_shift);
	int pagemask = (pagesize -1);
	int oobsize = 1 << ra->flash->oob_shift;
	int i;
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
	unsigned int blocksize = (1 << ra->flash->erase_shift);
	int block;
	int srcpage;
#endif

#ifdef TCSUPPORT_NAND_RT63368
	int physical_block;
	int logic_page;
	unsigned long addr_offset_in_block;
	unsigned long logic_addr;
	unsigned short phy_block_bbt;
	char dat[SIZE_2KiB_BYTES + SIZE_64iB_BYTES];
#endif

	loff_t addr = to;   //logic address

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
	int srcblock = addr >> ra->flash->erase_shift;
	unsigned long srcaddr = addr;

	addr += (block_table[srcblock] - srcblock) << ra->flash->erase_shift;

	if(nand_partition_check(srcblock)){
		printk("%s: block %d >> %d address over partition size, write fail \n", __func__, srcblock, block_table[srcblock]);
		return -ENOSPC;
	}
#endif

#ifdef TCSUPPORT_NAND_RT63368
	logic_addr = addr;        //logic address
#endif

//	ra_dbg("%s: to:%x, ops data:%p, oob:%p datalen:%x ooblen:%x, ooboffs:%x oobmode:%x \n", 
//	       __func__, (unsigned int)to, data, oob, datalen, ooblen, ops->ooboffs, ops->mode);

	ops->retlen = 0;
	ops->oobretlen = 0;

	/* Invalidate the page cache, when we write to the cached page */
	ra->buffers_page = -1;

	if (data ==0)
		datalen = 0;
	
#if 0
	// oob sequential (burst) write
	if (datalen == 0 && ooblen) {
		int len = ((ooblen + ops->ooboffs) + (ra->oob->oobavail - 1)) / ra->oob->oobavail * oobsize;

		/* select chip, and check if it is write protected */
		if (nfc_enable_chip(ra, addr, 0))
			return -EIO;
		
		//FIXME, need sanity check of block boundary
		page = (int)((to & (((loff_t)1<<ra->flash->chip_shift)-1)) >> ra->flash->page_shift); //chip boundary
		ranfc_page = page;

		/* frankliao added 20101029 */
		if (nand_block_checkbad(ra, addr
	#ifdef TCSUPPORT_NAND_RT63368
		,BAD_BLOCK_RAW
	#endif
		)) {
			printk(KERN_WARNING "nand_do_write_ops: attempt to write a "
			       "bad block at 0x%08x\n", page);
			return -EFAULT;
		}	
		// frankliao added 20110304, clear write buffer 
		memset(ra->buffers, 0x0ff, pagesize);
		//fixme, should we reserve the original content?
		if (ops->mode == MTD_OOB_AUTO) {
//			nfc_read_oob(ra, page, 0, ra->buffers, len, FLAG_USE_GDMA);
			nfc_read_oob(ra, page, 0, ra->buffers, len, FLAG_NONE);
		}
		//prepare buffers
		nand_write_oob_buf(ra, ra->buffers, oob, ooblen, ops->mode, ops->ooboffs);
		// write out buffer to chip
//		nfc_write_oob(ra, page, 0, ra->buffers, len, FLAG_USE_GDMA);
		nfc_write_oob(ra, page, 0, ra->buffers, len, FLAG_NONE);

		ops->oobretlen = ooblen;
		ooblen = 0;
	}
#endif

	// data sequential (burst) write
	if (datalen && ooblen == 0) {
		/* frankliao modify 20101021 */ // code review tag
//		printk("ranfc can not support write_data_burst, since hw-ecc and fifo constraints..\n");
	}

	// page write
	while (datalen || ooblen) {
		int len;
		int ret;
		int offs;
		int ecc_en = 0;
/*
		printk("datalen = %d\n", datalen);
		printk("ooblen = %d\n", ooblen);
*/
		ra_dbg("%s : addr:%llx, ops data:%p, oob:%p datalen:%x ooblen:%x, ooboffs:%x \n", 
		       __func__, addr, data, oob, datalen, ooblen, ops->ooboffs);

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		block = srcaddr >> ra->flash->erase_shift;
		if(srcblock != block)
		{
			srcblock = block;
			addr = srcaddr + ((block_table[block] - block) << ra->flash->erase_shift);

			if(nand_partition_check(srcblock)){
				printk("%s: block %d >> %d address over partition size, write fail \n", __func__, srcblock, block_table[srcblock]);
				return -ENOSPC;
			}
		}
#endif

#ifdef TCSUPPORT_NAND_RT63368
		addr_offset_in_block = logic_addr % (1 << ra->flash->erase_shift);  //logic address offset
		physical_block = get_mapping_block_index(logic_addr >> ra->flash->erase_shift, &phy_block_bbt); //physical block
		addr = (physical_block << ra->flash->erase_shift) + addr_offset_in_block;   //physical address offset
#endif

		page = (int)((addr & (((loff_t)1<<ra->flash->chip_shift)-1)) >> ra->flash->page_shift); //chip boundary
		
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		srcpage = (int)((srcaddr & ((1<<ra->flash->chip_shift)-1)) >> ra->flash->page_shift);
#endif

#ifdef TCSUPPORT_NAND_RT63368
		logic_page = (int)((logic_addr & ((1<<ra->flash->chip_shift)-1)) >> ra->flash->page_shift); //logic page
#endif

		ranfc_page = page;
#if 0
		/* frankliao added 20101029 */
		if (nand_block_checkbad(ra, addr)) {
			printk(KERN_WARNING "nand_do_write_ops: attempt to write a "
			       "bad block at 0x%08x\n", page);
			return -EFAULT;
		}	
#endif
		/* select chip, and check if it is write protected */
		if (nfc_enable_chip(ra, addr, 0))
			return -EIO;
		/* frankliao deleted 20101007, have wait time problem */
/*		// oob write
		if (ops->mode == MTD_OOB_AUTO) {
			//fixme, this path is not yet varified 
			nfc_read_oob(ra, page, 0, ra->buffers + pagesize, oobsize, FLAG_NONE);
		}	
*/		
		if (oob && ooblen > 0) {
			memset(ra->buffers + pagesize, 0xff, oobsize);
			len = nand_write_oob_buf(ra, ra->buffers + pagesize, oob, ooblen, ops->mode, ops->ooboffs);
			if (len < 0) 
				return -EINVAL;
			oob += len;
			ops->oobretlen += len;
			ooblen -= len;
		} 

		// data write
		offs = addr & pagemask;
		len = min_t(size_t, datalen, pagesize - offs);
		/* frankliao mark */
//		len = datalen;
		if (data && len > 0) {
			/* frankliao modify 20110208, reset buffer */
//			memset(ra->buffers, 0xff, len);
		#if defined(TCSUPPORT_NAND_BADBLOCK_CHECK) || defined(TCSUPPORT_NAND_RT63368)
			memset(ra->buffers, 0xff, pagesize + oobsize);
		#else
			memset(ra->buffers, 0xff, pagesize);
		#endif
			memcpy(ra->buffers + offs, data, len);	// we can not sure ops->buf wether is DMA-able.
/*			
			printk("In nand_do_write_ops\n");
			printk("offset : %d\n", offs);
			printk("ra->buffer\n");
			for (i=0; i<pagesize; i++) {
				printk("%d ", ra->buffers[i]);
			}
			printk("\n\n\n");

			printk("data buf\n");
			for (i=0; i<pagesize; i++) {
				printk("%d ", data[i]);
			}
			printk("\n\n\n");
*/
		#ifdef TCSUPPORT_NAND_RT63368
			if(block_is_in_bmt_region(physical_block))
			{
				memcpy(ra->buffers + pagesize + OOB_INDEX_OFFSET, &phy_block_bbt, OOB_INDEX_SIZE);
			}
		#endif
			data += len;
			datalen -= len;
			ops->retlen += len;
			ecc_en = FLAG_ECC_EN;
		}
//		ret = nfc_write_page(ra, ra->buffers, page, FLAG_VERIFY); 
		/* frankliao modify 20101004 */
//		ret = nfc_write_page(ra, ra->buffers, page, FLAG_USE_GDMA | FLAG_VERIFY |
//				     ((ops->mode == MTD_OOB_RAW || ops->mode == MTD_OOB_PLACE) ? 0 : ecc_en ));
//		ret = nfc_write_page(ra, ra->buffers, page, FLAG_VERIFY |
//				     ((ops->mode == MTD_OOB_RAW || ops->mode == MTD_OOB_PLACE) ? 0 : ecc_en ));
		
//		ret = nfc_write_page(ra, ra->buffers, page, FLAG_VERIFY | ranfc_flags);
	#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		ranfc_flags = (FLAG_VERIFY | ecc_en);
		//ranfc_flags = 0;
	#elif defined(TCSUPPORT_NAND_RT63368)
		ranfc_flags = (FLAG_VERIFY | ecc_en);
	#endif

	#ifdef TCSUPPORT_NAND_RT63368
		if(!(data && len > 0))
		{
			ret = nfc_write_oob(ra, page, 0, ra->buffers + pagesize, oobsize, ranfc_flags);
			if(ret)
				nfc_read_page(ra, ra->buffers, page, ranfc_flags);
		}
		else{
	#endif
		ret = nfc_write_page(ra, ra->buffers, page, ranfc_flags);
	#ifdef TCSUPPORT_NAND_RT63368
		}
	#endif
		if (ret) {
	#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
			if(nand_is_rwfs_partition(ra, NULL, &srcpage, NULL))
				printk("%s: write rwfs page %d fail\n", __FUNCTION__, srcpage);
			else
				page = nand_write_next_goodblock(ra, srcpage, page);
	#elif defined(TCSUPPORT_NAND_RT63368)
			printk("write fail at page: %d \n", page);
			memcpy(dat, ra->buffers, SIZE_2KiB_BYTES + SIZE_64iB_BYTES);
			if (update_bmt(page << ra->flash->page_shift,
				UPDATE_WRITE_FAIL, dat, dat + SIZE_2KiB_BYTES))
			{
				printk("Update BMT success\n");
			}
			else
			{
				printk("Update BMT fail\n");
				return -1;
			}
	#else
			nand_bbt_set(ra, addr >> ra->flash->erase_shift, BBT_TAG_BAD);
			return ret;
	#endif
		}

//		nand_bbt_set(ra, addr >> ra->flash->erase_shift, BBT_TAG_GOOD);

		addr = (page+1) << ra->flash->page_shift;   //physical address

	#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		srcaddr = (srcpage+1) << ra->flash->page_shift;
	#endif

	#ifdef TCSUPPORT_NAND_RT63368
		logic_addr = (logic_page + 1) << ra->flash->page_shift; //logic address
	#endif
	}

	return 0;
}

/**
 * nand_do_read_ops - [Internal] Read data with ECC
 *
 * @mtd:	MTD device structure
 * @from:	offset to read from
 * @ops:	oob ops structure
 *
 * Internal function. Called with chip held.
 */
static int 
nand_do_read_ops(struct ra_nand_chip *ra, loff_t from,
			    struct mtd_oob_ops *ops)
{

	int page;
	uint32_t datalen = ops->len;
	uint32_t ooblen = ops->ooblen;
	uint8_t *oob = ops->oobbuf;
	uint8_t *data = ops->datbuf;
	int pagesize = (1 << ra->flash->page_shift);
	int oobsize = (1 << ra->flash->oob_shift);
	int pagemask = (pagesize -1);
	loff_t addr = from; //logic address

#ifdef TCSUPPORT_NAND_RT63368
	int physical_block;
	int logic_page;
	unsigned long addr_offset_in_block;
	unsigned long logic_addr = addr;
	unsigned short phy_block_bbt;
#endif

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
	int block;
	unsigned int blocksize = (1 << ra->flash->erase_shift);
	int srcblock = addr >> ra->flash->erase_shift;
	unsigned long srcaddr = addr;
	int srcpage;

	addr += (block_table[srcblock] - srcblock) << ra->flash->erase_shift;

	if(nand_partition_check(srcblock)){
		printk("%s: block %d >> %d address over partition size, read fail \n", __func__, srcblock, block_table[srcblock]);
		return -EFAULT;
	}
#endif
//	ra_dbg("READ FROM\n");
//	ra_dbg("read from %llx\n\n", from);

//	ra_dbg("%s: addr:%llx, ops data:%p, oob:%p datalen:%x ooblen:%x, ooboffs:%x \n", 
//	       __func__, (unsigned long long)addr, data, oob, datalen, ooblen, ops->ooboffs);

	ops->retlen = 0;
	ops->oobretlen = 0;

	if (data == 0)
		datalen = 0;

	while(datalen || ooblen) {
		int len, ret, offs;

//		ra_dbg("%s : addr:%llx, ops data:%p, oob:%p datalen:%x ooblen:%x, ooboffs:%x \n", 
//		       __func__, (unsigned long long)addr, data, oob, datalen, ooblen, ops->ooboffs);

		/* select chip */
		if (nfc_enable_chip(ra, addr, 1) < 0)
			return -EIO;
#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		block = srcaddr >> ra->flash->erase_shift;
		if(srcblock != block)
		{
			srcblock = block;
			addr = srcaddr + ((block_table[block] - block) << ra->flash->erase_shift);

			if(nand_partition_check(srcblock)){
				printk("%s: block %d >> %d address over partition size, read fail \n", __func__, srcblock, block_table[srcblock]);
				return -EFAULT;
			}
		}
#endif

#ifdef TCSUPPORT_NAND_RT63368
		addr_offset_in_block = logic_addr % (1 << ra->flash->erase_shift);  //logic address offset
		physical_block = get_mapping_block_index(logic_addr >> ra->flash->erase_shift, &phy_block_bbt); //physical block
		addr = (physical_block << ra->flash->erase_shift) + addr_offset_in_block;   //physical address
#endif

		page = (int)((addr & (((loff_t)1<<ra->flash->chip_shift)-1)) >> ra->flash->page_shift); 

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		srcpage = (int)((srcaddr & ((1<<ra->flash->chip_shift)-1)) >> ra->flash->page_shift);
#endif

#ifdef TCSUPPORT_NAND_RT63368
		logic_page = (int)((logic_addr & ((1<<ra->flash->chip_shift)-1)) >> ra->flash->page_shift); //logic page
#endif

#if 0
		/* frankliao added 20101029 */
		if (nand_block_checkbad(ra, addr)) {
			printk(KERN_WARNING "nand_do_read_ops: attempt to read a "
			       "bad block at 0x%08x\n", page);
			return -EFAULT;
		}	
#endif
		ranfc_page = page;

//		ret = nfc_read_page(ra, ra->buffers, page, FLAG_USE_GDMA | FLAG_VERIFY | 
//				    ((ops->mode == MTD_OOB_RAW || ops->mode == MTD_OOB_PLACE) ? 0: FLAG_ECC_EN ));  

	#if defined(TCSUPPORT_NAND_BADBLOCK_CHECK) || defined(TCSUPPORT_NAND_RT63368)
		//ranfc_flags = (FLAG_VERIFY | FLAG_ECC_EN);
		ranfc_flags = (FLAG_ECC_EN | FLAG_VERIFY);
	#endif

#ifdef TCSUPPORT_NAND_RT63368
		if(data && len > 0) {
#endif
		/* frankliao test delete */
		ret = nfc_read_page(ra, ra->buffers, page, ranfc_flags); 
		//FIXME, something strange here, some page needs 2 more tries to guarantee read success.
		if (ret) {
//			printk("read again:\n");
			/* frankliao modify 20101001 */
			ret = nfc_read_page(ra, ra->buffers, page, ranfc_flags); 
//			ret = nfc_read_page(ra, ra->buffers, page, FLAG_USE_GDMA | FLAG_VERIFY | 
//					    ((ops->mode == MTD_OOB_RAW || ops->mode == MTD_OOB_PLACE) ? 0: FLAG_ECC_EN ));
			if (ret) {
//				printk("read again fail \n");
		#if !defined(TCSUPPORT_NAND_BADBLOCK_CHECK) && !defined(TCSUPPORT_NAND_RT63368)
				nand_bbt_set(ra, addr >> ra->flash->erase_shift, BBT_TAG_BAD);
				if ((ret != -EUCLEAN) && (ret != -EBADMSG)) {
					return ret;
				} else {
					/* ecc verification fail, but data need to be returned. */
				}
			#else
				return ret;
		#endif
			} else {
//				printk(" read again susccess \n");
			}
		}

#ifdef TCSUPPORT_NAND_RT63368
		}
#endif
		// oob read
		if (oob && ooblen > 0) {
#ifdef TCSUPPORT_NAND_RT63368
			memset(ra->buffers + pagesize, 0xff, oobsize);
			nfc_read_oob(ra, page, 0, ra->buffers + pagesize, oobsize, FLAG_NONE);
#endif
			len = nand_read_oob_buf(ra, oob, ooblen, ops->mode, ops->ooboffs);
			if (len < 0) {
				printk("nand_read_oob_buf: fail return %x \n", len);
				return -EINVAL;
			}
			oob += len;
			ops->oobretlen += len;
			ooblen -= len;
		}	
		
		// data read
		offs = addr & pagemask;
		/* frankliao mark */
		len = min_t(size_t, datalen, pagesize - offs);
//		len = datalen;
		if (data && len > 0) {
			memcpy(data, ra->buffers + offs, len);	// we can not sure ops->buf wether is DMA-able.
			data += len;
			datalen -= len;
			ops->retlen += len;
			if (ret) {
				return ret;
			}
		}

//		nand_bbt_set(ra, addr >> ra->flash->erase_shift, BBT_TAG_GOOD);
		// address go further to next page, instead of increasing of length of write. This avoids some special cases wrong.
		addr = ((loff_t)(page+1) << ra->flash->page_shift); //physical address

	#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
		srcaddr = (srcpage+1) << ra->flash->page_shift;
	#endif

	#ifdef TCSUPPORT_NAND_RT63368
		logic_addr = (logic_page + 1) << ra->flash->page_shift; //logic address
	#endif
	}

	return 0;
//	return page;
}


/*
 * nand_setup - setup nand flash info and opcode
 *
 * frankliao added 20101014
 */
static int
nand_setup(void)
{
	int flash_id, i, subpage_bit = 0;
	unsigned int id_mask, mfr_id, dev_id, nbits, endian = 0;
	int alloc_size, bbt_size, buffers_size;

	struct nand_info *flash = NULL;
	struct nand_opcode *opcode = NULL;

	flash_id = nfc_read_id();
	if (flash_id == -1) {
		printk("read flash id fail\n");
		return -1;
	}
	
	id_mask = 0xffff;
	nbits = 16;

	if ((flash_id >> nbits) != 0) {
		nbits = 8;
		id_mask = 0xff;
	}

	// frank added 20110421
	#ifdef CONFIG_MIPS_RT63365
	endian = ra_inl(NFC_CTRLII) & 0x02;

	if (endian == NFC_BIG_ENDIAN) {
		mfr_id = ((flash_id >> 24) & id_mask);
		dev_id = ((flash_id >> (24-nbits)) & id_mask);
	} else {
		mfr_id = (flash_id & id_mask);
		dev_id = ((flash_id >> nbits) & id_mask);
	}
	#else
	mfr_id = (flash_id & id_mask);
	dev_id = ((flash_id >> nbits) & id_mask);
	#endif

//	printk("mfr_id : %2x\n", mfr_id);
//	printk("dev_id : %2x\n", dev_id);

	for (i=0; i < ARRAY_SIZE(flash_tables); i++) {

//		radbg("index %d\n", i);
//		radbg("mfr_id : %2x\n", flash_tables[i].mfr_id);
//		radbg("dev_id : %2x\n", flash_tables[i].dev_id);

		if ((mfr_id == flash_tables[i].mfr_id) && (dev_id == flash_tables[i].dev_id)) {
			flash = &flash_tables[i];
			break;
		}
	}
	
	if (flash == NULL) {
		printk("Undefined Manufactor ID and Devcie ID\n");
		return -1;
	}
	
	
	if (flash->page_shift == SIZE_512iB_BIT) {
		subpage_bit = 1;
	}

	column_addr_cycle = (flash->page_shift - subpage_bit + 7) / 8;
	row_addr_cycle = (flash->chip_shift - flash->page_shift + 7) / 8;
	addr_cycle = column_addr_cycle + row_addr_cycle;

	for (i=0; i < ARRAY_SIZE(opcode_tables); i++) {
		if (flash->opcode_type == opcode_tables[i].type) {
			opcode = &opcode_tables[i];
			break;
		}
	}

	if (opcode == NULL) {
		printk("Undefined Opcode\n");
		return -1;
	}

//#define ALIGN_32(a) (((unsigned long)(a)+32) & ~31)	// code review tag
#define ALIGN_32(a) (((unsigned long)(a)+31) & ~31)	// code review tag
	buffers_size = ALIGN_32((1<<flash->page_shift) + (1<<flash->oob_shift)); //ra->buffers
	bbt_size = BBTTAG_BITS * (1<<(flash->chip_shift - flash->erase_shift)) / 8; //ra->bbt
	bbt_size = ALIGN_32(bbt_size);

	alloc_size = buffers_size + bbt_size;
	alloc_size += buffers_size; //for ra->readback_buffers
	alloc_size += sizeof(*ra); 
	alloc_size += sizeof(*ranfc_mtd);

#if !defined (__UBOOT__)
	ra = (struct ra_nand_chip *)kzalloc(alloc_size, GFP_KERNEL | GFP_DMA);
#else
    ra = (struct ra_nand_chip *)malloc(alloc_size);
#endif

	if (!ra) {
		printk("%s: mem alloc fail \n", __func__);
		return -ENOMEM;
	}

#if defined(__UBOOT__)
	memset(ra, 0, alloc_size);
#endif	

	printk("%s: alloc %x, at %p , btt(%p, %x), ranfc_mtd:%p\n", 
	       __func__ , alloc_size, ra, ra->bbt, bbt_size, ranfc_mtd);

	//dynamic
	ra->buffers = (char *)((char *)ra + sizeof(*ra));
	ra->readback_buffers = ra->buffers + buffers_size; 
	ra->bbt = ra->readback_buffers + buffers_size; 
	ranfc_mtd = (struct mtd_info *)(ra->bbt + bbt_size);
	ra->buffers_page	= -1;

	if (flash->page_shift == SIZE_512iB_BIT) {
		ra->oob				= &oob_layout_tables[ STANDARD_SMALL_FLASH ];
	} else if (flash->page_shift == SIZE_2KiB_BIT) {
		ra->oob				= &oob_layout_tables[ STANDARD_LARGE_FLASH ];
	} else {
		printk("Undefined NAND OOB LAYOUT\n");
		return -1;
	}

	ra->flash = flash;
	ra->opcode = opcode;

#ifdef TCSUPPORT_NAND_RT63368
	bmt_pool_size = calc_bmt_pool_size(ra);
	printk("bmt pool size: %d \n", bmt_pool_size);

	if (!g_bmt)
	{
		if ( !(g_bmt = init_bmt(ra, bmt_pool_size)) )
		{
			printk("Error: init bmt failed \n");
			return -1;
		}
	}

	if (!g_bbt)
	{
		if ( !(g_bbt = start_init_bbt()) )
		{
			printk("Error: init bbt failed \n");
			return -1;
		}
	}

	if(write_bbt_or_bmt_to_flash() != 0)
	{
		printk("Error: save bbt or bmt to nand failed \n");
		return -1;
	}

	if(create_badblock_table_by_bbt())
	{
		printk("Error: create bad block table failed \n");
		return -1;
	}

#endif

	return 0;
}


/************************************************************
 * the following are mtd necessary interface.
 ************************************************************/


/**
 * nand_erase - [MTD Interface] erase block(s)
 * @mtd:	MTD device structure
 * @instr:	erase instruction
 *
 * Erase one ore more blocks
 */
static int 
ramtd_nand_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	int ret;
	
	struct ra_nand_chip *ra = (struct ra_nand_chip *)mtd->priv;

	ra_dbg("%s: \n", __func__);

	/* Grab the lock and see if the device is available */
	nand_get_device(ra, FL_ERASING);

	ret = nand_erase_nand((struct ra_nand_chip *)mtd->priv, instr);

	/* Deselect and wake up anyone waiting on the device */
	nand_release_device(ra);
	
	return ret;

}

/**
 * nand_write - [MTD Interface] NAND write with ECC
 * @mtd:	MTD device structure
 * @to:		offset to write to
 * @len:	number of bytes to write
 * @retlen:	pointer to variable to store the number of written bytes
 * @buf:	the data to write
 *
 * NAND write with ECC
 */
static int 
ramtd_nand_write(struct mtd_info *mtd, loff_t to, size_t len,
			    size_t *retlen, const uint8_t *buf)
{
	struct ra_nand_chip *ra = mtd->priv;
	struct mtd_oob_ops ops;
	int ret;
	/* frankliao added 20100106, 2K page NAND oobsize */
//	uint8_t oob_buffer[64];		
	
	ra_dbg("%s: \n", __func__);
	/* Do not allow reads past end of device */
	if ((to + len) > mtd->size)
		return -EINVAL;
	if (!len)
		return 0;
		
	nand_get_device(ra, FL_WRITING);

	memset(&ops, 0, sizeof(ops));
	
	ops.len = len;

	ops.datbuf = (uint8_t *)buf;

	ops.oobbuf = NULL;
	ops.ooblen = 0;

//	oobsize = (1<<ra->flash->oob_shift);

//	ops.ooblen = ra->oob->oobavail;

	/* frankliao added */
//	for (i=0; i<oobsize; i++) {
//		oob_buffer[i] = 0x11;
//	}

//	ops.oobbuf = oob_buffer;

//	ops.oobbuf = (uint8_t *)(buf + (1 << ra->flash->page_shift));
	/* for ecc write frankliao added 20101021 */
//	oobsize = (1<<ra->flash->oob_shift);

	ops.mode =  MTD_OOB_AUTO;

	ret = nand_do_write_ops(ra, to, &ops);

	/* write to next block, frankliao added 20101029 */
/*	while (ret != 0) {
		to += (1 << ra->flash->erase_shift);
		ret = nand_do_write_ops(ra, to, &ops);
	} */

	*retlen = ops.retlen;

	nand_release_device(ra);

	return ret;

}

/**
 * nand_read - [MTD Interface] MTD compability function for nand_do_read_ecc
 * @mtd:	MTD device structure
 * @from:	offset to read from
 * @len:	number of bytes to read
 * @retlen:	pointer to variable to store the number of read bytes
 * @buf:	the databuffer to put data
 *
 * Get hold of the chip and call nand_do_read
 */
static int 
ramtd_nand_read(struct mtd_info *mtd, loff_t from, size_t len,
			   size_t *retlen, uint8_t *buf)
{

	struct ra_nand_chip *ra = mtd->priv;
	int ret;
//	int oobsize;
	struct mtd_oob_ops ops;
	/* frankliao added 20100107 */
//	unsigned int ecc_en;

	/* frankliao added 20100106, 2K page NAND oobsize */
//	uint8_t oob_buffer[64];			

//	ra_dbg("%s: mtd:%p from:%llx, len:%x, buf:%p \n", __func__, mtd, (unsigned long long)from, len, buf);

	/* Do not allow reads past end of device */
	if ((from + len) > mtd->size)
		return -EINVAL;
	if (!len)
		return 0;

	nand_get_device(ra, FL_READING);
	// frankliao test 
	memset(&ops, 0, sizeof(ops));

	ops.ooblen = 0;
	ops.oobbuf = NULL; 

	/* frankliao added */
//	ops.ooblen = ra->oob->oobavail;

	ops.len = len;
	ops.datbuf = buf;

//	ops.oobbuf = NULL; 

	/* frankliao modify */
//	ops.oobbuf = oob_buffer; 

	ops.mode = MTD_OOB_AUTO;

	ret = nand_do_read_ops(ra, from, &ops);
//	for (i=0; i<(int)len; i++) {
//		if (buf[i] != *(unsigned char*)((unsigned long)(from) + (unsigned long)i + 0xbfc00000))
//			buf[i] = *(unsigned char *)((unsigned long)(from) + (unsigned long)i + 0xbfc00000);
//	}
//	ret = 0;
//	*retlen = len;

//	oobsize = (1<<ra->flash->oob_shift);
	/* frankliao added */
//	for (i=0; i<oobsize; i++) {
//		printk("oob_buffer[%d] : %x\n", i, oob_buffer[i] & 0xff);
//	}

	*retlen = ops.retlen;
//	dump_stack();

	/* read from next block, frankliao added 20101029 */
/*	while (ret != 0) {	// code review tag
		
		printk("ret = %d\n", ret);
		printk("read ofs : %x\n", from);
		from += (1 << ra->flash->erase_shift);

		ret = nand_do_read_ops(ra, from, &ops);
	}	*/

/*
	*retlen = ops.retlen;
	printk("nand VS SPI\n");
	printk("from : %llx\n", from);
	for (i=0; i<(int)len; i++) {
		if (buf[i] != (*(char *)((unsigned long)(from) + (unsigned long)i + 0xbfc00000) & 0xff) ) {
			printk("nand : %x\n ", buf[i]);
			printk("spi %x\n ", (*(char *)((unsigned long)(from) + (unsigned long)i + 0xbfc00000)) & 0xff);
		}	
	}
*/

	nand_release_device(ra);

	return ret;
}

unsigned int ra_nand_read_byte(unsigned long long from)
{
	unsigned char ch;
	size_t retlen;
	ramtd_nand_read(ranfc_mtd, (loff_t)from, 1, &retlen, &ch);
	return ch;
}

unsigned int ra_nand_read_dword(unsigned long long from)
{
	unsigned char data[4];
	unsigned long dword;
	size_t retlen;
	int ret, i;

	ret = ramtd_nand_read(ranfc_mtd, (loff_t)from, 4, &retlen, data);
	if (ret != 0)
		return -1;

	dword = 0;	
	for (i=0; i<4; i++) {
		dword += (unsigned long)data[i];
		if (i<3)
			dword <<= 8;
	}
	return dword;
}

/**
 * nand_read_oob - [MTD Interface] NAND read data and/or out-of-band
 * @mtd:	MTD device structure
 * @from:	offset to read from
 * @ops:	oob operation description structure
 *
 * NAND read data and/or out-of-band data
 */
static int 
ramtd_nand_readoob(struct mtd_info *mtd, loff_t from,
			      struct mtd_oob_ops *ops)
{
	struct ra_nand_chip *ra = mtd->priv;
	int ret;

	ra_dbg("%s: \n", __func__);

	nand_get_device(ra, FL_READING);

	ret = nand_do_read_ops(ra, from, ops);

	nand_release_device(ra);

	return ret;
}

/**
 * nand_write_oob - [MTD Interface] NAND write data and/or out-of-band
 * @mtd:	MTD device structure
 * @to:		offset to write to
 * @ops:	oob operation description structure
 */
static int 
ramtd_nand_writeoob(struct mtd_info *mtd, loff_t to,
			       struct mtd_oob_ops *ops)
{
	struct ra_nand_chip *ra = mtd->priv;
	int ret;

	ra_dbg("%s: \n", __func__);

	nand_get_device(ra, FL_READING);

	ret = nand_do_write_ops(ra, to, ops);

	nand_release_device(ra);

	return ret;
}



/**
 * nand_block_isbad - [MTD Interface] Check if block at offset is bad
 * @mtd:	MTD device structure
 * @offs:	offset relative to mtd start
 */
static int 
ramtd_nand_block_isbad(struct mtd_info *mtd, loff_t offs)
{

//	ra_dbg("%s: \n", __func__);

	if (offs > mtd->size)
		return -EINVAL;
	return 0;
	return nand_block_checkbad((struct ra_nand_chip *)mtd->priv, offs
#ifdef TCSUPPORT_NAND_RT63368
	, BAD_BLOCK_RAW
#endif
	);
}


/**
 * nand_block_markbad - [MTD Interface] Mark block at the given offset as bad
 * @mtd:	MTD device structure
 * @ofs:	offset relative to mtd start
 */
static int 
ramtd_nand_block_markbad(struct mtd_info *mtd, loff_t ofs)
{
	int ret;
	struct ra_nand_chip *ra = mtd->priv;

	ra_dbg("%s: \n", __func__);

	nand_get_device(ra, FL_WRITING);

	ret = nand_block_markbad(ra, ofs
#ifdef TCSUPPORT_NAND_RT63368
	, BAD_BLOCK_RAW
#endif
	);

	nand_release_device(ra);
	return ret; 
}



/************************************************************
 * the init/exit section.
 */

static int nand_access_page_read_proc(char *buf, char **start, off_t off,
         int count, int *eof, void *data)
{
    int len;

	len = 0;
    len += sprintf(buf, "%d\n", ranfc_page);

    len -= off;
    *start = buf + off;

    if (len > count)
        len = count;
    else
        *eof = 1;

    if (len < 0)
        len = 0;

    return len;

}


static int nand_flags_read_proc(char *buf, char **start, off_t off,
         int count, int *eof, void *data)
{
    int len;

	len = 0;
    len += sprintf(buf+len, "NAND Flags Status : %d\n", ranfc_flags);
	len += sprintf(buf+len, " 0 : NONE\n");
	len += sprintf(buf+len, " 1 : ECC Enable\n");
	len += sprintf(buf+len, " 2 : GDMA Enable\n");
	len += sprintf(buf+len, " 4 : Read/Write Verification Enable\n");
	len += sprintf(buf+len, "-------------------------------------\n");

    len -= off;
    *start = buf + off;

    if (len > count)
        len = count;
    else
        *eof = 1;

    if (len < 0)
        len = 0;

    return len;

}



static int nand_flags_write_proc(struct file *file, const char *buffer,
    unsigned long count, void *data) {
    char val_string[8];

    if (count > sizeof(val_string) - 1)
        return -EINVAL;

    if (copy_from_user(val_string, buffer, count))
        return -EFAULT;

    val_string[count] = '\0';
	sscanf(val_string, "%d", &ranfc_flags);

	printk(" nand flag : %d\n", ranfc_flags);
	printk(" 0 : NONE\n");
	printk(" 1 : ECC Enable\n");
	printk(" 2 : GDMA Enable\n");
	printk(" 4 : Read/Write Verification Enable\n");
	printk("-------------------------------------\n");

	if (ranfc_flags & 0x01) {
		printk(" ECC Enable\n");
	} 

	if (ranfc_flags & 0x02) {
		printk(" GDMA Enable\n");
	}

	if (ranfc_flags & 0x04) {
		printk(" Read/Write Verification Enable\n");
	}

    return count;
}


extern int
doNandRegCheck(uint32_t type, uint32_t pattern);

#define REG_DEF_CHK 1
#define REG_RW_CHK  2

static int nand_regchk_write_proc(struct file *file, const char *buffer,
    unsigned long count, void *data) {
	
    char val_string[8];
	int type;

    if (count > sizeof(val_string) - 1)
        return -EINVAL;

    if (copy_from_user(val_string, buffer, count))
        return -EFAULT;

    val_string[count] = '\0';
	sscanf(val_string, "%d", &type);

	printk(" NAND controller register check\n" );
	printk(" type 1 : Reg Def Check\n");
	printk(" teyp 2 : Reg RW Check\n");
	printk(" -------------------------------------\n");

	if (type == REG_RW_CHK) {

		printk("Input Type : %d\n", REG_RW_CHK);
		
		printk("Test Pattern 0xffffffff\n");
		printk("------------------------------------\n");
		doNandRegCheck(REG_RW_CHK, 0xffffffff);
		
		printk("Test Pattern 0x55555555\n");
		printk("------------------------------------\n");
		doNandRegCheck(REG_RW_CHK, 0x55555555);
		
		printk("Test Pattern 0x00000000\n");
		printk("------------------------------------\n");
		doNandRegCheck(REG_RW_CHK, 0x00000000);
		
		printk("Test Pattern 0xaaaaaaaa\n");
		printk("------------------------------------\n");
		doNandRegCheck(REG_RW_CHK, 0xaaaaaaaa);
		
		printk("Test Pattern 0xa5a5a5a5\n");
		printk("------------------------------------\n");
		doNandRegCheck(REG_RW_CHK, 0xa5a5a5a5);

		printk("Test Pattern 0x5a5a5a5a\n");
		printk("------------------------------------\n");
		doNandRegCheck(REG_RW_CHK, 0x5a5a5a5a);
		
	} 

    return count;
}

#ifdef TCSUPPORT_NAND_RT63368
int mt6573_nand_erase_hw(struct ra_nand_chip *ra, unsigned long page)
{
	return nfc_erase_block(ra, page);
}

int mt6573_nand_exec_write_page(struct ra_nand_chip *ra, int page, u32 page_size, u8 *dat, u8 *oob)
{
	memset(ra->buffers, 0xff, sizeof(ra->buffers));
	memcpy(ra->buffers, dat, page_size);
	memcpy(ra->buffers + page_size, oob, 1 << ra->flash->oob_shift);

	return nfc_write_page(ra, ra->buffers, page, FLAG_ECC_EN);
}

int mt6573_nand_exec_read_page(struct ra_nand_chip *ra, int page, u32 page_size, u8 *dat, u8 *oob)
{
	int ret = 0;
	ret = nfc_read_page(ra, ra->buffers, page, FLAG_ECC_EN | FLAG_VERIFY);

	if(ret)
	{
		ret = nfc_read_page(ra, ra->buffers, page, FLAG_ECC_EN | FLAG_VERIFY);
		if(ret)
		{
			printk("[%s]: read again fail!", __func__);
			goto read_fail;
		}
	}

	memcpy(dat, ra->buffers, page_size);
	memcpy(oob, ra->buffers + page_size, 1 << ra->flash->oob_shift);

read_fail:
	return ret;
}

int mt6573_nand_block_markbad_hw(struct ra_nand_chip *ra, unsigned long ofs, unsigned long bmt_block)
{
	unsigned long page;
	int block;

	block = ofs >> ra->flash->erase_shift;
	page = block * (1<<(ra->flash->erase_shift - ra->flash->page_shift));

	nfc_erase_block(ra, page);
	nand_block_markbad(ra, ofs, bmt_block);
	return 0;
}

int mt6573_nand_block_bad_hw(struct ra_nand_chip *ra, unsigned long ofs, unsigned long bmt_block)
{
	return nand_block_checkbad(ra, ofs, bmt_block);
}

int calc_bmt_pool_size(struct ra_nand_chip *ra)
{
	int chip_size = 1 << ra->flash->chip_shift;
	int block_size = 1 << ra->flash->erase_shift;
	int total_block = chip_size / block_size;
	int last_block = total_block - 1;

	u16 valid_block_num = 0;
	u16 need_valid_block_num = total_block * POOL_GOOD_BLOCK_PERCENT;
#if 0
	printk("need_valid_block_num:%d \n", need_valid_block_num);
	printk("total block:%d \n", total_block);
#endif
	for(;last_block > 0; --last_block)
	{
		if(nand_block_checkbad(ra, last_block * block_size, BAD_BLOCK_RAW))
		{
			continue;
		}
		else
		{
			valid_block_num++;
			if(valid_block_num == need_valid_block_num)
			{
				break;
			}

		}
	}

	return (total_block - last_block);
}
#endif

static struct mtd_info *nandflash_probe(struct map_info *map)
//int __devinit ra_nand_init(void) 
{
	int ret, num;

	/* frankliao added for nand flash test */
    struct proc_dir_entry *nand_flags_proc;
    struct proc_dir_entry *nand_regchk_proc;
    struct proc_dir_entry *nand_access_page_proc;

	/* frank */
	if ( (ra_inl(CR_AHB_BASE+0x64) >> 16) == 0x4){
		ra_outl(0xbfb00860, ra_inl(0xbfb00860) | (0x00000100));
	}


	// frank modify 20110425
	ra_outl(NFC_CTRL, ra_inl(NFC_CTRL) | 0x01); //set wp to high

	nfc_all_reset(ra);

	ret = nand_setup();

	if (ret == -1) {
		printk("%s: nand flash setup flash info and opcode fail\n", __func__);
		return NULL;
	}

#ifndef CONFIG_MIPS_RT63365
	if (ra->flash->page_shift == SIZE_512iB_BIT) {		// RT63165
		ra_outl(NFC_CTRLII, ra_inl(NFC_CTRLII) | 0x01); 
	}	
#else
	if (ra->flash->page_shift == SIZE_2KiB_BIT) {
		ra_outl(NFC_CTRLII, 0x76501);
	} else  if (ra->flash->page_shift == SIZE_512iB_BIT) {
		ra_outl(NFC_CTRLII, 0x76500);
	}

	ra_outl(NFC_CTRLII, ra_inl(NFC_CTRLII) | 0x02); //big endian for rt63365
#endif	

//	nfc_all_reset(ra);

//	printk("numchips : %d\n", ra->flash->numchips );
//	printk("chip_shift : %d\n", ra->flash->chip_shift );
//	printk("page_shift : %d\n", ra->flash->page_shift );
//	printk("oob_shift : %d\n", ra->flash->oob_shift );
//	printk("erase_shift : %d\n", ra->flash->erase_shift );
//	printk("badblockpos : %d\n", ra->flash->badblockpos );

	//static 
//	ra->numchips		= CONFIG_NUMCHIPS;
//	ra->chip_shift		= CONFIG_CHIP_SIZE_BIT;
//	ra->page_shift		= CONFIG_PAGE_SIZE_BIT;
//	ra->oob_shift		= CONFIG_OOBSIZE_PER_PAGE_BIT;
//	ra->erase_shift		= (CONFIG_PAGE_SIZE_BIT + CONFIG_NUMPAGE_PER_BLOCK_BIT);
//	ra->badblockpos		= CONFIG_BAD_BLOCK_POS;


	ranfc_mtd->type				= MTD_NANDFLASH;
	ranfc_mtd->flags			= MTD_CAP_NANDFLASH;
#ifdef TCSUPPORT_NAND_RT63368
	ranfc_mtd->size				= nand_logic_size;
#else
	ranfc_mtd->size				= CONFIG_NUMCHIPS * (1<<ra->flash->chip_shift);
#endif
	ranfc_mtd->erasesize		= (1<<ra->flash->erase_shift);
	ranfc_mtd->writesize		= (1<<ra->flash->page_shift);
	ranfc_mtd->oobsize 			= (1<<ra->flash->oob_shift);
	ranfc_mtd->oobavail			= ra->oob->oobavail;
	ranfc_mtd->name				= "ra_nfc";
	//ranfc_mtd->index
	ranfc_mtd->ecclayout		= ra->oob;
	//ranfc_mtd->numberaseregions
	//ranfc_mtd->eraseregions
	//ranfc_mtd->bansize
	ranfc_mtd->erase 			= ramtd_nand_erase;
	//ranfc_mtd->point
	//ranfc_mtd->unpoint
	ranfc_mtd->read				= ramtd_nand_read;
	ranfc_mtd->write			= ramtd_nand_write;
	ranfc_mtd->read_oob			= ramtd_nand_readoob;
	ranfc_mtd->write_oob		= ramtd_nand_writeoob;
	//ranfc_mtd->get_fact_prot_info; ranfc_mtd->read_fact_prot_reg; 
	//ranfc_mtd->get_user_prot_info; ranfc_mtd->read_user_prot_reg;
	//ranfc_mtd->write_user_prot_reg; ranfc_mtd->lock_user_prot_reg;
	//ranfc_mtd->writev; ranfc_mtd->sync; ranfc_mtd->lock; ranfc_mtd->unlock; ranfc_mtd->suspend; ranfc_mtd->resume;
	ranfc_mtd->block_isbad		= ramtd_nand_block_isbad;
	ranfc_mtd->block_markbad	= ramtd_nand_block_markbad;
	//ranfc_mtd->reboot_notifier
	//ranfc_mtd->ecc_stats;
	// subpage_sht;

	//ranfc_mtd->get_device; ranfc_mtd->put_device
	ranfc_mtd->priv = ra;
	/* frankliao added 20101222 frank*/
	if (IS_NANDFLASH) {
		map->fldrv_priv = ra;
	}	
	ranand_read_byte = ra_nand_read_byte;
	ranand_read_dword = ra_nand_read_dword;

#ifdef TCSUPPORT_NAND_BADBLOCK_CHECK
	nandflash_scan_badblock();
#endif

#if !defined (__UBOOT__)
	ranfc_mtd->owner = THIS_MODULE;
	ra->controller = &ra->hwcontrol;
	mutex_init(ra->controller);

	/* Register the partitions */
	if (IS_SPIFLASH) {
		num = ARRAY_SIZE(rt63165_test_partitions);
		rt63165_test_partitions[ num-1 ].size = ranfc_mtd->size;
		add_mtd_partitions(ranfc_mtd, rt63165_test_partitions, num);
	}

    nand_flags_proc = create_proc_entry("nand_flag", 0, NULL);
    nand_flags_proc->read_proc = nand_flags_read_proc;
    nand_flags_proc->write_proc = nand_flags_write_proc;

    nand_regchk_proc = create_proc_entry("nand_regchk", 0, NULL);
    nand_regchk_proc->write_proc = nand_regchk_write_proc;

    nand_access_page_proc = create_proc_entry("nand_access_page", 0, NULL);
    nand_access_page_proc->read_proc = nand_access_page_read_proc;



	if (IS_NANDFLASH){

	#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
	#ifdef TCSUPPORT_NAND_RT63368
		offset = SLAVE_IMAGE_OFFSET;
	#else
		offset = (1 << ra->flash->chip_shift)/2;
	#endif
	#endif
		return ranfc_mtd;
	}
	else	
		return 0;
#else
	return ranfc_mtd;
#endif

}

static void nandflash_destroy(struct mtd_info *mtd)
//static void __devexit ra_nand_remove(void)
{
	struct ra_nand_chip *ra;
	
	if (ranfc_mtd) {
		ra = (struct ra_nand_chip  *)ranfc_mtd->priv;

#if !defined (__UBOOT__)
		/* Deregister partitions */
		del_mtd_partitions(ranfc_mtd);
	    kfree(ra);
#else
	    free(ra);
#endif
	}
}

int __devinit ra_nand_init(void) 
{
	nandflash_probe(NULL);
	return 0;
}

void __devexit ra_nand_remove(void)
{
	nandflash_destroy(NULL);
}

static struct mtd_chip_driver nandflash_chipdrv = {
	.probe   = nandflash_probe,
	.destroy = nandflash_destroy,
	.name    = "nandflash_probe",
	.module  = THIS_MODULE
};


static int __init nandflash_probe_init(void)
{
	register_mtd_chip_driver(&nandflash_chipdrv);
	return 0;
}

static void __exit nandflash_probe_exit(void)
{
	unregister_mtd_chip_driver(&nandflash_chipdrv);
}


#if !defined (__UBOOT__)
module_init(nandflash_probe_init);
module_exit(nandflash_probe_exit);
//rootfs_initcall(ra_nand_init);
//module_exit(ra_nand_remove);

MODULE_LICENSE("GPL");
#endif
