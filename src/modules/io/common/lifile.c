/*
 * lifile.c
 *
 *  Created on: 2016.11.16
 *      Author: Yun
 */

#include "liCommonInner.h"
//#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include "lifile.h"

int liisTerminal;
int saved_stdin;
int saved_stdout;

void litestwriteIntAtPos() {
    FILE *fpt = fopen("input", "w");
    if (!fpt) {
        fclose(fpt);
        return;
    }
    liwriteStream(fpt, "yo  lsaeni");
    liwriteIntAtPos(2, 33, fpt, 0);
    liwriteStream(fpt, "yolsaeni");
    fclose(fpt);
}

void litest2tos(int bitIgnored) {
    FILE *fps = fopen("output", "r");
    if (!fps) {
        fclose(fps);
        return;
    }
    FILE *fpt = fopen("input2", "w");
    if (!fpt) {
        fclose(fpt);
        return;
    }
    li2tos(fps, fpt, bitIgnored);
}

void liteststo2() {
    FILE *fps = fopen("input", "r");
    if (!fps) {
        fclose(fps);
        return;
    }
    FILE *fpt = fopen("output", "w");
    if (!fpt) {
        fclose(fpt);
        return;
    }

    int bitAdded = 0;
    listo2(fps, fpt, &bitAdded);

}

void listo2(FILE *fps, FILE *fpt, int *bitAdded) {

    char c;
    int n = 0;
    while (_listo2(fps, &c, &n)) {
        fprintf(fpt, "%c", c);
    }
    if (n > 0) {
        fprintf(fpt, "%c", c);
        *bitAdded = 8 - n;
    }

    fprintf(stderr, "bitAdded:%d\n", *bitAdded);

}

int li2tos(FILE *fs, FILE *ft, int bitIgnored) {
    char bits[8];
    int c2 = fgetc(fs);
    if (c2 == EOF)
        return 0;
    while (1) {
        licharto2(c2, bits);
        c2 = fgetc(fs);
        if (c2 == EOF) {
            fwrite(bits, sizeof(char), 8 - bitIgnored, ft);
            fprintf(stderr, "bitIgnored:%d\n", bitIgnored);
            return 0;
        } else
            fwrite(bits, sizeof(char), 8, ft);
    }
    return 1;
}

/*read 01 string, write to char. */
int _listo2(FILE *fs, char *bitBufp, int *np) {
    int c2;
    int i = 0, j;
    while (i < 8) {
        c2 = fgetc(fs);
        if (c2 == EOF) {
            j = *np;
            if (i == 0) {
                *np = 0;
                return 0;
            }
            while (i < 8) {
                liWriteBit(0, bitBufp, &j);
                i++;
            }
            return 0;
        }
        liWriteBit(c2 - '0', bitBufp, np);
        i++;
    }
    return 1;
}

int liwriteIntAtPos(int pos, int i, FILE *fp, int isBinary) {
    int prev = ftell(fp);
    fseek(fp, pos, SEEK_SET);

    if (isBinary)
        fwrite(&i, sizeof(int), 1, fp);
    else
        fprintf(fp, "%d", i);
    fseek(fp, prev, SEEK_SET);
//	else {
//		if(isBinary)fseek(fp, prev + sizeof(int), SEEK_SET);
//		else fseek(fp, prev + snprintf( NULL, 0, "%d", i), SEEK_SET);
//	}
    return 1;
}

int liredirect(int argc, char **argv, char *lilibName) {
    char *f;
    if ((f = lifindArgRedirectN((char *) "<", argc, argv)) != 0) {

        char *path = liscat3N(liappDir(), "/", lilibName);
        liensureFilePathFN(path, &f);
        lifree(path);
        if (f) {
            freopen(f, "r", stdin);
            free(f);
        }
    } else {

    }
    if ((f = lifindArgRedirectN((char *) ">", argc, argv)) != 0) {
        //    	liLog("\n>\n");
        char *path = liscat3N(liappDir(), "/", lilibName);
        liensureFilePathFN(path, &f);
        lifree(path);
        if (f) {
            freopen(f, "w", stdout);
            liisTerminal = 0;
            free(f);
        }
    } else {
        liisTerminal = 1;
    }
    return 1;
}

/* null lib is ok */
void liplugLib(int argc, char **argv, char *lilibName) {
#ifdef MATH21_WIN_MSVC
    assert(0 && "NOT SUPPORTED WHEN MSVS!");
#else
    char *path = liscat3N(liappDir(), "/", lilibName);
    limkdirs(path);
    lifree(path);
    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    liredirect(argc, argv, lilibName);
#endif
}

void liunplugLib() {
#ifdef MATH21_WIN_MSVC
    assert(0 && "NOT SUPPORTED WHEN MSVS!");
#else
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
#endif
}

int ligetIsTerminal() {
    return liisTerminal;
}

void lipwd() {
#ifdef MATH21_WIN_MSVC
    assert(0 && "NOT SUPPORTED WHEN MSVS!");
#else
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    livlog("%s\n", cwd);
#endif
}

/*must set sn to 0, or might crash*/
FILE *lifopen(char *name, const char *s, const char *t) {
    char *sn = 0;
    FILE *f;
    if (name) {
        char *path = liscat3N(liappDir(), "/", name);
        limkdirs(path);
        sn = liensureFilePathN(path, (char *) s);
        lifree(path);
        if (sn) {
//			lilog(sn);
            f = fopen(sn, t);
        } else
            return 0;
    } else {
        f = fopen(s, t);
    }
    lifree(sn);
    return f;
}

/*must set sn to 0, or might crash*/
FILE *liHomefopen(char *name, const char *s, const char *t) {
    char *sn = 0;
    FILE *f;
    if (name) {
        char *path = liscat3N(liHomeDir(), "/", name);
        limkdirs(path);
        sn = liensureFilePathN(path, s);
        lifree(path);
        if (sn) {
//			lilog(sn);
            f = fopen(sn, t);
        } else
            return 0;
    } else {
        f = fopen(s, t);
    }
    lifree(sn);
    return f;
}

int lifclose(FILE **pp) {
    int re = 1;
    if (*pp)
        re = fclose(*pp);
    *pp = 0;
    return re;
}

char *lifilePar2N(char *s) {
    if (s == 0)
        return 0;
    int po = listrstrInv(s, "/", 0, -1);
    if (po == -1)
        return 0;
    return lisubstringN(s, 0, po);
}

int lifileParN(char *s, char **parp, int *np) {
    if (s == 0)
        return 0;
    int po = listrstrInv(s, "/", 0, -1);
    if (po == -1)
        return 0;
    return lisubstring(s, parp, np, 0, po);
}

int lifilePar4N(char *s, char **parp) {
    int n = 0;
    if (s == 0)
        return 0;
    int po = listrstrInv(s, "/", 0, -1);
    if (po == -1) {
        po = listrstrInv(s, "\\", 0, -1);
        if (po == -1)
            return 0;
    }
    return lisubstring(s, parp, &n, 0, po);
}

int lifileName4N(char *s, char **name) {
    int n = 0;
    if (s == 0)
        return 0;
    int po = listrstrInv(s, "/", 0, -1);
    if (po == -1) {
        po = listrstrInv(s, "\\", 0, -1);
        if (po == -1)
            return 0;
    }
    return lisubstring(s, name, &n, po + 1, -1);
}

int lifileNameNoSuf4N(char *s, char **name) {
    int n = 0;
    if (s == 0)
        return 0;
    int po = listrstrInv(s, ".", 0, -1);
    if (po == -1) {
        return 0;
    }
    return lisubstring(s, name, &n, 0, po);
}

int lifileNameSuf4N(char *s, char **name) {
    int n = 0;
    if (s == 0)
        return 0;
    int po = listrstrInv(s, ".", 0, -1);
    if (po == -1) {
        return 0;
    }
    return lisubstring(s, name, &n, po + 1, -1);
}

int lifileNameN(char *s, char **name, int *np) {
    if (s == 0)
        return 0;
    int po = listrstrInv(s, "/", 0, -1);
    if (po == -1)
        return 0;
    return lisubstring(s, name, np, po + 1, -1);
}

int liremove(char *name, const char *s) {
    int re;
    char *sn = 0;
    if (name && liscmp(name, "null") != 0) {
        char *path = liscat3N(liappDir(), "/", name);
        sn = liensureFilePathN(path, (char *) s);
        lifree(path);
        if (sn)
            re = remove(sn);
        else
            re = -1;
    } else {
        re = remove(s);
    }
    lifree(sn);
    return re;
}

int lisearchFilesN(void **pp, const char *par, char **patternpp, int patternNum,
                   int *np, int *usep, int level, int recTimes,
                   void (*f)(void *, const char *, ...), void *finput) {
#ifdef MATH21_WIN_MSVC
    assert(0 && "NOT SUPPORTED WHEN MSVS!");
    return 0;
#else

    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    int re = 1, re2;

    if (!liensurevvN(pp, np, usep, 8, sizeof(char *), lifreess3))
        return 0;

    if (!(dir = opendir(par))) {
        re = 0;
        return re;
    }
    if (!(entry = readdir(dir))) {
        re = 1;
        return re;
    }

    do {
        int l3 = listrlen(par) + listrlen(entry->d_name) + 1;
        char path[l3 + 1];
        int len = snprintf(path, sizeof(path), "%s%s%s", par, liunixSeparator,
                           entry->d_name);
        if (len < 0)
            return 0;

        if (stat(path, &statbuf) == -1) {
            if (liisDebug())
                perror(path);
            continue;
        }
        if (S_ISDIR(statbuf.st_mode))
//		if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0
                || strcmp(entry->d_name, "..") == 0)
                continue;

            if (recTimes != 0) {
                if (f) {
                    if (finput)
                        f(finput, "%s\n", path);
                    else
                        f(finput, "%*s[%s]\n", level * 2, "", entry->d_name);
                }

                re2 = lisearchFilesN(pp, path, patternpp, patternNum, np, usep,
                                     level + 1, recTimes - 1, f, finput);
                if (!re2)
                    return 0;
            }
        } else {

            if (!patternpp
                || liregex2(entry->d_name, patternpp, patternNum, 0, -1)
                   == 1) {
                char *sn = liscat3N(par, liunixSeparator, entry->d_name);
                if (!sn)
                    return 0;

                if (!liensurevvN(pp, np, usep, *np, sizeof(char *), lifreess3))
                    return 0;

                *(char **) (*pp + sizeof(char *) * ((*usep)++)) = sn;

                if (f) {
                    if (finput)
                        f(finput, "%s\n", sn);
                    else
                        f(finput, "%*s- %s\n", level * 2, "", entry->d_name);
                }

            }

        }
    } while ((entry = readdir(dir)));
    closedir(dir);

    return re;
#endif
}

int lisearchSatisfy(char *s, char **patternpp, int patternNum, int fr, int to) {
    if (!patternpp || liregex2(s, patternpp, patternNum, fr, to) == 1)
        return 1;
    else
        return 0;
}

/*********************************************
 * @D: here use 8
 ********************************************/
int lisearchTackle(void **pp, int *np, int *usep, const char *par, char *entry,
                   int isDir) {
    char *sn = liscat3N(par, liunixSeparator, entry);
    if (!sn)
        return 0;

    if (!liensurevvN(pp, np, usep, 8, sizeof(char *), lifreess3))
        return 0;
    *(char **) ((char *) (*pp) + sizeof(char *) * ((*usep)++)) = sn;
    return 1;
}

/*********************************************
 * @D: here use 8
 ********************************************/
// deprecate?
int lisearchTackle2(void **pp, int *np, int *usep, const char *par, char *entry,
                    int isDir) {
    char *sn = liscat3N(par, liunixSeparator, entry);
    if (!sn)
        return 0;
    if (pp) {
        if (!liensurevvN(pp, np, usep, 8, sizeof(char *), lifreess3))
            return 0;
        *(char **) ((char *) (*pp) + sizeof(char *) * ((*usep)++)) = sn;
    }
    if (isDir)
        livlogColor(0xff0000, "%*s%s\n", 0, "", entry);
    else
        livlog("%*s%s\n", 0, "", entry);
    return 1;
}

int lisearchTackle21_print(void *container, const char *par, char *entry,
                           int isDir) {

    if (container) {
    }
    if (isDir)
        livlogColor(0xff0000, "%*s%s\n", 0, "", entry);
    else
        livlog("%*s%s\n", 0, "", entry);
    return 1;
}

int liremoveTackle(void **pp, int *np, int *usep, const char *par, char *entry,
                   int isDir) {
    char *sn = liscat3N(par, liunixSeparator, entry);
    if (!sn)
        return 0;
    if (pp) {
        if (!liensurevvN(pp, np, usep, 8, sizeof(char *), lifreess3))
            return 0;
        *(char **) ((char *) (*pp) + sizeof(char *) * ((*usep)++)) = sn;
    }
    if (isDir)
        lirmdir(sn);
    else
        remove(sn);
    return 1;
}

int liremove3(char *par, char **pattern, int patternNum) {
    char **pp = 0;
    int use = 0;

//	char* patternpp[] = { "*.dll", "*.a" };
    int n = 0;

//	int re= lisearchAndTackle((void**) &pp, par, patternp, 1, &n, &use,2, 1, 1, 1, 0, lisearchSatisfy, lisearchTackle);
    int re = _litackle_frame((void **) &pp, par, pattern, patternNum, &n, &use,
                             -1, 1, 1, lisearchSatisfy, liremoveTackle);
    liprintfss(pp, use);

    lifreess3((void **) &pp, use);
    return re;
}

/*can't remove file whose name is 클래식 (영화) in GBK windows.
 * 'cause the name won't be able to be represented using GBK.
 * We can remove it in Linux, 'cause using UTF-8.*/
int _lirm_pat(char *patPath, int recTimes, int tf, int td) {
    char **pp = 0;
    int use = 0;
    char *par = 0;
    char *pat = 0;
    int patternNum = 1;
    int n = 0;
    int output = LI_SUCCESS;
    if (lifilePar4N(patPath, &par) == LI_FAILURE) {
        par = ".";
        pat = listrdupN(patPath);
    } else if (lifileName4N(patPath, &pat) == LI_FAILURE) {
        output = LI_FAILURE;
        goto _error;
    }
    output = _litackle_frame(0, par, &pat, patternNum, &n, &use, recTimes, tf,
                             td, lisearchSatisfy, liremoveTackle);
//	output = _litackle_frame((void**) &pp, par, &pat, patternNum, &n, &use,
//			recTimes, tf, td, lisearchSatisfy, liremoveTackle);
//	liprintfss(pp, use);

    _error:
    lifree(par);
    lifree(pat);
    lifreess3((void **) &pp, use);
    return output;
}

int lirmdir(char *par) {
    char **pp = 0;
    int use = 0;
    int n = 0;
    int output = LI_SUCCESS;

    output = _litackle_frame((void **) &pp, par, 0, 0, &n, &use, -1, 1, 1,
                             lisearchSatisfy, liremoveTackle);
//		liprintfss(pp, use);
    rmdir(par);
    lifreess3((void **) &pp, use);
    return output;
}

/**-r is to remove directories and their contents recursively
 * means we tackle directory.
 * here we set td=1 if appearing -r
 * Usage: use pos_max to get unspecified option value,
 * and compare option size count and pos_max argv count to
 * check wrong options.
 * Note: recTime is 0 which is different from search.
 * More specifically, "search -r . u.c"
 * is different from "remove -r . u.c"
 * where for searching, -r means recursively,
 * but for removing, -r means tackle directory..**/
int lirm(int argc, char **argv) {
    int isfound;
    int pos = 0, pos_max = 0;
    int i;
    int recTimes = 0, tf = 1, td = 0;
    if (argc == 2 && liscmp(argv[1], "--help") == 0) {
        _liremoveUsage();
        return LI_FAILURE;
    }
    isfound = lipickOption2(argc, argv, "f", 0, &pos, liisStrExistin);
    if (isfound == 1) {
        if (pos > pos_max)
            pos_max = pos;
    }
    if (isfound == -1)
        return LI_FAILURE;
    isfound = lipickOption2(argc, argv, "r", 0, &pos, liisStrExistin);
    if (isfound == 1) {
        td = 1;
        if (pos > pos_max)
            pos_max = pos;
    }
    if (isfound == -1)
        return LI_FAILURE;
    // check for unknown option call
    if (pos_max == argc - 1) {
        livlogColor(0x00ff00,
                    "rm: missing operand\nTry 'rm --help' for more information.\n");
        return LI_FAILURE;
    }
    for (i = pos_max + 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            livlogColor(0xff0000, "Fatal error: option \"-%s\" is wrong.\n",
                        argv[i] + 1);
            _liremoveUsage();
            return LI_FAILURE;
        }
    }
    // check for input image
    for (i = pos_max + 1; i < argc; i++) {
        if (_lirm_pat(argv[i], recTimes, tf, td) != LI_SUCCESS) {
            return LI_FAILURE;
        }
    }
    return LI_SUCCESS;
}

void _liremoveUsage() {
    puts(
            "Usage: rm [OPTION]... FILE...\n\
	Remove (unlink) the FILE(s).\n\
\n\
	  -f, --force           ignore nonexistent files, never prompt\n\
	  -i                    prompt before every removal\n\
	  -I                    prompt once before removing more than three files, or\n\
	                          when removing recursively.  Less intrusive than -i,\n\
	                          while still giving protection against most mistakes\n\
	      --interactive[=WHEN]  prompt according to WHEN: never, once (-I), or\n\
	                          always (-i).  Without WHEN, prompt always\n\
	      --one-file-system  when removing a hierarchy recursively, skip any\n\
	                          directory that is on a file system different from\n\
	                          that of the corresponding command line argument\n\
	      --no-preserve-root  do not treat `/' specially\n\
	      --preserve-root   do not remove `/' (default)\n\
	  -r, -R, --recursive   remove directories and their contents recursively\n\
	  -v, --verbose         explain what is being done\n\
	      --help     display this help and exit\n\
	      --version  output version information and exit\n\
\n\
	By default, rm does not remove directories.  Use the --recursive (-r or -R)\n\
	option to remove each listed directory, too, along with all of its contents.\n\
\n\
	To remove a file whose name starts with a `-', for example `-foo',\n\
	use one of these commands:\n\
	  rm -- -foo\n\
\n\
	  rm ./-foo\n\
\n\
	Note that if you use rm to remove a file, it might be possible to recover\n\
	some of its contents, given sufficient expertise and/or time.  For greater\n\
	assurance that the contents are truly unrecoverable, consider using shred.\n\
\n\
	Report rm bugs to bug-coreutils@gnu.org\n\
	GNU coreutils home page: <http://www.gnu.org/software/coreutils/>\n\
	General help using GNU software: <http://www.gnu.org/gethelp/>\n\
	For complete documentation, run: info coreutils 'rm invocation'\n");
}

// same as lisearchAndTackle
int _litackle_frame(void **pp, const char *par, char **patternpp, int patternNum,
                    int *np, int *usep, int recTimes, int tf, int td,
                    int (*isSatisfied)(char *s, char **patternpp, int patternNum, int fr,
                                       int to),
                    int (*tackle)(void **pp, int *np, int *usep, const char *par, char *entry,
                                  int isDir)) {
#ifdef MATH21_WIN_MSVC
    assert(0 && "NOT SUPPORTED WHEN MSVS!");
    return 0;
#else
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    int re = 1, re2;

    if (!(dir = opendir(par))) {
        re = 0;
        return re;
    }
    if (!(entry = readdir(dir))) {
        re = 1;
        return re;
    }

    do {
        int l3 = listrlen(par) + listrlen(entry->d_name) + 1;
        char path[l3 + 1];
        int len = snprintf(path, sizeof(path), "%s%s%s", par, liunixSeparator,
                           entry->d_name);
        if (len < 0)
            return 0;

        if (stat(path, &statbuf) == -1) {
            if (liisDebug())
                perror(path);
            continue;
        }
        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(entry->d_name, ".") == 0
                || strcmp(entry->d_name, "..") == 0)
                continue;
            if (recTimes != 0) {

                re2 = _litackle_frame(pp, path, patternpp, patternNum, np, usep,
                                      recTimes - 1, tf, td, isSatisfied, tackle);
                if (!re2)
                    return 0;
            }
            if (td) {
                if (isSatisfied
                    && isSatisfied(entry->d_name, patternpp, patternNum, 0,
                                   -1)) {
                    if (tackle)
                        tackle(pp, np, usep, par, entry->d_name, 1);
                }
            }
        } else {

            if (tf) {
                if (isSatisfied
                    && isSatisfied(entry->d_name, patternpp, patternNum, 0,
                                   -1)) {
                    if (tackle)
                        tackle(pp, np, usep, par, entry->d_name, 0);
                }
            }
        }
    } while ((entry = readdir(dir)));
    closedir(dir);

    return re;
#endif
}

/*
 * tackle first, then recursive.
 */
int _lisearchBase10(void **pp, char *par, int *np, int *usep, int recTimes,
                    void *fargs,
                    int (*tackle)(void **pp, int *np, int *usep, char *path, int recTimes,
                                  void *fargs)) {
#ifdef MATH21_WIN_MSVC
    assert(0 && "NOT SUPPORTED WHEN MSVS!");
    return 0;
#else
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    int re = 1, re2;

    if (!(dir = opendir(par))) {
        re = 0;
        return re;
    }
//	livlog("a2:%d\n", re);
    if (!(entry = readdir(dir))) {
        re = 1;
        return re;
    }
//	livlog("aa:%d\n", re);
    do {
        int l3 = listrlen(par) + listrlen(entry->d_name) + 1;
        char path[l3 + 1];

        if (!lifileNameToPath(entry->d_name, par, path, sizeof(path))) {
            return 0;
        }
//		livlog("%s:%s: %d: %s\n", par, entry->d_name, listrlen(path), path);
        if (stat(path, &statbuf) == -1) {
            if (liisDebug()) {
                perror(path);
            }
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(entry->d_name, ".") == 0
                || strcmp(entry->d_name, "..") == 0)
                continue;
            if (tackle)
                tackle(pp, np, usep, path, recTimes, fargs);

            if (recTimes != 0) {
                re2 = _lisearchBase10(pp, path, np, usep, recTimes - 1, fargs,
                                      tackle);
                if (!re2)
                    return 0;
            }

        } else {
            if (tackle)
                tackle(pp, np, usep, path, recTimes, fargs);

        }
    } while ((entry = readdir(dir)));
    closedir(dir);

    return re;
#endif
}

//####

// deprecate, use _lils2 instead!
int _lils(char *par, int level, int recTimes) {
    char **pp = 0;
    int use = 0;
    char *pat = 0;
    int patternNum = 1;
    int n = 0;
    int output = LI_SUCCESS;
    output = _litackle_frame(0, par, 0, patternNum, &n, &use, recTimes, 1, 1,
                             lisearchSatisfy, lisearchTackle2);
//	output = _litackle_frame((void**) &pp, par, &pat, patternNum, &n, &use,
//			recTimes, tf, td, lisearchSatisfy, liremoveTackle);
//	liprintfss(pp, use);

    lifree(pat);
    lifreess3((void **) &pp, use);
    return output;
}

//####


/*********************************************
 * @D: r is recursively
 * tf is tackleFile,
 * td is tackleDirectory
 * tnr is tackle not recursively, meaning if we tackled dir, then don't recursive it.
 ********************************************/
int lisearchAndTackle(void **pp, const char *par, char **patternpp, int patternNum,
                      int *np, int *usep, int level, int recTimes, int tf, int td, int tnr,
                      int (*isSatisfied)(char *s, char **patternpp, int patternNum, int fr,
                                         int to),
                      int (*tackle)(void **pp, int *np, int *usep, const char *par, char *entry,
                                    int isDir)) {
#ifdef MATH21_WIN_MSVC
    assert(0 && "NOT SUPPORTED WHEN MSVS!");
    return 0;
#else
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    int re = 1, re2;

    if (!(dir = opendir(par))) {
        re = 0;
        return re;
    }
    if (!(entry = readdir(dir))) {
        re = 1;
        return re;
    }
    do {
        int l3 = listrlen(par) + listrlen(entry->d_name) + 1;
        char path[l3 + 1]; // yii
        int len = snprintf(path, sizeof(path), "%s%s%s", par, liunixSeparator,
                           entry->d_name);
        if (len < 0) {
            if (liisDebug())
                printf("searchError:%d:%d\n", len, (int) sizeof(path));
            return 0;
        }

        if (stat(path, &statbuf) == -1) {
            if (liisDebug())
                perror(path);
            continue;
        }
        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(entry->d_name, ".") == 0
                || strcmp(entry->d_name, "..") == 0)
                continue;
            if (td) {
                if (isSatisfied
                    && isSatisfied(entry->d_name, patternpp, patternNum, 0,
                                   -1)) {
                    if (tackle)
                        tackle(pp, np, usep, par, entry->d_name, 1);
                    if (tnr)
                        continue;
                }
            }
            if (recTimes != 0) {

                re2 = lisearchAndTackle(pp, path, patternpp, patternNum, np,
                                        usep, level + 1, recTimes - 1, tf, td, tnr, isSatisfied,
                                        tackle);
                if (!re2) {
                    if (liisDebug())
                        perror("return 0 recursively");
                    return 0;
                }
            }
        } else {

            if (tf) {
                if (isSatisfied
                    && isSatisfied(entry->d_name, patternpp, patternNum, 0,
                                   -1)) {
                    if (tackle)
                        tackle(pp, np, usep, par, entry->d_name, 0);
                }
            }
        }
    } while ((entry = readdir(dir)));
    closedir(dir);

    return re;
#endif
}

int lisearchFileSufN(void **pp, char *par, char *suf, int *np, int *usep,
                     int level, int recTimes, int ignorePermission,
                     void (*f)(void *, const char *, ...), void *finput) {
#ifdef MATH21_WIN_MSVC
    assert(0 && "NOT SUPPORTED WHEN MSVS!");
    return 0;
#else
    //    static int sqr3=0;
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    int re = 1, re2;
//    lilog(par);
    if (!liensurevvN(pp, np, usep, 8, sizeof(char *), lifreess3))
        return 0;

    if (!(dir = opendir(par))) {
        if (ignorePermission) {
            re = 1;
//            livlog("ignored %s",par);
        } else {
            re = 0;
        }
        return re;
    }
    if (!(entry = readdir(dir))) {
        re = 1;
        return re;
    }

    do {
        int l3 = listrlen(par) + listrlen(entry->d_name) + 1;
        char path[l3 + 1];
        int len = snprintf(path, sizeof(path), "%s/%s", par, entry->d_name);
        if (len < 0)
            return 0;

        if (stat(path, &statbuf) == -1) {
            if (liisDebug())
                perror(path);
            continue;
        }
//        lilog(path);
        if (S_ISDIR(statbuf.st_mode))
//		if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0
                || strcmp(entry->d_name, "..") == 0)
                continue;

            if (recTimes != 0) {

                if (f) {
                    if (finput)
                        f(finput, "%s\n", path);
                    else
                        f(finput, "%*s[%s]\n", level * 2, "", entry->d_name);
                }

                re2 = lisearchFileSufN(pp, path, suf, np, usep, level + 1,
                                       recTimes - 1, ignorePermission, f, finput);
                if (!re2)
                    return 0;
            }
        } else {

            if (liendsWith(entry->d_name, suf)) {
                char *sn = liscat3N(par, "/", entry->d_name);
                if (!sn)
                    return 0;

                if (!liensurevvN(pp, np, usep, *np, sizeof(char *), lifreess3))
                    return 0;

                *(char **) (*pp + sizeof(char *) * ((*usep)++)) = sn;

                if (f) {
                    if (finput)
                        f(finput, "%s\n", sn);
                    else
                        f(finput, "%*s- %s\n", level * 2, "", entry->d_name);
                }

            }

        }
    } while ((entry = readdir(dir)));
    closedir(dir);

    return re;
#endif
}

void lisearchFileSufNtest() {
    char **pp = 0, *par = ".", *suf = ".dll";
    int n = 0, use = 0, level = 2, recTimes = 5;

    lisearchFileSufN((void **) &pp, par, suf, &n, &use, level, recTimes, 0,
                     livlogfn2, 0);

    liprintfss(pp, use);

//	fclose(fp);
    lifreess3((void **) &pp, use);
    lifreess3((void **) &pp, use);
}

void lisearchFiles2Ntest(char *par) {
    char **pp = 0;
    int use = 0;

//	char* sufpp[] = { ".mk", "*B*", "a*" };
    char *sufpp[] = {"*.pgm", "*.png", "*.JPG"};
    lisearchFiles2N((void **) &pp, par, sufpp, sizeof(sufpp) / sizeof(*sufpp),
                    &use);
//	lisearchFiles2N((void**) &pp, par, 0, 3, &use);
    liprintfss(pp, use);

    lifreess3((void **) &pp, use);
}

void lisearchFiles2N(void **pp, const char *par, char **patternpp, int patternNum,
                     int *usep) {
//	char** pp = 0, *par = ".", *suf = ".dll";
    int n = 0;

//	lisearchAndTackle(pp, par, patternpp, patternNum, &n, usep, 2, -1, 1, 1, 1,
//			lisearchSatisfy, lisearchTackle);

    lisearchFiles4N(pp, par, patternpp, patternNum, &n, usep, 2, -1);
//	lisearchFilesN(pp, par, patternpp, patternNum, &n, usep, 2, 0, 0,0);

//	liprintfss(*pp, *usep);
}

int lisearchFiles4N(void **pp, const char *par, char **patternpp, int patternNum,
                    int *np, int *usep, int level, int recTimes) {
    return lisearchAndTackle(pp, par, patternpp, patternNum, np, usep, level,
                             recTimes, 1, 0, 1, lisearchSatisfy, lisearchTackle);
}

int lisearchFiles5N(void **pp, const char *par, char **patternpp, int patternNum,
                    int *usep, int recTimes) {
    int n = 0;
    return lisearchAndTackle(pp, par, patternpp, patternNum, &n, usep, 2,
                             recTimes, 1, 0, 1, lisearchSatisfy, lisearchTackle);
}

void lisearchAndTackletest(char *par) {
    char **pp = 0;
    int use = 0;

    char *sufpp[] = {"*.pgm", "*.jpg", "*.png", "*.PGM", "*.JPG", "*.PNG"};

    int n = 0;
    lisearchAndTackle((void **) &pp, par, sufpp, sizeof(sufpp) / sizeof(*sufpp),
                      &n, &use, 2, -1, 1, 0, 1, lisearchSatisfy, lisearchTackle);

    liprintfss(pp, use);

    lifreess3((void **) &pp, use);
}

//////////////////////

typedef struct __LiInputArgs {
    int argc;
    char **argv;
} LiInputArgs;

int lisearchSatisfy10(char *s, char **patternpp, int patternNum, int fr, int to) {
    if (!patternpp || liregex2(s, patternpp, patternNum, fr, to) == 1)
        return 1;
    else
        return 0;
}

int lisearchTackle10(void **pp, int *np, int *usep, char *path, int recTimes,
                     void *fargs) {

    LiInputArgs *pLiInputArgs = (LiInputArgs *) fargs;
    int isSatisfy = 1;
    if (pLiInputArgs->argc) {
//		char* sufpp[] = { "*.pgm", "*.jpg", "*.png", "*.PGM", "*.JPG", "*.PNG" };

        char *name = 0;
        lifileName4N(path, &name);

        isSatisfy = lisearchSatisfy10(name, pLiInputArgs->argv,
                                      pLiInputArgs->argc, 0, -1);
        lifree(name);
    }

    if (!isSatisfy)
        return 1;

    if (!path)
        return 0;

    if (!liensurevvN(pp, np, usep, 8, sizeof(char *), lifreess3))
        return 0;
    int len = listrlen(path);
    char *path2 = (char *) malloc(len + 1);
    memcpy(path2, path, len + 1);

    *(char **) ((char *) (*pp) + sizeof(char *) * ((*usep)++)) = path2;
    return 1;
}

void _lisearchUsage() {
    puts(
            "Usage: search [OPTION]... FILE...\n\
	Remove (unlink) the FILE(s).\n\
\n\
	  \
	  -r 2, -R, --recursive  search directories and their contents recursively\n\
	  -p .., --par         explain what is being done\n\
	      --help     display this help and exit\n\
	      --version  output version information and exit\n\
\n\
	By default, rm does not remove directories.  Use the --recursive (-r or -R)\n\
	option to remove each listed directory, too, along with all of its contents.\n\
\n\
	To remove a file whose name starts with a `-', for example `-foo',\n\
	use one of these commands:\n\
	  search -p ../.. -r 3 *.png *jpg\n\
	  search -p ../.. -r -1 *.png *jpg\n\
\n\
\n\
	Note that if you use rm to remove a file, it might be possible to recover\n\
	some of its contents, given sufficient expertise and/or time.  For greater\n\
	assurance that the contents are truly unrecoverable, consider using shred.\n\
\n\
	Report rm bugs to bug-coreutils@gnu.org\n\
	GNU coreutils home page: <http://www.gnu.org/software/coreutils/>\n\
	General help using GNU software: <http://www.gnu.org/gethelp/>\n\
	For complete documentation, run: info coreutils 'rm invocation'\n");
}

int lisearchN(int argc, char **argv, void **ppp, int *usep) {
    int isfound;
    int pos = 0, pos_max = 0;
    int i;
    int recTimes = 0, tf = 1, td = 0;
    char par[256] = {'.'};
    char recTimesString[32];

    if (argc == 2 && liscmp(argv[1], "--help") == 0) {
        _lisearchUsage();
        return LI_FAILURE;
    }
    isfound = lipickOption2(argc, argv, "r", recTimesString, &pos,
                            liisStrExistin);
    if (isfound == 1) {
        if (!liparseInt(recTimesString, &recTimes)) {
            return LI_FAILURE;
        }
        if (pos > pos_max)
            pos_max = pos;
    } else if (isfound == -1) {
        _lisearchUsage();
        return LI_FAILURE;

    }

    isfound = lipickOption2(argc, argv, "p", par, &pos, liisStrExistin);
    if (isfound == 1) {
        if (pos > pos_max)
            pos_max = pos;
    } else if (isfound == -1) {
        _lisearchUsage();
        return LI_FAILURE;

    }

    // check for unknown option call
    if (pos_max == argc - 1) {
//		livlogColor(0x00ff00,
//				"search: missing operand\nTry 'search --help' for more information.\n");
//		return LI_FAILURE;
    }

    for (i = pos_max + 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            livlogColor(0xff0000, "Fatal error: option \"-%s\" is wrong.\n",
                        argv[i] + 1);
            _lisearchUsage();
            return LI_FAILURE;
        }
    }

    int ssl = argc - 1 - pos_max;
    char **ss = (char **) malloc(ssl * sizeof *ss);

    if (ss == 0)
        return LI_FAILURE;
    // check for input image
    char *s;
    for (i = pos_max + 1; i < argc; i++) {
        s = listrdupN(argv[i]);
        if (!s) {
            return LI_FAILURE;
        }
        ss[i - pos_max - 1] = s;
    }

//	liprintfss(ss, ssl);

    LiInputArgs liInputArgs;
    liInputArgs.argc = ssl;
    liInputArgs.argv = ss;

    int n = 0;

    _lisearchBase10(ppp, par, &n, usep, recTimes, (void *) &liInputArgs,
                    lisearchTackle10);

//	liprintfss(pp, *usep);

    lifreess3((void **) &ss, ssl);

    return LI_SUCCESS;
}

int lisearch(int argc, char **argv) {
    int re = 1;
    char **pp = 0;
    int use = 0;
    re = lisearchN(argc, argv, (void **) &pp, &use);
    liprintfss(pp, use);
    lifreess3((void **) &pp, use);
    return re;
}

void lisearchImagesNtest(char *par) {
    char **pp = 0;
    int use = 0;

    lisearchImagesN((void **) &pp, par, &use);
    liprintfss(pp, use);

    lifreess3((void **) &pp, use);
}

int lisearchImagesN(void **pp, const char *par, int *usep) {

//	char* sufpp[] = { ".mk", "*B*", "a*" };
    char *sufpp[] = {"*.pgm", "*.jpg", "*.png", "*.PGM", "*.JPG", "*.PNG"};

    return lisearchFiles5N(pp, par, sufpp, sizeof(sufpp) / sizeof(*sufpp), usep,
                           -1);

}

void lisearchFileSuf2N(void **pp, char *par, char *suf, int *usep) {
//	char** pp = 0, *par = ".", *suf = ".dll";
    int n = 0;

    lisearchFileSufN(pp, par, suf, &n, usep, 2, -1, 1, 0, 0);

//	liprintfss(*pp, *usep);
}

off_t lifsize(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}

void lifileSize(char *file) {
    int *val;
    int use = 0;
    int s = lifsize(file);
    if (li10toxN(1024, s, &val, &use) == 0) {
        return;
    }
    while (use--) {
        livlog("%d ", val[use]);
    }
    lilog("\n");
    lifree2((void **) &val, 0);
}

void do_ls(char dirname[]) {
#ifdef MATH21_WIN_MSVC
    assert(0 && "NOT SUPPORTED WHEN MSVS!");
#else
    DIR *dir_ptr;
    struct dirent *direntp;

    if ((dir_ptr = opendir(dirname)) == NULL)
        fprintf(stderr, "ls:cannot open %s\n", dirname);
    else {
        while ((direntp = readdir(dir_ptr)) != NULL)
            printf("%s\n", direntp->d_name);
        closedir(dir_ptr);
    }
#endif
}

int _lisearchFiles(char *par, char **patternpp, int patternNum, int recTimes) {
    char **pp = 0;
    int use = 0, n = 0;
    FILE *fp = lifopen(liappTmp, liFileSearch, "w");
    if (!fp) {
        return 0;
    }

    lisearchFilesN((void **) &pp, par, patternpp, patternNum, &n, &use, 2,
                   recTimes, 0, 0);
    lilogss(fp, pp, use);

    fclose(fp);
    lifreess3((void **) &pp, use);
    return 1;
}

int lirenameFiles() {
    int re = 1;
    FILE *fp = 0, *fp2 = 0, *fw = 0;
    char *s1 = 0, *s2 = 0;
    fp = lifopen(liappTmp, liFileSelect, "r");
    if (!fp) {
        re = 0;
        goto label;
    }
    fp2 = lifopen(liappTmp, liFileDir, "r");
    if (!fp2) {
        re = 0;
        goto label;
    }

    fw = lifopen(liappTmp, liFileRename, "w");
    if (!fw) {
        re = 0;
        goto label;
    }

    int maxlen1 = 8, maxlen2 = 8;
    s1 = (char *) malloc(maxlen1);
    s2 = (char *) malloc(maxlen2);
    if (!s1 || !s2) {
        re = 0;
        goto label;
    }

    if (xjreadline(fp2, &s2, &maxlen2) == 0 || listrlen(s2) == 0) {
        re = 0;
        goto label;
    }
    int i = 0;
    char *par = 0;
    int n = 0;
    char tmp[8];
    while (xjreadline(fp, &s1, &maxlen1) && listrlen(s1) != 0) {
        if (lifileParN(s1, &par, &n) == 0) {
            re = 0;
            goto label;
        }
        snprintf(tmp, sizeof(tmp), "%d", i++);
        char *sn = liscat4N(par, "/", s2, tmp);
        if (rename(s1, sn) != 0)
            livlog("rename %s fail!\n", s1);
        fprintf(fw, "%s\n%s\n", s1, sn);
        free(sn);
    }

    label:
    if (s1)
        free(s1);
    if (s2)
        free(s2);
    if (par)
        free(par);
    if (fp)
        fclose(fp);
    if (fp2)
        fclose(fp2);
    if (fw)
        fclose(fw);
    return re;
}

int lirenameFiles2(int undo) {
    int re = 1;
    FILE *fp = 0;
    char *s1 = 0, *s2 = 0;
    fp = lifopen(liappTmp, liFileRename, "r");
    if (!fp) {
        re = 0;
        goto label;
    }

    int maxlen1 = 8, maxlen2 = 8;
    s1 = (char *) malloc(maxlen1);
    s2 = (char *) malloc(maxlen2);
    if (!s1 || !s2) {
        re = 0;
        goto label;
    }

    while (xjreadline(fp, &s1, &maxlen1) && xjreadline(fp, &s2, &maxlen2)) {
        if (undo)
            rename(s2, s1);
        else
            rename(s1, s2);
    }

    label:
    if (s1)
        free(s1);
    if (s2)
        free(s2);

    if (fp)
        fclose(fp);

    return re;
}

int licopyFiles() {
    int re = 1;
    FILE *fp = 0, *fp2 = 0, *fw = 0;
    char *s1 = 0, *s2 = 0;
    fp = lifopen(liappTmp, liFileSearch, "r");
    if (!fp) {
        re = 0;
        goto label;
    }
    fp2 = lifopen(liappTmp, liFileDir, "r");
    if (!fp2) {
        re = 0;
        goto label;
    }

    fw = lifopen(liappTmp, liFileCopy, "w");
    if (!fw) {
        re = 0;
        goto label;
    }

    int maxlen1 = 8, maxlen2 = 8;
    s1 = (char *) malloc(maxlen1);
    s2 = (char *) malloc(maxlen2);
    if (!s1 || !s2) {
        re = 0;
        goto label;
    }
    if (xjreadline(fp2, &s2, &maxlen2) == 0 || listrlen(s2) == 0) {
        re = 0;
        goto label;
    }

    int i = 0;
    char *name = 0;
    int n = 0;
    char tmp[8];
    while (xjreadline(fp, &s1, &maxlen1) && listrlen(s1) != 0) {
        if (lifileNameN(s1, &name, &n) == 0) {
            re = 0;
            goto label;
        }
        snprintf(tmp, sizeof(tmp), "%d", i++);
        char *sn = liscat3N(s2, liunixSeparator, name);

        char *cmd = liscat4N(licopy, s1, " ", s2);
        if (liisMingw()) {
            char *cmd2 = 0;
            if (lisubstitutebyN(cmd, "/", "\\", 0, -1, &cmd2) == 0) {
                re = 0;
                goto label;
            }
            free(cmd);
            cmd = cmd2;
        }
        if (system(cmd) <= 0) {
            livlog("copy %s fail!\n", cmd);
        }
        fprintf(fw, "%s\n%s\n", s1, sn);
        free(sn);
        free(cmd);
    }

    label:
    if (s1)
        free(s1);
    if (s2)
        free(s2);
    if (name)
        free(name);
    if (fp)
        fclose(fp);
    if (fp2)
        fclose(fp2);
    if (fw)
        fclose(fw);
    return re;
}

int lirenameFilesRemoveAndAdd(int removed, char *suf) {
    int re = 1;
    FILE *fp = 0, *fp2 = 0, *fw = 0;
    char *s1 = 0, *s2 = 0;
    fp = lifopen(liappTmp, liFileSearch, "r");
    if (!fp) {
        re = 0;
        goto label;
    }

    fw = lifopen(liappTmp, liFileRename, "w");
    if (!fw) {
        re = 0;
        goto label;
    }

    int maxlen1 = 8;
    s1 = (char *) malloc(maxlen1);
    if (!s1) {
        re = 0;
        goto label;
    }

    int slen;
//	char* sn;
//	int newlen;
    while (xjreadline(fp, &s1, &maxlen1) && (slen = listrlen(s1)) > 0) {

//		if (lisubstring(s1, &sn, &newlen, 0, slen - removed) == 0) {
//			re = 0;
//			goto label;
//		}
        int snlen = slen - removed;
        if (snlen <= 0) {
            re = 0;
            goto label;
        }
        char *sn2;
        char *sn = lisubstringN(s1, 0, snlen);
        if (!suf || !liscmp(suf, "null"))
            sn2 = lisubstringN(sn, 0, -1);
        else
            sn2 = liscat2N(sn, suf);
        if (rename(s1, sn2) != 0)
            livlog("rename %s fail!\n", s1);
        fprintf(fw, "%s\n%s\n", s1, sn2);
        free(sn);
        free(sn2);
    }

    label:
    free(s1);
//	free(sn);
    if (fp)
        fclose(fp);
    if (fw)
        fclose(fw);
    return re;
}

int lisafeFiles(char *suf) {
    int re = 1;
    FILE *fp = 0, *fw = 0;
    char *s1 = 0;
    fp = lifopen(liappTmp, liFileSearch, "r");
    if (!fp) {
        re = 0;
        goto label;
    }

    fw = lifopen(liappTmp, liFileSafe, "w");
    if (!fw) {
        re = 0;
        goto label;
    }

    int maxlen1 = 8;
    s1 = (char *) malloc(maxlen1);
    if (!s1) {
        re = 0;
        goto label;
    }
    while (xjreadline(fp, &s1, &maxlen1) && listrlen(s1) != 0) {
        char *sn = liscat2N(s1, suf);
        if (lisafeFile(s1, sn, "LilyAndNina") == 0)
            livlog("safe %s fail!\n", s1);
        if (!sn)
            fprintf(fw, "%s\n%s\n", s1, s1);
        else
            fprintf(fw, "%s\n%s\n", s1, sn);
        lifree(sn);
    }

    label:
    if (s1)
        free(s1);

    if (fp)
        fclose(fp);

    if (fw)
        fclose(fw);
    return re;
}

int lisafeFiles2(int undo) {
    int re = 1;
    FILE *fp = 0;
    char *s1 = 0, *s2 = 0;
    fp = lifopen(liappTmp, liFileSafe, "r");
    if (!fp) {
        re = 0;
        goto label;
    }

    int maxlen1 = 8, maxlen2 = 8;
    s1 = (char *) malloc(maxlen1);
    s2 = (char *) malloc(maxlen2);
    if (!s1 || !s2) {
        re = 0;
        goto label;
    }

    while (xjreadline(fp, &s1, &maxlen1) && xjreadline(fp, &s2, &maxlen2)) {
        if (undo)
            lisafeFile(s2, s1, "LilyAndNina");
        else
            lisafeFile(s1, s2, "LilyAndNina");
    }

    label:
    if (s1)
        free(s1);
    if (s2)
        free(s2);

    if (fp)
        fclose(fp);

    return re;
}

void lisafeDoc() {
    lilog("void lisafe1(char* s);\n");
    lilog("void lisafe2(char* s);\n");
    lilog("void lisafe3(char* s);\n");
}

//int limoveFiles() {
//	int re = 1;
//	FILE *fp = 0, *fp2 = 0, *fw = 0;
//	char* s1 = 0, *s2 = 0;
//	fp = lifopen(liappTmp, liFileSelect, "r");
//	if (!fp) {
//		re = 0;
//		goto label;
//	}
//	fp2 = lifopen(liappTmp, liFileDir, "r");
//	if (!fp2) {
//		re = 0;
//		goto label;
//	}
//
//	fw = lifopen(liappTmp, liFileMove, "w");
//	if (!fw) {
//		re = 0;
//		goto label;
//	}
//
//	int maxlen1 = 8, maxlen2 = 8;
//	s1 = malloc(maxlen1);
//	s2 = malloc(maxlen2);
//	if (!s1 || !s1) {
//		re = 0;
//		goto label;
//	}
//
//	if (xjreadline(fp2, &s2, &maxlen2) == 0||listrlen(s2)==0) {
//		re = 0;
//		goto label;
//	}
//	int i = 0;
//	char* par = 0;
//	int n = 0;
//	char tmp[8];
//	while (xjreadline(fp, &s1, &maxlen1)&&listrlen(s1)!=0) {
//		if (lifilePar(s1, &par, &n) == 0) {
//			re = 0;
//			goto label;
//		}
//		snprintf(tmp, sizeof(tmp), "%d", i++);
//		char* sn = liscat4N(par, "/", s2, tmp);
//		if(rename(s1, sn)!=0)livlog("rename %s fail!\n",s1);
//		fprintf(fw, "%s\n%s\n", s1, sn);
//	}
//
//	label: if (s1)
//		free(s1);
//	if (s2)
//		free(s2);
//	if (par)
//		free(par);
//	if (fp)
//		fclose(fp);
//	if (fp2)
//		fclose(fp2);
//	if (fw)
//		fclose(fw);
//	return re;
//}
int lisearchFiles(char *par) {
    int recTimes = -1;
    return _lisearchFiles(par, 0, 0, recTimes);
}

void lisearchFilestest() {
    char *par = "../yyy";
//	char* realPath=0;
    int recTimes = -1;
    char *pat[] = {".mk"};
    char **patternpp = 0;
//	char**patternpp = pat;
    int patternNum = 1;
//	realPath = realpath(par, 0);
//	if (!realPath)
//		return;
    _lisearchFiles(par, patternpp, patternNum, recTimes);
//	free(realPath);
}

void litestlifile() {

}

void lisafe1(char *s) {
    lisearchFiles(s);
    lirenameFilesRemoveAndAdd(0, ".c");
    lisearchFiles(s);
    lisafeFiles(".3");
    lisearchFiles(s);
    lisafeFiles(".3");
}

void lisafe2(char *s) {
    lisearchFiles(s);
    lirenameFilesRemoveAndAdd(6, 0);
}

void lisafe3(char *s) {
    lisearchFiles(s);
    lirenameFilesRemoveAndAdd(0, ".c");
    lisearchFiles(s);
    lisafeFiles(".3");
    lisearchFiles(s);
    lisafeFiles(".3");
    lisearchFiles(s);
    lirenameFilesRemoveAndAdd(6, 0);
}

int liJiShiBenWrite(char *name, const char *content) {
    FILE *fo = liHomefopen("JiShiBen", name, "ab");
    fprintf(fo, "%s\n", content);
    fclose(fo);
    return 1;
}

int liTextTotalLines(FILE *file) {

    if (!file)
        return -1;
    fpos_t position;
    fgetpos(file, &position);
    int ch;
    unsigned int number_of_lines = 0;
    while (EOF != (ch = getc(file))) {
        if ('\n' == ch)
            ++number_of_lines;
    }
    fsetpos(file, &position);
    return number_of_lines;
}

/* not check args*/
int lireadLines(ReadLineArgs *pReadLineArgs, void *fargs,
                int (*f)(void *entry, void *fargs)) {

    if (!pReadLineArgs)
        return 0;

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    int read;

    fp = liHomefopen("JiShiBen", pReadLineArgs->name, "r");

    if (fp == NULL)
        return 0;
    int totalLines = liTextTotalLines(fp);

    int i = 0;
    int startLine = pReadLineArgs->startLine;
//    livlog("%d...%d",totalLines,startLine);
    if (startLine < 0)
        startLine += totalLines;
    int endLine = startLine + pReadLineArgs->maxLineCount;

    len = 8;
    line = (char *) malloc(len);
    if (!line) {
        return 0;
    }

    while ((read = xjreadline(fp, &line, (int *) &len))) {
        if (i >= endLine)
            break;
        if (i >= startLine)
            if (!f(line, fargs))
                break;
        i++;
    }

    free(line);
    return 1;

}

int lireadTackle(void *entry, void *fargs) {
    if (!entry)
        return 2;
    StringArr *pStringArr = (StringArr *) fargs;
    char *line = (char *) entry;
    pStringArr->pp[pStringArr->use] = strdup(line);
    pStringArr->use++;
    return 1;
}

int lireadLinesTest(const char *name, int startLine, int maxLineCount) {
    ReadLineArgs readLineArgs;
    readLineArgs.maxLineCount = maxLineCount;
    readLineArgs.startLine = startLine;
    readLineArgs.name = name;

    StringArr sa;
    sa.max = maxLineCount;
    sa.use = 0;
    if (!(sa.pp = (char **) malloc(sa.max * sizeof *sa.pp)))
        return 0;
    lireadLines(&readLineArgs, &sa, lireadTackle);

    liprintfss(sa.pp, sa.use);
//	free(0);

    lifreess3((void **) &sa.pp, sa.use);

    return 1;
}

/*
 * start: where to start read, can be negative.
 * */
int lireadLinesMainN(int argc, char **argv, void *output) {
    int startLine = 0;
    int maxLineCount = 1;

    int isfound;
    int pos = 0, pos_max = 0;
    int i;
    char name[256] = {'.'};
    char recTimesString[32];

    if (argc == 2 && liscmp(argv[1], "--help") == 0) {
        _lisearchUsage();
        return LI_FAILURE;
    }
    isfound = lipickOption2(argc, argv, "start", recTimesString, &pos,
                            liisStrExistin);
    if (isfound == 1) {
        if (!liparseInt(recTimesString, &startLine)) {
            return LI_FAILURE;
        }
        if (pos > pos_max)
            pos_max = pos;
    } else if (isfound == -1) {
        _lisearchUsage();
        return LI_FAILURE;

    }

    isfound = lipickOption2(argc, argv, "max", recTimesString, &pos,
                            liisStrExistin);
    if (isfound == 1) {
        if (!liparseInt(recTimesString, &maxLineCount)) {
            return LI_FAILURE;
        }
        if (pos > pos_max)
            pos_max = pos;
    } else if (isfound == -1) {
        _lisearchUsage();
        return LI_FAILURE;

    }

    isfound = lipickOption2(argc, argv, "name", name, &pos, liisStrExistin);
    if (isfound == 1) {
        if (pos > pos_max)
            pos_max = pos;
    } else if (isfound == -1) {
        _lisearchUsage();
        return LI_FAILURE;

    }

    // check for unknown option call
    if (pos_max == argc - 1) {
//		livlogColor(0x00ff00,
//				"search: missing operand\nTry 'search --help' for more information.\n");
//		return LI_FAILURE;
    }

    for (i = pos_max + 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            livlogColor(0xff0000, "Fatal error: option \"-%s\" is wrong.\n",
                        argv[i] + 1);
            _lisearchUsage();
            return LI_FAILURE;
        }
    }

    ////////////////////
    ReadLineArgs readLineArgs;
    readLineArgs.maxLineCount = maxLineCount;
    readLineArgs.startLine = startLine;
    readLineArgs.name = name;

    StringArr *pStringArr = (StringArr *) output;
    pStringArr->max = maxLineCount;
    pStringArr->use = 0;
    if (!(pStringArr->pp = (char **) malloc(
            pStringArr->max * sizeof *pStringArr->pp)))
        return 0;
    lireadLines(&readLineArgs, output, lireadTackle);
    return 1;
}

int lireadLinesTest2(char *cmd) {

    int argc = 0;
//    char* cmd2 ="lily -name ho.txt -max 3 -start 0";
    char **argv = lisplitby2N(cmd, " ", 0, -1, &argc, 0);
//	liprintfss(argv,argc);
//    re = main(argc, argv);
    StringArr sa;
    lireadLinesMainN(argc, argv, &sa);
    lifreess(argv, argc);

    liprintfss(sa.pp, sa.use);
//	free(0);

    lifreess3((void **) &sa.pp, sa.use);

    return 1;
}
