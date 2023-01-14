cmake_minimum_required(VERSION 3.24)
project(DesignPatterns)

set(CMAKE_CXX_STANDARD 17)

add_executable(Test TestPointer.cpp)
add_subdirectory(Creational)
