#include "inner.h"
#ifdef MATH21_WIN_MSVC
#include <io.h>
#else
#include <unistd.h>
#include <dirent.h>
#endif
#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>
#include <string>
#include <cstring>
#include "lifile.h"
#include "listring.h"
#include "files.h"

namespace star {
// same as lisearchAndTackle
    int _litackle_frame_string(void *container, const char *par, const Seqce<std::string> &patterns,
                               int recTimes, int tf, int td,
                               int (*isSatisfied)(char *s, const Seqce<std::string> &patterns, int fr,
                                                  int to),
                               int (*tackle)(void *container, const char *par, char *entry,
                                             int isDir)) {
        DIR *dir;
        struct dirent *entry;
        struct stat statbuf;
        int re = 1, re2;

        if (!(dir = opendir(par))) {
#ifdef MATH21_DEBUG
            perror("Error opendir()");
#endif
            return 0;
        }

        while ((entry = readdir(dir)) != NULL) {
            std::string path = std::string(par) + "/" + std::string(entry->d_name);
            if (stat(path.c_str(), &statbuf) == -1) {
#ifdef MATH21_DEBUG
                perror(path.c_str());
#endif
                continue;
            }
            if (S_ISDIR(statbuf.st_mode)) {
                if (strcmp(entry->d_name, ".") == 0
                    || strcmp(entry->d_name, "..") == 0)
                    continue;
                if (recTimes != 0) {

                    re2 = _litackle_frame_string(container, path.c_str(), patterns,
                                                 recTimes - 1, tf, td, isSatisfied, tackle);
                    if (!re2)
                        return 0;
                }
                if (td) {
                    if (isSatisfied
                        && isSatisfied(entry->d_name, patterns, 0,
                                       -1)) {
                        if (tackle)
                            tackle(container, par, entry->d_name, 1);
                    }
                }
            } else {

                if (tf) {
                    if (isSatisfied
                        && isSatisfied(entry->d_name, patterns, 0,
                                       -1)) {
                        if (tackle)
                            tackle(container, par, entry->d_name, 0);
                    }
                }
            }
        }
        closedir(dir);

        return re;
    }

    int lisearchSatisfy_string(char *s, const Seqce<std::string> &patterns, int fr, int to) {
        if (patterns.isEmpty() || liregex2_string(s, patterns, fr, to) == 1)
            return 1;
        else
            return 0;
    }

    int _lils2(void *container, const char *par, const Seqce<std::string> &patterns, int recTimes,
               int tf, int td,
               int (*isSatisfied)(char *s, const Seqce<std::string> &patterns, int fr,
                                  int to),
               int (*tackle)(void *container, const char *par, char *entry,
                             int isDir)) {
        return _litackle_frame_string(container, par, patterns, recTimes, tf, td,
                                      isSatisfied, tackle);
    }

    int lils(int ac, const char **av) {
        int re = 1;
        Seqce<std::string> patterns;
        if (ac == 1) {
            re = _lils2(0, ".", patterns, 0, 1, 1, lisearchSatisfy_string, lisearchTackle21_print);
        } else
            while (--ac) {
                if (_lils2(0, *++av, patterns, 0, 1, 1, lisearchSatisfy_string, lisearchTackle21_print) == 0) {
                    re = 0;
                }
            }
        return re;
    }
}