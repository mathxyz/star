#include "yuv2rgb.h"
#include "io_yuv.h"

namespace star {

    NumN star_image_get_format_NumN(const std::string &name) {
        NumN format;
        if (name == "i420") {
            format = STAR_IMAGE_YUV_I420;
        } else if (name == "yv12") {
            format = STAR_IMAGE_YUV_YV12;
        } else if (name == "nv12") {
            format = STAR_IMAGE_YUV_NV12;
        } else if (name == "nv21") {
            format = STAR_IMAGE_YUV_NV21;
        } else if (name == "argb8888") {
            format = STAR_IMAGE_ARGB8888;
        } else if (name == "mat") {
            format = STAR_IMAGE_MAT;
        } else if (name == "blank") {
            format = STAR_IMAGE_BLANK;
        } else {
            format = 0;
        }
        return format;
    }

    std::string star_image_get_format_string(NumN format) {
        std::string name;
        if (format == STAR_IMAGE_YUV_I420) {
            name = "i420";
        } else if (format == STAR_IMAGE_YUV_YV12) {
            name = "yv12";
        } else if (format == STAR_IMAGE_YUV_NV12) {
            name = "nv12";
        } else if (format == STAR_IMAGE_YUV_NV21) {
            name = "nv21";
        } else if (format == STAR_IMAGE_ARGB8888) {
            name = "argb8888";
        } else if (format == STAR_IMAGE_MAT) {
            name = "mat";
        } else if (format == STAR_IMAGE_BLANK) {
            name = "blank";
        } else {
            name = "unknown";
        }
        return name;
    }

    namespace rgb2yuv {
#define RGB2Y(r, g, b) (uint8_t)(((66 * (r) + 129 * (g) +  25 * (b) + 128) >> 8) +  16)
#define RGB2U(r, g, b) (uint8_t)(((-38 * (r) - 74 * (g) + 112 * (b) + 128) >> 8) + 128)
#define RGB2V(r, g, b) (uint8_t)(((112 * (r) - 94 * (g) -  18 * (b) + 128) >> 8) + 128)

        inline void rgb888_to_yuv(uint8_t r, uint8_t g, uint8_t b, uint8_t *y, uint8_t *u, uint8_t *v) {
            *y = RGB2Y((int) r, (int) g, (int) b);
            *u = RGB2U((int) r, (int) g, (int) b);
            *v = RGB2V((int) r, (int) g, (int) b);
        }

    }

/*
I420: YYYYYYYY UUVV => YUV420P
YV12: YYYYYYYY VVUU => YUV420P
NV12: YYYYYYYY UVUV => YUV420SP
NV21: YYYYYYYY VUVU => YUV420SP
 * */

    // read to rgba
    int io_yuv420_read(const char *path, TenR &m, NumN nr, NumN nc, NumN format) {
        uint8_t *pY;
        uint8_t *pUV;
        uint8_t *pU;
        uint8_t *pV;
        uint32_t *output;
        int width;
        int height;

        width = (int) nc;
        height = (int) nr;
        size_t size = (size_t) width * height * 3 / 2;
        uint8_t *data = (uint8_t *) calloc(size, 1);
        output = (uint32_t *) calloc(width * height * sizeof(uint32_t), 1);

        math21_io_read_file(path, data, size);

        pY = data;
        pUV = data + width * height;

        if (format == STAR_IMAGE_YUV_I420) {
            pU = pUV;
            pV = pUV + width * height / 4;
            convert_yuv420_to_argb8888(pY, pU, pV, output, width, height, width, width / 2, 1);
        } else if (format == STAR_IMAGE_YUV_YV12) {
            pV = pUV;
            pU = pUV + width * height / 4;
            convert_yuv420_to_argb8888(pY, pU, pV, output, width, height, width, width / 2, 1);
        } else if (format == STAR_IMAGE_YUV_NV12) {
#ifdef STAR_ALGORITHM_SPEED
            convert_nv12_to_argb8888(pY, pUV, output, width, height);
#else
            pU = pUV;
            pV = pUV + 1;
            convert_yuv420_to_argb8888(pY, pU, pV, output, width, height, width, width, 2);
#endif
        } else if (format == STAR_IMAGE_YUV_NV21) {
#ifdef STAR_ALGORITHM_SPEED
            convert_nv21_to_argb8888(pY, pUV, output, width, height);
#else
            pV = pUV;
            pU = pUV + 1;
            convert_yuv420_to_argb8888(pY, pU, pV, output, width, height, width, width, 2);
#endif
        } else {
            MATH21_ASSERT(0, "format not support " << star_image_get_format_string(format));
        }

        math21_image_argb8888_to_tensor(output, m, nr, nc);

        free(data);
        free(output);
        return 1;
    }

    int io_tensor_to_yuv420(const TenR &m, uint8_t *const yuv420sp, NumN format) {
        MATH21_ASSERT(m.dims() == 3 &&
                      (m.dim(1) == 3 || m.dim(1) == 4))

        NumN nr, nc, nch;
        nr = m.dim(2);
        nc = m.dim(3);

        // for sp format
        uint8_t *yData = yuv420sp;
        uint8_t *uvData = yuv420sp + nr * nc;

        // for p format
        uint8_t *uData;
        uint8_t *vData;

        if (format == STAR_IMAGE_YUV_I420) {
            uData = uvData;
            vData = uvData + nr * nc / 4;
        } else if (format == STAR_IMAGE_YUV_YV12) {
            vData = uvData;
            uData = uvData + nr * nc / 4;
        }

        NumN i1, i2, i3;
        uint8_t r, g, b;
        uint8_t y, u, v;

        if (format == STAR_IMAGE_YUV_I420 || format == STAR_IMAGE_YUV_YV12) {
            for (i2 = 1; i2 <= nr; ++i2) {
                for (i3 = 1; i3 <= nc; ++i3) {
                    r = (uint8_t) math21_operator_number_clip(m(1, i2, i3), 0, 255);
                    g = (uint8_t) math21_operator_number_clip(m(2, i2, i3), 0, 255);
                    b = (uint8_t) math21_operator_number_clip(m(3, i2, i3), 0, 255);

                    rgb2yuv::rgb888_to_yuv(r, g, b, &y, &u, &v);
                    *yData++ = y;
                    if (i2 % 2 == 1 && i3 % 2 == 1) {
                        *uData++ = u;
                        *vData++ = v;
                    }
                }
            }
        } else if (format == STAR_IMAGE_YUV_NV12) {
            for (i2 = 1; i2 <= nr; ++i2) {
                for (i3 = 1; i3 <= nc; ++i3) {
                    r = (uint8_t) math21_operator_number_clip(m(1, i2, i3), 0, 255);
                    g = (uint8_t) math21_operator_number_clip(m(2, i2, i3), 0, 255);
                    b = (uint8_t) math21_operator_number_clip(m(3, i2, i3), 0, 255);

                    rgb2yuv::rgb888_to_yuv(r, g, b, &y, &u, &v);
                    *yData++ = y;
                    if (i2 % 2 == 1 && i3 % 2 == 1) {
                        *uvData++ = u;
                        *uvData++ = v;
                    }
                }
            }
        } else if (format == STAR_IMAGE_YUV_NV21) {
            for (i2 = 1; i2 <= nr; ++i2) {
                for (i3 = 1; i3 <= nc; ++i3) {
                    r = (uint8_t) math21_operator_number_clip(m(1, i2, i3), 0, 255);
                    g = (uint8_t) math21_operator_number_clip(m(2, i2, i3), 0, 255);
                    b = (uint8_t) math21_operator_number_clip(m(3, i2, i3), 0, 255);

                    rgb2yuv::rgb888_to_yuv(r, g, b, &y, &u, &v);
                    *yData++ = y;
                    if (i2 % 2 == 1 && i3 % 2 == 1) {
                        *uvData++ = v;
                        *uvData++ = u;
                    }
                }
            }
        } else {
            MATH21_ASSERT(0, "format error!")
        }

        return 1;
    }

    // write to yuv file
    int io_yuv420_write(const char *path, const TenR &m, NumN format) {
        NumN nr;
        NumN nc;
        MATH21_ASSERT(m.dims() == 3)
        nr = m.dim(2);
        nc = m.dim(3);

        size_t size = nr * nc * 3 / 2;
        auto *data = (uint8_t *) calloc(size, 1);

        io_tensor_to_yuv420(m, data, format);

        math21_io_write_file(path, data, size);

        free(data);
        return 1;
    }
}