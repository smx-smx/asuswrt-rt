# brcm
#LIBS=-latmd
#LIBDEPS=../lib/libatmd.a
#LIBS=-lfl # lex may want  -ll  here
#INCLUDES=
#include ../Rules.make
MIPS=true
DEBUG=false
UCLIB=true

LDFLAGS= -s --static

CFLAGS= -fomit-frame-pointer -Wall 
LIBS=-lc
UCLIB_ARM_WRAPPER_DIR=../../libs/uClibc/extra/gcc-uClibc

#CC=$(CROSS)gcc
AR=$(CROSS)ar
RANLIB=$(CROSS)ranlib
LD=$(CROSS)ld
LIBDIR=/usr/lib
#LIBS=-lelf -lc //7300 have

ifeq ($(strip $(UCLIB)), true)
	# Using uClibc
	ifeq ($(strip $(ARM)),true)
		CROSS=arm-linux-
		CC=$(UCLIB_ARM_WRAPPER_DIR)/arm-uclibc-gcc
		LD=$(UCLIB_ARM_WRAPPER_DIR)/arm-uclibc-ld
		LIBDIR=/skiff/local/arm-linux/lib
	endif
	###################5100##############################
	ifeq ($(strip $(MIPS)),true)
		CROSS_PATH=/usr/local/mips-uclinux
		CROSS=$(CROSS_PATH)/bin/mips-uclibc-
		#CC=$(CROSS)gcc
		AR=$(CROSS)ar
		LD=$(CROSS)ld
		LIBDIR=$(CROSS_PATH)/lib
		RANLIB=$(CROSS)ranlib
        ###################################################
	else
		CC=$(UCLIB_ARM_WRAPPER_DIR)/i386-uclibc-gcc
		LD=$(UCLIB_ARM_WRAPPER_DIR)/i386-uclibc-ld
		LIBDIR=/usr/lib
	endif
else
	ifeq ($(strip $(ARM)),true)
		CROSS=arm-linux-
		LIBDIR=/skiff/local/arm-linux/lib
	else
		#CROSS=
		LIBDIR=/usr/lib
	endif
endif
ifeq ($(strip $(DEBUG)),true)
	CFLAGS=-g -Wall 
endif


EXECS = rt2684d 
#ifeq ($(strip $(BUILD_RT2684D)),static)
#CFLAGS += -DBUILD_STATIC
#endif
LDFLAGS += -Wl,-allow-shlib-undefined
OBJS=rt2684d.o arp.o io.o itf.o table.o
#BOOTPGMS=atmarp atmarpd
MAN8=atmarp.8 atmarpd.8
SYSHDR=atmarp.h atmarpd.h


all: $(EXECS) 

dynamic: lib-atm-atmd $(EXECS) install

static: lib-atm-atmd rt2684d.a

lib-atm-atmd:
	$(MAKE) -C ../lib $(MFLAGS) all

install:
	install -m 755 rt2684d $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/rt2684d

rt2684d.a: $(OBJS)
	$(AR) rcs rt2684d.a $(OBJS)

$(EXECS): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -L../lib -latmd -latm


#.c.o:
#	$(CC) -c $(CFLAGS) $<

clean:
	$(MAKE) -C ../lib clean
	rm -f *.o *.a $(EXECS)
