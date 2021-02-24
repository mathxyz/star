APP :=lily_z

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

#INC_PATH := -I$(DIR)$(bs)common
INC_PATH := 
LIB_PATH := 
LIBS := 
#LIBS := -lliCommon -lz
CXXFLAGS :=	-O2 -g -Wall -fmessage-length=0

define walk
  $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

LOCAL_PATH := .
ALLFILES = $(call walk, $(LOCAL_PATH))
FILE_LIST := $(filter %.c, $(ALLFILES))

SRCS =   adler32.c compress.c crc32.c deflate.c  gzclose.c   gzlib.c gzread.c gzwrite.c inflate.c infback.c inftrees.c  inffast.c trees.c  uncompr.c zutil.c
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


adler32.o: zlib.h zconf.h
compress.o: zlib.h zconf.h
crc32.o: crc32.h zlib.h zconf.h
deflate.o: deflate.h zutil.h zlib.h zconf.h
gzclose.o: zlib.h zconf.h gzguts.h
gzlib.o: zlib.h zconf.h gzguts.h
gzread.o: zlib.h zconf.h gzguts.h
gzwrite.o: zlib.h zconf.h gzguts.h
inffast.o: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
inflate.o: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
infback.o: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
inftrees.o: zutil.h zlib.h zconf.h inftrees.h
trees.o: deflate.h zutil.h zlib.h zconf.h trees.h
uncompr.o: zlib.h zconf.h
zutil.o: zutil.h zlib.h zconf.h
.PHONY: all clean install sqr all_inner clean_inner