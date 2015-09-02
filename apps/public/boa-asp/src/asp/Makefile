#CROSS=mips-linux-gnu-
#CC	= $(CROSS)gcc
CFLAGS 	= -g -O2 -Wall -I. -I../ -I../../../libdisk/
CFLAGS+= -DTRENDCHIP 

ifneq ($(TC_CFLAGS), )
        CFLAGS+=$(TC_CFLAGS)
endif

ifneq ($(TCSUPPORT_WEBSERVER_SSL), )
CFLAGS += -I../../../matrixssl-1-8
endif
#LDFLAGS = -mips1 -G0 -msoft-float -O2 -fshort-wchar
LDFLAGS = -mips32r2 -G0 -msoft-float -O2 -fshort-wchar

SRC = grammar.c gb-lex.c asp.c asp_handler.c http-get-utils.c parsers.c
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
