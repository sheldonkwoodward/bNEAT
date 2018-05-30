//
// Created by Sheldon Woodward on 4/27/18.
//

#include "ANN.hpp"

// static initialization
std::vector<Gene> ANN::innovations = std::vector<Gene>();

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
    this->age = 0;

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

// crossover
ANN::ANN(ANN &ann1, ANN &ann2) : ANN(ann1.inputNodes.size(), ann1.outputNodes.size(), "") {
    auto genomeItr1 = ann1.innovationSortedGenome.begin();
    auto genomeItr2 = ann2.innovationSortedGenome.begin();

    // determine them most fit ANN
    ANN* moreFit = &ann1;
    ANN* lessFit = &ann2;
    if (moreFit->fitness < lessFit->fitness) std::swap(moreFit, lessFit);

    // non-excess genes
    while (genomeItr1 != ann1.innovationSortedGenome.end() && genomeItr2 != ann2.innovationSortedGenome.end()) {
        // matching innovation
        if ((*genomeItr1)->getInnovation() == (*genomeItr2)->getInnovation()) {
            // choose connection to inherit
            auto randomGeneItr = genomeItr1;
            if (ann1.fitness == ann2.fitness) {
                if (rand() % 2) randomGeneItr = genomeItr1;
                else randomGeneItr = genomeItr2;
            } else if (ann1.fitness < ann2.fitness) {
                randomGeneItr = genomeItr2;
            }
            // add connection to genome
            genome.emplace_back(findOrCreateNode((*randomGeneItr)->getFrom()->getNodeNum()),
                                findOrCreateNode((*randomGeneItr)->getTo()->getNodeNum()),
                                (*randomGeneItr)->getWeight(),
                                (*randomGeneItr)->getInnovation(),
                                (*randomGeneItr)->getEnabled());
            // increment both iterators
            if (genomeItr1 != ann1.innovationSortedGenome.end()) ++genomeItr1;
            if (genomeItr2 != ann2.innovationSortedGenome.end()) ++genomeItr2;
        }
        // disjoint innovation
        else {
            // determine ptr with smallest innovation
            auto smallerGenomePtr = &genomeItr1;
            ANN* smallerAnn = &ann1;
            if ((*genomeItr2)->getInnovation() < (*genomeItr1)->getInnovation()) {
                smallerGenomePtr = &genomeItr2;
                smallerAnn = &ann2;
            }
            // try skip if unequal fitness
            if (ann1.fitness == ann2.fitness || smallerAnn == moreFit) {
                // add smaller innovation to genome
                genome.emplace_back(findOrCreateNode((**smallerGenomePtr)->getFrom()->getNodeNum()),
                                    findOrCreateNode((**smallerGenomePtr)->getTo()->getNodeNum()),
                                    (**smallerGenomePtr)->getWeight(),
                                    (**smallerGenomePtr)->getInnovation(),
                                    (**smallerGenomePtr)->getEnabled());
            }
            // increment iterator with smallest innovation
            if(*smallerGenomePtr != smallerAnn->innovationSortedGenome.end()) ++(*smallerGenomePtr);
        }
    }
    // excess genes
    while (ann1.fitness >= ann2.fitness && genomeItr1 != ann1.innovationSortedGenome.end()) {
        genome.emplace_back(findOrCreateNode((*genomeItr1)->getFrom()->getNodeNum()),
                            findOrCreateNode((*genomeItr1)->getTo()->getNodeNum()),
                            (*genomeItr1)->getWeight(),
                            (*genomeItr1)->getInnovation(),
                            (*genomeItr1)->getEnabled());
        ++genomeItr1;
    }
    while (ann2.fitness >= ann1.fitness && genomeItr2 != ann2.innovationSortedGenome.end()) {
        genome.emplace_back(findOrCreateNode((*genomeItr2)->getFrom()->getNodeNum()),
                            findOrCreateNode((*genomeItr2)->getTo()->getNodeNum()),
                            (*genomeItr2)->getWeight(),
                            (*genomeItr2)->getInnovation(),
                            (*genomeItr2)->getEnabled());
        ++genomeItr2;
    }
    setup();
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

unsigned int ANN::getAge() {
    return age;
}

void ANN::incrementAge() {
    age++;
}

// sort
bool ANN::fitnessSort(ANN &ann1, ANN &ann2) {
        return ann1.fitness < ann2.fitness;
}

// setup functions
void ANN::setup() {
    determineLayers();
    sortNodes();
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
    std::deque<int> stack = std::deque<int>();
    for (auto &on : outputNodes) {
        determineLayers(on, 0, stack);
    }
    for (auto &node : inputNodes) {
        node->setLayer(layerCount);
    }
}

void ANN::determineLayers(Node* node, unsigned int layer, std::deque<int> &stack) {
    stack.push_back(node->getNodeNum());
    if (node->getLayer() <= layer && std::find(outputNodes.begin(), outputNodes.end(), node) == outputNodes.end()) {
        node->setLayer(layer + 1);
        if (layerCount <= layer + 1) {
            layerCount = layer + 1;
        }
    }
    for (auto cg = genome.begin(); cg != genome.end(); ++cg) {
        if (cg->getTo() == node && cg->getEnabled()) {
            cg->setLayer(node->getLayer());
            auto recurrentConnection = std::find(stack.begin(), stack.end(), cg->getFrom()->getNodeNum());
            if (recurrentConnection == stack.end()) {
                determineLayers(cg->getFrom(), node->getLayer(), stack);
            } else {
                // TODO: look into better way of removing recurrence
                cg->setEnabled(false);
            }
        }
    }
    stack.pop_back();
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
    if (genome.empty()) return;
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

    // new genes
    auto newGene1 = Gene(randomConnection->getFrom()->getNodeNum(), nodes.back().getNodeNum(), 0);
    auto match1 = std::lower_bound(innovations.begin(), innovations.end(), newGene1, Gene::sort);
    auto newGene2 = Gene(nodes.back().getNodeNum(), randomConnection->getTo()->getNodeNum(), 0);
    auto match2 = std::lower_bound(innovations.begin(), innovations.end(), newGene2, Gene::sort);

    // gene 1
    if (innovations.empty() || *match1 != newGene1) {
        genome.emplace_back(randomConnection->getFrom(), &nodes.back(), randomWeight(), innovations.size());
        innovations.emplace_back(randomConnection->getFrom()->getNodeNum(), nodes.back().getNodeNum(), innovations.size());
    } else {
        genome.emplace_back(randomConnection->getFrom(), &nodes.back(), randomWeight(), match1->innovation);
    }

    // gene 2
    if (innovations.empty() || *match2 != newGene2) {
        genome.emplace_back(&nodes.back(), randomConnection->getTo(), randomWeight(), innovations.size());
        innovations.emplace_back(nodes.back().getNodeNum(), randomConnection->getTo()->getNodeNum(), innovations.size());
    } else {
        genome.emplace_back(&nodes.back(), randomConnection->getTo(), randomWeight(), match2->innovation);
    }

    // sort and setup
    std::sort(innovations.begin(), innovations.end(), Gene::sort);
    setup();
}

void ANN::connectionMutation() {
    // find random connection
    unsigned long randFrom;
    unsigned long randTo;
    do {
        randFrom = rand() % (layerSortedNodes.size() - 1);
        randTo = rand() % (layerSortedNodes.size() - randFrom - 1) + randFrom + 1;
    } while(layerSortedNodes[randFrom]->getLayer() == layerSortedNodes[randTo]->getLayer() || connectionExists(layerSortedNodes[randFrom], layerSortedNodes[randTo]) );
    Gene newGene = Gene(layerSortedNodes[randFrom]->getNodeNum(), layerSortedNodes[randFrom]->getNodeNum(), 0);

    // add to genome
    auto match = std::lower_bound(innovations.begin(), innovations.end(), newGene, Gene::sort);
    if (innovations.empty() || *match != newGene) {
        genome.emplace_back(layerSortedNodes[randFrom], layerSortedNodes[randTo], randomWeight(), innovations.size());
        innovations.emplace_back(newGene.from, newGene.to, innovations.size());
    } else {
        genome.emplace_back(layerSortedNodes[randFrom], layerSortedNodes[randTo], randomWeight(), match->innovation);
    }

    // sort and setup
    std::sort(innovations.begin(), innovations.end(), Gene::sort);
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
    // TODO: binary search
    for (auto &cg : genome)
        if (cg.getFrom() == from && cg.getTo() == to) return true;
    return false;
}

void ANN::dumpTopology(std::string file) {
    std::ofstream dumpFile;
    dumpFile.open(file);
    for (auto node : nodes) {
        dumpFile << node.getNodeNum() << " " << node.getLayer() << "\n";
    }
    dumpFile << "###\n";
    for (auto cg : genome) {
        if (!cg.getEnabled()) continue;
        dumpFile << cg.getFrom()->getNodeNum() << " " << cg.getTo()->getNodeNum() << " " << cg.getWeight() << "\n";
    }
}

void ANN::printNodes() {
    std::cout << "NODES" << std::endl;
    for (auto &node : nodes) {
        std::cout << node.getNodeNum() << " " << node.getLayer() << std::endl;
    }
}

void ANN::printGenome() {
    printGenome(false);
}

void ANN::printGenome(bool showDisabled) {
    std::cout << "GENOME" << std::endl;
    for (auto &gene : genome) {
        if (showDisabled || gene.getEnabled()) {
            if (showDisabled && !gene.getEnabled()) {
                std::cout << "D ";
            } else {
                std::cout << "  ";
            }
            std::cout << gene.getFrom()->getNodeNum()
                      << " -> "
                      << gene.getTo()->getNodeNum()
                      << "\t"
                      << gene.getInnovation()
                      << "\t"
                      << gene.getWeight()
                      << std::endl;
        }
    }
}
