#pragma once

#ifdef __cplusplus
extern "C" {
#endif

unsigned char *io_jpg_read_u8(const char *input_filename,
                              size_t * nxp, size_t * nyp, size_t * ncp);
int io_jpg_write_u8(const char *filename, unsigned char *data, size_t width, size_t height, size_t channels, int quality);

#ifdef __cplusplus
}
#endif

