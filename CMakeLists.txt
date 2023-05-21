cmake_minimum_required(VERSION 3.25)
project(SHA256)

set(CMAKE_CXX_STANDARD 20)

add_executable(SHA256 main.cpp sha256.cpp sha256.h constants.h state.cpp state.h)
