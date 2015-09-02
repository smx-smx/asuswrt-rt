MIPS=true
DEBUG=false
UCLIB=false

LDFLAGS= -s --static

#CFLAGS= -fomit-frame-pointer -Wall 
#CFLAGS= -mips1 -msoft-float -fomit-frame-pointer -Wall
CFLAGS= -fomit-frame-pointer -Wall

ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os
endif

LIBS=-lc
UCLIB_ARM_WRAPPER_DIR=../../libs/uClibc/extra/gcc-uClibc

#CROSS=mips-linux-
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
#		CROSS_PATH=/usr/local/mips-uclinux
#		CROSS=$(CROSS_PATH)/bin/mips-uclibc-
		#CROSS=mips-linux-
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
RANLIB=$(CROSS)ranlib
LD=$(CROSS)ld
LIBDIR=/usr/lib
STRIP=$(CROSS)strip
STRIP_FG=--strip-unneeded

ATM_OBJS=text2atm.o atm2text.o atmequal.o sdu2cell.o text2qos.o qos2text.o \
  qosequal.o sap2text.o text2sap.o sapequal.o misc.o common.o diag.o kptr.o text2ip.o timer.o unix.o
ifeq ($(LIB_PATH)/libresolv.a,$(wildcard $(LIB_PATH)/libresolv.a))
ATM_OBJS += ans_l.o
else
ATM_OBJS += ans.o
endif
ATMD_OBJS=common.o diag.o kptr.o text2ip.o timer.o unix.o
PGMS=#test
GENLIBS=libatm.a libatmd.a
SYSHDR=atm.h atmd.h atmsap.h
OPTSYSHDR=stdint.h

do_all:			libatm.a libatmd.a

include ../Rules.make

../lib/libatm.a:	libatm.a

libatm.a:		$(ATM_OBJS)
			$(AR) rcs libatm.a $(ATM_OBJS)
			$(STRIP) $(STRIP_FG) libatm.a

../lib/libatmd.a:	libatmd.a

libatmd.a:		$(ATMD_OBJS)
			$(AR) rcs libatmd.a $(ATMD_OBJS)

ans_l.o:		ans.o
			$(LD) -r -o ans_l.o ans.o -L$(LIB_PATH) -lresolv
clean:
			rm -f *.a *.o
