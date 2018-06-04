#include <iostream>
#include "src/NEAT.hpp"


int main() {
    NEAT neat = NEAT(10, 10, 123);
    neat.train();

    return 0;
}
