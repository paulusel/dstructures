cmake_minimum_required(VERSION 4.0)
project(dstructures DESCRIPTION "Common data structures in C")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(SRC_DIR "src")
file(GLOB_RECURSE SRCFILES "${SRC_DIR}/*.c")

add_library(dstruct ${SRCFILES})

target_include_directories(dstruct PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
