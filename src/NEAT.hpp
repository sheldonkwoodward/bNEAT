//
// Created by Sheldon Woodward on 5/12/18.
//

#ifndef NEAT_NEAT_HPP
#define NEAT_NEAT_HPP

#include <algorithm>
#include <cstdlib>
#import <deque>
#include <list>
#include <map>
#import <string>
#include <vector>

#import "SnakeGame.hpp"


class NEAT {
    SnakeGame snake;
    std::list<ANN> population;
    std::deque<std::pair<ANN*, ANN*>> parents;
    std::map<std::string, std::vector<ANN*>> species;
    unsigned int generationCount;

public:
    NEAT(int sizeX, int sizeY, unsigned int seed);

public:
    // genetic algorithm parameters
    unsigned int POP_SIZE = 100; // population
    float MUT_RATE = 0.05f;  // rate at which mutations occur
    int CONN_MUT_RATIO = 1;  // connection mutation ratio
    int NODE_MUT_RATIO = 1;  // node mutation ratio
    int WEIGHT_MUT_RATIO = 1;  // weight mutation ratio
    float SPEC_RATE = 0.05f;  // rate at which normal NEAT speciation occurs
    float SPEC_THRESH = 1.0f;  // max compatibility between two ANNs to be considered in the same species
    float COMP_C0 = 750.0f;  // compatibility constant modifier for excess innovations
    float COMP_C1 = 750.0f;  // compatibility constant modifier for disjoint innovations
    float COMP_C2 = 700.0f;  // compatibility constant modifier for average weight difference sum
    std::string PS_ALG = "RWS";  // algorithm used for parent selection - RWS, TS, RAND
    int TS_K = 3;  // the number of participants in each tournament for TS selection
    std::string SS_ALG = "FBS";  // algorithm used for survivor selection - ABS, FBS

public:
    // train
    void train();

private:
    // genetic algorithm
    void populate();
    void parentSelection();
    void crossover();
    void survivorSelection();

private:
    // speciation
    void addToSpecies(ANN &ann);
    void removeFromSpecies(ANN &ann);

public:
    // info dump
    void printGenerationInfo();
};


#endif //NEAT_NEAT_HPP
