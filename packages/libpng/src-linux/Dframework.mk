include $(CLEAR_VARS)

LOCAL_PATH := $(call my-dir)

LOCAL_SRC_FILES := png.c pngerror.c pngget.c pngmem.c pngpread.c pngread.c pngrio.c pngrtran.c pngrutil.c pngset.c pngtrans.c pngwio.c pngwrite.c pngwtran.c pngwutil.c

LOCAL_INCLUDES := 

LOCAL_CFLAGS := -fPIC -DHAVE_CONFIG_H=1

LOCAL_MODULE := libpng

include $(BUILD_STATIC_LIBRARY)
