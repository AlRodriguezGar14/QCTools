cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_COMPILER c++)
set(CMAKE_CXX_STANDARD 17)
project(QC LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(qctool main.cpp
        InactivePixelsTracker.cpp
        duplicateFramesTracker.cpp
        converter.cpp
        FramesTracker.cpp
        HTML.cpp
        ProgressTracker.cpp
        HTML.cpp)
target_link_libraries(qctool ${OpenCV_LIBS})
