#include "ralink_nand.h"

#define VPint *(volatile unsigned int *)

typedef struct reg_check_s{
	char* name; 		/*Register Name*/
	#define	VC_TYPE		(1 << 0)
	#define	RO			(1 << 1)
	#define	WO			(1 << 2)
	#define	RW			(1 << 3)
	#define	NO_DEF		(1 << 4)
	uint8_t type;			/*0:VC_TYPE, 1:Read Only, 2: Write Only, 3: No default value*/
	uint32_t addr;		/*Register location*/
	uint32_t def_value;	/*Default value*/
	uint32_t mask;		/*For read/write test*/
} reg_check_t;

/*_____________________________________________________________________________
**      function name: tsarmRegDefCheck
**      descriptions:
**         SAR registers read/write test.
**         Steps as flows:
**         1. Write the test pattern into SAR register. 
**         2. Read the value of SAR register. 
**         3. Compare the value of SAR register is fit in with test pattern. 
**             
**      parameters:
**           pattern: Test pattern.
**             
**      global:
**            sar_reg
**             
**      return:
**            Success:        0
**            Otherwise:     -1
**	     
**      call:
**           delay1ms
**      
**      revision:
**      1. Here 2008/08/15
**____________________________________________________________________________
*/
reg_check_t nand_reg[]=
{
	/*Register name, Type,        Address,       Default Value,      Mask*/
	{"NFC_CTRL",     RW,   (NFC_BASE + 0x0010),   0x00c82f20,     0x0000fffe},
	{"NFC_CONF",     RW,   (NFC_BASE + 0x0014),   0x00000000, 	  0xfff73ffe},
	{"NFC_CMD1",     RW,   (NFC_BASE + 0x0018),   0x00000050,     0x00ffffff},
	{"NFC_CMD2",     RW,   (NFC_BASE + 0x001c),   0x00000000,     0x00ffffff},
	{"NFC_CMD3",     RW,   (NFC_BASE + 0x0020),   0x00000000,     0x00ffffff},
	{"NFC_ADDR",     RW,   (NFC_BASE + 0x0024),   0x00000000,     0xffffffff},
//	{"NFC_DATA",     RW,   (NFC_BASE + 0x0028),   0x00000000,     0xffffffff},
	{"NFC_ECC",      RO,   (NFC_BASE + 0x002c),   0x00000000,     0x00000000},
	{"NFC_STATUS",   RO,   (NFC_BASE + 0x0030),   0x2f000004,     0x00000000},
	{"NFC_INT_EN",   RW,   (NFC_BASE + 0x0034),   0x00000000,     0x000000ff},
//	{"NFC_INT_ST",   RW,   (NFC_BASE + 0x0038),   0x00000000,     0x000000ff},
	{"NFC_ADDR2",    RW,   (NFC_BASE + 0x003c),   0x00000000,     0x000000ff},
	{"NFC_ECC2",     RO,   (NFC_BASE + 0x0040),   0x00000000,     0x00000000},
	{"NFC_ECC3",     RO,   (NFC_BASE + 0x0044),   0x00000000,     0x00000000},
	{"NFC_ECC4",     RO,   (NFC_BASE + 0x0048),   0x00000000,     0x00000000},
	{"NFC_ECC_ST2",  RO,   (NFC_BASE + 0x004c),   0x00000000,     0x00000000},
	{"NFC_ECC_ST3",  RO,   (NFC_BASE + 0x0050),   0x00000000,     0x00000000},
};


int
nand_reg_rwtest(uint32_t pattern)
{
	int i=0, j=0;
	uint32_t befVal=0;
	uint8_t loop=0;
	uint32_t reg_addr=0x0;
	int retval=0;
	int err=0;
	
	for (i=1; nand_reg[i].name!=NULL; i++){/*Skip Software reset register(TSARM_RAI)*/
		loop = 1;
		for (j=0; j<loop; j++){
			err=0;
			reg_addr= nand_reg[i].addr+(j<<2);
			befVal= (uint32_t)VPint(reg_addr);
		
			if (nand_reg[i].type & RO) {
				VPint(reg_addr)=~(VPint(reg_addr));
				if(VPint(reg_addr) != befVal){
					retval=-1;
					err=1;
				}
			} else if (nand_reg[i].type & WO) {
				VPint(reg_addr) = (pattern & nand_reg[i].mask);
				if(VPint(reg_addr)!=0x00000000){
					retval=-1;
					err=1;
				}
			} else if (nand_reg[i].type & RW){
				VPint(reg_addr) = (pattern & nand_reg[i].mask);
				if (VPint(reg_addr) != (pattern & nand_reg[i].mask)) {
					retval=-1;
					err=1;
				}
			}
			
			if (err==1) {
				printk("Error(j:%d): %s is error, Pattern: 0x%x before:0x%x After:0x%x\n", \
					j, nand_reg[i].name, (pattern & nand_reg[i].mask), befVal, (uint32_t) VPint(reg_addr));	
			}
		}
	}
	return retval;

}/*nand_reg_rwtest*/

/*_____________________________________________________________________________
**      function name: tsarmRegDefCheck
**      descriptions:
**          Read the value of SAR register and to compare the default value is fit in with data 
**         sheet. 
**             
**      parameters:
**           None
**             
**      global:
**            sar_reg
**             
**      return:
**            Success:        0
**            Otherwise:     -1
**	     
**      call:
**           None
**      
**      revision:
**      1. Here 2008/08/15
**____________________________________________________________________________
*/
int
nand_reg_defcheck(void)
{
	int i = 0, j = 0;
	uint32_t reg_addr = 0;
	uint8_t loop = 0;
	int retval = 0;

	for (i=0; nand_reg[i].name!=NULL; i++) {
		reg_addr = nand_reg[i].addr;
		if (nand_reg[i].type & NO_DEF) {
			/*If the register is no default value, we skip this register.*/
			continue;
		} else {
			loop = 1;
		}

		for(j=0; j<loop; j++) {
			if(VPint(reg_addr + (j << 2)) != nand_reg[i].def_value) {
				printk("Error(j:%d):%s is error, Default: 0x%x Real:0x%x\n", j, nand_reg[i].name, nand_reg[i].def_value, (uint32_t) VPint(reg_addr));
				retval=-1;
			}
		}
	}
	return retval;
} /*end nand_reg_defcheck*/


/*_____________________________________________________________________________
**      function name: doTsarmRegCheck
**      descriptions:
**           Verify registers of SAR Modules, It's inclued default value and read/write test. 
**             
**      parameters:
**            argc:            argument counter
**            argv:            argument array strings
**            p:
**             
**      global:
**             
**      return:
**            Success:        0
**            Otherwise:     -1
**	     
**      call:
**      	tsarmReset
**      	doTsarmRegCheck
**      	tsarmRegRWTest
**      
**      revision:
**      1. Here 2008/08/15
**____________________________________________________________________________
*/

int 
doNandRegCheck(uint32_t type, uint32_t pattern)
{
	if(type == REG_DEF_CHK) {
		if(nand_reg_defcheck() == -1) {
			printk("NAND Register default value verification is failure!!\r\n");
		} else{
			printk("NAND Register default value verification is ok!!\r\n");
		}
	} else if(type == REG_RW_CHK) {
		if(nand_reg_rwtest(pattern) == -1) {
			printk("NAND Register  Read/Write verification is failure!!\r\n");
		} else {
			printk("NAND Register Read/Write verification is ok!!\r\n");
		}
	} else {
		printk("Usage:nand reg_check <partten>\n");
	}
	return 0;
}


