cmake_minimum_required(VERSION 3.16)
project(gameoflife)

set(CMAKE_CXX_STANDARD 17)

add_executable(gameoflife main.cpp)
target_link_libraries(gameoflife -lGLU32 -lOpenGL32 -lfreeGLUT)