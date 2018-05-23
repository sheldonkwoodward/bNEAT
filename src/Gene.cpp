//
// Created by Sheldon Woodward on 5/23/18.
//

#include "Gene.h"

Gene::Gene(unsigned int from, unsigned int to) : Gene(from, to, true) {

}

Gene::Gene(unsigned int from, unsigned int to, bool enabled) : Gene(from, to, enabled, true) {

}

Gene::Gene(unsigned int from, unsigned int to, bool enabled, bool exist) {
    this->from = from;
    this->to = to;
    this->enabled = enabled;
    this->exist = exist;
}
