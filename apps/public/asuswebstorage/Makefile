#***********************************************************************
#
# Makefile
#
#***********************************************************************

#C = $(CC)
#TOP = $(TOP)
CFLAGS = -I$(APP_CURL_DIR)/include  -I$(APP_LIBXML2_DIR)/include  -I$(APP_OPENSSL_DIR)/include
CFLAGS += -I. -I$(APP_SHARED_DIR) -I$(APP_TCAPILIB_DIR)
#CFLAGS = -g -rdynamic
CFLAGS += -D_FILE_OFFSET_BITS=64
CFLAGS += -DWORDS_BIGENDIAN
CFLAGS += -DUSE_TCAPI
LDFLAGS = -L$(APP_CURL_DIR)/lib/.libs  -L$(APP_LIBXML2_DIR)/.libs  -L$(APP_OPENSSL_DIR) -L$(APP_TCAPILIB_DIR) -L$(APP_SHARED_DIR) -lshared
LIBS = -lcurl -lxml2 -lssl -lcrypto -lpthread -ltcapi

# ifeq ($(wildcard $(TOP)/asuswebstorage/*.c),)
# all:
	# -cp -f $(TOP)/asuswebstorage/prebuild/asuswebstorage asuswebstorage
# else
all: asuswebstorage
# endif

asuswebstorage: asuswebstorage.o api.o function.o list.o mem_pool.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS)  $^ -o $@ 


.PHONY: clean
clean:
	-rm -rf *.o asuswebstorage
