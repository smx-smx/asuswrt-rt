
#include <linux/types.h>

#include "bmt.h"

typedef struct {
    char signature[3];
    u8 version;
    u8 bad_count;           // bad block count in pool
    u8 mapped_count;        // mapped block count in pool
    u8 checksum;
    u8 reseverd[13];
} phys_bmt_header;

typedef struct {
    phys_bmt_header header;
    bmt_entry table[MAX_BMT_SIZE];
} phys_bmt_struct;

typedef struct {
    char signature[3];
} bmt_oob_data;

typedef struct {
    char signature[4];
    u32 checksum;
    u8 version;
    u8 badblock_count;
    u8 reserved[2];
}init_table_header;

typedef struct {
    init_table_header header;
    u16 badblock_table[MAX_RAW_BAD_BLOCK_SIZE];
}init_table_struct;

static char BBT_SIGNATURE[] = "RAWB";
#define BBT_SIGNATURE_SIZE      (4)

static char MAIN_SIGNATURE[] = "BMT";
#define SIGNATURE_SIZE      (3)

#define MAX_DAT_SIZE        0x1000
#define MAX_OOB_SIZE        0x80
#define MAX_RAW_BBT_SIZE    65536

static u8 need_write_bmt_to_nand = 0;
static u8 need_write_bbt_to_nand = 0;

#define __UBOOT_NAND__

static struct ra_nand_chip *nand_chip_bmt = NULL;

#define BLOCK_SIZE_BMT          (1 << nand_chip_bmt->flash->erase_shift)
#define PAGE_SIZE_BMT           (1 << nand_chip_bmt->flash->page_shift)

#define OFFSET(block)       ((block) * BLOCK_SIZE_BMT)           
#define PAGE_ADDR(block)    ((block) * BLOCK_SIZE_BMT / PAGE_SIZE_BMT)

/*********************************************************************
* Flash is splited into 2 parts, system part is for normal system    *
* system usage, size is system_block_count, another is replace pool  *
*    +-------------------------------------------------+             *
*    |     system_block_count     |   bmt_block_count  |             *
*    +-------------------------------------------------+             *
*********************************************************************/
static u32 total_block_count;       // block number in flash
static u32 system_block_count;      // system block number
static int bmt_block_count;         // bmt block number

int nand_logic_size;                // logic size
static int page_per_block;          // page per count
static int oob_bad_index_offset = OOB_INDEX_OFFSET;       // bad index offset in oob

static u32 bmt_block_index = 0;     // bmt block index
static bmt_struct bmt;              // dynamic created global bmt table

static u32 bbt_block_index = 0;     // bbt block index
static init_bbt_struct init_bbt;    // dynamic created global bbt table
static u16 pBbt[MAX_RAW_BBT_SIZE];              // raw badblock table

static u8 dat_buf[MAX_DAT_SIZE];
static u8 oob_buf[MAX_OOB_SIZE];
static bool pool_erased;

/***************************************************************
*                                                              
* Interface adaptor for preloader/uboot/kernel                 
*    These interfaces operate on physical address, read/write
*       physical data.
*                                                              
***************************************************************/

int nand_read_page_bmt(u32 page, u8 *dat, u8 *oob)
{
    return mt6573_nand_exec_read_page(nand_chip_bmt, page, PAGE_SIZE_BMT, dat, oob);
}

int nand_block_bad_bmt(u32 offset, u32 bmt_block)
{
    return mt6573_nand_block_bad_hw(nand_chip_bmt, offset, bmt_block);
}

int nand_erase_bmt(u32 offset)
{

    return mt6573_nand_erase_hw(nand_chip_bmt, offset / PAGE_SIZE_BMT);

}

int mark_block_bad_bmt(u32 offset, u32 bmt_block)
{
    return mt6573_nand_block_markbad_hw(nand_chip_bmt, offset, bmt_block);
}

int nand_write_page_bmt(u32 page, u8 *dat, u8 *oob)
{
    return mt6573_nand_exec_write_page(nand_chip_bmt, page, PAGE_SIZE_BMT, dat, oob);
}

/***************************************************************
*                                                              *
* static internal function                                     *
*                                                              *
***************************************************************/
static void dump_bmt_info(bmt_struct *bmt)
{
    int i;
    
    MSG("BMT v%d.", bmt->version);
    MSG("total %d mapping\n", bmt->mapped_count);
    for (i = 0; i < bmt->mapped_count; i++)
    {
        MSG("%d -> %d \n", bmt->table[i].bad_index, bmt->table[i].mapped_index);
    }
}

static bool match_bmt_signature(u8 *dat, u8 *oob)
{
    if (memcmp(dat + MAIN_SIGNATURE_OFFSET, MAIN_SIGNATURE, SIGNATURE_SIZE))
    {
        return false;
    }
 
    return true;
}

static bool match_bbt_signature(u8 *dat, u8 *oob)
{
    if (memcmp(dat + BBT_SIGNATURE_OFFSET, BBT_SIGNATURE, BBT_SIGNATURE_SIZE))
    {
        return false;
    }

    return true;
    
}

static u8 cal_bmt_checksum(phys_bmt_struct *phys_table, int bmt_size)
{
    int i;
    u8 checksum = 0;
    u8 *dat = (u8 *)phys_table;

    checksum += phys_table->header.version;
    checksum += phys_table->header.mapped_count;

    dat += sizeof(phys_bmt_header);
    for (i = 0; i < bmt_size * sizeof(bmt_entry); i++)
    {
        checksum += dat[i];
    }

    return checksum;
}

static u16 cal_bbt_checksum(init_table_struct *bbt_table)
{
    int i;
    u16 checksum = 0;
    u8 *dat = (u8*)bbt_table;

    checksum += bbt_table->header.version;
    checksum += bbt_table->header.badblock_count;

    dat += sizeof(init_table_header);

    for (i = 0; i < sizeof(bbt_table->badblock_table); i++)
    {
        checksum += dat[i];            

    }

    return checksum;

}

static int is_block_mapped(int index)
{
    int i;
    for (i = 0; i < bmt.mapped_count; i++)
    {
        if (index == bmt.table[i].mapped_index)
            return i;
    }
    return -1;
}

static int is_badblock_raw(u16 index)
{
    int i;
    
    for (i = 0; i < bmt.mapped_count; i++)
    {
        if (index == bmt.table[i].bad_index)
            return 0;
    }
    return 1;

}

static bool valid_bmt_data(phys_bmt_struct *phys_table)
{
    int i;
    u8 checksum = cal_bmt_checksum(phys_table, bmt_block_count);
    
    // checksum correct?
    if ( phys_table->header.checksum != checksum)
    {
        MSG("bmt checksum error \n");
        return false;
    }
    
    // block index correct?
    for (i = 0; i < phys_table->header.mapped_count; i++)
    {
        if (phys_table->table[i].bad_index >= total_block_count ||
            phys_table->table[i].mapped_index >= total_block_count ||
            phys_table->table[i].mapped_index < system_block_count)
        {
            MSG("bmt block index error \n");
            return false;
        }
    }

    // pass check, valid bmt.
    MSG("Valid BMT, version v%d\n", phys_table->header.version);
    return true;
}

static bool valid_bbt_data(init_table_struct *bbt_table)
{
    int i;
    u16 checksum = cal_bbt_checksum(bbt_table);

    if (bbt_table->header.checksum != checksum)
    {
        MSG("BBT Data checksum error: 0x%x 0x%x\n", bbt_table->header.checksum, checksum);
        return false;
    }

    MSG("BBT Checksum is: 0x%x\n", bbt_table->header.checksum);

    for (i = 0; i < bbt_table->header.badblock_count; i++)
    {
        if (bbt_table->badblock_table[i] >= system_block_count)
        {
            MSG("error: badblock_table[%d]: %d \n", i, bbt_table->badblock_table[i]);
            return false;
        }
    }

    MSG("Valid BBT, version v%d\n", bbt_table->header.version);
    return true;

}


static void fill_nand_bmt_buffer(bmt_struct *bmt, u8 *dat, u8 *oob)
{
    phys_bmt_struct phys_bmt;

    dump_bmt_info(bmt);

    // fill phys_bmt_struct structure with bmt_struct
    memset(&phys_bmt, 0xFF, sizeof(phys_bmt));
    
    memcpy(phys_bmt.header.signature, MAIN_SIGNATURE, SIGNATURE_SIZE);
    phys_bmt.header.version = BMT_VERSION;
    phys_bmt.header.mapped_count = bmt->mapped_count;
    memcpy(phys_bmt.table, bmt->table, sizeof(bmt_entry) * bmt_block_count);

    phys_bmt.header.checksum = cal_bmt_checksum(&phys_bmt, bmt_block_count);

    memcpy(dat + MAIN_SIGNATURE_OFFSET, &phys_bmt, sizeof(phys_bmt));

    return;
}

static void fill_nand_bbt_buffer(init_bbt_struct *bbt, u8 *dat, u8 *oob)
{
    init_table_struct init_table;

    memset(&init_table, 0xFF, sizeof(init_table));

    memcpy(init_table.header.signature, BBT_SIGNATURE, BBT_SIGNATURE_SIZE);
    
    init_table.header.version = BBT_VERSION;
    init_table.header.badblock_count = bbt->badblock_count;

    memcpy(init_table.badblock_table, bbt->badblock_table, sizeof(bbt->badblock_table));

    init_table.header.checksum = cal_bbt_checksum(&init_table);

    memcpy(dat + BBT_SIGNATURE_OFFSET, &init_table, sizeof(init_table));

    return;

}

// return valid index if found BMT, else return 0
static int load_bmt_data(int start, int pool_size)
{
    int bmt_index = start + pool_size - 1;        // find from the end
    phys_bmt_struct phys_table;
    int i;
 
    MSG("begin to search BMT from block %d \n", bmt_index);

    for (bmt_index = start + pool_size - 1; bmt_index >= start; bmt_index--)
    {
        if (nand_block_bad_bmt(OFFSET(bmt_index), BAD_BLOCK_RAW) || nand_block_bad_bmt(OFFSET(bmt_index), BMT_BADBLOCK_GENERATE_LATER))
        {
            MSG("Skip bad block: %d \n", bmt_index);
            continue;
        }
        
        if (nand_read_page_bmt(PAGE_ADDR(bmt_index), dat_buf, oob_buf))
        {
            MSG("Error found when read block: %d\n", bmt_index);
            continue;
        }

        if (!match_bmt_signature(dat_buf, oob_buf))
        {
            continue;
        }

        MSG("Match bmt signature @ block: %d\n", bmt_index);
        
        memcpy(&phys_table, dat_buf + MAIN_SIGNATURE_OFFSET, sizeof(phys_table));

        if (!valid_bmt_data(&phys_table))
        {
            MSG("BMT data is not correct: %d\n", bmt_index);
            continue;
        }
        else
        {
            bmt.mapped_count = phys_table.header.mapped_count;
            bmt.version = phys_table.header.version;
            memcpy(bmt.table, phys_table.table, bmt.mapped_count * sizeof(bmt_entry));

            MSG("bmt found at block: %d, mapped block: %d\n", bmt_index, bmt.mapped_count);

            for (i = 0; i < bmt.mapped_count; i++)
            {
                if (!nand_block_bad_bmt(OFFSET(bmt.table[i].bad_index), BAD_BLOCK_RAW))
                {
                    MSG("block %d is not mark bad, should be power lost last time\n", bmt.table[i].bad_index);
                    mark_block_bad_bmt(OFFSET(bmt.table[i].bad_index), BAD_BLOCK_RAW);
                }
            }
            
            return bmt_index;
        }
    }    

    MSG("bmt not found!\n");
    return 0;
}

static int load_bbt_data(int start, int pool_size, init_bbt_struct *init_bbt)
{
    int i;
    int ret = 0;

    int bbt_index = start;
    init_table_struct init_table;

    for(;bbt_index < (start + pool_size); bbt_index++)
    {

        if (nand_block_bad_bmt(OFFSET(bbt_index), BAD_BLOCK_RAW) || nand_block_bad_bmt(OFFSET(bbt_index), BMT_BADBLOCK_GENERATE_LATER))
        {
            MSG("Skip bad block: %d\n", bbt_index);
            continue;
        }
        
        if (nand_read_page_bmt(PAGE_ADDR(bbt_index), dat_buf, oob_buf))
        {
            MSG("Error found when read block %d\n", bbt_index);
            continue;
        }
        
        if (!match_bbt_signature(dat_buf, oob_buf))
        {
            continue;
        }

        MSG("Match bbt signature \n");

        memcpy(&init_table, dat_buf + BBT_SIGNATURE_OFFSET, sizeof(init_table));

        if (!valid_bbt_data(&init_table))
        {
            MSG("BBT data is not correct \n");
            continue;
        }
        else
        {
            init_bbt->badblock_count = init_table.header.badblock_count;
            init_bbt->version = init_table.header.version;
            memcpy(init_bbt->badblock_table, init_table.badblock_table, (init_bbt->badblock_count) * 2);

            MSG("bbt found, bad block count: %d\n", init_bbt->badblock_count);

            for (i = 0; i < init_bbt->badblock_count; i++)
            {
                MSG("init_bbt->badblock_table[%d]: %d \n", i, init_bbt->badblock_table[i]);
            }
            
            return bbt_index;
        }

    }

    return ret;
    
}


/*************************************************************************
* Find an available block and erase.                                     *
* start_from_end: if true, find available block from end of flash.       *
*                 else, find from the beginning of the pool              *
* need_erase: if true, all unmapped blocks in the pool will be erased    *
*************************************************************************/
static int find_available_block(bool start_from_end)
{
    int i;
    int block = system_block_count;
    int direction;
    MSG("Try to find_available_block, pool_erase: %d\n", pool_erased);

    // erase all un-mapped blocks in pool when finding avaliable block
    if (!pool_erased)
    {
        for (i = 0; i < bmt_block_count; i++)
        {
	        if ((block + i) == bmt_block_index)    
	        {
	            MSG("Skip bmt block %d \n", block + i);
	            continue;
	        }            

	        if ((block + i) == bbt_block_index)    
	        {
	            MSG("Skip bbt block %d \n", block + i);
	            continue;
	        }  
	        
	        if (nand_block_bad_bmt(OFFSET(block + i), BAD_BLOCK_RAW) || nand_block_bad_bmt(OFFSET(block + i), BMT_BADBLOCK_GENERATE_LATER))
            {
                MSG("Skip bad block %d \n", block + i);
                continue;
            }

            if (is_block_mapped(block + i) >= 0)
            {
                MSG("Skip mapped block %d \n", block + i);
                continue;
            }

            if (nand_erase_bmt(OFFSET(block + i)))
            {
                MSG("Erase block %d fail\n", block + i);
                mark_block_bad_bmt(OFFSET(block + i), BMT_BADBLOCK_GENERATE_LATER);
            }
        }

        pool_erased = 1;
    }

    if (start_from_end)
    {
        block = total_block_count - 1;
        direction = -1;
    }
    else
    {
        block = system_block_count;
        direction = 1;
    }

    for (i = 0; i < bmt_block_count; i++, block += direction)
    {
        if (block == bmt_block_index)
        {
            MSG("Skip bmt block %d \n", block);
            continue;
        }

        if (block == bbt_block_index)
        {
            MSG("Skip bbt block %d \n", block);
            continue;
        }
        
        if (nand_block_bad_bmt(OFFSET(block), BAD_BLOCK_RAW) || nand_block_bad_bmt(OFFSET(block), BMT_BADBLOCK_GENERATE_LATER))
        {
            MSG("Skip bad block %d \n", block);
            continue;
        }

        if (is_block_mapped(block) >= 0)
        {
            MSG("Skip mapped block %d \n", block);
            continue;
        }

        MSG("Find block %d available\n", block);
        return block;
    }

    return 0;
}


static unsigned short get_bad_index_from_oob(u8 *oob_buf)
{
    unsigned short index;

	memcpy(&index, oob_buf + oob_bad_index_offset, OOB_INDEX_SIZE);

    return index;
}

void set_bad_index_to_oob(u8 *oob, u16 index)
{
	memcpy(oob + oob_bad_index_offset, &index, sizeof(index));
}

static int migrate_from_bad(int offset, u8 *write_dat, u8 *write_oob)
{
    int page;
    int error_block = offset / BLOCK_SIZE_BMT;
    int error_page = (offset / PAGE_SIZE_BMT) % page_per_block;
    int to_index;

    to_index = find_available_block(false);
  

    if (!to_index)
    {
        MSG("Cannot find an available block for BMT\n");
        return 0;
    }
    
    for (page = 0; page < error_page; page++)
    {
        nand_read_page_bmt(PAGE_ADDR(error_block) + page, dat_buf, oob_buf);

        if (error_block < system_block_count)
        {
            set_bad_index_to_oob(oob_buf, error_block);
        }
 
        if (nand_write_page_bmt(PAGE_ADDR(to_index) + page, dat_buf, oob_buf))
        {
            MSG("Write page %d fail\n", PAGE_ADDR(to_index) + page);
            mark_block_bad_bmt(OFFSET(to_index), BMT_BADBLOCK_GENERATE_LATER);
            return migrate_from_bad(offset, write_dat, write_oob);
        }          
    }


    {       

        memset(oob_buf, 0xFF, sizeof(oob_buf));
        memcpy(oob_buf, write_oob, 1 << nand_chip_bmt->flash->oob_shift);
   
        if (error_block < system_block_count)
            set_bad_index_to_oob(oob_buf, error_block);       // if error_block is already a mapped block, original mapping index is in OOB.
        
        if (nand_write_page_bmt(PAGE_ADDR(to_index) + error_page, write_dat, oob_buf))
        {
            MSG("Write page %d fail\n", PAGE_ADDR(to_index) + error_page);
            mark_block_bad_bmt(OFFSET(to_index), BMT_BADBLOCK_GENERATE_LATER);
            return migrate_from_bad(offset, write_dat, write_oob);
        }
    }

    MSG("Migrate from %d to %d done!\n",error_block, to_index);

    return to_index;
}

static bool write_bmt_to_flash(u8 *dat, u8 *oob)
{
    bool need_erase = true;
    MSG("Try to write BMT\n");
    
    if (bmt_block_index == 0)
    {
        // if we don't have index, we don't need to erase found block as it has been erased in find_available_block()
        need_erase = false;     
        if ( !(bmt_block_index = find_available_block(true)) )
        {
            MSG("Cannot find an available block for BMT\n");
            return false;
        }
    }
   
    // write bmt to flash
    if (need_erase)
    {
        if (nand_erase_bmt(OFFSET(bmt_block_index)))
        {
            MSG("BMT block erase fail, mark bad: 0x%x\n", bmt_block_index);
            mark_block_bad_bmt(OFFSET(bmt_block_index), BMT_BADBLOCK_GENERATE_LATER);
            
            bmt_block_index = 0;
            return write_bmt_to_flash(dat, oob);        // recursive call 
        }
    }

    if ( nand_write_page_bmt(PAGE_ADDR(bmt_block_index), dat, oob) )
    {
        MSG("Write BMT data fail \n");
        mark_block_bad_bmt(OFFSET(bmt_block_index), BMT_BADBLOCK_GENERATE_LATER);
        
        bmt_block_index = 0;
        return write_bmt_to_flash(dat, oob);        // recursive call 
    }

    MSG("Write BMT to block %d success\n", bmt_block_index);
    return true;
}

static bool write_bbt_to_flash(u8 *dat, u8 *oob)
{
    
    if ( !(bbt_block_index = find_available_block(false)) )
    {
        MSG("Cannot find an available block for BBT\n");
        return false;
    }

    if ( nand_write_page_bmt(PAGE_ADDR(bbt_block_index), dat, oob) )
    {
        MSG("Write BBT data fail \n");
        mark_block_bad_bmt(OFFSET(bbt_block_index), BMT_BADBLOCK_GENERATE_LATER);
        
        bbt_block_index = 0;
        return write_bbt_to_flash(dat, oob);        // recursive call 
    }

    MSG("Write BBT to block %d success\n", bbt_block_index);
    return true;
}

int scan_badblock_raw(init_bbt_struct *init_bbt)
{
    u16 block = 0;
    int count = 0;
    
    for(block = 1; block < system_block_count; block++)
    {
        if(nand_block_bad_bmt(OFFSET(block), BAD_BLOCK_RAW))
        {
            if(is_badblock_raw(block))
            {
                if(count >= MAX_RAW_BAD_BLOCK_SIZE)
                {
                    return -1;
                }
                init_bbt->badblock_table[count] = block;
                count++;
            }
            else
            {
                continue;
            }

        }

    }

    init_bbt->badblock_count = count;

    return 0;

}
/*******************************************************************
* Reconstruct bmt, called when found bmt info doesn't match bad 
* block info in flash.
* 
* Return NULL for failure
*******************************************************************/
bmt_struct *reconstruct_bmt(bmt_struct * bmt)
{
    int i;
    int index = system_block_count;
    unsigned short bad_index;
 
    // init everything in BMT struct 
    bmt->version = BMT_VERSION;
    bmt->bad_count = 0;
    bmt->mapped_count = 0;
    
    memset(bmt->table, 0, bmt_block_count * sizeof(bmt_entry));

    for (i = 0; i < bmt_block_count; i++, index++)
    {
        if (nand_block_bad_bmt(OFFSET(index), BAD_BLOCK_RAW) || nand_block_bad_bmt(OFFSET(index), BMT_BADBLOCK_GENERATE_LATER))
        {
            MSG("Skip bad block: %d \n", index);
            continue;
        }

        nand_read_page_bmt(PAGE_ADDR(index), dat_buf, oob_buf);

        if ((bad_index = get_bad_index_from_oob(oob_buf)) >= system_block_count)
        {
            MSG("get bad index: 0x%x \n", bad_index);
            if (bad_index != 0xFFFF)
                MSG("Invalid bad index found in block: %d \n", index);
            continue;
        }

        MSG("Block 0x%x is mapped to bad block: 0x%x\n", index, bad_index);

        if (!nand_block_bad_bmt(OFFSET(bad_index), BAD_BLOCK_RAW))
        {
            mark_block_bad_bmt(OFFSET(bad_index), BAD_BLOCK_RAW);
            MSG("block %d is not marked as bad, mark it\n", bad_index);
        }

        {
            // add mapping to BMT
            bmt->table[bmt->mapped_count].bad_index = bad_index;
            bmt->table[bmt->mapped_count].mapped_index = index;
            bmt->mapped_count++;
        }

        MSG("Add mapping: %d -> %d to BMT\n", bad_index, index);
    }

    MSG("Scan replace pool done, mapped block: %d\n", bmt->mapped_count);

    return bmt;
}

init_bbt_struct* reconstruct_bbt(init_bbt_struct* init_bbt)
{
    init_bbt->version = BBT_VERSION;
    init_bbt->badblock_count = 0;
    memset(init_bbt->badblock_table, 0, sizeof(init_bbt->badblock_table));

    if(scan_badblock_raw(init_bbt))
    {
        MSG("scan_badblock_raw fail (%d)! \n", BBT_VERSION);
        return NULL;
    }

    return init_bbt;
}

/*******************************************************************
* [BMT Interface]
*
* Description:
*   Init bmt from nand. Reconstruct if not found or data error
*
* Parameter:
*   size: size of bmt and replace pool
* 
* Return: 
*   NULL for failure, and a bmt struct for success
*******************************************************************/
bmt_struct *init_bmt(struct ra_nand_chip *ra, int size)
{
    
    if (size > 0 && size < MAX_BMT_SIZE)
    {
        MSG("Init bmt table, size: %d\n", size);
        bmt_block_count = size;
    }
    else
    {
        MSG("Invalid bmt table size: %d\n", size);
        return NULL;
    }

    nand_chip_bmt = ra;
    total_block_count = (1 << ra->flash->chip_shift) / (1 << ra->flash->erase_shift);
    system_block_count = total_block_count - bmt_block_count;

    page_per_block = BLOCK_SIZE_BMT / PAGE_SIZE_BMT;

    MSG("bmt count: %d, system count: %d\n", bmt_block_count, system_block_count);

    // set this flag, and unmapped block in pool will be erased.
    pool_erased = 0;

    // alloc size for bmt.
    memset(bmt.table, 0, size * sizeof(bmt_entry));

    bmt_block_index = load_bmt_data(system_block_count, size);

    // load bmt if exist
    if (bmt_block_index)
    {
        MSG("Load bmt data success @ block %d \n", bmt_block_index);
        dump_bmt_info(&bmt);
        return &bmt;
    }
    else
    {
        MSG("Load bmt data fail! \n");

        if (reconstruct_bmt(&bmt))
        {
            need_write_bmt_to_nand = 1;
            return &bmt;
        }
        else
            return NULL;
    }
	
}

init_bbt_struct* start_init_bbt(void)
{
    bbt_block_index = load_bbt_data(system_block_count, bmt_block_count, &init_bbt);
    
    if(bbt_block_index)
    {
        MSG("Load bbt data success \n");
        return &init_bbt;
    }  
    else
    {

        MSG("Load bbt data fail! \n");
        if(reconstruct_bbt(&init_bbt))
        {
            need_write_bbt_to_nand = 1;
            return &init_bbt;

        }
        else
            return NULL;
    }

}

int write_bbt_or_bmt_to_flash(void)
{
    if(need_write_bmt_to_nand)
    {
        // fill NAND BMT buffer
        memset(dat_buf, 0xFF, sizeof(dat_buf));
        memset(oob_buf, 0xFF, sizeof(oob_buf));
        fill_nand_bmt_buffer(&bmt, dat_buf, oob_buf);

        // write BMT back
        if (!write_bmt_to_flash(dat_buf, oob_buf))
        {
            MSG("save bmt to nand fail! \n");
            return -1;
        }

    }

    if(need_write_bbt_to_nand)
    {
        // fill NAND BBT buffer
        memset(dat_buf, 0xFF, sizeof(dat_buf));
        memset(oob_buf, 0xFF, sizeof(oob_buf));
        fill_nand_bbt_buffer(&init_bbt, dat_buf, oob_buf);

        // write BBT back
        if (!write_bbt_to_flash(dat_buf, oob_buf))
        {
            MSG("save bbt to nand fail! \n");
            return -1;
        }
    }

    return 0;

}

int create_badblock_table_by_bbt(void)
{
    int i = 0, j = 0, k = 0;
    int badblock_table_size = system_block_count - init_bbt.badblock_count;

    for(i = 0; i < badblock_table_size; i++)
    {
        pBbt[i] = i;

    }

    for(i = 0; i < init_bbt.badblock_count; i++)
    {
        for(; j < badblock_table_size; j++)
        {
            if(pBbt[j] == init_bbt.badblock_table[i])
            {
                k = j;
                break;

            }

        }

        for(; k < badblock_table_size; k++)
        {
            pBbt[k]++;
        }

    }

    nand_logic_size = (system_block_count - init_bbt.badblock_count) * (1 << nand_chip_bmt->flash->erase_shift);

    return 0;
}


/*******************************************************************
* [BMT Interface]
*
* Description:
*   Update BMT.
*
* Parameter:
*   offset: update block/page offset.
*   reason: update reason, see update_reason_t for reason.
*   dat/oob: data and oob buffer for write fail.
* 
* Return: 
*   Return true for success, and false for failure.
*******************************************************************/
bool update_bmt(u32 offset, update_reason_t reason, u8 *dat, u8 *oob)
{
    int map_index;
    int orig_bad_block = -1;
    int i;
    int bad_index = offset / BLOCK_SIZE_BMT;

    if (reason == UPDATE_WRITE_FAIL)
    {
        if ( !(map_index = migrate_from_bad(offset, dat, oob)) )
        {
            MSG("migrate fail \n");
            return false;
        }
    }
    else
    {
        if ( !(map_index = find_available_block(false)) )
        {
            MSG("Cannot find block in pool \n");
            return false;
        }
    }

    // now let's update BMT
    if (bad_index >= system_block_count)     // mapped block become bad, find original bad block
    {
        for (i = 0; i < bmt_block_count; i++)
        {
            if (bmt.table[i].mapped_index == bad_index)
            {
                orig_bad_block = bmt.table[i].bad_index;
                break;
            }
        }
        MSG("Mapped block becomes bad, orig bad block is %d \n", orig_bad_block);

        bmt.table[i].mapped_index = map_index;
    }
    else
    {
        bmt.table[bmt.mapped_count].mapped_index = map_index;
        bmt.table[bmt.mapped_count].bad_index = bad_index;
        bmt.mapped_count++;
    }

    memset(dat_buf, 0xFF, sizeof(dat_buf));
    memset(oob_buf, 0xFF, sizeof(oob_buf));
    fill_nand_bmt_buffer(&bmt, dat_buf, oob_buf);
    if (!write_bmt_to_flash(dat_buf, oob_buf))
        return false;

    if (bad_index >= system_block_count)
        mark_block_bad_bmt(offset, BMT_BADBLOCK_GENERATE_LATER);
    else
        mark_block_bad_bmt(offset, BAD_BLOCK_RAW);

    return true;
}

/*******************************************************************
* [BMT Interface]
*
* Description:
*   Given an block index, return mapped index if it's mapped, else 
*   return given index.
*
* Parameter:
*   index: given an block index. This value cannot exceed 
*   system_block_count.
*
* Return NULL for failure
*******************************************************************/
int get_mapping_block_index_by_bmt(int index)
{
    int i;

    if (index >= system_block_count)
    {
        MSG("BMT Given index exceed: %d > %d \n", index, system_block_count);
        return index;
    }

    for (i = 0; i < bmt.mapped_count; i++)
    {
        if (bmt.table[i].bad_index == index)
        {
            MSG("Redirect %d to %d \n", index, bmt.table[i].mapped_index);
            return bmt.table[i].mapped_index;
        }
    }

    return index;
}

int get_mapping_block_index_by_bbt(int index)
{

    if (index >= (system_block_count - init_bbt.badblock_count))
    {
        MSG("BBT Given index exceed: %d > %d \n", index, (system_block_count - init_bbt.badblock_count));
        return index;
    }

    return pBbt[index];

}

int get_mapping_block_index(int index, u16 *phy_block_bbt)
{
    int block;

    block = get_mapping_block_index_by_bbt(index);
    *phy_block_bbt = block;
    block = get_mapping_block_index_by_bmt(block);

    return block;

}

int block_is_in_bmt_region(int index)
{
    if(index >= system_block_count)
        return 1;
    else
        return 0;
}

