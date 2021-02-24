#pragma once

#include "inner.h"

namespace star {

    std::string star_string_trim(const std::string &s);

    NumB star_string_is_null_or_spaces(const std::string &str);

    void string_test();

    int file_replace_character(std::ifstream &in, std::ofstream &out, char ch_src, char ch_dst);

    int ml_file_replace_character(const char *file_path_src, const char *file_path_dst, char ch_src,
                                  char ch_dst);

    //Not perfect.
    bool xjStrEndsWith(const char *path, const char *suf);

    bool xjStrCmp(const char *s1, const char *s2);

    NumB star_string_is_start_with(const std::string &s, const std::string &start);

    NumB star_str_suffix_index(const char *path, int &index0);

    std::string star_string_remove_suffix(const std::string &s);

    std::string star_string_get_suffix(const std::string &s);

    std::string star_string_remove_file_name(const std::string &s);

    std::string star_string_get_file_parent(const std::string &s);

    std::string star_string_get_file_name(const std::string &s);

    std::string star_string_get_file_base_name(const std::string &s);

    NumB star_string_is_containing(const std::string &s, char c);

    NumB star_string_get_word_1_from_start(const std::string &s, std::string &t1, char character);

    NumB star_string_get_word_2_from_start(const std::string &s, std::string &t1, std::string &t2, char ch1, char ch2);

    NumZ star_string_find_first(const std::string &s, char c);

    std::string star_string_get_file_name_without_suffix(const std::string &s);

    NumN star_string_args_find_NumN(int argc, const char **argv, const char *s, NumN def);

    NumZ star_string_args_find_NumZ(int argc, const char **argv, const char *s, NumZ def);

    NumR star_string_args_find_NumR(int argc, const char **argv, const char *s, NumR def);

    const char *star_string_args_find_c_string(int argc, const char **argv, const char *s, const char *def);

    std::string star_string_args_find_string(int argc, const char **argv, const char *s, const char *def);

    std::string
    star_string_get_assign_right(const std::string &s, const std::string &name, const std::string &def = "");

    NumB star_string_is_absolute_path(const std::string &s);

    std::string star_string_home_path(const std::string &s);

    std::string star_string_sky_path(const std::string &s);

    std::string star_string_to_save_path(const std::string &s);

    std::string star_string_get_current_time(const char *separator = 0);

    std::string star_string_get_current_time_for_print();

    struct SatisfyFunction_string {
        SatisfyFunction_string() {
        }

        NumB satisfy(const std::string &s) {
            return 1;
        }
    };

    // if x is satisfied, it will start a new line.
    template<typename SatisfyFunction>
    void star_string_1d_to_2d(const Seqce<std::string> &input, Seqce<Seqce<std::string>> &output, SatisfyFunction &f) {
        output.clear();
        Seqce<std::string> v;
        for (NumN i = 1; i <= input.size(); ++i) {
            if (f.satisfy(input(i))) {
                if (!v.isEmpty()) {
                    output.push(v);
                    v.clear();
                }
            }
            v.push(input(i));
        }
        if (!v.isEmpty()) {
            output.push(v);
            v.clear();
        }
    }

    // if x is satisfied, it will be left out.
    template<typename SatisfyFunction>
    void star_string_1d_remove_element(const Seqce<std::string> &input,
                                       Seqce<std::string> &output, SatisfyFunction &f) {
        output.clear();
        for (NumN i = 1; i <= input.size(); ++i) {
            if (!f.satisfy(input(i))) {
                output.push(input(i));
            }
        }
    }
}