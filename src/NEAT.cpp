//
// Created by Sheldon Woodward on 5/12/18.
//

#include "NEAT.hpp"

NEAT::NEAT(int sizeX, int sizeY, unsigned int seed) : snake(sizeX, sizeY) {
    srand(seed);
}

void NEAT::train(float mutationRate, std::string fitness, std::string parentSelection, std::string survivorSelection) {
    // populate
    // parent selection
    // crossover
    // mutation
    // survivor selection
}
