/*
 * Copyright(c) 2013, ASUSTeK Inc.
 * All rights reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTeK Inc.;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

#ifndef __UTILS_H
#define __UTILS_H

enum {
	ETHERNET_BYTES_SENT=0,
	ETHERNET_BYTES_RECEIVED,
	ETHERNET_PACKETS_SENT,
	ETHERNET_PACKETS_RECEIVED,
	ETHERNET_ERRORS_SENT,
	ETHERNET_ERRORS_RECEIVED,
	ETHERNET_DISCARD_PACKETS_SENT,
	ETHERNET_DISCARD_PACKETS_RECEIVED
};

void generate_wep_key(int unit, int subunit, char *passphrase);
unsigned long get_statistic_of_net_dev(char *net_dev, int field);
unsigned int num_of_mssid_support(unsigned int unit);
#ifdef RTCONFIG_QTN
unsigned int get_statistic_of_qtn_dev(char *prefix, int field);
#endif

/* For cpuusage */
typedef struct PACKED
{ 
	char name[20];
	unsigned int user;
	unsigned int nice;
	unsigned int system;
	unsigned int idle;
}CPU_OCCUPY; 

void get_cpuoccupy (CPU_OCCUPY *cpust);
int cal_cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n);
int getInstNum(char *name, char *objname);
int icmp_test(char *intf, char *host, unsigned int count, unsigned int timeout, unsigned int datasize, unsigned char tos,
	unsigned int *cntOK, unsigned int *cntFail, unsigned int *timeAvg, unsigned int *timeMin, unsigned int *timeMax, unsigned int needWaitRsp);

#ifdef DSL_ASUSWRT
/* Functions for DSL-ASUSWRT */
int handleRomfile();	/* copy from apps/public/boa-asp/src/request.c */
int decryptRomfile(char *path, unsigned int *length, unsigned int offset);		/* copy from apps/private/cfg_manager/utility.c */
int get_dsl_diag_attr(char *attr_name, char *diag_buf);
#endif

#endif /*__UTILS_H*/
