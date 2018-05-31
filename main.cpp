#include <iostream>
#include "src/NEAT.hpp"


int main() {
    NEAT neat = NEAT(7, 7, 123);
    neat.train();

    return 0;
}
