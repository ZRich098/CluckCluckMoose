########################
# 
# Main Application Entry
#
########################
LOCAL_PATH := $(call my-dir)
PROJ_PATH  := $(LOCAL_PATH)/../../../..
CUGL_PATH  := $(PROJ_PATH)/cugl
include $(CLEAR_VARS)

LOCAL_MODULE := main

LOCAL_C_INCLUDES := $(CUGL_PATH)/include
LOCAL_C_INCLUDES := $(CUGL_PATH)/include

# Add your application source files here...
LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, $(CUGL_PATH)/build-android/SDL_android_main.c) \
	$(subst $(LOCAL_PATH)/,, $(wildcard $(PROJ_PATH)/source/*.cpp))

# Line the libraries	
LOCAL_SHARED_LIBRARIES := hidapi
LOCAL_SHARED_LIBRARIES += SDL2
LOCAL_SHARED_LIBRARIES += SDL2_image
LOCAL_SHARED_LIBRARIES += SDL2_ttf
LOCAL_SHARED_LIBRARIES += SDL2_codec
LOCAL_STATIC_LIBRARIES := CUGL
LOCAL_STATIC_LIBRARIES += GPG
LOCAL_STATIC_LIBRARIES += ndk_helper
LOCAL_STATIC_LIBRARIES += JuiHelper
LOCAL_STATIC_LIBRARIES += android_native_app_glue
LOCAL_STATIC_LIBRARIES += cpufeatures


LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lGLESv3 -llog

include $(BUILD_SHARED_LIBRARY)
