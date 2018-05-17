//
// Created by Sheldon Woodward on 4/27/18.
//

#include "ANN.hpp"

// constructor
ANN::ANN(int inputNum, int outputNum, std::string species) {
    this->nodes = std::deque<Node>();
    this->genome = std::deque<ConnectionGene>();
    this->species = std::move(species);
    this->inputNodes = std::deque<Node*>();
    this->outputNodes = std::deque<Node*>();

    // add input and output nodes
    for (int i = 0; i < inputNum; i++) {
        nodes.emplace_back((int)nodes.size());
        inputNodes.push_back(&nodes.back());
    }
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
    for (int row = 0; row < nodes.size() - inputNodes.size(); row++) {
        weightMatrix.emplace_back();
        for (int col = 0; col < nodes.size(); col++) {
            weightMatrix[row].push_back(0.0);
        }
    }

    // TODO: reorder input vector and weight matrix rows.
    // values are being calculated before their dependant values are calculated yielding 0 value incrementation
    for (auto &cg : getEnabledSortedGenome()) {
        weightMatrix[cg->getTo()->getNodeNum() - inputNodes.size()][cg->getFrom()->getNodeNum()] = cg->getWeight();
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
    }
    for (auto &cg : genome) {
        if (cg.getTo() == node) {
            cg.setLayer(node->getLayer());
            determineLayers(cg.getFrom(), node->getLayer());
        }
    }
}

std::deque<float> ANN::compute(std::deque<float> inputs) {
    // set inputs
    if (inputs.size() != inputNodes.size()) return std::deque<float>();
    for (int i = 0; i < inputs.size(); i++) inputNodes[i]->setValue(inputs[i]);

    // set inputVector
    inputVector = std::deque<float*>();
    for (auto &node : getSortedNodes()) inputVector.push_back(node.getValuePtr());

    // feed network
    for (int weightRow = 0; weightRow < weightMatrix.size(); weightRow++) {
        float* currentInput = inputVector[weightRow + inputNodes.size()];
        *currentInput = 0.0;
        for (int step = 0; step < nodes.size(); step++) {
            *currentInput += weightMatrix[weightRow][step] * *inputVector[step];
        }
        if (*currentInput < 0) *currentInput = *currentInput / 100.0f; // activation function
    }

    // gather outputs
    std::deque<float> outputs = std::deque<float>();
    for (auto &node : outputNodes) outputs.push_back(node->getValue());
    return outputs;
}

ConnectionGene* ANN::addConnectionGene(ConnectionGene connectionGene) {
    genome.push_back(connectionGene);
    return &genome.back();
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
    determineLayers();
    determineWeightMatrix();
}
//
//void ANN::addConnectionMutation() {
//    std::deque<ConnectionGene> possibleConnections = std::deque<ConnectionGene>();
//    for (unsigned long n1 = 0; n1 < nodes.size(); n1++) {
//        for (unsigned long n0 = 0; n0 < nodes.size(); n0++) {
//            if (nodes[n0].getLayer() < nodes[n1].getLayer() && findConnection(&nodes.at(n0), &nodes.at(n1)) == nullptr) {
//                // TODO: implement innovation numbers
//                // TODO: check if innovation exists
//                possibleConnections.emplace_back(&nodes.at(n0), &nodes.at(n1), randomWeight());
//            }
//        }
//    }
//    if (possibleConnections.empty()) {
//        return;
//    }
//    genome.push_back(possibleConnections[rand() % possibleConnections.size()]);
//    determineLayers();
//    determineWeightMatrix();
//}

float ANN::randomWeight() {
//    return (float)(rand() % 1000) / 1000.0f;
    return 1.0f;
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

// TODO: sort actual nodes
std::deque<Node> ANN::getSortedNodes() {
    std::deque<Node> sortedNodes = nodes;
    std::sort(sortedNodes.begin(), sortedNodes.end(), Node::ptrComparison);
    return sortedNodes;
}
