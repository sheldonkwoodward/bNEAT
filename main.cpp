#include "src/ANN.hpp"


int main() {
    ANN ann = ANN();

    auto n0 = ann.addNode(Node(0));
    auto n1 = ann.addNode(Node(1));
    auto n2 = ann.addNode(Node(2));
    auto n3 = ann.addNode(Node(3));
    auto n4 = ann.addNode(Node(4));
    auto n5 = ann.addNode(Node(5));

    auto cg0 = ann.addConnectionGene(ConnectionGene(n0, n3, 1.0));
    auto cg1 = ann.addConnectionGene(ConnectionGene(n1, n3, 1.0));
    auto cg2 = ann.addConnectionGene(ConnectionGene(n2, n3, 1.0));
    auto cg3 = ann.addConnectionGene(ConnectionGene(n0, n4, 1.0));
    auto cg4 = ann.addConnectionGene(ConnectionGene(n1, n4, 1.0));
    auto cg5 = ann.addConnectionGene(ConnectionGene(n2, n4, 1.0));
    auto cg6 = ann.addConnectionGene(ConnectionGene(n3, n5, 1.0));
    auto cg7 = ann.addConnectionGene(ConnectionGene(n4, n5, 1.0));
//    auto cg8 = ann.addConnectionGene(ConnectionGene(n0, n5, 1));

    std::deque<float> inputs = std::deque<float>();
    inputs.push_back(1.0);
    inputs.push_back(1.0);
    inputs.push_back(1.0);
    while (true) ann.compute(inputs);

    return 0;
}