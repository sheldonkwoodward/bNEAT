//
// Created by Sheldon Woodward on 5/12/18.
//

#ifndef NEAT_SNAKE_HPP
#define NEAT_SNAKE_HPP

#import <deque>
#import "ANN.hpp"


class Snake {

    std::deque<std::pair<int, int>> snake;
    std::pair<int,int> food;
    int timeOut;
    int width;
    int height;

    bool gameOver();

    void generateFood();

    void parseInput(std::deque<float> &input);

    int validMove(std::deque<float> &output) ;

public:
    Snake(int sizeX, int sizeY);

    float fitness(ANN agent, bool record);

};


#endif //NEAT_SNAKE_HPP
