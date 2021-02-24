#include <fstream>
#include "kmean.h"

namespace star {
    void ml_kmeans(const char *data_path, const char *data_config_path, const char *dst_dir) {
        std::ifstream io(data_path, std::ifstream::in);
        if (!io.is_open()) {
            printf("open %s fail!\n", data_path);
            return;
        }
        NumN total_points, total_values, K, max_iterations, has_name;
        io >> total_points >> total_values >> K >> max_iterations >> has_name;
        MATH21_LOG_NAME_VALUE(total_points);
        MATH21_LOG_NAME_VALUE(total_values);
        MATH21_LOG_NAME_VALUE(K);
        MATH21_LOG_NAME_VALUE(max_iterations);

        std::string point_name;


        Seqce<TenR> data(total_points);
        VecN labels(total_points);
        VecN d(1);
        d = total_values;
        math21_tool_setSize(data, d);
        for (NumN i = 1; i <= total_points; ++i) {

            TenR &A = data.at(i);
            for (NumN j = 1; j <= total_values; ++j) {
                double value;
                io >> value;
                A(j) = value;
            }

            if (has_name) {
                io >> point_name;
                A.setName(point_name.c_str());
            }
        }

        ml_kmeans_config config(K, total_points, total_values, max_iterations);
        Seqce<VecN> points_in_clusters;
        ml_kmeans(data, labels, points_in_clusters, config);
        if (data_config_path) {
            MATH21_ASSERT(dst_dir)
//            points_in_clusters.log("points_in_clusters");
            ml_data_save_as_vector_using_index_multiple(data, points_in_clusters, data_config_path, dst_dir, 10, 32);

        }
    }

}

int sky_ml_kmeans_test() {
    std::string home_path = STAR_WORKING_PATH;
    std::string data_path = home_path + "/assets/kmeans/datasets/dataset1.txt";
    star::ml_kmeans(data_path.c_str(), "data_config.txt", "clusters");
    return 1;
}

int sky_ml_kmeans(int argc, const char **argv) {
    if (argc == 2) {
        if (star::math21_string_is_equal(argv[1], "--help") || star::math21_string_is_equal(argv[1], "--h")) {
            std::cout << "\tml_kmeans data_path dst_config_path dst_dir\n";
            return 1;
        } else if (star::math21_string_is_equal(argv[1], "test")) {
            sky_ml_kmeans_test();
            return 1;
        }
    }
    if (argc == 4) {
        star::ml_kmeans(argv[1], argv[2], argv[3]);
    } else {
        return 0;
    }
    return 1;
}

