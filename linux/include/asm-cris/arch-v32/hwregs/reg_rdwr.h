/* $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/linux/include/asm-cris/arch-v32/hwregs/reg_rdwr.h#1 $
 *
 * Read/write register macros used by *_defs.h
 */

#ifndef reg_rdwr_h
#define reg_rdwr_h


#define REG_READ(type, addr) *((volatile type *) (addr))

#define REG_WRITE(type, addr, val) \
   do { *((volatile type *) (addr)) = (val); } while(0)

#endif
