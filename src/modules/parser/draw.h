#pragma once

#include "inner.h"

namespace star {

    struct ImageDraw_Tensor : public ImageDraw {

    private:
        std::string dir;
        NumN axis_x;
        NumN axis_y;
        TenR image;
        Seqce <VecN> colors_traj;
        Interval2D I;
        MatR T;
        NumB isTSet;
        VecN color_bg;

        void init();

    public:

        void reset() override;

        ImageDraw_Tensor();

        void setAxisSize(NumN axis_x, NumN axis_y);

        // deprecate
        void draw(const TenR &A, const char *name, NumB isBatch = 1) override;

        void setDir(const char *name) override;

        // don't save
        void plot(const TenR &data, const char *name, NumB isBatch = 1) override;

        void get_T(const TenR &data, NumB isBatch);

        void save(const char *name) override;
    };

    void star_plot_container_data(const Seqce <MatR> &data, const char *path);

    int sky_plot_data(int argc, const char **argv);
}