#
# Makefile	Main Makefile for the net-tools Package
#
# NET-TOOLS	A collection of programs that form the base set of the
#		NET-3 Networking Distribution for the LINUX operating
#		system.
#
# Version:	2001-02-13
#
# Author:	Bernd Eckenfels <net-tools@lina.inka.de>
#		Copyright 1995-1996 Bernd Eckenfels, Germany
#
# URLs:		ftp://ftp.inka.de/pub/comp/Linux/networking/NetTools/ 
#		ftp://ftp.linux.org.uk/pub/linux/Networking/PROGRAMS/NetTools/
#		http://www.inka.de/sites/lina/linux/NetTools/index_en.html
#
# Based on:	Fred N. van Kempen, <waltje@uwalt.nl.mugnet.org>
#		Copyright 1988-1993 MicroWalt Corporation
#
# Modifications:
#		Extensively modified from 01/21/94 onwards by
#		Alan Cox <A.Cox@swansea.ac.uk>
#		Copyright 1993-1994 Swansea University Computer Society
#
# Be careful! 
# This Makefile doesn't describe complete dependencies for all include files.
# If you change include files you might need to do make clean. 
#
#	{1.20}	Bernd Eckenfels:	Even more modifications for the new 
#					package layout
#	{1.21}	Bernd Eckenfels:	Check if config.in is newer than 
#					config.status
#	{1.22}  Bernd Eckenfels:	Include ypdomainname and nisdomainame
#
#	1.3.50-BETA6 private Release
#				
#960125	{1.23}	Bernd Eckenfels:	Peter Tobias' rewrite for 
#					makefile-based installation
#	1.3.50-BETA6a private Release
#
#960201 {1.24}	Bernd Eckenfels:	net-features.h added
#
#960201 1.3.50-BETA6b private Release
#
#960203 1.3.50-BETA6c private Release
#
#960204 1.3.50-BETA6d private Release
#
#960204 {1.25}	Bernd Eckenfels:	DISTRIBUTION added
#
#960205 1.3.50-BETA6e private Release
#
#960206	{1.26}	Bernd Eckenfels:	afrt.o removed (cleaner solution)
#
#960215 1.3.50-BETA6f Release
#
#960216 {1.30}	Bernd Eckenfels:	net-lib support
#960322 {1.31}	Bernd Eckenfels:	moveable netlib, TOPDIR
#960424 {1.32}	Bernd Eckenfels:	included the URLs in the Comment
#
#960514 1.31-alpha release
#
#960518 {1.33}	Bernd Eckenfels:	-I/usr/src/linux/include comment added
#
#	This program is free software; you can redistribute it
#	and/or  modify it under  the terms of  the GNU General
#	Public  License as  published  by  the  Free  Software
#	Foundation;  either  version 2 of the License, or  (at
#	your option) any later version.
#


ifndef PREFIX
PREFIX=/usr/local/
endif

ifndef SBINDIR
SBINDIR=$(PREFIX)/sbin
endif

PROGS	+= mii-tool

# Compiler and Linker Options
# You may need to uncomment and edit these if you are using libc5 and IPv6.
COPTS = -D_GNU_SOURCE -O2 -Wall -g -msoft-float -muclibc      # -I/usr/inet6/include
ifeq ($(origin LOPTS), undefined)
LOPTS = 
endif
RESLIB = # -L/usr/inet6/lib -linet6

# -------- end of user definitions --------

.EXPORT_ALL_VARIABLES:

TOPDIR  := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)

CFLAGS	= $(COPTS) -I. -idirafter ./include/ 
LDFLAGS	= $(LOPTS) -msoft-float -muclibc      

SUBDIRS	= man/ 

ifeq ($(origin CC), undefined)
CC	= gcc
endif
CC	= mips-linux-gnu-gcc
LD	= $(CC)

MDEFINES = COPTS='$(COPTS)' LOPTS='$(LOPTS)' TOPDIR='$(TOPDIR)'

%.o:		%.c $<
		$(CC) $(CFLAGS) -c $<

all:		subdirs $(PROGS)

install:	all
	strip --strip-unneeded mii-tool
	install -m 0755 -d         $(DESTDIR)$(SBINDIR)
	install -m 0755 mii-tool   $(DESTDIR)$(SBINDIR)
	$(MAKE) -C man install

clean: 
		rm -f *.o DEADJOE config.new *~ *.orig lib/*.o
		rm -f $(PROGS)
		@for i in $(SUBDIRS); do (cd $$i && $(MAKE) clean) ; done

subdirs:
		@for i in $(SUBDIRS); do $(MAKE) -C $$i $(MDEFINES) ; done

mii-tool:	mii-tool.o
		$(CC) $(LDFLAGS) -o mii-tool mii-tool.o


# End of Makefile.
