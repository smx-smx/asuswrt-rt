#***********************************************************************
#
# Makefile for mipsel-big asuswrt
#
#***********************************************************************

CC = mips-linux-uclibc-gcc

CFLAGS = -g -DHAVE_LIBSMBCLIENT_H -I$(APP_SAMBA_FOR_WEBDAV_DIR)/source3/include -I$(APP_LIBXML2_DIR)/include
CFLAGS += -I. -I$(APP_SHARED_DIR) -I$(APP_TCAPILIB_DIR) -D_FILE_OFFSET_BITS=64 -DNVRAM_ -DUSE_TCAPI 

LDFLAGS = -L$(APP_SAMBA_FOR_WEBDAV_DIR)/source3/bin -L$(APP_TCAPILIB_DIR) -L$(APP_LIBXML2_DIR)/.libs -L$(APP_SHARED_DIR)

LIBS = -lsmbclient -lwbclient -lpthread -ltcapi -lxml2 -lshared


objects = main.o smb_api.o list.o usb.o

all : sambaclient

sambaclient : $(objects)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $^

.PHONY : clean
clean:
	rm -rf *.o sambaclient

