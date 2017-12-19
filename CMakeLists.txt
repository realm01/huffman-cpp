cmake_minimum_required(VERSION 3.9)
project(huffman_cpp)

set(CMAKE_CXX_STANDARD 11)

add_executable(huffman_cpp
        src/huffman.cpp
        src/huffman.h
        src/main.cpp)
