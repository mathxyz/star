#include <sys/stat.h>
#include "../parser/files.h"
#include "files.h"
#include "file.h"

namespace star {
    // Size of file, in bytes.
    long star_file_size(const std::string &name) {
#ifdef _WIN32
#define xj_stat _stat
#define XJ_S_IFDIR _S_IFDIR
#else
#define xj_stat stat
#define XJ_S_IFDIR S_IFDIR
#endif
        struct xj_stat file_info;
        if (xj_stat(name.c_str(), &file_info) != 0) {
            return -1;
        } else {
            return file_info.st_size;
        }
#undef xj_stat
#undef XJ_S_IFDIR
    }

    void star_file_rename_files(const Seqce<std::string> &ls_files,
                                const char *name_prefix, NumZ start, const char *name_suffix, NumN width, NumB isLog) {
        Io io;
        NumB flag;
        for (NumN i = 1; i <= ls_files.size(); ++i) {
            std::string s;
            s = ls_files(i);
            std::string dst_name;
            if (name_prefix) {
                dst_name = math21_string_to_string(name_prefix) + dst_name;
            }
            dst_name += math21_string_NumZ_to_string(start - 1 + (NumZ) i, width);
            if (name_suffix) {
                dst_name += math21_string_to_string(name_suffix);
            }
            std::string par = star_string_get_file_parent(s);
            if (!star_string_is_null_or_spaces(par)) {
                par += "/";
            }
            dst_name = par + dst_name;

            flag = io.renameFile(s, dst_name);
            if (isLog) {
                if (!flag) {
                    std::cout << "Failed: ";
                }
                std::cout << ls_files(i) << "\n-> " << dst_name << std::endl;
            }
        }
    }

    void star_file_rename_files(const char *folder,
                                const char *name_prefix, NumZ start, const char *name_suffix, NumN width, NumB isLog) {
        Io io;
        Seqce<std::string> v;
        io.ls(v, folder, Seqce<std::string>(), 1, 1, 0);
        math21_string_sort(v);
        star_file_rename_files(v, name_prefix, start, name_suffix, width, isLog);
    }

    int sky_file_rename_files(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tsky_file_rename_files \n"
                       "\tdir [flags]\n"
                       "\tflags:\n"
                       "\t  --name_prefix\n"
                       "\t      The default value is ''.\n"
                       "\t  --start\n"
                       "\t      The default value is '1'.\n"
                       "\t  --name_suffix\n"
                       "\t      The default value is ''.\n"
                       "\t  --width\n"
                       "\t      width of the number.\n"
                       "\t      The default value is '0'.\n"
                       "\t  --print\n"
                       "\t      Print files.\n"
                );
                m21log("\te.x., \n"
                       "\tstar_ls folder --pattern *.txt\n");
                return 1;
            }
        }

        if (argc < 2) {
            return 0;
        }

        const char *folder = 0;
        const char *name_prefix = 0;
        NumZ start = 1;
        const char *name_suffix = 0;
        NumN width = 0;
        NumB isLog = 0;
        for (int i = 1; i < argc; ++i) {
            if (math21_string_is_equal(argv[i], "--name_prefix")) {
                name_prefix = argv[i + 1];
                ++i;
            } else if (math21_string_is_equal(argv[i], "--name_suffix")) {
                name_suffix = argv[i + 1];
                ++i;
            } else if (math21_string_is_equal(argv[i], "--start")) {
                start = math21_string_to_NumZ(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--width")) {
                width = math21_string_to_NumN(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--print")) {
                isLog = 1;
            } else {
                folder = argv[i];
            }
        }
        star_file_rename_files(folder, name_prefix, start, name_suffix, width, isLog);
        return 1;
    }

    // n_ele elements to be set.
    // type = 0, set one by one
    // type = 1, set all.
    NumB star_file_byte_log_or_set(
            const char *path1, NumN n_ele, const Seqce<NumN> &indexes,
            const Seqce<NumN> &values, NumN type, NumB isSetByte, NumB isLog) {
        if (isSetByte && indexes.size() != values.size()) {
            if (isLog) {
                printf("Not equal!\nindexes.size = %d, values.size = %d\n", indexes.size(), values.size());
            }
            return 0;
        }
        if (indexes.size() == 0) {
            if (isLog) {
                printf("Empty!\nindexes.size = %d\n", indexes.size());
            }
            return 0;
        }
        if (n_ele == 0) {
            if (isLog) {
                printf("Empty!\nn = %d\n", n_ele);
            }
            return 0;
        }
        if (type == 0) {
            if (n_ele > indexes.size()) {
                if (isLog) {
                    printf("Error: indexes.size < n\nindexes.size = %d\nn = %d\n", indexes.size(), n_ele);
                }
                return 0;
            }
        }
        Io io;
        size_t n1 = (size_t) io.fileSize(path1);
        NumN8 *x = math21_vector_raw_deserialize_from_file_byte_cpu(path1, n1);
        size_t n = n1;
        if (n == 0) {
            if (isLog) {
                printf("Empty file\n%s\n", path1);
            }
            return 0;
        }
        NumN index;
        NumN8 value;
        if (type == 0) {
            for (NumN i = 1; i <= n_ele; ++i) {
                index = indexes(i);
                if (isSetByte) {
                    value = (NumN8) values(i);
                    math21_vector_set_byte_cpu(1, value, x + index, 1);
                } else {
                    math21_vector_log_byte_cpu(1, x + index, 1);
                }
            }
        } else {
            index = indexes(1);
            if (isSetByte) {
                value = (NumN8) values(1);
                math21_vector_set_byte_cpu(n_ele, value, x + index, 1);
            } else {
                math21_vector_log_byte_cpu(n_ele, x + index, 1);
            }
        }

        if (isSetByte) {
            math21_vector_raw_serialize_to_file_byte_cpu(path1, x, n);
        } else {
            printf("\n");
        }
        math21_vector_free_cpu(x);
        return 1;
    }

    NumB star_file_byte_set(
            const char *path1, NumN n_ele, const Seqce<NumN> &indexes,
            const Seqce<NumN> &values, NumN type, NumB isLog) {
        return star_file_byte_log_or_set(path1, n_ele, indexes, values, type, 1, isLog);
    }

    NumB star_file_byte_log(
            const char *path1, NumN n_ele, const Seqce<NumN> &indexes,
            NumN type, NumB isLog) {
        return star_file_byte_log_or_set(path1, n_ele, indexes, Seqce<NumN>(), type, 0, isLog);
    }

    // type 0: equal, type 1: min_max, type 2: Lp space.
    NumB star_file_vector_c_analysis(const char *path1, const char *path2, NumN type,
                                     NumR norm, NumR epsilon, NumN logLevel) {
        size_t n1 = 0;
        size_t n2 = 0;
        float *x = math21_vector_deserialize_from_file_cpu(path1, &n1);
        float *y = 0;
        if (path2) {
            y = math21_vector_deserialize_from_file_cpu(path2, &n2);
        }
        if (logLevel > 1) {
            printf("size = " SIZE_T_SPECIFIER ", %s\n", n1, path1);
            if (path2) {
                printf("size = " SIZE_T_SPECIFIER ", %s\n", n2, path2);
            }
        }
        if (type == 0) {
            if (n1 != n2) {
                if (logLevel) {
                    printf("Different number of elements\nsize = " SIZE_T_SPECIFIER ", %s\nsize = " SIZE_T_SPECIFIER ", %s\n",
                           n1, path1, n2, path2);
                }
                return 0;
            }
            size_t n = n1;
            if (n <= 0) {
                return 1;
            }
            if (logLevel > 2) {
                printf("epsilon = %.60lf\n", epsilon);
            }
            NumB flag = math21_vector_is_equal_cpu(x, y, n, epsilon, logLevel);
            if (flag) {
                if (logLevel) {
                    printf("Equal!\n");
                }
            } else {
                if (logLevel) {
                    printf("Not Equal!\n");
                }
            }
        } else if (type == 1) {
            float max_x = math21_vector_max_cpu(x, n1);
            float min_x = math21_vector_min_cpu(x, n1);
            if (logLevel) {
                printf("min_x = %f, max_x = %f\n", min_x, max_x);
            }
            if (y) {
                float max_y = math21_vector_max_cpu(y, n2);
                float min_y = math21_vector_min_cpu(y, n2);
                if (logLevel) {
                    printf("min_y = %f, max_y = %f\n", min_y, max_y);
                }
            }
        } else if (type == 2) {
            if (n1 != n2) {
                if (logLevel) {
                    printf("Different number of elements\nsize = " SIZE_T_SPECIFIER ", %s\nsize = " SIZE_T_SPECIFIER ", %s\n",
                           n1, path1, n2, path2);
                }
                return 0;
            }
            size_t n = n1;
            if (n <= 0) {
                return 1;
            }
            NumR result = math21_vector_distance_cpu(x, y, n, norm);
            if (logLevel) {
                if (norm < 1) {
                    printf("||x - y||.L%.1f = %lf\n", norm, result);
                } else {
                    printf("||x - y||.L%.0f = %lf\n", norm, result);
                }
                NumB flag = math21_operator_num_isEqual(result, 0, epsilon);
                if (flag) {
                    printf("Equal!\n");
                } else {
                    printf("Not Equal!\n");
                }
            }
        }
        math21_vector_free_cpu(x);
        math21_vector_free_cpu(y);
        return 1;
    }

    int sky_file_vector_c_analysis(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tsky_file_vector_c_analysis \n"
                       "\t  --file1\n"
                       "\t      path to file or folder.\n"
                       "\t  --file2\n"
                       "\t      path to file or folder.\n"
                       "\t  --type\n"
                       "\t      equal, min_max, Lp, The default value is 'equal'.\n"
                       "\t  --norm\n"
                       "\t      norm used by Lp, The default value is '2'.\n"
                       "\t  --epsilon\n"
                       "\t      used when type = equal, The default value is '0'.\n"
                       "\t  --logLevel\n"
                       "\t      logLevel, 0: no log, 1: log, 2: log detail.\n"
                );
                m21log("\te.x., \n"
                       "\tsky_file_is_same --file1 a.bin --file2 b.bin\n");
                return 1;
            }
        }

        if (argc < 2) {
            return 0;
        }

        const char *path1 = 0;
        const char *path2 = 0;
        NumN type = 0;
        NumR epsilon = 0;
        NumN logLevel = 0;
        NumR norm = 2;
        for (int i = 1; i < argc; ++i) {
            if (math21_string_is_equal(argv[i], "--file1")) {
                path1 = argv[i + 1];
                ++i;
            } else if (math21_string_is_equal(argv[i], "--file2")) {
                path2 = argv[i + 1];
                ++i;
            } else if (math21_string_is_equal(argv[i], "--type")) {
                if (math21_string_is_equal(argv[i + 1], "equal")) {
                    type = 0;
                } else if (math21_string_is_equal(argv[i + 1], "min_max")) {
                    type = 1;
                } else if (math21_string_is_equal(argv[i + 1], "Lp")) {
                    type = 2;
                }
                ++i;
            } else if (math21_string_is_equal(argv[i], "--epsilon")) {
                epsilon = math21_string_to_NumR(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--norm")) {
                norm = math21_string_to_NumR(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--logLevel")) {
                logLevel = math21_string_to_NumN(argv[i + 1]);
                ++i;
            }
        }

        Io io;
        NumB flag;
        flag = star_file_vector_c_analysis(path1, path2, type, norm, epsilon, logLevel);
        return flag;
    }

    // [from , to], e.x., [-2, -1]
    NumB star_file_is_same(const char *path1, const char *path2, NumB isLog,
                           NumZ from1, NumZ to1,
                           NumZ from2, NumZ to2) {
        Io io;
        NumN n1 = io.fileSize(path1);
        NumN n2 = io.fileSize(path2);
        NumN n1_file = n1;
        NumN n2_file = n2;

        // empty
        if (n1 == 0 && n2 == 0) {
            return 1;
        }

        math21_number_get_from_and_to(n1, from1, to1);
        math21_number_get_from_and_to(n2, from2, to2);
        if (!math21_number_check_from_and_to(n1, from1, to1)) {
            if (isLog) {
                printf("Index out of bounds.\nn1 = %d, from1 = %d, to1 = %d\n", n1, from1, to1);
            }
            return 0;
        }
        if (!math21_number_check_from_and_to(n2, from2, to2)) {
            if (isLog) {
                printf("Index out of bounds.\nn2 = %d, from2 = %d, to2 = %d\n", n2, from2, to2);
            }
            return 0;
        }
        n1 = to1 - from1 + 1;
        n2 = to2 - from2 + 1;

        if (n1 != n2) {
            if (isLog) {
                printf("Different number of bytes\nsize = %d, %s\nsize = %d, %s\n", n1, path1, n2, path2);
            }
            return 0;
        }
        size_t n = n1;

        NumN8 *x = math21_vector_raw_deserialize_from_file_byte_cpu(path1, n1_file);
        NumN8 *y = math21_vector_raw_deserialize_from_file_byte_cpu(path2, n2_file);
        NumB flag = math21_vector_is_same_byte_cpu(x + from1 - 1, y + from2 - 1, n, isLog);
        math21_vector_free_cpu(x);
        math21_vector_free_cpu(y);
        return flag;
    }

    NumB star_file_is_text(const char *path1, NumB isLog,
                           NumZ from1, NumZ to1) {
        Io io;
        NumN n1 = io.fileSize(path1);
        NumN n1_file = n1;

        // empty
        if (n1 == 0) {
            return 1;
        }

        math21_number_get_from_and_to(n1, from1, to1);
        if (!math21_number_check_from_and_to(n1, from1, to1)) {
            if (isLog) {
                printf("Index out of bounds.\nn1 = %d, from1 = %d, to1 = %d\n", n1, from1, to1);
            }
            return 0;
        }
        n1 = to1 - from1 + 1;

        size_t n = n1;

        NumN8 *x = math21_vector_raw_deserialize_from_file_byte_cpu(path1, n1_file);
        NumB flag = math21_vector_is_byte_text(x + from1 - 1, n, isLog);
        math21_vector_free_cpu(x);
        return flag;
    }

    NumB star_file_folder_is_same(const char *path1, const char *path2, NumN logLevel) {
        Io io;
        Seqce<std::string> v1;
        Seqce<std::string> v2;
        io.ls(v1, path1, Seqce<std::string>(), 1);
        io.ls(v2, path2, Seqce<std::string>(), 1);

        math21_string_sort(v1);
        math21_string_sort(v2);
        if (logLevel == 2) {
            v1.log(path1);
            v2.log(path2);
        }
        if (v1.size() != v2.size()) {
            if (logLevel) {
                printf("Different number of files\nsize = %d, %s\nsize = %d, %s\n", v1.size(), path1, v2.size(), path2);
            }
            return 0;
        }
        NumB flag = 1;
        for (NumN i = 1; i <= v1.size(); ++i) {
            if (logLevel > 2) {
                printf("comparing %d file in %d\n", i, v1.size());
            }
            flag = star_file_is_same(v1(i).c_str(), v2(i).c_str(), logLevel);
            if (!flag) {
                if (logLevel) {
                    printf("Different from file\n%s\n%s\n", v1(i).c_str(), v2(i).c_str());
                }
                break;
            }
        }
        return flag;
    }

    NumB star_file_folder_is_text(const char *path1, const _Set<std::string> &exclude_suffixes, NumN logLevel) {
        Io io;
        Seqce<std::string> v1;
        io.ls(v1, path1, Seqce<std::string>(), 1, 1, 0);

        math21_string_sort(v1);
        if (logLevel == 2) {
            v1.log(path1);
        }
        NumB flag = 1;
        for (NumN i = 1; i <= v1.size(); ++i) {
            if (exclude_suffixes.contains(star_string_get_suffix(v1(i)))) {
                continue;
            }
            if (logLevel == 4) {
                printf("analyzing file %s\n", v1(i).c_str());
            }else if(logLevel == 3){
                printf("analyzing %d file in %d\n", i, v1.size());
            }
            flag = star_file_is_text(v1(i).c_str(), logLevel);
            if (!flag) {
                if (logLevel) {
                    printf("Not satistied file\n%s\n", v1(i).c_str());
                }
                break;
            }
        }
        return flag;
    }

    int sky_file_byte_set(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tsky_file_byte_set \n"
                       "\t  --file\n"
                       "\t      file\n"
                       "\t  --n\n"
                       "\t      the number of elements to be set\n"
                       "\t  --kvp\n"
                       "\t      key value\n"
                       "\t      used by setByte mode\n"
                       "\t  --indexes\n"
                       "\t      used by peek mode\n"
                       "\t  --type\n"
                       "\t      several, all (default several)\n"
                       "\t      If type = all, it just reads one kvp.\n"
                       "\t      And set all n elements starting from key to the value.\n"
                       "\t  --peek\n"
                       "\t  --logLevel\n"
                       "\t      logLevel, 0: no log, 1: log, 2: log detail.\n"
                );
                m21log("\te.x., \n"
                       "\tsky_file_is_same --file a.bin --n 2 --kvp 1 255 2 255\n");
                return 1;
            }
        }

        if (argc < 2) {
            return 0;
        }

        const char *path1 = 0;
        NumN n = 0;
        NumB isSetByte = 1;
        NumN logLevel = 0;
        NumN type = 0;
        Seqce<NumN> indexes;
        Seqce<NumN> values;
        for (int i = 1; i < argc; ++i) {
            if (math21_string_is_equal(argv[i], "--file")) {
                path1 = argv[i + 1];
                ++i;
            } else if (math21_string_is_equal(argv[i], "--n")) {
                n = math21_string_to_NumN(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--type")) {
                if (math21_string_is_equal(argv[i + 1], "several")) {
                    type = 0;
                } else if (math21_string_is_equal(argv[i + 1], "all")) {
                    type = 1;
                }
                ++i;
            } else if (math21_string_is_equal(argv[i], "--peek")) {
                isSetByte = 0;
            } else if (math21_string_is_equal(argv[i], "--logLevel")) {
                logLevel = math21_string_to_NumN(argv[i + 1]);
                ++i;
            }
        }

        if (!n) {
            if (logLevel) {
                printf("no kvp\n");
            }
            return 0;
        }

        for (int i = 1; i < argc; ++i) {
            if (isSetByte) {
                if (math21_string_is_equal(argv[i], "--kvp")) {
                    for (NumN j = 1; j <= n; ++j) {
                        indexes.push(math21_string_to_NumN(argv[i + 2 * j - 1]));
                        values.push(math21_string_to_NumN(argv[i + 2 * j]));
                        if (type == 1) {
                            break;
                        }
                    }
                }
            } else {
                if (math21_string_is_equal(argv[i], "--indexes")) {
                    for (NumN j = 1; j <= n; ++j) {
                        indexes.push(math21_string_to_NumN(argv[i + j]));
                        if (type == 1) {
                            break;
                        }
                    }
                }
            }
        }
        if (logLevel > 1) {
            indexes.log("indexes");
            values.log("values");
        }

        if (isSetByte) {
            return star_file_byte_set(path1, n, indexes, values, type, logLevel);
        } else {
            return star_file_byte_log(path1, n, indexes, type, logLevel);
        }
    }

    int sky_file_is_same(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tsky_file_is_same \n"
                       "\t  --file1\n"
                       "\t      path to file or folder.\n"
                       "\t  --file2\n"
                       "\t      path to file or folder.\n"
                       "\t  --type\n"
                       "\t      file, folder, The default value is 'file'.\n"
                       "\t  --from1\n"
                       "\t      from byte when type is file. from is NumZ\n"
                       "\t  --to1\n"
                       "\t      to byte when type is file. to is NumZ\n"
                       "\t  --from2\n"
                       "\t  --to2\n"
                       "\t  --logLevel\n"
                       "\t      logLevel, 0: no log, 1: log, 2: log detail.\n"
                );
                m21log("\te.x., \n"
                       "\tsky_file_is_same --file1 a.bin --file2 b.bin --from -2\n");
                return 1;
            }
        }

        if (argc < 5) {
            return 0;
        }

        const char *path1 = star_string_args_find_c_string(argc, argv, "--file1", 0);
        const char *path2 = star_string_args_find_c_string(argc, argv, "--file2", 0);
        std::string type = star_string_args_find_string(argc, argv, "--type", "file");
        NumN logLevel = star_string_args_find_NumN(argc, argv, "--logLevel", 0);
        NumZ from1 = star_string_args_find_NumZ(argc, argv, "--from1", 0);
        NumZ to1 = star_string_args_find_NumZ(argc, argv, "--to1", 0);
        NumZ from2 = star_string_args_find_NumZ(argc, argv, "--from2", 0);
        NumZ to2 = star_string_args_find_NumZ(argc, argv, "--to2", 0);

        Io io;
        NumB flag = 1;
        if (type == "file") {
            if (!io.isFileExists(path1)) {
                printf("Not exists %s!\n", path1);
                flag = 0;
            }
            if (!io.isFileExists(path2)) {
                printf("Not exists %s!\n", path2);
                flag = 0;
            }
            if (!flag) {
                return flag;
            }
            flag = star_file_is_same(path1, path2, logLevel, from1, to1, from2, to2);
        } else {
            if (!io.isDirExists(path1)) {
                printf("Not exists %s!\n", path1);
                flag = 0;
            }
            if (!io.isDirExists(path2)) {
                printf("Not exists %s!\n", path2);
                flag = 0;
            }
            if (!flag) {
                return flag;
            }
            flag = star_file_folder_is_same(path1, path2, logLevel);
        }
        if (flag) {
            printf("Same\n%s\n%s\n", path1, path2);
        } else {
            printf("Different\n%s\n%s\n", path1, path2);
        }
        return flag;
    }

    int sky_file_is_text(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tsky_file_is_text \n"
                       "\t  --file1\n"
                       "\t      path to file or folder.\n"
                       "\t  --type\n"
                       "\t      file, folder, The default value is 'file'.\n"
                       "\t  --from1\n"
                       "\t      from byte when type is file. from is NumZ\n"
                       "\t  --to1\n"
                       "\t      to byte when type is file. to is NumZ\n"
                       "\t  --exclude_suffixes\n"
                       "\t      e.x., jpg,png (valid only for type is folder)\n"
                       "\t  --logLevel\n"
                       "\t      logLevel, 0: no log, 1: log, 2: log detail.\n"
                );
                m21log("\te.x., \n"
                       "\tsky_file_is_text --file1 a.bin --file2 b.bin --from -2\n");
                return 1;
            }
        }

        if (argc < 5) {
            return 0;
        }

        const char *path1 = star_string_args_find_c_string(argc, argv, "--file1", 0);
        std::string type = star_string_args_find_string(argc, argv, "--type", "file");
        std::string s_exclude_suffixes = star_string_args_find_string(argc, argv, "--exclude_suffixes", "");
        NumN logLevel = star_string_args_find_NumN(argc, argv, "--logLevel", 0);
        NumZ from1 = star_string_args_find_NumZ(argc, argv, "--from1", 0);
        NumZ to1 = star_string_args_find_NumZ(argc, argv, "--to1", 0);

        Io io;
        NumB flag = 1;
        if (type == "file") {
            if (!io.isFileExists(path1)) {
                printf("Not exists %s!\n", path1);
                flag = 0;
            }
            if (!flag) {
                return flag;
            }
            flag = star_file_is_text(path1, logLevel, from1, to1);
        } else {
            if (!io.isDirExists(path1)) {
                printf("Not exists %s!\n", path1);
                flag = 0;
            }
            if (!flag) {
                return flag;
            }
            std::stringstream ss;
            MatStr matRows;
            ss.str("");
            ss << s_exclude_suffixes;
            star_data_parse_csv_like_to_MatStr(ss, matRows);
            _Set<std::string> exclude_suffixes;
            exclude_suffixes.add(matRows);
            
            flag = star_file_folder_is_text(path1, exclude_suffixes, logLevel);
        }
        if (flag) {
            printf("Yes\n%s\n", path1);
        } else {
            printf("No\n%s\n", path1);
        }
        return flag;
    }
}