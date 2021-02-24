LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := 

#LOCAL_CFLAGS := -std=c11 -std=gnu11

LOCAL_MODULE    := lily_io

#FILE_LIST := $(wildcard $(LOCAL_PATH)/*.c)
#LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)


#traverse all the directory and subdirectory
define walk
  $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

ALLFILES = $(call walk, $(LOCAL_PATH))
FILE_LIST := $(filter %.c, $(ALLFILES))

#$(warning $(FILE_LIST))

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
	
LOCAL_C_INCLUDES := 

LOCAL_LDLIBS := -llog -lm -pthread
LOCAL_SHARED_LIBRARIES := 

include $(BUILD_SHARED_LIBRARY)
