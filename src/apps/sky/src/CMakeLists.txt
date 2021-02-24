set(module_name sky)
message(STATUS "${module_name}")

get_directory_property(has_parent PARENT_DIRECTORY)
#    message(FATAL_ERROR ${has_parent})

set(CMAKE_CXX_STANDARD 11)
####################################
#option(IS_ANDROID "IS_ANDROID" OFF)
#option(IS_ANDROID "IS_ANDROID" ON)

##########################################################
# The version number.
set(Tutorial_VERSION_MAJOR 1)
set(Tutorial_VERSION_MINOR 0)
##########################################################

FILE(GLOB sourcefiles "*.cc" "*.c")

if (ANDROID)
    add_library(${module_name} SHARED ${sourcefiles})
else ()
    add_executable(${module_name} ${sourcefiles})
endif ()

if (WIN32)
    target_link_libraries(${module_name} star -static)
#    target_link_libraries(${module_name} star)
    target_compile_options(${module_name} PRIVATE -fno-exceptions)
else ()
    target_link_libraries(${module_name} star)
endif ()

install(TARGETS ${module_name}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

install(FILES sky.h  DESTINATION include)
#######################################################

# build a CPack driven installer package
#include(InstallRequiredSystemLibraries)
#set(CPACK_RESOURCE_FILE_LICENSE
#        "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
#set(CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}")
#set(CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")
#include(CPack)
