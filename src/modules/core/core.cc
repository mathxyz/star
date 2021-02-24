#include "inner_cc.h"
#include "wrapper.h"
#include "core.h"

namespace star {
    int sky_dummy(int argc, const char **argv) {
        if(argc>1){
            math21_tool_assert(0);
        }
        return 0;
    }

    void setMap(_Map<std::string, int (*)(int argc, const char **argv)> &maps) {
        maps.clear();
        maps.add("sky", &sky_dummy);
        maps.add("sky_ml_img_draw_and_save", &sky_ml_img_draw_and_save);
        maps.add("sky_ml_img_draw_and_save_csv_like", &sky_ml_img_draw_and_save_csv_like);
        maps.add("sky_ml_img_draw_and_save_csv_like_multiple", &sky_ml_img_draw_and_save_csv_like_multiple);
        maps.add("sky_data_edit_csv_like", &sky_data_edit_csv_like);
        maps.add("sky_ml_file_replace_character", &sky_ml_file_replace_character);
        maps.add("sky_ml_img_draw_and_save_data_2d_multiple", &sky_ml_img_draw_and_save_data_2d_multiple);
        maps.add("star_img_read_and_write", &star_img_read_and_write);
        maps.add("star_ls", &star_ls);
        maps.add("star_ls_clear", &star_ls_clear);
        maps.add("sky_file_rename_files", &sky_file_rename_files);
        maps.add("sky_file_is_same", &sky_file_is_same);
        maps.add("sky_file_is_text", &sky_file_is_text);
        maps.add("sky_file_byte_set", &sky_file_byte_set);
        maps.add("sky_file_vector_c_analysis", &sky_file_vector_c_analysis);
        maps.add("sky_plot_data", &sky_plot_data);
        maps.add("sky_loop_forever", &sky_loop_forever);
        maps.add("sky_exit", &sky_exit);
        maps.add("ml_kmeans", &sky_ml_kmeans);
        maps.add("ml_rl_chapter01_tic_tac_toe", &sky_ml_rl_chapter01_tic_tac_toe);
        maps.add("ml_rl_chapter02_k_armed_bandit", &sky_ml_rl_chapter02_k_armed_bandit);
        maps.add("ml_rl_chapter03_grid_world", &sky_ml_rl_chapter03_grid_world);
        maps.add("ml_rl_chapter04_gamblers_problem", &sky_ml_rl_chapter04_gamblers_problem);
        maps.add("ml_rl_chapter04_car_rental", &sky_ml_rl_chapter04_car_rental);
        maps.add("ml_rl_chapter04_car_rental_synchronous", &sky_ml_rl_chapter04_car_rental_synchronous);
        maps.add("ml_rl_chapter04_grid_world", &sky_ml_rl_chapter04_grid_world);
        maps.add("ml_rl_chapter05_blackjack", &sky_ml_rl_chapter05_blackjack);
        maps.add("ml_rl_chapter06_random_walk", &sky_ml_rl_chapter06_random_walk);
        maps.add("ml_rl_chapter06_cliff_walking", &sky_ml_rl_chapter06_cliff_walking);
        maps.add("sky_test", &sky_test);
        maps.add("heart", &star_ml_img_draw_and_save_heart);
        maps.add("tool_endian", &sky_tool_log_endian);
        maps.add("tool_num_type", &sky_tool_log_num_type);
        maps.add("star_tool_monitor_turn_on", &star_tool_monitor_turn_on);
        maps.add("star_tool_monitor_turn_off", &star_tool_monitor_turn_off);
        maps.add("star_tool_timer_setInterval", &star_tool_timer_setInterval);
        maps.add("star_tool_timer_setTimeout", &star_tool_timer_setTimeout);
        maps.add("star_tool_system_cmd", &star_tool_system_cmd);
        maps.add("star_tool_curl", &star_tool_curl);
        maps.add("star_tool_wget", &star_tool_wget);
        maps.add("sky_ml_net_run_rnn", &sky_ml_net_run_rnn);
        maps.add("sky_ml_detect", &sky_ml_detect);
        maps.add("sky_ml_function_paras_convert", &sky_ml_function_paras_convert);
    }

    Star::Star() {
        setMap(maps);
    }

    const _Map<std::string, int (*)(int argc, const char **argv)> &Star::getMaps() const {
        return maps;
    }

    class StarWrapper {
    public:
        static Star s;

        StarWrapper() {

        }

        virtual ~StarWrapper() {}
    };

    Star StarWrapper::s = Star();

    Star &star_get_default() {
        return StarWrapper::s;
    }

    void star_test() {
        m21log(__FUNCTION__);
        exit(0);
    }
}