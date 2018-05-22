//
// Created by Sheldon Woodward on 5/12/18.
//

#include "NEAT.hpp"

NEAT::NEAT(int sizeX, int sizeY, unsigned int seed) : snake(sizeX, sizeY) {
    srand(seed);
}

void NEAT::train(float mutationRate, std::string fitness, std::string parentSelection, std::string survivorSelection) {
    // TODO: populate
    for (int i = 0; i < 20; i++) {
        population.emplace_back(100,4,"AAA");
        snake.fitness(population.back(), false);
    }

    // TODO: parent selection
    // TODO: crossover
    // TODO: mutation
    // TODO: survivor selection
}

ANN NEAT::crossover(ANN* ann1, ANN* ann2) {
    return ANN(0, 0, "");
}
