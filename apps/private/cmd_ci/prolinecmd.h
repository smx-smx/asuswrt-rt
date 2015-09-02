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
#ifndef _PROLINECMD_H
#define _PROLINECMD_H

#ifdef TCSUPPORT_PRODUCTIONLINE
#include    "../../public/mtd/tc_partition.h"

#define SUCCESS 0
#define FAIL -1

/*common struct*/
typedef	struct _operPara{
	int 	flashsize;		//cover size of flash size
	int 	flashoffset;		//offset of base address of current zone
	char zonename[16];	//name of zone
	char readfile[64];		//mtd cmd used
	char writefile[64];		//mtd cmd used
}operPara;

#define PL_CP 0	
typedef struct _proLineFlag{
	int type;

	int flag;	//according to types
	struct _operPara para;
} proLineFlag;

/*
PL:product line
CP:cwmp parameter
*/
#define PL_CP_PRODUCTCLASS 0
#define PL_CP_MANUFACUREROUI 1
#define PL_CP_SERIALNUM 2

#define PL_CP_PRODUCTCLASS_FLAG (1<<0)
#define PL_CP_MANUFACUREROUI_FLAG (1<<1)
#define PL_CP_SERIALNUM_FLAG (1<<2)

#define PROLINECMD_SOCK_PATH 	"/tmp/prolinecmd_sock"
#define PROLINECMD_CWMPPARA_FILE  "/tmp/prolinecwmppara"
#define PROLINECMD_CWMPPARAREAD_FILE  "/tmp/prolinecwmppararead"
#define PROLINECMD_CWMPPARAWRITE_FILE  "/tmp/prolinecwmpparwrite"

#define CWMPPARAMAGIC 0x12344321

#define PRDDUCTCLASSLEN 64
#define MANUFACUREROUILEN 64
#define SERIALNUMLEN 128

#if !defined(TCSUPPORT_PRODUCTIONLINE_CONTENT) 
#define PRODUCTCONTENTLEN 0
#endif

#if !defined(TCSUPPORT_PRODUCTIONLINE_DATE) 
#define PROGDATELEN 0
#define MFTDATELEN 0
#endif

#define RESERVELEN (PROLINE_CWMPPARA_RA_SIZE-PRDDUCTCLASSLEN-MANUFACUREROUILEN-SERIALNUMLEN-PROGDATELEN-MFTDATELEN-PRODUCTCONTENTLEN-2*sizeof(int))

typedef struct _proline_cwmpPara{
	int flag;
	char productclass[PRDDUCTCLASSLEN];
	char manufacturerOUI[MANUFACUREROUILEN];
	char serialnum[SERIALNUMLEN];
	char reserve[RESERVELEN];
	int magic;
} proline_cwmpPara;

#endif
#endif
 
