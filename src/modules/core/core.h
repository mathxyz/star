#pragma once

#include "inner.h"

namespace star {

    typedef _Map<std::string, int (*)(int argc, const char **argv)> StarMaps;

    struct StarPointer {
        NumB used;
        void *p;

        StarPointer() {
            used = 0;
            p = 0;
        }
    };

    class Star {
    private:
        StarMaps maps;
    public:
        StarPointer starPointer[4];

        Star();

        virtual ~Star() {}

        const StarMaps &getMaps() const;
    };

    Star &star_get_default();
    void star_test();
}