
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

MY_AGENT_PATH := $(LOCAL_PATH)/Agent5.3

LOCAL_MODULE    := tragent-jni
LOCAL_SRC_FILES := com_example_TRAgent53_JNI.c

LOCAL_STATIC_LIBRARIES := tr
LOCAL_LDLIBS := -llog -ldl -L./ -lssl -lcrypto -lsqlite

include $(BUILD_SHARED_LIBRARY)

#build the executable of agent
include $(MY_AGENT_PATH)/source/Android.mk

#########################################
