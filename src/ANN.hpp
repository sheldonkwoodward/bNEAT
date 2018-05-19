//
// Created by Sheldon Woodward on 4/27/18.
//

#ifndef NEAT_ANN_HPP
#define NEAT_ANN_HPP

#include <deque>
#include <string>
#include <iostream>
#include <cstdlib>
#include <limits>

#include "Node.hpp"
#include "ConnectionGene.hpp"


class ANN {
private:
    // topology
    std::deque<Node> nodes;
    std::deque<ConnectionGene> genome;

    // pointer structures
    std::deque<Node*> inputNodes;
    std::deque<Node*> outputNodes;
    std::deque<Node*> layerSortedNodes;
    std::deque<Node*> nonInputLayerSortedNodes;
    std::deque<ConnectionGene*> enabledSortedGenome;

    // computation structures
    std::deque<std::deque<float>> weightMatrix;
    std::deque<float*> inputVector;

    // other attributes
    std::string species;
    unsigned int layerCount;

public:
    // constructors
    ANN(int inputNum, int outputNum, std::string species);

    // set get
    std::deque<ConnectionGene> getGenome();
    std::string getSpecies();

    // computation functions
    void determineWeightMatrix();
    void determineLayers();
    void determineLayers(Node* node, unsigned int layer);
    std::deque<float> compute(std::deque<float> inputs);

    // mutation functions
    void addNodeMutation();
    void addConnectionMutation();

    // general functions
    float randomWeight();
    ConnectionGene* findConnection(Node* from, Node* to);
//    std::deque<ConnectionGene*> getEnabledSortedGenome();
    void sortNodes();
    void sortGenome();
    std::deque<Node*> getSequentialNodes();
};


#endif //NEAT_ANN_HPP
