/*
 *   $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/radvd-1.5/privsep-bsd44.c#1 $
 *
 *   Authors:
 *    Jim Paris			<jim@jtan.com>
 *    Pedro Roque		<roque@di.fc.ul.pt>
 *    Lars Fenneberg		<lf@elemental.net>	 
 *
 *   This software is Copyright 1996,1997,2008 by the above mentioned author(s), 
 *   All Rights Reserved.
 *
 *   The license which is distributed with this software in the file COPYRIGHT
 *   applies to this software. If your distribution is missing this file, you
 *   may request it from <pekkas@netcore.fi>.
 *
 */

#include <config.h>
#include <includes.h>
#include <radvd.h>
#include <pathnames.h>

/* Not available */
int
privsep_init(void)
{
	return 0;
}

int
privsep_enabled(void)
{
	return 0;
}
