//
// Created by Sheldon Woodward on 5/12/18.
//

#ifndef NEAT_NEAT_HPP
#define NEAT_NEAT_HPP

#import <deque>
#import <string>
#include <cstdlib>

#import "Snake.hpp"


class NEAT {
    Snake snake;
    std::deque<ANN> population;
    std::deque<std::pair<ANN*, ANN*>> parents;

public:
    NEAT(int sizeX, int sizeY, unsigned int seed);

    void train(float mutationRate, std::string fitness, std::string parentSelectionAlg, std::string survivorSelectionAlg);
    void populate(unsigned int populationSize);
    void parentSelection(std::string algorithm); // define multiple algorithms for parents selection
    void crossover(float mutationRate);
    ANN crossover(ANN &ann1, ANN &ann2, float mutationRate);
    void survivorSelection(std::string algorithm);
};


#endif //NEAT_NEAT_HPP
