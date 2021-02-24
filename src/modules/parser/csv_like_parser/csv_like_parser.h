#pragma once

#include "inner.h"

namespace star {
    typedef Seqce <Seqce<std::string>> StarCsvVector;
    typedef Seqce <std::string> StarCsvColumn;

    void star_log_csv(std::ostream &out, const char *name, const MatStr &csvData, char character_row = 10,
                      char character_col = ',');

    void star_log_csv(const char *name, const MatStr &csvData, char character_row = 10,
                      char character_col = ',');

    NumB data_csv_like_to_vector(std::vector<std::vector<std::string> > &csvData, Seqce <TenR> &A);

    NumB data_csv_like_to_mat_n_1(std::vector<std::vector<std::string> > &csvData, TenR &A);

    NumB data_csv_like_to_MatR(std::vector<std::vector<std::string> > &csvData, TenR &A);

    NumB star_data_csv_like_to_MatStr(StarCsvVector &csvData, MatStr &A);

    NumB star_data_csv_like_to_MatR(StarCsvVector &csvData, TenR &A);

    void ml_data_parse_and_save_csv_like(const char *data_src, const char *data_dst, char input_character,
                                         char output_character);

    void ml_data_parse_csv_like_n_1(const char *data_path, TenR &A, char character);

    void ml_data_parse_csv_like(const char *data_path, TenR &A, char character);

    void data_get_csv_like_sample_pair(const char *data_path, TenR &X_sample, TenR &Y_sample, char character);

    void data_get_csv_like_data_2d(const char *data_path, TenR &data, char character);

    NumB data_parse_csv_like(std::istream &input, std::vector<std::vector<std::string> > &output, char character_row,
                             char character_col);

    NumB star_data_parse_csv_to_StarCsvVector(std::istream &input, StarCsvVector &output, char character_row,
                                              char character_col);

    void CsvVector_to_StarCsvVector(const std::vector<std::vector<std::string> > &A, StarCsvVector &B);

    void math21_log_info_StarCsvVector(const StarCsvVector &A);

    void math21_log_StarCsvVector(const StarCsvVector &A);

    NumB star_data_parse_csv_like(std::istream &input, StarCsvVector &output, char character_row, char character_col);

    NumB star_data_parse_csv_like_no_space(std::istream &input, StarCsvVector &output, char character_row,
                                           char character_col);

    NumB star_data_parse_csv_like_to_MatStr(std::istream &input, MatStr &output,
                                            char character_row = 10,
                                            char character_col = ',');

    int sky_data_edit_csv_like(int argc, const char **argv);

    void star_csv_vector_remove_space(const StarCsvVector &A, StarCsvVector &B);

    // 1<=start<=end, default: 0
    NumB data_string_to_num(std::vector<std::string> &strings, VecR &A, NumN start = 0, NumN end = 0);
}