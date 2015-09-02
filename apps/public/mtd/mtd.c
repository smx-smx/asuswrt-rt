/*
 * mtd - simple memory technology device manipulation tool
 *
 * Copyright (c) 2006, 2007 Thorsten Glaser <tg@freewrt.org>
 * Copyright (C) 2005 Waldemar Brodkorb <wbx@freewrt.org>,
 *	                  Felix Fietkau <nbd@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * The code is based on the linux-mtd examples.
 */

#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <error.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/reboot.h>
#include <sys/syscall.h>
#include <string.h>
#include <linux/reboot.h>
#include <linux/types.h>

#include <linux/version.h>
#include "../../../tools/trx/trx.h"
#ifdef TCSUPPORT_MTD_ENCHANCEMENT
#include "tc_partition.h"
#endif

#if KERNEL_VERSION(2,6,0) <= LINUX_VERSION_CODE
#define IS_KERNEL26 1
#else
#define IS_KERNEL26 0
#endif

#if IS_KERNEL26
#include <mtd/mtd-user.h>
#else
#include <linux/mtd/mtd.h>
#endif

//#define TRX_MAGIC       0x30524448      /* "HDR0" */
//#define TRX_MAGIC1       0x31524448      /* "HDR1" */
#define BUFSIZE (16 * 1024)
#define MAX_ARGS 8

#define DEBUG

#define SYSTYPE_UNKNOWN     0
#define SYSTYPE_BROADCOM    1
#if 0
/* to be continued */

struct trx_header {
	uint32_t magic;		/* "HDR0" */
	uint32_t len;		/* Length of file including header */
	uint32_t crc32;		/* 32-bit CRC from flag_version to end of file */
	uint32_t flag_version;	/* 0:15 flags, 16:31 version */
	uint32_t offsets[3];    /* Offsets of partitions from start of header */
};
#endif
#ifndef TRENDCHIP
int image_check_bcom(int, const char *);
#else
int image_check_tc(int, const char *);
#endif
int image_check(int, const char *);
int mtd_check(char *);
int mtd_unlock(const char *);
int mtd_open(const char *, int);
int mtd_erase(const char *);
int mtd_write(int, const char *, int, int);
void usage(void) __attribute__((noreturn));

typedef char char_aligned_32bit __attribute__((aligned (4), may_alias));

__attribute__((aligned (4), may_alias)) buf[BUFSIZE];
int buflen;
#ifdef TRENDCHIP
int n_bytes = 0;
#endif
#ifdef TCSUPPORT_MTD_ENCHANCEMENT
int writeflash(int imagefd, const char *device,unsigned long tcoffset, unsigned long tclen, int quiet);
int readflash(int imagefd, const char *device,unsigned long tcoffset,unsigned long tclen);
#endif
static int reboot_flag = 0;

#ifndef TRENDCHIP
int
image_check_bcom(int imagefd, const char *mtd)
{
	struct trx_header *trx = (struct trx_header *) buf;
	struct mtd_info_user mtdInfo;
	int fd;

	buflen = read(imagefd, buf, 32);
	if (buflen < 32) {
		fprintf(stdout, "Could not get image header, file too small (%d bytes)\n", buflen);
		return 0;
	}

	switch(trx->magic) {
		case 0x47343557: /* W54G */
		case 0x53343557: /* W54S */
		case 0x73343557: /* W54s */
		case 0x46343557: /* W54F */
		case 0x55343557: /* W54U */
			/* ignore the first 32 bytes */
			buflen = read(imagefd, buf, sizeof(struct trx_header));
			break;
		case 0x464c457f: /* ELF Netgear WGT634u */
			fprintf(stderr, "found ELF header\n");
			if (lseek(imagefd, 128*1024, SEEK_SET) == -1) {
				if (errno != ESPIPE)
					err(1, "lseek");
				/*
				 * the lseek failed because stdin is a pipe,
				 * so fake it; we know we've already read
				 * 32 bytes in the above call, so just read
				 * in (128 * 1024 - 32) bytes now, modulo
				 * sizeof (buf). why is this 128 KiB btw,
				 * wouldn't it be better to parse the ELF
				 * header?
				 */
				buflen = 128 * 1024 - 32;
				while (buflen) {
					ssize_t n;

					n = read(imagefd, buf,
					    MIN((size_t)buflen, sizeof (buf)));
					if (n < 0)
						err(1, "read");
					buflen -= n;
				}
			}
			buflen = read(imagefd, buf, sizeof(struct trx_header));
			break;
	}

	if (trx->magic != TRX_MAGIC || trx->len < sizeof(struct trx_header)) {
		fprintf(stderr, "Bad trx header\n");
		fprintf(stderr, "If this is a firmware in bin format, like some of the\n"
				"original firmware files are, use following command to convert to trx:\n"
				"dd if=firmware.bin of=firmware.trx bs=32 skip=1\n");
		return 0;
	}

	/* check if image fits to mtd device */
	fd = mtd_open(mtd, O_RDWR | O_SYNC);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		exit(1);
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		exit(1);
	}

	if(mtdInfo.size < trx->len) {
		fprintf(stderr, "Image too big for partition: %s\n", mtd);
		close(fd);
		return 0;
	}

	close(fd);
	return 1;
}
#endif

int
image_check_tc(int imagefd, const char *mtd)
{
	struct trx_header *trx = (struct trx_header *) buf;
	struct mtd_info_user mtdInfo;
	int fd;

	if(strncmp(mtd,"romfile",7) == 0)
	{
		return 1;
	}
	//buflen = read(imagefd, buf, 32);
	buflen = read(imagefd, buf, sizeof(struct trx_header));
	if (buflen < sizeof(struct trx_header)) {
		fprintf(stdout, "Could not get image header, file too small (%d bytes)\n", buflen);
		return 0;
	}

	if(strcmp(mtd, "kernel") == 0)
	{
	if (trx->magic != TRX_MAGIC || trx->len < sizeof(struct trx_header)) {
		fprintf(stderr, "Bad trx header\n");
		fprintf(stderr, "If this is a firmware in bin format, like some of the\n"
				"original firmware files are, use following command to convert to trx:\n"
				"dd if=firmware.bin of=firmware.trx bs=32 skip=1\n");
		return 0;
	}
	}else
	{
		if (trx->magic != TRX_MAGIC1 || trx->len < sizeof(struct trx_header)) {
			fprintf(stderr, "Bad trx header\n");
			fprintf(stderr, "If this is a firmware in bin format, like some of the\n"
					"original firmware files are, use following command to convert to trx:\n"
					"dd if=firmware.bin of=firmware.trx bs=32 skip=1\n");
			return 0;
		}
	}

	/* check if image fits to mtd device */
	fd = mtd_open(mtd, O_RDWR | O_SYNC);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		exit(1);
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		exit(1);
	}

	if(mtdInfo.size < trx->len) {
		fprintf(stderr, "Image too big for partition: %s\n", mtd);
		close(fd);
		return 0;
	}

	close(fd);
	return 1;
}

int
image_check(int imagefd, const char *mtd)
{
	#ifndef TRENDCHIP
	int systype;
	char *c;
	FILE *f;

	systype = SYSTYPE_UNKNOWN;
	f = fopen("/proc/cpuinfo", "r");
	while (!feof(f) && (fgets((char *)buf, BUFSIZE - 1, f) != NULL)) {
		if ((memcmp(buf, "system type", 11) == 0) &&
		    (c = strchr((char *)buf, ':'))) {
			c += 2;
			if (strncmp(c, "Broadcom BCM947XX", 17) == 0)
				systype = SYSTYPE_BROADCOM;
		}
	}
	fclose(f);

	switch(systype) {
		case SYSTYPE_BROADCOM:
			return image_check_bcom(imagefd, mtd);
		default:
			return 1;
	}
	#else
	return image_check_tc(imagefd, mtd);
	#endif
}

int
mtd_check(char *mtd)
{
	struct mtd_info_user mtdInfo;
	int fd;

	fd = mtd_open(mtd, O_RDWR | O_SYNC);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		return 0;
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		close(fd);
		return 0;
	}

	close(fd);
	return 1;
}

int
mtd_unlock(const char *mtd)
{
	int fd;
	struct mtd_info_user mtdInfo;
	struct erase_info_user mtdLockInfo;

	fd = mtd_open(mtd, O_RDWR | O_SYNC);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		exit(1);
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		close(fd);
		exit(1);
	}

	mtdLockInfo.start = 0;
	mtdLockInfo.length = mtdInfo.size;
	if(ioctl(fd, MEMUNLOCK, &mtdLockInfo)) {
		close(fd);
		return 0;
	}

	close(fd);
	return 0;
}

int
mtd_open(const char *mtd, int flags)
{
	FILE *fp;
	char dev[PATH_MAX];
	int i;

	if ((fp = fopen("/proc/mtd", "r"))) {
		while (fgets(dev, sizeof(dev), fp)) {
			if (sscanf(dev, "mtd%d:", &i) && strstr(dev, mtd)) {
				#ifndef TRENDCHIP
				snprintf(dev, sizeof(dev), "/dev/mtd/%d", i);
				#else
				snprintf(dev, sizeof(dev), "/dev/mtd%d", i);
				#endif
				fclose(fp);
				return open(dev, flags);
			}
		}
		fclose(fp);
	}

	return open(mtd, flags);
}

int
mtd_erase(const char *mtd)
{
	int fd;
	struct mtd_info_user mtdInfo;
	struct erase_info_user mtdEraseInfo;

	fd = mtd_open(mtd, O_RDWR | O_SYNC);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		exit(1);
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		close(fd);
		exit(1);
	}

	mtdEraseInfo.length = mtdInfo.erasesize;

	for (mtdEraseInfo.start = 0;
		 mtdEraseInfo.start < mtdInfo.size;
		 mtdEraseInfo.start += mtdInfo.erasesize) {

		ioctl(fd, MEMUNLOCK, &mtdEraseInfo);
		if(ioctl(fd, MEMERASE, &mtdEraseInfo)) {
			fprintf(stderr, "Could not erase MTD device: %s\n", mtd);
			close(fd);
			exit(1);
		}
	}

	close(fd);
	return 0;

}

#if defined(TCSUPPORT_START_TRAP) || defined(TCSUPPORT_SYSLOG_ENHANCE)
#define		SIGNAL_PATH		"/var/tmp/signal_reboot"
static int quit_signal(void)
{
	int ret;
	FILE *fp = NULL;
	char buf[8];

	memset(buf, 0, sizeof(buf));
	
	fp = fopen(SIGNAL_PATH, "r");

	if (fp == NULL)
		ret = 0;
	else {
		fgets(buf, 8, fp);
		
		if (buf[0] == '1')
			ret = 1;
		else 
			ret = 0;

		fclose(fp);
		unlink(SIGNAL_PATH);
	}

	return ret;
}
#endif


int
mtd_write(int imagefd, const char *mtd, int quiet, int do_erase)
{
	int fd, result;
	size_t r, w, e;
	struct mtd_info_user mtdInfo;
	struct erase_info_user mtdEraseInfo;
#if defined(TCSUPPORT_START_TRAP) || defined(TCSUPPORT_SYSLOG_ENHANCE)
	int count = 0;
#endif

	fd = mtd_open(mtd, O_RDWR | O_SYNC);
	if(fd < 0) {
		fprintf(stderr, "Could not open mtd device: %s\n", mtd);
		exit(1);
	}

	if(ioctl(fd, MEMGETINFO, &mtdInfo)) {
		fprintf(stderr, "Could not get MTD device info from %s\n", mtd);
		close(fd);
		exit(1);
	}

	r = w = e = 0;
	if (!quiet)
		fprintf(stderr, " [ ]");

	for (;;) {
		#ifndef TRENDCHIP
		/* buffer may contain data already (from trx check) */
		r = buflen;
		r += read(imagefd, buf + buflen, BUFSIZE - buflen);
		#else
		r = 0;
		r +=read(imagefd, buf, BUFSIZE);
		#endif
		w += r;

		/* EOF */
		if (r <= 0) break;

		/* need to erase the next block before writing data to it */
		while (do_erase && w > e) {
			mtdEraseInfo.start = e;
			mtdEraseInfo.length = mtdInfo.erasesize;

			if (!quiet)
				fprintf(stderr, "\b\b\b[e]");
			/* erase the chunk */
			if (ioctl (fd,MEMERASE,&mtdEraseInfo) < 0) {
				fprintf(stderr, "Erasing mtd failed: %s\n", mtd);
				exit(1);
			}
			e += mtdInfo.erasesize;
		}

		if (!quiet)
			fprintf(stderr, "\b\b\b[w]");

		if ((result = write(fd, buf, r)) < (ssize_t)r) {
			if (result < 0) {
				fprintf(stderr, "Error writing image.\n");
				exit(1);
			} else {
				fprintf(stderr, "Insufficient space.\n");
				exit(1);
			}
		}

		buflen = 0;
#ifdef TRENDCHIP
		if( n_bytes != 0  ){
			if( w >= n_bytes )
				break;
		}
#endif
	}
	if (!quiet)
		fprintf(stderr, "\b\b\b\b");

	close(fd);
	
 	if(reboot_flag){
	#if defined(TCSUPPORT_START_TRAP) || defined(TCSUPPORT_SYSLOG_ENHANCE)	
		system("killall -SIGUSR1 cfg_manager");
		/* wait cfg_manager done */
		while (!quit_signal() && count++ < 10)
			sleep(1);
	#endif
	#ifdef TCSUPPORT_SYSLOG_ENHANCE
		system("killall -9 syslogd");
	#endif

		if ((fd=open("/proc/watchdog_reset", O_WRONLY)) == -1) {
			exit(1);
		}

		write(fd,"1\0",1);
		close(fd);
	}
	return 0;
}

#ifdef TCSUPPORT_MTD_ENCHANCEMENT
long getFileSize(char *filename)
{
	struct stat f_stat;
	if(stat(filename,&f_stat) == -1)
	{
		fprintf(stderr, "get file size error!!");
		return -1;
	}

	return (long)f_stat.st_size;
}
int writeflash(int imagefd, const char *device,unsigned long tcoffset, unsigned long tclen,int quiet)
{
	int fd;
	struct mtd_info_user mtdInfo;
	//char tcbuf[256] = {0};way 1 use this 
	unsigned long judge_count = 0,read_count = 0,count = 0;
	unsigned long sector_start = 0,sector_num = 0,sector_size = 0,total_size = 0;
	struct erase_info_user mtdEraseInfo;
	char *sector = NULL;
	
	fd = mtd_open(device, O_RDWR);
	if(fd < 0) 
	{
		fprintf(stderr, "Could not open mtd device: %s\n", device);
		exit(1);
	}

	//get mtd information
	if(ioctl(fd, MEMGETINFO, &mtdInfo)) 
	{
		fprintf(stderr, "Could not get MTD device info from %s\n", device);
		goto writeflasherror;
	}
	
	//fprintf(stderr, "erasesize:0x%x,size:0x%x\n",mtdInfo.erasesize,mtdInfo.size);
	
	//let's do some sanity checks
	if(tcoffset >= mtdInfo.size)
	{
		fprintf(stderr, "writeflash: device \"%s\" is out of reach -- exit(1)\n", device);	
		goto writeflasherror;
	}
	else if(tcoffset + tclen > mtdInfo.size)
	{
		tclen = mtdInfo.size - tcoffset;
		fprintf(stderr, "writeflash: extends beyond the end of device \"%s\" -- size truncated to %#x\n", device,tclen);
	}

	//malloc one block memory area
	sector = malloc(mtdInfo.erasesize);
	if(sector == NULL)
	{
		fprintf(stderr, "writeflash: malloc error!!not enough memory\n");
		goto writeflasherror;
	}

	//real action(one block per cycle)
	while(tclen > 0)
	{
		if(quiet < 2)
			fprintf(stderr, "enter real action -----tclen:0x%x,tcoffset:0x%x\n",tclen,tcoffset);
		sector_start = tcoffset  &  (  ~ (mtdInfo.erasesize - 1) );
		sector_size = mtdInfo.erasesize - (tcoffset - sector_start);
		if(sector_size  > tclen)
			sector_size = tclen;
		if(quiet < 2)
			fprintf(stderr, "writeflash: write %d 'st sector,start from 0x%x,0x%x bytes\n",sector_num,sector_start,sector_size);

		//first ,read the whole sector from flash to memory cache
		lseek(fd,sector_start,SEEK_SET);
		count = read(fd,sector, mtdInfo.erasesize);
		if(count != mtdInfo.erasesize)
		{
			fprintf(stderr, "writeflash: fail to read the %d 'st sector\n",sector_num);
			goto writeflasherror;
		}
#if 0 //way 1
		//then,read data from file and store into memory cache
		read_count = 0;
		count = 0;
		
		lseek(imagefd,judge_count,SEEK_SET);
		while(read_count < sector_size)
		{
			//fprintf(stderr, "\n");
			count = read(imagefd,tcbuf,sizeof(tcbuf));
			if(count <= 0)
				break;
			if(read_count+count > sector_size)
			{
				lseek(imagefd,judge_count+read_count,SEEK_SET);//must use lseek to re-point the read point
				count = read(imagefd,tcbuf,sector_size - read_count);			
				//fprintf(stderr, "\ttcbuf is %s",tcbuf);
				//fprintf(stderr, "\tsector_size - read_count is 0x %x",sector_size - read_count);
				//fprintf(stderr, "\trecount is 0x %x",count);
			}

			memcpy(sector + tcoffset - sector_start +  read_count,tcbuf,count);//store the data to the according area
			read_count += count;
		}
		
		judge_count += write_count;//judge_count used for re-point 
#else	//way 2
		//then,read data from file and store into memory cache
		read_count = 0;	
		lseek(imagefd,judge_count,SEEK_SET);	
		read_count = read(imagefd,sector + tcoffset - sector_start,sector_size);	
		//below check is necessarily
		if(read_count <= 0)
		{
			fprintf(stderr, "read:count <= 0\n");
			break;
		}
		//fprintf(stderr, "\tcount is 0x%x\n",count);
		//judge_count += sector_size;
		judge_count += read_count;
		//fprintf(stderr, "\tjudge_count is 0x%x\n",judge_count);
#endif		
		//and then,erase the sector
		if (!quiet)
			fprintf(stderr, "\b\b\b[e]");
		mtdEraseInfo.start = sector_start;
		mtdEraseInfo.length = mtdInfo.erasesize;
		if (ioctl (fd,MEMERASE,&mtdEraseInfo) < 0) 
		{
			fprintf(stderr, "writeflash:Erasing device failed: %s\n", device);
			goto writeflasherror;
		}

		//at last,write back data from memory cashe to flash
		if (!quiet)
			fprintf(stderr, "\b\b\b[w]");
		lseek(fd,sector_start,SEEK_SET);
		count = write(fd,sector, mtdInfo.erasesize);
		if(count != mtdInfo.erasesize)
		{
			fprintf(stderr, "writeflash: fail to write the %d 'st sector\n",sector_num);
			goto writeflasherror;
		}

		sector_num++;
		tclen -= sector_size;
		tcoffset += sector_size;
		//total_size += sector_size;	
		total_size += read_count;
	}
	if (!quiet)
		fprintf(stderr, "\b\b\b\b");
	if(quiet < 2)
		fprintf(stderr, "writeflash: total write 0x%x bytes\n",total_size);
	
	if(sector)
		free(sector);
	close(fd);
	return 0;

writeflasherror:
	if(sector)
		free(sector);
	close(fd);
	exit(1);		
}

int readflash(int imagefd, const char *device,unsigned long tcoffset,unsigned long tclen)
{
	int fd;
	struct mtd_info_user mtdInfo;
	char tcbuf[256] = {0};
	unsigned long read_conut = 0,write_count = 0,count = 0;

	fd = mtd_open(device, O_RDWR | O_SYNC);
	if(fd < 0) 
	{
		fprintf(stderr, "Could not open mtd device: %s\n", device);
		exit(1);
	}

	//get mtd information
	if(ioctl(fd, MEMGETINFO, &mtdInfo)) 
	{
		fprintf(stderr, "Could not get MTD device info from %s\n", device);
		close(fd);
		exit(1);
	}

	//let's do some sanity checks
	if(tcoffset >= mtdInfo.size)
	{
		tclen = 0;
		fprintf(stderr, "readflash: device \"%s\" is out of reach -- do nothing\n", device);
	}
	else if(tcoffset + tclen > mtdInfo.size)
	{
		tclen = mtdInfo.size - tcoffset;
		fprintf(stderr, "readflash: extends beyond the end of device \"%s\" -- size truncated to %#x\n", device,tclen);
	}

	//seek the read point and read from flash first,and then write to the memeory file
	lseek(fd,tcoffset,SEEK_SET);
	lseek(imagefd,0,SEEK_SET);

	while(read_conut < tclen)
	{
		count = read(fd,tcbuf,sizeof(tcbuf));
		if(count <= 0)
			break;
		read_conut += count;
		count = write(imagefd,tcbuf,count);
		if(count <= 0)
			break;
		write_count +=count;
	}

	//do some sanity checks
	if(read_conut != write_count)
	{
		fprintf(stderr, "readflash:error when readsize is %lu,writesize is %lu", read_conut,write_count);
		close(fd);
		exit(1);
	}
	
	close(fd);
	return 0;
	
}
#endif

void
usage(void)
{
	fprintf(stderr, "Usage: mtd [<options> ...] <command> [<arguments> ...] <device>\n\n"
	"The device is in the format of mtdX (eg: mtd4) or its label.\n"
	"mtd recognises these commands:\n"
	"        unlock                  unlock the device\n"
	"        erase                   erase all data on device\n"
#ifdef TRENDCHIP
	"        write <imagefile>|- <n> <offset> write n bytes from offset of <imagefile> to device\n"
#else
	"        write <imagefile>|-     write <imagefile> (use - for stdin) to device\n"
#endif
#ifdef TCSUPPORT_MTD_ENCHANCEMENT	
"        writeflash <imagefile> <n> <offset> <device> write <imagefile> to  n bytes from offset of <device>\n"
"        readflash  <imagefile> <n> <offset> <device> read n bytes from offset of <device> to <imagefile>\n"
#endif
	"Following options are available:\n"
	"        -q                      quiet mode (once: no [w] on writing,\n"
	"                                           twice: no status messages)\n"
	"        -r                      reboot after successful command\n"
	"        -f                      force write without trx checks\n"
	"        -e <device>             erase <device> before executing the command\n\n"
	"Example: To write linux.trx to mtd1 labeled as linux and reboot afterwards\n"
	"         mtd -r write linux.trx linux\n\n");
	exit(1);
}

int
main(int argc, char **argv)
{
	int ch, i, boot, imagefd = -1, force, quiet, unlocked;
	int clean_cache;
	char *erase[MAX_ARGS], *device;
	const char *imagefile = NULL;
	enum {
		CMD_ERASE,
		CMD_WRITE,
	#ifdef TCSUPPORT_MTD_ENCHANCEMENT	
		CMD_READ,
	#endif
		CMD_UNLOCK
	} cmd;
#ifdef TRENDCHIP
	int offset;
#endif
#ifdef TCSUPPORT_MTD_ENCHANCEMENT
	int operateflag = 0;
	unsigned long tcoffset = 0;
	unsigned long tclen = 0;	
	long tcfilelen = -1;
#endif

	erase[0] = NULL;
	boot = 0;
	force = 0;
	buflen = 0;
	quiet = 0;
	clean_cache = 1;

	while ((ch = getopt(argc, argv, "c:Ffrqe:")) != -1)
		switch (ch) {
			case 'F':
				quiet = 1;
				/* FALLTHROUGH */
			case 'f':
				force = 1;
				break;
			case 'r':
				boot = 1;
				reboot_flag = 1;
				break;
			case 'q':
				quiet++;
				break;
			case 'e':
				i = 0;
				while ((erase[i] != NULL) && ((i + 1) < MAX_ARGS))
					i++;

				erase[i++] = optarg;
				erase[i] = NULL;
				break;
			case 'c':
				clean_cache = atoi(optarg);
				break;

			case '?':
			default:
				usage();
		}
	argc -= optind;
	argv += optind;

	if (argc < 2)
		usage();

	if ((strcmp(argv[0], "unlock") == 0) && (argc == 2)) {
		cmd = CMD_UNLOCK;
		device = argv[1];
	} else if ((strcmp(argv[0], "erase") == 0) && (argc == 2)) {
		cmd = CMD_ERASE;
		device = argv[1];
//#ifdef TRENDCHIP
#if defined (TRENDCHIP) || defined (TCSUPPORT_MTD_ENCHANCEMENT)
	} else if ((strcmp(argv[0], "write") == 0)	
#ifdef TCSUPPORT_MTD_ENCHANCEMENT	
	||(strcmp(argv[0], "writeflash") == 0)
#endif	
	) {	
#ifdef TCSUPPORT_MTD_ENCHANCEMENT	
	if(strcmp(argv[0], "writeflash") == 0)
		operateflag = 1;//use new mtd write function and don't do check
#endif
#else
	} else if ((strcmp(argv[0], "write") == 0) && (argc == 3)) {
#endif
		cmd = CMD_WRITE;

		if (strcmp(argv[1], "-") == 0) {
			imagefile = "<stdin>";
			imagefd = 0;
		} else {
			imagefile = argv[1];
			if ((imagefd = open(argv[1], O_RDONLY)) < 0) {
				fprintf(stderr, "Couldn't open image file: %s!\n", imagefile);
				exit(1);
			}
		}
#ifdef TCSUPPORT_MTD_ENCHANCEMENT
		if(!operateflag)
		{
#endif
#ifdef TRENDCHIP
		if(argc == 3)
			device = argv[2];
		else if(argc == 5){
			n_bytes = atoi(argv[2]);
			offset = atoi(argv[3]);
			device = argv[4];

			fprintf(stderr, "mtd:n_bytes=%d offset=%d\n", n_bytes, offset);

			/* ignore the offset bytes */
			while(1){
				if( offset <= BUFSIZE ){
					read(imagefd, buf, offset);
					break;
				}
				else{
					read(imagefd, buf, BUFSIZE);
					offset -= BUFSIZE;
				}
			}
		}
		else
			usage();
#else
		device = argv[2];
#endif


		/* check trx file before erasing or writing anything */
		if (!force)  if (!image_check(imagefd, device)) {
			if ((quiet < 2) || !force)
				fprintf(stderr, "TRX check failed!\n");
			if (!force)
				exit(1);
		}
		if (!mtd_check(device)) {
			fprintf(stderr, "Can't open device for writing!\n");
			exit(1);
		}
#ifdef TCSUPPORT_MTD_ENCHANCEMENT
		}
		else
		{
			if(argc == 5)
			{
				tcfilelen = getFileSize(argv[1]);
				tclen = atoi(argv[2]);
				tcoffset = atoi(argv[3]);
				device = argv[4];
				if (quiet < 2)
					fprintf(stderr, "mtd[writeflash]:device=%s tclen=%lu tcoffset=%lu tcfilelen =%d\n",device, tclen, tcoffset,tcfilelen);	
				#if 0
				if(tcfilelen < tclen)
				{
					fprintf(stderr, "operate size is over file max size!so truncate size to file max size\n");
					tclen = tcfilelen;
				}
				#endif
			}
			else 
			{
				usage();//in this function ,call exit(1),so here not call exit again
			}
			
			if (!mtd_check(device)) 
			{
				fprintf(stderr, "Can't open device for writing!\n");
				exit(1);
			}
		}
#endif	
	} 
#ifdef TCSUPPORT_MTD_ENCHANCEMENT
	else if((strcmp(argv[0], "readflash") == 0))
	{
		if(argc == 5)
		{
			cmd = CMD_READ;
			imagefile = argv[1];
			if ((imagefd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC)) < 0) 
			{
				fprintf(stderr, "Couldn't open image file: %s!\n", imagefile);
				exit(1);
			}
			tclen = atoi(argv[2]);
			tcoffset = atoi(argv[3]);
			device = argv[4];
			if (quiet < 2)
				fprintf(stderr, "mtd[readflash]:device=%s tclen=%d tcoffset=%d\n",device, tclen, tcoffset);
		}
		else
		{
			usage();//in this function ,call exit(1),so here not call exit again
		}
	}
#endif
	else {
		usage();
	}

	sync();
//#if defined(TCSUPPORT_2_6_36_KERNEL)
#if defined(TCSUPORT_CLEAN_CACHE)
	if (clean_cache == 1)
		system("echo 1 > /proc/sys/vm/drop_caches");
#endif
	
	i = 0;
	unlocked = 0;
	while (erase[i] != NULL 
	#ifdef TCSUPPORT_MTD_ENCHANCEMENT	
		&& !operateflag
	#endif	
		) {
		if (quiet < 2)
			fprintf(stderr, "Unlocking %s ...\n", erase[i]);
		mtd_unlock(erase[i]);
		if (quiet < 2)
			fprintf(stderr, "Erasing %s ...\n", erase[i]);
		mtd_erase(erase[i]);
		if (strcmp(erase[i], device) == 0)
			/* this means that <device> is unlocked and erased */
			unlocked = 1;
		i++;
	}

	if (!unlocked) {
		if (quiet < 2)
			fprintf(stderr, "Unlocking %s ...\n", device);
		mtd_unlock(device);
	}

	switch (cmd) {
		case CMD_UNLOCK:
			break;
		case CMD_ERASE:
			if (unlocked) {
				fprintf(stderr, "Already erased: %s\n", device);
				break;
			}
			if (quiet < 2)
				fprintf(stderr, "Erasing %s ...\n", device);
			mtd_erase(device);
			break;
		case CMD_WRITE:
			if (quiet < 2)
				fprintf(stderr, "Writing from %s to %s ... \n", imagefile, device);
			#ifdef TCSUPPORT_MTD_ENCHANCEMENT
			if(!operateflag){
				if (reboot_flag) {
					system("killall -9 tcwdog");
				}
				mtd_write(imagefd, device, quiet, (unlocked == 0));
			}
			else
				writeflash(imagefd,device ,tcoffset, tclen,quiet);
			#else
				mtd_write(imagefd, device, quiet, (unlocked == 0));
			#endif
			if (quiet < 2)
				fprintf(stderr, "\n");
			break;
		#ifdef TCSUPPORT_MTD_ENCHANCEMENT	
		case CMD_READ:
			if (quiet < 2)
				fprintf(stderr, "Reading from %s to %s ... \n",device ,imagefile);
			readflash(imagefd, device,tcoffset, tclen);
			break;
		default:
			if (quiet < 2)
				fprintf(stderr, "unknown cmd type!\n");
			break;
		#endif
	}

	sync();

	close(imagefd);
	
	if (boot) {
		if (quiet < 2)
			fprintf(stderr, "\nRebooting ... ");
		fflush(stdout);
		fflush(stderr);
		syscall(SYS_reboot,LINUX_REBOOT_MAGIC1,LINUX_REBOOT_MAGIC2,LINUX_REBOOT_CMD_RESTART,NULL);
	}
	return 0;
}
