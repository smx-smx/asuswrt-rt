#***********************************************************************
#
# Makefile for Mipsel-Big
#
#***********************************************************************

CXX=mips-linux-uclibc-g++
		
CPPFLAGS = -I$(APP_LIBICONV_DIR)/include -I$(APP_CURL_DIR)/include -I$(APP_OPENSSL_DIR)/include -I$(APP_LIBXML2_DIR)/include
CPPFLAGS += -g -D_FILE_OFFSET_BITS=64 -DNVRAM_ -DUSE_TCAPI -I. -I$(APP_SHARED_DIR) -I$(APP_TCAPILIB_DIR)

LDFLAGS = -L$(APP_CURL_DIR)/lib/.libs -L$(APP_LIBXML2_DIR)/.libs -L$(APP_LIBICONV_DIR)/lib/.libs -L$(APP_OPENSSL_DIR) -L$(APP_TCAPILIB_DIR) -L$(APP_SHARED_DIR) -lshared 

LIBS = -lpthread -liconv -lcurl -lcrypto -lssl -lxml2 -ltcapi

objects = ftpclient.o api.o list.o
all: ftpclient

ftpclient: $(objects)
	$(CXX)  $(CPPFLAGS) $(LDFLAGS) $(LIBS) -o $@ $^

.PHONY: clean
clean:
	-rm -rf ftpclient $(objects)
