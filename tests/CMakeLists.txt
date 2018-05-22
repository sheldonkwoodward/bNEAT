cmake_minimum_required(VERSION 3.8)

project(tests)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
set(CMAKE_CXX_FLAGS "-Dprivate=public")

set(SOURCE_FILES testSnake.cpp catch.hpp "../src/Snake.cpp")
add_executable(tests ${SOURCE_FILES})
