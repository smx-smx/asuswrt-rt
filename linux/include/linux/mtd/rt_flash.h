#ifndef __MTD_RT_FLASH_H__
#define __MTD_RT_FLASH_H__

#ifndef VPint
#define VPint			*(volatile unsigned long int *)
#endif

#define READ_FLASH_BYTE(i)  	( (VPint(0xbfb0008c) & 0x1) ? \
								((ranand_read_byte != NULL) ? ranand_read_byte((i)) : -1) \
								: (*((unsigned char*)i)) )


/* for read flash, frankliao added 20101216 */
#define READ_FLASH_DWORD(i)  ( (VPint(0xbfb0008c) & 0x1) ? \
								((ranand_read_dword != NULL) ? ranand_read_dword((i)) : -1) \
								: (*((unsigned int*)i)) )

/* frankliao added 20101215 */
extern unsigned long flash_base;
extern unsigned int (*ranand_read_byte)(unsigned long long);
extern unsigned int (*ranand_read_dword)(unsigned long long);

#endif /* __MTD_MTD_RT_FLASH_H__ */
