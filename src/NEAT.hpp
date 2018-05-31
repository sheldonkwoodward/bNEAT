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
    static const unsigned int POP_SIZE;  // max population size
    static const float MUT_RATE;  // rate at which mutations occur
    static const float SPEC_RATE;  // rate at which normal NEAT speciation occurs
    static const float SPEC_THRESH;  // max compatibility between two ANNs to be considered in the same species
    static const float COMP_C0;  // compatiblity constant modifier for excess innovations
    static const float COMP_C1;  // compatiblity constant modifier for disjoint innovations
    static const float COMP_C2;  // compatiblity constant modifier for average weight difference sum
    static const std::string PS_ALG;  // algorithm used for parent selection
    static const std::string SS_ALG;  // algorithm used for survivor selection

    // genetic algorithm
    void train();
    void populate();
    void parentSelection(); // define multiple algorithms for parents selection
    void crossover();
//    ANN crossover(ANN &ann1, ANN &ann2, float mutationRate, float speciationRate, float speciationThreshold);
    void survivorSelection();
    void addToSpecies(ANN &ann);
    void removeFromSpecies(ANN &ann);
};


#endif //NEAT_NEAT_HPP
