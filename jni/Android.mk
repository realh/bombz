LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CFLAGS += -fexceptions -frtti -D_GNU_SOURCE

LOCAL_MODULE := bombz
LOCAL_SRC_FILES := \
		$(wildcard hgame/*.cpp) \
		$(wildcard hgl/*.cpp) \
		$(wildcard hgles/*.cpp) \
		$(wildcard handroid/*.cpp) \
		$(wildcard bombz/*.cpp) \
		AndroidMain.cpp

LOCAL_LDLIBS := -landroid -llog
LOCAL_STATIC_LIBRARIES := android_native_app_glue
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
