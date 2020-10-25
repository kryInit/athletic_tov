cmake_minimum_required(VERSION 3.16)
project(tov)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-DDEBUG")
add_executable(tov main.cpp vec3.hpp unit.hpp utility.hpp utility.cpp)

