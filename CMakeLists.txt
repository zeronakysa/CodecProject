cmake_minimum_required(VERSION 3.8)
project(CodecProject)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c functions.c functions.h)
add_executable(CodecProject ${SOURCE_FILES})