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
    static bool sort(Gene &gene1, Gene &gene2);
    bool operator==(const Gene &rhs);
    bool operator!=(const Gene &rhs);
    static bool lessThan(const Gene &lhs, const Gene &rhs);
};


#endif //NEAT_GENE_H
