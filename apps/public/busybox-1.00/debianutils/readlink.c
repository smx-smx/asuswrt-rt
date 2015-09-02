/* vi: set sw=4 ts=4: */
/*
 * Mini readlink implementation for busybox
 *
 * Copyright (C) 2000,2001 Matt Kraai <kraai@alumni.carnegiemellon.edu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "busybox.h"

#define READLINK_OPTIONS "f"

#define READLINK_OPT_F (1 << 0)

int readlink_main(int argc, char **argv)
{
	char *buf = NULL;
	unsigned long opt;

	opt = bb_getopt_ulflags(argc, argv, READLINK_OPTIONS);

	if(opt & READLINK_OPT_F)
	{
		if(argc == 3)
			buf = xrealpath(argv[2]);
		else
			bb_show_usage();
	}
	else
	{	if(argc == 2)
			buf = xreadlink(argv[1]);
		else
			bb_show_usage();
	}

	if (!buf)
		return EXIT_FAILURE;

	puts(buf);
	free(buf);

	return EXIT_SUCCESS;
}
