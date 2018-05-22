#include <iostream>
#include "src/NEAT.hpp"


int main() {
    NEAT neat = NEAT(10, 10, 123);
    neat.train(0.05f, "", "", "");

    return 0;
}
