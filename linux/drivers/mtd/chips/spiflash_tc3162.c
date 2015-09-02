/*
 * SPIFLASH support for TC3162
 */

/*
 * MTD driver for the SPI Flash Memory support.
 *
 * Copyright (c) 2005-2006 Atheros Communications Inc.
 * Copyright (C) 2006-2007 FON Technology, SL.
 * Copyright (C) 2006-2007 Imre Kaloz <kaloz@openwrt.org>
 * Copyright (C) 2006-2007 Felix Fietkau <nbd@openwrt.org>
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/
#include <linux/proc_fs.h> 

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/version.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/gen_probe.h>
#include <asm/delay.h>
#include <linux/timer.h>
#include <asm/io.h>
#include <asm/tc3162/tc3162.h>
#include "spiflash_tc3162.h"

/* debugging */
/* #define SPIFLASH_DEBUG */
#define TC_SOC

#define MIN(a,b)        ((a) < (b) ? (a) : (b))
#define FALSE 	0
#define TRUE 	1

#define ID_MASK         		0xffff

#define MANUFACTURER_ID(id) 	((id >> 16) & ID_MASK)
#define DEVICE_ID(id)       	(id & ID_MASK)

#define SIZE_64KiB      		0x10000
#define SIZE_64MiB              0x4000000
#define SIZE_32MiB              0x2000000
#define SIZE_16MiB              0x1000000
#define SIZE_8MiB       		0x800000
#define SIZE_4MiB       		0x400000
#define SIZE_2MiB       		0x200000

/* Manufacturers */
#define MANUFACTURER_ST 		0x0020
#define MANUFACTURER_WINBOND	0x00ef
#define MANUFACTURER_SST		0x00bf
#define MANUFACTURER_MXIC		0x00c2
#define MANUFACTURER_SPANSION	0x0001
#define MANUFACTURER_EON		0x001c

/* ST */
#define M25P16	          		0x2015
#define M25P32	          		0x2016
#define M25P64	          		0x2017

/* Winbond */
#define W25X16					0x3015
#define W25X32					0x3016
#define W25X64					0x3017
#define W25X128					0x3017

#define W25Q16					0x4015
#define W25Q32					0x4016
#define W25Q64					0x4017
#define W25Q128					0x4018

/* SST */
#define SST25VF032B				0x254a

/* MXIC */
#define MX25L3205D				0x2016
#define MX25L6405D              0x2017
#define MX25L12805D             0x2018
#define MX25L25635E  			0x2019

/* SPANSION */
#define S25FL016A          		0x0214
#define S25FL032A          		0x0215
#define S25FL064A          		0x0216
#define S25FL128P          		0x2018

/* EON */
#define EN25Q64          		0x3017

#if defined(TC_SOC) 
static __u32 reg0x28;
DECLARE_MUTEX(SPI_SEM);//Make sure all related SPI operations are atomic
#define SPI_REG_BASE		0xbfbc0000
#define SPI_REG_MASTER		0xbfbc0028
#define SPI_REG_MOREBUF		0xbfbc002c
#define SPI_FLASH_DATA2          0x0C
#define SPI_FLASH_DATA3          0x10
#define SPI_FLASH_DATA4          0x14
#define SPI_FLASH_DATA5          0x18
#define SPI_FLASH_DATA6          0x1C
#define SPI_FLASH_DATA7          0x20
#define SPI_FLASH_DATA8          0x24
#endif
static __u32 spiflash_regread32(int reg);
static void spiflash_regwrite32(int reg, __u32 data);
static __u32 spiflash_sendcmd (int op);

struct spi_flash_info {
	const u16 mfr_id;
	const u16 dev_id;
	const u16 extra_id;
	const char *name;
	const int DeviceSize;
	const int EraseSize;
};

#if defined(TCSUPPORT_VOIP)
/*#11542: For voice afftected by Flash action issue*/
typedef struct spiEraseParam_s{
	struct mtd_info* tmpMtd;
	struct erase_info* tmpInstr;
}spiEraseParam_t;
spiEraseParam_t spiEraseParam;
static struct timer_list eraseCheck_Timer;
extern atomic_t eraseAction;
#endif

struct spi_chip_info {
	struct spi_flash_info *flash;
	void (*destroy)(struct spi_chip_info *chip_info);

	u32 (*read)(struct map_info *map, u32 from, u32 to, u32 size);
	u32 (*write)(struct map_info *map, u32 from, u32 to, u32 size);
	u32 (*erase)(struct map_info *map, u32 addr);
};

/* Mapping of generic opcodes to STM serial flash opcodes */
struct opcodes {
    __u16 code;
    __s8 tx_cnt;
    __s8 rx_cnt;
} stm_opcodes[] = {
        {STM_OP_WR_ENABLE, 1, 0},
        {STM_OP_WR_DISABLE, 1, 0},
        {STM_OP_RD_STATUS, 1, 1},
        {STM_OP_WR_STATUS, 2, 0},
        {STM_OP_RD_DATA, 4, 4},
        {STM_OP_FAST_RD_DATA, 1, 0},
        {STM_OP_PAGE_PGRM, 8, 0},
        {STM_OP_SECTOR_ERASE, 4, 0},
        {STM_OP_BULK_ERASE, 1, 0},
        {STM_OP_DEEP_PWRDOWN, 1, 0},
        {STM_OP_RD_SIG, 4, 1},
        {STM_OP_RD_ID, 1, 3}
};

static wait_queue_head_t spiflash_wq;
static spinlock_t spiflash_mutex;
static int spiflash_state;

static __u8 byte_program_mode = 0;
static __u8 four_byte_mode = 0;

static __u32
spiflash_regread32(int reg)
{
	volatile __u32 *addr = (__u32 *)(CR_SPI_BASE + reg);

	return (*addr);
}

static void
spiflash_regwrite32(int reg, __u32 data)
{
	volatile __u32 *addr = (__u32 *)(CR_SPI_BASE + reg);

	*addr = data;
	return;
}

static __u32
spiflash_sendcmd (int op)
{
	 __u32 reg;
	 __u32 mask;
	struct opcodes *ptr_opcode;

	ptr_opcode = &stm_opcodes[op];

	do {
		reg = spiflash_regread32(SPI_FLASH_CTL);
	} while (reg & SPI_CTL_BUSY);

	spiflash_regwrite32(SPI_FLASH_OPCODE, ptr_opcode->code);

	reg = (reg & ~SPI_CTL_TX_RX_CNT_MASK) | ptr_opcode->tx_cnt |
        	(ptr_opcode->rx_cnt << 4) | SPI_CTL_START;

	spiflash_regwrite32(SPI_FLASH_CTL, reg);

	if (ptr_opcode->rx_cnt > 0) {
        	do {
          		reg = spiflash_regread32(SPI_FLASH_CTL);
        	} while (reg & SPI_CTL_BUSY);

        	reg = (__u32) spiflash_regread32(SPI_FLASH_DATA);

        	switch (ptr_opcode->rx_cnt) {
        	case 1:
            		mask = 0x000000ff;
            		break;
        	case 2:
            		mask = 0x0000ffff;
            		break;
        	case 3:
            		mask = 0x00ffffff;
            		break;
        	default:
            		mask = 0xffffffff;
            		break;
        	}

        	reg &= mask;
   	} else {
       		reg = 0;
	}

	return reg;
}

/* Probe SPI flash device
 * Function returns 0 for failure.
 * and flashconfig_tbl array index for success.
 */
static u32
spiflash_read_id (void)
{
	u32 flash_id;

	flash_id = spiflash_sendcmd(SPI_RD_ID);
	flash_id = ((flash_id & 0xff) << 16) | (flash_id & 0xff00) | ((flash_id >> 16) & 0xff);
	return flash_id;
}

static u32
spiflash_erase (struct map_info *map, u32 addr)
{
	struct opcodes *ptr_opcode;
	__u32 temp, reg;
	int finished = FALSE;

	ptr_opcode = &stm_opcodes[SPI_SECTOR_ERASE];

	temp = ((__u32)addr << 8) | (__u32)(ptr_opcode->code);
	spiflash_sendcmd(SPI_WRITE_ENABLE);
	do {
		reg = spiflash_regread32(SPI_FLASH_CTL);
	} while (reg & SPI_CTL_BUSY);

	spiflash_regwrite32(SPI_FLASH_OPCODE, temp);

	if(four_byte_mode == 1){
		reg = ((__u32)addr & 0xff000000) |(reg & ~SPI_CTL_TX_RX_CNT_MASK) | (ptr_opcode->tx_cnt+1) | SPI_CTL_START;
	}else{
		reg = (reg & ~SPI_CTL_TX_RX_CNT_MASK) | ptr_opcode->tx_cnt | SPI_CTL_START;
	}
	spiflash_regwrite32(SPI_FLASH_CTL, reg);

	do {
		reg = spiflash_sendcmd(SPI_RD_STATUS);
		if (!(reg & SPI_STATUS_WIP)) {
			finished = TRUE;
		}
	} while (!finished);

   	return (0);
}

#if defined(TCSUPPORT_VOIP)
static int spiflash_wait_erase_ready(unsigned long data)
{
	int ret;
	unsigned long adr, len;
	struct mtd_info* mtd = spiEraseParam.tmpMtd;
	struct erase_info* instr = spiEraseParam.tmpInstr;
	struct map_info *map = mtd->priv;
	struct spi_chip_info *chip_info = (struct spi_chip_info *)map->fldrv_priv;
	adr = instr->addr;
	len = instr->len;


	while (len) {
		ret = chip_info->erase(map, adr);
		adr += mtd->erasesize;
		len -= mtd->erasesize;

	}
	instr->state = MTD_ERASE_DONE;
	if (instr->callback){
		//printk("Damn! \n");
		instr->callback(instr);
	}
	del_timer(&eraseCheck_Timer);
	up(&SPI_SEM);
	return 0;
}
#endif

/* wait until the flash chip is ready and grab a lock */
static int spiflash_wait_ready(int state)
{
	DECLARE_WAITQUEUE(wait, current);

retry:
	spin_lock_bh(&spiflash_mutex);
	if (spiflash_state != FL_READY) {
		set_current_state(TASK_UNINTERRUPTIBLE);
		add_wait_queue(&spiflash_wq, &wait);
		spin_unlock_bh(&spiflash_mutex);
		schedule();
		remove_wait_queue(&spiflash_wq, &wait);

		if(signal_pending(current))
			return 0;

		goto retry;
	}
	spiflash_state = state;

	return 1;
}

static inline void spiflash_done(void)
{
	spiflash_state = FL_READY;
	spin_unlock_bh(&spiflash_mutex);
	wake_up(&spiflash_wq);
}

static u32
spiflash_read (struct map_info *map, u32 from, u32 to, u32 size)
{
	memcpy((char *)to, (char *)(map->virt + from), size);

   	return (0);
}

static u32
spiflash_write (struct map_info *map, u32 from, u32 to, u32 len)
{
	int done = FALSE, page_offset, bytes_left, finished;
#if defined(TC_SOC) 
	__u32 xact_len, spi_data[8], opcode, reg;
	__u32 reg_value;
	unsigned char words, bytes, finalrun, i, j;
#else
	__u32 xact_len, spi_data = 0, opcode, reg;
#endif
	unsigned char *buf;

	buf = (unsigned char *) from;

	opcode = stm_opcodes[SPI_PAGE_PROGRAM].code;
	bytes_left = len;

	while (done == FALSE) {
		if (byte_program_mode)
			xact_len = MIN(bytes_left, sizeof(__u8));
		else
#if defined(TC_SOC) 
			xact_len = MIN(bytes_left, sizeof(__u32)<<3);
#else
			xact_len = MIN(bytes_left, sizeof(__u32));
#endif

		/* 32-bit writes cannot span across a page boundary
		 * (256 bytes). This types of writes require two page
		 * program operations to handle it correctly. The STM part
		 * will write the overflow data to the beginning of the
		 * current page as opposed to the subsequent page.
		 */
		page_offset = (to & (STM_PAGE_SIZE - 1)) + xact_len;

		if (page_offset > STM_PAGE_SIZE) {
			xact_len -= (page_offset - STM_PAGE_SIZE);
		}

		spiflash_sendcmd(SPI_WRITE_ENABLE);

		do {
			reg = spiflash_regread32(SPI_FLASH_CTL);
		} while (reg & SPI_CTL_BUSY);

#if defined(TC_SOC) 
		words = xact_len >> 2;
//		if(!(xact_len % 4))
		if(!(xact_len & 0x00000003))
			words--;
		bytes = 3;
//		finalrun = xact_len % 4 - 1;
		finalrun = (xact_len & 0x00000003) - 1;
		if(finalrun == 0xFF)
			finalrun = 3;
		for(i = 0; i <= words; i++){
			spi_data[i] = 0;//Make sure the initial value of spi_data[i] is 0
			if(i == words)
				bytes = finalrun;
			for(j = 0; j <= bytes; j++){
				spi_data[i] |= (buf[j + (i<<2)] << (j<<3));
			}
		}
#else
		switch (xact_len) {
			case 1:
			 	spi_data = (__u8)*buf;
				break;
			case 2:
				spi_data = (buf[1] << 8) | buf[0];
				break;
			case 3:
				spi_data = (buf[2] << 16) | (buf[1] << 8) | buf[0];
				break;
			case 4:
				spi_data = (buf[3] << 24) | (buf[2] << 16) |
							(buf[1] << 8) | buf[0];
				break;
			default:
				printk("spiflash_write: default case\n");
				break;
		}
#endif

#if defined(TC_SOC)
        if (!byte_program_mode){
        	/*20101119 pork modified for Slic lower SPI speed request*/
        	reg_value = VPint(SPI_REG_MASTER);
//		VPint(SPI_REG_MASTER) = 0x38804;//Set bit [2] to 1 to enter more buffer mode
		VPint(SPI_REG_MASTER) = reg_value | (1 << 2);
 //       	VPint(SPI_REG_MOREBUF) = 0x20000100;//Set bits [8:0] to 128 (data bit counts) and bits[29:24] to 32(comman bit counts)
        	/* write exact data size into flash */
		if(four_byte_mode == 1){
			VPint(SPI_REG_MOREBUF) = 0x28000000|(xact_len<<3);//Set bits [8:0] to data bit counts and bits[29:24] to 40(command bit counts)
		}else{
           		VPint(SPI_REG_MOREBUF) = 0x20000000|(xact_len<<3);//Set bits [8:0] to data bit counts and bits[29:24] to 32(command bit counts)
		}	
        }
#endif

#if defined(TC_SOC) 
		spiflash_regwrite32(SPI_FLASH_DATA, spi_data[0]);
		if (!byte_program_mode){
			spiflash_regwrite32(SPI_FLASH_DATA2, spi_data[1]);
			spiflash_regwrite32(SPI_FLASH_DATA3, spi_data[2]);
			spiflash_regwrite32(SPI_FLASH_DATA4, spi_data[3]);
			spiflash_regwrite32(SPI_FLASH_DATA5, spi_data[4]);
			spiflash_regwrite32(SPI_FLASH_DATA6, spi_data[5]);
			spiflash_regwrite32(SPI_FLASH_DATA7, spi_data[6]);
			spiflash_regwrite32(SPI_FLASH_DATA8, spi_data[7]);
			if(four_byte_mode == 1){
				opcode = ((__u32)to);
			}else{	
				opcode = (0x02 << 24) | ((__u32)to);
			}	
		}
		else
#else
		spiflash_regwrite32(SPI_FLASH_DATA, spi_data);
#endif
		opcode = (opcode & SPI_OPCODE_MASK) | ((__u32)to << 8);

		spiflash_regwrite32(SPI_FLASH_OPCODE, opcode);

		if(four_byte_mode == 1){
#if defined(TC_SOC)
			reg = ((0x02 << 24) | (reg & 0x00ffff00)) | (xact_len + 5) | SPI_CTL_START; 
#else
			reg = ((__u32)to & 0xff000000 ) | (reg & 0x00ffff00) | (xact_len + 5) | SPI_CTL_START;
#endif
		}else{
			reg = (reg & ~SPI_CTL_TX_RX_CNT_MASK) | (xact_len + 4) | SPI_CTL_START;
		}	
		spiflash_regwrite32(SPI_FLASH_CTL, reg);
		finished = FALSE;

#if defined(TC_SOC) 
		if (!byte_program_mode){
			/*20101119 pork modified for Slic lower SPI speed request*/
			while(VPint(SPI_REG_BASE) & 0x10000);//Make sure the bit spi_master_busy is 0 and then continue
			VPint(SPI_REG_MOREBUF) = 0x00000000;//Set the default value back
//			VPint(SPI_REG_MASTER) = 0x38800;//Set the default value back
			VPint(SPI_REG_MASTER) = reg_value;
		}
#endif

		do {
			udelay(1);
			reg = spiflash_sendcmd(SPI_RD_STATUS);
			if (!(reg & SPI_STATUS_WIP)) {
				finished = TRUE;
			}
		} while (!finished);

		bytes_left -= xact_len;
		to += xact_len;
		buf += xact_len;

		if (bytes_left == 0) {
			done = TRUE;
		}
	}

   	return (0);
}

static struct spi_flash_info flash_tables[] = {
	{
		mfr_id: MANUFACTURER_ST,
		dev_id: M25P64,
		name: "ST M25P64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_ST,
		dev_id: M25P32,
		name: "ST M25P32",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_ST,
		dev_id: M25P16,
		name: "ST M25P16",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25X128,
		name: "Winbond W25X128",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25X64,
		name: "Winbond W25X64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25X32,
		name: "Winbond W25X32",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25X16,
		name: "Winbond W25X16",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25Q128,
		name: "Winbond W25Q128",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25Q64,
		name: "Winbond W25Q64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25Q32,
		name: "Winbond W25Q32",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_WINBOND,
		dev_id: W25Q16,
		name: "Winbond W25Q16",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SST,
		dev_id: SST25VF032B,
		name: "SST 25VF032B",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX25L3205D,
		name: "MX25L3205D",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX25L6405D,
		name: "MX25L6405D",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX25L12805D,
		name: "MX25L12805D",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_MXIC,
		dev_id: MX25L25635E,
		name: "MX25L25635DE",
		DeviceSize: SIZE_32MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL064A,
		name: "S25FL064A",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL032A,
		name: "S25FL032A",
		DeviceSize: SIZE_4MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL016A,
		name: "S25FL016A",
		DeviceSize: SIZE_2MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_SPANSION,
		dev_id: S25FL128P,
		name: "S25FL128P",
		DeviceSize: SIZE_16MiB,
		EraseSize: SIZE_64KiB,
	},
	{
		mfr_id: MANUFACTURER_EON,
		dev_id: EN25Q64,
		name: "EN25Q64",
		DeviceSize: SIZE_8MiB,
		EraseSize: SIZE_64KiB,
	},
};

static void spiflash_unlock (void)
{
	struct opcodes *ptr_opcode;
	__u32 reg;
	__u32 status_reg;

	status_reg = spiflash_sendcmd(SPI_RD_STATUS);
	ptr_opcode = &stm_opcodes[SPI_WR_STATUS];

	spiflash_sendcmd(SPI_WRITE_ENABLE);
	do {
		reg = spiflash_regread32(SPI_FLASH_CTL);
	} while (reg & SPI_CTL_BUSY);

	status_reg &= ~0x1c;
	spiflash_regwrite32(SPI_FLASH_DATA, status_reg);
	spiflash_regwrite32(SPI_FLASH_OPCODE, ptr_opcode->code);

	reg = (reg & ~SPI_CTL_TX_RX_CNT_MASK) | ptr_opcode->tx_cnt | SPI_CTL_START;
	spiflash_regwrite32(SPI_FLASH_CTL, reg);
}

static struct spi_chip_info *spiflash_tc3162_setup(struct map_info *map)
{
	struct spi_chip_info *chip_info;

	chip_info = kmalloc(sizeof(*chip_info), GFP_KERNEL);
	if (!chip_info) {
		printk(KERN_WARNING "Failed to allocate memory for chip_info\n");
		return NULL;
	}

	memset(chip_info, 0, sizeof(struct spi_chip_info));

	return chip_info;
}

static void spiflash_tc3162_destroy(struct spi_chip_info *chip_info)
{
	if (chip_info) {
		kfree(chip_info);
	}
}

struct spi_chip_info *spiflash_probe_tc3162(struct map_info *map)
{
	int i;
	struct spi_chip_info *chip_info = NULL;
	unsigned long flash_id;
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	if(down_interruptible(&SPI_SEM))
		return -ERESTARTSYS;

	*((__u32 *)(CR_SPI_BASE | SPI_FLASH_MM)) = reg0x28;
#endif
	spin_lock_init(&spiflash_mutex);
	init_waitqueue_head(&spiflash_wq);
	spiflash_state = FL_READY;

	flash_id = spiflash_read_id();

	for (i=0; i < ARRAY_SIZE(flash_tables); i++) {
		if ((MANUFACTURER_ID(flash_id) == flash_tables[i].mfr_id) &&
		    (DEVICE_ID(flash_id) == flash_tables[i].dev_id)) {

			if (MANUFACTURER_ID(flash_id) == MANUFACTURER_SST) {
				spiflash_unlock();
				byte_program_mode = 1;
			}

			if(flash_tables[i].DeviceSize >= SIZE_32MiB){
				VPint(SPI_REG_BASE) |= ((1<<19)|(1<<20));
				four_byte_mode = 1;	
			}
			chip_info = spiflash_tc3162_setup(map);
			if (chip_info) {
				chip_info->flash      = &flash_tables[i];
				chip_info->destroy    = spiflash_tc3162_destroy;

				chip_info->read       = spiflash_read;
				chip_info->write      = spiflash_write;
				chip_info->erase      = spiflash_erase;
			}
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
			up(&SPI_SEM);
#endif
			return chip_info;
		}
	}
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	up(&SPI_SEM);
#endif
	return NULL;
}

#if defined(TCSUPPORT_VOIP)
int mtd_spiflash_erase_sp(struct mtd_info *mtd, struct erase_info *instr)
{
	struct map_info *map = mtd->priv;
	struct spi_chip_info *chip_info = (struct spi_chip_info *)map->fldrv_priv;
	unsigned long adr, len;
	int ret = 0;
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	if(down_interruptible(&SPI_SEM))
		return -ERESTARTSYS;

	*((__u32 *)(CR_SPI_BASE | SPI_FLASH_MM)) = reg0x28;
#endif

	if (instr->addr & (mtd->erasesize - 1)){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up(&SPI_SEM);
#endif
		return -EINVAL;
	}

	if (instr->len & (mtd->erasesize -1)){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up(&SPI_SEM);
#endif
		return -EINVAL;
	}
	if ((instr->len + instr->addr) > mtd->size){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up(&SPI_SEM);
#endif
		return -EINVAL;
	}

	memset(&spiEraseParam,0,sizeof(spiEraseParam));
	spiEraseParam.tmpMtd = mtd;
	spiEraseParam.tmpInstr = instr;
	init_timer(&eraseCheck_Timer);
	eraseCheck_Timer.data = NULL;
	eraseCheck_Timer.function = spiflash_wait_erase_ready;
	eraseCheck_Timer.expires = jiffies + 1;
	add_timer(&eraseCheck_Timer);
	return 0;
}
#endif

int mtd_spiflash_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	struct map_info *map = mtd->priv;
	struct spi_chip_info *chip_info = (struct spi_chip_info *)map->fldrv_priv;
	unsigned long adr, len;
	int ret = 0;
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	if(down_interruptible(&SPI_SEM))
		return -ERESTARTSYS;

	*((__u32 *)(CR_SPI_BASE | SPI_FLASH_MM)) = reg0x28;
#endif
	if (!chip_info->erase){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up(&SPI_SEM);
#endif
		return -EOPNOTSUPP;
	}

	if (instr->addr & (mtd->erasesize - 1)){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up(&SPI_SEM);
#endif
		return -EINVAL;
	}

	if (instr->len & (mtd->erasesize -1)){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up(&SPI_SEM);
#endif
		return -EINVAL;
	}		

	if ((instr->len + instr->addr) > mtd->size){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up(&SPI_SEM);
#endif
		return -EINVAL;
	}

	adr = instr->addr;
	len = instr->len;

	while (len) {
		if (!spiflash_wait_ready(FL_ERASING)){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
			up(&SPI_SEM);
#endif
			return -EINTR;
		}
		ret = chip_info->erase(map, adr);
		spiflash_done();
		if (ret){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
			up(&SPI_SEM);
#endif
			return ret;
		}

		adr += mtd->erasesize;
		len -= mtd->erasesize;
	}

	instr->state = MTD_ERASE_DONE;
	if (instr->callback)
		instr->callback(instr);

#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	up(&SPI_SEM);
#endif
	return 0;
}

int mtd_spiflash_read(struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf)
{
	struct map_info *map = mtd->priv;
	struct spi_chip_info *chip_info = (struct spi_chip_info *)map->fldrv_priv;
	int ret = 0;
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	if(down_interruptible(&SPI_SEM))
		return -ERESTARTSYS;
#if defined(TCSUPPORT_VOIP)
	atomic_set(&eraseAction, 1);
#endif
	*((__u32 *)(CR_SPI_BASE | SPI_FLASH_MM)) = reg0x28;
#endif
	if (!chip_info->read){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up(&SPI_SEM);
#endif
#if defined(TCSUPPORT_VOIP)
	atomic_set(&eraseAction, 0);
#endif
		return -EOPNOTSUPP;
	}

	ret = chip_info->read(map, from, (u32)buf, len);
	if (ret){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up(&SPI_SEM);
#endif
#if defined(TCSUPPORT_VOIP)
		atomic_set(&eraseAction, 0);
#endif
		return ret;
	}

	if(retlen)
		(*retlen) = len;

#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	up(&SPI_SEM);
#endif
#if defined(TCSUPPORT_VOIP)
		atomic_set(&eraseAction, 0);
#endif
	return 0;
}

int mtd_spiflash_write(struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf)
{
	struct map_info *map = mtd->priv;
	struct spi_chip_info *chip_info = (struct spi_chip_info *)map->fldrv_priv;
	int ret = 0;
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	if(down_interruptible(&SPI_SEM))
		return -ERESTARTSYS;

#if defined(TCSUPPORT_VOIP)
		atomic_set(&eraseAction, 1);
#endif
	*((__u32 *)(CR_SPI_BASE | SPI_FLASH_MM)) = reg0x28;
#endif	
	if (!chip_info->write){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up(&SPI_SEM);
#endif
#if defined(TCSUPPORT_VOIP)
		atomic_set(&eraseAction, 0);
#endif
		return -EOPNOTSUPP;
	}

	if (!spiflash_wait_ready(FL_WRITING)){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up(&SPI_SEM);
#endif
#if defined(TCSUPPORT_VOIP)
		atomic_set(&eraseAction, 0);
#endif
		return -EINTR;
	}
	ret = chip_info->write(map, (u32)buf, to, len);
	spiflash_done();
	if (ret){
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
		up(&SPI_SEM);
#endif
#if defined(TCSUPPORT_VOIP)
		atomic_set(&eraseAction, 0);
#endif
		return ret;
	}

	if (retlen)
		(*retlen) = len;

#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	up(&SPI_SEM);
#endif
#if defined(TCSUPPORT_VOIP)
	atomic_set(&eraseAction, 0);
#endif
	return 0;
}

#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
int offset = 0;
#endif

static struct mtd_info *spiflash_probe(struct map_info *map)
{
	struct spi_chip_info *chip_info = NULL;
	struct mtd_info *mtd;

	chip_info = spiflash_probe_tc3162(map);
	if (!chip_info)
		return NULL;

	mtd = kzalloc(sizeof(*mtd), GFP_KERNEL);
	if (!mtd) {
		printk(KERN_WARNING "Failed to allocate memory for MTD device\n");
		return NULL;
	}

	mtd->priv = map;
	mtd->type = MTD_NORFLASH;
#if defined(TCSUPPORT_VOIP)
	mtd->erase   = mtd_spiflash_erase_sp;
#else
	mtd->erase   = mtd_spiflash_erase;
#endif


	mtd->write   = mtd_spiflash_write;
	mtd->read    = mtd_spiflash_read;
	mtd->flags   = MTD_CAP_NORFLASH;
	mtd->name    = map->name;
	mtd->writesize = 1;
	mtd->owner   = THIS_MODULE;

	mtd->size      = chip_info->flash->DeviceSize;
	mtd->erasesize = chip_info->flash->EraseSize;

	map->fldrv_priv = chip_info;

	printk(KERN_INFO "%s: Found SPIFLASH %dMiB %s\n",
		map->name, chip_info->flash->DeviceSize/(1024*1024), chip_info->flash->name);
#ifdef TCSUPPORT_DUAL_IMAGE_ENHANCE
		offset = chip_info->flash->DeviceSize/2;
#endif
	return mtd;
}

static void spiflash_destroy(struct mtd_info *mtd)
{
	struct map_info *map = (struct map_info *)mtd->priv;
	struct spi_chip_info *chip_info = (struct spi_chip_info *)map->fldrv_priv;

	if (chip_info->destroy) {
		chip_info->destroy(chip_info);
	}
}

static struct mtd_chip_driver spiflash_chipdrv = {
	.probe	 = spiflash_probe,
	.destroy = spiflash_destroy,
	.name	 = "spiflash_probe",
	.module	 = THIS_MODULE
};

#if  defined(TCSUPPORT_SUPPORT_FLASH)
static int read_proc_support_flash(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;
	unsigned int flash_id;
	int  index_flash;
	char *buf_proc = NULL;
	char buf_line[200];
	char buf_name[100];
	char *buf_replace = NULL;
	int total_len=0;

	
	buf_proc = kmalloc(4*1024, GFP_KERNEL);
	if (!buf_proc) 
	{
		printk(KERN_WARNING "Failed to allocate memory for /proc/tc3162/support_flash\n");
		return 0;
	}

	memset(buf_proc,0,4*1024);

	for(index_flash=0; index_flash < sizeof(flash_tables)/sizeof(struct spi_flash_info); index_flash++)
	{
		strcpy(buf_name,flash_tables[index_flash].name);//replace whitespace with '_'
		while( (buf_replace=strstr(buf_name, " "))!=NULL)
			*buf_replace='#';

		if(flash_tables[index_flash].DeviceSize/0x100000 <4)
			continue;

		flash_id = (flash_tables[index_flash].mfr_id <<16) | ( flash_tables[index_flash].dev_id & 0xffff);

		sprintf(buf_line,"%s %#x %d \n",buf_name , flash_id, 
			flash_tables[index_flash].DeviceSize/0x100000);

		total_len += strlen(buf_line);
		if(total_len>4*1024)
			break;
		strcat(buf_proc,buf_line);
	}

	len = sprintf(page, "%s", buf_proc);

	len -= off;
	*start = page + off;

	if (len > count)
		len = count;
	else
		*eof = 1;

	if (len < 0)
		len = 0;
	
	kfree(buf_proc);
	return len;
}
#endif

static int __init spiflash_probe_init(void)
{
#if defined(TCSUPPORT_SUPPORT_FLASH)
	create_proc_read_entry("tc3162/support_flash", 0, NULL, read_proc_support_flash, NULL);
#endif

#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
	reg0x28 = *((__u32 *)(CR_SPI_BASE | SPI_FLASH_MM));

	if  (isRT63368) {
		reg0x28 &= (0xf000ffff);
		reg0x28 |= (0x10 << 16);
		VPint(SPI_REG_MASTER) = reg0x28;
	}
#endif
	register_mtd_chip_driver(&spiflash_chipdrv);
	return 0;
}

static void __exit spiflash_probe_exit(void)
{
#if defined(TCSUPPORT_SUPPORT_FLASH)
	remove_proc_entry("tc3162/support_flash",  NULL);
#endif

	unregister_mtd_chip_driver(&spiflash_chipdrv);
}

module_init(spiflash_probe_init);
module_exit(spiflash_probe_exit);
#if defined(TC_SOC) && defined(CONFIG_MIPS_TC3262)
EXPORT_SYMBOL(SPI_SEM);
#endif

