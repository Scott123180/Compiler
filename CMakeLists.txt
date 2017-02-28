cmake_minimum_required(VERSION 3.6)
project(Compiler)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        project/lexer.cpp
        project/lexer.h
        project/main.cpp
        project/main.h
        project/token.cpp
        project/token.h)

add_executable(Compiler ${SOURCE_FILES})