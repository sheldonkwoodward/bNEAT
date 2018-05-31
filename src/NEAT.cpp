//
// Created by Sheldon Woodward on 5/12/18.
//

#include "NEAT.hpp"

const unsigned int NEAT::POP_SIZE = 100;
const float NEAT::MUT_RATE = 0.02f;  // rate at which mutations occur
const float NEAT::SPEC_RATE = 0.1f;  // rate at which normal NEAT speciation occurs
const float NEAT::SPEC_THRESH = 0.5f;  // max compatibility between two ANNs to be considered in the same species
const float NEAT::COMP_C0 = 2.0f;  // compatiblity constant modifier for excess innovations
const float NEAT::COMP_C1 = 2.0f;  // compatiblity constant modifier for disjoint innovations
const float NEAT::COMP_C2 = 0.0f;  // compatiblity constant modifier for average weight difference sum
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
        printGenerationInfo();
        i++;
    }
}

void NEAT::populate() {
    // initial population
    population.clear();
    for (int p = 0; p < POP_SIZE; p++) {
        population.emplace_back(25, 4, "0");
        addToSpecies(population.back());
        population.back().connectionMutation();
        population.back().setFitness(snake.fitness(population.back(), false));
    }
}

void NEAT::parentSelection() {
    // remove old parents
    parents.clear();
    // choose parents from species
    for (auto &spec : species) {
        // calculate total fitness of species
        int fitnessSum = 0;
        for (auto &ann : spec.second) fitnessSum += ann->getFitness();
        // all fitness 0 in species
        if (fitnessSum == 0) {
            for (int i = 0; i < spec.second.size(); i++) {
                parents.emplace_back();
                parents.back().first = spec.second[rand() % spec.second.size()];
                parents.back().second = spec.second[rand() % spec.second.size()];
            }
        } else {
            // roulette wheel spinning
            for (int i = 0; i < spec.second.size(); i++) {
                // wheel prep
                parents.emplace_back();
                int roulette1 = rand() % fitnessSum;
                int roulette2 = rand() % fitnessSum;
                int rouletteCounter = 0;
                // spin wheel
                for (int ann = 0; ann < spec.second.size(); ann++) {
                    rouletteCounter += spec.second[ann]->getFitness();
                    if (spec.second[ann]->getFitness() != 0 && roulette1 <= rouletteCounter &&
                        parents.back().first == nullptr)
                        parents.back().first = spec.second[ann];
                    if (spec.second[ann]->getFitness() != 0 && roulette2 <= rouletteCounter &&
                        parents.back().second == nullptr)
                        parents.back().second = spec.second[ann];
                    if (parents.back().first != nullptr && parents.back().second != nullptr) break;
                }
                if (parents.back().first == nullptr || parents.back().second == nullptr)
                    std::cout << "BAD PARENT SELECTION: " << parents.back().first << " " << parents.back().second
                              << std::endl;
            }
        }
    }
}

void NEAT::crossover() {
//    std::cout << "CROSS" << std::endl;
    for (auto &p : parents) {
        // crossover
        population.emplace_back(*p.first, *p.second);
        // mutations
        if ((float)(rand() % 1000) / 1000.0f < MUT_RATE) {
            int mutationRand = rand() % 3;
            switch (mutationRand) {
                case 0:
                    population.back().weightMutation();
                    break;
                case 1:
                    population.back().connectionMutation();
                    break;
                case 2:
                    population.back().nodeMutation();
                    break;
                default:
                    break;
            }
        }
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
                population.back().setSpecies(std::to_string(generationCount));
                std::cout << "-!- ADDED NEW SPECIES: " << std::to_string(generationCount) << std::endl;
            }
        } else {
            population.back().setSpecies(p.first->getSpecies());
        }
        addToSpecies(population.back());
    }
}

void NEAT::survivorSelection() {
    // TODO: other options than complete population replacement
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
            if (species[ann.getSpecies()].size() == 0) {
                species.erase(ann.getSpecies());
            }
            return;
        }
    }
}

void NEAT::printGenerationInfo() {
    std::deque<ANN> copy = population;
    std::sort(copy.begin(), copy.end(), ANN::fitnessSort);
    std::cout << "Gen: " << generationCount << " - MaxFit: " << copy.back().getFitness() << " - SpeciesNum: " << species.size() << std::endl;
    if (generationCount % 50 == 0) copy.back().printGenome();
}
