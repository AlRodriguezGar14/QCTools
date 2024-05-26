cmake_minimum_required(VERSION 3.28)
project(QC)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic -std=c++11")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(qctool main.cpp
        InactivePixelsTracker.cpp)
target_link_libraries(qctool ${OpenCV_LIBS})
