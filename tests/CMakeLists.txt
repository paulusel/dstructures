cmake_minimum_required(VERSION 4.0)

message(STATUS "Building test binaries")

file(GLOB_RECURSE TEST_SRCS "src/*.c")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
endif ()

add_executable(test ${TEST_SRCS})

target_link_libraries(test PRIVATE dstruct)
target_include_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set_target_properties(test PROPERTIES COMPILE_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
