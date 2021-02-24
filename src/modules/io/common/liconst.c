#include "liconst.h"
#include "listring.h"

#ifdef LI_ANDROID
#include <android/log.h>
#include <jni.h>
#endif


int liisCygwin(){
#ifdef LI_CYGWIN
return 1;
#else
return 0;
#endif
}
int liisMingw(){
#ifdef LI_MINGW
return 1;
#else
return 0;
#endif
}
int liisWindows(){
#if !defined(LI_WINDOWS)
	return 0;
#else
	return 1;
#endif
}
int liisLinux(){
#ifdef LI_LINUX
return 1;
#else
return 0;
#endif
}
int liisAndroid() {
#ifdef LI_ANDROID
return 1;
#else
return 0;
#endif
}

int liisDebug(){
	return 0;
}
char* liappDir(){
	if(liisAndroid()){
		return "/sdcard/LilyBeauty/LiFile";
	}else{
		return "LilyBeauty/LiFile";
	}
}


char* liHomeDir(){
	if(liisAndroid()){
		return "/sdcard/LilyBeauty";
	}else{
		return "LilyBeauty";
	}
}
char* liunixSeparator="/";
char* liwinSeparator="\\";

#ifdef __MINGW32__
char* liseparator="\\";
char* licopy="copy ";
#else
char* liseparator="/";
char* licopy="cp ";
#endif

char* liappTmp="tmp";
char* liFileSelect="file_select";
char* liFileDir="file_dir";
char* liFileMove="file_move";
char* liFileCopy="file_copy";
char* liFileRename="file_rename";
char* liFileSearch="file_search";
char* liFileSafe="file_safe";
char* liFileTrash="file_trash";
char* liFileTrashRestore="file_trash_restore";
char* lidebugTag(){
	if(liisAndroid()){
		return "LI_NDK";
	}
	else{
		return "LI_PC";
	}
}
int liandroidLogPriority(){
#if defined(LI_ANDROID)
	return ANDROID_LOG_ERROR;
#else
	return 1;
#endif
}
void liendian(){
	int t=0x12345678;
	if(*(char*)&t!=0x78)lilog("this machine is big-endian.");
	else lilog("this machine is little-endian.");
}
