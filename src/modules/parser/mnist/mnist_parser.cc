/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#include <cstdint>
#include <fstream>
#include "mnist_parser.h"
#include "../files.h"

namespace star {
    namespace detail {

        struct mnist_header {
            uint32_t magic_number;
            uint32_t num_items;
            uint32_t num_rows;
            uint32_t num_cols;
        };

        void parse_mnist_header(std::ifstream &ifs, mnist_header &header) {
            ifs.read(reinterpret_cast<char *>(&header.magic_number), 4);
            ifs.read(reinterpret_cast<char *>(&header.num_items), 4);
            ifs.read(reinterpret_cast<char *>(&header.num_rows), 4);
            ifs.read(reinterpret_cast<char *>(&header.num_cols), 4);

            if (star_is_little_endian()) {
                reverse_endian(&header.magic_number);
                reverse_endian(&header.num_items);
                reverse_endian(&header.num_rows);
                reverse_endian(&header.num_cols);
            }

            if (header.magic_number != 0x00000803 || header.num_items <= 0)
            {
                MATH21_ASSERT(0,"MNIST label-file format error");
            }
            if (ifs.fail() || ifs.bad()) {
                MATH21_ASSERT(0,"file error");
            }
        }

        void parse_mnist_image(std::ifstream &ifs,
                               const mnist_header &header,
                               int x_padding,
                               int y_padding,
                               TenR &A) {
            const int width = header.num_cols + 2 * x_padding;
            const int height = header.num_rows + 2 * y_padding;

            std::vector<uint8_t> image_vec(header.num_rows * header.num_cols);

            ifs.read(reinterpret_cast<char *>(&image_vec[0]),
                     header.num_rows * header.num_cols);

            A.setSize(height, width);
            MATH21_ASSERT(math21_operator_isEqualZero(A));

            for (uint32_t y = 0, i = 1; y < header.num_rows; y++, i++) {
                for (uint32_t x = 0, j = 1; x < header.num_cols; x++, j++) {
                    A(i + y_padding, j + x_padding) =
                            image_vec[y * header.num_cols + x];
                }
            }
        }
    }  // namespace detail

/**
 * parse MNIST database format labels with rescaling/resizing
 * http://yann.lecun.com/exdb/mnist/
 *
 * @param label_file [in]  filename of database (i.e.train-labels-idx1-ubyte)
 * @param labels     [out] parsed label data
 **/
    // label tensor is a number actually.
    void ml_data_mnist_parse_labels(const std::string &label_file,
                                 Seqce<TenR> &labels2, NumN max_size) {
        std::ifstream ifs(label_file.c_str(), std::ios::in | std::ios::binary);

        if (ifs.bad() || ifs.fail())
        {
            MATH21_ASSERT(0,"failed to open file:" + label_file);
        }

        uint32_t magic_number, num_items;

        ifs.read(reinterpret_cast<char *>(&magic_number), 4);
        ifs.read(reinterpret_cast<char *>(&num_items), 4);

        if (star_is_little_endian()) {  // MNIST data is big-endian format
            reverse_endian(&magic_number);
            reverse_endian(&num_items);
        }

        if (magic_number != 0x00000801 || num_items <= 0)
        {
            MATH21_ASSERT(0,"MNIST label-file format error");
        }

        NumN size;
        if (max_size > 0) {
            size = xjmin((NumN) num_items, max_size);
        } else {
            size = num_items;
        }

        labels2.setSize(size);
        for (uint32_t i = 0, j = 1; i < size; i++, j++) {
            uint8_t label;
            ifs.read(reinterpret_cast<char *>(&label), 1);
            TenR &A = labels2.at(j);
            A.setSize(1);
            A(1) = label;
        }
//        labels2.log("label2");
    }


/**
 * parse MNIST database format images with rescaling/resizing
 * http://yann.lecun.com/exdb/mnist/
 * - if original image size is WxH, output size is
 *(W+2*x_padding)x(H+2*y_padding)
 * - extra padding pixels are filled with scale_min
 *
 * @param image_file [in]  filename of database (i.e.train-images-idx3-ubyte)
 * @param images     [out] parsed image data
 * @param scale_min  [in]  min-value of output
 * @param scale_max  [in]  max-value of output
 * @param x_padding  [in]  adding border width (left,right)
 * @param y_padding  [in]  adding border width (top,bottom)
 *
 * [example]
 * scale_min=-1.0, scale_max=1.0, x_padding=1, y_padding=0
 *
 * [input]       [output]
 *  64  64  64   -1.0 -0.5 -0.5 -0.5 -1.0
 * 128 128 128   -1.0  0.0  0.0  0.0 -1.0
 * 255 255 255   -1.0  1.0  1.0  1.0 -1.0
 *
 **/
    // image tensor is (28, 28) actually.
    //if scale_min >= scale_max , scale; else not scale
    void ml_data_mnist_parse_images(const std::string &image_file,
                                 Seqce<TenR> &images2,
                                 NumN max_size,
                                 int x_padding,
                                 int y_padding) {
        if (x_padding < 0 || y_padding < 0)
        {
            MATH21_ASSERT(0,"padding size must not be negative");
        }

        std::ifstream ifs(image_file.c_str(), std::ios::in | std::ios::binary);

        if (ifs.bad() || ifs.fail())
        {
            MATH21_ASSERT(0,"failed to open file:" + image_file);
        }

        detail::mnist_header header;

        detail::parse_mnist_header(ifs, header);

        NumN size;
        if (max_size > 0) {
            size = xjmin((NumN) header.num_items, max_size);
        } else {
            size = header.num_items;
        }

        images2.setSize(size);
        for (uint32_t i = 0, j = 1; i < size; i++, j++) {
            detail::parse_mnist_image(ifs, header, x_padding,
                                      y_padding, images2.at(j));
        }
    }

    void ml_data_mnist_image_1d_to_2d(Seqce<TenR> &images, NumN nr, NumN nc) {
        if (images.isEmpty()) {
            return;
        }
        MATH21_ASSERT(images.at(1).dims() == 1 && images.at(1).dim(1) == nr * nc)
        VecN d(2);
        d = nr, nc;
        NumN i;
        NumN n = images.size();
        for (i = 1; i <= n; ++i) {
            images.at(i).reshape(d);
        }
    }

    void ml_data_mnist_image_2d_to_3d(Seqce<TenR> &images) {
        if (images.isEmpty()) {
            return;
        }
        MATH21_ASSERT(images.at(1).dims() == 2)
        VecN d(3);
        d = 1, images.at(1).dim(1), images.at(1).dim(2);
        NumN i;
        NumN n = images.size();
        for (i = 1; i <= n; ++i) {
            images.at(i).reshape(d);
        }
    }

    // tensor is a number actually, so it is converted to tensor with shape (10, 1, 1).
    // e.x., 2-> 0, 0, 1, 0, 0, 0, 0, 0, 0, 0
    void ml_data_mnist_label_0d_to_3d(Seqce<TenR> &labels) {
        if (labels.isEmpty()) {
            return;
        }
        MATH21_ASSERT(labels.at(1).dims() == 1 && labels.at(1).dim(1) == 1)
        VecN d(3);
        d = 1, labels.at(1).dim(1), labels.at(1).dim(2);
        NumN i;
        NumN n = labels.size();
        NumN number;
        for (i = 1; i <= n; ++i) {
            TenR &A = labels.at(i);
            number = (NumN) A(1);

            A.setSize(10, 1, 1);
            A = 0;
            A(number + 1, 1, 1) = 1;
        }
    }

    void ml_data_mnist_idx3_to_images(const char* src_path, NumN max_size, const char* data_config_path, const char* dst_dir) {
        Seqce<TenR> train_images;
        // parse from mnist format files
        ml_data_mnist_parse_images(src_path, train_images, max_size, 0, 0);
        // save data
        ml_data_write_to_img(train_images, data_config_path, dst_dir);
    }

    void ml_data_mnist_idx1_to_text(const char* src_path, NumN max_size, const char* dst_path) {
        Seqce<TenR> train_labels;
        // parse from mnist format files
        ml_data_mnist_parse_labels(src_path, train_labels, max_size);
        // save data
        ml_data_save_as_vector(train_labels, dst_path, 10, 32);
    }

    void ml_data_mnist_convert_test() {
        m21log(__FUNCTION__);
        std::string home_path = STAR_HOME_PATH;
        std::string data_dir = home_path + "/assets/data/mnist";
        std::string train_labels_path = data_dir + "/train-labels.idx1-ubyte";
        std::string train_images_path = data_dir + "/train-images.idx3-ubyte";
        std::string test_labels_path = data_dir + "/t10k-labels.idx1-ubyte";
        std::string test_images_path = data_dir + "/t10k-images.idx3-ubyte";

//    NumN train_max_size = 0;
        NumN train_max_size = 20;
//    NumN test_max_size = 0;
        NumN test_max_size = 20;

        ml_data_mnist_idx3_to_images(train_images_path.c_str(), train_max_size, "train_images.txt", "train_images");
        ml_data_mnist_idx1_to_text(train_labels_path.c_str(), train_max_size, "train_labels.txt");
        ml_data_mnist_idx3_to_images(test_images_path.c_str(), test_max_size, "test_images.txt", "test_images");
        ml_data_mnist_idx1_to_text(test_labels_path.c_str(), test_max_size, "test_labels.txt");
    }
}  // namespace star
