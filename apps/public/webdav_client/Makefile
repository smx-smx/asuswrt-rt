#***********************************************************************
#
# Makefile
#
#***********************************************************************

#C = $(CC)
#TOP = $(TOP)
CFLAGS = -I$(APP_NEON_DIR)/src -I$(APP_LIBXML2_DIR)/include -I$(APP_OPENSSL_DIR)/include
CFLAGS += -I. -I$(APP_SHARED_DIR) -I$(APP_TCAPILIB_DIR) -D_FILE_OFFSET_BITS=64 -DNVRAM_ -DUSE_TCAPI

LDFLAGS = -L$(APP_NEON_DIR)/src/.libs -L$(APP_LIBXML2_DIR)/.libs -L$(APP_TCAPILIB_DIR) -L$(APP_SHARED_DIR) -L$(APP_ZLIB_DIR) -L$(APP_OPENSSL_DIR)

LIBS = -lpthread -ltcapi -lneon -lxml2 -lshared -lz -lcrypto -lssl

# ifeq ($(wildcard $(TOP)/webdav_client/*.c),)
# all:
	# -cp -f $(TOP)/webdav_client/prebuilt/webdav_client webdav_client
# else
all: webdav_client
# endif

webdav_client: webdav.o base.o list.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS)  $^ -o $@ 

.PHONY: clean
clean:
	-rm -rf *.o webdav_client

