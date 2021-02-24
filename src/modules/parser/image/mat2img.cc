#include "../png_parser/io_png.h"
#include "mat2img.h"
#include "image.h"

namespace star {

    // deprecate
    void img_png_write(const MatR &m, const char *path) {
        NumN i, j;
        NumN img_i, img_j;
        NumN nr = m.nrows();
        NumN nc = m.ncols();
        MATH21_ASSERT(nr >= 1 && nc >= 1, "empty matrix");
        float *img = (float *) malloc(nr * nc * sizeof(float));
        for (i = 1, img_i = 0; i <= nr; i++, img_i++) {
            for (j = 1, img_j = 0; j <= nc; j++, img_j++) {
                img[img_i * nc + img_j] = (float) m(i, j);
            }
        }
        io_png_write_f32(path, img, nc, nr, 1);
        free(img);
    }

    void img_png_read(MatR &m, const char *path) {
        NumN i, j;
        NumN img_i, img_j;
        size_t nr, nc, nch;
        float *img = io_png_read_f32(path, &nc, &nr, &nch);
        if (img == 0) {
            printf("io_png_read_f32 fail!\n");
            return;
        }
        MATH21_ASSERT(nch == 1, "channel is not 1");
        MATH21_ASSERT(nr >= 1 && nc >= 1, "empty matrix");
        m.setSize(nr, nc);
        for (i = 1, img_i = 0; i <= nr; i++, img_i++) {
            for (j = 1, img_j = 0; j <= nc; j++, img_j++) {
                m(i, j) = (NumR) img[img_i * nc + img_j];
            }
        }
        free(img);
    }

    void img_png_read(MatN &m, const char *path) {
        NumN i, j;
        NumN img_i, img_j;
        size_t nr, nc, nch;
        unsigned char *img = io_png_read_u8(path, &nc, &nr, &nch);
        if (img == 0) {
            printf("io_png_read_f32 fail!\n");
            return;
        }
        MATH21_ASSERT(nch == 1, "channel is not 1");
        MATH21_ASSERT(nr >= 1 && nc >= 1, "empty matrix");
        m.setSize(nr, nc);
        for (i = 1, img_i = 0; i <= nr; i++, img_i++) {
            for (j = 1, img_j = 0; j <= nc; j++, img_j++) {
                m(i, j) = (NumN) img[img_i * nc + img_j];
            }
        }
        free(img);
    }

    void img_png_write(const MatN &m, const char *path) {
        NumN i, j;
        NumN img_i, img_j;
        NumN nr = m.nrows();
        NumN nc = m.ncols();
        MATH21_ASSERT(nr >= 1 && nc >= 1, "empty matrix");
        unsigned char *img = (unsigned char *) malloc(nr * nc * sizeof(unsigned char));
        for (i = 1, img_i = 0; i <= nr; i++, img_i++) {
            for (j = 1, img_j = 0; j <= nc; j++, img_j++) {
                img[img_i * nc + img_j] = (unsigned char) m(i, j);
            }
        }
        io_png_write_u8(path, img, nc, nr, 1);
        free(img);
    }

    //write matrix-represented function to image without changing old contents of image.
    void img_init_image(TenR &image,
                        NumN axis_x,
                        NumN axis_y,
                        const VecN &color_bg,
                        NumB isSetBgColor) {
        MATH21_ASSERT(image.isEmpty() ||
                      (image.dim(1) == 3 && image.dim(2) == axis_y && image.dim(3) == axis_x),
                      "image.isEmpty() " << image.isEmpty()
                                         << "\n\taxis_x " << axis_x
                                         << "\n\taxis_y " << axis_y
                                         << "\n\timage.size() " << image.shape());
        if (image.isEmpty()) {
            NumN image_height, image_width;
            image_width = axis_x;
            image_height = axis_y;
            image.setSize(3, image_height, image_width);
        }

        if (isSetBgColor) {
            img_set_color(image, color_bg);
        }
    }

    // Todo: use math21_la_2d_matrix_compute_matrix_axis_to_matrix instead.
    //write matrix-represented function to image without changing old contents of image.
    void img_write_function(const TenN &matrix_axis, TenR &image, const VecN &color, const VecN &color_bg,
                            NumB isSetBgColor) {
        if (matrix_axis.isEmpty()) {
            return;
        }
        MATH21_ASSERT(matrix_axis.dims() == 2, "must be 2-D tensor in current version!");
        img_init_image(image, matrix_axis.dim(1), matrix_axis.dim(2), color_bg, isSetBgColor);
        MATH21_ASSERT(color.size() == 3, "must be rgb color in [0, 255]!");

        NumN tmp;
        NumN i1, i2, j2;
//#pragma omp parallel for private(i2, j2, tmp)
        for (i1 = 1; i1 <= matrix_axis.dim(1); i1++) {
            for (i2 = 1, j2 = matrix_axis.dim(2); i2 <= matrix_axis.dim(2); i2++, j2--) {
                tmp = matrix_axis(i1, i2);
                if (tmp == 1) {
                    image(1, j2, i1) = color(1);
                    image(2, j2, i1) = color(2);
                    image(3, j2, i1) = color(3);
                }
            }
        }
    }

    void img_write_test_sine() {
        TenR X_sample, Y_sample;
        NumN seconds;

        TenN matrix_axis;
        NumN axis_x, axis_y;
        axis_x = 500;
        axis_y = 300;

        TenR image;
        VecN color(3);
        VecN color_bg;
        color_bg.setSize(3);
        color_bg = 0, 0, 0;

        for (NumN i = 0; i < 7; i++) {
            seconds = i * 2;
            sample_sine_voice(X_sample, Y_sample, seconds);
            X_sample.logInfo();

            matrix_axis.zeros();
            draw_data_at_matrix_axis(X_sample, Y_sample, matrix_axis, axis_x, axis_y);

            color = (i * 155) % 256, (i * 66) % 256, (i * 77) % 256;
            img_write_function(matrix_axis, image, color, color_bg, 0);
        }
        img_write(image, "D:/z3.jpg");
    }

    void img_write_test() {
        TenR X_sample, Y_sample;
        NumN seconds;

        TenN matrix_axis;
        NumN axis_x, axis_y;
        axis_x = 500;
        axis_y = 300;

        TenR image;
        VecN color(3);
        VecN color_bg;
        color_bg.setSize(3);
        color_bg = 0, 0, 0;

        for (NumN i = 0; i < 3; i++) {
            seconds = i * 2;
            sample_sine_voice(X_sample, Y_sample, seconds);
            X_sample.logInfo();

            matrix_axis.zeros();
            draw_data_at_matrix_axis(X_sample, Y_sample, matrix_axis, axis_x, axis_y);

            color = (i * 155) % 256, (i * 66) % 256, (i * 77) % 256;
            img_write_function(matrix_axis, image, color, color_bg, 0);
        }
        img_write(image, "z3.jpg");
    }
}
