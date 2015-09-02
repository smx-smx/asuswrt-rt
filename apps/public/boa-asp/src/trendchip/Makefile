#CROSS=mips-linux-gnu-
#CC	= $(CROSS)gcc
CFLAGS 	= -g -O2 -Wall -I. -I.. -I../asp
#CFLAGS += -mips1 -G0 -msoft-float -O2 -fshort-wchar
CFLAGS += -G0 -O2 -fshort-wchar
CFLAGS += -G0 -O2 -fshort-wchar

all:	CLASSLIB

TRENDCHIP_OBJS = \
	trendchip-def.o \
	trendchip_main.o \
	home_wan.o \
	access_snmp.o \
	tools_admin.o \
	status_statistics.o \
	status_log.o \
	home_lan.o \
	tools_system.o \
	adv_adsl.o \
	nat.o \
	route.o \
	access_acl.o\
	access_ipfilter.o\
	access_upnp.o\
	access_ddns.o\
	tools_test.o\
	tools_time.o\
	device_status.o\
	adv_vlan_pvid.o\
	adv_vlan_group.o\
	adv_vlan_top.o\
	adv_qos.o\
	netmask.o\
	adv_firewall.o\
	home_wireless.o\


CLASSLIB : $(TRENDCHIP_OBJS)	

.c.o:
	$(CC) $(CFLAGS) -c $<
	
clean:
	rm $(TRENDCHIP_OBJS) core -f		
