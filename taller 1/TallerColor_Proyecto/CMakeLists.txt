cmake_minimum_required(VERSION 3.10)
project(TallerColor_Proyecto)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV
find_package(OpenCV REQUIRED)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    main.cpp
    src/Ejercicio1.cpp
    src/Ejercicio2.cpp
    src/Ejercicio3.cpp
    src/Ejercicio4.cpp
    src/Ejercicio6.cpp
    src/Ejercicio7.cpp
)

# Create executable
add_executable(TallerColor_Proyecto ${SOURCES})

# Link OpenCV
target_link_libraries(TallerColor_Proyecto ${OpenCV_LIBS})

# Log status
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
