//
// Created by Sheldon Woodward on 5/23/18.
//

#ifndef NEAT_GENE_H
#define NEAT_GENE_H


struct Gene {
    unsigned int from;
    unsigned int to;
    bool enabled;
    bool exist;

    Gene(unsigned int from, unsigned int to);
    Gene(unsigned int from, unsigned int to, bool enabled);
    Gene(unsigned int from, unsigned int to, bool enabled, bool exist);
};


#endif //NEAT_GENE_H
