//
// Created by Sheldon Woodward on 5/7/18.
//

#include "ConnectionGene.hpp"

unsigned int ConnectionGene::innovationCount = 0;

// constructor
ConnectionGene::ConnectionGene(Node *from, Node *to, float weight) : ConnectionGene(from, to, weight, innovationCount++) {

}

ConnectionGene::ConnectionGene(Node *from, Node *to, float weight, unsigned int innovation) : ConnectionGene(from, to, weight, innovation, true) {

}

ConnectionGene::ConnectionGene(Node *from, Node *to, float weight, unsigned int innovation, bool enabled) {
    this->from = from;
    this->to = to;
    this->weight = weight;
    this->enabled = enabled;
    this->layer = 0;
    this->innovation = innovation;
}

// sort
bool ConnectionGene::layerSort(ConnectionGene *cg1, ConnectionGene *cg2) {
    if (cg1->layer <= cg2->layer) return false;
    else if (cg1->layer == cg2->layer && cg1->innovation < cg2->innovation) return false;
    return true;
}

bool ConnectionGene::innovationSort(ConnectionGene &cg1, ConnectionGene &cg2) {
    return cg1.innovation < cg2.innovation;
}

bool ConnectionGene::innovationSortPtr(ConnectionGene *cg1, ConnectionGene *cg2) {
    return cg1->innovation < cg2->innovation;
}

