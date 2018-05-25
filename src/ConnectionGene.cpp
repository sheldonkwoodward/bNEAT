//
// Created by Sheldon Woodward on 5/7/18.
//

#include "ConnectionGene.hpp"

unsigned int ConnectionGene::innovationCount = 0;

// constructor
ConnectionGene::ConnectionGene(Node* from, Node* to, float weight) {
    this->from = from;
    this->to = to;
    this->weight = weight;
    this->enabled = true;
    this->layer = 0;
    this->innovation = innovationCount++;
}

ConnectionGene::ConnectionGene(Node *from, Node *to, float weight, unsigned int innovation) {
    this->from = from;
    this->to = to;
    this->weight = weight;
    this->enabled = true;
    this->layer = 0;
    this->innovation = innovation;
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
    if (cg1->getLayer() <= cg2->getLayer()) return false;
    else if (cg1->getLayer() == cg2->getLayer() && cg1->getInnovation() < cg2->getInnovation()) return false;
    return true;
}

bool ConnectionGene::innovationSort(ConnectionGene &cg1, ConnectionGene &cg2) {
    return cg1.getInnovation() > cg1.getInnovation();
}

bool ConnectionGene::innovationSortPtr(ConnectionGene *cg1, ConnectionGene *cg2) {
    return cg1->getInnovation() > cg1->getInnovation();
}

Node *ConnectionGene::getFrom() {
    return this->from;
}

Node *ConnectionGene::getTo() {
    return this->to;
}

// get set
float ConnectionGene::getWeight() {
    return this->weight;
}

void ConnectionGene::setWeight(float weight) {
    this->weight = weight;
}

bool ConnectionGene::getEnabled() {
    return this->enabled;
}

void ConnectionGene::setEnabled(bool enabled) {
    this->enabled = enabled;
}

int ConnectionGene::getInnovation() {
    return innovation;
}

void ConnectionGene::setInnovation(int innovation) {
    this->innovation = innovation;
}

unsigned int ConnectionGene::getLayer() {
    return layer;
}

void ConnectionGene::setLayer(unsigned int layer) {
    this->layer = layer;
}

unsigned int ConnectionGene::getInnovationCount() {
    return innovationCount;
}
