
CC=gcc
CFLAGS += -DTRENDCHIP
OBJS=Reducer.o
EXECS=Reducer
all: Reducer

.c.o:
	$(CC) -c $(CFLAGS) $<

Reducer: $(OBJS) $(MIPSLIBS)
	$(CC) $(OBJS) -o $@

clean:
	rm -f *.o *.a $(EXECS)

.PHONY : release
release:
	rm -rf *.c
	mv Makefile.release Makefile
