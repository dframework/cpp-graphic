LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    Graph.cpp

LOCAL_CFLAGS := -fPIC

LOCAL_INCLUDES :=                \
    ${LOCAL_PATH}/../../include  \

LOCAL_MODULE := libdframework-graphic-graph

include $(BUILD_STATIC_LIBRARY)

