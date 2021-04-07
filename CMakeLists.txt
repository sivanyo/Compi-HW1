cmake_minimum_required(VERSION 3.17)
project(hw1)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(hw1_a part_a.cpp tokens.hpp lex.yy.c)
add_executable(hw1_b part_b.cpp tokens.hpp lex.yy.c)
