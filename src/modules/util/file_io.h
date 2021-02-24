#pragma once

#include "inner.h"

bool xjFileExists(const char *name);

bool xjDirExists(const char *name);

bool xjDirExists(const char *name, int &is_same_name_file_exist);

int xjmkdir(const char *path);

int xjmkdirs(const char *path);

int xjrmdir(const char *name);

int xjrmfile(const char *name);

int xjrename(const char *src, const char *dst);

int lisearchTackle_put_to_seqce(void *container, const char *par, char *entry,
                                int isDir);

int lisearchTackle_remove(void *container, const char *par, char *entry,
                          int isDir);

namespace star {
    class Io {
    private:
        Seqce <std::string> v;

        void init();

    public:

        Io();

        ~Io();

        void clear();

        // It doesn't clear container.
        Seqce <std::string> &ls(Seqce <std::string> &v, const char *path,
                                const Seqce <std::string> &patterns, NumB isRecursive = 0, NumB tf = 1,
                                NumB td = 1) const;

        Seqce <std::string> &ls(const char *path,
                                const Seqce <std::string> &patterns = Seqce<std::string>(),
                                NumB isRecursive = 0, NumB tf = 1,
                                NumB td = 1);

        NumB mkdir(const std::string &name) const;

        NumB mkdirs(const std::string &name) const;

        NumB isDirExists(const std::string &name) const;

        NumB isFileExists(const std::string &name) const;

        NumB rmdir(const std::string &name) const;

        // shallow rename
        NumB renameFile(const std::string &src, const std::string &dst) const;

        NumB rmdirs(const std::string &name) const;

        NumB rmfile(const std::string &name) const;

        long fileSize(const std::string &name) const;
    };

    void test_file_io();

}