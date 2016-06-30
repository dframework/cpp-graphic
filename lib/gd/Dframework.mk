LOCAL_PATH := $(call my-dir)

@. ${LOCAL_PATH}/../../Application.sh
ifeq (${DFRAMEWORK_CPP_COMMON}, "")
   DFRAMEWORK_CPP_COMMON := ${LOCAL_PATH}/../../../cpp-common
endif

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    GDImage.cpp    \
    GDFont.cpp

LOCAL_INCLUDES :=                     \
    ${DFRAMEWORK_CPP_COMMON}/include  \
    ${DFRAMEWORK_CPP_GRAPHIC}/include \
    ${DFRAMEWORK_CPP_GRAPHIC}/packages/libpng/src-linux  \
    ${DFRAMEWORK_CPP_GRAPHIC}/packages/libjpeg/src-linux \
    ${DFRAMEWORK_CPP_GRAPHIC}/packages/libgd/src-linux

LOCAL_CFLAGS := -fpermissive -fPIC

LOCAL_MODULE := libdframework-graphic-gd

include $(BUILD_STATIC_LIBRARY)

