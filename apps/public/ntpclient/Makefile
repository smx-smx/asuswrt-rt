# Under Solaris, you need to 
#    CFLAGS += -xO2 -Xc
#    LDLIBS += -lnsl -lsocket
# Some versions of Linux may need
#    CFLAGS += -D_GNU_SOURCE
# To cross-compile
#    CC = arm-linux-gcc

CROSS_PATH=
#CROSS=$(CROSS_PATH)mips-linux-
#CC=$(CROSS)gcc
AR=$(CROSS)ar
LD=$(CROSS)ld
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size
ELF2FLT=$(CROSS)elf2flt
LIBDIR=$(CROSS_PATH)/lib
ifeq ($(TCSUPPORT_CT),)
LIBS=-L $(LIB_DIR) -ltcapi
else
LIBS=
endif

CFLAGS = -Wall

ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os
else
CFLAGS += -O2 
endif

CFLAGS += -fomit-frame-pointer -D__UCLINUX__ -D__uClinux__
#CFLAGS +=-mips1 -msoft-float -O2 -fomit-frame-pointer -D__UCLINUX__ -D__uClinux__
NTPCLIENT = ntpclient
ifneq ($(TC_CFLAGS),)
CFLAGS+=$(TC_CFLAGS)
endif

all: ntpclient

test: ntpclient
	./ntpclient -d -r <test.dat

ntpclient: ntpclient.o phaselock.o
	$(CC) $(CFLAGS) -o $@ $(LIBS) ntpclient.o phaselock.o
	$(STRIP) $(NTPCLIENT)
clean:
	rm -f ntpclient *.o
