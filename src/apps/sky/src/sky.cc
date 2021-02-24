#include <cstring>
#include "inner.h"
#include "sky.h"

using namespace star;

#define SKY_VERSION "1.4"
#define SKY_VERSION_DETAIL ".1"

std::string home_name = "Alice";
std::string sky_name = "sky";

void sky_init(int argc, const char **argv) {
#ifdef MATH21_ANDROID
    MATH21_ASSERT(argc >= 2)
    Io io;
    std::string storage_path = argv[0];
    std::string home_path = star_string_home_path(storage_path);
    std::string sky_path = star_string_sky_path(storage_path);
    if (!io.isDirExists(sky_path)) {

        io.mkdir(home_path);
        io.mkdir(sky_path);
        XjIoHelper ioHelper;
        XjIoHelper::setSystemPath(home_path);
        std::string s = "";
        s = s + "sky" + "  #" + argv[0] + "\n";
        ioHelper.saveString(s, "sky.txt", "sky", 1);
    }
#endif
}


struct SatisfyFunction_newline_sky {
    const _Map<std::string, int (*)(int argc, const char **argv)> &maps;

    explicit SatisfyFunction_newline_sky(const _Map<std::string, int (*)(int argc, const char **argv)> &maps) : maps(
            maps) {
    }

    NumB satisfy(const std::string &s) {
        if (maps.has(s)) {
            return 1;
        } else {
            return 0;
        }
    }
};

struct SatisfyFunction_leftout_sky {
    NumB satisfy(const std::string &s) {
        if (star_string_is_start_with(s, "#")) {
            return 1;
        } else {
            return 0;
        }
    }
};

NumB star_data_parse_csv_for_config(std::istream &input, Seqce<std::string> &output, char character_row) {
    SatisfyFunction_leftout_sky leftout;

    output.clear();
    std::string csvLine;
    while (getline(input, csvLine, character_row)) {
        if (leftout.satisfy(csvLine)) {
            continue;
        }
        csvLine = star_string_trim(csvLine);
        if (csvLine.empty()) {
            continue;
        }

        NumB isFunctionLine = 0;
        NumZ pos_eq = star_string_find_first(csvLine, '=');
        NumZ pos_space = star_string_find_first(csvLine, ' ');
        if (pos_eq < 0) {
            isFunctionLine = 1;
        } else if (pos_space > 0 && pos_space < pos_eq) {
            auto s = csvLine.substr((NumN) pos_space);
            s = star_string_trim(s);
            if (s[0] != '=') {
                isFunctionLine = 1;
            }
        }

        if (isFunctionLine) {
            std::string s;
            if (!star_string_get_word_1_from_start(csvLine, s, ' ')) {
                return 0;
            }
            output.push(s);
        } else {
            std::string s1, s2;
            if (!star_string_get_word_2_from_start(csvLine, s1, s2, '=', ' ')) {
                m21log("not key value pair!");
                return 0;
            }
            output.push(s1);
            output.push(s2);
        }
    }
    return 1;
}

void runFunctions(const StarMaps &maps, const Seqce<Seqce<std::string>> &functions) {
    int argc;
    const char **argv;
    timer time;
    for (NumN i = 1; i <= functions.size(); ++i) {
        time.start();
        math21_string_2_argv(functions(i), argc, argv);
        m21log(argv[0]);
        maps.valueAt(argv[0])(argc, argv);
        math21_string_free_argv(argc, argv);
        time.end();
        if (time.time() > 0) {
            m21log("\ntime", time.time());
        }
    }
}

/* sky
 * function
 * x=1
 * -y
 * 2
 * */
int sky(int argc, const char **argv) {
    m21log("sky-app ("
           SKY_VERSION
           ")\n");

    m21log(star_string_get_current_time_for_print());
    sky_init(argc, argv);

    Star &star = star_get_default();

    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            std::cout << "version detail: " << SKY_VERSION << SKY_VERSION_DETAIL << "\n";
            std::cout << "authors: LB, YE.\n";
            std::cout << "website: https://github.com/sky-app\n";
            _Set<std::string> X;
            star.getMaps().getX(X);
            X.log();
#ifdef MATH21_ANDROID
            // system path has already been set.
            XjIoHelper ioHelper;
            std::string s;
            math21_string_log_2_string(X, s);
            ioHelper.saveString(s, "help.txt", "sky", 1);
#endif
            return 0;
        }
    }

    std::istream *p_input = 0;
    std::ifstream io;

    // get stream
    if (argc == 1) {
        printf("Close this window.. Drag input file to app directly!\n");
        p_input = &std::cin;

#if  defined(MATH21_WINDOWSZZ)
                io.open(path, std::ifstream::in);
                if (!io.is_open()) {
                    printf("open %s fail!\n", path);
                    return 1;
                } else {
                    p_input = &io;
                }
#elif defined(MATH21_LINUX)
        //        const char *path = "/mathxyz/src/3rdparty/star/src/apps/sky/src/sky.txt";
                const char *path = "/mathxyz/convert/config.txt";

                io.open(path, std::ifstream::in);
                if (!io.is_open()) {
                    printf("open %s fail!\n", path);
                    return 1;
                } else {
                    p_input = &io;
                }
#endif

    } else if (argc == 2) {
        const char *path = argv[1];
        if (!star.getMaps().has(path)) {
            io.open(path, std::ifstream::in);
            if (!io.is_open()) {
                printf("open %s fail!\n", path);
                return 0;
            } else {
                p_input = &io;
            }
        }
    }

    // get argc, argv
    Seqce<std::string> strs;
    if (p_input) {
        std::istream &input = *p_input;

        if (!star_data_parse_csv_for_config(input, strs, 10)) {
            return 0;
        }
        if (!math21_string_is_equal(strs(1).c_str(), "sky")) {
            return 0;
        }
    } else if (argc >= 2) {
        Seqce<std::string> strings0;
        math21_string_argv_2_string(argc, argv, strings0);
        SatisfyFunction_leftout_sky leftout;
        star_string_1d_remove_element(strings0, strs, leftout);
    }

    if (strs.isEmpty()) {
        return 0;
    }

    Seqce<Seqce<std::string>> functions;
    SatisfyFunction_newline_sky newline(star.getMaps());
    star_string_1d_to_2d(strs, functions, newline);
    runFunctions(star.getMaps(), functions);
    return 0;
}


#ifndef MATH21_ANDROID

int main(int argc, const char **argv) {
    sky(argc, argv);
}

#else

JNIEXPORT void JNICALL Java_org_sky_app_Sky_run(JNIEnv *env,
                                                jclass clazz, jstring jstr) {

    const jsize len = env->GetStringUTFLength(jstr);
    const char *str = env->GetStringUTFChars(jstr, (jboolean *) 0);
    if (str == NULL) {
        return;
    }

    std::string s1 = str;
    std::string s2 = str;
    env->ReleaseStringUTFChars(jstr, str);

    const char *argv[2];
    argv[0] = s1.c_str();
    s2 = s2 + "/Alice/sky/sky.txt";
    argv[1] = s2.c_str();

//    SimpleStreamBuf ssb;
//    std::streambuf *backup;
//    backup = std::cout.rdbuf();
//    std::cout.rdbuf(&ssb);
//    std::cout << "nn2>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.\n";
//    std::cout << argv[1] << "\n";
//    std::cout.rdbuf(backup);

    sky(2, argv);
//    sky(0, argv);
}

#endif

