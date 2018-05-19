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
    // TODO: remove this constructor
    Node();
    explicit Node(int nodeNum);

    // operator overloads
    static bool layerSort(Node n1, Node n2);
    static bool ptrLayerSort(Node* n1, Node* n2);
    static bool nodeNumSort(Node* n1, Node* n2);

    // set get
    int getNodeNum();
    unsigned int getLayer();
    void setLayer(unsigned int layer);
    void setValue(float value);
    float getValue();
    float* getValuePtr();
};


#endif //NEAT_NODE_HPP
