#pragma once

#include "inner.h"

namespace star {

    void img_write_test();

    void img_init_image(TenR &image,
                        NumN axis_x,
                        NumN axis_y,
                        const VecN &color_bg,
                        NumB isSetBgColor);

    void img_write_function(const TenN &matrix_axis, TenR &image, const VecN &color, const VecN &color_bg,
                            NumB isSetBgColor);

    // deprecate
    void img_png_write(const MatR &m, const char *path);

    void img_png_write(const MatN &m, const char *path);

    void img_png_read(MatR &m, const char *path);

    void img_png_read(MatN &m, const char *path);
}

#include "mat2img_impl"