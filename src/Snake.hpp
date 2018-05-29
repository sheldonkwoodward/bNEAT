//
// Created by Sheldon Woodward on 5/12/18.
//

#ifndef NEAT_SNAKE_HPP
#define NEAT_SNAKE_HPP

#import <deque>
#include <optional>
#import "ANN.hpp"


class Snake {
    std::deque<std::pair<int, int>> snake;
    std::optional<std::pair<int, int>> food;
    int timeOut;
    int width;
    int height;

    bool gameOver(int time);

    void generateFood();

    void parseInput(std::deque<float> &input);

    int validMove(std::deque<float> &output);

public:
    Snake(int sizeX, int sizeY);

    int fitness(ANN &agent, bool record);

};


#endif //NEAT_SNAKE_HPP
