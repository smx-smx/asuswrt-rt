
CC=gcc
CFLAGS += -DTRENDCHIP
OBJS= cplopts_formcheck.o cplopts_remove.o
EXECS= cplopts_formckeck cplopts_remove
all:  cplopts_formcheck cplopts_remove

.c.o:
	$(CC) -c $(CFLAGS) $<

cc: $(OBJS) $(MIPSLIBS)
	$(CC) -o $@ $(OBJS)

clean:
	rm -f *.o *.a $(EXECS)

.PHONY : release
release:
	rm -rf *.c
	mv Makefile.release Makefile
