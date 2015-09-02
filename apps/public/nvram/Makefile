
CFLAGS  += -I. -Wall -fPIC $(EXTRACFLAGS)
#CFLAGS += -g -DDEBUG
CFLAGS  += -s -O2

LDFLAGS += -L. -L../shared -lshared
CFLAGS += -I../shared

LDFLAGS +=  -L../../private/tcapi_lib -ltcapi 

ifeq ($(STATIC),1)
CFLAGS += -static
endif

all: libnvram.so libnvram.a nvram

libnvram.so: nvram_linux.o nvram_convert.o
	@echo " [nvram] CC $@"
	@$(CC) -shared $(CFLAGS) -o $@ $^

libnvram.a: nvram_linux.o nvram_convert.o
	@echo " [nvram] AR $@"
	@$(AR) cruv $@ $^
	
nvram: nvram.o defaults.o libnvram.so
	@echo " [nvram] CC $@"
ifeq ($(STATIC),1)
	$(CC) $(CFLAGS) -static -o $@ nvram.o defaults.o $(LDFLAGS) -lnvram
else
	@$(CC) $(CFLAGS) -o $@ nvram.o defaults.o $(LDFLAGS) -lnvram
endif

clean:
	rm -f nvram *.o *.a *.so

%.o: %.c .%.depend
	@echo " [nvram] CC $@"
	@$(CC) $(CFLAGS) -o $@ -c $<

.depend: $(OBJS:%.o=%.c)
	@$(CC) $(CFLAGS) -M $^ > .depend

.%.depend: %.c
	@$(CC) $(CFLAGS) -M $< > $@

-include $(OBJS:%.o=.%.depend)

.PHONY : release
release:
	rm -rf *.c $(EXECS)
	mv Makefile.release Makefile
