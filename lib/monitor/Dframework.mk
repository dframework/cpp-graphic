LOCAL_PATH := $(call my-dir)

@. ${LOCAL_PATH}/../../Application.sh
ifeq (${DFRAMEWORK_CPP_COMMON}, "")
   DFRAMEWORK_CPP_COMMON := ${LOCAL_PATH}/../../../cpp-common
endif

include $(CLEAR_VARS)

LOCAL_SRC_FILES :=     \
    GdBaseMonitor.cpp  \
    GdMonPacket.cpp    \
    GdMonMemory.cpp    \
    GdMonLoadavg.cpp   \
    GdMonDiskstats.cpp \
    GdMonSocket.cpp    \
    GdMonCpustat.cpp

LOCAL_CFLAGS := -fPIC

LOCAL_INCLUDES :=                    \
    ${LOCAL_PATH}/../../include      \
    ${DFRAMEWORK_CPP_COMMON}/include

LOCAL_MODULE := libdframework-graphic-monitor

include $(BUILD_STATIC_LIBRARY)

