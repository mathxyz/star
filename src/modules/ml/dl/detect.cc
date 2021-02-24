#include "../../data/files.h"
#include "../../util/files.h"
#include "inner.h"

#include "detect.h"

using namespace star;

void
star_ml_detect_image_demo(const char *cfgfile, const char *weightfile, const char *datafile, const char *filePath) {
    m21list *options = star_data_read_config(datafile);
    const char *dst_dir = math21_function_option_find_str(options, "dst_dir", 0);

    mlfunction_net *fnet = star_ml_function_net_create_from_file(cfgfile, weightfile, 0);
    math21_c_seed_random_generator(212121);

    timer time;
    time.start();
    m21image img = star_image_read_image_normalized_with_rc(filePath, 0, 0);
    math21_ml_function_net_predict_image(fnet, img);

    float thresh = .5;
    float nms = .45;
    int ndetections = 0;
    mldetection *detections = math21_ml_function_net_boxes_get(fnet, img.nr, img.nc, thresh, 1, &ndetections);
    if (nms) {
        math21_ml_box_do_nms_obj(detections, ndetections, nms);
    }
    math21_ml_box_draw_detections(img, detections, ndetections, thresh);
    char name[256];
    NumN buffSize = sizeof(name);
    math21_string_get_file_name(filePath, name, buffSize);
    if (!dst_dir) {
        dst_dir = ".";
    }
    std::string savePath = math21_string_to_string(dst_dir) + "/" + math21_string_to_string(name);
    star_image_write_image_normalized(img, savePath.c_str());

    // log
    int nresults = 10;
    mlobject results[10] = {0};
    math21_ml_box_detection_to_results(ndetections, detections, nresults, results);
    math21_ml_box_log_object(results, nresults);

    math21_image_destroy_image(&img);
    math21_ml_function_net_boxes_destroy(detections, ndetections);

    time.end();
    printf(
            "src %s\n"
            "dst %s\n"
            "Predicted in %f seconds.\n", filePath, savePath.c_str(), time.time() / 1000);
}

int sky_ml_detect(int argc, const char **argv) {
    if (argc == 2) {
        if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
            m21log("\tsky_ml_detect \n"
                   "\t[train/test] [flags]\n"
                   "\tflags:\n"
                   "\t  -data\n"
                   "\t  -function_form\n"
                   "\t  -function_paras\n"
                   "\t  -image\n"
                   "\t  -dst_path\n"
                   "\t  -logLevel\n"
            );
            return 1;
        }
    }

    if (argc < 2) {
        return 0;
    }
    int clear = 0;
    NumN logLevel = star_string_args_find_NumN(argc, argv, "-logLevel", 0);
    const char *image = star_string_args_find_c_string(argc, argv, "image", 0);
    const char *function_form = star_string_args_find_c_string(argc, argv, "function_form", 0);
    const char *function_paras = star_string_args_find_c_string(argc, argv, "function_paras", 0);
    const char *data = star_string_args_find_c_string(argc, argv, "data", 0);

    math21_gpu_set_global_varible_wrapper(0);
    math21_gpu_set_device_by_global_variable_wrapper();

    if (0 == strcmp(argv[1], "train")) {
    } else if (0 == strcmp(argv[1], "test")) {
        star_ml_detect_image_demo(function_form, function_paras, data, image);
    } else {
        printf("not support rnn flags\n");
    }
    return 1;
}
