#include <fstream>
#include <cstring>

#include "star_string.h"

namespace star {

    const std::string WHITESPACE = " \n\r\t\f\v";

    std::string ltrim(const std::string &s) {
        size_t start = s.find_first_not_of(WHITESPACE);
        return (start == std::string::npos) ? "" : s.substr(start);
    }

    std::string rtrim(const std::string &s) {
        size_t end = s.find_last_not_of(WHITESPACE);
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }

    std::string star_string_trim(const std::string &s) {
        return rtrim(ltrim(s));
    }

    // str: "", " ", "   ", or "\r\n".
    NumB star_string_is_null_or_spaces(const std::string &str) {
        if (star_string_trim(str).empty()) {
            return 1;
        } else {
            return 0;
        }
    }

    NumB star_string_is_start_with(const std::string &s, const std::string &start) {
        if (s.find(start) == 0) {
            return 1;
        } else {
            return 0;
        }
    }

    int file_replace_character(std::ifstream &in, std::ofstream &out, char ch_src, char ch_dst) {
        if (!in || !out)
            return 0;

        char c;
        while (in.get(c)) {
            if (c == ch_src) {
                out << ch_dst;
            } else {
                out << c;
            }
        }
        return 1;
    }


    int ml_file_replace_character(const char *file_path_src, const char *file_path_dst, char ch_src,
                                  char ch_dst) {
        std::ifstream in(file_path_src, std::ifstream::in);
        std::ofstream out(file_path_dst, std::ofstream::out);
        if (!in.is_open()) {
            printf("open %s fail!\n", file_path_src);
            return 0;
        }
        if (!out.is_open()) {
            printf("open %s fail!\n", file_path_dst);
            return 0;
        }
        file_replace_character(in, out, ch_src, ch_dst);
        in.close();
        out.close();

        if (out) {
            return 1;
        } else {
            return 0;
        }
    }

    NumB star_string_is_absolute_path(const std::string &s) {
        if (star_string_is_start_with(s, "/")) {
            return 1;
        } else {
            return 0;
        }
    }

    std::string star_string_home_path(const std::string &s) {
        if (star_string_is_absolute_path(s)) {
            return s + "/Alice";
        } else {
            return "/sdcard/Alice";
        }
    }

    std::string star_string_sky_path(const std::string &s) {
        if (star_string_is_absolute_path(s)) {
            return s + "/Alice/sky";
        } else {
            return "/sdcard/Alice/sky";
        }
    }

    std::string star_string_to_save_path(const std::string &s) {
        if (star_string_is_start_with(std::string(s), "/")) {
            return s;
        }

#ifdef MATH21_ANDROID
        return "/sdcard/Alice/sky/" + s;
#else
        return s;
#endif
    }

    //////////////////////////

    //Not perfect.
    bool xjStrEndsWith(const char *path, const char *suf) {
        int index;
        if (star_str_suffix_index(path, index) == 0)
            return 0;
        if (xjStrCmp(path + index, suf))
            return 1;
        return 0;
    }

    bool xjStrCmp(const char *s1, const char *s2) {
        if (strcmp(s1, s2) == 0) {
            return XJ_TRUE;
        } else {
            return XJ_FALSE;
        }
    }

    // path is not null
    // return 0 if path is empty, 1 success
    NumB star_str_suffix_index(const char *path, int &index0) {
        return math21_string_find_last(path, '.', index0);
    }

    std::string star_string_remove_suffix(const std::string &s) {
        int index;
        std::string s2;
        NumB flag = star_str_suffix_index(s.c_str(), index);
        if (flag == 0) {
            index = (int) s.size();
        }
        if (index == 0) {
            return s2;
        }
        s2 = s.substr(0, (size_t) index);
        return s2;
    }

    std::string star_string_get_suffix(const std::string &s) {
        int index;
        std::string s2;
        NumB flag = star_str_suffix_index(s.c_str(), index);
        if (flag == 0) {
            return s2;
        }
        s2 = s.substr(index+1, s.size());
        return s2;
    }

    std::string star_string_base_name(const std::string &s) {
        int index;
        std::string s2;
        NumB flag = math21_string_find_last(s.c_str(), '.', index);
        if (flag == 0) {
            index = (int) s.size();
        }
        if (index == 0) {
            return s2;
        }
        s2 = s.substr(0, (size_t) index);
        return s2;
    }

    std::string star_string_remove_file_name(const std::string &s) {
        return star_string_get_file_parent(s);
    }

    std::string star_string_get_file_parent(const std::string &s) {
        int index;
        std::string s2;
        NumB flag = math21_string_find_last(s.c_str(), '/', index);

        // if not find, or find at first index, return empty.
        if (flag == 0 || index == 0) {
            return s2;
        }
        s2 = s.substr(0, (size_t) index);
        return s2;
    }

    std::string star_string_get_file_name(const std::string &s) {
        int index;
        std::string s2;
        NumB flag = math21_string_find_last(s.c_str(), '/', index);

        // if not find, we return s directly.
        if (flag == 0) {
            return s;
        }

        // if find, but at end, return empty.
        if (index == s.size() - 1) {
            return s2;
        }
        s2 = s.substr((size_t) index + 1, s.size());
        return s2;
    }

    std::string star_string_get_file_base_name(const std::string &s) {
        std::string name = star_string_get_file_name(s);
        name = star_string_remove_suffix(name);
        LightVector<char> v(name.size(), &name[0]);
        NumN index;
        if (!math21_operator_container_find_first(v, '.', index)) {
            return name;
        } else {
            if (index == 1) {
                return "";
            }
            std::string newName;
            newName.resize(index - 1);
            LightVector<char> w(index - 1, &newName[0]);
            math21_operator_container_subcontainer(v, w, 1, index - 1);
            return newName;
        }
    }

    NumB star_string_is_containing(const std::string &s, char c) {
        const LightVector<char> v(s.size(), &s[0]);
        NumN index;
        if (!math21_operator_container_find_first(v, c, index)) {
            return 0;
        } else {
            return 1;
        }
    }

    NumB star_string_get_word_1_from_start(const std::string &s, std::string &t1, char character) {
        std::istringstream iss(s);
        std::string t;
        NumN i = 0;
        while (getline(iss, t, character)) {
            t = star_string_trim(t);
            if (t.empty()) {
                continue;
            }
            ++i;
            t1 = t;
            break;
        }
        if (i == 1) {
            return 1;
        }
        return 0;
    }

    NumB star_string_get_word_2_from_start(const std::string &s, std::string &t1, std::string &t2, char ch1, char ch2) {
        std::istringstream iss(s);
        std::string t;
        char ch;
        NumN i = 0;
        ch = ch1;
        while (getline(iss, t, ch)) {
            t = star_string_trim(t);
            if (t.empty()) {
                continue;
            }
            ++i;
            if (i == 1) {
                t1 = t;
                ch = ch2;
                continue;
            } else if (i == 2) {
                t2 = t;
            }
            break;
        }
        if (i == 2) {
            return 1;
        }
        return 0;
    }

    NumZ star_string_find_first(const std::string &s, char c) {
        const LightVector<char> v(s.size(), &s[0]);
        NumN index;
        if (!math21_operator_container_find_first(v, c, index)) {
            return -1;
        } else {
            return index - 1;
        }
    }

    std::string star_string_get_file_name_without_suffix(const std::string &s) {
        std::string name = star_string_get_file_name(s);
        name = star_string_remove_suffix(name);
        return name;
    }


    NumN star_string_args_find_index(int argc, const char **argv, const char *s) {
        LightVector<const char *> v((NumN) argc, argv);
        NumN index;
        detail_li::Compare_ss comp;
        if (!math21_operator_container_find_first_using_compare(v, s, index, comp)) {
            return 0;
        }
        return index;

    }

    NumN star_string_args_find_NumN(int argc, const char **argv, const char *s, NumN def) {
        LightVector<const char *> v((NumN) argc, argv);
        NumN index;
        detail_li::Compare_ss comp;
        if (!math21_operator_container_find_first_using_compare(v, s, index, comp)) {
            return def;
        }
        return math21_string_to_NumN(argv[index]);
    }

    NumZ star_string_args_find_NumZ(int argc, const char **argv, const char *s, NumZ def) {
        LightVector<const char *> v((NumN) argc, argv);
        NumN index;
        detail_li::Compare_ss comp;
        if (!math21_operator_container_find_first_using_compare(v, s, index, comp)) {
            return def;
        }
        return math21_string_to_NumZ(argv[index]);
    }

    NumR star_string_args_find_NumR(int argc, const char **argv, const char *s, NumR def) {
        LightVector<const char *> v((NumN) argc, argv);
        NumN index;
        detail_li::Compare_ss comp;
        if (!math21_operator_container_find_first_using_compare(v, s, index, comp)) {
            return def;
        }
        return math21_string_to_NumR(argv[index]);
    }

    const char *star_string_args_find_c_string(int argc, const char **argv, const char *s, const char *def) {
        LightVector<const char *> v((NumN) argc, argv);
        NumN index;
        detail_li::Compare_ss comp;
        if (!math21_operator_container_find_first_using_compare(v, s, index, comp)) {
            return def;
        }
        return argv[index];
    }

    std::string star_string_args_find_string(int argc, const char **argv, const char *s, const char *def) {
        LightVector<const char *> v((NumN) argc, argv);
        NumN index;
        detail_li::Compare_ss comp;
        if (!math21_operator_container_find_first_using_compare(v, s, index, comp)) {
            return def;
        }
        return argv[index];
    }

    std::string star_string_get_assign_right(const std::string &s, const std::string &key, const std::string &def) {
        if (star_string_is_start_with(s, key + "=")) {
            return s.substr(key.length() + 1);;
        } else {
            fprintf(stderr, "%s: Using default '%s'\n", key.c_str(), def.c_str());
            return def;
        }
    }

    // Get current date/time, format is YYYY_MM_DD_HH_mm_ss
    std::string star_string_get_current_time(const char *format) {
        if (!format) {
            format = "%.4d_%.2d_%.2d_%.2d_%.2d_%.2d";
        }
        std::time_t t = std::time(0);   // get time now
        std::tm &now = *std::localtime(&t);
        char buf[32];
        sprintf(buf, format,
                (now.tm_year + 1900), (now.tm_mon + 1),
                now.tm_mday, now.tm_hour,
                now.tm_min, now.tm_sec);
        return buf;
    }

    std::string star_string_get_current_time_for_print() {
        const char *format = "%.4d-%.2d-%.2d %.2d:%.2d:%.2d";
        return star_string_get_current_time(format);
    }

}