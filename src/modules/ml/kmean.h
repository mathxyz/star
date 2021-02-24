#pragma once

#include "inner.h"

namespace star {
    void ml_kmeans(const char *data_path, const char *data_config_path, const char *dst_dir);
}

#ifdef __cplusplus
extern "C"
{
#endif

int sky_ml_kmeans(int argc, const char **argv);

#ifdef __cplusplus
}
#endif

