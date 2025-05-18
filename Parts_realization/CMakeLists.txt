cmake_minimum_required(VERSION 3.10)
project(TEST2025)
set(CMAKE_CXX_STANDARD 20)

add_executable(Test2025 part1/src/Matrix.cpp part1/src/Model.cpp part2/main_matrix.cpp)

