set (module_name lily_io3)
message(STATUS "${module_name}")


include_directories(../includes)

#FILE(GLOB sourcefiles "*.cpp" "*.c")
FILE(GLOB_RECURSE sourcefiles "*.cpp" "*.cc" "*.c")

#FILE(GLOB sourcefiles "src/*.cpp" "src/*.c")
#add_library(${module_name} SHARED ${sourcefiles})
add_library(${module_name} STATIC ${sourcefiles})


install (TARGETS ${module_name}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

