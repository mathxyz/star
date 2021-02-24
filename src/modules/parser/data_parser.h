#pragma once

#include "inner.h"

namespace star {

    //////////// text

    void ml_data_save_as_vector_using_index(std::ostream &myfile, const Seqce<TenR> &data, const VecN &indexes,
                                            char character_row, char character_col);

    void ml_data_save_as_vector_using_index_multiple(const Seqce<TenR> &data, const Seqce<VecN> &labels,
                                                     const char *data_config_path, const char *dir,
                                                     char character_row, char character_col);

    void ml_data_save_as_vector(std::ostream &myfile, const Seqce<TenR> &A,
                                char character_row = 10, char character_col = 32);

    void ml_data_save_as_vector(const Seqce<TenR> &A, const char *data_dst,
                                char character_row = 10, char character_col = 32);

    void ml_data_save_as_vector(std::ostream &myfile, const TenR &A, char character = 10);

    void ml_data_save_as_vector(const TenR &A, const char *data_dst, char output_character = 10);

    void ml_data_parse_as_vector(std::istream &input, Seqce<TenR> &A,
                                 char character_row = 10, char character_col = 32);

    void ml_data_parse_as_vector(Seqce<TenR> &A, const char *data_path,
                                 char character_row = 10, char character_col = 32);

    void ml_data_parse_as_vector(std::istream &io, TenR &A, char character);

    void ml_data_parse_as_vector(TenR &A, const char *data_path, char character = 10);


    //////////// image

    void ml_data_read_from_img(Seqce<TenR> &data, const char *data_config_path, NumN max_size,
                               const char *data_config_path_prefix = 0);

    void data_vector_read_from_png(Seqce<VecR> &data, const char *dir, NumN dataset_size, NumN max_size);

    void data_read_from_png(Seqce<TenR> &data, const char *dir, NumN dataset_size, NumN max_size);

    void ml_data_write_to_img(const Seqce<TenR> &data, const char *data_config_path, const char *dir,
                              const char *img_suffix = ".png");

    void data_vector_write_to_png(const Seqce<VecR> &data, const char *dir, NumN r, NumN c);

    void data_scale(Seqce<TenR> &data, NumR a, NumR b, NumR c, NumR d);

    void data_scale(Seqce<VecR> &data, NumR a, NumR b, NumR c, NumR d);

    void img_resize_test(const Seqce<TenR> &images);
}