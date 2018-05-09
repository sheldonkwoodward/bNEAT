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

void ANN::determineWeightMatrix() {
    weightMatrix = std::deque<std::deque<float>>();
    for (int row = 0; row < nodes.size() - inputNodes.size(); row++) {
        weightMatrix.emplace_back();
        for (int col = 0; col < nodes.size(); col++) {
            weightMatrix[row].push_back(0.0);
        }
    }
    for (auto cg : genome) {
        weightMatrix[cg.getTo()->getNodeNum() - inputNodes.size()][cg.getFrom()->getNodeNum()] = cg.getWeight();
    }
}

void ANN::setup() {
    determineInputOutput();
    determineWeightMatrix();
}

std::deque<float> ANN::compute(std::deque<float> inputs) {
    // set inputs
    if (inputs.size() != inputNodes.size()) return std::deque<float>();
    for (int i = 0; i < inputs.size(); i++) inputNodes[i]->setValue(inputs[i]);

    // set input vector
    inputVector = std::deque<float*>();
    for (auto &node : nodes) inputVector.push_back(node.getValuePtr());

    // feed network
    for (int weightRow = 0; weightRow < weightMatrix.size(); weightRow++) {
        float* currentInput = inputVector[weightRow + inputNodes.size()];
        *currentInput = 0.0;
        for (int step = 0; step < nodes.size(); step++) {
            *currentInput += weightMatrix[weightRow][step] * *inputVector[step];
        }
        if (*currentInput < 0) *currentInput = static_cast<float>(*currentInput / 100.0); // activation function
    }

    // gather outputs
    std::deque<float> outputs = std::deque<float>();
    for (auto node : outputNodes) outputs.push_back(node->getValue());
    return outputs;
}

Node* ANN::addNode(Node node) {
    this->nodes.push_back(node);
    return &this->nodes.back();
}

ConnectionGene* ANN::addConnectionGene(ConnectionGene connectionGene) {
    this->genome.push_back(connectionGene);
    return &this->genome.back();
}
