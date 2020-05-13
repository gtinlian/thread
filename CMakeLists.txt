cmake_minimum_required(VERSION 3.11)
project(thread)

set(CMAKE_CXX_STANDARD 14)

link_directories("/usr/lib64/mysql")
add_executable(thread main.cpp)

target_link_libraries(thread pthread)