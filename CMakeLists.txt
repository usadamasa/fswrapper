########################################################################
#
# Project-wide settings

# Name of the project.
#
# CMake files in this project can refer to the root source directory
# as ${fswrapper_SOURCE_DIR} and to the root binary directory as
# ${fswrapper_BINARY_DIR}.
# Language "C" is required for find_package(Threads).
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS
        "-Wno-deprecated-declarations -std=c++0x")

include(${CMAKE_SOURCE_DIR}/cmake/boost.cmake)

include_directories(
        "${PROJECT_SOURCE_DIR}/src")

add_subdirectory(src)

# tests
if(NOT without-test)
    enable_testing()
    add_subdirectory(test)
endif()
