LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

TR_OPENSSL_PATH := $(LOCAL_PATH)/../../android-external-openssl/include

TR_INCLUDE_PATH := $(LOCAL_PATH)/../include  $(LOCAL_PATH)/../war/android/include $(LOCAL_PATH)/../interface/xml
#TR_INCLUDE_PATH := $(LOCAL_PATH)/../include  $(LOCAL_PATH)/../war/android/include $(LOCAL_PATH)/../interface/sqlite

LOCAL_MODULE    := tr
LOCAL_CFLAGS    := -DVT100_COMPAT -DUSE_C99 -DCODE_DEBUG -D__ENABLE_SSL__ -DOS_ANDROID -DTR069_WIB -DTR143 -DTR111 -DTR157 -DTR196
LOCAL_C_INCLUDES := $(TR_INCLUDE_PATH) $(TR_OPENSSL_PATH)


LOCAL_SRC_FILES := ao.c  cookie.c gpn.c network.c  sched.c  tcp.c atomic.c \
gpva.c     periodic.c  tr.c \
b64.c           decrypt.c  grm.c     pkey.c      session.c    trconfig.c \
buffer.c        do.c       hex.c     pm.c        si.c         tr_strings.c \
cli.c           echo.c     http.c    pub.c       spa.c        tr_strptime.c \
cm.c            encrypt.c  inform.c  reboot.c    spv.c        udp.c \
connection.c    event.c    log.c     request.c   ssl.c        wib.c \
fr.c       		sendtocli.c method.c  retry.c     task.c       xml.c \
../war/android/error/war_errorcode.c \
../war/android/file/war_file.c \
../war/android/math/war_math.c \
../war/android/socket/war_socket.c \
../war/android/string/war_string.c \
../war/android/time/war_time.c \
../war/android/log/war_log.c \
../interface/xml/tr_lib.c  # change xml to sqlite for sqlite version


include $(BUILD_STATIC_LIBRARY)
