cmake_minimum_required(VERSION 3.20)
project(aes128_cbc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(aes128_cbc main.cpp block.cpp aes.cpp constants.h)
