#include "inner.h"
#include "book.h"

using namespace star;

int sky_ml_rl_chapter01_tic_tac_toe(int argc, const char **argv) {

    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tsky_ml_rl_chapter01_tic_tac_toe config_path\n";
            return 1;
        }
    }

    if (argc > 2) {
        return 0;
    }

    StarCsvVector output;
    std::istream *p_input;
    std::ifstream io;
    if (argc == 1) {
        printf("Close this window. Drag input file to app directly!\n");
        p_input = &std::cin;


#ifdef MATH21_LINUX
        const char *path = "/mathxyz/z/src/3rdparty/math21/src/rl/config.txt";

        io.open(path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", path);
            return 1;
        }
        p_input = &io;
#endif

    } else if (argc == 2) {
        const char *path = argv[1];
        io.open(path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", path);
            return 1;
        }
        p_input = &io;
    }

    std::istream &input = *p_input;
    star_data_parse_csv_like_no_space(input, output, 10, ' ');
    if (output.size() != 4) {
        return 0;
    }

    std::string s;
    NumN nr;
    NumN nc;
    NumN epochs;
    NumN first_or_second;
    s = output(1)(1);
    nr = (NumN) math21_string_to_NumR(s);
    s = output(2)(1);
    nc = (NumN) math21_string_to_NumR(s);
    s = output(3)(1);
    epochs = (NumN) math21_string_to_NumR(s);
    s = output(4)(1);
    first_or_second = (NumN) math21_string_to_NumR(s);
//    output.log();
    math21_rl_chapter01_tic_tac_toe(nr, nc, epochs, first_or_second);
    return 1;
}

int sky_ml_rl_chapter02_k_armed_bandit(int argc, const char **argv) {

    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tml_rl_chapter02_k_armed_bandit config_path\n";
            std::cout << "\tconfig_path:\n";
            std::cout << "\t2       # figure num, 0, 2, 3, 4, 5, 6\n"
                         "\t2000    # runs\n"
                         "\t1000    # time\n"
                         "\t200     # axis_x\n"
                         "\t150     # axis_y\n";
            return 1;
        }
    }

    if (argc > 2) {
        return 0;
    }

    StarCsvVector output;
    std::istream *p_input;
    std::ifstream io;
    if (argc == 1) {
        printf("Close this window! Drag input file to app directly!\n");
        p_input = &std::cin;


#ifdef MATH21_LINUX
        const char *path = "bandit_config.txt";
        io.open(path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", path);
            return 1;
        }
        p_input = &io;
#endif

    } else if (argc == 2) {
        const char *path = argv[1];
        io.open(path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", path);
            return 1;
        }
        p_input = &io;
    }

    std::istream &input = *p_input;

    star_data_parse_csv_like_no_space(input, output, 10, ' ');

//    output.log();
    if (output.size() != 5) {
        return 0;
    }

//    output.log();

    ImageDraw_Tensor f;
    f.setAxisSize(math21_string_to_NumN(output(4)(1)), math21_string_to_NumN(output(5)(1)));
    math21_rl_chapter02_k_armed_bandit(f, math21_string_to_NumN(output(1)(1)), math21_string_to_NumN(output(2)(1)),
                                       math21_string_to_NumN(output(3)(1)));

    return 1;
}

int sky_ml_rl_chapter03_grid_world(int argc, const char **argv) {

    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tml_rl_chapter03_grid_world\n";
            return 1;
        }
    }

    if (argc > 1) {
        return 0;
    }
    math21_rl_chapter03_grid_world();

    return 1;
}

int sky_ml_rl_chapter04_gamblers_problem(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tml_rl_chapter04_gamblers_problem\n";
            return 1;
        }
    }

    if (argc > 1) {
        return 0;
    }
    ImageDraw_Tensor f;
    math21_rl_chapter04_gamblers_problem(f);

    return 1;
}

int sky_ml_rl_chapter04_car_rental(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tml_rl_chapter04_car_rental\n";
            return 1;
        }
    }

    if (argc > 1) {
        return 0;
    }
    ImageDraw_Tensor f;
    math21_rl_chapter04_car_rental(f);
    return 1;
}

int sky_ml_rl_chapter04_car_rental_synchronous(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tml_rl_chapter04_car_rental_synchronous\n";
            return 1;
        }
    }

    if (argc > 1) {
        return 0;
    }
    ImageDraw_Tensor f;
    math21_rl_chapter04_car_rental_synchronous(f);
    return 1;
}

int sky_ml_rl_chapter04_grid_world(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tml_rl_chapter04_grid_world\n";
            return 1;
        }
    }

    if (argc > 1) {
        return 0;
    }
    math21_rl_chapter04_grid_world();
    return 1;
}

int sky_ml_rl_chapter05_blackjack(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tml_rl_chapter05_blackjack\n";
            return 1;
        }
    }

    if (argc > 1) {
        return 0;
    }
    ImageDraw_Tensor f;
    math21_rl_chapter05_blackjack(f);
    return 1;
}

int sky_ml_rl_chapter06_random_walk(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tml_rl_chapter06_random_walk\n";
            return 1;
        }
    }

    if (argc > 1) {
        return 0;
    }
    ImageDraw_Tensor f;
    math21_rl_chapter06_random_walk(f);
    return 1;
}

int sky_ml_rl_chapter06_cliff_walking(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tml_rl_chapter06_cliff_walking\n";
            return 1;
        }
    }

    if (argc > 1) {
        return 0;
    }
    ImageDraw_Tensor f;
    math21_rl_chapter06_cliff_walking(f);
    return 1;
}
