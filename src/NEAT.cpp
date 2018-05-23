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
    ann2.connectionMutation();

    ann1.setFitness(1.0f);
    ann2.setFitness(2.0f);
    crossover(ann1, ann2);

    // TODO: mutation
    // TODO: survivor selection
}

ANN NEAT::crossover(ANN &ann1, ANN &ann2) {
    std::cout << "ANN 1" << std::endl;
    for (auto g : ann1.getGenes()) {
        std::cout << g.from << " -> " << g.to << std::endl;
    }
    std::cout << "ANN 2" << std::endl;
    for (auto g : ann2.getGenes()) {
        std::cout << g.from << " -> " << g.to << std::endl;
    }

    //    std::cout << "ANN 1 " << std::endl;
//    int accum = 0;
//    for (auto pair : ann1.getGenomeGenes()) {
//        std::cout << accum++ << ": ";
//        if (pair != nullptr) std::cout << pair->from << " -> " << pair->to << std::endl;
//        else std::cout << std::endl;
//    }
//
//    std::cout << "ANN 2" << std::endl;
//    accum = 0;
//    for (auto pair : ann2.getGenomeGenes()) {
//        std::cout << accum++ << ": ";
//        if (pair != nullptr) std::cout << pair->from << " -> " << pair->to << std::endl;
//        else std::cout << std::endl;
//    }


    return ANN(0, 0, "");
}
