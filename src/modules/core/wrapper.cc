#include "inner_cc.h"
#include "wrapper.h"

using namespace star;

int sky_ml_img_draw_and_save(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tsky_ml_img_draw_and_save data_path image_path axis_x axis_y\n";
            std::cout << "\t\tdefault: axis_x = 50, axis_y = 30\n";
            std::cout << "\t\tdata is text, cols: 1 or 2. format:\n"
                         "\t\tcols: 2\n"
                         "\t\t1 2\n"
                         "\t\t3 4\n"
                         "\t\t5 6\n";
            return 1;
        }
    }
    if (argc == 3) {
        ml_img_draw_and_save(argv[1], argv[2], 50, 30);
    } else if (argc == 5) {
        ml_img_draw_and_save(argv[1], argv[2], NumN(atoi(argv[3])), NumN(atoi(argv[4])));
    } else {
        return 0;
    }
    return 1;
}

int sky_ml_img_draw_and_save_csv_like(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tsky_ml_img_draw_and_save_csv_like data_path image_path\n"
                         "\taxis_x axis_y character\n";
            std::cout << "\t\tdefault: axis_x = 50, axis_y = 30\n";
            std::cout << "\t\tdefault: character is ascii code, e.x.,\n"
                         "\t\t character = 44 which represents ','\n";
            std::cout << "\t\tdata is csv like text. format:\n"
                         "\t\t1, 2,\n"
                         "\t\t4, 5, 6\n"
                         "\t\t7, 8, 9\n";
            return 1;
        }
    }
    const char *data_path = star_string_args_find_c_string(argc, argv, "data_path", 0);
    const char *image_path = star_string_args_find_c_string(argc, argv, "image_path", 0);
    NumN axis_x = star_string_args_find_NumN(argc, argv, "axis_x", 50);
    NumN axis_y = star_string_args_find_NumN(argc, argv, "axis_y", 30);
    char character = (char )star_string_args_find_NumN(argc, argv, "character", ',');
    if(!data_path){
        m21log("data_path is not set!");
        return 0;
    }
    if(!image_path){
        m21log("image_path is not set!");
        return 0;
    }
    ml_img_draw_and_save_csv_like(data_path, image_path, axis_x, axis_y, character);
    return 1;
}

int sky_ml_img_draw_and_save_csv_like_multiple(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tsky_ml_img_draw_and_save_csv_like_multiple data_config_path\n"
                         "\timage_path axis_x axis_y\n";
            std::cout << "\t\tdefault: axis_x = 50, axis_y = 30\n";
            std::cout << "\t\tdata_config is text, format:\n"
                         "\t\tbg color\n"
                         "\t\tdata path, fg color, character_ascii\n"
                         "\t\te.x.:\n"
                         "\t\tdata_01_path, 255, 255, 255, 44\n"
                         "\t\tdata_02_path, 0, 255, 255, 44\n"
                         "\t\tdata_03_path, 255, 255, 0, 44\n";
            return 1;
        }
    }
    if (argc == 3) {
        ml_img_draw_and_save_csv_like_multiple(argv[1], argv[2], 50, 30);
    } else if (argc == 5) {
        ml_img_draw_and_save_csv_like_multiple(argv[1], argv[2], NumN(atoi(argv[3])), NumN(atoi(argv[4])));
    } else {
        return 0;
    }

    return 1;
}

int sky_ml_file_replace_character(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h") ||
            math21_string_is_equal(argv[1], "ml_file_replace_character")) {
            std::cout << "\tsky_ml_file_replace_character file_path_src\n"
                         "\tfile_path_dst char_src char_dst\n";
            std::cout << "\t\tNote: char_src, char_dst are ascii code\n";
            return 1;
        }
    }

    if (argc == 5) {
        ml_file_replace_character(argv[1], argv[2], char(atoi(argv[3])), char(atoi(argv[4])));
    } else {
        return 0;
    }

    return 1;
}

int sky_ml_img_draw_and_save_data_2d_multiple(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tsky_ml_img_draw_and_save_data_2d_multiple data_config_path\n"
                         "\timage_path axis_x axis_y\n";
            std::cout << "\t\tdefault: axis_x = 50, axis_y = 30\n";
            std::cout << "\t\tdata_config is text, format:\n"
                         "\t\tbg color\n"
                         "\t\tdata path, fg color, character_ascii\n"
                         "\t\te.x.:\n"
                         "\t\tdata_01_path, 255, 255, 255, 44\n"
                         "\t\tdata_02_path, 0, 255, 255, 44\n"
                         "\t\tdata_03_path, 255, 255, 0, 44\n";
            return 1;
        }
    }

    if (argc == 3) {
        ml_img_draw_and_save_data_2d_multiple(argv[1], argv[2], 50, 30);
    } else if (argc == 5) {
        ml_img_draw_and_save_data_2d_multiple(argv[1], argv[2], math21_string_to_NumN(argv[3]),
                                              math21_string_to_NumN(argv[4]));
    } else {
        return 0;
    }
    return 1;
}

int sky_test5(int argc, const char **argv) {
    m21log(__FUNCTION__);

    NumR t = math21_time_getticks();

    TenR img, histogram;
//    img_read(img, "/mathxyz/assets/stitching/11/2019-08-13.09-21-51.jpg");
    img_read(img, "/mathxyz/rl_py/images/example_6_2.png");
//    math21_img_histogram(img, histogram);
//    math21_img_gamma(img, histogram);
//    math21_operator_container_linear_kx_b_to(0.5, img, 0);
//    math21_operator_container_clip_in(img, 100, 200);

//    img_write(img, "img.jpg");
    img_write(img, "img.png");
//    ImageDraw_Tensor f;
//    f.draw(histogram, "histogram");

    t = math21_time_getticks() - t;
    m21log("time", t);

    return 1;
}

int star_image_rotate_test_01(const char *input, const char *output) {
    m21log(__FUNCTION__);
    NumR t = math21_time_getticks();

    TenR A, B;
    img_read(A, input);
    VecR center(2);
    center = 0, 0;
    NumR angle;
    angle = -MATH21_PI / 6;
    MatR T;
    math21_la_getRotationMatrix2D(center, angle, T);
    B.setSize(A.dim(1), 2 * A.dim(2), 2 * A.dim(3));
    math21_la_affine_transform_image(A, B, T);
    img_write(B, output);

    t = math21_time_getticks() - t;
    m21log("time", t);
    return 1;
}

int star_image_rotate_test_02(const char *input, const char *output) {
    m21log(__FUNCTION__);
    NumR t = math21_time_getticks();

    TenR A, B;
    img_read(A, input);
    VecR center(2);
    center = 256, 0;
    NumR angle;
    angle = MATH21_PI / 6;
    MatR T;
    math21_la_translate_and_rotate_axis_reverse_mode(center, angle, T);
    B.setSize(A.dim(1), 2 * A.dim(2), 2 * A.dim(3));
    math21_la_affine_transform_image_reverse_mode(A, B, T);
    img_write(B, output);

    t = math21_time_getticks() - t;
    m21log("time", t);
    return 1;
}

int star_image_rotate_test_03(const char *input, const char *output) {
    m21log(__FUNCTION__);
    NumR t = math21_time_getticks();

    TenR A, B;
    img_read(A, input);
    VecR center(2);
    center = 200, 0;
    NumR angle;
    angle = MATH21_PI / 6;
    MatR T;
    math21_la_rotate_and_translate_point(center, angle, T);
    B.setSize(A.dim(1), 2 * A.dim(2), 2 * A.dim(3));
    math21_la_affine_transform_image(A, B, T);
    img_write(B, output);

    t = math21_time_getticks() - t;
    m21log("time", t);
    return 1;
}

int star_image_rotate_test_04(const char *input, const char *output) {
    m21log(__FUNCTION__);
    NumR t = math21_time_getticks();

    TenR A, B;
    img_read(A, input);
    NumR angle;
    angle = MATH21_PI / 6;
    MatR T(4, 4);
    NumR f, x0, y0;
    f = 200;
    x0 = 200;
    y0 = 200;
    T =
            f, 0, x0, 0,
            0, f, y0, 0,
            0, 0, 1, 0;

    math21_la_3d_post_rotate_axis_about_y(angle, T);
    B.setSize(A.dim(1), 2 * A.dim(2), 2 * A.dim(3));
    math21_la_affine_transform_image(A, B, T);
    img_write(B, output);

    t = math21_time_getticks() - t;
    m21log("time", t);
    return 1;
}

int star_image_rotate_test_05(const char *input, const char *output) {
    m21log(__FUNCTION__);
    NumR t = math21_time_getticks();

    TenR A, B;
    img_read(A, input);
    NumR angle;
    angle = MATH21_PI / 6;
    MatR T(4, 4);
    math21_operator_mat_eye(T);
    math21_la_3d_post_rotate_axis_about_x(angle, T);
    B.setSize(A.dim(1), 2 * A.dim(2), 2 * A.dim(3));
    if (!math21_la_3d_perspective_projection_image(A, B, T)) {
        return 0;
    }
    img_write(B, output);

    t = math21_time_getticks() - t;
    m21log("time", t);
    return 1;
}

int math21_la_matrix_test(const char *input, const char *output) {
    m21log(__FUNCTION__);
    timer t;
    t.start();

    // read
    TenR A, B;
    img_read(A, input);
    B.setSize(A.dim(1), A.dim(2), A.dim(3));
    B = A;

    math21_la_3d_matrix_test(A, B);

    // write
    img_write(B, output);
    t.end();
    m21log("time", t.time());
    return 1;
}

int sky_test(int argc, const char **argv) {
    m21log(__FUNCTION__);
//    math21_la_matrix_test("D:\\mathxyz\\assets\\orange.jpg",
//                              "D:\\mathxyz\\assets\\z.jpg");

//    math21_la_matrix_test("/mathxyz/assets/orange.png",
//                          "/mathxyz/assets/z2.png");

    img_draw_test();
    return 1;
}