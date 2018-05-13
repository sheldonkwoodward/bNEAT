//
// Created by Sheldon Woodward on 5/7/18.
//

#ifndef NEAT_NODE_HPP
#define NEAT_NODE_HPP


class Node {
    int nodeNum;
    int layer;
    float value;
public:
    // constructor
    Node();
    Node(int nodeNum, int layer);

    // set get
    int getNodeNum();
    int getLayer();
    void setValue(float value);
    float getValue();
    float* getValuePtr();
};


#endif //NEAT_NODE_HPP
