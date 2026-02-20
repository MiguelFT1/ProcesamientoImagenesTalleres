cmake_minimum_required(VERSION 3.10)
project(Taller3 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

# Ejecutable Parte A: Frame Differencing
add_executable(parte_a src/main_frame_diff.cpp src/frame_diff.cpp)
target_link_libraries(parte_a ${OpenCV_LIBS})

# Ejecutable Parte B: GMM
add_executable(parte_b src/main_gmm.cpp src/gmm_segmenter.cpp)
target_link_libraries(parte_b ${OpenCV_LIBS})
