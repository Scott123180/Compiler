cmake_minimum_required(VERSION 3.6)
project(Compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    project/testcases/minimal
    project/main.cpp
    project/main.h
    project/makefile
    project/not_main.cpp
    project/token.cpp
    project/token.h)

add_executable(Compiler ${SOURCE_FILES})