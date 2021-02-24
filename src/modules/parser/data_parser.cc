#include "data_parser.h"
#include "image/files.h"
#include "csv_like_parser/files.h"
#include <fstream>


namespace star {

    ////////////// text

    void ml_data_save_as_vector_using_index(std::ostream &myfile, const Seqce<TenR> &data, const VecN &indexes,
                                            char character_row, char character_col) {
        NumN i;
        for (i = 1; i < indexes.size(); ++i) {
            ml_data_save_as_vector(myfile, data(indexes(i)), character_col);
            myfile << character_row;
        }
        if (i == indexes.size()) {
            ml_data_save_as_vector(myfile, data(indexes(i)), character_col);
        }
    }

    // save tensor as vector
    void ml_data_save_as_vector_using_index_multiple(const Seqce<TenR> &data, const Seqce<VecN> &labels,
                                                     const char *data_config_path, const char *dir,
                                                     char character_row, char character_col) {
        std::string s;
        NumN i;

        if (!xjDirExists(dir)) {
            xjmkdir(dir);
        }
        std::ofstream myfile;
        myfile.open(data_config_path);

        std::stringstream ss;
        char character = 10;
        for (i = 1; i <= labels.size(); i++) {
            ss.str("");
            ss << dir;
            ss << "/";
            ss << i;
            ss << ".txt";
            std::ofstream myfile2;
            myfile2.open(ss.str().c_str());
            ml_data_save_as_vector_using_index(myfile2, data, labels(i), character_row, character_col);
            myfile2.close();
            myfile << ss.str().c_str();
            if (i < labels.size()) {
                myfile << character;
            }
        }
        myfile.close();
    }

    // save tensor as vector
    void ml_data_save_as_vector(std::ostream &myfile, const Seqce<TenR> &A,
                                char character_row, char character_col) {
        std::string s;
        NumN i;
        for (i = 1; i < A.size(); ++i) {
            ml_data_save_as_vector(myfile, A(i), character_col);
            myfile << character_row;
        }
        if (i == A.size()) {
            ml_data_save_as_vector(myfile, A(i), character_col);
        }
    }

    // save tensor as vector
    void ml_data_save_as_vector(std::ostream &myfile, const TenR &A, char character) {
        std::string s;
        NumN i;
        for (i = 1; i < A.size(); ++i) {
            myfile << A(i) << character;
        }
        if (i == A.size()) {
            myfile << A(i);
        }
    }

    void ml_data_save_as_vector(const TenR &A, const char *data_dst, char output_character) {
        std::ofstream myfile;
        myfile.open(data_dst);
        ml_data_save_as_vector(myfile, A, output_character);
        myfile.close();
    }

    void ml_data_save_as_vector(const Seqce<TenR> &A, const char *data_dst,
                                char character_row, char character_col) {
        std::ofstream myfile;
        myfile.open(data_dst);
        ml_data_save_as_vector(myfile, A, character_row, character_col);
        myfile.close();
    }

    void ml_data_parse_as_vector(std::istream &input, TenR &A, char character) {
        std::vector<std::vector<std::string> > csvData;
        data_parse_csv_like(input, csvData, 10, character);
        data_csv_like_to_mat_n_1(csvData, A);
        A.toVector();
    }

    void ml_data_parse_as_vector(TenR &A, const char *data_path, char character) {
        std::ifstream io(data_path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", data_path);
            return;
        }
        printf("break character is \'%c\' with ascii %d\n", character, character);
        ml_data_parse_as_vector(io, A, character);
    }

    void ml_data_parse_as_vector(std::istream &input, Seqce<TenR> &A,
                                 char character_row, char character_col) {
        std::vector<std::vector<std::string> > csvData;
        data_parse_csv_like(input, csvData, character_row, character_col);
        data_csv_like_to_vector(csvData, A);
    }

    void ml_data_parse_as_vector(Seqce<TenR> &A, const char *data_path,
                                 char character_row, char character_col) {
        std::ifstream input(data_path, std::ifstream::in);
        if (!input.is_open()) {
            printf("open %s fail!\n", data_path);
            return;
        }
        printf("row break character is \'%c\' with ascii %d\n", character_row, character_row);
        printf("col break character is \'%c\' with ascii %d\n", character_col, character_col);
        ml_data_parse_as_vector(input, A, character_row, character_col);
    }

    /////////////// images
    void ml_data_write_to_img(const Seqce<TenR> &data, const char *data_config_path, const char *dir,
                              const char *img_suffix) {
        if (!xjDirExists(dir)) {
            xjmkdir(dir);
        }
        std::ofstream myfile;
        myfile.open(data_config_path);

        NumN i;
        std::stringstream ss;
        char character = 10;
        for (i = 1; i <= data.size(); i++) {
            ss.str("");
            ss << dir;
            ss << "/";
            ss << i;
            ss << img_suffix;
            img_write(data(i), ss.str().c_str());
            myfile << ss.str().c_str();
            if (i < data.size()) {
                myfile << character;
            }
        }
        myfile.close();
    }

    void data_vector_write_to_png(const Seqce<VecR> &data, const char *dir, NumN r, NumN c) {
        if (!xjDirExists(dir)) {
            xjmkdir(dir);
        }

        MatR dst_mat, dst_mat2;
        NumN i;
        std::stringstream ss;
        VecN d(2);
        d = r, c;
        for (i = 1; i <= data.size(); i++) {
            ss.str("");
            ss << dir;
            ss << "/";
            ss << i;
            ss << ".png";

            math21_operator_share_reshape(data(i), dst_mat, d);
            img_write(dst_mat, ss.str().c_str());
//        resize(dst_mat, dst_mat2, 10, 10);
//        img_png_write(dst_mat2, ss.str().c_str());
        }
    }

    void data_vector_read_from_png(Seqce<VecR> &data, const char *dir, NumN dataset_size, NumN max_size) {
        MATH21_ASSERT(dataset_size > 0, "empty dataset!");
        NumN size;
        if (max_size > 0) {
            size = xjmin(dataset_size, max_size);
        } else {
            size = dataset_size;
        }
        data.setSize(size);
        MatR dst_mat;
        NumN i;
        std::stringstream ss;
        for (i = 1; i <= size; i++) {
            ss.str("");
            ss << dir;
            ss << "/";
            ss << i;
            ss << ".png";
            img_png_read(dst_mat, ss.str().c_str());

            math21_operator_share_reshape_to_vector(dst_mat, data.at(i));
        }
    }

    void data_read_from_png(Seqce<TenR> &data, const char *dir, NumN dataset_size, NumN max_size) {
        MATH21_ASSERT(dataset_size > 0, "empty dataset!");
        NumN size;
        if (max_size > 0) {
            size = xjmin(dataset_size, max_size);
        } else {
            size = dataset_size;
        }
        data.setSize(size);
        NumN i;
        std::stringstream ss;
        for (i = 1; i <= size; i++) {
            ss.str("");
            ss << dir;
            ss << "/";
            ss << i;
            ss << ".png";
            img_read(data.at(i), ss.str().c_str());
        }
    }

    void data_scale(Seqce<TenR> &data, NumR a, NumR b, NumR c, NumR d) {
        MatR C;
        for (NumN i = 1; i <= data.size(); i++) {
            math21_operator_scale(data.at(i), a, b, c, d);
        }
    }

    /*data_config is text, format:
     * data path
     * e.x.:
    data_01_path.png,
    data_02_path.jpg,
    data_03_path.png,
     */
    // data_config_path_prefix can be null, or like abc_, a/b, a/b/c/, and will be added before data path directly.
    void ml_data_read_from_img(Seqce<TenR> &data, const char *data_config_path, NumN max_size,
                               const char *data_config_path_prefix) {

        std::ifstream io(data_config_path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", data_config_path);
            return;
        }
//        std::vector<std::vector<std::string> > output;
        StarCsvVector  output;
//        data_parse_csv_like(io, output, 10, ',');
        star_data_parse_csv_like_no_space(io, output, 10, ',');
        if (output.isEmpty()) {
            printf("format error!\n");
            return;
        }
        NumN dataset_size = output.size();

        NumN size;
        if (max_size > 0) {
            size = xjmin(dataset_size, max_size);
        } else {
            size = dataset_size;
        }
        data.setSize(size);
        NumN i;
        const char *data_path;
        std::string data_path_2;
        for (i = 1; i <= size; i++) {
//            data_path = output[i - 1][0].c_str();
            data_path = output(i)(1).c_str();

            if (data_config_path_prefix) {
                data_path_2 = std::string(data_config_path_prefix) + data_path;
            } else {
                data_path_2 = data_path;
            }
            printf("parsing file %s\n", data_path_2.c_str());
            img_read(data.at(i), data_path_2.c_str());
        }
    }

    void img_resize_test(const Seqce<TenR> &images) {
        if (images.isEmpty()) {
            return;
        }
        Seqce<TenR> tmp;
        tmp.setSize(images.size());
        VecN d;
        if (images(1).dims() == 2) {
            d.setSize(2);
            d = 6, 6;
        } else {
            d.setSize(3);
            d = images(1).dim(1), 6, 6;
        }
        math21_tool_setSize(tmp, d);
        math21_img_resize(images, tmp);
        ml_data_write_to_img(tmp, "train_images.txt", "train_images");
    }

}