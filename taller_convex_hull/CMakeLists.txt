cmake_minimum_required(VERSION 3.10)
project(ConvexHullMotionDetection)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Encontrar OpenCV
find_package(OpenCV REQUIRED)

include_directories(include)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp src/convex_hull.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
