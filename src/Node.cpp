//
// Created by Sheldon Woodward on 5/7/18.
//

#include "Node.hpp"

Node::Node() {
    this->nodeNum = 0;
    this->value = 0;
    this->layer = 0;
}

Node::Node(int nodeNum) {
    this->nodeNum = nodeNum;
    this->layer = 0;
    this->value = 0;
}

bool Node::layerSort(Node n1, Node n2) {
    if (n1.getLayer() <= n2.getLayer()) return false;
    else if (n1.getLayer() == n2.getLayer() && n1.getNodeNum() < n2.getNodeNum()) return false;
    return true;
}

bool Node::ptrLayerSort(Node *n1, Node* n2) {
    if (n1->getLayer() <= n2->getLayer()) return false;
    else if (n1->getLayer() == n2->getLayer() && n1->getNodeNum() < n2->getNodeNum()) return false;
    return true;
}

bool Node::nodeNumSort(Node* n1, Node* n2) {
    return n1->getNodeNum() < n2->getNodeNum();
}


int Node::getNodeNum() {
    return this->nodeNum;
}

unsigned int Node::getLayer() {
    return this->layer;
}

void Node::setLayer(unsigned int layer) {
    this->layer = layer;
}

void Node::setValue(float value) {
    this->value = value;
}

float Node::getValue() {
    return this->value;
}

float *Node::getValuePtr() {
    return &this->value;
}
