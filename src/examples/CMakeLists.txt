set (module_name star_test)
message(STATUS "${module_name}")


#FILE(GLOB_RECURSE SOURCE_FILES "star/*.c" "star/*.cpp")
FILE(GLOB SOURCE_FILES "*.c" "*.cc")
#set (sourcefiles ${sourcefiles2})

add_executable(${module_name} ${SOURCE_FILES})
target_link_libraries(${module_name} star)

install (TARGETS ${module_name}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
