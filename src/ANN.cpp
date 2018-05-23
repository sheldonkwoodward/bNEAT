//
// Created by Sheldon Woodward on 4/27/18.
//

#include <algorithm>
#include "ANN.hpp"

// constructor
ANN::ANN(int inputNum, int outputNum, std::string species) {
    this->nodes = std::deque<Node>();
    this->genome = std::deque<ConnectionGene>();

    this->inputNodes = std::deque<Node*>();
    this->outputNodes = std::deque<Node*>();
    this->layerSortedNodes = std::deque<Node*>();
    this->nonInputLayerSortedNodes = std::deque<Node*>();
    this->sequentialSortedNodes = std::deque<Node*>();
    this->enabledSortedGenome = std::deque<ConnectionGene*>();

    this->weightMatrix = std::deque<std::deque<float>>();
    this->inputVector = std::deque<float*>();

    this->species = std::move(species);
    this->layerCount = 1;
    this->fitness = NULL;

    // add inputs and outputs
    for (int i = 0; i < inputNum; i++) {
        nodes.emplace_back((int)nodes.size());
        inputNodes.push_back(&nodes.back());
    }
    for (int o = 0; o < outputNum; o++) {
        nodes.emplace_back((int)nodes.size());
        outputNodes.push_back(&nodes.back());
    }
    setup();
}

// set get
std::string ANN::getSpecies() {
    return species;
}

float ANN::getFitness() {
    return fitness;
}

void ANN::setFitness(float fitness) {
    this->fitness = fitness;
}

std::deque<Gene> ANN::getGenes() {
    std::deque<Gene> genes = std::deque<Gene>();
    auto cg = genome.begin();
    for (int i = 0; i < ConnectionGene::getInnovationCount(); i++) {
        if (cg->getInnovation() != i) {
            genes.emplace_back(0, 0, false, false);
        } else {
            genes.emplace_back(cg->getFrom()->getNodeNum(), cg->getTo()->getNodeNum(), cg->getEnabled());
            cg++;
        }
    }
    return genes;
}

// setup functions
void ANN::setup() {
    sortNodes();
    determineLayers();
    sortGenome();
    determineWeightMatrix();
}

void ANN::sortNodes() {
    layerSortedNodes.clear();
    nonInputLayerSortedNodes.clear();
    sequentialSortedNodes.clear();
    for (auto &node : nodes) {
        layerSortedNodes.push_back(&node);
        sequentialSortedNodes.push_back(&node);
        if (std::find(inputNodes.begin(), inputNodes.end(), &node) == inputNodes.end()) {
            nonInputLayerSortedNodes.push_back(&node);
        }
    }
    std::sort(layerSortedNodes.begin(), layerSortedNodes.end(), Node::layerSort);
    std::sort(nonInputLayerSortedNodes.begin(), nonInputLayerSortedNodes.end(), Node::layerSort);
    std::sort(sequentialSortedNodes.begin(), sequentialSortedNodes.end(), Node::sequentialSort);
}

void ANN::sortGenome() {
    enabledSortedGenome.clear();
    for (auto &cg : genome) {
        if (cg.getEnabled()) {
            enabledSortedGenome.push_back(&cg);
        }
    }
    std::sort(enabledSortedGenome.begin(), enabledSortedGenome.end(), ConnectionGene::layerSort);
}

void ANN::determineLayers() {
    for (auto &on : outputNodes) {
        determineLayers(on, 0);
    }
    for (auto &node : inputNodes) {
        node->setLayer(layerCount);
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
}

void ANN::determineWeightMatrix() {
    weightMatrix.clear();
    for (int row = 0; row < nodes.size(); row++) {
        weightMatrix.emplace_back(nodes.size(), 0.0f);
    }

    // build weight matrix
    for (auto &cg : enabledSortedGenome) {
        weightMatrix[cg->getTo()->getNodeNum()][cg->getFrom()->getNodeNum()] = cg->getWeight();
    }
}

// mutations
void ANN::weightMutation() {
    ConnectionGene* randomConnection = &genome.at(rand() % genome.size());
    static const int OPERATION_NUM = 5;
    int operation = rand() % OPERATION_NUM;
    switch (operation) {
        // change to random weight
        case 0: {
            randomConnection->setWeight(randomWeight());
            break;
        }
        // change by 0.5% to 1.5%
        case 1: {
            randomConnection->setWeight(randomConnection->getWeight() * ((float) (rand() % 1000) / 1000.0f + 0.5f));
            break;
        }
        // add or subtract between 0 and 1
        case 2: {
            float randVal = ((float) (rand() % 2000 - 1000) / 1000.0f);
            randomConnection->setWeight(randomConnection->getWeight() + randVal);
            break;
        }
        // change sign of weight
        case 3: {
            randomConnection->setWeight(-randomConnection->getWeight());
            break;
        }
        // swap two weights
        case 4: {
            ConnectionGene *otherRandomConnection = &genome.at(rand() % genome.size());
            float savedWeight = randomConnection->getWeight();
            randomConnection->setWeight(otherRandomConnection->getWeight());
            otherRandomConnection->setWeight(savedWeight);
            break;
        }
        default: {
            break;
        }
    }
    setup();
}

void ANN::nodeMutation() {
    if (genome.empty()) return;
    ConnectionGene* randomConnection = enabledSortedGenome.at(rand() % enabledSortedGenome.size());
    nodes.emplace_back((int)nodes.size());
    genome.emplace_back(randomConnection->getFrom(), &nodes.back(), randomWeight());
    genome.emplace_back(&nodes.back(), randomConnection->getTo(), randomWeight());
    randomConnection->setEnabled(false);
    setup();
}

void ANN::connectionMutation() {
    std::deque<ConnectionGene> possibleConnections = std::deque<ConnectionGene>();
    for (unsigned long n1 = 0; n1 < nodes.size(); n1++) {
        for (unsigned long n0 = 0; n0 < nodes.size(); n0++) {
            if (nodes[n0].getLayer() > nodes[n1].getLayer() && !connectionExists(&nodes.at(n0), &nodes.at(n1))) {
                possibleConnections.emplace_back(&nodes.at(n0), &nodes.at(n1), randomWeight(), 0);
            }
        }
    }
    if (possibleConnections.empty()) return;
    auto connection = possibleConnections[rand() % possibleConnections.size()];
    genome.emplace_back(connection.getFrom(), connection.getTo(), connection.getWeight());
    setup();
}

// computation
std::deque<float> ANN::compute(std::deque<float> inputs) {
    // set inputs and outputs
    if (inputs.size() != inputNodes.size()) return std::deque<float>(outputNodes.size(), 0.0f);
    for (int i = 0; i < inputs.size(); i++) inputNodes[i]->setValue(inputs[i]);
    std::deque<float> outputs = std::deque<float>();

    // set inputVector
    inputVector.clear();
    for (auto &node : sequentialSortedNodes) inputVector.push_back(node->getValuePtr());

    // feed network
    for (auto &node : nonInputLayerSortedNodes) {
        float* currentInput = inputVector[node->getNodeNum()];
        *currentInput = 0.0;
        for (int n = 0; n < nodes.size(); n++) {
            *currentInput += weightMatrix[node->getNodeNum()][n] **inputVector[n];
        }
        if (node->getLayer() != 0) hiddenActivation(*currentInput);
        else {
            outputActivation(*node->getValuePtr());
            outputs.push_back(node->getValue());
        }
    }

    // gather outputs
    outputs.clear();
    for (auto &node : outputNodes) outputs.push_back(node->getValue());
    return outputs;
}

// activation
void ANN::hiddenActivation(float &value) {
    // leaky ReLU activation
    if (value < 0.0f) value = value / 100.0f;
}

void ANN::outputActivation(float &value) {
    // identity activation
}

// other
float ANN::randomWeight() {
    return (float)(rand() % 2000 - 1000) / 1000.0f;
}

bool ANN::connectionExists(Node* from, Node* to) {
    for (auto &cg : genome)
        if (cg.getFrom() == from && cg.getTo() == to) return true;
    return false;
}
