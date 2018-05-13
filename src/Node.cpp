//
// Created by Sheldon Woodward on 5/7/18.
//

#include "Node.hpp"

Node::Node() {
    this->nodeNum = 0;
    this->value = 0;
    this->layer = 0;
}

Node::Node(int nodeNum, int layer) {
    this->nodeNum = nodeNum;
    this->layer = layer;
    this->value = 0;
}

int Node::getNodeNum() {
    return this->nodeNum;
}

int Node::getLayer() {
    return this->layer;
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
