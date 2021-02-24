#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {
    star_data_type_none,
    star_data_type_char_rnn,
    star_data_type_tokenized_rnn, // generalized char rnn
    star_data_type_stock_rnn,
    star_data_type_sin_rnn,
} star_data_type;

star_data_type star_data_type_string_to_type(const char *type);

void star_ml_function_rnn_print_symbol(int n, char **tokens);

int star_ml_function_rnn_sample_array(float *a, int n);

void star_ml_function_rnn_train(const char *datacfg, const char *cfgfile,
                                const char *weightfile, int clear, NumN logLevel);

void
star_ml_function_rnn_test_char_rnn(const char *cfgfile, const char *weightfile, int num, const char *seed, float temp,
                                   unsigned int rseed, const char *token_file, NumN logLevel);

int sky_ml_net_run_rnn(int argc, const char **argv);

#ifdef __cplusplus
}
#endif
