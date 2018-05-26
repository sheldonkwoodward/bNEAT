//
// Created by Sheldon Woodward on 5/23/18.
//

#include "Gene.h"

Gene::Gene(int from, int to, unsigned int innovation) {
    this->from = from;
    this->to = to;
    this->innovation = innovation;
}

bool Gene::sort(Gene &gene1, Gene &gene2) {
    return gene1.from > gene2.from || gene1.to > gene2.to;
}

bool Gene::operator==(const Gene &rhs) {
    return from == rhs.from && to == rhs.to;
}

bool Gene::operator!=(const Gene &rhs) {
    return from != rhs.from || to != rhs.to;
}

bool Gene::lessThan(const Gene &lhs, const Gene &rhs) {
    return lhs.from < rhs.from || lhs.to < rhs.to;
}
