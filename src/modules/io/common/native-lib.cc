/*#include "liCommonInner.h"

#ifdef LI_ANDROID
#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_lili_file_MyFrame_helloworldFromJNI(
        JNIEnv* env,
        jobject *//* this *//*) {
    std::string hello = "\nHello LiFile";
    return env->NewStringUTF(hello.c_str());
}
#else
#endif*/
