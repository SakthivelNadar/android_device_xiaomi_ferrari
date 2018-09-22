LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE             := libwcnss_qmi
LOCAL_MODULE_TAGS        := optional
LOCAL_PROPRIETARY_MODULE := true
LOCAL_SRC_FILES          := wcnss_xiaomi_client.c
LOCAL_CFLAGS             += -Wall
LOCAL_C_INCLUDES         += hardware/qcom/wlan/wcnss_service
LOCAL_SHARED_LIBRARIES   := libc libcutils libutils liblog
include $(BUILD_SHARED_LIBRARY)
