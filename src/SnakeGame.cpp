//
// Created by Sheldon Woodward on 5/12/18.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include "SnakeGame.hpp"

SnakeGame::SnakeGame(int sizeX, int sizeY) {
    timeOut = sizeX * sizeY;
    width = sizeX;
    height = sizeY;
}

float SnakeGame::look(std::pair<int, int> loc, std::pair<int, int> direction) {
    float count = 0;
    do {
        count += 1;
        loc.first += direction.first;
        loc.second += direction.second;
    } while (!outOfBounds(loc) && !(loc == food) && !snake.inBody(loc, false));

    if (loc == food) {
        return static_cast<float>(1.0 / count);
    }

    return static_cast<float>(-1.0 / count);
}

int SnakeGame::fitness(ANN &agent) {
    snake = Snake(width / 2, (height / 2) - 1, width / 2, height / 2);
    std::deque<float> input;
    std::deque<float> output;
    food = std::nullopt;
    int time = -1;

    // record begin
    agent.resetLog();
    agent.addLog("Board: " + std::to_string(width) + "," + std::to_string(height) + "\n");

    do {
        time += 1;
        if (!food) {
            generateFood();
        }

//        std::cout << "food: " << food.value().first << ", " << food.value().second << std::endl;
//        std::cout << "snake: " << std::endl;
//        std::cout << snake.toString() << std::endl;

        // record body
        agent.addLog("food:" + std::to_string(food.value().first) + "," + std::to_string(food.value().second) + "\n");
        agent.addLog(snake.toString());
        agent.addLog("@@@\n");

        input = getDeque();

//        std::cout << "input: " << std::endl;
//        for (auto it : input) {
//            std::cout << it << std::endl;
//        }
        output = agent.compute(input);

//        std::cout << "output: " << std::endl;
//        for (auto it : output) {
//            std::cout << it << std::endl;
//        }


        int max = static_cast<int>(std::distance(output.begin(), std::max_element(output.begin(), output.end())));


        std::pair<int, int> nextStep;
        switch (max) {
            case 0: {
                nextStep = std::pair(snake.getHead().first + snake.left().first,
                                     snake.getHead().second - snake.left().second);
                if (nextStep == food) {
                    snake.eat(nextStep);
                    food = std::nullopt;
                } else {
                    snake.move(nextStep);
                }
                break;
            }
            case 1: {
                nextStep = std::pair(snake.getHead().first + snake.direction().first,
                                     snake.getHead().second - snake.direction().second);
                if (nextStep == food) {
                    snake.eat(nextStep);
                    food = std::nullopt;
                } else {
                    snake.move(nextStep);
                }
                break;
            }
            case 2: {
                nextStep = std::pair(snake.getHead().first + snake.right().first,
                                     snake.getHead().second - snake.right().second);
                if (nextStep == food) {
                    snake.eat(nextStep);
                    food = std::nullopt;
                    time -= 10;
                } else {
                    snake.move(nextStep);
                }
                break;
            }
        }


    } while (!gameOver(time));
    //Log end
    agent.addLog("###\n");

    return snake.size() - 2;
}

std::deque<float> SnakeGame::getDeque() {
    std::pair<int, int> direction = snake.direction();
    std::deque<float> input;

    input.push_back(look(snake.getHead(), snake.left()));
    input.push_back(look(snake.getHead(), snake.leftDiagonal()));
    input.push_back(look(snake.getHead(), direction));
    input.push_back(look(snake.getHead(), snake.rightDiagonal()));
    input.push_back(look(snake.getHead(), snake.right()));
    return input;
}

bool SnakeGame::outOfBounds(std::pair<int, int> loc) {
    return (loc.first >= width || loc.first < 0 || loc.second >= height || loc.second < 0);
}

bool SnakeGame::gameOver(int time) {
    std::pair<int, int> head = snake.getHead();
    return ((time == timeOut) || snake.inBody(snake.getHead(), false) || outOfBounds(head));
}

void SnakeGame::generateFood() {
    std::pair<int, int> testFood;
    do {
        testFood.first = rand() % width;
        testFood.second = rand() % height;
    } while (snake.inBody(testFood, true));

    food = testFood;
}


