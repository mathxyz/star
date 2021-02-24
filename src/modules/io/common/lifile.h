/*
 * lifile.h
 *
 *  Created on: 2016.11.16
 *      Author: Yun
 */

#ifndef COMMON_LIFILE_H_
#define COMMON_LIFILE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "limacro.h"
#include <stdio.h>

#include "lifile_export.h"

typedef struct __ReadLineArgs {
    const char *name;
    int startLine;
    int maxLineCount;
} ReadLineArgs;

typedef struct __StringArr {
    char **pp;
    int max;
    int use;
} StringArr;


void litestlifile();

void listo2(FILE *fps, FILE *fpt, int *bitAdded);

int li2tos(FILE *fs, FILE *ft, int bitIgnored);

int _listo2(FILE *fs, char *bitBufp, int *np);

int liwriteIntAtPos(int pos, int i, FILE *fp, int isBinary);

void liplugLib(int argc, char **argv, char *lilibName);

void liunplugLib();

int ligetIsTerminal();

char *lifilePar2N(char *s);

int lifileParN(char *s, char **parp, int *np);

int lifilePar4N(char *s, char **parp);

int lifileName4N(char *s, char **parp);

int lifileNameNoSuf4N(char *s, char **parp);

int lifileNameSuf4N(char *s, char **parp);

int lifileNameN(char *s, char **name, int *np);

int liremove(char *path, const char *s);

FILE *lifopen(char *path, const char *s, const char *t);

int lifclose(FILE **);

void lipwd();

void lifileSize(char *file);

int _lils(char *name, int level, int recTimes);

int liremove3(char *par, char **pattern, int patternNum);

int lirmdir(char *filename);

int lirm(int argc, char **argv);

void _liremoveUsage();

int _lirm_pat(char *patPath, int recTimes, int tf, int td);

int lisearchFileSufN(void **pp, char *par, char *suf, int *np, int *usep, int level,
                     int recTimes, int ignorePermission, void (*f)(void *, const char *, ...), void *finput);

void lisearchFileSuf2N(void **pp, char *par, char *suf, int *usep);

void lisearchFiles2N(void **pp, const char *par, char **patternpp, int patternNum, int *usep);

int lisearchFilesN(void **pp, const char *par, char **patternpp, int patternNum, int *np, int *usep, int level,
                   int recTimes, void (*f)(void *, const char *, ...), void *finput);

int lisearchFiles4N(void **pp, const char *par, char **patternpp, int patternNum, int *np, int *usep, int level,
                    int recTimes);

int lisearchFiles5N(void **pp, const char *par, char **patternpp, int patternNum,
                    int *usep, int recTimes);

int lisearchImagesN(void **pp, const char *par, int *usep);

int lisearchAndTackle(void **pp, const char *par, char **patternpp, int patternNum,
                      int *np, int *usep, int level, int recTimes, int tf, int td, int tnr,
                      int (*satisfy)(char *s, char **patternpp, int patternNum, int fr, int to),
                      int (*tackle)(void **pp, int *np, int *usep, const char *par, char *entry,
                                    int isDir));

int _litackle_frame(void **pp, const char *par, char **patternpp, int patternNum, int *np,
                    int *usep, int recTimes, int tf, int td,
                    int (*satisfy)(char *s, char **patternpp, int patternNum, int fr, int to),
                    int (*tackle)(void **pp, int *np, int *usep, const char *par, char *entry,
                                  int isDir));

/*
 * tackle first, then recursive.
 */
int _lisearchBase10(void **pp, char *par, int *np, int *usep, int recTimes, void *fargs,
                    int (*tackle)(void **pp, int *np, int *usep, char *path, int recTimes, void *fargs));

int lisearchN(int argc, char **argv, void **ppp, int *usep);

// search 10
int lisearch(int argc, char **argv);

void lisearchFiles2Ntest(char *);

void lisearchFileSufNtest();

int lisearchSatisfy(char *s, char **patternpp, int patternNum, int fr, int to);

int lisearchTackle(void **pp, int *np, int *usep, const char *par, char *entry, int isDir);

int liremoveTackle(void **pp, int *np, int *usep, const char *par, char *entry, int isDir);

int lisearchTackle2(void **pp, int *np, int *usep, const char *par, char *entry,
                    int isDir);


int _lisearchFiles(char *par, char **patternpp, int patternNum,
                   int recTimes);

int lisearchFiles(char *par);

int lirenameFiles();

int lirenameFiles2(int undo);

int licopyFiles();

int lirenameFilesRemoveAndAdd(int removed, char *suf);

int lisafeFiles2(int undo);

int lisafeFiles(char *suf);

void lisafe1(char *s);

void lisafe2(char *s);

void lisafe3(char *s);

void lisafeDoc();

void lisearchFilestest();


int liJiShiBenWrite(char *name, const char *content);


/* not check args*/
int lireadLines(ReadLineArgs *pReadLineArgs, void *fargs,
                int (*f)(void *entry, void *fargs));

int lireadTackle(void *entry, void *fargs);

int lireadLinesMainN(int argc, char **argv, void *output);

#ifdef __cplusplus
}
#endif
#endif /* COMMON_LIFILE_H_ */
