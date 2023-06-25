cmake_minimum_required(VERSION 3.25)
project(SHA256)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_executable(SHA256 main.cpp sha256.cpp sha256.h constants.h state.cpp state.h)
