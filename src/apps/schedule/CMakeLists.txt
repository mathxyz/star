set (module_name schedule)
message(STATUS "${module_name}")

FILE(GLOB sourcefiles "src/*.cc" "src/*.c")

add_executable(${module_name} ${sourcefiles})

if (WIN32)
    target_link_libraries(${module_name} star -static)
else ()
    target_link_libraries(${module_name} star)
endif ()

install (TARGETS ${module_name}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
