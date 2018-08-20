cmake_minimum_required(VERSION 3.8)

project(neat)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

add_subdirectory (third_party EXCLUDE_FROM_ALL)
add_subdirectory (lib)
add_subdirectory (app)

enable_testing ()

# set(SOURCE_FILES main.cpp src/ANN.cpp src/ANN.hpp src/ConnectionGene.cpp src/ConnectionGene.hpp src/Gene.cpp src/Gene.h src/Node.cpp src/Node.hpp src/NEAT.cpp src/NEAT.hpp src/SnakeGame.cpp src/SnakeGame.hpp src/Snake.cpp src/Snake.hpp)





# # Tests
# enable_testing()
# add_subdirectory(tests)
# add_executable(neat ${SOURCE_FILES})
