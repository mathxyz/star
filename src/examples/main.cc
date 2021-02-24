#include "inner.h"
#include "test.h"

using namespace star;

std::string home_path = STAR_HOME_PATH;
std::string data_path = home_path + "/assets/data/mnist";

int main(int argc, char **argv) {

    test_file_io();
    return 0;
    if (argc == 2) {
//            sample_mlp(argv[1]);
        sample_cnn_mnist(argv[1]);
    } else {
//            sample_mlp(data_path);
        sample_cnn_mnist(data_path);
    }


}
