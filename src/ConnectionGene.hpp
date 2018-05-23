//
// Created by Sheldon Woodward on 5/7/18.
//

#ifndef NEAT_CONNECTIONGENE_HPP
#define NEAT_CONNECTIONGENE_HPP

#include <deque>

#include "Node.hpp"


class ConnectionGene {
    Node* from;
    Node* to;
    float weight;
    bool enabled;
    int innovation;
    unsigned int layer;
    static unsigned int innovationCount;

public:
    // constructor
    ConnectionGene(Node* from, Node* to, float weight);
    ConnectionGene(Node* from, Node* to, float weight, unsigned int innovation);

    // sort
    static bool layerSort(ConnectionGene *cg1, ConnectionGene *cg2);
    static bool innovationSort(ConnectionGene &cg1, ConnectionGene &cg2);

    // get set
    Node* getFrom();
    Node* getTo();
    float getWeight();
    void setWeight(float weight);
    bool getEnabled();
    void setEnabled(bool enabled);
    int getInnovation();
    void setInnovation(int innovation);
    unsigned int getLayer();
    void setLayer(unsigned int layer);
    static unsigned int getInnovationCount();
};


#endif //NEAT_CONNECTIONGENE_HPP
