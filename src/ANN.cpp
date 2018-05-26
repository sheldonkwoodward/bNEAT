//
// Created by Sheldon Woodward on 4/27/18.
//

#include <algorithm>
#include "ANN.hpp"

// constructor
ANN::ANN(unsigned long inputNum, unsigned long outputNum, std::string species) {
    this->nodes = std::deque<Node>();
    this->genome = std::deque<ConnectionGene>();

    this->inputNodes = std::deque<Node*>();
    this->outputNodes = std::deque<Node*>();
    this->layerSortedNodes = std::deque<Node*>();
    this->nonInputLayerSortedNodes = std::deque<Node*>();
    this->sequentialSortedNodes = std::deque<Node*>();
    this->innovationSortedGenome = std::deque<ConnectionGene*>();
    this->enabledSortedGenome = std::deque<ConnectionGene*>();

    this->weightMatrix = std::deque<std::deque<float>>();
    this->inputVector = std::deque<float*>();

    this->species = std::move(species);
    this->layerCount = 1;
    this->fitness = 0.0f;

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

ANN::ANN(ANN &ann1, ANN &ann2) : ANN(ann1.inputNodes.size(), ann1.outputNodes.size(), "") {
    auto genomeItr1 = ann1.innovationSortedGenome.begin();
    auto genomeItr2 = ann2.innovationSortedGenome.begin();

    // same fitness
    while (genomeItr1 != ann1.innovationSortedGenome.end() && genomeItr2 != ann2.innovationSortedGenome.end()) {
        // matching innovation
        if ((*genomeItr1)->getInnovation() == (*genomeItr2)->getInnovation()) {
            // add connection to genome
            genome.emplace_back(findOrCreateNode((*genomeItr1)->getFrom()->getNodeNum()),
                                findOrCreateNode((*genomeItr1)->getTo()->getNodeNum()),
                                (*genomeItr1)->getWeight(),
                                (*genomeItr1)->getInnovation(),
                                (*genomeItr1)->getEnabled());
            // increment both iterators
            if (genomeItr1 != ann1.innovationSortedGenome.end()) ++genomeItr1;
            if (genomeItr2 != ann2.innovationSortedGenome.end()) ++genomeItr2;
        }
        // disjoint or excess innovation
        else {
            // determine ptr with smallest innovation
            auto smallerGenomePtr = &genomeItr1;
            ANN* smallerAnn = &ann1;
            if ((*genomeItr2)->getInnovation() < (*genomeItr1)->getInnovation()) {
                smallerGenomePtr = &genomeItr2;
                smallerAnn = &ann2;
            }
            // add smaller innovation to genome
            genome.emplace_back(findOrCreateNode((**smallerGenomePtr)->getFrom()->getNodeNum()),
                                findOrCreateNode((**smallerGenomePtr)->getTo()->getNodeNum()),
                                (**smallerGenomePtr)->getWeight(),
                                (**smallerGenomePtr)->getInnovation(),
                                (**smallerGenomePtr)->getEnabled());
            // increment iterator with smallest innovation
            if(*smallerGenomePtr != smallerAnn->innovationSortedGenome.end()) ++(*smallerGenomePtr);
        }
    }
    // excess genes
    while (genomeItr1 != ann1.innovationSortedGenome.end()) {
        genome.emplace_back(findOrCreateNode((*genomeItr1)->getFrom()->getNodeNum()),
                            findOrCreateNode((*genomeItr1)->getTo()->getNodeNum()),
                            (*genomeItr1)->getWeight(),
                            (*genomeItr1)->getInnovation(),
                            (*genomeItr1)->getEnabled());
        ++genomeItr1;
    }
    while (genomeItr2 != ann2.innovationSortedGenome.end()) {
        genome.emplace_back(findOrCreateNode((*genomeItr2)->getFrom()->getNodeNum()),
                            findOrCreateNode((*genomeItr2)->getTo()->getNodeNum()),
                            (*genomeItr2)->getWeight(),
                            (*genomeItr2)->getInnovation(),
                            (*genomeItr2)->getEnabled());
        ++genomeItr2;
    }
    for (auto node : nodes) {
        std::cout << node.getNodeNum() << std::endl;
    }
    for (auto gene : genome) {
        std::cout << gene.getFrom()->getNodeNum() << "\t-> " << gene.getTo()->getNodeNum() << std::endl;
    }
    setup();
}

std::vector<Gene> ANN::innovations = std::vector<Gene>();

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
    innovationSortedGenome.clear();
    enabledSortedGenome.clear();
    for (auto &cg : genome) {
        innovationSortedGenome.push_back(&cg);
        if (cg.getEnabled()) {
            enabledSortedGenome.push_back(&cg);
        }
    }
    std::sort(innovationSortedGenome.begin(), innovationSortedGenome.end(), ConnectionGene::innovationSortPtr);
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
    // find random connections
    ConnectionGene* randomConnection = enabledSortedGenome.at(rand() % enabledSortedGenome.size());
    randomConnection->setEnabled(false);
    // TODO: ensure nodeNum doesn't already exist
    nodes.emplace_back((int)nodes.size());

    // gene 1
    auto newGene = Gene(randomConnection->getFrom()->getNodeNum(), nodes.back().getNodeNum(), 0);
    auto match = std::lower_bound(innovations.begin(), innovations.end(), newGene, Gene::lessThan);
    if (innovations.empty() || *match != newGene) {
        genome.emplace_back(randomConnection->getFrom(), &nodes.back(), randomWeight(), innovations.size());
        innovations.emplace_back(randomConnection->getFrom()->getNodeNum(), nodes.back().getNodeNum(), innovations.size());
    } else {
        genome.emplace_back(randomConnection->getFrom(), &nodes.back(), randomWeight(), match->innovation);
    }
    // TODO: implement with only one sort
    std::sort(innovations.begin(), innovations.end(), Gene::sort);


    // gene 2
    newGene = Gene(nodes.back().getNodeNum(), randomConnection->getTo()->getNodeNum(), 0);
    match = std::lower_bound(innovations.begin(), innovations.end(), newGene, Gene::lessThan);
    if (innovations.empty() || *match != newGene) {
        genome.emplace_back(&nodes.back(), randomConnection->getTo(), randomWeight(), innovations.size());
        innovations.emplace_back(nodes.back().getNodeNum(), randomConnection->getTo()->getNodeNum(), innovations.size());
    } else {
        genome.emplace_back(&nodes.back(), randomConnection->getTo(), randomWeight(), match->innovation);
    }

    // sort and setup
    std::sort(innovations.begin(), innovations.end(), Gene::sort);
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

    auto newGene = Gene(connection.getFrom()->getNodeNum(), connection.getTo()->getNodeNum(), 0);
    auto match = std::lower_bound(innovations.begin(), innovations.end(), newGene, Gene::lessThan);
    if (innovations.empty() || *match != newGene) {
        genome.emplace_back(connection.getFrom(), connection.getTo(), randomWeight(), innovations.size());
        innovations.emplace_back(connection.getFrom()->getNodeNum(), connection.getTo()->getNodeNum(), innovations.size());
    } else {
        genome.emplace_back(connection.getFrom(), connection.getTo(), randomWeight(), match->innovation);
    }

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

Node* ANN::findOrCreateNode(int node) {
    for (int n = 0; n < nodes.size(); n++) {
        if (nodes[n].getNodeNum() == node) {
            return &nodes.at((unsigned long)n);
        }
    }
    nodes.emplace_back(node);
    return &nodes.back();
}

bool ANN::connectionExists(Node* from, Node* to) {
    for (auto &cg : genome)
        if (cg.getFrom() == from && cg.getTo() == to) return true;
    return false;
}

bool ANN::innovationExists(int innovation) {
    for (auto &gene : genome) {
        if (gene.getInnovation() == innovation) return true;
    }
    return false;
}
