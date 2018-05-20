//
// Created by Sheldon Woodward on 5/7/18.
//

#ifndef NEAT_NODE_HPP
#define NEAT_NODE_HPP


class Node {
    int nodeNum;
    unsigned int layer;
    float value;
public:
    // constructor
    explicit Node(int nodeNum);

    // sort
    static bool layerSort(Node *n1, Node *n2);
    static bool sequentialSort(Node *n1, Node *n2);

    // set get
    int getNodeNum();
    unsigned int getLayer();
    void setLayer(unsigned int layer);
    float getValue();
    void setValue(float value);
    float* getValuePtr();
};


#endif //NEAT_NODE_HPP
