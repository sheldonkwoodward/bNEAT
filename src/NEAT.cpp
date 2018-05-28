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
    ANN ann3 = ANN(3, 1, "AAA");

    ANN ann4 = ANN(3, 1, "AAA");
    ANN ann5 = ANN(3, 1, "AAA");
    ANN ann6 = ANN(3, 1, "AAA");

    for (int i = 0; i < 20; i++) {
        ann1.connectionMutation();
        ann2.connectionMutation();
        ann3.connectionMutation();

        ann1.nodeMutation();
        ann2.nodeMutation();
        ann3.nodeMutation();

        ann1.setFitness(1.0f);
        ann2.setFitness(1.0f);
        ann3.setFitness(1.0f);

        ann1 = ANN(ann2, ann3);
        ann2 = ANN(ann1, ann3);
        ann3 = ANN(ann1, ann2);
    }

    for (int i = 0; i < 20; i++) {
        ann4.connectionMutation();
        ann5.connectionMutation();
        ann6.connectionMutation();

        ann4.nodeMutation();
        ann5.nodeMutation();
        ann6.nodeMutation();

        ann4.setFitness(1.0f);
        ann5.setFitness(1.0f);
        ann6.setFitness(1.0f);

        ann4 = ANN(ann5, ann6);
        ann5 = ANN(ann4, ann6);
        ann6 = ANN(ann4, ann5);
    }

    ANN ann = ANN(ann3, ann6);

    ann3.printNodes();
    ann3.printGenome();
    ann6.printNodes();
    ann6.printGenome();
    ann.printNodes();
    ann.printGenome();

    std::cout << "COMPUTE" << std::endl;
    for (auto &o : ann.compute(std::deque<float>(3, 1.0f))) {
        std::cout << o << " ";
    }
    std::cout << std::endl;

    // TODO: mutation
    // TODO: survivor selection
}
