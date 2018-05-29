#include <iostream>
#include "src/NEAT.hpp"


int main() {
    NEAT neat = NEAT(10, 10, 123);
    neat.train(0.05f, "", "", "");

    // setup
    srand(NULL);
    ANN ann = ANN(100, 4, "Hello");
    Snake bradley = Snake(10, 10);
    for (int i = 0; i < 50; ++i) {
        ann.connectionMutation();
        ann.nodeMutation();
    }
    bradley.fitness(ann, true);

    return 0;
}
