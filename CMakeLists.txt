cmake_minimum_required(VERSION 3.9)
project(MUSASHI)

set(CMAKE_CXX_STANDARD 11)

add_executable(MUSASHI
        src/cpp/include.hpp
        src/cpp/main.cpp
        src/cpp/util.cpp
        src/cpp/util.hpp
        src/cpp/core.cpp
        src/cpp/core.hpp)
