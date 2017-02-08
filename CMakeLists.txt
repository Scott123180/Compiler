cmake_minimum_required(VERSION 3.6)
project(Compiler)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        project/lexer.cpp
        project/main.cpp
        project/README.md
        project/token.cpp
        project/token.h project/lexer.h)

add_executable(Compiler ${SOURCE_FILES})