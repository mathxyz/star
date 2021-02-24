#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

int sky(int argc, const char **argv);

#if defined(__ANDROID__)

#include <jni.h>
JNIEXPORT void JNICALL
Java_org_sky_app_Sky_run(JNIEnv *env, jclass clazz, jstring jstr);
#endif

#ifdef __cplusplus
}
#endif
