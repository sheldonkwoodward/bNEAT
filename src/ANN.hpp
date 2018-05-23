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
    // topology
    std::deque<Node> nodes;
    std::deque<ConnectionGene> genome;
    std::deque<std::pair<unsigned int, unsigned int>*> genomePairs;

    // pointer structures
    std::deque<Node*> inputNodes;
    std::deque<Node*> outputNodes;
    std::deque<Node*> layerSortedNodes;
    std::deque<Node*> nonInputLayerSortedNodes;
    std::deque<Node*> sequentialSortedNodes;
    std::deque<ConnectionGene*> enabledSortedGenome;

    // computation structures
    std::deque<std::deque<float>> weightMatrix;
    std::deque<float*> inputVector;

    // other attributes
    std::string species;
    unsigned int layerCount;
    float fitness;

public:
    // constructors
    ANN(int inputNum, int outputNum, std::string species);

    // set get
    std::deque<std::pair<unsigned int, unsigned int>*> getGenomePairs();
    std::string getSpecies();
    float getFitness();
    void setFitness(float fitness);

    // setup
    void setup();
    void sortNodes();
    void sortGenome();
    void determineLayers();
    void determineLayers(Node* node, unsigned int layer);
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
    void addGenomePair(std::pair<unsigned int, unsigned int>* pair);
};


#endif //NEAT_ANN_HPP
