#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include "libjpeg/jpeglib.h"
#include "io_jpeg.h"

/**
 * @brief read a jpg file into a 8bit unsigned char array
 *
 * The array contains the interlaced channels.
 *
 * @param fname jpg file name
 * @param nxp, nyp, ncp pointers to variables to be filled with the number of
 *        columns, lines and channels of the image
 * @return pointer to an allocated unsigned char array of pixels,
 *         or NULL if an error happens
 */
unsigned char *io_jpg_read_u8(const char *input_filename,
                              size_t *nxp, size_t *nyp, size_t *ncp) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *input_file;
    JSAMPARRAY buffer;
    int row_width;

    unsigned char *output_buffer;
    unsigned char *tmp = NULL;

    cinfo.err = jpeg_std_error(&jerr);

    if ((input_file = fopen(input_filename, "rb")) == NULL) {
        fprintf(stderr, "can't open %s\n", input_filename);
        return 0;
    }

    jpeg_create_decompress(&cinfo);

    /* Specify data source for decompression */
    jpeg_stdio_src(&cinfo, input_file);

    /* Read file header, set default decompression parameters */
    (void) jpeg_read_header(&cinfo, TRUE);

    /* Start decompressor */
    (void) jpeg_start_decompress(&cinfo);

    row_width = cinfo.output_width * cinfo.output_components;

    buffer = (*cinfo.mem->alloc_sarray)
            ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_width, 1);

    *ncp = (size_t) cinfo.output_components;
    *nxp = (size_t) cinfo.output_width;
    *nyp = (size_t) cinfo.output_height;


    output_buffer = (unsigned char *) malloc(row_width * cinfo.output_height);
    memset(output_buffer, 0, row_width * cinfo.output_height);
    tmp = output_buffer;

    /* Process data */
    while (cinfo.output_scanline < cinfo.output_height) {
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);

        memcpy(tmp, *buffer, row_width);
        tmp += row_width;
    }
    (void) jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    /* Close files, if we opened them */
    fclose(input_file);

    return output_buffer;
}

// * data is interlaced (RGB, RGB, ...), and m(i1, i2, i3) is in [0, 255].
int io_jpg_write_u8(const char *filename, unsigned char *data,
                    size_t width, size_t height, size_t channels, int quality) {
    if (data == 0) {
        printf("no data");
        return 0;
    }
    struct jpeg_compress_struct cinfo;
    int i = 0;
    struct jpeg_error_mgr jerr;
    /* More stuff */
    FILE *outfile;  /* target file */
    JSAMPROW row_pointer[1]; /* pointer to JSAMPLE row[s] */
    int row_stride;  /* physical row width in image buffer */

    /* Step 1: allocate and initialize JPEG compression object */

    /* We have to set up the error handler first, in case the initialization
     * step fails.  (Unlikely, but it could happen if you are out of memory.)
     * This routine fills in the contents of struct jerr, and returns jerr's
     * address which we place into the link field in cinfo.
     */
    cinfo.err = jpeg_std_error(&jerr);
    /* Now we can initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);

    /* Step 2: specify data destination (eg, a file) */
    /* Note: steps 2 and 3 can be done in either order. */

    /* Here we use the library-supplied code to send compressed data to a
     * stdio stream. You can also write your own code to do something else.
     * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
     * requires it in order to write binary files.
     */
    if ((outfile = fopen(filename, "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", filename);
        return 0;
    }
    jpeg_stdio_dest(&cinfo, outfile);

    /* Step 3: set parameters for compression */

    /* First we supply a description of the input image.
     * Four fields of the cinfo struct must be filled in:
     */
    cinfo.image_width = width; /* image width and height, in pixels */
    cinfo.image_height = height;
    cinfo.input_components = channels;  /* # of color components per pixel */
    if (cinfo.input_components == 1) {
        cinfo.in_color_space = JCS_GRAYSCALE;
    } else {
        cinfo.in_color_space = JCS_RGB;
    }
    /* colorspace of input image */
    /* Now use the library's routine to set default compression parameters.
     * (You must set at least cinfo.in_color_space before calling this,
     * since the defaults depend on the source color space.)
     */
    jpeg_set_defaults(&cinfo);
    /* Now you can set any non-default parameters you wish to.
     * Here we just illustrate the use of quality (quantization table) scaling:
     */
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

    /* Step 4: Start compressor */

    /* TRUE ensures that we will write a complete interchange-JPEG file.
     * Pass TRUE unless you are very sure of what you're doing.
     */
    jpeg_start_compress(&cinfo, TRUE);

    /* Step 5: while (scan lines remain to be written) */
    /*     jpeg_write_scanlines(...); */

    /* Here we use the library's state variable cinfo.next_scanline as the
     * loop counter, so that we don't have to keep track ourselves.
     * To keep things simple, we pass one scanline per call; you can pass
     * more if you wish, though.
     */
    row_stride = cinfo.image_width * cinfo.input_components; /* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height) {
        /* jpeg_write_scanlines expects an array of pointers to scanlines.
         * Here the array is only one element long, but you could pass
         * more than one scanline at a time if that's more convenient.
         */
        row_pointer[0] = &data[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    /* Step 6: Finish compression */

    jpeg_finish_compress(&cinfo);
    /* After finish_compress, we can close the output file. */
    fclose(outfile);

    /* Step 7: release JPEG compression object */

    /* This is an important step since it will release a good deal of memory. */
    jpeg_destroy_compress(&cinfo);

    /* And we're done! */
    return 1;
}
