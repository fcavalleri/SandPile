cmake_minimum_required(VERSION 3.17)
project(SandPile)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE -Ofast)
set(CMAKE_BUILD_TYPE Release)
add_executable(SandPile main.cpp)
target_link_libraries(SandPile sfml-graphics sfml-window sfml-system)