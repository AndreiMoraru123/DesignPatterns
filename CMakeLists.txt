cmake_minimum_required(VERSION 3.24)
project(DesignPatterns)

set(CMAKE_CXX_STANDARD 11)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

add_executable(Test TestPointer.cpp)
add_subdirectory(Creational)
add_subdirectory(Structural)
add_subdirectory(Behavioral)
