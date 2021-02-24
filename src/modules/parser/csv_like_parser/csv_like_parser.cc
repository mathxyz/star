#include "../png_parser/io_png.h"
#include "../image/files.h"
#include <fstream>
#include "csv_like_parser.h"

namespace star {
    typedef std::vector<std::vector<std::string> > csvVector;

    void star_log_csv(std::ostream &out, const char *name, const MatStr &m, char character_row,
                      char character_col) {
        if (name) {
            printf("%s\n", name);
        }
        for (NumN i = 1; i <= m.nrows(); ++i) {
            for (NumN j = 1; j <= m.ncols(); ++j) {
                out << m(i, j);
                if (j != m.ncols()) {
                    out << character_col;
                }
            }
            if (i != m.nrows()) {
                out << character_row;
            }
        }
    }

    void star_log_csv(const char *name, const MatStr &csvData, char character_row,
                      char character_col) {
        star_log_csv(std::cout, name, csvData, character_row, character_col);
    }

    // deprecated, use star_data_parse_csv_to_StarCsvVector instead.
    NumB data_parse_csv_like(std::istream &input, csvVector &output, char character_row,
                             char character_col) {
        std::string csvLine;
        while (getline(input, csvLine, character_row)) {
            std::istringstream csvStream(csvLine);
            std::vector<std::string> csvColumn;
            std::string csvElement;
            while (getline(csvStream, csvElement, character_col)) {
                csvColumn.push_back(csvElement);
            }
            output.push_back(csvColumn);
        }
        return 1;
    }

    /*
    * 10	0A	LF	Line feed
    * 32	20		Space
    * 33	21	!
    * 44    2C  ,
    * 48	30	0
    * 65	41	A
    *
    * A is n*1 matrix
    * */
    // character to break on: ',' or ' '
    NumB star_data_parse_csv_to_StarCsvVector(std::istream &input, StarCsvVector &output, char character_row,
                             char character_col) {
        std::string csvLine;
        while (getline(input, csvLine, character_row)) {
            std::istringstream csvStream(csvLine);
            StarCsvColumn csvColumn;
            std::string csvElement;
            while (getline(csvStream, csvElement, character_col)) {
                csvColumn.push(csvElement);
            }
            output.push(csvColumn);
        }
        return 1;
    }

    void CsvVector_to_StarCsvVector(const csvVector &A, StarCsvVector &B) {
        NumN nr = A.size();
        B.setSize(nr);
        for (NumN i = 1; i <= nr; ++i) {
            NumN nc = A[i - 1].size();
            B(i).setSize(nc);
            for (NumN j = 1; j <= nc; ++j) {
                B(i)(j) = A[i - 1][j - 1];
            }
        }
    }

    void math21_log_info_StarCsvVector(const StarCsvVector &A) {
        A.logInfo("CsvVector");
        for (NumN i = 1; i <= A.size(); ++i) {
            A(i).logInfo();
        }
    }

    void math21_log_StarCsvVector(const StarCsvVector &A) {
        A.logInfo("CsvVector");
        for (NumN i = 1; i <= A.size(); ++i) {
            A(i).log(0, 1);
        }
    }

    // trim
    void star_csv_vector_remove_space(const StarCsvVector &A, StarCsvVector &B) {

        std::string s;
        NumN row_num = 0;
        for (NumN i = 1; i <= A.size(); ++i) {
            NumN col_idx = 0;
            const StarCsvColumn &csvColumn = A(i);
            for (NumN j = 1; j <= csvColumn.size(); ++j) {
                s = csvColumn(j);
                if (star_string_is_null_or_spaces(s)) {
                    continue;
                }
                ++col_idx;
                break;
            }
            if (col_idx != 0) {
                ++row_num;
            }
        }
        if (row_num == 0) {
            return;
        }
        B.setSize(row_num);
        row_num = 0;
        for (NumN i = 1; i <= A.size(); ++i) {
            NumN col_idx = 0;
            const StarCsvColumn &csvColumn = A(i);
            for (NumN j = 1; j <= csvColumn.size(); ++j) {
                s = csvColumn(j);
                if (star_string_is_null_or_spaces(s)) {
                    continue;
                }
                ++col_idx;
            }
            if (col_idx != 0) {
                ++row_num;
                B(row_num).setSize(col_idx);
                col_idx = 0;
                for (NumN j = 1; j <= csvColumn.size(); ++j) {
                    s = csvColumn(j);
                    if (star_string_is_null_or_spaces(s)) {
                        continue;
                    }
                    ++col_idx;
                    B.at(row_num).at(col_idx) = star_string_trim(s);
                }
            }
        }
    }

    /*
    * 10	0A	LF	Line feed
    * 32	20		Space
    * 33	21	!
    * 44    2C  ,
    * 48	30	0
    * 65	41	A
    *
    * A is n*1 matrix
    * */
    // character to break on: ',' or ' '
    NumB star_data_parse_csv_like(std::istream &input, StarCsvVector &output, char character_row,
                                  char character_col) {
        NumB flag;
        csvVector output1;
        flag = data_parse_csv_like(input, output1, character_row, character_col);
        CsvVector_to_StarCsvVector(output1, output);
        return flag;
    }

    NumB star_data_parse_csv_like_no_space(std::istream &input, StarCsvVector &output,
                                           char character_row,
                                           char character_col) {
        StarCsvVector output0;
        star_data_parse_csv_like(input, output0, character_row, character_col);
        star_csv_vector_remove_space(output0, output);
        return 1;
    }

    NumB star_data_parse_csv_like_to_MatStr(std::istream &input, MatStr &output,
                                            char character_row,
                                            char character_col) {
        StarCsvVector csvVector;
        star_data_parse_csv_like_no_space(input, csvVector, character_row, character_col);
        star_data_csv_like_to_MatStr(csvVector, output);
        return 1;
    }

    int sky_data_edit_csv_like(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                std::cout << "\tsky_data_edit_csv_like \n";
                return 1;
            }
        }
        const char *src_path = star_string_args_find_c_string(argc, argv, "-src_path", 0);
        const char *dst_path = star_string_args_find_c_string(argc, argv, "-dst_path", 0);
        auto src_character_row = (char) star_string_args_find_NumN(argc, argv, "-src_character_row", 10);
        auto src_character_col = (char) star_string_args_find_NumN(argc, argv, "-src_character_col", ',');
        auto dst_character_row = (char) star_string_args_find_NumN(argc, argv, "-dst_character_row", 10);
        auto dst_character_col = (char) star_string_args_find_NumN(argc, argv, "-dst_character_col", ',');
        auto rows = star_string_args_find_string(argc, argv, "-rows", "0");
        auto cols = star_string_args_find_string(argc, argv, "-cols", "0");
        NumN logLevel = star_string_args_find_NumN(argc, argv, "-logLevel", 0);
        std::ifstream io_in;
        io_in.open(src_path, std::ifstream::in);
        if (!io_in.is_open()) {
            printf("open %s fail!\n", src_path);
            return 1;
        }
        std::ofstream io_out;
        if (dst_path) {
            io_out.open(dst_path, std::ofstream::out);
            if (!io_out.is_open()) {
                printf("open %s fail!\n", dst_path);
                return 0;
            }
        }
        MatStr matStr;
        star_data_parse_csv_like_to_MatStr(io_in, matStr, src_character_row, src_character_col);

        std::stringstream ss;
        MatStr matRows;
        ss.str("");
        ss << rows;
        star_data_parse_csv_like_to_MatStr(ss, matRows);
        MatStr matCols;
        ss.clear(); // clear is not clearing contents.
        ss.str("");
        ss << cols;
        star_data_parse_csv_like_to_MatStr(ss, matCols);

        // process
        VecN row_indexes;
        row_indexes = matRows.toVector();
        VecN col_indexes;
        col_indexes = matCols.toVector();
        MatStr matStr_sub;
        math21_operator_matrix_submatrix(matStr, matStr_sub, row_indexes, col_indexes);
        if (logLevel > 200) {
            row_indexes.log("row_indexes");
            col_indexes.log("col_indexes");
        }

        // save
        if (io_out.is_open()) {
            star_log_csv(io_out, 0, matStr_sub, dst_character_row, dst_character_col);
        }

        if (logLevel > 10) {
            star_log_csv(0, matStr_sub, dst_character_row, dst_character_col);
        }

        io_in.close();
        io_out.close();
        return 1;
    }

    void data_save_csv_like(std::ofstream &myfile, csvVector &csvData, char character) {
        std::string s;
        for (csvVector::iterator i = csvData.begin(); i != csvData.end(); ++i) {
            for (std::vector<std::string>::iterator j = i->begin(); j != i->end(); ++j) {
                s = *j;
                if (star_string_is_null_or_spaces(s)) {
                    continue;
                }
                myfile << s << character;
            }
            myfile << "\n";
        }
    }

    void ml_data_parse_and_save_csv_like(const char *data_src, const char *data_dst,
                                         char input_character,
                                         char output_character) {
        std::fstream file(data_src, std::ios::in);
        std::ofstream myfile;
        myfile.open(data_dst);

        csvVector csvData;
        data_parse_csv_like(file, csvData, 10, input_character);
        data_save_csv_like(myfile, csvData, output_character);
        myfile.close();
    }

    /*
     * A is vec, [start, end], start: [1, ...]
     * */
    NumB data_string_to_num(std::vector<std::string> &strings, VecR &A, NumN start, NumN end) {
        if (start == 0) {
            start = 1;
        }
        if (end == 0) {
            end = strings.size();
        }
        std::string s;
        if (start > end) {
            return 0;
        }
        if (end > strings.size()) {
            return 0;
        }
        NumN num = end + 1 - start;
        if (A.size() == num) {
        } else {
            A.setSize(num);
        }
        for (NumN i = start, j = 1; i <= end; ++i, ++j) {
            if (strings[i - 1].empty()) {
                continue;
            }
            A(j) = math21_string_to_NumR(strings[i - 1]);
        }
        return 1;
    }

    /*
     * A is n*1 matrix, csvData is not in good format.
     * */
    NumB data_csv_like_to_mat_n_1(csvVector &csvData, TenR &A) {
        std::string s;
        NumN num = 0;
        for (csvVector::iterator i = csvData.begin(); i != csvData.end(); ++i) {
            for (std::vector<std::string>::iterator j = i->begin(); j != i->end(); ++j) {
                s = *j;
                if (star_string_is_null_or_spaces(s)) {
                    continue;
                }
                ++num;
            }
        }

        if (num == 0) {
            return 0;
        }
        if (A.dims() == 2 && A.nrows() == num && A.ncols() == 1) {
        } else {
            A.setSize(num, 1);
        }
        num = 0;
        for (csvVector::iterator i = csvData.begin(); i != csvData.end(); ++i) {
            for (std::vector<std::string>::iterator j = i->begin(); j != i->end(); ++j) {
                s = *j;
                if (star_string_is_null_or_spaces(s)) {
                    continue;
                }
                ++num;
                A(num, 1) = math21_string_to_NumR(s);
            }
        }
        return 1;
    }

    /*
     * A is n*1 matrix if csvData is n*1.
     * A is n*m matrix if csvData is n*m, or csvData has max column m with some data missing.
     * */
    NumB data_csv_like_to_MatR(csvVector &csvData, TenR &A) {
        std::string s;
        NumN row_num = 0;
        NumN col_idx = 0;
        NumN col_num = 0;
        for (csvVector::iterator i = csvData.begin(); i != csvData.end(); ++i) {
            col_idx = 0;
            for (std::vector<std::string>::iterator j = i->begin(); j != i->end(); ++j) {
                s = *j;
                if (star_string_is_null_or_spaces(s)) {
                    continue;
                }
                ++col_idx;
            }
            if (col_num < col_idx) {
                col_num = col_idx;
            }
            if (col_idx != 0) {
                ++row_num;
            }
        }

        if (row_num == 0) {
            return 0;
        }
        if (A.dims() == 2 && A.nrows() == row_num && A.ncols() == col_num) {
        } else {
            A.setSize(row_num, col_num);
        }
        row_num = 0;
        for (csvVector::iterator i = csvData.begin(); i != csvData.end(); ++i) {
            col_num = 0;
            for (std::vector<std::string>::iterator j = i->begin(); j != i->end(); ++j) {
                s = *j;
                if (star_string_is_null_or_spaces(s)) {
                    continue;
                }
                ++col_num;
                A(row_num + 1, col_num) = math21_string_to_NumR(s);
            }
            if (col_num != 0) {
                ++row_num;
            }
        }
        return 1;
    }

    // see math21_operator_container2d_max_cols
    /*
     * A is n*1 matrix if csvData is n*1.
     * A is n*m matrix if csvData is n*m, or csvData has max column m with some data missing.
     * */
    NumB star_data_csv_like_to_MatStr(StarCsvVector &csvData, MatStr &A) {
        std::string s;
        NumN row_num = 0;
        NumN col_idx = 0;
        NumN col_num = 0;
        for (NumN i = 1; i <= csvData.size(); ++i) {
            StarCsvColumn &csvColumn = csvData.at(i);
            col_idx = 0;
            for (NumN j = 1; j <= csvColumn.size(); ++j) {
                s = csvColumn.at(j);
                if (star_string_is_null_or_spaces(s)) {
                    continue;
                }
                ++col_idx;
            }
            if (col_num < col_idx) {
                col_num = col_idx;
            }
            if (col_idx != 0) {
                ++row_num;
            }
        }

        if (row_num == 0) {
            return 0;
        }
        if (A.dims() == 2 && A.nrows() == row_num && A.ncols() == col_num) {
        } else {
            A.setSize(row_num, col_num);
        }
        row_num = 0;
        for (NumN i = 1; i <= csvData.size(); ++i) {
            StarCsvColumn &csvColumn = csvData.at(i);
            col_num = 0;
            for (NumN j = 1; j <= csvColumn.size(); ++j) {
                s = csvColumn.at(j);
                if (star_string_is_null_or_spaces(s)) {
                    continue;
                }
                ++col_num;
                A(row_num + 1, col_num) = s;
            }
            if (col_num != 0) {
                ++row_num;
            }
        }
        return 1;
    }

    /*
     * A is n*1 matrix if csvData is n*1.
     * A is n*m matrix if csvData is n*m, or csvData has max column m with some data missing.
     * */
    NumB star_data_csv_like_to_MatR(StarCsvVector &csvData, TenR &A) {
        std::string s;
        NumN row_num = 0;
        NumN col_idx = 0;
        NumN col_num = 0;
        for (NumN i = 1; i <= csvData.size(); ++i) {
            StarCsvColumn &csvColumn = csvData.at(i);
            col_idx = 0;
            for (NumN j = 1; j <= csvColumn.size(); ++j) {
                s = csvColumn.at(j);
                if (star_string_is_null_or_spaces(s)) {
                    continue;
                }
                ++col_idx;
            }
            if (col_num < col_idx) {
                col_num = col_idx;
            }
            if (col_idx != 0) {
                ++row_num;
            }
        }

        if (row_num == 0) {
            return 0;
        }
        if (A.dims() == 2 && A.nrows() == row_num && A.ncols() == col_num) {
        } else {
            A.setSize(row_num, col_num);
        }
        row_num = 0;
        for (NumN i = 1; i <= csvData.size(); ++i) {
            StarCsvColumn &csvColumn = csvData.at(i);
            col_num = 0;
            for (NumN j = 1; j <= csvColumn.size(); ++j) {
                s = csvColumn.at(j);
                if (star_string_is_null_or_spaces(s)) {
                    continue;
                }
                ++col_num;
                A(row_num + 1, col_num) = math21_string_to_NumR(s);
            }
            if (col_num != 0) {
                ++row_num;
            }
        }
        return 1;
    }

    /*
     * tensor is as vector.
     * Each tensor may not have same size.
     * visit 3 times, bad!
     * Todo: visit less times.
     * */
    NumB data_csv_like_to_vector(csvVector &csvData, Seqce<TenR> &A) {
        NumN nr = 0;
        NumN nc = 0;
        // get nr
        for (csvVector::iterator i = csvData.begin(); i != csvData.end(); ++i) {
            nc = 0;
            for (std::vector<std::string>::iterator j = i->begin(); j != i->end(); ++j) {
                if (star_string_is_null_or_spaces(*j)) {
                    continue;
                }
                ++nc;
            }
            if (nc > 0) {
                ++nr;
            }
        }

        if (nr == 0) {
            return 0;
        }
        if (A.size() != nr) {
            A.setSize(nr);
        }
        nr = 0;
        for (csvVector::iterator i = csvData.begin(); i != csvData.end(); ++i) {
            // get nc
            nc = 0;
            for (std::vector<std::string>::iterator j = i->begin(); j != i->end(); ++j) {
                if (star_string_is_null_or_spaces(*j)) {
                    continue;
                }
                ++nc;
            }
            if (nc > 0) {
                ++nr;
            }
            if (nc == 0) {
                continue;
            }
            A.at(nr).setSize(nc);

            // assign
            nc = 0;
            for (std::vector<std::string>::iterator j = i->begin(); j != i->end(); ++j) {
                if (star_string_is_null_or_spaces(*j)) {
                    continue;
                }
                ++nc;
                A.at(nr)(nc) = math21_string_to_NumR(*j);
            }
        }
        return 1;
    }

    void ml_data_parse_csv_like_n_1(const char *data_path, TenR &A, char character) {
        std::ifstream io(data_path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", data_path);
            return;
        }
        printf("break character is \'%c\' with ascii %d\n", character, character);
        csvVector csvData;
        data_parse_csv_like(io, csvData, 10, character);
        data_csv_like_to_mat_n_1(csvData, A);
    }

    void ml_data_parse_csv_like(const char *data_path, TenR &A, char character) {
        std::ifstream io(data_path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", data_path);
            return;
        }
        printf("break character is \'%c\' with ascii %d\n", character, character);
        csvVector csvData;
        data_parse_csv_like(io, csvData, 10, character);
        data_csv_like_to_MatR(csvData, A);
    }


    void data_get_csv_like_sample_pair(const char *data_path, TenR &X_sample, TenR &Y_sample,
                                       char character) {

        ml_data_parse_csv_like_n_1(data_path, Y_sample, character);
        const NumN n_samples = Y_sample.nrows();
        if (n_samples == 0) {
            return;
        }
        X_sample.setSize(n_samples, 1);
        for (NumN i = 1; i <= n_samples; i++) {
            X_sample(i, 1) = i;
        }
    }

    void data_get_csv_like_data_2d(const char *data_path, TenR &data, char character) {

        ml_data_parse_csv_like(data_path, data, character);
    }
}
