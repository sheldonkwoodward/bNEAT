//
// Created by Sheldon Woodward on 5/12/18.
//

#include "NEAT.hpp"

NEAT::NEAT(int sizeX, int sizeY, unsigned int seed) : snake(sizeX, sizeY) {
    srand(seed);
    population = std::deque<ANN>();
}

void NEAT::train(float mutationRate, std::string fitness, std::string parentSelection, std::string survivorSelection) {
    // TODO: populate
    // TODO: parent selection
    // TODO: crossover

    ANN ann1 = ANN(3, 1, "AAA");
    ANN ann2 = ANN(3, 1, "AAA");

    for (int i = 0; i < 2; i++) {
        ann1.connectionMutation();
        ann2.connectionMutation();
        ann1.nodeMutation();
        ann2.nodeMutation();
    }

    ann1.setFitness(1.0f);
    ann2.setFitness(1.0f);

    ANN ann3 = ANN(ann1, ann2);

    ann1.printGenome();
    std::cout << "OUTPUT" << std::endl;
    for (auto &o : ann1.compute(std::deque<float>(3, 100.0f))) {
        std::cout << o << " ";
    }
    std::cout << std::endl;

    // TODO: mutation
    // TODO: survivor selection
}
