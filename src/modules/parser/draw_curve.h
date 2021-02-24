#pragma once

#include "inner.h"

namespace star {
    void ml_img_draw_and_save(const char *data_path, const char *image_path, NumN axis_x, NumN axis_y);

    void ml_img_draw_and_save_csv_like(const char *data_path, const char *image_path, NumN axis_x, NumN axis_y,
                                       char character = ',');

    void ml_img_draw_and_save_csv_like_multiple(const char *data_config_path, const char *image_path, NumN axis_x,
                                                NumN axis_y);

    void ml_img_draw_and_save_data_2d_multiple(const char *data_config_path, const char *image_path, NumN axis_x,
                                               NumN axis_y);

    void math21_img_plot_data_2d_with_fixed_T(const MatR &batch_data, const MatR &M,
                                              const VecN &color, TenR &image,
                                              NumN axis_x,
                                              NumN axis_y);

    void math21_img_plot_data_2d(const MatR &batch_data,
                                 const VecN &color, TenR &image,
                                 NumN axis_x,
                                 NumN axis_y);

    void star_img_draw_and_save_data_2d(const MatR &data,
                                        const VecN &color, const char *image_path,
                                        NumN axis_x,
                                        NumN axis_y);

    void math21_img_plot_data_2d_multiple_with_fixed_T(const TenR &batch_data, const MatR &M,
                                                       const Seqce <VecN> &colors, TenR &image,
                                                       NumN axis_x,
                                                       NumN axis_y);

    void math21_img_plot_data_2d_multiple(const TenR &batch_data,
                                          const Seqce <VecN> &colors, TenR &image,
                                          NumN axis_x,
                                          NumN axis_y);

    void star_img_draw_and_save_data_2d_multiple(const TenR &batch_data,
                                                 const Seqce <VecN> &colors, const char *image_path,
                                                 NumN axis_x,
                                                 NumN axis_y);

    void test_ml_img_draw_and_save_data_2d();

    int star_ml_img_draw_and_save_heart(int argc, const char **argv);

    void img_draw_test();

    void img_draw_curve_test();

    void img_draw_and_save_test();
}