# makefile template

include MakeInclude

LDLIBS =  

VLAN_OBJS = vconfig.o

ALL_OBJS = ${VLAN_OBJS}

VCONFIG = vconfig                  #program to be created


#all: ${VCONFIG} macvlan_config
all: ${VCONFIG}


#This is pretty silly..
vconfig.h: Makefile
	touch vconfig.h


$(VCONFIG): $(VLAN_OBJS)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $(VCONFIG) $(VLAN_OBJS) $(LDLIBS)
	$(STRIP) $(VCONFIG)

macvlan_config: macvlan_config.c
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ $<

$(ALL_OBJS): %.o: %.c %.h
	@echo " "
	@echo "Making $<"
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.o

purge: clean
	rm -f *.flc ${VCONFIG} macvlan_config vconfig.h
	rm -f *~





