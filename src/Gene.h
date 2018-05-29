//
// Created by Sheldon Woodward on 5/23/18.
//

#ifndef NEAT_GENE_H
#define NEAT_GENE_H


struct Gene {
    int from;
    int to;
    unsigned int innovation;

    Gene(int from, int to, unsigned int innovation);
    static bool sort(const Gene &lhs, const Gene &rhs);
    bool operator==(const Gene &rhs);
    bool operator!=(const Gene &rhs);
};


#endif //NEAT_GENE_H
