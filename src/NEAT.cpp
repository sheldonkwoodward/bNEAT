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
    std::deque<ANN> anns = std::deque<ANN>();
    for (int i = 0; i < 100; i++) {
        anns.emplace_back(100, 4, "");
        anns.back().setFitness((float)(rand() % 10));
    }
    for (int i = 0; i < 10000; i++) {
        anns.push_back(crossover(anns[rand() % anns.size()], anns[rand() % anns.size()], mutationRate));
        anns.back().setFitness((float)(rand() % 10));
        anns.pop_front();
        if (i % 100 == 0) std::cout << i << std::endl;
    }
    anns.back().printGenome();
    std::cout << "OUTPUTS" << std::endl;
    for (auto &o : anns.back().compute(std::deque<float>(100, 1.0f))) std::cout << o << std::endl;

    // TODO: mutation
    // TODO: survivor selection
}

ANN NEAT::crossover(ANN &ann1, ANN &ann2, float mutationRate) {
    ANN newANN = ANN(ann1, ann2);
    if ((float)(rand() % 1000) / 1000.0f < mutationRate) newANN.weightMutation();
    if ((float)(rand() % 1000) / 1000.0f < mutationRate) newANN.connectionMutation();
    if ((float)(rand() % 1000) / 1000.0f < mutationRate) newANN.nodeMutation();
    return newANN;
}
