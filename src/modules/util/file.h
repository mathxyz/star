#pragma once

#include "inner.h"

namespace star {
    long star_file_size(const std::string &path);

    int sky_file_rename_files(int argc, const char **argv);

    void star_file_rename_files(const Seqce <std::string> &ls_files,
                                const char *name_prefix = 0, NumZ start = 1, const char *name_suffix = 0,
                                NumN width = 0, NumB isLog = 0);

    void star_file_rename_files(const char *folder,
                                const char *name_prefix = 0, NumZ start = 1, const char *name_suffix = 0,
                                NumN width = 0, NumB isLog = 0);

    NumB
    star_file_byte_set(const char *path1, NumN n, const Seqce <NumN> &indexes, const Seqce <NumN> &values, NumN type,
                       NumB isLog);

    NumB star_file_byte_log(const char *path1, NumN n, const Seqce <NumN> &indexes, NumN type, NumB isLog);

    NumB star_file_vector_c_analysis(const char *path1, const char *path2, NumN type, NumR norm, NumR epsilon,
                                     NumN logLevel);

    int sky_file_vector_c_analysis(int argc, const char **argv);

    NumB star_file_is_same(const char *path1, const char *path2, NumB isLog,
                           NumZ from1 = 0, NumZ to1 = 0, NumZ from2 = 0, NumZ to2 = 0);

    NumB star_file_is_text(const char *path1, NumB isLog,
                           NumZ from1 = 0, NumZ to1 = 0);

    // logLevel, 0: no log, 1: log, 2: log detail
    NumB star_file_folder_is_same(const char *path1, const char *path2, NumN logLevel);

    int sky_file_byte_set(int argc, const char **argv);

    int sky_file_is_same(int argc, const char **argv);

    int sky_file_is_text(int argc, const char **argv);
}