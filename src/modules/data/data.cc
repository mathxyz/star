#include "../util/files.h"
#include "data_c.h"

using namespace star;

m21list *star_data_get_lines_from_file(const char *filename) {
    char *path;
    FILE *file = fopen(filename, "r");
    if (!file) math21_file_error(filename);
    m21list *lines = math21_data_structure_list_create();
    while ((path = math21_file_get_line_c(file))) {
        math21_data_structure_list_insert(lines, path);
    }
    fclose(file);
    return lines;
}

char **star_data_get_labels(const char *filename) {
    m21list *plist = star_data_get_lines_from_file(filename);
    char **labels = (char **) math21_data_structure_list_to_array(plist);
    math21_data_structure_list_free(plist);
    return labels;
}

m21list *star_data_read_config(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == 0) math21_file_error(filename);
    char *line;
    int nu = 0;
    m21list *options = math21_data_structure_list_create();
    while ((line = math21_file_get_line_c(file)) != 0) {
        ++nu;
        math21_string_strip(line);
        switch (line[0]) {
            case '\0':
            case '#':
            case ';':
                free(line);
                break;
            default:
                if (!math21_function_option_read(line, options)) {
                    fprintf(stderr, "Config file error line %d, could parse: %s\n", nu, line);
                    free(line);
                }
                break;
        }
    }
    fclose(file);
    return options;
}

mllabels star_data_get_labels_from_data_config(const char *data_config) {
    mllabels m = {0};
    m21list *options = star_data_read_config(data_config);

    const char *name_list = math21_function_option_find_str(options, "names", 0);
    if (!name_list) name_list = math21_function_option_find_str(options, "labels", 0);
    if (!name_list) {
        fprintf(stderr, "No names or labels found\n");
    } else {
        m.names = star_data_get_labels(name_list);
    }
    m.n = math21_function_option_find_int(options, "classes", 2);
    math21_data_structure_list_free(options);
    return m;
}

const char *star_data_get_backup_dir(m21list *l, const char *def) {
    const char *backup_directory = math21_function_option_find_str(l, "backup", def);
    fprintf(stderr, "You should make sure %s exists!\n", backup_directory);
    return backup_directory;
}

size_t star_pr_rand_size_t() {
    return math21_pr_rand_size_t();
}

star_float_pair
star_ml_function_rnn_data_get_one_hot_n(const int *tokens, size_t *mb_offsets, int x_size, size_t n_data_x, int mbs,
                                        int n_time_step) {
    float *x = (float *) math21_vector_calloc_cpu(n_time_step * mbs * x_size, sizeof(float));
    float *y = (float *) math21_vector_calloc_cpu(n_time_step * mbs * x_size, sizeof(float));
    int i, j;
    for (j = 0; j < n_time_step; ++j) {
        for (i = 0; i < mbs; ++i) {
            int curr = tokens[(mb_offsets[i]) % n_data_x];
            int next = tokens[(mb_offsets[i] + 1) % n_data_x];

            x[(j * mbs + i) * x_size + curr] = 1;
            y[(j * mbs + i) * x_size + next] = 1;

            mb_offsets[i] = (mb_offsets[i] + 1) % n_data_x;

            if (curr >= x_size || curr < 0 || next >= x_size || next < 0) {
                math21_error("Bad char");
            }
        }
    }
    star_float_pair p;
    p.x = x;
    p.y = y;
    return p;
}

// x shape: n_time_step * mbs * x_size, x_size = 256, and x in x_size is one-hot.
star_float_pair star_ml_function_rnn_data_get_one_hot_255(const unsigned char *data, size_t *mb_offsets, int x_size,
                                                          size_t n_data_x, int mbs, int n_time_step) {
    float *x = (float *) math21_vector_calloc_cpu(n_time_step * mbs * x_size, sizeof(float));
    float *y = (float *) math21_vector_calloc_cpu(n_time_step * mbs * x_size, sizeof(float));

    int i, j;
    for (j = 0; j < n_time_step; ++j) {
        for (i = 0; i < mbs; ++i) {
            unsigned char curr = data[(mb_offsets[i]) % n_data_x];
            unsigned char next = data[(mb_offsets[i] + 1) % n_data_x];

            x[(j * mbs + i) * x_size + curr] = 1;
            y[(j * mbs + i) * x_size + next] = 1;

            mb_offsets[i] = (mb_offsets[i] + 1) % n_data_x;

            if (curr <= 0 || next <= 0) {
                math21_error("Bad char");
            }
        }
    }
    star_float_pair p;
    p.x = x;
    p.y = y;
    return p;
}

star_float_pair star_ml_function_rnn_data_get_n_dim(
        const float *data, size_t *mb_offsets, int x_size,
        size_t n_data_x, int mbs, int n_time_step) {
    auto *x = (float *) math21_vector_calloc_cpu(n_time_step * mbs * x_size, sizeof(float));
    auto *y = (float *) math21_vector_calloc_cpu(n_time_step * mbs * x_size, sizeof(float));
    int its, imb, ix;
    for (its = 0; its < n_time_step; ++its) {
        for (imb = 0; imb < mbs; ++imb) {
            size_t curr = (mb_offsets[imb]) % n_data_x;
            size_t next = (mb_offsets[imb] + 1) % n_data_x;
            for (ix = 0; ix < x_size; ++ix) {
                x[(its * mbs + imb) * x_size + ix] = data[curr * x_size + ix];
                y[(its * mbs + imb) * x_size + ix] = data[next * x_size + ix];
            }
            mb_offsets[imb] = (mb_offsets[imb] + 1) % n_data_x;
        }
    }
    star_float_pair p;
    p.x = x;
    p.y = y;
    return p;
}

star_float_pair star_ml_function_rnn_data_stock_get_n_dim(
        const float *data, size_t *mb_offsets, int x_size,
        size_t n_data_x, int mbs, int n_time_step) {
    auto *x = (float *) math21_vector_calloc_cpu(n_time_step * mbs * x_size, sizeof(float));
    auto *y = (float *) math21_vector_calloc_cpu(n_time_step * mbs * x_size, sizeof(float));
    int its, imb, ix;
    for (its = 0; its < n_time_step; ++its) {
        for (imb = 0; imb < mbs; ++imb) {
            size_t curr = (mb_offsets[imb]) % n_data_x;
            size_t next = (mb_offsets[imb] + 1) % n_data_x;
            for (ix = 0; ix < x_size; ++ix) {
                x[(its * mbs + imb) * x_size + ix] = data[curr * x_size + ix];
                y[(its * mbs + imb) * x_size + ix] = data[next * x_size + ix];
            }
            mb_offsets[imb] = (mb_offsets[imb] + 1) % n_data_x;
        }
    }
    star_float_pair p;
    p.x = x;
    p.y = y;
    return p;
}

// Normalise window with a base value of zero
void star_ml_function_rnn_data_stock_normalise_windows(
        float *x, float *y, int mbs, int n_time_step, int x_size) {
    int its, imb, ix;

    for (imb = 0; imb < mbs; ++imb) {
        for (its = n_time_step - 1; its >= 0; --its) {
            for (ix = 0; ix < x_size; ++ix) {
                int index0 = 0 * mbs * x_size + imb * x_size + ix;
                int index = its * mbs * x_size + imb * x_size + ix;
                x[index] = x[index] / x[index0] - 1;
                y[index] = y[index] / y[index0] - 1;
            }
        }
    }
}

star_float_pair star_ml_function_rnn_data_get_stock(
        float *data, size_t *mb_offsets, int x_size,
        size_t n_data_x, int mbs, int n_time_step) {
    star_float_pair p = star_ml_function_rnn_data_stock_get_n_dim(
            data, mb_offsets, x_size,
            n_data_x, mbs, n_time_step);
    star_ml_function_rnn_data_stock_normalise_windows(p.x, p.y, mbs, n_time_step, x_size);
    return p;
}

// x_size = y_size
star_float_pair star_ml_function_rnn_data_get_sin_01(
        float *data, size_t *mb_offsets, int x_size,
        size_t n_data_x, int mbs, int n_time_step) {
    star_float_pair p = star_ml_function_rnn_data_stock_get_n_dim(
            data, mb_offsets, x_size,
            n_data_x, mbs, n_time_step);
    return p;
}

// y has no time dimension.
// x_size != y_size
star_float_pair star_ml_function_rnn_data_get_sin(
        float *data, size_t *mb_offsets, int x_size, int y_size,
        size_t n_data_x, int mbs, int n_time_step) {
    auto *x = (float *) math21_vector_calloc_cpu(n_time_step * mbs * x_size, sizeof(float));
//    auto *y = (float *) math21_vector_calloc_cpu(n_time_step * mbs * y_size, sizeof(float));
    auto *y = (float *) math21_vector_calloc_cpu(mbs * y_size, sizeof(float));
    int its, imb, ix;
    size_t n_data_raw = n_data_x * x_size;
    for (imb = 0; imb < mbs; ++imb) {
        for (its = 0; its < n_time_step; ++its) {
            size_t curr = (mb_offsets[imb]) % n_data_x;
            for (ix = 0; ix < x_size; ++ix) {
                x[(its * mbs + imb) * x_size + ix] = data[curr * x_size + ix];
            }
            mb_offsets[imb] = (mb_offsets[imb] + 1) % n_data_x;
        }
        // for y
        size_t curr = (mb_offsets[imb]) % n_data_x;
        for (ix = 0; ix < y_size; ++ix) {
            y[imb * y_size + ix] = data[(curr * x_size + ix) % n_data_raw];
        }
    }
    star_float_pair p;
    p.x = x;
    p.y = y;
    return p;
}

char **star_data_read_tokens(const char *filename, size_t *read) {
    MATH21_ASSERT(0)
    return 0;

    /*size_t size = 512;
    size_t count = 0;
    FILE *fp = fopen(filename, "r");
    char **d = (char **) math21_vector_calloc_cpu(size, sizeof(char *));
    char *line;
    while ((line = math21_file_get_line_c(fp)) != 0) {
        ++count;
        if (count > size) {
            size = size * 2;
            d = (char **) math21_vector_realloc_cpu(d, size * sizeof(char *));
        }
        if (0 == strcmp(line, "<NEWLINE>")) line = "\n";
        d[count - 1] = line;
    }
    fclose(fp);
    d = (char **) math21_vector_realloc_cpu(d, count * sizeof(char *));
    *read = count;
    return d;*/
}

void star_ml_function_net_load_function_paras_from_config_upto_dk(mlfunction_net *fnet, const char *filename,
                                                                  int index_node_start,
                                                                  int index_node_cutoff) {
#ifndef MATH21_FLAG_USE_CPU
    if (fnet->gpuDevice >= 0) {
        math21_gpu_set_device_wrapper(fnet->gpuDevice);
    }
#endif
    fprintf(stdout, "Loading weights from %s...\n", filename);
    fflush(stdout);
    FILE *fp = fopen(filename, "rb");
    if (!fp) math21_file_error(filename);

    int major;
    int minor;
    int revision;
    fread(&major, sizeof(int), 1, fp);
    fread(&minor, sizeof(int), 1, fp);
    fread(&revision, sizeof(int), 1, fp);

    if ((major * 10 + minor) >= 2 && major < 1000 && minor < 1000) {
        size_t n_seen;
        fread(&n_seen, sizeof(size_t), 1, fp);
        fnet->n_seen = n_seen;
    } else {
        int n_seen = 0;
        fread(&n_seen, sizeof(int), 1, fp);
        fnet->n_seen = n_seen;
    }
#if 1
    m21log("major", major);
    m21log("minor", minor);
    m21log("revision", revision);
    m21log("fnet->n_seen", fnet->n_seen);
#endif
    int transpose = (major > 1000) || (minor > 1000);

    int i;
    for (i = index_node_start; i < fnet->n_node && i < index_node_cutoff; ++i) {
        mlfunction_node *fnode = fnet->nodes[i];
//        layer l = net->layers[i];
        if (fnode->dontload) continue;
        if (fnode->type == mlfnode_type_conv) {
            mlfunction_conv *f = (mlfunction_conv *) fnode->function;
            math21_ml_function_conv_load_theta(f, fp);
        } else if (fnode->type == mlfnode_type_fully_connected) {
            mlfunction_fully_connected *f = (mlfunction_fully_connected *) fnode->function;
            math21_ml_function_fully_connected_load_theta_order_bwsmv_flipped(f, fp, transpose);
        } else if (fnode->type == mlfnode_type_rnn) {
            mlfunction_rnn *f = (mlfunction_rnn *) fnode->function;
            math21_ml_function_fully_connected_load_theta_order_bwsmv_flipped(f->input_layer, fp, transpose);
            math21_ml_function_fully_connected_load_theta_order_bwsmv_flipped(f->self_layer, fp, transpose);
            math21_ml_function_fully_connected_load_theta_order_bwsmv_flipped(f->output_layer, fp, transpose);
        } else if (fnode->type == mlfnode_type_lstm) {
            mlfunction_lstm *f = (mlfunction_lstm *) fnode->function;
            math21_tool_assert(f->implementationMode==1);
            math21_ml_function_fully_connected_load_theta_order_bwsmv_flipped(f->fcUi, fp, transpose);
            math21_ml_function_fully_connected_load_theta_order_bwsmv_flipped(f->fcUf, fp, transpose);
            math21_ml_function_fully_connected_load_theta_order_bwsmv_flipped(f->fcUo, fp, transpose);
            math21_ml_function_fully_connected_load_theta_order_bwsmv_flipped(f->fcUg, fp, transpose);
            math21_ml_function_fully_connected_load_theta_order_bwsmv_flipped(f->fcWi, fp, transpose);
            math21_ml_function_fully_connected_load_theta_order_bwsmv_flipped(f->fcWf, fp, transpose);
            math21_ml_function_fully_connected_load_theta_order_bwsmv_flipped(f->fcWo, fp, transpose);
            math21_ml_function_fully_connected_load_theta_order_bwsmv_flipped(f->fcWg, fp, transpose);
        } else if (fnode->type == mlfnode_type_batchnorm) {
            math21_tool_assert(0);
            mlfunction_batchnorm *f = (mlfunction_batchnorm *) fnode->function;
            math21_ml_function_batchnorm_load_theta(f, fp, 1);
        }
    }
    fprintf(stderr, "Done!\n");
    fclose(fp);
}

void star_ml_function_net_load_function_paras_from_config(mlfunction_net *fnet, const char *filename) {
    if (math21_ml_is_function_paras_file(filename)) {
        math21_ml_function_net_load_function_paras_from_config(fnet, filename);
    } else {
        star_ml_function_net_load_function_paras_from_config_upto_dk(fnet, filename, 0, fnet->n_node);
    }
}

mlfunction_net *star_ml_function_net_create_from_file(const char *function_file, const char *paras_file, int isClear) {
    mlfunction_net *fnet = math21_ml_function_net_load_function_form_from_config(function_file);
    if (paras_file && paras_file[0] != 0) {
        star_ml_function_net_load_function_paras_from_config(fnet, paras_file);
    }
    if (isClear) fnet->n_seen = 0;
    return fnet;
}

void
star_ml_function_net_save_function_paras_upto_dk(mlfunction_net *fnet, const char *filename, int index_node_cutoff) {
#ifndef MATH21_FLAG_USE_CPU
    if (fnet->gpuDevice >= 0) {
        math21_gpu_set_device_wrapper(fnet->gpuDevice);
    }
#endif
    fprintf(stdout, "Saving weights to %s\n", filename);
    FILE *fp = fopen(filename, "wb");
    if (!fp) math21_file_error(filename);

    int major = 0;
    int minor = 2;
    int revision = 0;
    fwrite(&major, sizeof(int), 1, fp);
    fwrite(&minor, sizeof(int), 1, fp);
    fwrite(&revision, sizeof(int), 1, fp);
    size_t n_seen = fnet->n_seen;
    fwrite(&n_seen, sizeof(size_t), 1, fp);

    int i;
    for (i = 0; i < fnet->n_node && i < index_node_cutoff; ++i) {
        mlfunction_node *fnode = fnet->nodes[i];
        if (fnode->dontsave) continue;
        if (fnode->type == mlfnode_type_conv) {
            mlfunction_conv *f = (mlfunction_conv *) fnode->function;
            math21_ml_function_conv_save_theta(f, fp);
        } else if (fnode->type == mlfnode_type_fully_connected) {
            mlfunction_fully_connected *f = (mlfunction_fully_connected *) fnode->function;
            math21_ml_function_fully_connected_save_theta_order_bwsmv(f, fp);
        } else if (fnode->type == mlfnode_type_rnn) {
            mlfunction_rnn *f = (mlfunction_rnn *) fnode->function;
            math21_ml_function_fully_connected_save_theta_order_bwsmv(f->input_layer, fp);
            math21_ml_function_fully_connected_save_theta_order_bwsmv(f->self_layer, fp);
            math21_ml_function_fully_connected_save_theta_order_bwsmv(f->output_layer, fp);
        } else if (fnode->type == mlfnode_type_lstm) {
            mlfunction_lstm *f = (mlfunction_lstm *) fnode->function;
            math21_tool_assert(f->implementationMode==1);
            math21_ml_function_fully_connected_save_theta_order_bwsmv(f->fcUi, fp);
            math21_ml_function_fully_connected_save_theta_order_bwsmv(f->fcUf, fp);
            math21_ml_function_fully_connected_save_theta_order_bwsmv(f->fcUo, fp);
            math21_ml_function_fully_connected_save_theta_order_bwsmv(f->fcUg, fp);
            math21_ml_function_fully_connected_save_theta_order_bwsmv(f->fcWi, fp);
            math21_ml_function_fully_connected_save_theta_order_bwsmv(f->fcWf, fp);
            math21_ml_function_fully_connected_save_theta_order_bwsmv(f->fcWo, fp);
            math21_ml_function_fully_connected_save_theta_order_bwsmv(f->fcWg, fp);
        } else if (fnode->type == mlfnode_type_batchnorm) {
            math21_tool_assert(0 && "not used!");
            mlfunction_batchnorm *f = (mlfunction_batchnorm *) fnode->function;
            math21_ml_function_batchnorm_save_theta(f, fp, 1);
        }
    }
    fclose(fp);
}

// save theta of the function f(x, theta)
void star_ml_function_net_save_function_paras(mlfunction_net *fnet, const char *filename, const char *type) {
    if (!type) {
        math21_ml_function_net_save_function_paras(fnet, filename);
    } else if (math21_string_is_equal(type, "dk")) {
        star_ml_function_net_save_function_paras_upto_dk(fnet, filename, fnet->n_node);
    }
}

void star_ml_function_paras_convert(const char *function_form, const char *function_paras, const char *dst_paras,
                                    const char *type) {

    std::string baseName = star_string_get_file_base_name(function_form);
    fprintf(stdout, "converting %s paras\n", baseName.c_str());
    mlfunction_net *fnet = star_ml_function_net_create_from_file(function_form, function_paras, 0);

    star_ml_function_net_save_function_paras(fnet, dst_paras, type);
}

int sky_ml_function_paras_convert(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            m21log("\tsky_ml_function_paras_convert \n"
                   "\tflags:\n"
                   "\t  -function_form\n"
                   "\t  -function_paras\n"
                   "\t  -dst_paras\n"
                   "\t  -type\n"
            );
            m21log("\te.x., \n"
                   "\tsky_ml_net_run_rnn train -data rnn.data -function_form lstm.train.cfg\n");
            return 1;
        }
    }

    if (argc < 2) {
        return 0;
    }
    const char *function_form = star_string_args_find_c_string(argc, argv, "function_form", 0);
    const char *function_paras = star_string_args_find_c_string(argc, argv, "function_paras", 0);
    const char *dst_paras = star_string_args_find_c_string(argc, argv, "dst_paras", 0);
    const char *type = star_string_args_find_c_string(argc, argv, "type", 0);
    star_ml_function_paras_convert(function_form, function_paras, dst_paras, type);
    return 1;
}