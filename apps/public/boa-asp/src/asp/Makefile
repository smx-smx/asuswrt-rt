#CROSS=mips-linux-gnu-
#CC	= $(CROSS)gcc
CFLAGS 	= -g -O2 -Wall -I. -I../ -I../../../libdisk/
CFLAGS+= -DTRENDCHIP

ifneq ($(TC_CFLAGS), )
CFLAGS+=$(TC_CFLAGS)
endif
CFLAGS += -I$(APP_TCAPILIB_DIR)
CFLAGS += -I$(APP_MSSL_DIR)
CFLAGS += -I$(APP_SHARED_DIR)
CFLAGS += -I$(APP_OPENSSL_DIR)/include
CFLAGS += -I$(LIB_UTILS_DIR)
CFLAGS += -I$(APP_JSONC_DIR)

ifneq ($(TCSUPPORT_WEBSERVER_SSL), )
CFLAGS += -I../../../matrixssl-1-8
endif

ifneq ($(strip $(TCSUPPORT_ACCESSLOG)),)
CFLAGS += -I$(APP_ACCESSLOG_DIR)
endif

CFLAGS += -I$(APP_NETWORKMAP_DIR)

#LDFLAGS = -mips1 -G0 -msoft-float -O2 -fshort-wchar
LDFLAGS = -mips32r2 -G0 -msoft-float -O2 -fshort-wchar

ifneq ($(strip $(TC_LDFLAGS)),)
LDFLAGS += $(TC_LDFLAGS)
endif

SRC = grammar.c gb-lex.c asp.c asp_handler.c http-get-utils.c parsers.c
ifneq ($(RTCONFIG_OPENVPN), )
SRC += openvpn_options.c
endif
OBJS = $(patsubst %.c, %.o, $(SRC))


all: mini-asp

depends: $(SRC)
	$(CC) -MM $(SRC)  > .depends

-include .depends

grammar.c grammar.h:grammar.y
	rm -f grammar.c grammar.h
	bison -d -p gb_ $<
	mv grammar.tab.c grammar.c
	mv grammar.tab.h grammar.h

mini-asp: $(OBJS)
       
.c.o:
	$(CC) $(CFLAGS) -c $< 

clean:
	-rm -f $(OBJS)
	-rm -f grammar.c grammar.h

mrproper: clean
	-rm -f .depends
