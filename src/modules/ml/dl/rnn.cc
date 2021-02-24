#include "../../data/files.h"
#include "../../util/files.h"
#include "inner.h"

#include "rnn.h"

using namespace star;

void star_ml_function_rnn_print_symbol(int n, char **tokens) {
    if (tokens) {
        printf("%s ", tokens[n]);
    } else {
        printf("%c", n);
//        printf("%d\n", n);
    }
    math21_log_flush_stdout();
}

int star_ml_function_rnn_sample_array(float *a, int n) {
    float sum = math21_vector_sum_cpu(a, n);
    math21_vector_kx_cpu(n, 1.0f / sum, a, 1);
    float r = math21_pr_rand_uniform(0, 1);
    int i;
    for (i = 0; i < n; ++i) {
        r = r - a[i];
        if (r <= 0) return i;
    }
    return n - 1;
}

star_data_type star_data_type_string_to_type(const char *type) {
    if (!type) {
        return star_data_type_none;
    }
    if (strcmp(type, "star_data_type_char_rnn") == 0) return star_data_type_char_rnn;
    if (strcmp(type, "star_data_type_tokenized_rnn") == 0) return star_data_type_tokenized_rnn;
    if (strcmp(type, "star_data_type_stock_rnn") == 0) return star_data_type_stock_rnn;
    if (strcmp(type, "star_data_type_sin_rnn") == 0) return star_data_type_sin_rnn;
    return star_data_type_none;
}

void star_ml_function_rnn_train(const char *datacfg, const char *cfgfile,
                                const char *weightfile, int clear, NumN logLevel) {

    MatR B;
    math21_io_generic_type_read_from_file(B, "/mathxyz/y/lstm-sin/sin_data.bin",
                                          0, 1);
    B.log("B");

//    exit(0);
    m21list *options = star_data_read_config(datacfg);
    const char *filename = math21_function_option_find_str(options, "file", "data/shakespeare.txt");
    const char *data_type_s = math21_function_option_find_str(options, "star_data_type", 0);
    star_data_type data_type = star_data_type_string_to_type(data_type_s);

    char backup_dir_default[256];
    sprintf(backup_dir_default, "%s/%s", ".", "backup");
    const char *backup_directory = star_data_get_backup_dir(options, backup_dir_default);

    math21_c_seed_random_generator_by_current_time();
    unsigned char *text = 0;
    int *tokens = 0;
    float *data = 0;
    size_t n_data_1d;
    size_t n_data_x;
    if (data_type == star_data_type_tokenized_rnn) {
        // NumZ data
        tokens = math21_vector_read_from_file_int(filename, &n_data_1d);
    } else if (data_type == star_data_type_stock_rnn) {
        data = math21_vector_read_from_file_float(filename, &n_data_1d);
    } else if (data_type == star_data_type_sin_rnn) {
        data = math21_vector_read_from_file_float(filename, &n_data_1d);
    } else {
        text = math21_string_read_file(filename);
        n_data_1d = strlen((const char *) text);
    }

    std::string baseName = star_string_get_file_base_name(cfgfile);
    fprintf(stdout, "%s\n", baseName.c_str());
    float avg_loss = -1;
    mlfunction_net *fnet = star_ml_function_net_create_from_file(cfgfile, weightfile, clear);

    math21_ml_function_net_log_opt_paras(fnet);

    fprintf(stdout, "input size: %d, mini-batch size: %d, n_time_step: %d\n",
            fnet->data_x_size, fnet->mini_batch_size, fnet->n_time_step_in_rnn);
    if (clear) fnet->n_seen = 0;

    if (data_type == star_data_type_stock_rnn || data_type == star_data_type_sin_rnn) {
        math21_tool_assert(n_data_1d % fnet->data_x_size == 0);
        n_data_x = n_data_1d / fnet->data_x_size;
        fprintf(stdout, "n_data_x: " SIZE_T_SPECIFIER "\n", n_data_x);
    } else {
        n_data_x = n_data_1d;
    }

    int rnn_batch_size = fnet->mini_batch_size / fnet->n_time_step_in_rnn;
    auto *rnn_batch_offsets = (size_t *) math21_vector_calloc_cpu(rnn_batch_size, sizeof(size_t));
    int j;
    for (j = 0; j < rnn_batch_size; ++j) {
        // rnn_batch_offsets[j] = star_pr_rand_size_t(); // todo: use this, because '%' is needless.
        rnn_batch_offsets[j] = star_pr_rand_size_t() % n_data_x;
    }

    timer time;
    int i=0;
    while (math21_ml_function_net_should_train_continue(fnet)) {
        time.start();
        i += 1;
        star_float_pair p;
        if (data_type == star_data_type_tokenized_rnn) {
            p = star_ml_function_rnn_data_get_one_hot_n(tokens, rnn_batch_offsets, fnet->data_x_size, n_data_x,
                                                        rnn_batch_size, fnet->n_time_step_in_rnn);
        } else if (data_type == star_data_type_stock_rnn) {
            p = star_ml_function_rnn_data_get_stock(data, rnn_batch_offsets, fnet->data_x_size, n_data_x,
                                                    rnn_batch_size, fnet->n_time_step_in_rnn);
        } else if (data_type == star_data_type_sin_rnn) {
            p = star_ml_function_rnn_data_get_sin(data, rnn_batch_offsets, fnet->data_x_size, fnet->data_y_size,
                                                  n_data_x,
                                                  rnn_batch_size, fnet->n_time_step_in_rnn);
        } else {
            p = star_ml_function_rnn_data_get_one_hot_255(text, rnn_batch_offsets, fnet->data_x_size, n_data_x,
                                                          rnn_batch_size, fnet->n_time_step_in_rnn);
        }

        math21_ml_function_net_data_feed(fnet, p.x, p.y);

        // todo: check loss = loss / rnn_batch_size or /batch_size
        float loss = math21_ml_function_net_train_one_mini_batch_in_function(fnet) / (fnet->mini_batch_size);
        free(p.x);
        free(p.y);
        if (avg_loss < 0) avg_loss = loss;
//        avg_loss = avg_loss * .9f + loss * .1f;
        avg_loss = avg_loss * .9 + loss * .1;

        time.end();
        fprintf(stdout, "%d: %f, avg %f, alpha %f, epochs %f\n", i, loss, avg_loss,
                math21_ml_function_net_opt_get_alpha(fnet), (float) fnet->n_seen / n_data_x);

        for (j = 0; j < rnn_batch_size; ++j) {
            if (rand() % 64 == 0) {
                //fprintf(stdout, "Reset\n");
                rnn_batch_offsets[j] = star_pr_rand_size_t() % n_data_x;
                math21_ml_function_net_reset_rnn_state_when_gpu(fnet, j);
            }
        }

        if (i % 10000 == 0 || (i < 1000 && i % 100 == 0) || (i < 5000 && i % 1000 == 0)) {
            char buff[256];
            sprintf(buff, "%s/%s_" NumSize_SPECIFIER ".theta", backup_directory, baseName.c_str(), math21_ml_function_net_get_update_count(fnet));
            math21_ml_function_net_save_function_paras(fnet, buff);
        }
        if (i % 100 == 0) {
            char buff[256];
            sprintf(buff, "%s/%s.theta.backup", backup_directory, baseName.c_str());
            math21_ml_function_net_save_function_paras(fnet, buff);
        }
    }
    char buff[256];
    sprintf(buff, "%s/%s_final.theta", backup_directory, baseName.c_str());
    math21_ml_function_net_save_function_paras(fnet, buff);

    // free
    math21_vector_free_cpu(text);
    math21_vector_free_cpu(tokens);
    math21_vector_free_cpu(data);
}

// todo: check malloc count and free count
void
star_ml_function_rnn_test_char_rnn(const char *cfgfile, const char *weightfile, int num, const char *seed, float temp,
                                   unsigned int rseed, const char *token_file, NumN logLevel) {
    char **tokens = 0;
    if (token_file) {
        size_t n;
        tokens = star_data_read_tokens(token_file, &n);
    }

    math21_c_seed_random_generator(rseed);
    std::string baseName = star_string_get_file_base_name(cfgfile);
    fprintf(stdout, "%s\n", baseName.c_str());

    mlfunction_net *fnet = star_ml_function_net_create_from_file(cfgfile, weightfile, 0);

    int inputs = fnet->data_x_size;

    int i, j;
    math21_ml_function_softmax_net_set_temperature(fnet, temp);
    int c = 0;
    int len = strlen(seed);
    float *input = (float *) math21_vector_calloc_cpu(inputs, sizeof(float));

    /*
       math21_vector_set_cpu(inputs, 0, input, 1);
       for(i = 0; i < 10; ++i){
       math21_ml_function_net_predict_input(fnet, input);
       }
       math21_vector_set_cpu(inputs, 0, input, 1);
     */

    for (i = 0; i < len - 1; ++i) {
        c = seed[i];
        input[c] = 1;
        math21_ml_function_net_predict_input(fnet, input);
        input[c] = 0;
        star_ml_function_rnn_print_symbol(c, tokens);
    }
    if (len) c = seed[len - 1];
    star_ml_function_rnn_print_symbol(c, tokens);
    for (i = 0; i < num; ++i) {
        input[c] = 1;
        float *out = math21_ml_function_net_predict_input(fnet, input);
        input[c] = 0;
        for (j = 32; j < 127; ++j) {
            //printf("%d %c %f\n",j, j, out[j]);
        }
        for (j = 0; j < inputs; ++j) {
            if (out[j] < .0001) out[j] = 0;
        }
        c = star_ml_function_rnn_sample_array(out, inputs);
        star_ml_function_rnn_print_symbol(c, tokens);
    }
    printf("\n");
}

int sky_ml_net_run_rnn(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            m21log("\tsky_ml_net_run_rnn \n"
                   "\tdir [flags]\n"
                   "\tflags:\n"
                   "\t  -data\n"
                   "\t      The default value is ''.\n"
                   "\t  -function_form\n"
                   "\t  -function_paras\n"
                   "\t      The default value is ''.\n"
                   "\t  -logLevel\n"
                   "\t      Print files.\n"
            );
            m21log("\te.x., \n"
                   "\tsky_ml_net_run_rnn train -data rnn.data -function_form lstm.train.cfg\n");
            return 1;
        }
    }

//    m21log(math21_test_c_ad_sin3(3));
//    m21log(math21_test_c_ad_sin3(3));
//    m21log(math21_test_c_ad_sin3(3));
//    m21log(math21_test_c_ad_sin3(3));
//    m21log(math21_test_c_ad_sin3(3));

    if (argc < 2) {
        return 0;
    }
    int clear = 0;
    NumN len = star_string_args_find_NumN(argc, argv, "-len", 1000);
    // wild pointer?
    const char *seed = star_string_args_find_c_string(argc, argv, "-seed", "");
    NumR temp = star_string_args_find_NumR(argc, argv, "-temp", 0.7);
    NumN rseed = star_string_args_find_NumN(argc, argv, "-srand", math21_c_tim(0));
    NumN logLevel = star_string_args_find_NumN(argc, argv, "-logLevel", 0);
    const char *tokens = star_string_args_find_c_string(argc, argv, "-tokens", 0);
    const char *function_form = star_string_args_find_c_string(argc, argv, "-function_form", 0);
    const char *function_paras = star_string_args_find_c_string(argc, argv, "-function_paras", 0);
    const char *data = star_string_args_find_c_string(argc, argv, "-data", 0);

    math21_gpu_set_global_varible_wrapper(0);
    math21_gpu_set_device_by_global_variable_wrapper();

    if (0 == strcmp(argv[1], "train")) {
        star_ml_function_rnn_train(data, function_form, function_paras, clear, logLevel);
    } else if (0 == strcmp(argv[1], "generate")) {
        star_ml_function_rnn_test_char_rnn(function_form, function_paras, len, seed, temp, rseed, tokens, logLevel);
    } else {
        printf("not support rnn flags\n");
    }
    return 1;
}
