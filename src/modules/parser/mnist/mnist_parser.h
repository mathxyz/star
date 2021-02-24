#pragma once

#include <cstdint>
#include <fstream>
#include "inner.h"

namespace star {
    /*
    // Image data can be saved as image, or text.

    // read and write as picture.
    data_read_from_img(train_images, "train_images.txt", train_max_size);
    data_read_from_img(test_images, "test_images.txt", test_max_size);
    data_write_to_png(train_images, "train_images.txt", "img_train");
    data_write_to_png(test_images, "test_images.txt", "img_test");

    // save and parse as text.
    ml_data_save_as_vector("train_images.txt", train_images, 10, 32);
    ml_data_save_as_vector("test_images.txt", test_images, 10, 32);

    ml_data_parse_as_vector("train_images.txt", train_images, 10, 32);
    ml_data_parse_as_vector("test_images.txt", test_images, 10, 32);
    ml_data_mnist_image_1d_to_2d(train_images, 28, 28);
    ml_data_mnist_image_1d_to_2d(test_images, 28, 28);


    // Labels can be saved as text.
    ml_data_save_as_vector("train_labels.txt", train_labels, 10, 32);
    ml_data_save_as_vector("test_labels.txt", test_labels, 10, 32);
    ml_data_parse_as_vector("train_labels.txt", train_labels, 10, 32);
    ml_data_parse_as_vector("test_labels.txt", test_labels, 10, 32);

     * */

    // label tensor is (10, 1, 1) actually.
    void ml_data_mnist_parse_labels(const std::string &label_file,
                                    Seqce<TenR> &labels2, NumN max_size);

    // image tensor is (28, 28) actually.
    //if scale_min >= scale_max , scale; else not scale
    void ml_data_mnist_parse_images(const std::string &image_file,
                                    Seqce<TenR> &images2,
                                    NumN max_size,
                                    int x_padding,
                                    int y_padding);

    void ml_data_mnist_image_1d_to_2d(Seqce<TenR> &images, NumN nr, NumN nc);

    void ml_data_mnist_image_2d_to_3d(Seqce<TenR> &images);

    void ml_data_mnist_label_0d_to_3d(Seqce<TenR> &labels);

    void ml_data_mnist_idx3_to_images(const char *src_path, NumN max_size, const char *data_config_path,
                                      const char *dst_dir);

    void ml_data_mnist_idx1_to_text(const char *src_path, NumN max_size, const char *dst_path);

    void ml_data_mnist_convert_test();

}  // namespace star
