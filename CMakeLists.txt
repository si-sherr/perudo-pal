cmake_minimum_required(VERSION 4.3)

project(perudo-pal)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_subdirectory(src/core)
add_subdirectory(src)
add_subdirectory(tests)
