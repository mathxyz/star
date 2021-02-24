#include "files.h"
#include "draw.h"

namespace star {

    void ImageDraw_Tensor::init() {
        axis_x = 200;
        axis_y = 150;

        color_bg.setSize(3);
        color_bg = 21, 21, 21;
        T.setSize(3, 3);

        reset();
    }


    void ImageDraw_Tensor::reset() {
        img_init_image(image, axis_x, axis_y, color_bg, 1);
        colors_traj.clear();
        isTSet = 0;
    }

    ImageDraw_Tensor::ImageDraw_Tensor() {
        init();
    }

    void ImageDraw_Tensor::setAxisSize(NumN axis_x, NumN axis_y) {
        this->axis_x = axis_x;
        this->axis_y = axis_y;
    }

    // deprecate
    void ImageDraw_Tensor::draw(const TenR &A, const char *name, NumB isBatch) {
        if (!name) {
            name = "A";
        }
        Seqce<VecN> colors(A.dim(1));
        for (NumN i = 1; i <= colors.size(); ++i) {
            colors.at(i).setSize(3);
            colors.at(i) = 255, 255, 0;
        }
        if (colors.size() >= 1) {
            colors.at(1) = 255, 0, 0;
        }
        if (colors.size() >= 2) {
            colors.at(2) = 0, 255, 0;
        }
        if (colors.size() >= 3) {
            colors.at(3) = 0, 0, 255;
        }

        std::string name2 = name;
        name2 = name2 + ".png";
        if (!dir.empty()) {
            dir = star_string_to_save_path(dir);
            Io io;
            io.mkdir(dir);
            name2 = dir + "/" + name2;
        }
        name2 = star_string_to_save_path(name2);
        if (isBatch) {
            star_img_draw_and_save_data_2d_multiple(A,
                                                    colors, name2.c_str(),
                                                    axis_x,
                                                    axis_y);
        } else {
            star_img_draw_and_save_data_2d(A,
                                           colors(1), name2.c_str(),
                                           axis_x,
                                           axis_y);

        }

    }

    void ImageDraw_Tensor::setDir(const char *name) {
        dir = name;
    }

    // don't save
    void ImageDraw_Tensor::plot(const TenR &data, const char *name, NumB isBatch) {
        if (!name) {
            name = "A";
        }
        Seqce<VecN> colors;
        RanUniform ranUniform;
        ranUniform.set(0, 255);
        if (isBatch) {
            colors.setSize(data.dim(1));
            math21_plot_set_color(colors);
        } else {
            colors.setSize(1);
            colors.at(1).setSize(3);
            if (colors_traj.size() < 7) {
                if (colors_traj.size() == 0) {
                    colors.at(1) = 255, 0, 0;
                } else if (colors_traj.size() == 1) {
                    colors.at(1) = 0, 255, 0;
                } else if (colors_traj.size() == 2) {
                    colors.at(1) = 0, 0, 255;
                } else if (colors_traj.size() == 3) {
                    colors.at(1) = 255, 255, 0;
                } else if (colors_traj.size() == 4) {
                    colors.at(1) = 255, 0, 255;
                } else if (colors_traj.size() == 5) {
                    colors.at(1) = 0, 255, 255;
                } else if (colors_traj.size() == 6) {
                    colors.at(1) = 255, 255, 255;
                }
            } else {
                math21_random_draw(colors.at(1), ranUniform);
            }
        }

        colors_traj.push(colors);

        std::string name2 = name;
        name2 = name2 + ".png";
        if (!dir.empty()) {
            dir = star_string_to_save_path(dir);
            Io io;
            io.mkdir(dir);
            name2 = dir + "/" + name2;
        }
        name2 = star_string_to_save_path(name2);

        get_T(data, isBatch);
        if (isBatch) {
            math21_img_plot_data_2d_multiple_with_fixed_T(data, T,
                                                          colors, image,
                                                          axis_x, axis_y);
        } else {
            math21_img_plot_data_2d_with_fixed_T(data, T,
                                                 colors(1), image, axis_x, axis_y);
        }
    }

    void ImageDraw_Tensor::get_T(const TenR &data, NumB isBatch) {

        Interval2D &I_old = I;
        Interval2D I_new;
        Interval2D I_data;
        if (isBatch) {
            math21_la_data_2d_bound_in_batch(data, I_data);
        } else {
            math21_la_data_2d_bound(data, I_data);
        }
        if (!isTSet) {
            I_old = I_data;
            I_new = I_data;
            isTSet = 1;
        } else {
            math21_algebra_interval2d_union(I_old, I_data, I_new);
        }

        Interval2D I_matrix_axis_old;
        I_matrix_axis_old(1).set(1, axis_x, 1, 1);
        I_matrix_axis_old(2).set(1, axis_y, 1, 1);

        Interval2D I_matrix_axis_new;
        I_matrix_axis_new = I_matrix_axis_old;

        if (!I_old.isEqual(I_new)) {
            TenR image_new;
            img_init_image(image_new, axis_x, axis_y, color_bg, 1);

            MatR T_final, T;
            T_final.setSize(3, 3);
            math21_operator_mat_eye(T_final);

            // I_image_old -> I_matrix_axis_old -> I_old -> I_new -> I_matrix_axis_new -> I_image_new
            math21_la_2d_matrix_compute_matrix_to_matrix_axis(axis_x, axis_y, T);
            math21_operator_multiply_to_B(1, T, T_final);
            math21_la_getScaleTranslationMatrix2D(T, I_matrix_axis_old, I_old);
            math21_operator_multiply_to_B(1, T, T_final);
            math21_operator_mat_eye(T);
            math21_operator_multiply_to_B(1, T, T_final);
            math21_la_getScaleTranslationMatrix2D(T, I_new, I_matrix_axis_new);
            math21_operator_multiply_to_B(1, T, T_final);
            math21_la_2d_matrix_compute_matrix_axis_to_matrix(axis_x, axis_y, T);
            math21_operator_multiply_to_B(1, T, T_final);

            // transform
            math21_la_2d_affine_transform_image(image, image_new, T_final);

            image.swap(image_new);
        }
        math21_la_getScaleTranslationMatrix2D(T, I_new, I_matrix_axis_new);
        I = I_new;
    }

    void ImageDraw_Tensor::save(const char *name) {
        std::string name2 = name;
        name2 = name2 + ".png";
        if (!dir.empty()) {
            dir = star_string_to_save_path(dir);
            Io io;
            io.mkdir(dir);
            name2 = dir + "/" + name2;
        }
        name2 = star_string_to_save_path(name2);
        img_write(image, name2.c_str());
    }

    void star_plot_container_data(const Seqce<MatR> &data, const char *path) {
        ImageDraw_Tensor f;
        f.reset();
        for (NumN i = 1; i <= data.size(); ++i) {
            f.plot(data(i), "", 0);
        }
        f.save(path);
    }

    int sky_plot_data(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tsky_plot_data \n"
                       "\tdata_path src_img_path(null) dst_img_path\n"
                       "\t  --i (NumN)\n"
                       "\t      plot i-th mat in container.\n"
                       "\t      or plot i-th point in mat.\n"
                       "\t      The default value is 'null'.\n"
                       "\t  --j (NumN)\n"
                       "\t      plot j-th point in data.\n"
                       "\t      The default value is 'null'.\n"
                       "\t  --data_format (default | text_2d)\n"
                       "\t      The default value is 'default'.\n"
                       "\t  --print\n"
                );
                m21log("\te.x., \n"
                       "\tsky_plot_data a.bin a.png\n");
                return 1;
            }
        }
        NumB isLog = 0;
        std::string data_format = "default";
        NumN index_i = 0;
        NumN index_j = 0;
        NumN j = 0;
        for (int i = 1; i < argc; ++i) {
            if (math21_string_is_equal(argv[i], "--print")) {
                isLog = 1;
            } else if (math21_string_is_equal(argv[i], "--data_format")) {
                data_format = argv[i + 1];
                ++i;
            } else if (math21_string_is_equal(argv[i], "--i")) {
                index_i = math21_string_to_NumN(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--j")) {
                index_j = math21_string_to_NumN(argv[i + 1]);
                ++i;
            } else {
                ++j;
            }
        }
        if (j < 3) {
            return 0;
        }

        const char *filePath = argv[1];
        NumN type;
        type = 0;
        math21_io_read_type_from_file(filePath, type);
        if (type == m21_type_Seqce) {
            data_format = "container_mat";
        } else if (type == m21_type_Tensor) {
            data_format = "mat";
        }

        if (math21_string_is_equal(data_format, "container_mat")) {
            Seqce<MatR> data;
            if (!math21_io_generic_type_read_from_file(data, filePath, 1)) {
                return 0;
            }
            if (math21_string_is_equal(argv[2], "null")) {
                star_plot_container_data(data, argv[3]);
            } else {
                TenR A;
                img_read(A, argv[2]);

                if (isLog) {
                    A.logInfo(argv[2]);
                }
                if (isLog) {
                    data.log("data");
                }
                math21_plot_container_with_option(data, A, index_i, index_j);
                img_write(A, argv[3]);
            }
        } else if (math21_string_is_equal(data_format, "mat")) {
            MatR data;
            if (!math21_io_generic_type_read_from_file(data, filePath, 1)) {
                return 0;
            }
            if (math21_string_is_equal(argv[2], "null")) {
                return 0;
            }
            TenR A;
            img_read(A, argv[2]);

            if (isLog) {
                A.logInfo(argv[2]);
            }
            if (isLog) {
                data.log("data");
            }
            NumN side = xjmin(math21_image_get_nr(A), math21_image_get_nc(A));
            NumR radius = -(side / 100.0);
            VecN color(3);
            math21_plot_set_color_red(color);
            math21_plot_mat_data_with_option(data, A, color, radius, index_i);
            img_write(A, argv[3]);
        } else if (math21_string_is_equal(data_format, "text_2d")) {
            MatR data;
            data_get_csv_like_data_2d(filePath, data, ',');

            if (math21_string_is_equal(argv[2], "null")) {
                return 0;
            }
            TenR A;
            img_read(A, argv[2]);

            if (isLog) {
                A.logInfo(argv[2]);
            }
            if (isLog) {
                data.log("data");
            }
            NumN side = xjmin(math21_image_get_nr(A), math21_image_get_nc(A));
            NumR radius = -(side / 100.0);
            VecN color(3);
            math21_plot_set_color_red(color);
            math21_plot_mat_data(data, A, color, radius);
            img_write(A, argv[3]);
        }

        return 1;
    }
}