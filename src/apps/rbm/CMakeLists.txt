#include_directories(../includes)
set (module_name rbm_test)
message(STATUS "${module_name}")

FILE(GLOB_RECURSE sourcefiles "*.cpp" "*.c" "*.cc")

#FILE(GLOB sourcefiles "src/*.cpp" "src/*.c")
add_executable(${module_name} ${sourcefiles})
target_link_libraries(${module_name} lily_z star)

install (TARGETS ${module_name}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
