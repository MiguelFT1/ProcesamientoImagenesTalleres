cmake_minimum_required(VERSION 3.10)
project(Taller2_Manual)

set(CMAKE_CXX_STANDARD 17)

# Find OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

# Executables
add_executable(frame_difference src/frame_differencing.cpp)
target_link_libraries(frame_difference ${OpenCV_LIBS})

add_executable(frame_average src/frame_averaging.cpp)
target_link_libraries(frame_average ${OpenCV_LIBS})

add_executable(main src/main.cpp)
target_link_libraries(main ${OpenCV_LIBS})
