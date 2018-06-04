//
// Created by charlie on 5/31/18.
//

#ifndef NEAT_SNAKE_HPP
#define NEAT_SNAKE_HPP


#include <deque>
#include <string>

class Snake {

    std::deque<std::pair<int,int>> body;
    std::deque<std::pair<int,int>> directions;

public:
    Snake();
    Snake(int x1, int y1, int x2, int y2);

    std::pair<int, int> getHead();

    bool inBody(std::pair<int, int> loc, bool whole);

    void move(std::pair<int, int>);

    void eat(std::pair<int, int> loc);

    std::string toString();

    int size();

    std::pair<int, int> direction();

    std::pair<int, int> left();


    std::pair<int, int> right();

    std::pair<int, int> leftDiagonal();

    std::pair<int, int> rightDiagonal();
};


#endif //NEAT_SNAKE_HPP
