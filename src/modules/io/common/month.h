/*********************************************
 * month.h
 * Author: Yolsa Eni
 * Created on: May 10, 2017
 ********************************************/

#ifndef COMMON_MONTH_H_
#define COMMON_MONTH_H_

#include "liCommon.h"
//////////////////////////////////////
#ifndef DO_NOT_USE_TIME
#define DO_NOT_USE_TIME
#endif
float getticks();
/////////////////////////////////////////
#ifdef __unix__
#include <unistd.h>
#elif defined _WIN32
#include <windows.h>
#define sleep(x) Sleep(1000 * x)
#endif
///////////////////////////////////////////
#ifndef DO_NOT_USE_LOG
#define DO_NOT_USE_LOG
#endif

#if !defined(YOLSA_PEA_RELEASE)
#define xjlog(...) livlog(__VA_ARGS__)
#else
#define xjlog(...) ;
#endif
////////////////////////////////////////
#endif /* COMMON_MONTH_H_ */
