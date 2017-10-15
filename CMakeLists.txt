cmake_minimum_required(VERSION 3.7)
project(pyds)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES test/test_main.cpp test/catch.hpp test/test_list.cpp src/list.hpp src/common.hpp)
add_executable(pyds_test ${SOURCE_FILES})
