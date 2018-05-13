//
// Created by Sheldon Woodward on 5/12/18.
//

#ifndef NEAT_SNAKE_HPP
#define NEAT_SNAKE_HPP

#import "ANN.hpp"


class Snake {
public:
    Snake(int sizeX, int sizeY);

    float fitness(std::string algorithm, bool record);
    float fitness1(ANN ann);
    float fitness2(ANN ann);
};


#endif //NEAT_SNAKE_HPP
