/*
 * limacro_public.h
 *
 *  Created on: 2016.11.16
 *      Author: Yun
 */

#ifndef COMMON_LIMACRO_PUBLIC_H_
#define COMMON_LIMACRO_PUBLIC_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "liconst.h"

//extern char* lilibName;

#define printf(format, ...) livlogRedirect(format, ##__VA_ARGS__)
#define fprintf(f,format, ...) livloge(f,format, ##__VA_ARGS__)
//#define fopen(s, t) lifopen(lilibName, s,t)
//#define	 remove(s) liremove(lilibName, s)
//#define licreateLib(x,y) liplugLib(x, y,lilibName);
//#define lidestroyLib() liunplugLib();


typedef unsigned char WXJ_U8;
typedef unsigned short WXJ_U16;
typedef unsigned int WXJ_U32;
typedef char WXJ_S8;
typedef short WXJ_S16;
typedef int WXJ_S32;
#ifndef _M_IX86
	typedef unsigned long long WXJ_U64;
	typedef long long WXJ_S64;
#else
	typedef __int64 WXJ_U64;
	typedef __int64 WXJ_S64;
#endif
typedef char WXJ_CHAR;
typedef char* WXJ_PCHAR;
typedef float WXJ_FLOAT;
typedef double WXJ_DOUBLE;
typedef void WXJ_VOID;
#ifdef __cplusplus
}
#endif
#endif /* COMMON_LIMACRO_PUBLIC_H_ */
