#include <iostream>
#include "src/NEAT.hpp"


int main() {
    // setup
    srand(1);
    const int INPUT_NUM = 100;
    const int OUTPUT_NUM = 4;

    // build ANN
    ANN ann = ANN(INPUT_NUM, OUTPUT_NUM, "A");
    for (int i = 0; i < 100; i++) {
        if (!(i % 2)) ann.addNodeMutation();
        ann.addConnectionMutation();
    }

    // print genome
    for (auto cg : ann.getEnabledSortedGenome()) {
        if (cg->getEnabled())
            std::cout << cg->getFrom()->getNodeNum() << " -> " << cg->getTo()->getNodeNum() << " " << cg->getLayer() <<
                                                                                                                  std::endl;
    }

    // add inputs
    std::deque<float> inputs = std::deque<float>();
    for (int i = 0; i < INPUT_NUM; i++) {
        inputs.push_back(1.0f);
    }

    // compute
    std::deque<float> output = ann.compute(inputs);

    // print output
    while (true) {
        output = ann.compute(inputs);
        std::cout << "OUTPUT" << std::endl;
        for (auto o : output) {
            std::cout << o << std::endl;
        }
    }

    return 0;
}