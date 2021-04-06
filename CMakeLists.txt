cmake_minimum_required(VERSION 3.17)
project(hw1)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(hw1
        part_a.cpp
        tokens.hpp)
