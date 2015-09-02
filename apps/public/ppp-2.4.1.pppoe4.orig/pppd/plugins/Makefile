#CC	= mips-linux-gcc
#CFLAGS	= -Os -I.. -I../../include -fPIC
CFLAGS	+= -Os -I$(KERNEL_DIR)/include -I$(APP_LIBATM_DIR)/lib -I.. -I../../include -fPIC -D_DISABLE_SERIAL_
LDFLAGS	= -shared
INSTALL	= install

all:	minconn.so passprompt.so pppoatm.so pppoe/pppoe.so
	$(MAKE) -C pppoe $(MFLAGS) all

pppoe/pppoe.so:
	$(MAKE) -C pppoe $(MFLAGS) pppoe.so

minconn.so: minconn.c
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) minconn.c

pppoatm.so: pppoatm.c
	$(CC) -D_linux_=1 -o $@ $(LDFLAGS) $(CFLAGS) pppoatm.c
	cp pppoatm.so libpppoatm.so

passprompt.so: passprompt.c
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) passprompt.c

LIBDIR	= /home/beaulieu/work/mips/userland/filesystem/tiny/usr/lib/pppd

install: minconn.so passprompt.so pppoe/pppoe.so
	version=`awk -F '"' '/VERSION/ { print $$2; }' ../patchlevel.h`; \
	$(INSTALL) -d $(LIBDIR)/$$version; \
	$(INSTALL) $? $(LIBDIR)/$$version

clean:
	$(MAKE) -C pppoe clean
	rm -rf *.o *.so
