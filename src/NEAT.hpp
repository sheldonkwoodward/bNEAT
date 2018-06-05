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
#include <algorithm>
#include <list>

#import "SnakeGame.hpp"


class NEAT {
    SnakeGame snake;
    std::list<ANN> population;
    std::deque<std::pair<ANN*, ANN*>> parents;
    std::map<std::string, std::vector<ANN*>> species;
    unsigned int generationCount;

public:
    NEAT(int sizeX, int sizeY, unsigned int seed);

    // constants
    unsigned int POP_SIZE = 100; // population
    // TODO: implement percent population replacement
    float MUT_RATE = 0.05f;  // rate at which mutations occur
    int CONN_MUT_RATIO = 1;  // connection mutation ratio
    int NODE_MUT_RATIO = 1;  // node mutation ratio
    int WEIGHT_MUT_RATIO = 1;  // weight mutation ratio
    float SPEC_RATE = 0.05f;  // rate at which normal NEAT speciation occurs
    float SPEC_THRESH = 1.0f;  // max compatibility between two ANNs to be considered in the same species
    float COMP_C0 = 750.0f;  // compatiblity constant modifier for excess innovations
    float COMP_C1 = 750.0f;  // compatiblity constant modifier for disjoint innovations
    float COMP_C2 = 700.0f;  // compatiblity constant modifier for average weight difference sum
    // TODO: implement rank selection
    std::string PS_ALG = "RWS";  // algorithm used for parent selection - RWS, TS, RS, RAND
    int TS_K = 3;  // the number of participants in each tournament for TS selection
    std::string SS_ALG = "FBS";  // algorithm used for survivor selection - ABS, FBS

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
