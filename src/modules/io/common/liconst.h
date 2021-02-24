/*
 * liconst.h
 *
 *  Created on: 2016.11.14
 *      Author: Yun
 */

#ifndef COMMON_LICONST_H_
#define COMMON_LICONST_H_

#ifdef __cplusplus
extern "C" {
#endif

extern char* liunixSeparator;
extern char* liwinSeparator;

extern char* liseparator;
extern char* liseparator2;
extern char* licopy;

extern char* liappTmp;

extern char* liFileSelect;
extern char* liFileDir;
extern char* liFileMove;
extern char* liFileCopy;
extern char* liFileRename;
extern char* liFileSearch;
extern char* liFileSafe;

extern char* liFileTrash;
extern char* liFileTrashRestore;

int liisCygwin();
int liisMingw();

int liisWindows();
int liisLinux();
int liisAndroid();
//int liisAndroid();

int liisDebug();
char* liappDir();
char* liHomeDir();
char* lidebugTag();
int liandroidLogPriority();
void liendian();

#ifdef __cplusplus
}
#endif
#endif /* COMMON_LICONST_H_ */
