LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := 

LOCAL_MODULE    := lily_png
LOCAL_SRC_FILES :=\
	png.c \
	pngerror.c \
	pngget.c \
	pngmem.c \
	pngpread.c \
	pngread.c \
	pngrio.c \
	pngrtran.c \
	pngrutil.c \
	pngset.c \
	pngtrans.c \
	pngwio.c \
	pngwrite.c \
	pngwtran.c \
	pngwutil.c \
	pngtest.c 

LOCAL_C_INCLUDES :=  
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../common/ 
#LOCAL_LDLIBS := -lz
LOCAL_LDLIBS := -lz
LOCAL_SHARED_LIBRARIES := 

include $(BUILD_SHARED_LIBRARY)
