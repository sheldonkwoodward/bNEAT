//
// Created by Sheldon Woodward on 5/7/18.
//

#include "Node.hpp"

// constructor
Node::Node(int nodeNum) {
    this->nodeNum = nodeNum;
    this->layer = 0;
    this->value = 0;
}

// sort functions
bool Node::layerSort(Node* n1, Node* n2) {
    if (n1->layer <= n2->layer) return false;
    else if (n1->layer == n2->layer && n1->nodeNum < n2->nodeNum) return false;
    return true;
}

bool Node::sequentialSort(Node* n1, Node* n2) {
    return n1->nodeNum < n2->nodeNum;
}

// set get
int Node::getNodeNum() {
    return this->nodeNum;
}

unsigned int Node::getLayer() {
    return this->layer;
}

void Node::setLayer(unsigned int layer) {
    this->layer = layer;
}

float Node::getValue() {
    return this->value;
}

void Node::setValue(float value) {
    this->value = value;
}

float *Node::getValuePtr() {
    return &this->value;
}
