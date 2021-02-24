APP :=lily_jpeg

WIN_SUFFIX:=.dll
LINUX_SUFFIX:=.so
LINUX_PREFIX:=lib
SUFFIX:=$(WIN_SUFFIX)
PREFIX:=

ifeq ($(IS_WIN), 1)

else ifeq ($(IS_LINUX), 1)
PREFIX =$(LINUX_PREFIX)
SUFFIX	=$(LINUX_SUFFIX)

else
#SHELL=cmd.exe
endif

DIR :=..$(bs)..
BUILD_PATH:=$(DIR)$(bs)$(BUILD)
APP_NAME :=$(PREFIX)$(APP)$(SUFFIX)
TARGET	:= $(DIR)$(bs)$(APP_NAME)

#INC_PATH := -I$(DIR)$(bs)common -I$(DIR)$(bs)3rdparty$(bs)libpng
LIB_PATH := -L$(DIR)
#LIBS := -llily_io -lm -llily_z

CXXFLAGS :=	-O2 -g -Wall -fmessage-length=0 -Wno-write-strings -std=c99 -pedantic  -D_POSIX_C_SOURCE=200809L 

define walk
  $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

LOCAL_PATH := .
ALLFILES = $(call walk, $(LOCAL_PATH))
FILE_LIST := $(filter %.c, $(ALLFILES))

SRCS = $(FILE_LIST)
OBJS = $(SRCS:.c=.o)


ifeq ($(IS_MINGW), 0)
all: all_inner
else
all: all_inner
endif

clean: clean_inner

all_inner:$(TARGET) sqr

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(LIB_PATH) $(LIBS)
	
.c.o:
	$(CXX) $(CXXFLAGS) $(INC_PATH) -fPIC  -o $@ -c  $<

sqr:
	@$(CP) $(TARGET) $(BUILD_PATH)$(bs)$(APP_NAME)
	@$(RM) $(TARGET)

clean_inner:
ifeq ($(IS_MINGW), 1)
	-@$(RM) $(TARGET) $(subst /,\\, $(OBJS))
	-@echo LilyBeauty
	$(warning using MINGW)
else
	-$(RM) $(TARGET) $(OBJS)
	-@echo LilyBeauty
endif

.PHONY: all clean install sqr all_inner clean_inner
