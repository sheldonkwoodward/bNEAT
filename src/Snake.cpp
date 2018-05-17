//
// Created by Sheldon Woodward on 5/12/18.
//

#include <algorithm>
#include "Snake.hpp"

Snake::Snake(int sizeX, int sizeY) : snake(snake) {
    timeOut = sizeX * sizeY;
    width = sizeX;
    height = sizeY;
}


bool Snake::gameOver() {
    return false;
}

float Snake::fitness(ANN agent, bool record) {
    snake = std::deque<std::pair<int, int>>();
    snake.push_back(std::pair(width / 2, height / 2));
    snake.push_back(std::pair(width / 2, (height / 2) + 1));
    std::deque<float> input(width * height, 0);
    std::deque<float> output;

    do {
        generateFood();
        parseInput(input);
        //output = agent.compute();


        switch (validMove(output)) {
            case 0:
                snake.push_back(std::pair(snake[0].first - 1, snake[0].second));
                break;
            case 1:
                snake.push_back(std::pair(snake[0].first + 1, snake[0].second));
                break;
            case 2:;
                snake.push_back(std::pair(snake[0].first, snake[0].second + 1));
                break;
            case 3:;
                snake.push_back(std::pair(snake[0].first, snake[0].second - 1));
                break;
        }


    } while (!gameOver());

    return 0;
}

int Snake::validMove(std::deque<float> &output) {
    int max = 0;
    int max2 = 0;

    for (int i = 0; i < output.size(); i++) {
        if (output[i] >= output[max]) {
            max2 = max;
            max = i;
        }
    }

    switch (max) {
        case 0: {
            if (snake[0].first - 1 == snake[1].first && snake[0].second == snake[1].second) {
                return max2;
            };
            break;
        }
        case 1: {
            if (snake[0].first + 1 == snake[1].first && snake[0].second == snake[1].second) {
                return max2;
            };
            break;
        }
        case 2: {
            if (snake[0].first == snake[1].first && snake[0].second + 1 == snake[1].second) {
                return max2;
            };
            break;
        }
        case 3: {
            if (snake[0].first - 1 == snake[1].first && snake[0].second - 1 == snake[1].second) {
                return max2;
            };
            break;
        }
    }

    return max;
}

void Snake::parseInput(std::deque<float> &input) {
    // parse snake into input
    for (int i = 0; i < input.size(); i++) {
        if (find(snake.begin(), snake.end(), std::pair(i % width, i / width)) != snake.end()) {
            input[i] = -1;
        } else {
            input[i] = 0;
        }
    }

    // parse food into input
    input[width * food.first + food.first] = 1;
}

void Snake::generateFood() {
    do {
        food.first = rand() % width;
        food.second = rand() % height;
    } while (find(snake.begin(), snake.end(), food) != snake.end());
}
