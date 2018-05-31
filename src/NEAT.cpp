//
// Created by Sheldon Woodward on 5/12/18.
//

#include "NEAT.hpp"

const unsigned int NEAT::POP_SIZE = 100;
const float NEAT::MUT_RATE = 0.05f;  // rate at which mutations occur
const float NEAT::SPEC_RATE = 0.05f;  // rate at which normal NEAT speciation occurs
const float NEAT::SPEC_THRESH = 10.0f;  // max compatibility between two ANNs to be considered in the same species
const float NEAT::COMP_C0 = 1.0f;  // compatiblity constant modifier for excess innovations
const float NEAT::COMP_C1 = 1.0f;  // compatiblity constant modifier for disjoint innovations
const float NEAT::COMP_C2 = 1.0f;  // compatiblity constant modifier for average weight difference sum
const std::string NEAT::PS_ALG;  // algorithm used for parent selection
const std::string NEAT::SS_ALG;  // algorithm used for survivor selection

NEAT::NEAT(int sizeX, int sizeY, unsigned int seed) : snake(sizeX, sizeY) {
    srand(seed);
    population = std::deque<ANN>();
    parents = std::deque<std::pair<ANN*, ANN*>>();
    species = std::map<std::string, std::vector<ANN*>>();
    generationCount = 0;
}

void NEAT::train() {
    // training
    populate();
    int i = 0;
    while(i > -1) {
        parentSelection();
        crossover();
        survivorSelection();
        if (i % 500 == 0) {
            std::sort(population.begin(), population.end(), ANN::fitnessSort);
            std::cout << "GENERATION " << i << " MAX FITNESS: " << population.back().getFitness() << std::endl;
        }
//        if (i == 2000) {
//            std::sort(population.begin(), population.end(), ANN::fitnessSort);
//            population.back().dumpTopology("/Users/sheldonwoodward/Desktop/ann-dumps/dump-g" + std::to_string(i) + ".txt");
//            break;
//        }
        i++;
    }

    // TODO: crossover and mutation
    // TODO: survivor selection
}

void NEAT::populate() {
    // initial population
    population.clear();
    for (int p = 0; p < POP_SIZE; p++) {
        // TODO: CHANGE ANN CREATION SIZE
        population.emplace_back(25, 4, "0");
        addToSpecies(population.back());
        population.back().connectionMutation();
        population.back().setFitness(snake.fitness(population.back(), false));
    }
}

void NEAT::parentSelection() {
    // remove old parents
    parents.clear();
    // calculate total fitness of population
    int fitnessSum = 0;
    for (auto &ann : population) fitnessSum += ann.getFitness();
    // all fitness 0 in population
    if (fitnessSum == 0) {
        for (int i = 0; i < population.size(); i++) {
            parents.emplace_back();
            parents.back().first = &population[rand() % population.size()];
            parents.back().second = &population[rand() % population.size()];
        }
        return;
    }
    // roulette wheel spinning
    for (int i = 0; i < population.size(); i++) {
        // wheel prep
        parents.emplace_back();
        int roulette1 = rand() % fitnessSum;
        int roulette2 = rand() % fitnessSum;
        int rouletteCounter = 0;
        // spin wheel
        for (int ann = 0; ann < population.size(); ann++) {
            rouletteCounter += population[ann].getFitness();
            if (population[ann].getFitness() != 0 && roulette1 <= rouletteCounter && parents.back().first == nullptr)
                parents.back().first = &population[ann];
            if (population[ann].getFitness() != 0 && roulette2 <= rouletteCounter && parents.back().second == nullptr)
                parents.back().second = &population[ann];
            if (parents.back().first != nullptr && parents.back().second != nullptr) break;
        }
        if (parents.back().first == nullptr || parents.back().second == nullptr) std::cout << "BAD PARENT SELECTION: " << parents.back().first << " " << parents.back().second << std::endl;
    }
}

void NEAT::crossover() {
//    std::cout << "CROSS" << std::endl;
    for (auto &p : parents) {
        // crossover
        population.emplace_back(*p.first, *p.second);
        // mutations
        if ((float)(rand() % 1000) / 1000.0f < MUT_RATE) population.back().weightMutation();
        if ((float)(rand() % 1000) / 1000.0f < MUT_RATE) population.back().connectionMutation();
        if ((float)(rand() % 1000) / 1000.0f < MUT_RATE) population.back().nodeMutation();
        // fitness
        population.back().setFitness(snake.fitness(population.back(), false));
        // speciation
        if ((float)(rand() % 1000) / 1000.0f < SPEC_RATE) {
            bool foundSpecies = false;
            for (auto &s : species) {
                if (s.second.size() == 0) continue;
                unsigned long randomIndex = rand() % s.second.size();
                if (ANN::compatibility(*s.second[randomIndex], population.back(), COMP_C0, COMP_C1, COMP_C2) < SPEC_THRESH) {
                    population.back().setSpecies(s.first);
                    foundSpecies = true;
                    break;
                }
            }
            if (!foundSpecies) {
                population.back().setSpecies(std::to_string(species.size()));
            }
        } else {
            population.back().setSpecies(p.first->getSpecies());
        }
        addToSpecies(population.back());
    }
}

void NEAT::survivorSelection() {
    // TODO: other options than complete population replacement
    std::sort(population.begin(), population.end(), ANN::fitnessSort);
    for (int i = 0; i < parents.size(); i++) {
        removeFromSpecies(population.front());
        population.pop_front();
    }
    for (auto &ann : population) ann.incrementAge();
    generationCount++;
}

void NEAT::addToSpecies(ANN &ann) {
    species[ann.getSpecies()].push_back(&ann);
}

void NEAT::removeFromSpecies(ANN &ann) {
    for (auto a = species.at(ann.getSpecies()).begin(); a != species.at(ann.getSpecies()).end(); ++a) {
        if (*a == &ann) {
            species[ann.getSpecies()].erase(a);
            return;
        }
    }
}
