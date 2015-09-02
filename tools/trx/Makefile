CC=gcc
CFLAGS += -DTRENDCHIP

ifneq ($(TC_CFLAGS), )
	CFLAGS+=$(TC_CFLAGS)
endif

ifneq ($(US_PROFILE),)
CFLAGS += -D$(US_PROFILE)
endif

ifeq ($(NAND),1)
CFLAGS += -DTCSUPPORT_BB_NAND
endif

OBJS=trx.o
EXECS=trx

ifeq ($(BOOTROM_LARGE_SIZE), 1)
CFLAGS += -DBOOTROM_LARGE_SIZE
endif

all: trx

.c.o:
	$(CC) -c $(CFLAGS) $<

trx: $(OBJS) $(MIPSLIBS)
	$(CC) $(OBJS) -o $@

clean:
	rm -f *.o *.a $(EXECS)

.PHONY : release
release:
	rm -rf *.c
	mv Makefile.release Makefile
