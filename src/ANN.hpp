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
    std::deque<Node> nodes;
    std::deque<ConnectionGene> genome;
    std::deque<Node*> inputNodes;
    std::deque<Node*> outputNodes;
    std::deque<std::deque<float>> weightMatrix;
    std::deque<float*> inputVector;
    std::string species;

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
    ConnectionGene* addConnectionGene(ConnectionGene connectionGene);


    // general functions
    void addNodeMutation();
//    void addConnectionMutation();
    float randomWeight();
    ConnectionGene* findConnection(Node* from, Node* to);
    std::deque<ConnectionGene*> getEnabledSortedGenome();
    std::deque<Node> getSortedNodes();
    std::deque<Node*> getSequentialNodes();
};


#endif //NEAT_ANN_HPP
