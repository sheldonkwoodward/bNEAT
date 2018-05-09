//
// Created by Sheldon Woodward on 4/27/18.
//

#ifndef NEAT_ANN_HPP
#define NEAT_ANN_HPP

#include <deque>
#include "Node.hpp"
#include "ConnectionGene.hpp"


class ANN {
private:
    std::deque<Node> nodes;
    std::deque<ConnectionGene> genome;
    std::deque<Node*> inputNodes;
    std::deque<Node*> outputNodes;

    // private functions
    void determineInputOutput();
public:
    // constructors
    ANN();

    // functions
    std::deque<float> compute(std::deque<float> inputs);
    Node* addNode(Node node);
    ConnectionGene* addConnectionGene(ConnectionGene connectionGene);
};


#endif //NEAT_ANN_HPP
