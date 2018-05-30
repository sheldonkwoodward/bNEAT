//
// Created by Sheldon Woodward on 5/12/18.
//

#include "NEAT.hpp"

NEAT::NEAT(int sizeX, int sizeY, unsigned int seed) : snake(sizeX, sizeY) {
    srand(seed);
    population = std::deque<ANN>();
    parents = std::deque<std::pair<ANN*, ANN*>>();
    generationCount = 0;
}

void NEAT::train(float mutationRate, std::string fitness, std::string parentSelectionAlg, std::string survivorSelectionAlg) {
    // constants
    const unsigned int POPULATION_SIZE = 100;

    // training
    populate(POPULATION_SIZE);
    int i = 0;
    while(i > -1) {
        parentSelection(parentSelectionAlg);
        crossover(mutationRate);
        survivorSelection(survivorSelectionAlg);
        if (i % 500 == 0) {
            std::sort(population.begin(), population.end(), ANN::fitnessSort);
            std::cout << "GENERATION " << i << " MAX FITNESS: " << population.back().getFitness() << std::endl;
        }
        if (i == 2000) {
            std::sort(population.begin(), population.end(), ANN::fitnessSort);
            population.back().dumpTopology("/Users/sheldonwoodward/Desktop/ann-dumps/dump-g" + std::to_string(i) + ".txt");
            break;
        }
        i++;
    }

    // TODO: crossover and mutation
    // TODO: survivor selection
}

void NEAT::populate(unsigned int populationSize) {
    // initial population
    population.clear();
    for (int p = 0; p < populationSize; p++) {
        // TODO: CHANGE ANN CREATION SIZE
        population.emplace_back(25, 4, std::to_string(p));
        population.back().connectionMutation();
        population.back().setFitness(snake.fitness(population.back(), false));
    }
}

void NEAT::parentSelection(std::string algorithm) {
    // TODO: maybe prevent self-crossover
    // TODO: maybe prevent parent chosen multiple times
    parents.clear();
    // determine parents
    int fitnessSum = 0;
    while(fitnessSum == 0) {
        for (auto &ann : population) fitnessSum += ann.getFitness();
        // TODO: better handling of all zero fitness
        if (fitnessSum == 0) {
            std::cout << "ZERO FITNESS" << std::endl;
            for (auto &ann : population) ann.connectionMutation();
        }
    }
    for (int i = 0; i < population.size() / 3; i++) {
        // wheel prep
        std::pair<ANN*, ANN*> newParents = std::pair<ANN*, ANN*>();
        int roulette1 = rand() % fitnessSum;
        int roulette2 = rand() % fitnessSum;
        int rouletteCounter = 0;
        // spin wheel
        for (int ann = 0; ann < population.size(); ann++) {
            rouletteCounter += population[ann].getFitness();
            if (population[ann].getFitness() != 0 && roulette1 <= rouletteCounter) newParents.first = &population[ann];
            if (population[ann].getFitness() != 0 && roulette2 <= rouletteCounter) newParents.second = &population[ann];
            if (newParents.first != nullptr && newParents.second != nullptr) break;
        }
        // add to parents
        parents.push_back(newParents);
    }
}

void NEAT::crossover(float mutationRate) {
    for (auto &p : parents) {
        population.push_back(crossover(*p.first, *p.second, mutationRate));
    }
}

ANN NEAT::crossover(ANN &ann1, ANN &ann2, float mutationRate) {
    ANN newANN = ANN(ann1, ann2);
    if ((float)(rand() % 1000) / 1000.0f < mutationRate) newANN.weightMutation();
    if ((float)(rand() % 1000) / 1000.0f < mutationRate) newANN.connectionMutation();
    if ((float)(rand() % 1000) / 1000.0f < mutationRate) newANN.nodeMutation();
    if (ann1.getFitness() >= 4 && ann2.getFitness() >= 4 && generationCount % 500 == 0) newANN.setFitness(snake.fitness(newANN, true));
    else newANN.setFitness(snake.fitness(newANN, false));
    return newANN;
}

void NEAT::survivorSelection(std::string algorithm) {
    // TODO: other options than complete population replacement
    std::sort(population.begin(), population.end(), ANN::fitnessSort);
    for (int i = 0; i < parents.size(); i++) {
        population.pop_front();
    }
    for (auto &ann : population) ann.incrementAge();
    generationCount++;
}
