//
// Created by Sheldon Woodward on 5/12/18.
//

#include "NEAT.hpp"

NEAT::NEAT(int sizeX, int sizeY, unsigned int seed) : snake(sizeX, sizeY) {
    srand(seed);
    population = std::list<ANN>();
    parents = std::deque<std::pair<ANN *, ANN *>>();
    species = std::map<std::string, std::vector<ANN *>>();
    generationCount = 0;
}

// train
void NEAT::train() {
    // training
    populate();
    int i = 0;
    while (i > -1) {
        parentSelection();
        crossover();
        survivorSelection();
        if (generationCount % 50 == 0) printGenerationInfo();
        i++;
    }
}

// genetic algorithm
void NEAT::populate() {
    // initial population
    population.clear();
    for (int p = 0; p < POP_SIZE; p++) {
        // add to population
        population.emplace_back(3, 3, "0");
        // setup constructs
        population.back().connectionMutation();
        population.back().setFitness(snake.fitness(population.back()));
        addToSpecies(population.back());
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
        if (fitnessSum == 0 || PS_ALG == "RAND") {
            for (int i = 0; i < spec.second.size(); i++) {
                parents.emplace_back();
                parents.back().first = spec.second[rand() % spec.second.size()];
                parents.back().second = spec.second[rand() % spec.second.size()];
            }
        } else if (PS_ALG == "RWS") {
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
            }
        } else if (PS_ALG == "TS") {
            for (int i = 0; i < spec.second.size(); i++) {
                // wheel prep
                parents.emplace_back();
                for (int r = 0; r < TS_K * 2; r++) {
                    // TODO: could choose same ANN twice
                    unsigned long roulette = rand() % spec.second.size();
                    if (r % 2 == 0 && (parents.back().first == nullptr || spec.second[roulette]->getFitness() > parents.back().first->getFitness())) {
                        parents.back().first = spec.second[roulette];
                    } else if (parents.back().second == nullptr || spec.second[roulette]->getFitness() > parents.back().second->getFitness()) {
                        parents.back().second = spec.second[roulette];
                    }
                }
            }
        }
    }
}

void NEAT::crossover() {
    for (auto &p : parents) {
        // crossover
        population.emplace_back(*p.first, *p.second);
        // mutations
        if ((float) (rand() % 10000) / 10000.0f < MUT_RATE) {
            int mutationRand = rand() % (CONN_MUT_RATIO + NODE_MUT_RATIO + WEIGHT_MUT_RATIO);
            if (mutationRand < CONN_MUT_RATIO) population.back().connectionMutation();
            else if (mutationRand < CONN_MUT_RATIO + NODE_MUT_RATIO) population.back().nodeMutation();
            else population.back().weightMutation();
        }
        // fitness
        population.back().setFitness(snake.fitness(population.back()));
        // speciation
        if ((float) (rand() % 1000) / 1000.0f < SPEC_RATE) {
            bool foundSpecies = false;
            for (auto &s : species) {
                if (s.second.size() == 0) continue;
                unsigned long randomIndex = rand() % s.second.size();
                if (ANN::compatibility(*s.second[randomIndex], population.back(), COMP_C0, COMP_C1, COMP_C2) <
                    SPEC_THRESH) {
                    population.back().setSpecies(s.first);
                    foundSpecies = true;
                    break;
                }
            }
            if (!foundSpecies) {
                // TODO: new species being killed immediately without time to develop
                population.back().setSpecies(
                        std::to_string(generationCount) + "-" + std::to_string(population.back().getId()));
//                std::cout << "-!- ADDED NEW SPECIES: " << std::to_string(generationCount) + "-" + std::to_string(population.back().getId()) << std::endl;
            }
        } else {
            population.back().setSpecies(p.first->getSpecies());
        }
        addToSpecies(population.back());
    }
}

void NEAT::survivorSelection() {
    // sort population
    if (SS_ALG == "ABS") population.sort(ANN::ageSort);
    else if (SS_ALG == "FBS") population.sort(ANN::fitnessSort);
    // kill survivors
    for (int i = 0; i < POP_SIZE; i++) {
        removeFromSpecies(population.front());
        population.pop_front();
    }
    // increment age of survivors
    for (auto &ann : population) ann.incrementAge();
    generationCount++;
}

// speciation
void NEAT::addToSpecies(ANN &ann) {
    species[ann.getSpecies()].push_back(&ann);
}

void NEAT::removeFromSpecies(ANN &ann) {
    for (auto a = species.at(ann.getSpecies()).begin(); a != species.at(ann.getSpecies()).end(); ++a) {
        if (*a == &ann) {
            species[ann.getSpecies()].erase(a);
            if (species[ann.getSpecies()].empty()) {
                species.erase(ann.getSpecies());
            }
            return;
        }
    }
}

// info dump
void NEAT::printGenerationInfo() {
    population.sort(ANN::fitnessSort);
    std::cout << "Gen: " << generationCount << " - MinFit: " << population.front().getFitness() << " - MaxFit: "
              << population.back().getFitness() << " - PopSize: " << population.size() << " - SpeciesNum: "
              << species.size() << std::endl;
    if (generationCount % 500 == 0) {
        population.back().dumpTopology("/Users/sheldonwoodward/Desktop/ann-dumps");
    }
}
