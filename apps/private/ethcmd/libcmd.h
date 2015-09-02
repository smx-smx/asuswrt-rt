/************************************************************************
 *
 *	Copyright (C) 2009 Trendchip Technologies, Corp.
 *	All Rights Reserved.
 *
 * Trendchip Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of Trendchip Technologies, Co. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * Trendchip Technologeis, Co.
 *
 *************************************************************************/
#ifndef _LIBCMD_H
#define _LIBCMD_H

#define MAX_CMD_LEN	256
#define NARG		40 /*Number of ARG*/

#define FALSE		0
#define TRUE		1

typedef	struct {
	char *name;		
	int	(*func)(int argc,char *argv[],void *p);
	int	flags;	
	int	argcmin;
	char *argc_errmsg;	
} cmds_t;

int cmdparse(const cmds_t cmds[], char *line, void *p);
int subcmd(const cmds_t tab[], int argc, char *argv[], void *p);
#endif
