# Makefile.  Generated from Makefile.in by configure.
#***********************************************************************
#
# Makefile
#
# Makefile for Roaring Penguin's Linux user-space PPPoE client.
#
# Copyright (C) 2000 Roaring Penguin Software Inc.
#
# This program may be distributed according to the terms of the GNU
# General Public License, version 2 or (at your option) any later version.
#
# LIC: GPL
#
# $Id: //BBN_Linux/Branch/Branch_for_MT75xx_ASIC_20130518/tclinux_phoenix/apps/public/rp-pppoe-3.10/src/Makefile#1 $
#***********************************************************************

# Version is set ONLY IN THE MAKEFILE!  Don't delete this!
VERSION=3.10

DEFINES=
prefix=/usr
exec_prefix=${prefix}
mandir=${prefix}/share/man
docdir=/usr/share/doc/rp-pppoe-$(VERSION)
install=/usr/bin/install -c
install_dir=/usr/bin/install -c -d
ifneq ($(TCSUPPORT_CT),)
sbindir=${exec_prefix}/bin
else
sbindir=${exec_prefix}/sbin

endif
# Plugin for pppd on Linux
LINUX_KERNELMODE_PLUGIN=
PPPD_INCDIR=/usr/include

# Licensed version
LIC_INCDIR=
LIC_LIBDIR=
LIC_LIB=
LIC_DEFINE=

# PPPoE relay -- currently only supported on Linux
PPPOE_RELAY=pppoe-relay

# Program paths
PPPOE_PATH=$(sbindir)/pppoe
ifneq ($(TCSUPPORT_CT),)
PPPD_PATH=/usr/bin/pppd
else
PPPD_PATH=/usr/sbin/pppd
endif

# Include ServPoET version if we're building for ServPoET


# Kernel-mode plugin gets installed here.
PLUGIN_DIR=/etc/ppp/plugins
PLUGIN_PATH=$(PLUGIN_DIR)/rp-pppoe.so

# Configuration file paths
PPPOESERVER_PPPD_OPTIONS=/etc/ppp/pppoe-server-options

PATHS='-DPPPOE_PATH="$(PPPOE_PATH)"' '-DPPPD_PATH="$(PPPD_PATH)"' \
	'-DPLUGIN_PATH="$(PLUGIN_PATH)"' \
	'-DPPPOE_SERVER_OPTIONS="$(PPPOESERVER_PPPD_OPTIONS)"'

CFLAGS= -g -Wall -Wstrict-prototypes $(LIC_INCDIR) $(DEFINES) $(LIC_DEFINE) $(PATHS) -Ilibevent -I../../../../linux-2.6.36/include/
ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os
else
CFLAGS += -O2
endif
ifneq ($(TCSUPPORT_CT),)
TARGETS=pppoe-server pppoe pppoe-sniff pppoe-relay
else
TARGETS=pppoe-relay
endif
PPPOE_SERVER_LIBS=$(LIC_LIBDIR) $(LIC_LIB)

ifneq ($(TC_CFLAGS),)
CFLAGS += $(TC_CFLAGS)
endif

all: $(TARGETS)
	@echo ""
ifneq ($(TCSUPPORT_CT),)
pppoe-sniff: pppoe-sniff.o if.o common.o debug.o
	$(CC) -o pppoe-sniff pppoe-sniff.o if.o common.o debug.o -L../../lib/ -ltcapi

pppoe-server: pppoe-server.o if.o debug.o common.o md5.o libevent/libevent.a 
	$(CC) -o pppoe-server  pppoe-server.o if.o debug.o common.o md5.o $(PPPOE_SERVER_LIBS) -Llibevent -levent -L../../lib/ -ltcapi

pppoe: pppoe.o if.o debug.o common.o ppp.o discovery.o
	$(CC) -o pppoe pppoe.o if.o debug.o common.o ppp.o discovery.o -L../../lib/ -ltcapi

pppoe-relay: relay.o if.o debug.o common.o
	$(CC) -o pppoe-relay relay.o if.o debug.o common.o -L../../lib/ -ltcapi
else
pppoe-sniff: pppoe-sniff.o if.o common.o debug.o
	$(CC) -o pppoe-sniff pppoe-sniff.o if.o common.o debug.o

pppoe-server: pppoe-server.o if.o debug.o common.o md5.o libevent/libevent.a 
	$(CC) -o pppoe-server  pppoe-server.o if.o debug.o common.o md5.o $(PPPOE_SERVER_LIBS) -Llibevent -levent

pppoe: pppoe.o if.o debug.o common.o ppp.o discovery.o
	$(CC) -o pppoe pppoe.o if.o debug.o common.o ppp.o discovery.o

pppoe-relay: relay.o if.o debug.o common.o
	$(CC) -o pppoe-relay relay.o if.o debug.o common.o

endif
pppoe.o: pppoe.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o pppoe.o pppoe.c

discovery.o: discovery.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o discovery.o discovery.c

ppp.o: ppp.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o ppp.o ppp.c

md5.o: md5.c md5.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o md5.o md5.c

pppoe-server.o: pppoe-server.c pppoe.h 
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o pppoe-server.o pppoe-server.c

pppoe-sniff.o: pppoe-sniff.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o pppoe-sniff.o pppoe-sniff.c

if.o: if.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o if.o if.c

libevent/libevent.a:
	cd libevent && $(MAKE) DEFINES="$(DEFINES)"

common.o: common.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o common.o common.c

debug.o: debug.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o debug.o debug.c

relay.o: relay.c relay.h pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o relay.o relay.c

# Linux-specific plugin
rp-pppoe.so: plugin/libplugin.a plugin/plugin.o
	$(CC) -o rp-pppoe.so -shared plugin/plugin.o plugin/libplugin.a

plugin/plugin.o: plugin.c
	$(CC) '-DRP_VERSION="$(VERSION)"' $(CFLAGS) -I$(PPPD_INCDIR) -c -o plugin/plugin.o -fPIC plugin.c

plugin/libplugin.a: plugin/discovery.o plugin/if.o plugin/common.o plugin/debug.o
	ar -rc $@ $^

plugin/discovery.o: discovery.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o plugin/discovery.o -fPIC discovery.c

plugin/if.o: if.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o plugin/if.o -fPIC if.c

plugin/debug.o: debug.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o plugin/debug.o -fPIC debug.c

plugin/common.o: common.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o plugin/common.o -fPIC common.c

clean:
ifneq ($(TCSUPPORT_CT),)
	rm -f *.o pppoe-relay pppoe-server pppoe
else
	rm -f *.o pppoe-relay
endif
	
distclean: clean
	rm -f Makefile config.h config.cache config.log config.status
	rm -f libevent/Makefile
	rm -f ../scripts/pppoe-connect ../scripts/pppoe-start ../scripts/pppoe-stop ../scripts/pppoe-init ../scripts/pppoe-setup ../scripts/pppoe-init-suse ../scripts/pppoe-init-turbolinux

# Convenience target for David!  Don't try to use this one.
km:
	./configure --enable-plugin=/home/dfs/Archive/PPP/ppp-2.4.0.pppoe4-patched-dfs

licensed-only:
	$(MAKE) -C licensed-only all VERSION=$(VERSION) SERVPOET_VERSION=$(SERVPOET_VERSION)

l2tp: libevent/libevent.a
	$(MAKE) -C l2tp all

.PHONY: update-version

.PHONY: clean

.PHONY: distclean

.PHONY: rpms

.PHONY: licensed-only

.PHONY: distro

.PHONY: l2tp
