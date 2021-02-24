LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := 

LOCAL_MODULE    := lily_z
LOCAL_SRC_FILES := adler32.c compress.c crc32.c deflate.c  gzclose.c   gzlib.c gzread.c gzwrite.c inflate.c infback.c inftrees.c  inffast.c trees.c  uncompr.c zutil.c


LOCAL_C_INCLUDES :=  
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../common/ 
LOCAL_LDLIBS := 
LOCAL_SHARED_LIBRARIES := 

include $(BUILD_SHARED_LIBRARY)
