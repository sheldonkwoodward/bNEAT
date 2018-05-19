//
// Created by Sheldon Woodward on 4/27/18.
//

#include "ANN.hpp"

// constructor
ANN::ANN(int inputNum, int outputNum, std::string species) {
    this->nodes = std::deque<Node>();
    this->genome = std::deque<ConnectionGene>();

    this->inputNodes = std::deque<Node*>();

    this->outputNodes = std::deque<Node*>();
    this->sortedNodes = std::deque<Node*>();
    this->nonInputSortedNodes = std::deque<Node*>();

    this->weightMatrix = std::deque<std::deque<float>>();
    this->inputVector = std::deque<float*>();

    this->species = std::move(species);
    this->layerCount = 1;

    // add inputs
    for (int i = 0; i < inputNum; i++) {
        nodes.emplace_back((int)nodes.size());
        inputNodes.push_back(&nodes.back());
    }

    // add outputs
    for (int o = 0; o < outputNum; o++) {
        nodes.emplace_back((int)nodes.size());
        outputNodes.push_back(&nodes.back());
    }

    // add input to output connections
    for (auto in : inputNodes) {
        for (auto on : outputNodes) {
            genome.emplace_back(in, on, randomWeight(), true, genome.size());
        }
    }

    // calculate layer values
    sortNodes();
    determineLayers();
    determineWeightMatrix();
}

// set get
std::deque<ConnectionGene> ANN::getGenome() {
    return genome;
}

std::string ANN::getSpecies() {
    return species;
}

// computation functions
void ANN::determineWeightMatrix() {
    weightMatrix = std::deque<std::deque<float>>();
    for (int row = 0; row < nodes.size(); row++) {
        weightMatrix.emplace_back(nodes.size(), 0.0f);
    }

    // build weight matrix
    auto sortGenome = getEnabledSortedGenome();
    for (auto &cg : sortGenome) {
        weightMatrix[cg->getTo()->getNodeNum()][cg->getFrom()->getNodeNum()] = cg->getWeight();
    }
}

void ANN::determineLayers() {
    for (auto &on : outputNodes) {
        determineLayers(on, 0);
    }
}

void ANN::determineLayers(Node* node, unsigned int layer) {
    if (node->getLayer() <= layer && std::find(outputNodes.begin(), outputNodes.end(), node) == outputNodes.end()) {
        node->setLayer(layer + 1);
        if (layerCount <= layer + 1) {
            layerCount = layer + 1;
        }
    }
    for (auto &cg : genome) {
        if (cg.getTo() == node) {
            cg.setLayer(node->getLayer());
            determineLayers(cg.getFrom(), node->getLayer());
        }
    }
    if (std::find(inputNodes.begin(), inputNodes.end(), node) != inputNodes.end()) {
        node->setLayer(layerCount);
    }
}

std::deque<float> ANN::compute(std::deque<float> inputs) {
    // set inputs
    if (inputs.size() != inputNodes.size()) return std::deque<float>(4, 0.0f);
    for (int i = 0; i < inputs.size(); i++) inputNodes[i]->setValue(inputs[i]);

    // set inputVector
    inputVector = std::deque<float*>();
    for (auto &node : getSequentialNodes()) inputVector.push_back(node->getValuePtr());

//    std::cout << "nonInputSortedNodes" << std::endl;
//    for (auto &node : nonInputSortedNodes) {
//        std::cout << node->getNodeNum() << std::endl;
//    }
//    std::cout << "getSortedNodes" << std::endl;
//    for (auto &node : getSortedNodes(true)) {
//        std::cout << node->getNodeNum() << std::endl;
//    }
//
//    int x;
//    std::cin >> x;

    // feed network
    for (auto &node : nonInputSortedNodes) {
        float* currentInput = inputVector[node->getNodeNum()];
        *currentInput = 0.0;
        // TODO: could potentially be optimized, iterates over all nodes
        for (int step = 0; step < nodes.size(); step++) {
            *currentInput += weightMatrix[node->getNodeNum()][step] * *inputVector[step];
        }

        // activation function
        if (*currentInput < 0.0f) *currentInput = *currentInput / 100.0f;

    }

    // gather outputs
    std::deque<float> outputs = std::deque<float>();
    for (auto &node : outputNodes) outputs.push_back(std::max(node->getValue(), 0.0f)); // output activation
    return outputs;
}

// general functions
void ANN::addNodeMutation() {
    std::deque<ConnectionGene*> enabledConnections = getEnabledSortedGenome();
    ConnectionGene* randomConnection = enabledConnections.at(rand() % enabledConnections.size());
    // TODO: could create duplicate layers
    nodes.emplace_back((int)nodes.size());
    // TODO: check if innovation exists
    genome.emplace_back(randomConnection->getFrom(), &nodes.back(), randomWeight(), true, genome.size());
    genome.emplace_back(&nodes.back(), randomConnection->getTo(), randomWeight(), true, genome.size());
    randomConnection->setEnabled(false);
    sortNodes();
    determineLayers();
    determineWeightMatrix();
}

// TODO: check that good connections are produced
void ANN::addConnectionMutation() {
    std::deque<ConnectionGene> possibleConnections = std::deque<ConnectionGene>();
    for (unsigned long n1 = 0; n1 < nodes.size(); n1++) {
        for (unsigned long n0 = 0; n0 < nodes.size(); n0++) {
            if (nodes[n0].getLayer() > nodes[n1].getLayer() && findConnection(&nodes.at(n0), &nodes.at(n1)) == nullptr) {
                // TODO: check if innovation exists
                possibleConnections.emplace_back(&nodes.at(n0), &nodes.at(n1), randomWeight(), true, genome.size());
            }
        }
    }
    if (possibleConnections.empty()) {
        return;
    }
    genome.push_back(possibleConnections[rand() % possibleConnections.size()]);
    sortNodes();
    determineLayers();
    determineWeightMatrix();
}

float ANN::randomWeight() {
    return (float)(rand() % 1000) / 1000.0f;
//    return 1.0f;
}

ConnectionGene* ANN::findConnection(Node *from, Node *to) {
    for (unsigned long cg = 0; cg < genome.size(); cg++) {
        if (genome[cg].getFrom() == from && genome[cg].getTo() == to) return &genome.at(cg);
    }
    return nullptr;
}

// TODO: sort actual genome
std::deque<ConnectionGene*> ANN::getEnabledSortedGenome() {
    std::deque<ConnectionGene*> enabledConnections = std::deque<ConnectionGene*>();
    for (unsigned long cg = 0; cg < genome.size(); cg++) {
        if (genome[cg].getEnabled()) {
            enabledConnections.push_back(&genome.at(cg));
        }
    }
    std::sort(enabledConnections.begin(), enabledConnections.end(), ConnectionGene::ptrComparison);

    return enabledConnections;
}

void ANN::sortNodes() {
    sortedNodes = std::deque<Node*>();
    nonInputSortedNodes = std::deque<Node*>();
    for (Node &node : nodes) {
        sortedNodes.push_back(&node);
        if (std::find(inputNodes.begin(), inputNodes.end(), &node) == inputNodes.end()) {
            nonInputSortedNodes.push_back(&node);
        }
    }
    std::sort(sortedNodes.begin(), sortedNodes.end(), Node::ptrLayerSort);
    std::sort(nonInputSortedNodes.begin(), nonInputSortedNodes.end(), Node::ptrLayerSort);
}

std::deque<Node*> ANN::getSequentialNodes() {
    std::deque<Node*> sortedNodes = std::deque<Node*>();
    for (auto &node : nodes) sortedNodes.push_back(&node);
    std::sort(sortedNodes.begin(), sortedNodes.end(), Node::nodeNumSort);
    return sortedNodes;
}
