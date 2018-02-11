cmake_minimum_required(VERSION 3.9)
project(wows_mod_loader)

set(CMAKE_CXX_STANDARD 11)

add_executable(wows_mod_loader
        src/cpp/includes.h
        src/cpp/main.cpp
        src/cpp/Utils.cpp
        src/cpp/Utils.h
        src/cpp/WMLKit.cpp
        src/cpp/WMLKit.h)
