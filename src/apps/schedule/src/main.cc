#include <iostream>
#include <cstring>
#include "files.h"

int main(int argc, char **argv) {
    printf("Vehicle Scheduling\n"
           "Version: 1.0\n"
           "Step 1\n");
    StarCsvVector output;
    std::istream *p_input;
    std::ifstream io;
    if (argc == 1) {
        printf("Close this window. Drag input file to app directly!\n");
        p_input = &cin;
    } else if (argc >= 2) {
        const char *path = argv[1];
        io.open(path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", path);
            return 1;
        }
        p_input = &io;
    } else {
        return 0;
    }

#ifdef MATH21_LINUX00

    const char *path = "paper_config.txt";

    io.open(path, std::ifstream::in);
    if (!io.is_open()) {
        printf("open %s fail!\n", path);
        return 1;
    }
    p_input = &io;
#endif

    std::istream &input = *p_input;
    star_data_parse_csv_like_no_space(input, output, 10, ' ');
    if (output.size() != 2) {
        return 0;
    }
    const char *str1 = output(1)(1).c_str();
    const char *str2 = output(2)(1).c_str();

    if (math21_string_is_equal(str1, "convert")) {
        parse_schedule_data_format_2(str2);
    } else {
        schedule_process(str1, str2);
    }
    return 0;
}