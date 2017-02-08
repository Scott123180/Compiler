cmake_minimum_required(VERSION 3.6)
project(Compiler)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        project/main.cpp
        project/not_main.cpp
        project/README.md
        project/token.cpp
        project/token.h project/main.h)

add_executable(Compiler ${SOURCE_FILES})