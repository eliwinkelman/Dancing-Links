cmake_minimum_required(VERSION 3.6)
project(Dancing_Links)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Sudoku.cpp Sudoku.h)
add_executable(Dancing_Links ${SOURCE_FILES})