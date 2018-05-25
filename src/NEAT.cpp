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
    ANN ann1 = ANN(100, 4, "AAA");
    ANN ann2 = ANN(100, 4, "AAA");

    ann1.connectionMutation();
    ann2.connectionMutation();
    ann1.nodeMutation();
//    ann2.nodeMutation();

    ann1.setFitness(1.0f);
    ann2.setFitness(2.0f);

    ANN ann3 = ANN(ann1, ann2);
    ann3.weightMutation();
    ann3.weightMutation();
    ann3.weightMutation();
    std::cout << std::endl << "CROSS" << std::endl;
    ANN ann4 = ANN(ann3, ann1);

    // TODO: mutation
    // TODO: survivor selection
}
