PROGS     = utelnetd

INSTDIR   = $(prefix)/bin/
INSTMODE  = 0755
INSTOWNER = root
INSTGROUP = root

#CROSS_COMPILE=mips-linux-

#CC=$(CROSS_COMPILE)gcc
AR =     $(CROSS_COMPILE)ar
LD =     $(CROSS_COMPILE)ld
NM =     $(CROSS_COMPILE)nm
STRIP=$(CROSS_COMPILE)strip
RANLIB = $(CROSS_COMPILE)ranlib
OBJDUMP=$(CROSS_COMPILE)objdump

#CFLAGS=-mips1 -msoft-float -O2 -fomit-frame-pointer 
CFLAGS= -fomit-frame-pointer 
ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os
else
CFLAGS += -O2
endif
OBJS = utelnetd.o

all: $(PROGS)

CFLAGS   += -DSHELLPATH=\"$(run_prefix)/bin/login\"
CFLAGS   += -DHAVE_DEVPTS_FS

ifneq ("","$(DEBUG)")
CFLAGS   += -DDEBUG
endif

ifneq ($(TC_CFLAGS), )
CFLAGS += $(TC_CFLAGS)
endif
LDFLAGS += -L ../lib -ltcapi

.c.o:
	$(CC) -c $(CFLAGS) $<

$(PROGS): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)
	$(STRIP) -s $@ 

.PHONY: install
install: $(PROGS)

.PHONY: clean
clean:
	rm -f $(PROGS) *.o core
