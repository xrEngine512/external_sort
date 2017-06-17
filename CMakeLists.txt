cmake_minimum_required(VERSION 2.8)
project(task)

set(CMAKE_CXX_STANDARD 14)

include_directories(common)

add_subdirectory(generator)
add_subdirectory(sorter)
add_subdirectory(checker)