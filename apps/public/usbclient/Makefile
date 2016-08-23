#***********************************************************************
#
# Makefile for mipsel-big asuswrt
#
#***********************************************************************

CC = mips-linux-uclibc-gcc

CFLAGS = -g 
CFLAGS += -I. -I$(APP_SHARED_DIR) -I$(APP_TCAPILIB_DIR) -D_FILE_OFFSET_BITS=64 -DNVRAM_ -DUSE_TCAPI 

LDFLAGS = -L$(APP_TCAPILIB_DIR)  -L$(APP_SHARED_DIR)

LIBS =  -lpthread -ltcapi -lshared -lgcc_s


objects = main.o list.o usb.o usb_api.o

all : usbclient

usbclient : $(objects)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $^

.PHONY : clean
clean:
	rm -rf *.o usbclient

