//
// Created by Sheldon Woodward on 5/7/18.
//

#ifndef NEAT_CONNECTIONGENE_HPP
#define NEAT_CONNECTIONGENE_HPP

#include "Node.hpp"


class ConnectionGene {
    Node* from;
    Node* to;
    float weight;
    bool enabled;
    int innovation;
public:
    // constructor
    ConnectionGene(Node* from, Node* to, float weight);
    ConnectionGene(Node* from, Node* to, float weight, bool enabled, int innovation);

    // get set
    Node* getFrom();
    Node* getTo();
    float getWeight();
};


#endif //NEAT_CONNECTIONGENE_HPP
