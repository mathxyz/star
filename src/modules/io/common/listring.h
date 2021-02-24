/*
 * listring.h
 *
 *  Created on: 2016.11.8
 *      Author: Yun
 */

#ifndef COMMON_LISTRING_H_
#define COMMON_LISTRING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "limacro.h"

#include "listring_export.h"

void litestlistring();
void lis2ascii(char* s);

void liWriteBit(int bit, char *bitBufp, int* np);

int limkdir(char *s);
int limkdirs(const char *dir);
char* liensureFilePathN(char* dir, const char* s);
void liensureFilePathFN(char* dir,char** ss);

unsigned int xjbitwisehash(char *word, int tsize, unsigned int seed);

//void lifprintf(FILE * f, const char *s, ...);
void livlogfn(void* finput,const char *s, ...);
void livlogfn2(void* finput,const char *s, ...);

void livlogColor(int color, const char *s, ...);
void livlog(const char *s, ...);
void linull(const char *s, ...);
void livloge(FILE *,const char *s, ...);
void lilog(char *s);
void liloge(FILE *,const char *s);
void livlogRedirect(const char *s, ...);
void lilogRedirect(const char *s);

//void livlogRedirect(int isTerminal,const char *s, ...);
//void lilogRedirect(int isTerminal,const char *s);
void liprintfint(int*s,int n);
void liprintfss(char** ss, int nn);
void lilogss(FILE*, char** ss, int nn);

void litest();
void litest2();
void liquicksorttest();
void limergesorttest();
void lishuffletest();
void lishuffletest2();

int liisdigit(int c);
double lirand01();
int lirandint(int n);
int lishuffle(void* s, int n, int esize, int seed);
int lideshuffle(void* s, int n, int esize, int seed);
int licmpss(void *s, void*t);
int licmpss2(void *s, void*t, ...);
int licmpDouble(void*a,void*b);
int licmpint(void*a,void*b);
int liswap(void*s,void*t,int esize);

int lifindArg(char *str, int argc, char **argv);
char* lifindArgRedirectN(char *str, int argc, char **argv);
int lipickOption2(int argc, char** argv, char* opt, char* val,int* pos,int (*scmpf) (char *s1,char *s2));



char** getCmdFromMainN(int argc, char **argv, int* ssl);
char** licmdToArgvN(const char*s, int* ssl);
char* lipwdN();
int liisInt(char* s);
int lis2i(char* s);
int liparseInt(char *s, int * val);

int liensurevvN(void** ss, int *np, int* usep, int step, int esize,
		void (*f)(void**, int));
int liensurevv2N(void** ss, int *np, int use, int step, int esize,
		void (*f)(void**, int));

LI_BOOL liisStrEqual(char*s1,char*s2);
LI_BOOL liisStrExistin(char* s, char* t);


int liscmp(char *s1, char *s2);
int liscmp2(const char *s, const char *t, int fr, int to);
int lisncmp(char *s1, char *s2, int n);
int lisncmp2(char *s, char *t, int n, int fr, int to);
int lisncmp3(const char *s, const char *t, int n, int fr, int to,int tfr,int tto);

char* lisubstringN(char *s, int fr, int to);
int lisubstring(char *s, char** snp, int* np, int fr, int to);
char* listrdupN(char*u);
int listrcpy(char*u, char*t, int n);

int listrlen(const char* s);
int listartsWith(const char *s, char *t, int fr, int to);
int listartsWith3(const char *s, const char *t, int fr, int to,int tfr,int tto);
int liendsWith(char *str, char *suffix);
int liendsWith2(const char *s, const char *t, int fr, int to);
int liendsWith3(const char *s, const char *t, int fr, int to,int tfr,int tto);
int liexistin(char* s, char* mid, int from, int to);
int liexistin2(char* s, char** t,int tn, int fr, int to);
int liexistin3(const char* s, const char* t, int fr, int to, int tfr, int tto);
int liregex2(char* s, char** t, int tn, int fr, int to);

int listrstr(char* s, char* t, int fr, int to);
int listrstrInv(char* s, char* t, int fr, int to);

int liisNull(char*);
int licountin(char* s, char* t, int fr, int to);

char** lisplitby2N(const char* s, char* t, int fr, int to, int* nn,int isKept);
//char** lisplitby(char* s, char* mid, int fr, int to, int* ssl);
int lisubstitutebyN(char* s, char* t, char* tn, int fr, int to, char** sn);

int lifindFirstBetween(char *input, char c, int fr, int to);
int lifindFirstBeforeInv(char *input, char c, int k);
int licountby(char *input, char c);
int licountby2(char *input, char c, int from, int to);
char** lisplitbyN(char *input, char c, int fr, int to, int* nn);
char* liremovecharN(char *input, char c);

void lifree(char* s);
void lifree2(void** s, int zero);
void lifreess(char **ss, int ssl);
void lifreess3(void **vv, int ssl);

int lisaddN(char**up, char*v);


int lifileNameToPath(char* name, char* par, char* path, int pathSize);
char* liscat2N(char*u, char*v);
char* liscat3N(const char *s1, const char *s2, const char*s3);
char* liscat4N(char *s1, char *s2, char*,char*);
char* liscatppN(char **v, int n);
int lirename(char *par,char *old,char *,FILE *fp);
int lirenameSuffix(char *par, char* name, char *old, char *,FILE *fp);
int lirenameSuffixes(char *par, char *old, char *, int isRec, FILE *fp);
int lirenameSuffixesRec(char *par, char *old, char *, int isRec,FILE *fp);
int lirestoreNames(FILE *fp);

void liauthorlog(FILE *fp, char *s);


int lireadEngWordAll(FILE *fp, void* vp) ;
int lireadline(FILE *fp, void (*func)(char*));
char* lireadlineN(FILE *fp);
int xjreadline(FILE *input, char **line, int * maxlenp);
int licat(char*s);

int xjstreamsize(FILE *fp);

int liwriteText(char* s,char* text);
int liwriteStream(FILE* fpt,char* text);
int lireadText(char*s,void (*f)(char*));
int lireadStream(FILE*fpt,void (*f)(char*));
#ifdef __cplusplus
}
#endif
#endif /* COMMON_LISTRING_H_ */
