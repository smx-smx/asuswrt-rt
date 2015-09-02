#HAVEMMU = 1
#CC=$(CROSS)gcc
#CFLAGS=-O2 -Wall -msoft-float
CFLAGS= -Wall

ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os
else
CFLAGS += -O2
endif

#TRENDCHIP
CFLAGS+= -DTRENDCHIP -I$(APP_TCAPILIB_DIR)
#
ifndef HAVEMMU
#LDFLAGS=-Wl,-elf2flt="-z -s 32768"
endif
ifneq ($(TCSUPPORT_CD_NEW_GUI),)
	CFLAGS+= -DTCSUPPORT_CD_NEW_GUI
endif
LIBS = -lm -L ../lib -ltcapi
OBJ = miniserver.o xmlpharse.o soappost.o tools.o

%.o:%.c
	$(CC) $(CFLAGS) -c $<

upnpd: miniserver.o xmlpharse.o soappost.o tools.o
	$(CC) -Wall $(OBJ)  -o upnpd $(LIBS)
	$(CROSS)strip upnpd
clean:
	rm -f *.o upnpd
