#include <iostream>
#include "src/NEAT.hpp"


int main() {
//    NEAT neat = NEAT(10, 10, 12345);
//    neat.train(0.01f, "FITNESS", "PSELECTION", "SSELECTION");
    srand(12345);
    ANN ann = ANN(3, 1, "A");
    ann.addNodeMutation();
    ann.addNodeMutation();
//    ann.addConnectionMutation();

    for (auto cg : ann.getGenome()) {
        if (cg.getEnabled())
            std::cout << cg.getFrom()->getNodeNum() << " -> " << cg.getTo()->getNodeNum() << std::endl;
    }

    std::deque<float> inputs = std::deque<float>();
    inputs.push_back(1.0f);
    inputs.push_back(1.0f);
    inputs.push_back(1.0f);
    std::deque<float> output = ann.compute(inputs);
    auto sorted = ann.getSortedNodes();
    auto genome = ann.getEnabledSortedGenome();

    for (auto o : output) {
        std::cout << o << std::endl;
    }

    return 0;
}