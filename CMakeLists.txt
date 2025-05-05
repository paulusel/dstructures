cmake_minimum_required(VERSION 4.0)
project(dstructures DESCRIPTION "Common data structures in C")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB_RECURSE SRCFILES "src/*.c")

add_library(dstruct ${SRCFILES})
target_include_directories(dstruct PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

option(BUILD_TEST "Build test executables" OFF)
if(BUILD_TEST)
    add_subdirectory("tests")
 endif(BUILD_TEST)

