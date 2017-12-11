#***********************************************************************
#
# Makefile
#
#***********************************************************************

#C = $(CC)
#TOP = $(TOP)
CFLAGS = -I$(APP_CURL_DIR)/include -I$(APP_OPENSSL_DIR)/include
CFLAGS += -I. -I$(APP_SHARED_DIR) -I$(APP_TCAPILIB_DIR)
CFLAGS += -D_FILE_OFFSET_BITS=64
CFLAGS += -DWORDS_BIGENDIAN
CFLAGS += -DNVRAM_
CFLAGS += -DUSE_TCAPI
LDFLAGS =  -L$(APP_CURL_DIR)/lib/.libs -L$(APP_OPENSSL_DIR) -L$(APP_TCAPILIB_DIR) -L$(APP_SHARED_DIR) -lshared
LIBS = -lcurl -lssl -lcrypto -lpthread -ltcapi -lm


all:dropbox_client

dropbox_client:dropbox.o function.o list.o cJSON.o mem_pool.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) $^ -o $@
.PHONY:clean
clean:
	rm -rf *.o dropbox_client
