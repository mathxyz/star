#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "inner.h"
#include "test.h"

using namespace star;


/*
 * LeNet-5
 * input:  1*28*28
 * C1:     6*28*28      6*1*5*5     ms=ns=1,    mp=np=2
 * S2:     6*14*14          2*2     ms=ns=2
 * C3:     16*10*10     16*6*5*5    ms=ns=1
 * S4:     16*5*5           2*2     ms=ns=2
 * C5:     120*1*1      120*16*5*5  ms=ns=1
 * F6:     84*1*1       84*1*1*120*1*1
 * F7:     10*1*1       10*1*1*84*1*1
 * */
void sample_cnn_mnist(const std::string &data_dir) {
    m21log(__FUNCTION__);
    std::string home_path = STAR_HOME_PATH;
    // load MNIST dataset
    Seqce<TenR> train_labels, test_labels;
    Seqce<TenR> train_images, test_images;

//    std::string train_labels_path = data_dir + "/train-labels.idx1-ubyte";
//    std::string train_images_path = data_dir + "/train-images.idx3-ubyte";
//    std::string test_labels_path = data_dir + "/t10k-labels.idx1-ubyte";
//    std::string test_images_path = data_dir + "/t10k-images.idx3-ubyte";

    const std::string file_dir = home_path + "/assets/data/mnist/images_a_few";
    const std::string data_path_prefix = home_path + "/assets/data/mnist/images_a_few/";
    std::string train_images_path = file_dir + "/train_images.txt";
    std::string test_images_path = file_dir + "/test_images.txt";
    std::string train_labels_path = file_dir + "/train_labels.txt";
    std::string test_labels_path = file_dir + "/test_labels.txt";

//    NumN train_max_size = 0;
//    NumN train_max_size = 1000;
//    NumN train_max_size = 100;
    NumN train_max_size = 20;
//    NumN test_max_size = 0;
    NumN test_max_size = 20;
//    NumN test_max_size = 100;

    // parse from mnist format files
//    ml_data_mnist_parse_labels(train_labels_path, train_labels, train_max_size);
//    ml_data_mnist_parse_images(train_images_path, train_images, train_max_size, 0, 0);
//    ml_data_mnist_parse_labels(test_labels_path, test_labels, test_max_size);
//    ml_data_mnist_parse_images(test_images_path, test_images, test_max_size, 0, 0);

    // read from images
    ml_data_read_from_img(train_images, train_images_path.c_str(), train_max_size, data_path_prefix.c_str());
    ml_data_read_from_img(test_images, test_images_path.c_str(), test_max_size, data_path_prefix.c_str());
    ml_data_parse_as_vector(train_labels, train_labels_path.c_str(), 10, 32);
    ml_data_parse_as_vector(test_labels, test_labels_path.c_str(), 10, 32);

    // save data
//    ml_data_write_to_png(train_images, "train_images.txt", "train_images");
//    ml_data_write_to_png(test_images, "test_images.txt", "test_images");
//    ml_data_save_as_vector(train_labels, "train_labels.txt", 10, 32);
//    ml_data_save_as_vector(test_labels, "test_labels.txt", 10, 32);


//    exit(0);

//    VecR tmp;
//    tmp = train_images(1);
    data_scale(train_images, 0, 255, -1, 1);
    data_scale(test_images, 0, 255, -1, 1);

//    data_scale(train_images, -1, 1, 0, 255);
//    data_scale(test_images, -1, 1, 0, 255);

//    VecR tmp2;
//    tmp2 = train_images(1);
//    VecR tmp3;
//    tmp3 = tmp-tmp2;
//    m21log("norm", tmp3.norm(1));

//    train_labels.log("label");
//    train_images.log("img");

    // initialize data: convert original data to required form.
    ml_data_mnist_image_2d_to_3d(train_images);
    ml_data_mnist_image_2d_to_3d(test_images);
    ml_data_mnist_label_0d_to_3d(train_labels);
    ml_data_mnist_label_0d_to_3d(test_labels);
    //
    std::cout << "start learning" << std::endl;
    std::cout << "training set size " << train_images.size() << std::endl;
    std::cout << "test set size " << test_images.size() << std::endl;

    //////////////////
    cnn f;
    VecR theta;
    //////////////////
    const char *model_file_name = "model_cnn_mnist3.bin";
    std::ifstream in;
    in.open(model_file_name, std::ifstream::binary);
    if (in.is_open()) {
        m21log("deserialize cnn");
        math21_deserialize_model(in, f, theta);
        in.close();
    }

    ////////////////// create cnn
    if (f.isEmpty()) {
        m21log("create cnn");
//        Seqce<cnn_config_fn *> config_fns(3);

        /*
 * LeNet-5
 * input:  1*28*28
 * C1:     6*28*28      6*1*5*5     ms=ns=1,    mp=np=2
 * S2:     6*14*14          2*2     ms=ns=2
 * C3:     16*10*10     16*6*5*5    ms=ns=1
 * S4:     16*5*5           2*2     ms=ns=2
 * C5:     120*1*1      120*16*5*5  ms=ns=1
 * F6:     84*1*1       84*1*1*120*1*1
 * F7:     10*1*1       10*1*1*84*1*1
 * */

        Seqce<cnn_config_fn *> config_fns(7);
//        NumN cnn_type_hn = cnn_type_hn_ReLU;
        NumN cnn_type_hn = cnn_type_hn_tanh;
        NumN cnn_type_pooling = cnn_type_pooling_max;
        VecN d(3);
        d = 6, 28, 28;
        config_fns(1) = new cnn_config_fn_conv(d, cnn_type_hn, 5, 5, 1, 1, 1, 1);
        d = 6, 14, 14;
        config_fns(2) = new cnn_config_fn_pooling(d, cnn_type_pooling);
        d = 16, 10, 10;
        config_fns(3) = new cnn_config_fn_conv(d, cnn_type_hn, 5, 5, 1, 1, 1, 1);
        d = 16, 5, 5;
        config_fns(4) = new cnn_config_fn_pooling(d, cnn_type_pooling);
        d = 120, 1, 1;
        config_fns(5) = new cnn_config_fn_conv(d, cnn_type_hn, 5, 5, 1, 1, 1, 1);
        d = 84, 1, 1;
        config_fns(6) = new cnn_config_fn_fully(d, cnn_type_hn);
        train_labels(1).shape(d);
        config_fns(7) = new cnn_config_fn_fully(d, cnn_type_hn_linear);

        NumB isUsingDiff = 1;
        f.setSize(train_images(1).shape(d), config_fns, isUsingDiff);

        for (NumN i = 1; i <= config_fns.size(); i++) {
            delete config_fns(i);
        }
    }

    ////////////////////
    CostFunctional_nll_CrossEntroy_softmax_class L;
    cnn_cost_class J(f, L, train_images, train_labels, 30, 64);
//    cnn_cost_class J(f, L, train_images, train_labels, 30, 4);
    OptimizationInterface_cnn oi;
    oi.setName(model_file_name);
//    sd_update_rule_normal update_rule(J);
//    update_rule.tao = 80000;
//    sd_update_rule_momentum update_rule(J);
//    sd_update_rule_Nesterove_momentum update_rule(J);
//    sd_update_rule_AdaGrad update_rule(J);
//    sd_update_rule_RMSProp update_rule(J);
    sd_update_rule_RMSProp_Nesterov_momentum update_rule(J);
//    sd_update_rule_Adam update_rule(J);
    if (!theta.isEmpty()) {
        update_rule.setInit(theta);
    }
    SteepestDescent opt(update_rule, oi);
//    update_rule.time_max = 100000;
    update_rule.time_max = 2;

//    update_rule.x = a.getTheta();
    opt.solve();
//    ((VecR &) opt.getMinimum()).log("minima");
    f.setTheta(opt.getMinimum());
//    evaluate_cnn(f, J, train_images, train_labels, 1);
    evaluate_cnn_error_rate(f, J, train_images, train_labels, 1);
    evaluate_cnn_error_rate(f, J, test_images, test_labels, 1);

}
