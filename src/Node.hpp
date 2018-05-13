//
// Created by Sheldon Woodward on 5/7/18.
//

#ifndef NEAT_NODE_HPP
#define NEAT_NODE_HPP


class Node {
    int nodeNum;
    float value;
public:
    // constructor
    Node();
    explicit Node(int nodeNum);

    // set get
    void setValue(float value);
    float getValue();
    float* getValuePtr();
    int getNodeNum();
};


#endif //NEAT_NODE_HPP
