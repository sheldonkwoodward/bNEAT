#include <iostream>
#include "src/NEAT.hpp"
#include <ctime>


int main() {
    // setup
    srand(1234);
    const int INPUT_NUM = 3;
    const int OUTPUT_NUM = 1;

    // build ANN
    ANN ann = ANN(INPUT_NUM, OUTPUT_NUM, "A");
    for (int i = 0; i < 5; i++) {
        if (!(i % 2)) ann.addNodeMutation();
        ann.addConnectionMutation();
        ann.weightMutation();
    }

    // output genome
    std::cout << "F -> T L W" << std::endl;
    for (auto &gene : ann.getGenome()) {
        if (gene.getEnabled()) {
            std::cout << gene.getFrom()->getNodeNum() << " -> " << gene.getTo()->getNodeNum() << " " << gene.getLayer() << " " << gene.getWeight() << std::endl;
        }
     }

    // add inputs
    std::deque<float> inputs = std::deque<float>();
    for (int i = 0; i < INPUT_NUM; i++) {
        inputs.push_back(1.0f);
    }

    // compute
    std::deque<float> output = std::deque<float>();

    // print output
    int start_s=clock();
    for (int i = 0; i < 1; i++) {
        output = ann.compute(inputs);
        std::cout << "OUTPUT " << i << std::endl;
        for (auto o : output) {
            std::cout << o << std::endl;
        }

    }
    int stop_s=clock();
    std::cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << std::endl;


//    srand(1);
//    const int INPUT_NUM = 3;
//    const int OUTPUT_NUM = 1;
//
//    ANN ann1 = ANN(INPUT_NUM, OUTPUT_NUM, "A");
//    ANN ann2 = ANN(INPUT_NUM, OUTPUT_NUM, "B");
//    for (int i = 0; i < 5; i++) {
//        if (!(i % 2)) {
//            ann1.addNodeMutation();
//            ann2.addNodeMutation();
//        }
//        ann1.addConnectionMutation();
//        ann2.addConnectionMutation();
//    }

    return 0;
}
