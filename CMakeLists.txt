cmake_minimum_required(VERSION 3.17)
project(depth-first-search)

set(CMAKE_CXX_STANDARD 20)
if(gcc)
    set(CMAKE_CXX_FLAGS -fconcepts -std=c++20)
endif()
if(msvc)
    set(CMAKE_CXX_FLAGS /std:c++latest)
endif()

add_executable(depth-first-search main.cpp)