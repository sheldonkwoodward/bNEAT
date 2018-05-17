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
    unsigned int layer;
public:
    // constructor
//    ConnectionGene(Node* from, Node* to, float weight);
    ConnectionGene(Node* from, Node* to, float weight, bool enabled, int innovation);

    static bool ptrComparison(ConnectionGene* cg1, ConnectionGene* cg2);

    // get set
    Node* getFrom();
    Node* getTo();
    float getWeight();
    bool getEnabled();
    void setEnabled(bool enabled);
    int getInnovation();
    void setInnovation(int innovation);
    unsigned int getLayer();
    void setLayer(unsigned int layer);
};


#endif //NEAT_CONNECTIONGENE_HPP
