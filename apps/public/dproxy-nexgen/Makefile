# compiler flags
#CC = mips-linux-gcc
#CFLAGS =-Wall -Os -s -mips1 -msoft-float
#M4= m4

CROSS_PATH=
#CROSS=mips-linux-
#CC=$(CROSS)gcc #-mips32r2 -msoft-float -muclibc
AR=$(CROSS)ar
LD=$(CROSS)ld
STRIP=$(CROSS)strip
#RANLIB=$(CROSS)ranlib
SIZE=$(CROSS)size
ELF2FLT=$(CROSS)elf2flt
LIBDIR=$(CROSS_PATH)/lib
AR=mips-linux-ar
#RANLIB=mips-linux-ranlib

CFLAGS = -Wall
#CFLAGS +=-mips1 -msoft-float -O2 -fomit-frame-pointer -D__UCLINUX__ -D__uClinux__
CFLAGS +=-O2 -fomit-frame-pointer -D__UCLINUX__ -D__uClinux__

BIN_DIR=/sbin
CONFIG_DIR=/etc

SLACKWARE_SCRIPT=/etc/rc.d/rc.inet2
##############################################
# uncoment the following if you have REDHAT 
#DIST= -DREDHAT
CACHE_DIR=/var/cache
DHCP_LEASES=/var/state/dhcp.leases
#RC_SCRIPT_DIR=/etc/rc.d/init.d

##############################################
# uncoment the following if you have DEBIAN
#DIST= -DDEBIAN
#CACHE_DIR=/var/cache
# not shure where the dhcp stuff is
#DHCP_LEASES=/var/state/dhcp.leases
#RC_SCRIPT_DIR=/etc/rc.d/init.d

##############################################
# uncoment the following if you have SUSE
#DIST= -DSUSE
#CACHE_DIR=/var/dproxy
# not shure where the dhcp stuff is
#DHCP_LEASES=/var/dhcpd/dhcp.leases
#RC_SCRIPT_DIR=/sbin/init.d

##############################################
# uncoment the following if you have SLACKWARE
#DIST= -DSLACK
#CACHE_DIR=/var/cache
#DHCP_LEASES=/var/state/dhcp.leases
######## END OF CONFIGURABLE OPTIONS #########

CACHE_FILE=/var/dproxy.cache
CONFIG_FILE=/etc/dproxy.conf

DEFAULTS= -DCACHE_FILE_DEFAULT=\"$(CACHE_FILE)\" \
	  -DDHCP_LEASES_DEFAULT=\"$(DHCP_LEASES)\" \
	  -DCONFIG_FILE_DEFAULT=\"$(CONFIG_FILE)\"

RCDEFS= $(DIST) -DBIN_DIR="$(BIN_DIR)" -DCONFIG_DIR="$(CONFIG_DIR)" 

# install stuf
INSTALL=install

OBJS=dproxy.o dns_decode.o dns_list.o cache.o conf.o dns_construct.o dns_io.o 

all: dproxy

dproxy: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
	$(STRIP) dproxy  

%.o : %.c Makefile
	$(CC) -c $(DEFAULTS) $(CFLAGS) $<

clean:
	rm -f *.o *~ core dproxy

install:  
	cp -f dproxy ../../target/usr/sbin/

uninstall:

dproxy.o: dproxy.c dproxy.h dns.h conf.h cache.c cache.h
cache.o: cache.c cache.h dproxy.h dns.h conf.h
conf.o: conf.c conf.h dproxy.h dns.h
dns_decode.o: dns_decode.c dns_decode.h dproxy.h
dns_list.o: dns_list.c dns_list.h dproxy.h
dns_construct.o: dns_construct.c dns_construct.h dproxy.h
