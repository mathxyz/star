#include <includes/star.h>
#include "util.h"
#include "files.h"

#ifdef MATH21_WINDOWS

#include <windows.h>

#endif
namespace star {
    int sky_tool_log_endian(int argc, const char **argv) {
        if (star_is_little_endian()) {
            m21log("little endian");
        } else {
            m21log("big endian");
        }
        return 1;
    }

    int sky_tool_log_num_type(int argc, const char **argv) {
        math21_tool_log_num_type();
        return 1;
    }

    int star_tool_monitor_turn_on(int argc, const char **argv) {
#ifdef MATH21_WINDOWS
        // Power the display on/low/off via lParam= -1/1/2.
        SendMessage((HWND) 0xffff, WM_SYSCOMMAND, SC_MONITORPOWER, -1);
#else
        const char *cmd = "sleep 1; xset dpms force on";
        system(cmd);
#endif
        return 1;
    }

    int star_tool_monitor_turn_off(int argc, const char **argv) {
#ifdef MATH21_WINDOWS
        SendMessage((HWND) 0xffff, WM_SYSCOMMAND, SC_MONITORPOWER, 2);
#else
        const char *cmd = "sleep 1; xset dpms force off";
        system(cmd);
#endif
        return 1;
    }


    int star_tool_timer_set(int argc, const char **argv, NumN type) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tstar_tool_timer_setInterval \n"
                       "\ttime function\n"
                       "\t  time (seconds)\n"
                       "\t  function (star function)\n");

                m21log("\te.x., \n"
                       "\t1 function=heart\n");
                return 1;
            }
        }

        if (argc < 3) {
            return 0;
        }

        Seqce<std::string> *p_strs = new Seqce<std::string>();
        math21_string_argv_2_string(argc, argv, *p_strs, 2);

        timer *p_t = new timer();

        if (type == 1) {
            {
                Star &star = star_get_default();
                if (star.starPointer[0].used) {
                    m21log("no free space left in star!");
                    return 0;
                }
                star.starPointer[0].used = 1;
                star.starPointer[0].p = p_strs;

                if (star.starPointer[1].used) {
                    m21log("no free space left in star!");
                    return 0;
                }
                star.starPointer[1].used = 1;
                star.starPointer[1].p = p_t;
            }

            p_t->setInterval([]() {
                Star &star = star_get_default();
                auto &maps = star.getMaps();
                Seqce<std::string> &strs = *(Seqce<std::string> *) star.starPointer[0].p;
                std::string fname = star_string_get_assign_right(strs(1), "function");
                strs.log();
                int argc3 = 0;
                const char **argv3 = 0;
                math21_string_2_argv(strs, argc3, argv3);
                maps.valueAt(fname)(argc3, argv3);
                math21_string_free_argv(argc3, argv3);
            }, math21_string_to_NumN(argv[1]) * 1000);
        } else if (type == 2) {
            {
                Star &star = star_get_default();
                if (star.starPointer[2].used) {
                    m21log("no free space left in star!");
                    return 0;
                }
                star.starPointer[2].used = 1;
                star.starPointer[2].p = p_strs;

                if (star.starPointer[3].used) {
                    m21log("no free space left in star!");
                    return 0;
                }
                star.starPointer[3].used = 1;
                star.starPointer[3].p = p_t;
            }

            p_t->setTimeout([]() {
                Star &star = star_get_default();
                auto &maps = star.getMaps();
                Seqce<std::string> &strs = *(Seqce<std::string> *) star.starPointer[2].p;
                std::string fname = star_string_get_assign_right(strs(1), "function");
                strs.log();
                int argc3 = 0;
                const char **argv3 = 0;
                math21_string_2_argv(strs, argc3, argv3);
                maps.valueAt(fname)(argc3, argv3);
                math21_string_free_argv(argc3, argv3);

                delete &strs;
                delete (timer *) star.starPointer[3].p;
                star.starPointer[2].used = 0;
                star.starPointer[3].used = 0;
            }, math21_string_to_NumN(argv[1]) * 1000);
        }
        return 1;
    }

    int star_tool_timer_setInterval(int argc, const char **argv) {
        return star_tool_timer_set(argc, argv, 1);
    }

    int star_tool_timer_setTimeout(int argc, const char **argv) {
        return star_tool_timer_set(argc, argv, 2);
    }

    // error, remove space when parse
    int star_tool_system_cmd(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tstar_tool_system_cmd \"cmd\"\n");
                m21log("\te.x., \n"
                       "\t\"ls .\"\n");
                return 1;
            }
        }

        if (argc != 2) {
            return 0;
        }

#ifdef MATH21_WINDOWS
        printf("not implement");
#else
        const char *cmd = argv[1];
//        const char *cmd = "curl -O http://leshi.cdn-zuyida.com/20180113/ZtgbOBzY/800kb/hls/QgOY1719029.ts";
        system(cmd);
#endif
        return 1;
    }

    // deprecate, use wget instead.
    int star_tool_curl(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tstar_tool_curl \"cmd\"\n"
                       "\t  --url_format\n"
                       "\t  --start (NumN)\n"
                       "\t      plot j-th point in data.\n"
                       "\t      The default value is 'null'.\n"
                       "\t  --num (NumN)\n"
                       "\t  --missing_only\n"
                       "\t      download missing only\n"
                       "\t  --print\n");
                m21log("\te.x., \n"
                       "\t\"--url_format http://a/%d.mp3 --start 0 --num 3\"\n");
                return 1;
            }
        }

        NumB isLog = 0;
        std::string url_format;
        NumN start = 0;
        NumN num = 0;
        for (int i = 1; i < argc; ++i) {
            if (math21_string_is_equal(argv[i], "--url_format")) {
                url_format = argv[i + 1];
                ++i;
            } else if (math21_string_is_equal(argv[i], "--start")) {
                start = math21_string_to_NumN(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--num")) {
                num = math21_string_to_NumN(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--print")) {
                isLog = 1;
            }
        }

        if (url_format.empty()) {
            return 1;
        }

#ifdef MATH21_WINDOWS
        printf("not implement");
#else
        char url[1024];
        std::string cmd;
        for (NumN i = 0; i < num; ++i) {
            sprintf(url, url_format.c_str(), start + i);
            cmd = "curl -O " + std::string(url);
            if (isLog) {
                m21log(cmd);
            }
//        const char *cmd = "curl -O http://leshi.cdn-zuyida.com/20180113/ZtgbOBzY/800kb/hls/QgOY1719029.ts";
            system(cmd.c_str());
        }
#endif
        return 1;
    }

    int star_tool_wget(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tstar_tool_wget \"cmd\"\n"
                       "\t  --url_format\n"
                       "\t  --start (NumN)\n"
                       "\t      plot j-th point in data.\n"
                       "\t      The default value is 'null'.\n"
                       "\t  --num (NumN)\n"
                       "\t  --is_missing_only\n"
                       "\t      download missing only\n"
                       "\t  --dst_dir\n"
                       "\t      destination directory\n"
                       "\t  --min_size\n"
                       "\t      min_size in M\n"
                       "\t  --print\n");
                m21log("\te.x., \n"
                       "\t\"--url_format http://a/%d.mp3 --start 0 --num 3\"\n");
                return 1;
            }
        }

        NumB isLog = 0;
        std::string url_format;
        std::string dst_dir;
        NumN start = 0;
        NumN num = 0;
        NumB is_missing_only = 0;
        NumR min_size=0;
        for (int i = 1; i < argc; ++i) {
            if (math21_string_is_equal(argv[i], "--url_format")) {
                url_format = argv[i + 1];
                ++i;
            } else if (math21_string_is_equal(argv[i], "--start")) {
                start = math21_string_to_NumN(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--num")) {
                num = math21_string_to_NumN(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--print")) {
                isLog = 1;
            } else if (math21_string_is_equal(argv[i], "--is_missing_only")) {
                is_missing_only = 1;
            } else if (math21_string_is_equal(argv[i], "--dst_dir")) {
                dst_dir = argv[i + 1];
                ++i;
            } else if (math21_string_is_equal(argv[i], "--min_size")) {
                min_size = math21_string_to_NumR(argv[i + 1]);
                ++i;
            }
        }

        if (url_format.empty()) {
            return 1;
        }

        Io io;
        if(!dst_dir.empty()){
            io.mkdirs(dst_dir);
        }

#ifdef MATH21_WINDOWS
        printf("not implement");
#else
        char url[1024];
        std::string cmd;
        for (NumN i = 0; i < num; ++i) {
            sprintf(url, url_format.c_str(), start + i);
            std::string name = star_string_get_file_name(url);
            if(!dst_dir.empty()){
                name = dst_dir + "/" +name;
            }
            if (is_missing_only && io.fileSize(name)>min_size*1024*1024) {
                if (isLog) {
                    m21log(name + " exits!");
                }
                continue;
            }
            cmd = "wget --no-check-certificate -c -O " + name + " "+ std::string(url);
            if (isLog) {
                m21log(cmd);
            }
            system(cmd.c_str());
        }
#endif
        return 1;
    }

}