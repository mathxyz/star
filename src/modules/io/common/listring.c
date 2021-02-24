/*
 * liCommon.c
 *
 *  Created on: 2016.10.16
 *      Author: Yun
 */

#include "liCommonInner.h"
//#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <dirent.h>

#ifndef LI_ANDROID
#define DEBUG_MACRO(x) printf("%s",x);
#endif
#ifdef LI_ANDROID
#include <android/log.h>
#include <jni.h>
#define DEBUG_TAG "LI_NDK"
#define DEBUG_MACRO(x) __android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "%s", x);
#endif

#define MAX_STRING_LENGTH 1000
//remember to undefine debug when release.
const int debug_message_max = 1024;
int debug = 0;

// N in liPwdN means that free must be called afterwards. N stands for new.
// 1 means yes and 0 means no; yes does not have to stand for right, likewise no doesn't have to be wrong.

void liWriteBit(int bit, char *bitBufp, int *np) {
    if (*np == 8) {
        (*np) = 0;
        (*bitBufp) = 0;
    }
    (*bitBufp) <<= 1;
    if (bit)
        (*bitBufp) |= 0x1;

    (*np)++;
}

char *liensureFilePathN(char *dir, const char *s) {
    char *sn;
    if (!listartsWith(s, "/", 0, -1)) {
        sn = liscat3N(dir, "/", s);
    } else {
        sn = liscat3N("", "", s);
    }
    return sn;
}

void liensureFilePathFN(char *dir, char **ss) {
    char *sn = *ss;
    if (!listartsWith(*ss, "/", 0, -1)) {
        sn = liscat3N(dir, "/", *ss);
    } else {
        sn = liscat3N("", "", *ss);
    }
    free(*ss);
    *ss = sn;
}

int _limkdir(const char *filename, mode_t mode) {
#ifdef LI_MINGW
    return mkdir(filename);
#else
    return mkdir(filename, mode);
#endif
}

int limkdir(char *s) {
    struct stat st = {0};
    int re = 0;

    if (stat(s, &st) == -1) {
        _limkdir(s, 0700);
        re = 1;
    }
    return re;
}

int limkdirs(const char *dir) {
    if (!dir)
        return -1;

    DIR *dir2;
    if (!(dir2 = opendir(dir))) {
    } else {
        return 1;
    }

    struct stat st = {0};
    char tmp[256];
    char *p = NULL;
    size_t len, re = 0;

    snprintf(tmp, sizeof(tmp), "%s", dir);
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++)
        if (*p == '/') {
            *p = 0;
            if (stat(tmp, &st) == -1) {
                _limkdir(tmp, S_IRWXU);
                re = 1;
            }

            *p = '/';
        }
    if (stat(tmp, &st) == -1) {
        _limkdir(tmp, S_IRWXU);
        re = 1;
    }
    return re;
}

/* Move-to-front hashing and hash function from Hugh Williams, http://www.seg.rmit.edu.au/code/zwh-ipl/ */

/* Simple bitwise hash function */
unsigned int xjbitwisehash(char *word, int tsize, unsigned int seed) {
    char c;
    unsigned int h;
    h = seed;
    for (; (c = *word) != '\0'; word++)
        h ^= ((h << 5) + c + (h >> 2));
    return ((unsigned int) ((h & 0x7fffffff) % tsize));
}

void liprintSmile() {
    int i, j;
    livlog("\n");
    for (i = 1; i < 11; i++) {
        for (j = 1; j <= i; j++) {
            livlog("%c%c", 216, 216);
        }
        livlog("\n");
    }
}

int liisdigit(int c) {
    return (c >= '0' && c <= '9');
}

int liisLeapYear(int year) {
    if ((year % 4) == 0 && ((year % 25) != 0 || (year % 16) == 0)) {
        /* leap year */
        livlog("leap year\n");
        return 1;
    }
    livlog("not leap year\n");
    return 0;
}

//[0,1)
double lirand01() {
    unsigned int re;
    do {
        re = rand();
    } while (re == RAND_MAX);
    return (double) re / RAND_MAX;
}

// [0,n), not including n.  lirand01()* n ;
int lirandint(int n) {
    unsigned int count;
    int re;
    if (n <= 0)
        return -1;
    count = RAND_MAX / n;
    do {
        re = rand() / count;
    } while (re >= n);
    return re;
}

extern void math21_c_seed_random_generator(unsigned int seed);

int lishuffle(void *s, int n, int esize, int seed) {
    int i, t;
    math21_c_seed_random_generator(seed);
    for (i = n - 1; i >= 0; i--) {
        t = lirandint(i + 1);
        if (t != i)
            liswap(s + esize * t, s + esize * i, esize);
    }
    return 1;
}

int lideshuffle(void *s, int n, int esize, int seed) {
    int i, *ip;
    ip = malloc(n * sizeof *ip);
    if (ip == 0)
        return 0;
    math21_c_seed_random_generator(seed);
    for (i = n - 1; i >= 0; i--)
        ip[i] = lirandint(i + 1);
    for (i = 0; i < n; i++) {
        liswap(s + esize * ip[i], s + esize * i, esize);
    }
    free(ip);
    return 1;
}

int licmpDouble(void *a, void *b) {
    if (*(double *) a == *(double *) b) {
        return 0;
    } else if (*(double *) a > *(double *) b) {
        return 1;
    } else {
        return -1;
    }
}

int licmpint(void *a, void *b) {
    if (*(int *) a == *(int *) b) {
        return 0;
    } else if (*(int *) a > *(int *) b) {
        return 1;
    } else {
        return -1;
    }
}

/* s and t is void** */
int licmpss(void *s, void *t) {
    char *s1, *s2;
    s1 = *(char **) s;
    s2 = *(char **) t;
    while (*s1 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

/* s and t is void** */
int licmpss2(void *s, void *t, ...) {
    char *s1, *s2;
    s1 = *(char **) s;
    s2 = *(char **) t;
    while (*s1 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

int liswap(void *s, void *t, int esize) {
    char p[esize];
    memcpy(p, s, esize);
    memcpy(s, t, esize);
    memcpy(t, p, esize);
    return 1;
}

void liprintfint(int *s, int n) {
    int i = 0;
    while (i < n) {
        livlog("%d ", s[i]);
        i++;
    }
    livlog("\n");
}

void liprintfss(char **ss, int nn) {
    int i;
    for (i = 0; i < nn; i++) {
        if (!ss[i])break;
        livlog("[%d]:length:%d: %s\n", i, listrlen(ss[i]), ss[i]);
    }
}

void lilogss(FILE *finput, char **ss, int nn) {
    int i;
    if (!finput)
        for (i = 0; i < nn; i++) {
            livlog("ss[%d]: %s\n", i, ss[i]);
        }
    else {
        for (i = 0; i < nn; i++) {
            fprintf((FILE *) finput, "%s\n", ss[i]);
        }
    }
}

int lifindArg(char *str, int argc, char **argv) {
    int i = 0;
    for (i = 1; i < argc; i++) {
        if (!liscmp(str, argv[i])) {
            if (i == argc - 1) {
                printf("Argument missing for %s\n", str);
                return -1;
            }
            return i;
        }
    }
    return 0;
}

char *lifindArgRedirectN(char *str, int argc, char **argv) {
    int i;
    char *s;
    for (i = 1; i < argc; i++) {
        if (listartsWith(argv[i], str, 0, -1)) {

            if (listrlen(argv[i]) > listrlen(str)) {
                s = lisubstringN(argv[i], listrlen(str), -1);
            } else {
                if (i == argc - 1) {
                    printf("No argument given for %s\n", str);
                    return 0;
                }
                s = listrdupN(argv[i + 1]);
            }
            return s;
        }
    }
    return 0;
}

// have little bugs
/**v's content is not changed,
 * return 1 means found.
 * 0 means not found
 * -1 means error.
 * scmpf is your compare function,
 * which compare argv[i]+1 and opt,
 * 0 stands for equality.
 * you can define exist in function,
 * which returns 0 to say they are equal.
 * here we use isEqual where returning one means equality.**/
int lipickOption2(int argc, char **argv, char *opt, char *val, int *pos,
                  int (*isEqual)(char *s1, char *s2)) {
    int output = 0;
    int i;
    // scan the command line for '-opt'
    for (i = 0; i < argc; i++) {
        if (argv[i][0] == '-' && 1 == isEqual(argv[i] + 1, opt)) {
            // check for a corresponding value
            if (val == 0) {
                *pos = i;
                output = 1;
            } else {
                if (i == argc - 1) {
                    output = -1;
                } else {
                    if (argv[i + 1][0] == '-' && !liisInt(argv[i + 1])) {
                        output = -1;
                    }
                        // if the option call is well formed
                    else {
                        // copy the option value ...
                        strcpy(val, argv[i + 1]);
                        *pos = i + 1;
                        output = 1;
                    }
                }
            }

            // print an error if not success
            if (output == -1) {
                fprintf(stderr,
                        "Fatal error: option %s requires an argument.\n", opt);
            }
        }
    }
    return output;
}

void liandroid_log_print(const char *s) {
#ifdef LI_ANDROID
    __android_log_print(liandroidLogPriority(), lidebugTag(), "%s", s);
#endif
}

void livlogfn(void *finput, const char *s, ...) {
    va_list va;
    va_start(va, s);
    char buffer[debug_message_max];
    vsprintf(buffer, s, va);
    va_end(va);
    if (!finput)
        lilog(buffer);
    else {
        lilog(buffer);
    }
}

void livlogfn2(void *finput, const char *s, ...) {
    va_list va;
    va_start(va, s);
    char buffer[debug_message_max];
    vsprintf(buffer, s, va);
    va_end(va);
    if (!finput)
        lilog(buffer);
    else {
        fprintf((FILE *) finput, "%s", buffer);
    }
}

void livlog(const char *s, ...) {
    va_list va;
    va_start(va, s);
    char buffer[debug_message_max];
    vsprintf(buffer, s, va);
    va_end(va);
    lilog(buffer);
}

void linull(const char *s, ...) {

}

void livlogColor(int color, const char *s, ...) {
    int myColor = 0;
    va_list va;
    va_start(va, s);
    char buffer[debug_message_max];
    vsprintf(buffer, s, va);
    va_end(va);
#ifdef LI_ANDROID
    lilog(buffer);
#elif defined(LI_LINUX)
    lilog(buffer);
#else

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;

    /* Save current attributes */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    switch (color) {
    case 0xff0000:
        myColor = FOREGROUND_RED;
        break;
    case 0x00ff00:
        myColor = FOREGROUND_GREEN;
        break;
    case 0x0000ff:
        myColor = FOREGROUND_BLUE;
        break;
    case 0xffff00:
        myColor = FOREGROUND_RED | FOREGROUND_GREEN;
        break;
    default:
        myColor = saved_attributes;
        break;
    }

    SetConsoleTextAttribute(hConsole, myColor);
//	    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    lilog(buffer);
    /* Restore original attributes */
    SetConsoleTextAttribute(hConsole, saved_attributes);
#endif
}

void livloge(FILE *f, const char *s, ...) {
    va_list va;
    va_start(va, s);
    char buffer[debug_message_max];
    vsprintf(buffer, s, va);
    va_end(va);

    liloge(f, buffer);
}

void lilog(char *s) {
    if (!liisAndroid())
        fprintf(stderr, "%s", s);
    else {
        liandroid_log_print(s);
    }
}

void livlogRedirect(const char *s, ...) {
    va_list va;
    va_start(va, s);
    char buffer[debug_message_max];
    vsprintf(buffer, s, va);
    va_end(va);

    lilogRedirect(buffer);
}

void lilogRedirect(const char *s) {
    if (!liisAndroid())
        printf("%s", s);
    else {
        if (ligetIsTerminal()) {
            liandroid_log_print(s);
        } else {
            printf("%s", s);
        }
    }
}

void liloge(FILE *f, const char *s) {
    if (!f)
        return;
    if (!liisAndroid())
        fprintf(f, "%s", s);
    else {
        if (f == stderr) {
            liandroid_log_print(s);
        } else {
            fprintf(f, "%s", s);
        }
    }
}

char **getCmdFromMainN(int argc, char **argv, int *ssl) {

    char **ss;
    if (argc < 1)
        return 0;
    char *s = liscatppN(argv, argc);
    if (!s)
        return 0;
    char *t = ";";

    ss = lisplitby2N(s, t, 0, -1, ssl, 1);
    free(s);
    return ss;
}

char **licmdToArgvN(const char *s, int *ssl) {
    return lisplitby2N(s, " ", 0, -1, ssl, 0);
}

char *lipwdN() {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
//	printf("%s\n", cwd);
    char *cwd2 = malloc(1025 * sizeof(char));
    strcpy(cwd2, cwd);
    return cwd2;
}

int liisInt(char *s) {
    int i = listrlen(s);
    if (i <= 0)
        return 0;
    if (*s && *s == '-') {
        s++;
        if (i == 1) {
            return 0;
        }
    }
    while (*s) {
        // get current character then increment
        int le = *s++;
        // transform s character to the 4bit equivalent number, using the ascii table indexes
        if (le >= '0' && le <= '9');
        else {
            return 0;
        }
    }
    return 1;
}

int lis2i(char *s) {
    int re = 0;
    int sign = 1;
    if (*s && *s == '-') {
        sign = -1;
        s++;
    }
    while (*s) {
        re = re * 10 + (*s - '0');
        s++;
    }
    return sign * re;
}

// 10
int liparseInt(char *s, int *val) {
    int i = listrlen(s);
    if (i <= 0)
        return 0;
    *val = 0;
    int sign = 1;
    if (*s && *s == '-') {
        sign = -1;
        s++;
        if (i == 1) {
            return 0;
        }
    }
    while (*s) {
        // get current character then increment
        int le = *s++;
        // transform s character to the 4bit equivalent number, using the ascii table indexes
        if (le >= '0' && le <= '9')
            le = le - '0';
        else {
            lilog("Error parsing base-%10 number\n");
            return 0;
        }
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        *val = *val * 10 + le;
    }
    *val = *val * sign;
    return 1;
}

/* ensure *np>*usep, where n<use is wrong.
 * np and usep use block index, not char index
 * step is realloc block size,
 * f is free function, and free use, not n */
int liensurevvN(void **ss, int *np, int *usep, int step, int esize,
                void (*f)(void **, int)) {
    if (!ss)
        return 0;
    void *stmp;
    if (*ss && *np > 0) {
        if (*np > *usep)
            return 1;
        else if (*np == *usep) {
            stmp = realloc(*ss, (*usep + step) * esize);
        } else {
            f(ss, *usep);
            return 0;
        }
    } else {
        *np = 0;
        *usep = 0;
        stmp = malloc(step * esize);
    }
    if (!stmp) {
        f(ss, *usep);
        return 0;
    }
    *ss = stmp;
    *np = *usep + step;

    return 1;
}

/* ensure *np>use,
 * np and use use block index, not char index
 * step is realloc block size,
 * f is free function, and free use, not n */
int liensurevv2N(void **ss, int *np, int use, int step, int esize,
                 void (*f)(void **, int)) {
    if (!ss)
        return 0;
    void *stmp;
    if (*ss && *np > 0) {
        if (*np > use)
            return 1;
        else {
            stmp = realloc(*ss, (use + step) * esize);
        }
    } else {
        *np = 0;
        stmp = malloc((use + step) * esize);
    }
    if (!stmp) {
        f(ss, use);
        return 0;
    }
    *ss = stmp;
    *np = use + step;

    return 1;
}

/* Efficient string comparison */
int liscmp(char *s1, char *s2) {
    while (*s1 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

LI_BOOL liisStrEqual(char *s1, char *s2) {
    if (liscmp(s1, s2) == 0)
        return LI_YES;
    else
        return LI_NO;
}

/* compare first n letter at most,
 * so we compare n-1 + 1 times at most*/
int lisncmp(char *s1, char *s2, int n) {
    int i = 1;
    while (*s1 != '\0' && *s1 == *s2 && i < n) {
        s1++;
        s2++;
        i++;
    }
    return (*s1 - *s2);
}

/* Users must ensure augments. 0<=fr<to<=listrlen(s);
 * equals lisncmp(lisubstringN(s,fr,to),t,n);
 * Note: c and cc is equal if n is 1*/
int lisncmp2(char *s, char *t, int n, int fr, int to) {
    int i = 1;
    char *sn = s + fr;
    while (sn != s + to && *t != 0 && *sn == *t && i < n) {
        sn++;
        t++;
        i++;
    }
    if (sn == s + to)
        return 0 - *t;
    return (*sn - *t);
}

int lisncmp3(const char *s, const char *t, int n, int fr, int to, int tfr, int tto) {
    int i = 1;
    const char *sn = s + fr;
    const char *tn = t + tfr;
    while (sn != s + to && tn != t + tto && *sn == *tn && i < n) {
        sn++;
        tn++;
        i++;
    }
    if (sn == s + to && tn == t + tto) {
        return 0;
    } else if (sn == s + to) {
        return 0 - *tn;
    } else if (tn == t + tto) {
        return *sn - 0;
    } else {
        return (*sn - *tn);
    }
}

/* Users must ensure augments. 0<=fr<to<=listrlen(s);
 * equals liscmp(lisubstringN(s,fr,to),t);*/
int liscmp2(const char *s, const char *t, int fr, int to) {
    int i = 1;
    const char *sn = s + fr;
    while (sn != s + to && *t != 0 && *sn == *t) {
        sn++;
        t++;
        i++;
    }
    if (sn == s + to)
        return 0 - *t;
    return (*sn - *t);
}

// won't produce empty string.
char *lisubstringN(char *s, int fr, int to) {
    int sl;
    sl = listrlen(s);
    if (to == -1)
        to = sl;
    if (to <= fr || fr < 0 || to > sl)
        return 0;
    char *sn = malloc(to - fr + 1);
    if (!sn)
        return sn;
    memcpy(sn, s + fr, to - fr);
    sn[to - fr] = 0;
    return sn;
}

// won't produce empty string.
int lisubstring(char *s, char **snp, int *np, int fr, int to) {
    int sl;
    sl = listrlen(s);
    if (to == -1)
        to = sl;
    if (to <= fr || fr < 0 || to > sl)
        return 0;
    if (liensurevv2N((void **) snp, np, to - fr, 1, sizeof(char), lifree2)
        == 0) {
        return 0;
    }
    memcpy(*snp, s + fr, to - fr);
    (*snp)[to - fr] = 0;
    return 1;
}

int listartsWith(const char *s, char *t, int fr, int to) {
    if (!s || !t)
        return 0;

    int sl;
    sl = listrlen(s);
    if (to == -1)
        to = sl;
    if (to <= fr || fr < 0 || to > sl)
        return 0;
    return lisncmp2((char*)s, t, listrlen(t), fr, to) == 0;
}

int listartsWith3(const char *s, const char *t, int fr, int to, int tfr, int tto) {
    if (!s || !t)
        return 0;

    int sl, tl;
    sl = listrlen(s);
    tl = listrlen(t);
    if (to == -1)
        to = sl;
    if (tto == -1) {
        tto = tl;
    }
    if (to <= fr || fr < 0 || to > sl)
        return 0;
    if (tto <= tfr || tfr < 0 || tto > tl)
        return 0;
    return lisncmp3(s, t, tto - tfr, fr, to, tfr, tto) == 0;
}

int liendsWith3(const char *s, const char *t, int fr, int to, int tfr, int tto) {
    if (!s || !t)
        return 0;

    int sl, tl;
    sl = listrlen(s);
    if (to == -1)
        to = sl;
    if (to <= fr || fr < 0 || to > sl)
        return 0;

    tl = listrlen(t);
    if (tto == -1) {
        tto = tl;
    }
    if (tto <= tfr || tfr < 0 || tto > tl)
        return 0;

    if (tto - tfr > to - fr)
        return 0;
    return liscmp2(s, t + tfr, to + tfr - tto, to) == 0;
}

int liendsWith(char *s, char *t) {
    if (!s || !t)
        return 0;
    size_t sl = listrlen(s);
    size_t tl = listrlen(t);
    if (tl > sl)
        return 0;
    return liscmp(s + sl - tl, t) == 0;
}

int liendsWith2(const char *s, const char *t, int fr, int to) {
    if (!s || !t)
        return 0;

    int sl, tl;
    sl = listrlen(s);
    if (to == -1)
        to = sl;
    if (to <= fr || fr < 0 || to > sl)
        return 0;

    tl = listrlen(t);
    if (tl > to - fr)
        return 0;
    return liscmp2(s, t, to - tl, to) == 0;
}

LI_BOOL liisStrExistin(char *s, char *t) {
    if (liexistin(s, t, 0, -1) == 1)
        return LI_YES;
    else
        return LI_NO;
}

// whether t appears in s. return -1 , 0 , 1;
int liexistin(char *s, char *t, int fr, int to) {
    int re = 0, i, l1 = listrlen(s), l2 = listrlen(t);
    if (to == -1)
        to = l1;
    if (l1 < 1 || l2 < 1 || fr < 0 || fr >= to || to > l1)
        return -1;
    i = fr;
    while (to - i >= l2) {
        if (listartsWith(s + i, t, 0, -1)) {
            re++;
            break;
        } else {
            i++;
        }
    }
    return re;
}

// whether t[i] appears in s. return -1 , 0 , 1;
int liexistin2(char *s, char **t, int tn, int fr, int to) {
    int re = 0, i = 0;
    while ((i < tn) && (re = liexistin(s, t[i++], fr, to)) == 0);
    return re;
}

int liexistin3(const char *s, const char *t, int fr, int to, int tfr, int tto) {
    int re = 0, i, sl = listrlen(s), tl = listrlen(t);
    if (to == -1)
        to = sl;
    if (sl < 1 || tl < 1 || fr < 0 || fr >= to || to > sl)
        return -1;
    if (tto == -1) {
        tto = tl;
    }
    if (tto <= tfr || tfr < 0 || tto > tl)
        return 0;
    i = fr;
    while (to - i >= tto - tfr) {
        if (listartsWith3(s + i, t, 0, -1, tfr, tto)) {
            re++;
            break;
        } else {
            i++;
        }
    }
    return re;
}

// whether t[i] appears in s. return -1 , 0 , 1;
int liregex2(char *s, char **t, int tn, int fr, int to) {
    int re = 0, i = 0;
    while ((i < tn) && (re = liregex3(s, t[i++], fr, to)) == 0);
    return re;
}

// whether t appears in s. return -1 , 0 , 1;
int liregex3(const char *s, const char *t, int fr, int to) {
    int re = 0, l1 = listrlen(s), l2 = listrlen(t);
    if (to == -1)
        to = l1;
    if (l1 < 1 || l2 < 1 || fr < 0 || fr >= to || to > l1)
        return -1;

    if (listartsWith(t, "*", 0, -1) && liendsWith2(t, "*", 0, -1)) {
        if (liexistin3(s, t, fr, to, 1, l2 - 1)) {
            re = 1;
        }
    } else if (listartsWith(t, "*", 0, -1)) {
        if (liendsWith3(s, t, 0, -1, 1, -1)) {
            re = 1;
        }
    } else if (liendsWith2(t, "*", 0, -1)) {
        if (listartsWith3(s, t, 0, -1, 0, l2 - 1)) {
            re = 1;
        }
    } else {
        if (liscmp2(s, t, fr, to) == 0) {
            re = 1;
        }
    }
    return re;
}
//void litest33(char* s){
//	int re=liendsWith3(s, "*.dll", 0, -1, 1, -1);
//	livlog("%d\n",re
//			);
//}
/* get position in s where t first appear.
 * return first letter index */
int listrstr(char *s, char *t, int fr, int to) {
    int re = -1, i, l1 = listrlen(s), l2 = listrlen(t);
    if (to == -1)
        to = l1;
    if (l1 < 1 || l2 < 1 || fr < 0 || fr >= to || to > l1)
        return -1;
    i = fr;
    while (to - i >= l2) {
        if (listartsWith(s + i, t, 0, -1)) {
            re = i;
            break;
        } else {
            i++;
        }
    }
    return re;
}

/* get position in s where t first appear.
 * return first letter index */
int listrstrInv(char *s, char *t, int fr, int to) {
    int re = -1, i, l1 = listrlen(s), l2 = listrlen(t);
    if (to == -1)
        to = l1;
    if (l1 < 1 || l2 < 1 || fr < 0 || fr >= to || to > l1)
        return -1;
    i = to;
    while (i - fr >= l2) {
        if (liendsWith2(s, t, fr, i)) {
            re = i - l2;
            break;
        } else {
            i--;
        }
    }
    return re;
}

int liisNull(char *s) {
    return !s || !liscmp(s, "null");
}

int licountin(char *s, char *t, int fr, int to) {
    int re = 0, i, l1 = listrlen(s), l2 = listrlen(t);
    if (to == -1)
        to = l1;
    if (l1 < 1 || l2 < 1 || fr < 0 || fr >= to || to > l1)
        return -1;
    i = fr;
    while (to - i >= l2) {
        if (listartsWith(s + i, t, 0, -1)) {
            re++;
            i = i + l2;
        } else {
            i++;
        }
    }
    return re;
}

// nn will be set to zero. isKept means weather or not *t is kept.
char **lisplitby2N(const char *s, char *t, int fr, int to, int *nn, int isKept) {

    int i, j, jj, k, bo = 0, l1 = listrlen(s), l2 = listrlen(t);
    int maxl = 4, left = 0;
    (*nn) = 0;
    char **ss = malloc(maxl * sizeof *ss);
    if (ss == 0)
        return 0;
    left = maxl;
    if (to == -1)
        to = l1;
    if (l1 < 1 || l2 < 1 || fr < 0 || fr >= to || to > l1)
        return 0;
    i = fr;
    j = i;
    k = i;
    while (to - i >= 0) {
        if (listartsWith(s + i, t, 0, -1)) {
            bo = 1;
            i = i + l2;
            if (isKept)
                k = i;
            continue;
        }
        if (to - i < l2)
            i = to;
        if (bo || i == to) {
            if (j != k) {
                if (left == 0) {
                    char **ssn = realloc(ss, (maxl *= 2) * sizeof *ss);
                    if (ssn == 0) {
                        lifreess(ss, maxl / 2);
                        return 0;
                    }
                    ss = ssn;
                    left = maxl / 2;
                }
                char *sn = malloc(k - j + 1);
                if (sn == 0) {
                    lifreess(ss, maxl);
                    return 0;
                }
                for (jj = 0; jj < k - j; jj++) {
                    sn[jj] = s[j + jj];
                }
                sn[k - j] = 0;
                ss[*nn] = sn;
                (*nn)++;
                left--;
            }
            j = i;
            k = i;
            bo = 0;

        }
        i++;
        k = i;
    }
    return ss;
}

int lisubstitutebyN(char *s, char *t, char *tn, int fr, int to, char **sn) {
    int re = 1;
    int sl = listrlen(s);
    int tl = listrlen(t);
    int tnl = listrlen(tn);
    if (to == -1)
        to = sl;
    if (sl <= 0 || tl <= 0 || tnl <= 0 || fr < 0 || to > sl || fr >= to) {
        re = 0;
        goto label;
    }
    int i;
    for (i = fr; i < to;) {
        if (listartsWith(s, t, i, to)) {
            lisaddN(sn, tn);
            i += tl;
        } else {
            char stmp[2] = {0, 0};
            stmp[0] = s[i];
            lisaddN(sn, stmp);
            i++;
        }
    }
    label:
    return re;
}

void lisubstitutebyNtest() {
    char *s = "LilyBeauty";
    char *t = "ea";
    char *tn = "uuii";
    char *sn = 0;
    lisubstitutebyN(s, t, tn, 3, -1, &sn);
    livlog("%s", sn);
    lifree(sn);
}

int lireadStream2(FILE *fpt) {
    if (!fpt) {
        return 0;
    }
    int maxlenp = 1024;
    char *line = malloc(maxlenp);
    if (!line)
        return 0;

    while (xjreadline(fpt, &line, &maxlenp)) {
        lilog(line);
    }
    free(line);
    return 1;
}

int licat(char *s) {
    FILE *fpt = fopen(s, "r");
    if (!fpt) {
        return 0;
    }
    int re = lireadStream2(fpt);
    fclose(fpt);
    return re;
}

void litestlistring() {
}

int listrlen(const char *s) {
    int i = 0;
    if (!s)
        return -1;
    while (s[i] != '\0')
        i++;
    return i;
}

int listrcpy(char *u, char *t, int n) {

    if (listrlen(u) < n || listrlen(t) < n)
        return -1;
    memcpy(u, t, n);
    u[n] = 0;
    return 1;
}

char *listrdupN(char *u) {
    return lisubstringN(u, 0, -1);
}

int lifileNameToPath(char *name, char *par, char *path, int pathSize) {
    int len;
    if (liendsWith(par, liunixSeparator)) {
        len = snprintf(path, pathSize, "%s%s", par, name);
    } else {
        len = snprintf(path, pathSize, "%s%s%s", par, liunixSeparator, name);
    }
    if (len < 0)
        return 0;
    else
        return 1;

}

char *liscat2N(char *u, char *v) {
    if (!u || !v)
        return 0;
    char *s = malloc(1 + listrlen(u) + listrlen(v));
    if (!s)
        return s;
    strcpy(s, u);
    strcat(s, v);
    return s;
}

int lisaddN(char **ss, char *v) {
    if (!ss || !v)
        return 0;
    int vl = listrlen(v);

    int sl = listrlen(*ss);
    char *tmp;
    if (*ss) {
        tmp = realloc(*ss, 1 + sl + vl);
    } else {
        sl = 0;
        tmp = malloc(1 + vl);
    }
    if (!tmp)
        return 0;
    *ss = tmp;
    memcpy((*ss) + sl, v, vl + 1);
    return 1;
}

char *liscat3N(const char *s1, const char *s2, const char *s3) {
    if (!s1 || !s2 || !s3)
        return 0;
    char *s = malloc(listrlen(s1) + listrlen(s2) + listrlen(s3) + 1);
    if (!s)
        return s;
    memcpy(s, s1, listrlen(s1));
    memcpy(s + listrlen(s1), s2, listrlen(s2));
    memcpy(s + listrlen(s1) + listrlen(s2), s3, listrlen(s3) + 1);
    return s;
}

char *liscat4N(char *s1, char *s2, char *s3, char *s4) {
    if (!s1 || !s2 || !s3 || !s4)
        return 0;
    char *s = malloc(
            listrlen(s1) + listrlen(s2) + listrlen(s3) + listrlen(s4) + 1);
    if (!s)
        return s;
    memcpy(s, s1, listrlen(s1));
    memcpy(s + listrlen(s1), s2, listrlen(s2));
    memcpy(s + listrlen(s1) + listrlen(s2), s3, listrlen(s3));
    memcpy(s + listrlen(s1) + listrlen(s2) + listrlen(s3), s4,
           listrlen(s4) + 1);
    return s;
}

char *liscatppN(char **v, int n) {
    if (!v || n < 1)
        return 0;
    int i, l = 0;
    for (i = 0; i < n; i++) {
        l += listrlen(v[i]);
    }
    char *s = malloc(1 + l);
    if (!s)
        return s;
    strcpy(s, v[0]);
    for (i = 1; i < n; i++)
        strcat(s, v[i]);
    return s;
}

char *liremovecharN(char *input, char c) {
    if (!input)
        return 0;
    int i, j, le = listrlen(input);
    char *output = malloc(le + 1);
    for (i = 0, j = 0; i < le; i++, j++) {
        if (input[i] != c)
            output[j] = input[i];
        else
            j--;
    }
    output[j] = 0;
    return output;
}

//// k<0 means finding from index 0;
int lifindFirstBetween(char *input, char c, int fr, int to) {
    if (fr < -1)
        fr = -1;
    int sl = listrlen(input);
    if (to == -1)
        to = sl;
    if (fr >= to || to > sl || fr < 0)
        return -1;
    int i;
    for (i = fr + 1; i < to; i++) {
        if (input[i] == c)
            break;
    }
    if (i == to)
        i = -1;
    return i;
}

int lifindFirstBeforeInv(char *input, char c, int k) {
    int l = listrlen(input);
    if (k > l)
        k = l;
    int i;
    for (i = k - 1; i > -1; i--) {
        if (input[i] == c)
            break;
    }
    return i;
}

int licountby(char *input, char c) {
    return licountby2(input, c, 0, listrlen(input));
}

int licountby2(char *input, char c, int from, int to) {
    int i, j = 0;
    int l = listrlen(input);
    if (from >= to || to > l || from < 0)
        return 0;
    for (i = from; i < to; i++) {
        if (input[i] == c)
            j++;
    }
    return j;
}

//liLog("%d %d %d",sizeof(char),sizeof(char*),sizeof(char**));
//1 4 4
// split input by c into *nn pieces where c will be kept or not.
char **lisplitbyN(char *input, char c, int fr, int to, int *nn) {
    int i, j = fr - 1, sl = listrlen(input);
    if (to == -1)
        to = sl;
    if (fr >= to || to > sl || fr < 0)
        return 0;
    *nn = licountby2(input, c, fr, to);
    if (input[sl - 1] != c)
        (*nn)++;
    char **sa = malloc(*nn * sizeof *sa);
    if (!sa)
        return 0;
    for (i = 0; i < *nn; i++) {
//		if(!iskept)lifindFirstNot()
        int ii, jj = j;
        j = lifindFirstBetween(input, c, j + 1, to);
        if (j == -1)
            j = to - 1;
        sa[i] = malloc(j - jj + 1);
        if (!sa[i])
            return 0;
        for (ii = 0; ii < j - jj; ii++) {
            sa[i][ii] = input[jj + ii + 1];
        }
        sa[i][ii] = 0;
    }
    return sa;
}

//char** lisplitby(char* s, char* mid, int fr, int to, int* ssl){
//
//}
//'for' loop initial declarations are only allowed in C99 or C11 mode
void lifreess(char **ss, int ssl) {
    int j;
    if (!ss)
        return;
    for (j = 0; j < ssl; j++) {
        free(ss[j]);
    }
    free(ss);
}

void lifree(char *s) {
    if (s)
        free(s);
}

void lifree2(void **s, int zero) {
    free(*s);
    *s = 0;
}

void lifreess3(void **vv, int ssl) {
    int j;
    char **ss = *vv;

    if (!ss)
        return;
    for (j = 0; j < ssl; j++) {
        if (!ss[j])break;
        free(ss[j]);
        ss[j] = 0;
    }
    free(*vv);
    *vv = 0;
}

int lirename(char *par, char *sold, char *snew, FILE *fp) {
    char *name1 = malloc(1 + 1 + listrlen(par) + listrlen(sold));
    strcpy(name1, par);
    strcat(name1, "/");
    strcat(name1, sold);

    char *name2 = malloc(1 + 1 + listrlen(par) + listrlen(snew));
    strcpy(name2, par);
    strcat(name2, "/");
    strcat(name2, snew);
    int result = rename(name1, name2);

    if (result != 0) {
        if (debug)
            livlog("LiError: unable to rename the file: %s\n", name1);
    } else if (fp) {
        fprintf(fp, "%s\n%s\n", name1, name2);
    }
    free(name1);
    free(name2);
    return result;
}

int lirenameSuffix(char *par, char *name, char *sold, char *snew, FILE *fp) {
    if (!liendsWith(name, sold) || !liscmp(sold, snew))
        return 0;
    char newName[listrlen(name) - listrlen(sold) + listrlen(snew) + 1];
    memcpy(newName, name, listrlen(name) - listrlen(sold));
    memcpy(newName + listrlen(name) - listrlen(sold), snew, listrlen(snew) + 1);
    return lirename(par, name, newName, fp);
}

int lirenameSuffixes(char *par, char *sold, char *snew, int isRec, FILE *fp) {
    if (fp) {
        liauthorlog(fp, "Odd line is old name. Even line is new name.");
    }
    return lirenameSuffixesRec(par, sold, snew, isRec, fp);
}

// recursively rename file suffix, isRec==0 means no recursion.
int lirenameSuffixesRec(char *par, char *sold, char *snew, int isRec, FILE *fp) {
    int fail = 0;
    DIR *d, *d2;
    struct dirent *dir;
    if (par == NULL)
        par = ".";
    d = opendir(par);
    if (d) {
        while ((dir = readdir(d)) != NULL) {

            if (!liscmp(dir->d_name, ".") || !liscmp(dir->d_name, ".."))
                continue;
            char *subPar = liscat3N(par, "/", dir->d_name);
            d2 = opendir(subPar);
            if (d2) {
                closedir(d2);
                if (isRec)
                    fail += lirenameSuffixesRec(subPar, sold, snew, isRec, fp);
            } else {
                if (lirenameSuffix(par, dir->d_name, sold, snew, fp) != 0)
                    fail++;
            }
            free(subPar);

        }
        closedir(d);
    }
    return fail;
}

// s1 is old name which will be restored.
// odd line is old name. even line is new name.
int lirestoreNames(FILE *fp) {
    char *s1, *s2;
    int maxlen1 = 8, maxlen2 = 8;
    s1 = malloc(maxlen1);
    s2 = malloc(maxlen2);
    if (!s1 || !s1)
        return 0;
    xjreadline(fp, &s1, &maxlen1);
    xjreadline(fp, &s2, &maxlen2);
    while (xjreadline(fp, &s1, &maxlen1) && xjreadline(fp, &s2, &maxlen2)) {
        rename(s2, s1);
    }

    if (s1)
        free(s1);
    if (s2)
        free(s2);
    return 1;
}


/* not getline in gnu extension, will write one later.
 * read line not including newline,
 * may return empty string
 * char *line = NULL;
 * int max_line_len = 1024;
 * line = malloc(max_line_len);
 * return 1 means successful, 0 means fail;
 * line is the address givn by malloc. */
int xjreadline(FILE *input, char **line, int *maxlenp) {
    int len;
    if (fgets(*line, *maxlenp, input) == NULL)
        return 0;

    while (1) {
        len = listrlen(*line);
        if (len > 0 && (*line)[len - 1] == '\n') {
            if (len > 1 && (*line)[len - 2] == 13) {
                (*line)[len - 2] = '\0';
                break;
            }
            (*line)[len - 1] = '\0';
            break;
        }

        *maxlenp *= 2;
        char *linen = realloc(*line, *maxlenp);
        if (!linen) {
            return 0;
        }
        *line = linen;
        len = *maxlenp / 2 - 1;
        if (fgets(*line + len, *maxlenp - len, input) == NULL)
            break;
    }
    return 1;
}

void liauthorlog(FILE *fp, char *s) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    if (!fp)
        livlog("Author by LilyBeauty at: %s", asctime(timeinfo));
    else
        fprintf(fp, "%s\n"
                    "Author by LilyBeauty at: %s", s, asctime(timeinfo));
}

// return length, return -1 when in error.
int lireadline(FILE *fp, void (*func)(char *)) {
    size_t lenmax = 128, left = lenmax;
    char *line = malloc(lenmax), *linep = line;
    int c;

    if (line == NULL)
        return -1;

    for (;;) {
        c = fgetc(fp);
        if (c == EOF) {
            if (lenmax - left == 0) {
                free(linep);
                return -1;
            } else {
                break;
            }
        } else if ((*line++ = c) == '\n') {
            line--;
            break;
        } else if (--left == 0) {
            left = lenmax;
            char *linen = realloc(linep, lenmax *= 2);

            if (linen == NULL) {
                free(linep);
                return -1;
            }
            line = linen + (line - linep);
            linep = linen;
        }

    }
    *line = '\0';
    if (func)
        func(linep);
    free(linep);
    return lenmax - left;
}

char *lireadlineN(FILE *fp) {
    size_t addrl = 128;
    char *addrp = malloc(addrl);
    int usel = 0;
    int c;

    if (addrp == NULL)
        return 0;

    for (;;) {
        c = fgetc(fp);
        if (c == EOF) {
            if (usel == 0) {
                free(addrp);
                return 0;
            } else {
                break;
            }
        } else if ((*(addrp + usel++) = c) == '\n') {
            usel--;
            break;
        } else if (usel == addrl) {
            char *addrtmp = realloc(addrp, addrl *= 2);

            if (addrtmp == NULL) {
                free(addrp);
                return 0;
            }
            addrp = addrtmp;
            usel = addrl / 2;
        }

    }
    *(addrp + usel++) = '\0';
    if (debug)
        fprintf(stderr, "%s\n", addrp);
    return addrp;
}

/*read all include space, '\n', word, '.', ect.*/
int lireadEngWordAll(FILE *fp, void *vp) {
    char *word = *(char **) vp;
    int c, i = 0;
    while (1) {
        c = fgetc(fp);
        if (c == EOF) {
            if (i == 0) {
                return 0;
            } else {
                break;
            }
        }

        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            word[i++] = c;
        } else {
            if (i == 0)
                word[i++] = c;
            else
                ungetc(c, fp);
            break;
        }
    }
    word[i] = 0;
    return 1;
}

int xjstreamsize(FILE *fp) {
    int prev = ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp);
    fseek(fp, prev, SEEK_SET); //go back to where we were
    return sz;
}

int liwriteText(char *s, char *text) {
    FILE *fpt = fopen(s, "w");
    if (!fpt) {
        return 0;
    }
    fputs(text, fpt);
    fclose(fpt);
    return 1;
}

int liwriteStream(FILE *fpt, char *text) {
    if (!fpt) {
        return 0;
    }
    fputs(text, fpt);
    return 1;
}

int lireadText(char *s, void (*f)(char *)) {
    FILE *fpt = fopen(s, "r");
    if (!fpt) {
        return 0;
    }
    int re = lireadStream(fpt, f);
    fclose(fpt);
    return re;
}

int lireadStream(FILE *fpt, void (*f)(char *)) {
    if (!fpt) {
        return 0;
    }
    int maxlenp = 1024;
    char *line = malloc(maxlenp);
    if (!line)
        return 0;

    while (xjreadline(fpt, &line, &maxlenp)) {
        f(line);
    }
    free(line);
    return 1;
}

void lis2ascii(char *s) {
    int i = 0;
    if (!s || !s[i])
        return;
    livlog("%s length:%d\n", s, listrlen(s));
    lilog("its ascii is:");
    do {
        int t = *(s + i);
        t = t & 0x000000ff;
        livlog(" %d ", t);
        i++;
    } while (*(s + i));
//	livlog("%c%c%c", 229, 165, 189);
    lilog("\n");
    fflush(stdout);
}

int liscanf(const char *fmt, ...) {
    int converted = 0;
    va_list args;

    va_start(args, fmt);

    while (*fmt) {
        if (*fmt++ == '%') {
            if (*fmt == 'd') {
                /* Parse and convert an integer */
                char buf[BUFSIZ];
                int i = 0, ch;
                char *end;

                /* Read up to whitespace */
                while ((ch = getchar()) != EOF && ch != ' ')
                    buf[i++] = (char) ch;

                if (i == 0)
                    break; /* No valid characters */

                /* Attempt the conversion */
                *va_arg(args, int*) = (int) strtol(buf, &end, 0);

                if (*end)
                    break; /* A complete conversion failed */

                ++converted;
            } else {
                break;
            }
        }
    }

    va_end(args);

    return converted ? converted : EOF;
}

void liungets(char *s, FILE *f) {
    int i;

    i = listrlen(s);

    if (i > 0) {
        ungetc('\n', f);
    }
    while (i > 0)
        ungetc(s[--i], f);
}

