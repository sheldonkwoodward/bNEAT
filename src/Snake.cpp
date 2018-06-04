//
// Created by charlie on 5/31/18.
//

#include <utility>
#include <iostream>
#include "Snake.hpp"


Snake::Snake(int x1, int y1, int x2, int y2) {
    body = std::deque<std::pair<int, int>> {};
    body.push_front(std::pair(x2, y2));
    body.push_front(std::pair(x1, y1));

    directions = std::deque<std::pair<int, int>>();
    directions.emplace_back(0, -1);
    directions.emplace_back(1, 0);
    directions.emplace_back(0, 1);
    directions.emplace_back(-1, 0);
}


std::pair<int, int> Snake::getHead() {
    return body.front();
};

bool Snake::inBody(std::pair<int, int> loc, bool whole) {
    int begin = 1;
    if (whole) {
        begin = 0;

    }
    for (int i = begin; i < body.size(); i++) {
        if (body[i] == loc) {
            return true;
        }
    }

    return false;
}

void Snake::move(std::pair<int, int> loc) {
    body.push_front(loc);
    body.pop_back();
}

void Snake::eat(std::pair<int, int> loc) {
    body.push_front(loc);
}

int Snake::size() {
    return static_cast<int>(body.size());
}

std::pair<int, int> Snake::direction() {
    return std::pair(body[0].first - body[1].first, body[0].second - body[1].second);
};

std::pair<int, int> Snake::left() {
    std::pair<int, int> dir = direction();
    int index = 0;
//    std:cout << "dirs" << std::endl;
    for (int i = 0; i < directions.size(); i++) {
//        std::cout << directions[i].first << "," << directions[i].second << std::endl;
        if (directions[i] == dir) {
            index = i;
        }
    }

    index -= 1;
    if (index < 0) {
        index = 3;
    }
    if (index > 3) {
        index = 0;
    }
    return directions[index];
};

std::pair<int, int> Snake::right() {
    std::pair<int, int> dir = direction();
    int index = 0;
    for (int i = 0; i < directions.size(); i++) {
        if (directions[i] == dir) {
            index = i;
        }

    }
    index += 1;
    if (index < 0) {
        index = 3;
    }
    if (index > 3) {
        index = 0;
    }
    return directions[index];
};

std::pair<int, int> Snake::leftDiagonal() {
    return std::pair<int, int>(left().first + direction().first, left().second + direction().second);
};

std::pair<int, int> Snake::rightDiagonal() {
    return std::pair<int, int>(right().first + direction().first, right().second + direction().second);
};

std::string Snake::toString() {
    std::string s = "";
    for (auto it : body) {
        s.append(std::to_string(it.first));
        s.append(",");
        s.append(std::to_string(it.second));
        s.append("\n");
    }
    return s;
}

Snake::Snake() {
}
