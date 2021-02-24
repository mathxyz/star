#pragma once

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <functional>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include <algorithm>

#include "nn_error.h"

namespace star {

    template<typename T>
    T *reverse_endian(T *p) {
        std::reverse(reinterpret_cast<char *>(p),
                     reinterpret_cast<char *>(p) + sizeof(T));
        return p;
    }

    inline NumB star_is_little_endian() {
        int x = 1;
        if (*reinterpret_cast<char *>(&x) != 0) {
            return 1;
        } else {
            return 0;
        }
    }

    int sky_tool_log_endian(int argc, const char **argv);

    int sky_tool_log_num_type(int argc, const char **argv);

    int star_tool_monitor_turn_on(int argc, const char **argv);

    int star_tool_monitor_turn_off(int argc, const char **argv);

    int star_tool_timer_setInterval(int argc, const char **argv);

    int star_tool_timer_setTimeout(int argc, const char **argv);

    int star_tool_system_cmd(int argc, const char **argv);

    int star_tool_curl(int argc, const char **argv);

    int star_tool_wget(int argc, const char **argv);
}