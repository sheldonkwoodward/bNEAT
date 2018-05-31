//
// Created by Sheldon Woodward on 5/12/18.
//

#ifndef NEAT_NEAT_HPP
#define NEAT_NEAT_HPP

#import <deque>
#include <vector>
#include <map>
#import <string>
#include <cstdlib>

#import "Snake.hpp"


class NEAT {
    Snake snake;
    std::deque<ANN> population;
    std::deque<std::pair<ANN*, ANN*>> parents;
    std::map<std::string, std::vector<ANN*>> species;
    unsigned int generationCount;

public:
    NEAT(int sizeX, int sizeY, unsigned int seed);

    // constants
    static const unsigned int POP_SIZE;
    static const float MUT_RATE;
    static const int CONN_MUT_RATIO;
    static const int NODE_MUT_RATIO;
    static const int WEIGHT_MUT_RATIO;
    static const float SPEC_RATE;
    static const float SPEC_THRESH;
    static const float COMP_C0;
    static const float COMP_C1;
    static const float COMP_C2;
    static const std::string PS_ALG;
    static const std::string SS_ALG;

    // genetic algorithm
    void train();
    void populate();
    void parentSelection(); // define multiple algorithms for parents selection
    void crossover();
//    ANN crossover(ANN &ann1, ANN &ann2, float mutationRate, float speciationRate, float speciationThreshold);
    void survivorSelection();
    void addToSpecies(ANN &ann);
    void removeFromSpecies(ANN &ann);
    void printGenerationInfo();
};


#endif //NEAT_NEAT_HPP
