/*********************************************
 * licstd.h
 * Author: Lily Beauty
 * Created on: Jan 23, 2017
 ********************************************/
#ifndef COMMON_LICSTD_H_
#define COMMON_LICSTD_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef LI_ANDROID
#include <jni.h>
#include <dlfcn.h>
#define LISTDCALL
#elif defined(LI_LINUX)
#include <dlfcn.h>
#define LISTDCALL
#else
#include <windows.h>
#define LISTDCALL __stdcall
#endif


#endif /* COMMON_LICSTD_H_ */
