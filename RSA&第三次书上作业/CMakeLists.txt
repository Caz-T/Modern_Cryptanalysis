cmake_minimum_required(VERSION 3.25)
project(RSA)

set(CMAKE_CXX_STANDARD 17)

add_executable(RSA main.cpp)
include_directories(RSA "/opt/homebrew/opt/gmp/include") # change to your gmp include path
target_link_libraries(RSA "/opt/homebrew/Cellar/gmp/6.2.1_1/lib/libgmp.a") # change to your gmp installation path
