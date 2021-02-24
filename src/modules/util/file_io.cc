#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include "star_string.h"
#include "file.h"
#include "file_io.h"

using namespace math21;
//std::string xjGetAbsPath(const char* path){
//    std::string absPath = std::string(STAR_HOME_PATH) + std::string(path);
//    return absPath;
//}

// 0: not exit, 1: is dir, 2: is file.
int _xjAnyFileExists(const char *name) {
#ifdef _WIN32
#define xj_stat _stat
#define XJ_S_IFDIR _S_IFDIR
#else
#define xj_stat stat
#define XJ_S_IFDIR S_IFDIR
#endif
    struct xj_stat file_info;
    if (xj_stat(name, &file_info) != 0) {
        return 0;
    }else if ((file_info.st_mode & XJ_S_IFDIR) != 0) {
        return 1;
    } else {
        return 2;
    }
#undef xj_stat
#undef XJ_S_IFDIR
}

bool xjFileExists(const char *name) {
    if (_xjAnyFileExists(name) == 2) {
        return 1;
    } else {
        return 0;
    }
}

bool xjDirExists(const char *name) {
    if (_xjAnyFileExists(name) == 1) {
        return 1;
    } else {
        return 0;
    }
}

bool xjDirExists(const char *name, int &is_same_name_file_exist) {
    int result = _xjAnyFileExists(name);

    if (result == 0) {
        is_same_name_file_exist = 0;
        return false;
    } else if (result == 1) {
        is_same_name_file_exist = 0;
        return true;
    } else {
        is_same_name_file_exist = 1;
        return false;
    }
}

int xjmkdir(const char *path) {
#ifdef _WIN32
    mkdir(path);
#else
    int is_same_name_file_exist;
    if (xjDirExists(path, is_same_name_file_exist) == 0) {
        if (is_same_name_file_exist == 0) {
            /* Directory does not exist. EEXIST for race condition */
            mode_t mode = S_IRWXU | S_IRGRP | S_IXGRP;
            if (mkdir(path, mode) != 0 && errno != EEXIST) {
                return 0;
            }
        } else {
            return 0;
        }
    }

#endif
    return 1;
}

int xjmkdirs(const char *path) {
    char *pp;
    char *sp;
    int status;
    char *copypath = strdup(path);
    if (copypath == 0) {
        printf("memory error\n");
        return 0;
    }

    status = 1;
    pp = copypath;
    while (status == 1 && (sp = strchr(pp, '/')) != 0) {
        // Neither root nor double slash in path
        if (sp != pp) {
            *sp = '\0';
            status = xjmkdir(copypath);
            *sp = '/';
        }
        pp = sp + 1;
    }
    if (status == 1) {
        status = xjmkdir(path);
    }
    free(copypath);
    return status;
}

int xjrmdir(const char *name) {
    rmdir(name);
    return 1;
}

int xjrmfile(const char *name) {
    remove(name);
    return 1;
}

int xjrename(const char *src, const char *dst) {
    NumZ flag;
    flag = rename(src, dst);
    if (flag == -1) {
        return 0;
    }
    return 1;
}

int lisearchTackle_put_to_seqce(void *container, const char *par, char *entry,
                                int isDir) {
    std::string path = std::string(par) + "/" + std::string(entry);

    if (container) {
        Seqce<std::string> &v = *(Seqce<std::string> *) container;
        v.push(path);
    }
    return 1;
}

int lisearchTackle_remove(void *container, const char *par, char *entry,
                          int isDir) {
    std::string path = std::string(par) + "/" + std::string(entry);

    if (container) {
        Seqce<std::string> &v = *(Seqce<std::string> *) container;
        v.push(path);
    }
    if (isDir) {
        xjrmdir(path.c_str());
    } else {
        xjrmfile(path.c_str());
    }
    return 1;
}

//##

namespace star {
    void Io::init() {
        v.clear();
    }
    
    Io::Io() {
        init();
    }

    Io::~Io() {
        clear();
    }

    void Io::clear() {
        v.clear();
    }

    // It doesn't clear container.
    Seqce <std::string> &Io::ls(Seqce <std::string> &v, const char *path,
                            const Seqce <std::string> &patterns, NumB isRecursive, NumB tf,
                            NumB td) const {
        int recTimes = 0;
        if (isRecursive) {
            recTimes = -1;
        }
        _lils2(&v, path, patterns, recTimes, (int) tf, (int) td, lisearchSatisfy_string,
               lisearchTackle_put_to_seqce);
        return v;
    }

    Seqce <std::string> &Io::ls(const char *path,
                            const Seqce <std::string> &patterns,
                            NumB isRecursive, NumB tf,
                            NumB td) {
        return ls(v, path, patterns, isRecursive, tf, td);
    }

    NumB Io::mkdir(const std::string &name) const {
        if (xjmkdir(name.c_str())) {
            return 1;
        } else {
            return 0;
        }
    }

    NumB Io::mkdirs(const std::string &name) const {
        if (xjmkdirs(name.c_str())) {
            return 1;
        } else {
            return 0;
        }
    }

    NumB Io::isDirExists(const std::string &name) const {
        if (xjDirExists(name.c_str())) {
            return 1;
        }
        return 0;
    }

    NumB Io::isFileExists(const std::string &name) const {
        if (xjFileExists(name.c_str())) {
            return 1;
        }
        return 0;
    }

    NumB Io::rmdir(const std::string &name) const {
        if (xjrmdir(name.c_str())) {
            return 1;
        }
        return 0;
    }

    // shallow rename
    NumB Io::renameFile(const std::string &src, const std::string &dst) const {
        if (xjrename(src.c_str(), dst.c_str())) {
            return 1;
        }
        return 0;
    }

    NumB Io::rmdirs(const std::string &name) const {
        int recTimes = -1;
        int flag;
        if (isDirExists(name) == 0) {
            return 1;
        }
        flag = _lils2(0, name.c_str(), Seqce<std::string>(), recTimes, 1, 1, lisearchSatisfy_string,
                      lisearchTackle_remove);
        if (flag == 0) {
            return 0;
        }
        flag = rmdir(name);
        if (flag == 0) {
            return 0;
        }
        return 1;
    }

    long Io::fileSize(const std::string &name) const{
        return star_file_size(name);
    }

    NumB Io::rmfile(const std::string &name) const {
        if (xjrmfile(name.c_str())) {
            return 1;
        }
        return 0;
    }
    
    void test_file_io() {
        Io io;
        Seqce<std::string> patterns;
//        patterns.push(".*");
        Seqce<std::string> &v = io.ls("/home/yol", patterns);
//        v.log("v");
        std::string s;
        for (NumN i = 1; i <= v.size(); ++i) {
            s = v(i);
            s = star_string_get_file_name(s);
            s = star_string_remove_suffix(s);
        }

//        lipriorityqueuetest();
//        lishuffletest();
//        lishuffletest2();
//        m21log(star_string_is_start_with("", "/"));
    }
}