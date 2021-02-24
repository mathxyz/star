#pragma once

//#ifdef __cplusplus
//extern "C" {
//#endif

#include "../inner.h"

namespace star {

    namespace detail{

        void *star_image_bitmap_read(const char *path);

        void *star_image_bitmap_create(NumN nr, NumN nc);

        void star_image_bitmap_delete(void *m);

        void star_image_bitmap_set_pixel(void *m, NumN ir, NumN ic, NumN8 r, NumN8 g, NumN8 b);

        void star_image_bitmap_get_pixel(void *m, NumN ir, NumN ic, NumN8 *r, NumN8 *g, NumN8 *b);

        void star_image_bitmap_get_dims(void *m, NumN *nr, NumN *nc);

        void star_image_bitmap_save(void *m, const char *path);

    }

    // Risk: not check image channel.
    template<typename T>
    int io_bmp_read(const char *filename, Tensor <T> &B) {
        void *image =  detail::star_image_bitmap_read(filename);
        NumN nr, nc, nch;
         detail::star_image_bitmap_get_dims(image, &nr, &nc);
        nch = 3;
        unsigned char a, r, g, b;
        NumN i1, i2, i3;
        if (B.isSameSize(nch, nr, nc) == 0) {
            B.setSize(nch, nr, nc);
        }

        for (i2 = 1; i2 <= B.dim(2); ++i2) {
            for (i3 = 1; i3 <= B.dim(3); ++i3) {
                 detail::star_image_bitmap_get_pixel(image, i2, i3, &r, &g, &b);
                B(1, i2, i3) = (T) r;
                B(2, i2, i3) = (T) g;
                B(3, i2, i3) = (T) b;
            }
        }
         detail::star_image_bitmap_delete(image);
        return 1;
    }

    template<typename T>
    int io_bmp_write(const char *filename, const Tensor <T> &B) {
        unsigned char a, r, g, b;
        NumN i1, i2, i3;
        if (B.dims() != 3) {
            m21log("not rgb data!");
            return 0;
        }
        if (B.dim(1) == 3 || B.dim(1) == 4) {
            void *image =  detail::star_image_bitmap_create(B.dim(2), B.dim(3));
            for (i2 = 1; i2 <= B.dim(2); ++i2) {
                for (i3 = 1; i3 <= B.dim(3); ++i3) {
                    r = (NumN8) math21_operator_number_clip(B(1, i2, i3), 0, 255);
                    g = (NumN8) math21_operator_number_clip(B(2, i2, i3), 0, 255);
                    b = (NumN8) math21_operator_number_clip(B(3, i2, i3), 0, 255);
                     detail::star_image_bitmap_set_pixel(image, i2, i3, r, g, b);
                }
            }
             detail::star_image_bitmap_save(image, filename);
             detail::star_image_bitmap_delete(image);
        } else {
            m21log("nch not support", B.dim(3));
            return 0;
        }
        return 1;
    }
}
//#ifdef __cplusplus
//}
//#endif

