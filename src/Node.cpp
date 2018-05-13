//
// Created by Sheldon Woodward on 5/7/18.
//

#include "Node.hpp"

Node::Node() {
    this->nodeNum = 0;
    this->value = 0;
}

Node::Node(int nodeNum) {
    this->nodeNum = nodeNum;
    this->value = 0;
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

int Node::getNodeNum() {
    return this->nodeNum;
}