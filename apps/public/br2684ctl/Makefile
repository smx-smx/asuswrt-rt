MIPS=true
DEBUG=false
UCLIB=false

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

#CROSS=mips-linux-
#CC=$(CROSS)gcc
AR=$(CROSS)ar
LD=$(CROSS)ld
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size
LIBDIR=$(CROSS_PATH)/lib

EXECS = br2684ctl

INCLUDE = -I$(KERNEL_DIR)/include -I../linux-atm/lib

CFLAGS = -Wall -DWEBS -DUEMF -DOS="LINUX" -DLINUX
CFLAGS+= -mips1 -msoft-float -fomit-frame-pointer 

ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os
else
CFLAGS += -O2
endif

BR2684OBJS=br2684ctl.o
BR2684LIBS=../linux-atm/lib/libatm.a
all: libatm  br2684ctl

.c.o:
	$(CC) -c $(CFLAGS) $<



libatm:
	$(MAKE) -C ../linux-atm/lib $(MFLAGS) all

install:
	install -d $(TARGET)/usr/sbin
	install -m 755 br2684ctl $(TARGET)/usr/sbin

br2684ctl: $(BR2684OBJS) $(BR2684LIBS)
	$(CC) $(BR2684OBJS) $(BR2684LIBS) -o $@
	$(STRIP) $(EXECS)

br2684ctl.o: br2684ctl.c
	$(CC) $(CFLAGS) $(INCLUDE) -Os -c -o br2684ctl.o br2684ctl.c

clean:
	$(MAKE) -C ../linux-atm/lib clean
	rm -f *.o *.a $(EXECS)
