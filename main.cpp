#include <iostream>
#include "src/NEAT.hpp"


int main() {
//    NEAT neat = NEAT(10, 10, 12345);
    srand(123456);
    ANN ann = ANN(3, 1, "A");
    ann.addNodeMutation();
    ann.addConnectionMutation();

    for (auto cg : ann.getGenome()) {
        if (cg.getEnabled())
            std::cout << cg.getFrom()->getNodeNum() << " -> " << cg.getTo()->getNodeNum() << std::endl;
    }

    return 0;
}