LOCAL_PATH := .

include $(CLEAR_VARS) 

APP_PLATFORM := android-8
LOCAL_MODULE := axcore
LOCAL_CFLAGS := -DANDROID_NDK
LOCAL_C_INCLUDES += ../../../include/
LOCAL_CFLAGS += -include ../../../src/pch/pch.h -Wno-psabi
# "-Wno-psabi" for turn off warnning: note: the mangling of 'va_list' has changed in GCC 4.4
LOCAL_LDLIBS += -llog


LOCAL_SRC_FILES := $(shell find ../../../src/ -name "*.cpp" )
 
include $(BUILD_STATIC_LIBRARY)


#================
include $(CLEAR_VARS)

LOCAL_MODULE    := axcore-dummy
#LOCAL_CFLAGS := -DANDROID_NDK 
LOCAL_SRC_FILES := jni/main.c
LOCAL_STATIC_LIBRARIES := axcore
#LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz

include $(BUILD_EXECUTABLE)

