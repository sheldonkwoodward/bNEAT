//
// Created by Sheldon Woodward on 5/7/18.
//

#ifndef NEAT_CONNECTIONGENE_HPP
#define NEAT_CONNECTIONGENE_HPP

#include <deque>

#include "Node.hpp"


struct ConnectionGene {
  Node *from;
  Node *to;
  float weight;
  bool enabled;
  int innovation;
  unsigned int layer;
  static unsigned int innovationCount;

  // constructor
  ConnectionGene(Node *from, Node *to, float weight);
  ConnectionGene(Node *from, Node *to, float weight, unsigned int innovation);
  ConnectionGene(Node *from, Node *to, float weight, unsigned int innovation,
                 bool enabled);

  // sort
  static bool layerSort(ConnectionGene *cg1, ConnectionGene *cg2);
  static bool innovationSort(ConnectionGene &cg1, ConnectionGene &cg2);
  static bool innovationSortPtr(ConnectionGene *cg1, ConnectionGene *cg2);
};

#endif // NEAT_CONNECTIONGENE_HPP
