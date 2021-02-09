cmake_minimum_required(VERSION 3.17)
project(SandPile)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE -O3)
set(CMAKE_BUILD_TYPE Debug)
add_executable(SandPile main.cpp)