cmake_minimum_required(VERSION 3.21)
project(aed)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(aed
        include/graph.h
        include/minHeap.h
        src/graph.cpp
        main.cpp)
