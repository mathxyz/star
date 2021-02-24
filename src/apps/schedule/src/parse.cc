#include <cstdio>
#include "inner.h"
#include "parse.h"
#include "Schedule.h"
#include "files.h"


void station_name_to_num_format_2(const std::string &name, NumN &num) {
    char c = name.c_str()[0];
    num = NumN(c - 'A' + 1);
}

void station_name_to_num(const std::string &name, NumN &num) {
    char c = name.c_str()[0];
    num = NumN(c - 'a' + 1);
}

void station_num_to_name(const NumN &num, std::string &name) {
    char c = (char) (num + 'a' - 1);
    name = c;
}

void parse_schedule(const char *path, Schedule &S) {
    std::ifstream io(path, std::ifstream::in);
    if (!io.is_open()) {
        printf("open %s fail!\n", path);
        return;
    }
    StarCsvVector output0, output;
    star_data_parse_csv_like(io, output0, 10, ' ');
    star_csv_vector_remove_space(output0, output);

    if (output.isEmpty()) {
        printf("empty!\n");
        return;
    }

//    m21logInfo(output);

    NumN i, pi, qi;
    StarTime tsi, tei;
    std::string s;
    for (NumN k = 1; k <= output.size(); ++k) {
        StarCsvColumn &csvColumn = output.at(k);
        s = csvColumn(1);
        i = (NumN) math21_string_to_NumR(s);

        s = csvColumn(2);

        station_name_to_num(s, pi);

        s = csvColumn(3);
        tsi.set(s, "%d:%d");

        s = csvColumn(4);
        station_name_to_num(s, qi);

        s = csvColumn(5);
        tei.set(s, "%d:%d");

        S.add(i, pi, tsi, qi, tei);
    }

//    S.log("S");
}

void parse_schedule_data_format_2(const char *path){
    Schedule S;
    parse_schedule_data_format_2(path, S);
    std::string str = path;
    str = star_string_remove_suffix(str);
    MATH21_ASSERT(!str.empty())
    save_schedule(S, "gen_convert.txt", 0);
}

/*
 6 26     7 22    A    B
 7 46     8 42    A    B
 9  6     9 55    A    B
11  6    11 55    A    B
 * */
void parse_schedule_data_format_2(const char *path, Schedule &S) {
    std::ifstream io(path, std::ifstream::in);
    if (!io.is_open()) {
        printf("open %s fail!\n", path);
        return;
    }
    StarCsvVector output0, output;
    star_data_parse_csv_like(io, output0, 10, ' ');
    star_csv_vector_remove_space(output0, output);

    if (output.isEmpty()) {
        printf("empty!\n");
        return;
    }

//    math21_log_StarCsvVector(output);

    NumN i, pi, qi;
    StarTime tsi, tei;
    std::string s;
    NumN h, m;
    S.clear();
    for (NumN k = 1; k <= output.size(); ++k) {
        StarCsvColumn &csvColumn = output.at(k);
        s = csvColumn(1);
        h = (NumN) math21_string_to_NumR(s);

        s = csvColumn(2);
        m = (NumN) math21_string_to_NumR(s);

        tsi.set(h, m, 0);

        s = csvColumn(3);
        h = (NumN) math21_string_to_NumR(s);

        s = csvColumn(4);
        m = (NumN) math21_string_to_NumR(s);

        tei.set(h, m, 0);

        s = csvColumn(5);

        station_name_to_num_format_2(s, pi);

        s = csvColumn(6);
        station_name_to_num_format_2(s, qi);

        S.add(S.size()+1, pi, tsi, qi, tei);
    }

//    S.log("S");
}

void parse_DH(const char *path, MatR &DH) {
    std::ifstream io(path, std::ifstream::in);
    if (!io.is_open()) {
        printf("open %s fail!\n", path);
        return;
    }
    StarCsvVector output0, output;
    star_data_parse_csv_like(io, output0, 10, ' ');
    star_data_csv_like_to_MatR(output0, DH);

//    DH.log("DH");
}