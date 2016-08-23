CFLAGS = -Os -Wall $(EXTRACFLAGS) -fPIC

ifneq ($(TC_CFLAGS), )
CFLAGS+=$(TC_CFLAGS)
endif

OBJS = wlan.o ttc.o 

all: libutils.so libutils.a

libutils.so: $(OBJS)
	$(CC) -shared $(CFLAGS) -o $@ $^
	$(STRIP) $@

libutils.a: $(OBJS)
	@echo " [tuils] AR $@"
	@rm -f $@
	@$(AR) cruv $@ $^


# install: all
	# @echo " [shared] Installing as $(INSTALLDIR)/usr/lib/libshared.so"
	# @install -d $(INSTALLDIR)/usr/lib
	# @install -m 755 libshared.so $(INSTALLDIR)/usr/lib
	# @$(STRIP) $(INSTALLDIR)/usr/lib/libshared.so


clean:
	rm -f *.o *.so *.a .*.depend

%.o: %.c .%.depend
	@echo " [utils] CC $@ $(CFLAGS)"
	@$(CC) $(CFLAGS) -o $@ -c $<

.depend: $(OBJS:%.o=%.c)
	@$(CC) $(CFLAGS) -M $^ > .depend

.%.depend: %.c
	@$(CC) $(CFLAGS) -M $< > $@

-include $(OBJS:%.o=.%.depend)

