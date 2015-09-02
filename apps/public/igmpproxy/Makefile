#CROSS=mips-linux-
#CC=$(CROSS)gcc
AR=$(CROSS)ar
LD=$(CROSS)ld
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size

#BINDIR=../bin
BINDIR=/usr/local/bin
ETCDIR=/etc
MANDIR=/usr/share/man


# CFLAGS=-g
CFLAGS= -DTRENDCHIP

ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os
else
CFLAGS += -O2
endif

OBJS=igmpproxy.o config.o confread.o request.o udpsock.o mcgroup.o rttable.o \
			igmp.o ifvc.o callout.o kern.o syslog.o lib.o mroute-api.o

default : build.h igmpproxy

all	: igmpproxy

clean	:
	rm -f *.o *.asm build.h igmpproxy  

install : 
	cp igmpproxy ${BINDIR}
	cp ../doc/igmpproxy.8 ${MANDIR}/man8
	cp ../doc/igmpproxy.conf.5 ${MANDIR}/man5
	if [ ! -e ${ETCDIR}/igmpproxy.conf ]; then cp igmpproxy.conf ${ETCDIR}; fi

igmpproxy : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o igmpproxy
	$(STRIP) --strip-unneeded igmpproxy

build.h	:  
	echo '#define BUILD "' `date +%y%m%d` '"' >build.h


