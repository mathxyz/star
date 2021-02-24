#include "csv_like_parser/files.h"
#include "draw_curve.h"
#include "image/files.h"
#include <fstream>

namespace star {


    void img_draw_curve(const TenR &X_sample, const TenR &Y_sample, TenR &image,
                        const VecN &color, const VecN &color_bg, NumB isSetBgColor,
                        NumN axis_x = 50,
                        NumN axis_y = 30) {

        TenN matrix_axis;
        matrix_axis.zeros();
        draw_data_at_matrix_axis(X_sample, Y_sample, matrix_axis, axis_x, axis_y);
        img_write_function(matrix_axis, image, color, color_bg, isSetBgColor);
    }

    void img_draw_curve_test() {
        TenR X_sample, Y_sample;
        sample_sine_voice(X_sample, Y_sample, 2);
        X_sample.logInfo();

        VecN color;
        color.setSize(3);
        color = 255, 255, 255;
        VecN color_bg;
        color_bg.setSize(3);
        color_bg = 0, 0, 0;
        TenR image;
        img_draw_curve(X_sample, Y_sample, image, color, color_bg, 1);
        img_write(image, "D:/z33.jpg");
    }

    int file_number_of_lines(const char *data_path, NumN &number) {
        std::ifstream io(data_path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", data_path);
            return 0;
        }

        number = 0;
        std::string line;
        while (std::getline(io, line)) {
            number++;
        }
        return 1;
    }

    // input starts at beginning.
    void file_number_of_lines(std::istream &input, NumN &number) {
        number = 0;
        std::string line;
        while (std::getline(input, line)) {
            number++;
        }
        input.clear();
        input.seekg(0, input.beg);
    }

    void data_parse_from_file(const char *data_path, TenR &A) {
        std::ifstream io(data_path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", data_path);
            return;
        }

        m21log("aaa");
        NumN number;
        file_number_of_lines(data_path, number);
        if (number < 2) {
            return;
        }
        std::string line;
        std::string name;
        NumN cols = 0;

        while (getline(io, line)) {
            std::stringstream linestream(line);
            std::string value;

            while (getline(linestream, value, ',')) {
                std::cout << "Value(" << value << ")\n";
            }
            std::cout << "Line Finished" << std::endl;
        }

//        while (1){
//            if(!std::getline(io, line)){
//                break;
//            }
//            std::istringstream iss(line);
//            while(1){
//                if (iss.peek() == ',') {
//                    iss.ignore();
//                }
//                if (!iss) {
//                    break;
//                }
//                iss >> cols;
//                m21log("",  cols);
//            }
//        }
        if (name != "cols:") {
            return;
        }
    }

    /*data_config is text, format:
     * bg color
     * data path, fg color, character_ascii
     * e.x.:
    0, 0, 0
    data_01_path, 255, 0, 255, 44
    data_02_path, 0, 255, 255, 44
    data_03_path, 255, 255, 0, 44
     */
    void
    ml_img_draw_and_save_csv_like_multiple(const char *data_config_path, const char *image_path,
                                           NumN axis_x,
                                           NumN axis_y) {

        std::ifstream io(data_config_path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", data_config_path);
            return;
        }
        std::vector<std::vector<std::string> > output;
        data_parse_csv_like(io, output, 10, ',');

        if (output.size() < 2) {
            printf("format error!\n");
            m21log(output);
            return;
        }
        NumN num_files = output.size() - 1;

        const char *data_path;
        char character;
        VecN color;
        color.setSize(3);
        color = 255, 255, 255;
        VecN color_bg;
        color_bg.setSize(3);
        color_bg = 0, 0, 0;
        TenR X_sample, Y_sample;
        TenR image;

        VecR tmp2;
        data_string_to_num(output[0], tmp2);
        color_bg(1) = (NumN) tmp2(1);
        color_bg(2) = (NumN) tmp2(2);
        color_bg(3) = (NumN) tmp2(3);
        color_bg.log("bg color");
        NumB isSetBgColor;
        for (NumN i = 1; i <= num_files; ++i) {
            data_path = output[i][0].c_str();
            data_string_to_num(output[i], tmp2, 2);
            color(1) = (NumN) tmp2(1);
            color(2) = (NumN) tmp2(2);
            color(3) = (NumN) tmp2(3);
            character = (char) (NumN) tmp2(4);

            printf("parsing file %s\n", data_path);
            data_get_csv_like_sample_pair(data_path, X_sample, Y_sample, character);
            X_sample.logInfo();
            Y_sample.logInfo();
//        X_sample.log();
//        Y_sample.log();
            color.log("color");
            if (i == 1) {
                isSetBgColor = 1;
            } else {
                isSetBgColor = 0;
            }
            img_draw_curve(X_sample, Y_sample, image, color, color_bg, isSetBgColor, axis_x,
                           axis_y);
        }
        img_write(image, image_path);
    }

    /*data_config is text, format:
     * bg color
     * data path, fg color, character_ascii
     * e.x.:
    0, 0, 0
    data_01_path, 255, 0, 255, 44
    data_02_path, 0, 255, 255, 44
    data_03_path, 255, 255, 0, 44
     */
    void ml_img_draw_and_save_data_2d_multiple(const char *data_config_path, const char *image_path,
                                               NumN axis_x,
                                               NumN axis_y) {

        std::ifstream io(data_config_path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", data_config_path);
            return;
        }
        std::vector<std::vector<std::string> > output;
        data_parse_csv_like(io, output, 10, ',');

        if (output.size() < 2) {
            printf("format error!\n");
            m21log(output);
            return;
        }
        NumN num_files = output.size() - 1;

        const char *data_path;
        char character;
        VecN color;
        color.setSize(3);
        color = 255, 255, 255;
        VecN color_bg;
        color_bg.setSize(3);
        color_bg = 0, 0, 0;
        TenR data, Y_sample;
        TenR image;

        VecR tmp2;
        data_string_to_num(output[0], tmp2);
        color_bg(1) = (NumN) tmp2(1);
        color_bg(2) = (NumN) tmp2(2);
        color_bg(3) = (NumN) tmp2(3);
        color_bg.log("bg color");
        NumB isSetBgColor;
        NumR a1, b1, a2, b2;
        for (NumN i = 1; i <= num_files; ++i) {
            data_path = output[i][0].c_str();
            data_string_to_num(output[i], tmp2, 2);
            character = (char) (NumN) tmp2(4);

            printf("parsing file %s\n", data_path);
            data_get_csv_like_data_2d(data_path, data, character);
            data.logInfo("data");
            NumR _a1, _b1, _a2, _b2;
            la_data_2d_bound(data, _a1, _b1, _a2, _b2);
            if (i == 1) {
                a1 = _a1;
                b1 = _b1;
                a2 = _a2;
                b2 = _b2;
            } else {
                if (_a1 < a1) {
                    a1 = _a1;
                }
                if (_b1 > b1) {
                    b1 = _b1;
                }
                if (_a2 < a2) {
                    a2 = _a2;
                }
                if (_b2 > b2) {
                    b2 = _b2;
                }
            }
        }
        MatR M;
        la_getScaleTranslationMatrix2D(M, a1, b1, a2, b2,
                                       0, axis_x, 0, axis_y);

        MatN matrix_axis(axis_x, axis_y);
        for (NumN i = 1; i <= num_files; ++i) {
            data_path = output[i][0].c_str();
            data_string_to_num(output[i], tmp2, 2);
            color(1) = (NumN) tmp2(1);
            color(2) = (NumN) tmp2(2);
            color(3) = (NumN) tmp2(3);
            character = (char) (NumN) tmp2(4);

            printf("parsing file %s\n", data_path);
            data_get_csv_like_data_2d(data_path, data, character);
            data.logInfo("data");
            la_scale_data_2d(data, M);
//        X_sample.log();
//        Y_sample.log();
            color.log("color");
            if (i == 1) {
                isSetBgColor = 1;
            } else {
                isSetBgColor = 0;
            }
            matrix_axis.zeros();
            la_draw_data_2d_at_matrix_axis_no_scale(data, matrix_axis);
            img_write_function(matrix_axis, image, color, color_bg, isSetBgColor);
        }
        img_write(image, image_path);
    }

    // data: nr * (x, y) or y with x setting to index.
    void math21_img_plot_data_2d_with_fixed_T(const MatR &batch_data, const MatR &M,
                                              const VecN &color, TenR &image,
                                              NumN axis_x,
                                              NumN axis_y) {

        if (batch_data.isEmpty()) {
            return;
        }
        MATH21_ASSERT((batch_data.dims() == 2 && batch_data.dim(2) == 2)
                      || (batch_data.dims() == 1))

        VecN color_bg;
        color_bg.setSize(3);
        color_bg = 0, 0, 0;
//        color_bg = 255, 255, 255;
//        color_bg.log("bg color");

        MatN matrix_axis(axis_x, axis_y);
        MatR data;
        la_scale_data_2d(batch_data, data, M);
//        X_sample.log();
//        Y_sample.log();
//        color.log("color");

        matrix_axis.zeros();
        la_draw_data_2d_at_matrix_axis_no_scale(data, matrix_axis);
        img_write_function(matrix_axis, image, color, color_bg, 0);
    }

    // data: nr * (x, y) or y with x setting to index.
    void math21_img_plot_data_2d(const MatR &batch_data,
                                 const VecN &color, TenR &image,
                                 NumN axis_x,
                                 NumN axis_y) {

        if (batch_data.isEmpty()) {
            return;
        }
        MATH21_ASSERT((batch_data.dims() == 2 && batch_data.dim(2) == 2)
                      || (batch_data.dims() == 1))

        NumR a1, b1, a2, b2;
        math21_la_data_2d_bound(batch_data, a1, b1, a2, b2);

        MatR M;
        la_getScaleTranslationMatrix2D(M, a1, b1, a2, b2,
                                       0, axis_x, 0, axis_y);

        math21_img_plot_data_2d_with_fixed_T(batch_data, M,
                                             color, image, axis_x, axis_y);
    }

    // data: nr * (x, y) or y with x setting to index.
    void star_img_draw_and_save_data_2d(const MatR &batch_data,
                                        const VecN &color, const char *image_path,
                                        NumN axis_x,
                                        NumN axis_y) {

        if (batch_data.isEmpty()) {
            return;
        }
        MATH21_ASSERT((batch_data.dims() == 2 && batch_data.dim(2) == 2)
                      || (batch_data.dims() == 1))

        VecN color_bg;
        color_bg.setSize(3);
        color_bg = 0, 0, 0;
//        color_bg = 255, 255, 255;
        TenR image;
//        color_bg.log("bg color");

        img_init_image(image, axis_x, axis_y, color_bg, 1);

        math21_img_plot_data_2d(batch_data,
                                color, image,
                                axis_x,
                                axis_y);

        img_write(image, image_path);
    }

    // data: batch * nr * (x, y) or batch * y with x setting to index.
    void math21_img_plot_data_2d_multiple_with_fixed_T(const TenR &batch_data, const MatR &M,
                                                       const Seqce<VecN> &colors, TenR &image,
                                                       NumN axis_x,
                                                       NumN axis_y) {

        if (batch_data.isEmpty()) {
            return;
        }
        MATH21_ASSERT((batch_data.dims() == 3 && batch_data.dim(3) == 2)
                      || (batch_data.dims() == 2))
        MATH21_ASSERT(batch_data.dim(1) == colors.size())

        NumN num_files = batch_data.dim(1);

        VecN color_bg;
        color_bg.setSize(3);
        color_bg = 0, 0, 0;

        MATH21_ASSERT(M.isSameSize(3, 3))

        MatN matrix_axis(axis_x, axis_y);
        for (NumN i = 1; i <= num_files; ++i) {
            VecN X;
            X.setSize(batch_data.dims());
            X = 0;
            X(1) = i;
            TensorView<NumR> data0 = batch_data.shrinkView(X);
//            data0.logInfo("data");
            MatR data;
            la_scale_data_2d(data0, data, M);
//        X_sample.log();
//        Y_sample.log();
            const VecN &color = colors(i);
//            color.log("color");

            matrix_axis.zeros();
            la_draw_data_2d_at_matrix_axis_no_scale(data, matrix_axis);
            img_write_function(matrix_axis, image, color, color_bg, 0);
        }
    }

    // data: batch * nr * (x, y) or batch * y with x setting to index.
    void math21_img_plot_data_2d_multiple(const TenR &batch_data,
                                          const Seqce<VecN> &colors, TenR &image,
                                          NumN axis_x,
                                          NumN axis_y) {

        if (batch_data.isEmpty()) {
            return;
        }

        MATH21_ASSERT((batch_data.dims() == 3 && batch_data.dim(3) == 2)
                      || (batch_data.dims() == 2))

        NumR a1, b1, a2, b2;

        math21_la_data_2d_bound_in_batch(batch_data, a1, b1, a2, b2);

        MatR T;
        la_getScaleTranslationMatrix2D(T, a1, b1, a2, b2,
                                       1, axis_x, 1, axis_y);

        math21_img_plot_data_2d_multiple_with_fixed_T(batch_data, T,
                                                      colors, image, axis_x, axis_y);
    }

    // data: batch * nr * (x, y) or batch * y with x setting to index.
    void star_img_draw_and_save_data_2d_multiple(const TenR &batch_data,
                                                 const Seqce<VecN> &colors, const char *image_path,
                                                 NumN axis_x,
                                                 NumN axis_y) {

        if (batch_data.isEmpty()) {
            return;
        }

        VecN color_bg;
        color_bg.setSize(3);
        color_bg = 0, 0, 0;
        TenR image;
        img_init_image(image, axis_x, axis_y, color_bg, 1);

        math21_img_plot_data_2d_multiple(batch_data, colors, image, axis_x, axis_y);

        img_write(image, image_path);
    }

    void math21_sample_sine_wave(MatR &A) {
        NumN n = 10000;
        A.setSize(n, 2);
        NumR x;
        for (NumN i = 1; i <= n; ++i) {
            x = i / 1000.0;
            A(i, 1) = x;
            A(i, 2) = xjsin(2 * MATH21_PI * x);
        }
    }

    // if n = 0, then we use step.
    void math21_sample_abs_sine(MatR &A, NumR a, NumR b, NumN n, NumR step) {
        MATH21_ASSERT(a <= b)
        if (n == 0) {
            MATH21_ASSERT(step > 0)
            n = (NumN) ((b - a) / step + 1);
        } else {
            MATH21_ASSERT(n >= 2)
            step = (b - a) / (n - 1);
        }
        A.setSize(n, 2);
        NumR x;
        for (NumN i = 1; i <= n; ++i) {
            x = a + step * (i - 1);
            A(i, 1) = x;
            A(i, 2) = 12.5 + xjabs(xjsin(x));
        }
    }

    void math21_sample_heart_and_abs_sine(TenR &A) {
        MatR B;
        math21_sample_heart(B);

        A.setSize(2, B.nrows(), 2);
        NumR a1, b1, a2, b2;
        math21_la_data_2d_bound(B, a1, b1, a2, b2);


        MatR C;
        math21_sample_abs_sine(C, a1, b1, B.nrows(), 0);

        VecN X;
        X.setSize(A.dims());
        X = 1, 0, 0;
        TenSubR ts = A.shrinkSub(X);
        ts.assign(B);

        X = 2, 0, 0;
        TenSubR ts_2 = A.shrinkSub(X);
        ts_2.assign(C);
    }

    void test_ml_img_draw_and_save_data_2d() {
        TenR batch_data;
        math21_sample_heart_and_abs_sine(batch_data);

        Seqce<VecN> colors(batch_data.dim(1));
        colors.at(1).setSize(3);
        colors.at(1) = 255, 0, 0;
        colors.at(2).setSize(3);
        colors.at(2) = 0, 255, 0;

        NumN axis_x = 100;
        NumN axis_y = 50;

        const char *image_path = "a.png";
        star_img_draw_and_save_data_2d_multiple(batch_data,
                                                colors, image_path,
                                                500,
                                                400);

        TenR batch_data2(2, 3);
        batch_data2.letters();
        const char *image_path2 = "a2.png";
        star_img_draw_and_save_data_2d_multiple(batch_data2,
                                                colors, image_path2,
                                                axis_x,
                                                axis_y);

        MatR batch_data3;
        math21_sample_sine_wave(batch_data3);
        const char *image_path3 = "a3.png";
        star_img_draw_and_save_data_2d(batch_data3,
                                       colors(1), image_path3,
                                       50,
                                       40);

        MatR batch_data4(21);
        batch_data4.letters();
        const char *image_path4 = "a4.png";
        star_img_draw_and_save_data_2d(batch_data4,
                                       colors(1), image_path4,
                                       axis_x,
                                       axis_y);

    }

    int star_ml_img_draw_and_save_heart(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\theart \n"
                       "\t(image_path axis_x axis_y)\n");
                m21log("\te.x., \n"
                       "\theart.png #or /sdcard/heart.png\n"
                       "\t200     # axis_x\n"
                       "\t150     # axis_y\n");
                return 1;
            }
        }

        if (argc != 1 && argc != 4) {
            return 0;
        }

        std::string image_path;
        NumN axis_x;
        NumN axis_y;

        if (argc == 1) {
            image_path = star_string_to_save_path("heart.png");
            axis_x = 200;
            axis_y = 150;
        } else {
            image_path = argv[1];
            axis_x = math21_string_to_NumN(argv[2]);
            axis_y = math21_string_to_NumN(argv[3]);
        }
        TenR batch_data;
        math21_sample_heart_and_abs_sine(batch_data);

        Seqce<VecN> colors(batch_data.dim(1));
        colors.at(1).setSize(3);
        colors.at(1) = 255, 0, 0;
        colors.at(2).setSize(3);
        colors.at(2) = 0, 255, 0;


        star_img_draw_and_save_data_2d_multiple(batch_data,
                                                colors, image_path.c_str(),
                                                axis_x,
                                                axis_y);
        return 1;
    }

    /*data is text, format:
    1, 2, 3
    4, 5, 6
    7, 8, 9
     */
    void ml_img_draw_and_save_csv_like(const char *data_path, const char *image_path, NumN axis_x,
                                       NumN axis_y,
                                       char character) {
        TenR X_sample, Y_sample;
        data_get_csv_like_sample_pair(data_path, X_sample, Y_sample, character);
        X_sample.logInfo();
        Y_sample.logInfo();
//        X_sample.log();
//        Y_sample.log();

        VecN color;
        color.setSize(3);
        color = 255, 255, 255;
        VecN color_bg;
        color_bg.setSize(3);
        color_bg = 0, 0, 0;
        TenR image;
        img_draw_curve(X_sample, Y_sample, image, color, color_bg, 1, axis_x, axis_y);
        img_write(image, image_path);
    }

    /*data is text, format:
    cols: 2
    1 2
    3 4
    5 6*/
    void
    ml_img_draw_and_save(const char *data_path, const char *image_path, NumN axis_x, NumN axis_y) {
        std::ifstream io(data_path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", data_path);
            return;
        }

        NumN number;
        file_number_of_lines(data_path, number);
        if (number < 2) {
            return;
        }
        std::string line;
        std::getline(io, line);
        std::istringstream iss(line);
        std::string name;
        NumN cols = 0;
        iss >> name;
        iss >> cols;
        if (name != "cols:") {
            return;
        }
        if (cols != 1 && cols != 2) {
            return;
        }
        TenR X_sample, Y_sample;

        const NumN n_samples = number - 1;
        X_sample.setSize(n_samples, 1);
        Y_sample.setSize(n_samples, 1);
        NumR tmp;
        for (NumN i = 1; i <= n_samples; i++) {
            std::getline(io, line);
            std::istringstream iss(line);
            if (cols == 1) {
                X_sample(i, 1) = i;
                iss >> tmp;
                Y_sample(i, 1) = tmp;
            } else {
                iss >> tmp;
                X_sample(i, 1) = tmp;
                iss >> tmp;
                Y_sample(i, 1) = tmp;
            }
        }

        X_sample.logInfo();
        Y_sample.logInfo();
//        X_sample.log();
//        Y_sample.log();

        VecN color;
        color.setSize(3);
        color = 255, 255, 255;
        VecN color_bg;
        color_bg.setSize(3);
        color_bg = 0, 0, 0;
        TenR image;
        img_draw_curve(X_sample, Y_sample, image, color, color_bg, 1, axis_x, axis_y);
        img_write(image, image_path);
    }

    void img_draw_and_save_test() {
//        ml_img_draw_and_save("D:\\mathxyz\\z\\z.txt", "D:/z33.jpg", 50, 30);

        TenR A;
//        data_parse_from_file("D:\\mathxyz\\z\\z.txt",A);
//        data_parse_CSV("D:\\mathxyz\\z\\z.txt", A);

        ml_data_parse_and_save_csv_like("D:\\mathxyz\\z\\z.txt",
                                        "D:\\mathxyz\\z\\zz3.txt",
                                        ',', 10);
        ml_data_parse_csv_like_n_1("D:\\mathxyz\\z\\z.txt", A, ',');
//        A.log("A");
    }

    struct m21function_sin_wave {
// y(t) = A*sin(2*pi*f*t + phi)
// y(t) = A*sin(2*pi*f*(t+t0)), t0 = phi/(2*pi*f)
// A amplitude, f ordinary frequency, phi phase
    private:
        NumR A;
        NumR f;
        NumR phi;
    public:
        m21function_sin_wave(NumR A, NumR f, NumR phi) {
            this->A = A;
            this->f = f;
            this->phi = phi;
        }

        void shiftBy(NumR t0) {
            phi = t0 * 2 * MATH21_PI * f;
        }

        NumR valueAt(const NumR &x) {
            return A * sin(2 * MATH21_PI * f * x + phi);
        }
    };

    template<typename FunctionType>
    void math21_data_generate_sample(MatR &x, MatR &y, NumN mbs, NumN imb, const VecR &ix, FunctionType &f) {
        NumN n = ix.size();
        if (!x.isSameSize(mbs, n)) {
            x.setSize(mbs, n);
        }
        if (!y.isSameSize(mbs, n)) {
            y.setSize(mbs, n);
        }
        MATH21_ASSERT(ix.size() == n)
        for (NumN i = 1; i <= n; ++i) {
            x(imb, i) = ix(i);
            y(imb, i) = f.valueAt(ix(i));
        }
    }

    void test_sin_wave() {
        VecR t;
        MatR x;
        MatR y;
        math21_operator_container_create_with_increment(t, 0, 0.01, 1, 0, 100);
        NumN mbs = 3;
        for (NumN imb = 1; imb <= mbs; ++imb) {
            m21function_sin_wave f(1, 1, 2);
            f.shiftBy((imb - 1) / (NumR) mbs);
            math21_data_generate_sample(x, y, mbs, imb, t, f);
        }
//    x.log("x");
//    y.log("y");
//    t.log("t");
        MatR B;
//        Tensor<NumR32> B;
//        math21_io_generic_type_write_to_file(B,
//                                             "/mathxyz/y/lstm-sin/data.bin2.txt",
//                                             0, 0);
//        math21_io_generic_type_read_from_file(B, "z2.txt", 0, 0);
        math21_io_generic_type_read_from_file(B, "/mathxyz/y/lstm-sin/sin_data.bin",
                                              0, 1);
        B.log("B");
    }


    void img_draw_test() {
//        img_draw_and_save_test();

        test_sin_wave();

        exit(0);
    }

}
