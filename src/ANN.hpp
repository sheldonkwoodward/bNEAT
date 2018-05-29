//
// Created by Sheldon Woodward on 4/27/18.
//

#ifndef NEAT_ANN_HPP
#define NEAT_ANN_HPP

#include <deque>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <algorithm>

#include "Node.hpp"
#include "ConnectionGene.hpp"
#include "Gene.h"


class ANN {
    // topology
    std::deque<Node> nodes;
    std::deque<ConnectionGene> genome;
    static std::vector<Gene> innovations;

    // pointer structures
    std::deque<Node*> inputNodes;
    std::deque<Node*> outputNodes;
    std::deque<Node*> layerSortedNodes;
    std::deque<Node*> nonInputLayerSortedNodes;
    std::deque<Node*> sequentialSortedNodes;
    std::deque<ConnectionGene*> innovationSortedGenome;
    std::deque<ConnectionGene*> enabledSortedGenome;

    // computation structures
    std::deque<std::deque<float>> weightMatrix;
    std::deque<float*> inputVector;

    // other attributes
    std::string species;
    unsigned int layerCount;
    float fitness;

public:
    // constructor
    ANN(unsigned long inputNum, unsigned long outputNum, std::string species);

    // crossover
    ANN(ANN &ann1, ANN &ann2);
    Node* findOrCreateNode(int node);

    // set get
    std::string getSpecies();
    float getFitness();
    void setFitness(float fitness);

    // setup
    void setup();
    void sortNodes();
    void sortGenome();
    void determineLayers();
    void determineLayers(Node* node, unsigned int layer, std::deque<int> &stack);
    void determineWeightMatrix();

    // mutations
    void weightMutation();
    void nodeMutation();
    void connectionMutation();

    // computation
    std::deque<float> compute(std::deque<float> inputs);

    // activation
    void hiddenActivation(float &value);
    void outputActivation(float &value);

    // other
    float randomWeight();
    bool connectionExists(Node* from, Node* to);
    void dumpTopology(std::string file);
    void printNodes();
    void printGenome();
    void printGenome(bool showDisabled);
};


#endif //NEAT_ANN_HPP
