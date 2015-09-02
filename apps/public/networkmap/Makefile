EXEC = networkmap
OBJS = function.o networkmap.o

ARP_EXEC = arpstorm
ARP_OBJS = shutils.o arpstorm.o

ASUSDISC_EXEC = asusdiscovery
ASUSDISC_OBJS = shutils.o packet.o ASUS_Discovery.o ASUS_Discovery_Debug.o

CFLAGS  += -Wall -I. -I../shared -I$(APP_TCAPILIB_DIR)
LDFLAGS += -L../shared -lshared -L$(APP_TCAPILIB_DIR) -ltcapi


all: $(EXEC) $(ARP_EXEC) $(ASUSDISC_EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

$(ARP_EXEC): $(ARP_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

$(ASUSDISC_EXEC): $(ASUSDISC_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

install:
	install -D $(EXEC) $(INSTALLDIR)/usr/sbin/$(EXEC)
	$(STRIP) $(INSTALLDIR)/usr/sbin/$(EXEC)
	install -D $(ARP_EXEC) $(INSTALLDIR)/usr/sbin/$(ARP_EXEC)
	$(STRIP) $(INSTALLDIR)/usr/sbin/$(ARP_EXEC)
	install -D $(ASUSDISC_EXEC) $(INSTALLDIR)/usr/sbin/$(ASUSDISC_EXEC)
	$(STRIP) $(INSTALLDIR)/usr/sbin/$(ASUSDISC_EXEC)
clean: 
	rm -rf *.o $(EXEC) $(ARP_EXEC) $(ASUSDISC_EXEC) *~
