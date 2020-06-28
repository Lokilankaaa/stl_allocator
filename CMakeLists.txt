cmake_minimum_required(VERSION 3.16)
project(stl_allocator)

set(CMAKE_CXX_STANDARD 14)

add_executable(stl_allocator main.cpp src/Memorypool.cpp src/Memorypool.h src/allocator.h src/Memorypool.cpp)