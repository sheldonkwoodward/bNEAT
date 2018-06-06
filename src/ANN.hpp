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
#include <cmath>

#include "Node.hpp"
#include "ConnectionGene.hpp"
#include "Gene.h"


class ANN {
    // topology
    std::deque<Node> nodes;
    std::deque<ConnectionGene> genome;
    static std::vector<Gene> innovations;
    unsigned int layerCount;

    // node lists
    std::deque<Node*> inputNodes;
    std::deque<Node*> outputNodes;
    std::deque<Node*> layerSortedNodes;
    std::deque<Node*> nonInputLayerSortedNodes;
    std::deque<Node*> numberSortedNodes;

    // genome lists
    std::deque<ConnectionGene*> innovationSortedGenome;
    std::deque<ConnectionGene*> enabledInnovationSortedGenome;

    // computation structures
    std::deque<std::deque<float>> weightMatrix;
    std::deque<float*> inputVector;

    // other attributes
    static unsigned long idCount;
    unsigned long id;
    std::string species;
    float fitness;
    unsigned int age;
    std::string log;

public:
    // constructor
    ANN(unsigned long inputNum, unsigned long outputNum);
    ANN(unsigned long inputNum, unsigned long outputNum, std::string species);

public:
    // crossover
    ANN(ANN &ann1, ANN &ann2);
    Node* findOrCreateNode(int node);
    static float compatibility(ANN &ann1, ANN &ann2, float C0, float C1, float C2);

public:
    // set get
    std::string getSpecies();
    void setSpecies(std::string species);
    float getFitness();
    void setFitness(float fitness);
    unsigned int getAge();
    void incrementAge();
    unsigned long getId();
    std::string getLog();

public:
    // logging
    void addLog(std::string log);
    void resetLog();

public:
    // sort
    static bool fitnessSort(ANN &ann1, ANN &ann2);
    static bool ageSort(ANN &ann1, ANN &ann2);

private:
    // setup
    void setup();
    void sortNodes();
    void sortGenome();
    void determineLayers();
    void determineLayers(Node* node, unsigned int layer, std::deque<int> &stack);
    void determineWeightMatrix();

public:
    // mutations
    void weightMutation();
    void nodeMutation();
    void connectionMutation();
    static float randomWeight();
    bool connectionExists(Node* from, Node* to);

public:
    // computation
    std::deque<float> compute(std::deque<float> inputs);

private:
    // activation
    void hiddenActivation(float &value);
    void outputActivation(float &value);

public:
    // info dump
    void printNodes();
    void printGenome();
    void printGenome(bool showDisabled);
    void dumpTopology(std::string file);
    void dumpTrainLog(std::string file);
};


#endif //NEAT_ANN_HPP
