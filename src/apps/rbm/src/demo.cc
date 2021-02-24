//#include "mnist.h"
#include "dbn.h"

/** Algorithm from Earl F. Glynn's web page:
* <a href="http://www.efg2.com/Lab/ScienceAndEngineering/Spectra.htm">Spectra Lab Report</a>
* */

//#define RBM_SAVE_IMAGE 0
#define RBM_SAVE_IMAGE 1

#if RBM_SAVE_IMAGE

#include "spectrum.inl"

#endif

#include <algorithm>
#include <functional>
#include "inner.h"

// MacOS X
// clang++ -g -O2 -std=c++11 -stdlib=libc++ -I/usr/local/include/GraphicsMagick -o dbn demo.cc /Users/jack/Downloads/GraphicsMagick-1.3.18/Magick++/lib/.libs/libGraphicsMagick++.a  /Users/jack/Downloads/GraphicsMagick-1.3.18/magick/.libs/libGraphicsMagick.a -lz -lxml2 -lpng -ljpeg -lbz2
// Ubuntu
// clang++ --std=c++0x -o dbn -g -O2 demo.cc -lz -I /usr/include/GraphicsMagick -lGraphicsMagick++

using namespace star;

int sample_rbm() {

    std::string home_path = STAR_HOME_PATH;
    const std::string file_dir = home_path + "/assets/data/mnist/images_a_few";
    const std::string data_path_prefix = home_path + "/assets/data/mnist/images_a_few/";
//    const std::string file_dir = home_path + "/assets/data/mnist/images";
//    const std::string data_path_prefix = home_path + "/assets/data/mnist/images/";
    std::string train_images_path = file_dir + "/train_images.txt";
    std::string test_images_path = file_dir + "/test_images.txt";
    std::string train_labels_path = file_dir + "/train_labels.txt";
    std::string test_labels_path = file_dir + "/test_labels.txt";

    Seqce<TenR> labels;
    Seqce<TenR> images;


    const std::string images_path = test_images_path;
    const std::string labels_path = test_labels_path;

//    std::string command = "train";
//    std::string command = "test";
    std::string command = "train-simple";
//    std::string command = "test-simple";
//    std::string command = "train-encoder";


//    std::vector<Sample> samples;
//    int n = mnist::read(images, labels, samples);
//    if (n <= 0) {
//        std::cerr << "failed to read mnist data files: " << images << " ," << labels << std::endl;
//        return 0;
//    }

    // read from images
    star::ml_data_read_from_img(images, images_path.c_str(), 0, data_path_prefix.c_str());

    star::ml_data_parse_as_vector(labels, labels_path.c_str(), 10, 32);
    if (images.size() != labels.size()) {
        std::cerr << "counts don't match" << std::endl;
    }
    int n = (int) images.size();
    if (n <= 0) {
        std::cerr << "failed to read mnist data files: " << images_path << " ," << labels_path << std::endl;
        return 0;
    }

    // initialize data: convert original data to required form.
//    int data_size = samples[0].data_.size();
    int data_size = (int) images(1).volume();
    std::vector<Vector> inputs(n);
    std::vector<Vector> targets(n);
    for (size_t i = 0; i < n; ++i) {
//        const Sample &sample = samples[i];
        const TenR &image = images.at(i + 1);
        const TenR &label = labels.at(i + 1);
        Vector &input = inputs[i];
        Vector &target = targets[i];

        input.resize(data_size);
        target.resize(10);
//        for (size_t j = 0; j < data_size; ++j) input[j] = sample.data_[j] / 255.0; // > 30 ? 1.0: 0.0; // binary
        for (size_t j = 0; j < data_size; ++j)
            input[j] = (float) (image(j + 1) / 255.0); // > 30 ? 1.0: 0.0; // binary
        target[(int) label(1)] = 1.0;
    }

#if RBM_SAVE_IMAGE

    // progress monitoring
    auto progress = [&](DeepBeliefNet &dbn) {
        static int i = 0;
        std::string name = "rbm-" + math21_string_to_string(i++);
        int width = 0, height = 0;
        Vector pixels;
        dbn.to_image(pixels, width, height);

        TenR image(3, (NumN) height * 2, (NumN) width * 2);
        VecN color(3);
        color = 255, 255, 255;
        star::img_set_color(image, color);

        for (NumN i2 = 1; i2 <= image.dim(2); ++i2) {
            for (NumN i3 = 1; i3 <= image.dim(3); ++i3) {
                int j = int(fabs(pixels[int((i2 - 1) / 2 * width + (i3 - 1) / 2)] * 255));
                if (j > 255 || j < 0) j = 255;
                image(1, i2, i3) = _pallet[j].r;
                image(2, i2, i3) = _pallet[j].g;
                image(3, i2, i3) = _pallet[j].b;
            }
        }
        std::string fn = name + ".png";
//        img.write(fn.c_str());
        star::img_write(image, fn.c_str());

        std::ofstream f(name + ".dat", std::ofstream::binary);
        dbn.store(f);
    };
#else
    std::function<void(DeepBeliefNet &)> progress = nullptr;
#endif

    // training and testing functions
    auto train_dbn_simple = [&]() {
        DeepBeliefNet dbn;
        dbn.build(std::vector<int>{data_size, 300, 300, 500}, std::vector<int>{0, 0, 10});

        LRBM::Conf conf;
        conf.max_epoch_ = 6;
        conf.max_batches_ = 100;
        conf.batch_size_ = 100;

        dbn.train(inputs, targets, dbn.max_layer(), conf, progress);

        std::ofstream f("dbn-s.dat", std::ofstream::binary);
        dbn.store(f);
    };

    auto train_dbn = [&]() {
        DeepBeliefNet dbn;

        dbn.build(std::vector<int>{data_size, 300, 300, 500, 10});
        auto &rbm = dbn.output_layer();
        rbm->type_ = RBM::Type::EXP;

        LRBM::Conf conf;

        bool resume = false;
//        bool resume = true;
        if (resume) {
            std::ifstream f("dbn.dat", std::ifstream::binary);
            dbn.load(f);
            conf.max_epoch_ = 2;
            conf.max_batches_ = 300;
            conf.batch_size_ = 200;
        } else {
            conf.max_epoch_ = 10;
            conf.max_batches_ = 300;
            conf.batch_size_ = 200;
            dbn.pretrain(inputs, conf, progress);
        }

        conf.max_epoch_ = 10;
        conf.max_batches_ /= 5;
        conf.batch_size_ *= 5;
        dbn.backprop(inputs, targets, conf, progress);

        std::ofstream f("dbn.dat", std::ofstream::binary);
        dbn.store(f);
    };

    auto train_autoencoder = [&]() {
        AutoEncoder enc;
        enc.build(std::vector<int>{data_size, 500, 30, 500, data_size});

        auto &rbm = enc.rbms_[enc.max_layer() / 2 - 1];
        rbm->type_ = RBM::Type::LINEAR;

        LRBM::Conf conf;
        conf.max_epoch_ = 10;
        conf.max_batches_ = 50;
        conf.batch_size_ = 100;
        enc.pretrain(inputs, conf, progress);

        conf.max_epoch_ = 10;
        conf.max_batches_ /= 5;
        conf.batch_size_ *= 5;
        enc.backprop(inputs, conf, progress);

        std::ofstream f("enc.dat", std::ofstream::binary);
        enc.store(f);
    };

    auto test_dbn = [&](bool is_simple) {
        DeepBeliefNet rbm;
        std::string file = is_simple ? "dbn-s.dat" : "dbn.dat";
        std::ifstream f(file, std::ifstream::binary);
        rbm.load(f);

        size_t correct = 0, second = 0;
        for (size_t i = 0; i < images.size(); ++i) {
//            const TenR & image = images.at(i+1);
            const TenR &label = labels.at(i + 1);
//            const Sample &sample = samples[i];

            std::vector<int> idx(10);
            for (int i = 0; i < 10; ++i) idx[i] = i;

            static Vector nil;
            Vector output(10);
            if (is_simple)
                rbm.predict(inputs[i], output, nil);
            else
                rbm.predict(inputs[i], nil, output);

            std::sort(idx.begin(), idx.end(), [&output](int x, int y) { return output[x] > output[y]; });

            if (idx[0] == (int) label(1)) ++correct;
            else if (idx[1] == (int) label(1)) ++second;


            if ((i + 1) % 100 == 0)
                std::cout << "# " << correct << "/" << i + 1 << " recognized. 1st: " << (correct * 100.0 / (i + 1))
                          << "%, 1st+2nd: " << (correct + second) * 100.0 / (i + 1) << "%" << std::endl;
        }

        std::cout << "# " << correct << " recognized." << std::endl;
    };

    // execute commands
    if (command == "train") train_dbn();
    else if (command == "train-simple") train_dbn_simple();
    else if (command == "test") test_dbn(false);
    else if (command == "test-simple") test_dbn(true);
    else if (command == "train-encoder") train_autoencoder();
    else {
        std::cerr << "unrecognized command: " << command << std::endl;
    }

    return 1;
}


NumB ml_dbn_predict(DeepBeliefNet &rbm,
                    const std::vector<Vector> &inputs, std::vector<Vector> &outputs, bool is_simple) {
    if (outputs.size() != inputs.size()) {
        outputs.resize(inputs.size());
    }
    for (size_t i = 0; i < inputs.size(); ++i) {
        static Vector nil;
        if (outputs[i].size() != 10) {
            outputs[i].resize(10);
        }
        if (is_simple)
            rbm.predict(inputs[i], outputs[i], nil);
        else
            rbm.predict(inputs[i], nil, outputs[i]);
    }
    return 1;
}

NumB ml_dbn_predict(const char *model_path, const char *image_path, NumN &outcome, bool is_simple = 1) {
    DeepBeliefNet rbm;
    std::ifstream f(model_path, std::ifstream::binary);
    if (!f.is_open()) {
        printf("open %s fail!\n", model_path);
        return 0;
    }
    rbm.load(f);

    Seqce<TenR> images;
    images.setSize(1);
    star::img_read(images.at(1), image_path);

    if (images.isEmpty()) {
        return 0;
    }

    math21_img_rgb_to_gray(images);
    star::img_write(images.at(1), "a.png");


    math21_img_gray_to_binary(images);
    star::img_write(images.at(1), "b.png");

    // make sure all images have size (28, 28)
    VecN d;
    d.setSize(2);
    d = 28, 28;
    math21_img_resize(images, d, img_resize_method_pooling);

    star::img_write(images.at(1), "c.png");


    int n = (int) images.size();

    // initialize data: convert original data to required form.
    int data_size = (int) images(1).volume();
    std::vector<Vector> inputs(n);
    for (size_t i = 0; i < n; ++i) {
        const TenR &image = images.at(i + 1);
        Vector &input = inputs[i];
        input.resize(data_size);
        for (size_t j = 0; j < data_size; ++j)
            input[j] = (float) (image(j + 1) / 255.0); // > 30 ? 1.0: 0.0; // binary
    }
    std::vector<Vector> outputs;
    ml_dbn_predict(rbm, inputs, outputs, is_simple);


    for (size_t i = 0; i < outputs.size(); ++i) {
        std::vector<int> idx(10);
        for (int i = 0; i < 10; ++i) idx[i] = i;

        static Vector nil;
        Vector &output = outputs[i];

        std::sort(idx.begin(), idx.end(), [&output](int x, int y) { return output[x] > output[y]; });
        outcome = (NumN) idx[0];
        std::cout << "output " << i + 1 << " recognized. 1st: " << idx[0] << ", 2nd: " << idx[1] << std::endl;
        break;
    }
    return 1;
}

NumB ml_dbn_predict(const char *model_path, const char *images_config_path, const char *data_path_prefix,
                    bool is_simple = 1) {
    DeepBeliefNet rbm;
    std::ifstream f(model_path, std::ifstream::binary);
    if (!f.is_open()) {
        printf("open %s fail!\n", model_path);
        return 0;
    }
    rbm.load(f);

    Seqce<TenR> images;
    star::ml_data_read_from_img(images, images_config_path, 0, data_path_prefix);

    int n = (int) images.size();
    if (n <= 0) {
        std::cerr << "failed to read mnist data files: " << images_config_path << std::endl;
        return 0;
    }

    // initialize data: convert original data to required form.
    int data_size = (int) images(1).volume();
    std::vector<Vector> inputs(n);
    for (size_t i = 0; i < n; ++i) {
        const TenR &image = images.at(i + 1);
        Vector &input = inputs[i];
        input.resize(data_size);
        for (size_t j = 0; j < data_size; ++j)
            input[j] = (float) (image(j + 1) / 255.0); // > 30 ? 1.0: 0.0; // binary
    }
    std::vector<Vector> outputs;
    ml_dbn_predict(rbm, inputs, outputs, is_simple);
    for (size_t i = 0; i < outputs.size(); ++i) {
        std::vector<int> idx(10);
        for (int i = 0; i < 10; ++i) idx[i] = i;

        static Vector nil;
        Vector &output = outputs[i];

        std::sort(idx.begin(), idx.end(), [&output](int x, int y) { return output[x] > output[y]; });
        std::cout << "output " << i + 1 << " recognized. 1st: " << idx[0] << ", 2nd: " << idx[1] << std::endl;
    }
    return 1;
}


void ml_dbn_predict_test() {
    std::string home_path = STAR_HOME_PATH;
    const std::string file_dir = home_path + "/assets/data/mnist/images_a_few";
    const std::string data_path_prefix = home_path + "/assets/data/mnist/images_a_few/";
//    const std::string file_dir = home_path + "/assets/data/mnist/images";
//    const std::string data_path_prefix = home_path + "/assets/data/mnist/images/";
    std::string images_path = file_dir + "/train_images.txt";
//    std::string images_path = file_dir + "/test_images.txt";

    const std::string model_path = home_path + "/assets/models/dbn-s.dat";

    ml_dbn_predict(model_path.c_str(), images_path.c_str(), data_path_prefix.c_str());
}

void ml_dbn_predict_test_2() {
    std::string home_path = STAR_HOME_PATH;
    std::string working_path = STAR_WORKING_PATH;

//    std::string image_path = home_path + "/assets/data/mnist/images_a_few" + "/train_images/5.png";
//    std::string image_path = home_path + "/assets/data/digits/b4.png";
//    std::string image_path = working_path + "/assets/data/digits/b2.png";
    std::string image_path = working_path + "/assets/data/digits/b3.png";
//    std::string image_path = working_path + "/assets/data/digits/b4.png";
//    std::string image_path = working_path + "/assets/data/digits/b6.png";
//    std::string image_path = working_path + "/assets/data/digits/b7.png";
//    std::string image_path = working_path + "/assets/data/digits/b8.png";

    const std::string model_path = home_path + "/assets/models/dbn-s.dat";

    NumN outcome;
    math21::m21log(image_path);
    ml_dbn_predict(model_path.c_str(), image_path.c_str(), outcome);
}


int main(int argc, const char *argv[]) {
    sample_rbm();
//    ml_dbn_predict_test();
//    ml_dbn_predict_test_2();
    return 0;
}