#pragma once

#include "inner_c.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct star_float_pair star_float_pair;
struct star_float_pair {
    float *x;
    float *y;
};

m21list *star_data_get_lines_from_file(const char *filename);

char **star_data_get_labels(const char *filename);

m21list *star_data_read_config(const char *filename);

mllabels star_data_get_labels_from_data_config(const char *data_config);

const char *star_data_get_backup_dir(m21list *l, const char *def);

size_t star_pr_rand_size_t();

star_float_pair
star_ml_function_rnn_data_get_one_hot_n(const int *tokens, size_t *offsets, int characters, size_t len, int batch,
                                        int steps);

star_float_pair
star_ml_function_rnn_data_get_one_hot_255(const unsigned char *data, size_t *batch_offsets, int input_size,
                                          size_t data_size, int batch_size, int n_time_step);

star_float_pair star_ml_function_rnn_data_get_n_dim(
        const float *data, size_t *batch_offsets, int x_size,
        size_t n_data_x, int batch_size, int n_time_step);

star_float_pair star_ml_function_rnn_data_get_stock(
        float *data, size_t *batch_offsets, int x_size,
        size_t n_data_x, int batch_size, int n_time_step);

star_float_pair star_ml_function_rnn_data_get_sin(
        float *data, size_t *batch_offsets, int x_size, int y_size,
        size_t n_data_x, int batch_size, int n_time_step);

char **star_data_read_tokens(const char *filename, size_t *read);

mlfunction_net *star_ml_function_net_create_from_file(const char *function_file, const char *paras_file, int isClear);

void star_ml_function_net_save_function_paras(mlfunction_net *fnet, const char *filename, const char *type);

int sky_ml_function_paras_convert(int argc, const char **argv);

#ifdef __cplusplus
}
#endif
