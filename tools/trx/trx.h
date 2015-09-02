/*
 * trx.h
 *
 *  Created on: Mar 19, 2009
 *      Author: pork
 */

#ifndef TRX_H_
#define TRX_H_
struct trx_header {
	unsigned int magic;			/* "HDR0" */
	unsigned int header_len;    /*Length of trx header*/
	unsigned int len;			/* Length of file including header */
	unsigned int crc32;			/* 32-bit CRC from flag_version to end of file */
	unsigned char version[32];  /*firmware version number*/
	unsigned char customerversion[32];  /*firmware version number*/
//	unsigned int flag_version;	/* 0:15 flags, 16:31 version */
#if 0
	unsigned int reserved[44];	/* Reserved field of header */
#else
	unsigned int kernel_len;	//kernel length
	unsigned int rootfs_len;	//rootfs length
        unsigned int romfile_len;	//romfile length
	#if 0
	unsigned int reserved[42];  /* Reserved field of header */
	#else
	unsigned char Model[32];
#if defined (TCSUPPORT_2_6_36_KERNEL)
	unsigned int decompAddr;//kernel decompress address
	unsigned int reserved[32];  /* Reserved field of header */
#else
	unsigned int reserved[33];  /* Reserved field of header */
#endif
	#endif
#endif	
};
#define TRX_MAGIC	0x30524448	/* "HDR0" */
#define TRX_MAGIC1	0x31524448	/* "HDR1" */
#define TRX_MAGIC2	0x32524448	/* "for tclinux" */
#define TRX_MAGIC3		0x33524448	/* "for romfile" */
#endif /* TRX_H_ */
