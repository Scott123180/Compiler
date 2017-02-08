cmake_minimum_required(VERSION 3.6)
project(Compiler)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        lexer.cpp
        main.cpp
        README.md
        token.cpp
        token.h)

add_executable(Compiler ${SOURCE_FILES})