#include <iostream>
#include "NEAT.hpp"


int main() {
    NEAT neat = NEAT(10, 10, 569283476);
    std::ofstream file;
    file.open("/Users/sheldonwoodward/Desktop/generation-dump.csv");
    file << "ID,Generation,Species,Fitness,Food Eaten,Layers,Connections,Nodes,Age\n";
    file.close();
    neat.train("/Users/sheldonwoodward/Desktop/generation-dump.csv");
    
    return 0;
}
