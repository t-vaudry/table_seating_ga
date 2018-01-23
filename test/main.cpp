#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "config.h"
#include "Individual.h"
#include "functions.h"

void setup() { srand(unsigned(time(NULL))); }

TEST_CASE("Testing Swap Mutation", "[Mutate]") {
    setup();
    Individual i = Individual(5);
    std::vector<int> v = i.GetSeatingArrangement();
    i.Mutate(SWAP);

    // for (auto x : v) std::cout << x << ' '; std::cout << std::endl;
    // for (auto x : i.GetSeatingArrangement()) std::cout << x << ' '; std::cout << std::endl;
}

TEST_CASE("Testing Insert Mutation", "[Mutate]") {
    setup();
    Individual i = Individual(5);
    std::vector<int> v = i.GetSeatingArrangement();
    i.Mutate(INSERT);

    // for (auto x : v) std::cout << x << ' '; std::cout << std::endl;
    // for (auto x : i.GetSeatingArrangement()) std::cout << x << ' '; std::cout << std::endl;
}

TEST_CASE("Testing Scramble Mutation", "[Mutate]") {
    setup();
    Individual i = Individual(8);
    std::vector<int> v = i.GetSeatingArrangement();
    i.Mutate(SCRAMBLE);

    // for (auto x : v) std::cout << x << ' '; std::cout << std::endl;
    // for (auto x : i.GetSeatingArrangement()) std::cout << x << ' '; std::cout << std::endl;
}

TEST_CASE("Testing Inversion Mutation", "[Mutate]") {
    setup();
    Individual i = Individual(8);
    std::vector<int> v = i.GetSeatingArrangement();
    i.Mutate(INVERSION);

    // for (auto x : v) std::cout << x << ' '; std::cout << std::endl;
    // for (auto x : i.GetSeatingArrangement()) std::cout << x << ' '; std::cout << std::endl;
}

TEST_CASE("Testing PMX", "[Crossover]") {
    setup();
    Individual* father = new Individual(std::vector<int> {3,4,7,6,2,9,1,8,5,10,14,15,-1,12,0,13,-2,11});
    Individual* mother = new Individual(std::vector<int> {7,-2,14,13,3,12,9,0,-1,10,11,8,6,5,15,1,2,4});

    auto children = Crossover(PMX, father, mother);

    // for (auto x : children.first->GetSeatingArrangement()) std::cout << x << ' '; std::cout << std::endl;
    // for (auto x : children.second->GetSeatingArrangement()) std::cout << x << ' '; std::cout << std::endl;
}

TEST_CASE("Testing full pass") {
    setup();
    std::vector< std::vector<int> > preferences { { 0,5,3,3,3,5,3,3,3,3,3,3,3,3,3 },
                                                  { 5,0,5,3,3,3,3,3,3,3,3,3,3,3,3 },
                                                  { 3,5,0,5,3,3,3,3,3,3,3,3,3,3,3 },
                                                  { 3,3,5,0,5,3,3,3,3,3,3,3,3,3,3 },
                                                  { 3,3,3,5,0,5,3,3,3,3,3,3,3,3,3 },
                                                  { 5,3,3,3,5,0,3,3,3,3,3,3,3,3,3 },
                                                  { 3,3,3,3,3,3,0,5,3,3,3,3,3,3,3 },
                                                  { 3,3,3,3,3,3,5,0,5,3,3,3,3,3,3 },
                                                  { 3,3,3,3,3,3,3,5,0,5,3,3,3,3,3 },
                                                  { 3,3,3,3,3,3,3,3,5,0,5,3,3,3,3 },
                                                  { 3,3,3,3,3,3,3,3,3,5,0,3,3,3,3 },
                                                  { 3,3,3,3,3,3,3,3,3,3,3,0,5,3,5 },
                                                  { 3,3,3,3,3,3,3,3,3,3,3,5,0,5,3 },
                                                  { 3,3,3,3,3,3,3,3,3,3,3,3,5,0,3 },
                                                  { 3,3,3,3,3,3,3,3,3,3,3,5,3,3,0 } };

    Configuration configuration = Configuration(6, 15, std::vector<std::string>(), preferences);
    Population population;
    Individual* champion = nullptr;

    // 1: Initialize Population
    InitializePopulation(population, configuration);

    // 2: Evaluate Fitness
    EvaluateFitness(population, configuration);
    champion = FindChampion(population);

    // 3: while not Termination do
    int numOfGenerations = 0;
    while (numOfGenerations < configuration.sMaxGenerations && champion->GetFitness() != 0) {
        // 4: Parent Selection
        Population parents = ParentSelection(population, configuration);

        // 5: Perform Crossover & Mutation
        Population offspring = GenerateOffspring(parents, configuration);

        // 6: Evaluate Fitness
        EvaluateFitness(offspring, configuration);

        // 7: Survivor Selection
        population = SurvivorSelection(parents, offspring, configuration);

        // 8: Find champion
        champion = FindChampion(population);
        numOfGenerations++;
    }

    REQUIRE(champion->GetFitness() == 0);
}