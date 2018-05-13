cmake_minimum_required(VERSION 3.8)
project(neat)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp src/ANN.cpp src/ANN.hpp src/ConnectionGene.cpp src/ConnectionGene.hpp src/Node.cpp src/Node.hpp src/NEAT.cpp src/NEAT.hpp src/Snake.cpp src/Snake.hpp)
add_executable(neat ${SOURCE_FILES})