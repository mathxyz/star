#pragma once

#include "../inner.h"

namespace star {

    enum {
        // YUV 420
        STAR_IMAGE_YUV_I420 = 1,
        STAR_IMAGE_YUV_YV12,
        STAR_IMAGE_YUV_NV12,
        STAR_IMAGE_YUV_NV21,

        // ARGB
        STAR_IMAGE_ARGB8888,
        
        STAR_IMAGE_MAT,
        STAR_IMAGE_BLANK,
    };

    NumN star_image_get_format_NumN(const std::string &name);

    std::string star_image_get_format_string(NumN format);

    int io_yuv420_read(const char *path, TenR &m, NumN nr, NumN nc, NumN format);

    int io_yuv420_write(const char *path, const TenR &m, NumN format);
}
