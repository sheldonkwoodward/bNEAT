//
// Created by Sheldon Woodward on 5/12/18.
//

#ifndef NEAT_NEAT_HPP
#define NEAT_NEAT_HPP

#import <vector>
#import <string>

#import "Snake.hpp"


class NEAT {
    Snake snake;
    std::vector<ANN> population;

public:
    NEAT(int sizeX, int sizeY);

    void train(float mutationRate, std::string fitness, std::string parentSelection, std::string survivorSelection);
    void populate();
    void parentSelection(std::string algorithm); // define multiple algorithms for parents selection
    ANN crossover(ANN ann1, ANN ann2);
    ANN mutate(ANN ann);
    void survivorSelection(std::string algorithm);
};


#endif //NEAT_NEAT_HPP
