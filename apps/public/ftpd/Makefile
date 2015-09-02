
CROSS=mips-linux-
CC=$(CROSS)gcc
AR=$(CROSS)ar
LD=$(CROSS)ld
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size

#HAVEMMU =1
#CC=mips-linux-gcc
#CFLAGS=-D__uClinux__
CFLAGS= -mips1 -msoft-float

ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os
endif

KERNEL_DIR=../../linux
INCLUDE = -I$(KERNEL_DIR)/include
ifndef HAVEMMU
#LDFLAGS=-Wl,-elf2flt="-z -s 16384"
endif
LDLIBS=-lcrypt
#LIBTCAPI= -lm -L /usr/local/lib -ltcapi
LIBTCAPI=-lm -L ../lib -ltcapi
EXEC = ftpd
OBJS = auth.o conf.o ftpcmd.o ftpd.o popen.o server_mode.o localhost.o xgetcwd.o logwtmp.o xmalloc.o

CFLAGS += -DHAVE_CONFIG_H=1 -I.
CFLAGS += -DPATH_FTPWELCOME=\"/etc/ftpwelcome\"
CFLAGS += -DPATH_FTPLOGINMESG=\"/etc/motd\"
CFLAGS += -DPATH_FTPUSERS=\"/etc/ftpusers\"
CFLAGS += -DPATH_BSHELL=\"/bin/sh\"
CFLAGS += -DPATH_FTPCHROOT=\"/usr/sbin/chroot\"
CFLAGS += -DPATH_FTPDPID=\"/var/run/ftpd.pid\"
CFLAGS += -DPATH_DEVNULL=\"/dev/null\"

ifndef CONFIG_UCLINUX
CFLAGS += -DPATH_NOLOGIN=\"/etc/nologin\"
CFLAGS += -DPATH_WTMP=\"/etc/wtmp\"
endif

CFLAGS += -DKEEP_OPEN=1 -DTRENDCHIP
CFLAGS += $(INCLUDE)
EXTRALIBS = $(LIBCRYPT)


all: tcapi_lib $(EXEC)
	
tcapi_lib:
	$(MAKE) -C ../tcapi_lib clean
	$(MAKE) -C ../tcapi_lib
	$(MAKE) -C ../tcapi_lib  install

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $(OBJS) $(EXTRALIBS) $(LDLIBS) $(LIBTCAPI)
	mips-linux-strip $(EXEC)

romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.elf2flt *.gdb *.o

