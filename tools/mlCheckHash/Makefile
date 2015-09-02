
CC=gcc
CFLAGS += -DTRENDCHIP
OBJS=mlCheckHash.o
EXECS=mlCheckHash
all: mlCheckHash

.c.o:
	$(CC) -c $(CFLAGS) $<

$(EXECS): $(OBJS) $(MIPSLIBS)
	$(CC) $(OBJS) -o $@

clean:
	rm -f *.o *.a $(EXECS)

.PHONY : release
release:
	rm -rf *.c
	mv Makefile.release Makefile
