# $Id: //BBN_Linux/Branch/Branch_for_SDK_Release_MultiChip_20111013/tclinux_phoenix/apps/public/tftp-1.0/Makefile#1 $

#CROSS=mips-linux-
#CC=$(CROSS)gcc
AR=$(CROSS)ar
LD=$(CROSS)ld
STRIP=$(CROSS)strip
RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size
CFLAGS= -mips1 -msoft-float -DTRENDCHIP
ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os
endif
#LIBTCAPI= -lm -L /usr/local/lib -ltcapi
#LIBTCAPI= -lm -L ../lib -ltcapi
LIBTCAPI=-L ../lib -ltcapi
INCLUDE = -I$(KERNEL_DIR)/include
ifneq ($(TC_CFLAGS), )
CFLAGS += $(TC_CFLAGS)
endif
ifeq ($(WITHVOIP),1)
CFLAGS += -DWITHVOIP
endif




all: tcapi_lib tftpd

tcapi_lib:
	$(MAKE) -C $(APP_TCAPILIB_DIR) clean
	$(MAKE) -C $(APP_TCAPILIB_DIR)
	$(MAKE) -C $(APP_TCAPILIB_DIR) install

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(INCLUDE) -c -o $@ $^

tftpd: tftpd.o
	$(CC) $(LIBTCAPI) -o $@ $^
	$(STRIP) $@

clean:
	rm -f *.o tftpd
