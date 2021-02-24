#include <libbmp/bitmap_image.hpp>
#include "io_bmp.h"

namespace star {

    namespace detail {

        void *star_image_bitmap_read(const char *path) {
            bitmap_image *image = new bitmap_image(path);
            return image;
        }

        void *star_image_bitmap_create(NumN nr, NumN nc) {
            bitmap_image *image = new bitmap_image(nc, nr);
            return image;
        }

        void star_image_bitmap_delete(void *m) {
            bitmap_image *image = (bitmap_image *) m;
            delete image;
        }

        void star_image_bitmap_set_pixel(void *m, NumN ir, NumN ic, NumN8 r, NumN8 g, NumN8 b) {
            bitmap_image *image = (bitmap_image *) m;
            image->set_pixel(ic - 1, ir - 1, r, g, b);
        }

        void star_image_bitmap_get_pixel(void *m, NumN ir, NumN ic, NumN8 *r, NumN8 *g, NumN8 *b) {
            bitmap_image *image = (bitmap_image *) m;
            image->get_pixel(ic - 1, ir - 1, *r, *g, *b);
        }

        void star_image_bitmap_get_dims(void *m, NumN *nr, NumN *nc) {
            bitmap_image *image = (bitmap_image *) m;
            *nr = image->height();
            *nc = image->width();
        }

        void star_image_bitmap_save(void *m, const char *path) {
            bitmap_image *image = (bitmap_image *) m;
            image->save_image(path);
        }
    }
}