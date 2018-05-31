#include <iostream>
#include "src/NEAT.hpp"


int main() {
    NEAT neat = NEAT(5, 5, 123);
    neat.train();

    return 0;
}
