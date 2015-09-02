#CROSS_COMPILE=mips-linux-
#CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar
LD=$(CROSS_COMPILE)ld
STRIP=$(CROSS_COMPILE)strip
RANLIB=$(CROSS_COMPILE)ranlib
SIZE=$(CROSS_COMPILE)size


INCLUDE = -I$(KERNEL_DIR)/include 

#CFLAGS = -mips1 -msoft-float -O2 -fomit-frame-pointer
CFLAGS = -fomit-frame-pointer

ifneq ($(strip $(TCSUPPORT_CODE_SHRINK)),)
CFLAGS += -Os
else
CFLAGS += -O2
endif

EXEC = cpu
OBJS = cpu.o

all: $(EXEC)

.c.o:
	$(CC) -c $(CFLAGS) $<

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
	$(STRIP) -s $@ 

clean:
	-rm -f $(EXEC) *.elf *.o

