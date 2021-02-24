cmake_minimum_required(VERSION 3.5)
project(star)

get_directory_property(has_parent PARENT_DIRECTORY)
#    message(FATAL_ERROR ${has_parent})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # use this instead of -fpic
###############  star  #####################

if (STAR_EXTERNAL_FLAG)
    option(STAR_FLAG_RELEASE "Enable Release" ${STAR_EXTERNAL_FLAG_RELEASE})
    option(STAR_FLAG_USE_CUDA "Use CUDA" ${STAR_EXTERNAL_FLAG_USE_CUDA})
    option(STAR_FLAG_USE_OPENCL "Use OpenCL" ${STAR_EXTERNAL_FLAG_USE_OPENCL})
else ()
    #                message(FATAL_ERROR "CHECK")
    option(STAR_FLAG_RELEASE "Enable Release" ON)
#    option(STAR_FLAG_USE_CUDA "Use CUDA" ON)
#    option(STAR_FLAG_USE_OPENCL "Use OpenCL" ON)
endif ()

message(STATUS "STAR_EXTERNAL_FLAG: ${STAR_EXTERNAL_FLAG}")
####################################
set(LI_INSTALL_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../z)
set(CMAKE_INSTALL_PREFIX ${LI_INSTALL_LOCATION})
###############  MATH21  #####################
option(MATH21_FLAG_EXTERNAL "Enable math21 external set" ON)
option(MATH21_FLAG_EXTERNAL_RELEASE "Enable Release" ${STAR_FLAG_RELEASE})
option(MATH21_FLAG_EXTERNAL_USE_CUDA "Use CUDA" ${STAR_FLAG_USE_CUDA})
option(MATH21_FLAG_EXTERNAL_USE_OPENCL "Use OpenCL" ${STAR_FLAG_USE_OPENCL})
option(FIT_FLAG_EXTERNAL "Enable fit external set" ON)
option(FIT_FLAG_EXTERNAL_USE_CUDA "Use CUDA" ${STAR_FLAG_USE_CUDA})
##########################################################
if (${STAR_FLAG_USE_CUDA})
    # can put to executable related file.
    set(ENV{CUDACXX} /usr/local/cuda/bin/nvcc)
    enable_language(CUDA)
    set(CMAKE_CUDA_ARCHITECTURES OFF)
endif ()
##########################################################
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpic")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")
##########################################################

####################################
if (1)
    #        option(STAR_RELEASE "Enable Release" ON)
    if (WIN32)
        message(STATUS "WIN32")
        set(STAR_HOME_PATH "D:/mathxyz")
    else ()
        message(STATUS "UNIX")
        set(STAR_HOME_PATH "/mathxyz")
    endif ()
endif ()

set(STAR_WORKING_PATH ${CMAKE_CURRENT_SOURCE_DIR}/..)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config_generated @ONLY)
####################################

if (has_parent)
else ()
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty z_out)
endif ()
include_directories(.)
include_directories(../..)
include_directories(includes)
include_directories(./3rdparty)
include_directories(./3rdparty/math21/includes)

add_subdirectory(3rdparty/math21)

add_subdirectory(3rdparty)
add_subdirectory(modules)
if (ANDROID)
else ()
    #    add_subdirectory(examples)
endif ()
add_subdirectory(apps)