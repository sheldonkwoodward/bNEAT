//
// Created by Sheldon Woodward on 4/27/18.
//

#include <iostream>
#include "ANN.hpp"

ANN::ANN() {
    this->nodes = std::deque<Node>();
    this->genome = std::deque<ConnectionGene>();
}

void ANN::determineInputOutput() {
    // clear existing inputs and outputs
    this->inputNodes = std::deque<Node*>();
    this->outputNodes = std::deque<Node*>();

    // check all nodes
    for (auto &node : this->nodes) {
        bool isInput = true;
        bool isOutput = true;
        // check against genome
        for (auto &g : this->genome) {
            if (&node == g.getTo()) isInput = false;
            else if (&node == g.getFrom()) isOutput = false;
        }
        // add to input or output list
        if (isInput) inputNodes.push_back(&node);
        else if (isOutput) outputNodes.push_back(&node);
    }
}

std::deque<float> ANN::compute(std::deque<float> inputs) {
    // set inputs
    if (inputs.size() != inputNodes.size()) return std::deque<float>();
    for (int i = 0; i < inputs.size(); i++) inputNodes[i]->setValue(inputs[i]);

    // create weight matrix
    std::deque<std::deque<float>> weights = std::deque<std::deque<float>>();
    for (int row = 0; row < nodes.size() - inputNodes.size(); row++) {
        weights.emplace_back();
        for (int col = 0; col < nodes.size(); col++) {
            weights[row].push_back(0.0);
        }
    }
    for (auto cg : genome) {
        weights[cg.getTo()->getNodeNum() - inputNodes.size()][cg.getFrom()->getNodeNum()] = cg.getWeight();
    }

    // create input vector
    std::deque<float*> inputVector = std::deque<float*>();
    for (int node = 0; node < nodes.size(); node++) {
        inputVector.push_back(nodes[node].getValuePtr());
    }

    // feed network
    for (int weightRow = 0; weightRow < weights.size(); weightRow++) {
        float* currentInput = inputVector[weightRow + inputNodes.size()];
        *currentInput = 0.0;
        for (int step = 0; step < nodes.size(); step++) {
            *currentInput += weights[weightRow][step] * *inputVector[step];
        }
        if (*currentInput < 0) *currentInput = static_cast<float>(*currentInput / 100.0);
    }

    std::deque<float> outputs = std::deque<float>();
    for (auto node : outputNodes) {
        std::cout << node->getValue() << " ";
        outputs.push_back(node->getValue());
    }
    std::cout << std::endl;

    return outputs;
}

Node* ANN::addNode(Node node) {
    this->nodes.push_back(node);
    return &this->nodes.back();
}

ConnectionGene* ANN::addConnectionGene(ConnectionGene connectionGene) {
    this->genome.push_back(connectionGene);
    determineInputOutput();
    return &this->genome.back();
}
