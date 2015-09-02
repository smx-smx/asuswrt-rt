/******************************************************************************/
/*
 * Copyright (C) 1994-2008 TrendChip Technologies, Corp.
 * All Rights Reserved.
 *
 * TrendChip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of TrendChip Technologies, Corp. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * TrendChip Technologies, Corp.
 */
/******************************************************************************/
#ifndef _COMMON_H
#define _COMMON_H
#define MAX_NODE_NAME 16
/*#define ATTR_SIZE 32*/
#define MAX_VALUE_SIZE	64
#define WAN_NAS	"nas"
#define NODE_ACTIVE	"Active"
#define WAN_PVC "PVC"
#define MAX_SKBMGR_LEN		512

//#define MTD_WRITE_WITH_REBOOT  "/userfs/bin/mtd -r write"
//#define MTD_WRITE_WITH_FORCE "/userfs/bin/mtd -f write"
//#define MTD_WRITE_WITH_FORCE_REBOOT "/userfs/bin/mtd -f -r write"
#define ROOTFS_PATH  "/var/tmp/rootfs.trx"
#define KERNEL_PATH  "/var/tmp/linux.7z.trx"
#define TCLINUX_PATH "/var/tmp/tclinux.bin"
//#define RAS_KERNEL_PATH  "/var/tmp/linux.7z"
//#define RAS_ROOTFS_PATH  "/var/tmp/rootfs"

#define CUSTOMER_POL	"POL"
#define CUSTOMER_WIND	"WIND"

#endif
