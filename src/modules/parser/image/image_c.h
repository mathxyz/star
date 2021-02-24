#pragma once

#include "inner_c.h"

#ifdef __cplusplus
extern "C" {
#endif

m21image star_image_read_image(const char *path);

m21image star_image_read_image_normalized(const char *path);

m21image star_image_read_image_normalized_with_rc(const char *path, NumN nr, NumN nc);

void star_image_write_image(m21image image, const char *path);

void star_image_write_image_normalized(m21image image0, const char *path);

#ifdef __cplusplus
}
#endif

