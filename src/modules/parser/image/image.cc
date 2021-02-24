#include "image_c.h"
#include "image.h"

namespace star {
    void img_set_color(TenR &image, const VecN &color_bg) {
        if (image.isEmpty()) {
            return;
        }
        MATH21_ASSERT(image.dims() == 3, "must be 3-D tensor.");
        MATH21_ASSERT(color_bg.size() == 3, "must be rgb color in [0, 255]!");
        NumN i2, i3;
//#pragma omp parallel for private(i3) collapse(2)
        for (i2 = 1; i2 <= image.dim(2); ++i2) {
            for (i3 = 1; i3 <= image.dim(3); ++i3) {
                image(1, i2, i3) = color_bg(1);
                image(2, i2, i3) = color_bg(2);
                image(3, i2, i3) = color_bg(3);
            }
        }
    }

    int star_img_read_and_write(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tstar_img_read_and_write \n"
                       "\t(src_path dst_path (or dst_dir) [flags])\n"
                       "\tNote:\n"
                       "\t  src_path == null enables batch mode."
                       "\t  dst_path or dst_dir\n"
                       "\t      dst_dir is used when in batch.\n"
                       "\tflags:\n"
                       "\t  --dst_suffix\n"
                       "\t      The default value is 'null'.\n"
                       "\t      Used only when in batch.\n"
                       "\t  --src_format (i420 | yv12 | nv12 | nv21 | argb8888 | mat | blank)\n"
                       "\t      The default value is 'null'.\n"
                       "\t  --dst_format (i420 | yv12 | nv12 | nv21 | argb8888 | mat)\n"
                       "\t      The default value is 'null'.\n"
                       "\t  --src_nr (NumN)\n"
                       "\t      The default value is '0'.\n"
                       "\t  --src_nc (NumN)\n"
                       "\t      The default value is '0'.\n"
                       "\t  --dst_nr (NumN)\n"
                       "\t      The default value is '0'.\n"
                       "\t  --dst_nc (NumN)\n"
                       "\t      The default value is '0'.\n"
                       "\t  --src_pixel (NumN)\n"
                       "\t      The default value is 'rgba'.\n"
                       "\t      e.x., '255 255 255 255'.\n"
                       "\t  --print\n"
                       "\t      Print files.\n"
                );
                m21log("\te.x., \n"
                       "\theart.yuv heart.jpg --src_format nv12 --src_nr 288 --src_nc 352\n");
                return 1;
            }
        }

        if (argc < 3) {
            return 0;
        }
        StarImageConfig src_config;
        StarImageConfig dst_config;
        std::string dst_suffix = "";
        NumB isLog = 0;
        for (int i = 3; i < argc; ++i) {
            if (math21_string_is_equal(argv[i], "--dst_suffix")) {
                dst_suffix = argv[i + 1];
                ++i;
            } else if (math21_string_is_equal(argv[i], "--src_format")) {
                src_config.format = star_image_get_format_NumN(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--src_nr")) {
                src_config.nr = math21_string_to_NumN(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--src_nc")) {
                src_config.nc = math21_string_to_NumN(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--dst_format")) {
                dst_config.format = star_image_get_format_NumN(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--dst_nr")) {
                dst_config.nr = math21_string_to_NumN(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--dst_nc")) {
                dst_config.nc = math21_string_to_NumN(argv[i + 1]);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--src_pixel")) {
                for (NumN k = 1; k <= 4; ++k) {
                    src_config.pixel.at(k) = math21_string_to_NumN(argv[i + k]);
                }
                i += 4;
            } else if (math21_string_is_equal(argv[i], "--print")) {
                isLog = 1;
            }
        }

        StarGlobalVariables &globalVariables = star_getGlobalVariables();
        Seqce<std::string> &ls_files = globalVariables.ls_files;
        if (math21_string_is_equal(argv[1], "null") && !ls_files.isEmpty()) {
            std::string dir = argv[2];
            Io io;
            io.mkdirs(dir);
            for (NumN i = 1; i <= ls_files.size(); ++i) {
                std::string s;
                s = ls_files(i);
                s = star_string_get_file_name(s);
                s = star_string_remove_suffix(s);
                std::string dst_name;
                if (!math21_string_is_equal(dst_suffix, "")) {
                    dst_name = std::string(dir) + "/" + s + "." + dst_suffix;
                } else {
                    dst_name = std::string(dir) + "/" + s;
                }
                if (isLog) {
                    std::cout << ls_files(i) << "\n-> " << dst_name << std::endl;
                }
                TenR image;
                img_read(image, ls_files(i).c_str(), src_config);
                img_write(image, dst_name.c_str(), dst_config);

            }
        } else {
            TenR image;
            if (isLog) {
                std::cout << argv[1] << "\n-> " << argv[2] << std::endl;
            }
            img_read(image, argv[1], src_config);
            img_write(image, argv[2], dst_config);
        }
        return 1;
    }

    StarGlobalVariables globalVariables;

    StarGlobalVariables &star_getGlobalVariables() {
        return globalVariables;
    }

    int star_ls(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tstar_ls \n"
                       "\t((dir) [flags])\n"
                       "\tflags:\n"
                       "\t  --pattern (*.* | *.txt | a* | *b)\n"
                       "\t      The default value is 'null'.\n"
                       "\t  --r or --recursive\n"
                       "\t      Search recursively.\n"
                       "\t  --print\n"
                       "\t      Print files.\n"
                );
                m21log("\te.x., \n"
                       "\tstar_ls folder --pattern *.txt\n");
                return 1;
            }
        }

        if (argc < 1) {
            return 0;
        }
        const char *path = ".";
        NumB isRecursive = 0;
        NumB isLog = 0;
        Io io;
        Seqce<std::string> patterns;

        std::string pat;
        for (int i = 1; i < argc; ++i) {
            if (math21_string_is_equal(argv[i], "--pattern")) {
                pat = argv[i + 1];
                patterns.push(pat);
                ++i;
            } else if (math21_string_is_equal(argv[i], "--r") || math21_string_is_equal(argv[i], "--recursive")) {
                isRecursive = 1;
            } else if (math21_string_is_equal(argv[i], "--print")) {
                isLog = 1;
            } else {
                path = argv[i];
            }
        }
        StarGlobalVariables &globalVariables = star_getGlobalVariables();
        globalVariables.ls_files.clear();
        io.ls(globalVariables.ls_files, path, patterns, isRecursive);

        if (isLog) {
            globalVariables.ls_files.log("files");
        }
        return 1;
    }

    int star_ls_clear(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tstar_ls_clear \n"
                       "\tNote:\n"
                       "\t  Clear all global variables.\n"
                );
                m21log("\te.x., \n"
                       "\tstar_ls_clear\n");
                return 1;
            }
        }

        if (argc < 1) {
            return 0;
        }

        StarGlobalVariables &globalVariables = star_getGlobalVariables();
        globalVariables.ls_files.clear();
        return 1;
    }

    int sky_exit(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tsky_exit \n"
                       "\tNote:\n"
                       "\t  exit.\n"
                );
                m21log("\te.x., \n"
                       "\tsky_exit\n");
                return 1;
            }
        }

//        fflush(stdout);
//        fflush(stderr);
//        std::cout<<std::flush;
        m21exit("sky");
        return 1;
    }
    int sky_loop_forever(int argc, const char **argv) {
        if (argc == 2) {
            if (math21_string_is_equal(argv[1], "--help") || math21_string_is_equal(argv[1], "--h")) {
                m21log("\tsky_loop_forever\n"
                       "\tNote:\n"
                       "\t  loop forever.\n"
                );
                m21log("\te.x., \n"
                       "\tsky_loop_forever\n");
                return 1;
            }
        }
        while (1);
        return 1;
    }
}

using namespace star;

m21image star_image_read_image(const char *path) {
    Tensor<NumR32> m;
    img_read_common_format(m, path);
    NumR32 *m_data = math21_memory_tensor_data_address(m);
    MATH21_ASSERT(m.dims()==3, "not color image")
    m21image image = math21_image_create_image(m.dim(2), m.dim(3), m.dim(1));
    math21_vector_copy_buffer_cpu(image.data, m_data, m.volume(), sizeof(NumR32));
    return image;
}

m21image star_image_read_image_normalized(const char *path) {
    m21image image = star_image_read_image(path);
    math21_vector_kx_cpu(image.nr * image.nc * image.nch, 1 / 255.0f, image.data, 1);
    return image;
}

m21image star_image_read_image_normalized_with_rc(const char *path, NumN nr, NumN nc) {
    m21image out = star_image_read_image_normalized(path);
    if ((nr && nc) && (nr != out.nr || nc != out.nc)) {
        m21image resized = math21_image_resize_image(out, nr, nc);
        math21_image_destroy_image(&out);
        out = resized;
    }
    return out;
}

void star_image_write_image(m21image image, const char *path) {
    Tensor<NumR32> m;
    m.setSize(image.nch, image.nr, image.nc);
    NumR32 *m_data = math21_memory_tensor_data_address(m);
    math21_vector_copy_buffer_cpu(m_data, image.data, m.volume(), sizeof(NumR32));
    star::detail::_img_write_common_format(m, path);
}

void star_image_write_image_normalized(m21image image0, const char *path) {
    m21image image = math21_image_clone_image(image0);
    math21_vector_kx_cpu(image.nr * image.nc * image.nch, 255.0f, image.data, 1);
    star_image_write_image(image, path);
    math21_image_destroy_image(&image);
}
