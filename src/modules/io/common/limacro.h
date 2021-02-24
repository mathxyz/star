/*
 * limacro.h
 *
 *  Created on: 2016.11.15
 *      Author: Yun
 */

#ifndef COMMON_LIMACRO_H_
#define COMMON_LIMACRO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <star_feature_config.h>

#define    LI_MAIN_SUCCESS 0
#define    LI_MAIN_FAILURE 1


#define LI_BOOL int
#define    LI_SUCCESS 1
#define    LI_FAILURE 0
#define    LI_YES 1
#define    LI_NO 0
#define    LI_ERROR -1

//
//#if defined(__ANDROID__) && !defined(LI_ANDROID)
//#define LI_ANDROID
//#elif defined(__linux__) && !defined(LI_LINUX)
//#define LI_LINUX
//#elif !defined(LI_WINDOWS)
//#define LI_WINDOWS
//#if defined(__CYGWIN__) && !defined(LI_CYGWIN)
//#define LI_CYGWIN
//#elif defined(__MINGW32__) && !defined(LI_MINGW)
//#define LI_MINGW
//#endif
//#endif


///////////////////////////////// system ////////////////////////////////////
#ifdef MATH21_WINDOWS
#if !defined(LI_WINDOWS)
#define LI_WINDOWS
#endif
#endif

#ifdef MATH21_ANDROID
#if !defined(LI_ANDROID)
#define LI_ANDROID
#endif
#endif

#ifdef MATH21_APPLE
#if !defined(LI_APPLE)
#define LI_APPLE
#endif
#endif

#ifdef MATH21_LINUX
#if !defined(LI_LINUX)
#define LI_LINUX
#endif
#endif

#ifdef LI_WINDOWS
#if defined(__MINGW32__) && !defined(LI_MINGW)
#define LI_MINGW
#endif
#if defined(__CYGWIN__) && !defined(LI_CYGWIN)
#define LI_CYGWIN
#endif
#endif


#ifdef MATH21_WIN_MSVC
#include <io.h>
#else
#include <unistd.h>
#include <dirent.h>
typedef DIR DIR;
#endif

#ifdef __cplusplus
}
#endif
#endif /* COMMON_LIMACRO_H_ */
