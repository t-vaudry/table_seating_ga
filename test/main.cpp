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

TEST_CASE("Testing Fitness", "[EvaluateFitness]") {
    setup();

    std::vector< std::vector<int> > preferences{ { 0,5,3,3,3,5,3,3,3,3,3,3,3,3,3 },
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
    Individual* individual = new Individual(std::vector<int> {2,3,4,5,6,13,12,7,8,9,10,11,0,-1,15,14,1,-2});

    Population population;
    population.push_back(individual);

    EvaluateFitness(population, configuration);

    REQUIRE(individual->GetFitness() == 200);
}

TEST_CASE("Testing Diversity Measurement", "[MeasureDiversity]") {
    setup();
    Configuration configuration = Configuration(4, 6, std::vector<std::string> {}, std::vector< std::vector<int> > {});
    Individual* individual1 = new Individual(std::vector<int> {1,2,3,4,5,6,-1,0});
    Individual* individual2 = new Individual(std::vector<int> {-1,5,6,0,1,2,3,4});

    int diversity = MeasureDiversity(individual1, individual2, configuration);

    REQUIRE(diversity == 0);
}

TEST_CASE("Testing Single Ideal Solution", "[!mayfail]") {
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
    std::vector<Population> population;
    for (int i = 0; i < configuration.sNumberOfIslands; i++) {
        // 1: Initialize Population
        population.push_back(Population());
        InitializePopulation(population[i], configuration);

        // 2: Evaluate Fitness
        EvaluateFitness(population[i], configuration);
    }

    int numOfGenerations = 0;

    // 3: while not Termination do
    while (numOfGenerations < configuration.sMaxGenerations) {

        for (int i = 0; i < configuration.sNumberOfIslands; i++) {
            // 4: Parent Selection
            Population parents = ParentSelection(population[i], configuration);

            // 5: Perform Crossover & Mutation
            Population offspring = GenerateOffspring(parents, configuration);

            // 6: Evaluate Fitness
            EvaluateFitness(offspring, configuration);

            // 7: Survivor Selection
            population[i] = SurvivorSelection(parents, offspring, configuration);
        }

        if (numOfGenerations % configuration.sEpochLength == 0) {
            // 9: Migration
            MigratePopulations(population, configuration);
        }

        numOfGenerations++;
    }

    Population mergedPopulation;
    for (int i = 0; i < configuration.sNumberOfIslands; i++) {
        mergedPopulation.insert(mergedPopulation.end(), population[i].begin(), population[i].end());
    }

    Individual* champion;
    champion = FindChampion(mergedPopulation);

    REQUIRE(champion->GetFitness() == 0);
}

TEST_CASE("Testing Multiple Ideal Solutions", "[!mayfail]") {
    setup();
    std::vector< std::vector<int> > preferences { { 0,4,4,4,4,4,3,3,3,3,3,3,3,3,3 },
                                                  { 4,0,4,4,4,4,3,3,3,3,3,3,3,3,3 },
                                                  { 4,4,0,4,4,4,3,3,3,3,3,3,3,3,3 },
                                                  { 4,4,4,0,4,4,3,3,3,3,3,3,3,3,3 },
                                                  { 4,4,4,4,0,4,3,3,3,3,3,3,3,3,3 },
                                                  { 4,4,4,4,4,0,3,3,3,3,3,3,3,3,3 },
                                                  { 3,3,3,3,3,3,0,4,4,4,4,3,3,3,3 },
                                                  { 3,3,3,3,3,3,4,0,4,4,4,3,3,3,3 },
                                                  { 3,3,3,3,3,3,4,4,0,4,4,3,3,3,3 },
                                                  { 3,3,3,3,3,3,4,4,4,0,4,3,3,3,3 },
                                                  { 3,3,3,3,3,3,4,4,4,4,0,3,3,3,3 },
                                                  { 3,3,3,3,3,3,3,3,3,3,3,0,4,4,4 },
                                                  { 3,3,3,3,3,3,3,3,3,3,3,4,0,4,4 },
                                                  { 3,3,3,3,3,3,3,3,3,3,3,4,4,0,4 },
                                                  { 3,3,3,3,3,3,3,3,3,3,3,4,4,4,0 } };

    Configuration configuration = Configuration(6, 15, std::vector<std::string>(), preferences);
    std::vector<Population> population;
    for (int i = 0; i < configuration.sNumberOfIslands; i++) {
        // 1: Initialize Population
        population.push_back(Population());
        InitializePopulation(population[i], configuration);

        // 2: Evaluate Fitness
        EvaluateFitness(population[i], configuration);
    }

    int numOfGenerations = 0;

    // 3: while not Termination do
    while (numOfGenerations < configuration.sMaxGenerations) {

        for (int i = 0; i < configuration.sNumberOfIslands; i++) {
            // 4: Parent Selection
            Population parents = ParentSelection(population[i], configuration);

            // 5: Perform Crossover & Mutation
            Population offspring = GenerateOffspring(parents, configuration);

            // 6: Evaluate Fitness
            EvaluateFitness(offspring, configuration);

            // 7: Survivor Selection
            population[i] = SurvivorSelection(parents, offspring, configuration);
        }

        if (numOfGenerations % configuration.sEpochLength == 0) {
            // 9: Migration
            MigratePopulations(population, configuration);
        }

        numOfGenerations++;
    }

    Population mergedPopulation;
    for (int i = 0; i < configuration.sNumberOfIslands; i++) {
        mergedPopulation.insert(mergedPopulation.end(), population[i].begin(), population[i].end());
    }

    Individual* champion;
    champion = FindChampion(mergedPopulation);

    REQUIRE(champion->GetFitness() == 0);
}

TEST_CASE("Testing No Ideal Solution", "[!mayfail]") {
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
                                                  { 3,3,3,3,3,3,3,3,3,3,3,0,5,3,3 },
                                                  { 3,3,3,3,3,3,3,3,3,3,3,5,0,5,3 },
                                                  { 3,3,3,3,3,3,3,3,3,3,3,3,5,0,3 },
                                                  { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,0 } };

    Configuration configuration = Configuration(6, 15, std::vector<std::string>(), preferences);
    std::vector<Population> population;
    for (int i = 0; i < configuration.sNumberOfIslands; i++) {
        // 1: Initialize Population
        population.push_back(Population());
        InitializePopulation(population[i], configuration);

        // 2: Evaluate Fitness
        EvaluateFitness(population[i], configuration);
    }

    int numOfGenerations = 0;

    // 3: while not Termination do
    while (numOfGenerations < configuration.sMaxGenerations) {

        for (int i = 0; i < configuration.sNumberOfIslands; i++) {
            // 4: Parent Selection
            Population parents = ParentSelection(population[i], configuration);

            // 5: Perform Crossover & Mutation
            Population offspring = GenerateOffspring(parents, configuration);

            // 6: Evaluate Fitness
            EvaluateFitness(offspring, configuration);

            // 7: Survivor Selection
            population[i] = SurvivorSelection(parents, offspring, configuration);
        }

        if (numOfGenerations % configuration.sEpochLength == 0) {
            // 9: Migration
            MigratePopulations(population, configuration);
        }

        numOfGenerations++;
    }

    Population mergedPopulation;
    for (int i = 0; i < configuration.sNumberOfIslands; i++) {
        mergedPopulation.insert(mergedPopulation.end(), population[i].begin(), population[i].end());
    }

    Individual* champion;
    champion = FindChampion(mergedPopulation);

    REQUIRE(champion->GetFitness() == 30);
}