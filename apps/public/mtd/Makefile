# $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/mtd/Makefile#1 $

#CROSS=mips-linux-gnu-
#CROSS=mips-linux-
#CC=$(CROSS)gcc
AR=$(CROSS)ar
LD=$(CROSS)ld
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size

CFLAGS= -DTRENDCHIP
ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os
endif
#CFLAGS= -mips32r2 -msoft-float -DTRENDCHIP -muclibc
ifneq ($(TC_CFLAGS), )
        CFLAGS+=$(TC_CFLAGS)
endif


INCLUDE = -I$(KERNEL_DIR)/include

all: mtd

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(INCLUDE) -c -o $@ $^

mtd: mtd.o
	$(CC) -o $@ $^
	$(CC) -static -o mtd-static $^
	$(STRIP) $@
	$(STRIP) mtd-static

clean:
	rm -f *.o mtd
