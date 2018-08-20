//
// Created by Sheldon Woodward on 5/12/18.
//

#ifndef NEAT_SNAKE_GAME_HPP
#define NEAT_SNAKE_GAME_HPP

#import <deque>
#include <optional>
#import "ANN.hpp"
#include "Snake.hpp"


class SnakeGame {
    Snake snake;
    std::optional<std::pair<int, int>> food;
    int timeOut;
    int width;
    int height;

    bool gameOver(int time);

    void generateFood();

    float look(std::pair<int,int> loc, std::pair<int,int> direction);

    bool outOfBounds(std::pair<int, int> loc);
    std::deque<float> getDeque();

public:
    SnakeGame(int sizeX, int sizeY);

    int fitness(ANN &agent);

};


#endif //NEAT_SNAKE_HPP
