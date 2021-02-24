#pragma once

#include "inner.h"
#include "mat2img.h"

namespace star {

    struct StarGlobalVariables {
        Seqce<std::string> ls_files;

        StarGlobalVariables() {
        }
    };

    StarGlobalVariables &star_getGlobalVariables();

    // set background color to image, color is (r, g, b) format.
    void img_set_color(TenR &image, const VecN &color_bg);

    template<typename T>
    void img_split_write(const Tensor<T> &m, const char *path);

    int star_img_read_and_write(int argc, const char **argv);

    int star_ls(int argc, const char **argv);

    int star_ls_clear(int argc, const char **argv);

    int sky_exit(int argc, const char **argv);

    int sky_loop_forever(int argc, const char **argv);

}

#include "image_impl"