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
//    std::deque<ANN> anns = std::deque<ANN>();
//    for (int i = 0; i < 30; i++) {
//        anns.emplace_back(3, 1, "");
//        anns.back().setFitness((float)(rand() % 3));
//    }
//    for (int i = 0; i < 500; i++) {
//        unsigned long someRand = rand() % anns.size();
//        anns[someRand] = ANN(anns[rand() % anns.size()], anns[rand() % anns.size()]);
//        anns[someRand].connectionMutation();
//        anns[someRand].nodeMutation();
//        anns[someRand].setFitness((float)(rand() % 3));
//        if (i % 100 == 0) std::cout << i << std::endl;
//    }
//
//    anns[rand() % anns.size()].printNodes();

    ANN ann1 = ANN(3, 1, "");
    ANN ann2 = ANN(3, 1, "");

    for (int i = 0 ; i < 5; i++) {
        ann1.connectionMutation();
        ann1.nodeMutation();
    }
    for (int i = 0 ; i < 5; i++) {
        ann2.connectionMutation();
        ann2.nodeMutation();
    }

    ann1.setFitness(2.0f);
    ann2.setFitness(3.0f);

    ANN ann3 = ANN(ann1, ann2);

    ann1.printGenome(true);
    ann2.printGenome(true);
    ann3.printGenome(true);

    // TODO: mutation
    // TODO: survivor selection
}
