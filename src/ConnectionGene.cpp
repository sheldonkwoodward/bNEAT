//
// Created by Sheldon Woodward on 5/7/18.
//

#include "ConnectionGene.hpp"

ConnectionGene::ConnectionGene(Node* from, Node* to, float weight) {
    this->from = from;
    this->to = to;
    this->weight = weight;
    this->enabled = true;
    this->innovation = 0;
}

ConnectionGene::ConnectionGene(Node* from, Node* to, float weight, bool enabled, int innovation) {
    this->from = from;
    this->to = to;
    this->weight = weight;
    this->enabled = enabled;
    this->innovation = innovation;
}

Node *ConnectionGene::getFrom() {
    return this->from;
}

Node *ConnectionGene::getTo() {
    return this->to;
}

float ConnectionGene::getWeight() {
    return this->weight;
}
