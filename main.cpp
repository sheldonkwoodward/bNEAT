#include <iostream>
#include "src/NEAT.hpp"


int main() {
    NEAT neat = NEAT(5, 5, 123);
    neat.train(0.1f, "", "", "");

    return 0;
}
