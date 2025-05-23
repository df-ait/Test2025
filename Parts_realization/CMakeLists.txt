cmake_minimum_required(VERSION 3.10)
project(TEST2025)
set(CMAKE_CXX_STANDARD 20)
set(OpenCV_DIR D:/ddddffff/opencv/build/x64/MinGW)
find_package(OpenCV REQUIRED)
include_directories(D:/ddddffff/opencv/build/x64/MinGW/install/include/opencv2/)
add_executable(Test2025 main_matrix.cpp)
target_link_libraries(Test2025 ${OpenCV_LIBS})

