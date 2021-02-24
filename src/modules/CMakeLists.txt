set(module_name star)
message(STATUS "${module_name}")


FILE(GLOB_RECURSE sourcefiles
        "core/*.cc" "core/*.c"
        "io/*.cc" "io/*.c"
        "ml/*.cc" "ml/*.c"
        "data/*.cc" "data/*.c"
        "parser/*.cc" "parser/*.c"
        "util/*.cc" "util/*.c")

if (ANDROID)
    set(LIB_SHARED_OR_STATIC SHARED)
else ()
    set(LIB_SHARED_OR_STATIC SHARED)
#    set(LIB_SHARED_OR_STATIC STATIC)
endif ()

add_library(${module_name} ${LIB_SHARED_OR_STATIC} ${sourcefiles})
message(STATUS "${module_name}: ${LIB_SHARED_OR_STATIC}")

target_link_libraries(${module_name} math21 lily_png lily_jpeg)
#target_link_libraries(${module_name} math21_imported lily_png lily_jpeg)

install(TARGETS ${module_name}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

