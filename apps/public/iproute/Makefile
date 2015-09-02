DESTDIR=/usr/
SBINDIR=/sbin
CONFDIR=/etc/iproute2
DOCDIR=/share/doc/iproute2
MANDIR=/share/man

# Path to db_185.h include
DBM_INCLUDE:=/usr/include

DEFINES= -DRESOLVE_HOSTNAMES -DDESTDIR=\"$(DESTDIR)\"

#options if you have a bind>=4.9.4 libresolv (or, maybe, glibc)
LDLIBS=-lresolv
ADDLIB=

#options for decnet
ADDLIB+=dnet_ntop.o dnet_pton.o

#options for ipx
ADDLIB+=ipx_ntop.o ipx_pton.o

#CC = mips-linux-gcc
HOSTCC = gcc

ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CCOPTS = -D_GNU_SOURCE -Os -Wstrict-prototypes -Wall
else
CCOPTS = -D_GNU_SOURCE -O2 -Wstrict-prototypes -Wall
endif

CFLAGS = $(CCOPTS) -I../include $(DEFINES)

CFLAGS+=$(TC_CFLAGS)
YACCFLAGS = -d -t -v

LDLIBS += -L../lib -lnetlink -lutil

SUBDIRS=lib ip

LIBNETLINK=../lib/libnetlink.a ../lib/libutil.a

all: Config
	@set -e; \
	for i in $(SUBDIRS); \
	do $(MAKE) $(MFLAGS) -C $$i; done

Config:
	sh configure $(KERNEL_INCLUDE)


snapshot:
	echo "static const char SNAPSHOT[] = \""`date +%y%m%d`"\";" \
		> include/SNAPSHOT.h

clean:
	rm -f cscope.*
	@for i in $(SUBDIRS) doc; \
	do $(MAKE) $(MFLAGS) -C $$i clean; done

clobber: clean
	rm -f Config

distclean: clobber

cscope:
	cscope -b -q -R -Iinclude -sip -slib -smisc -snetem -stc

.EXPORT_ALL_VARIABLES:
