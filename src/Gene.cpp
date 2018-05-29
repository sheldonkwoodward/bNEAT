//
// Created by Sheldon Woodward on 5/23/18.
//

#include "Gene.h"

Gene::Gene(int from, int to, unsigned int innovation) {
    this->from = from;
    this->to = to;
    this->innovation = innovation;
}

bool Gene::sort(const Gene &lhs, const Gene &rhs) {
    if (lhs.from < rhs.from) return true;
    return lhs.from == rhs.from && lhs.to < rhs.to;
}

bool Gene::operator==(const Gene &rhs) {
    return from == rhs.from && to == rhs.to;
}

bool Gene::operator!=(const Gene &rhs) {
    return from != rhs.from || to != rhs.to;
}
