#include <iostream>
#include <time.h>
#include <algorithm>
#include <cstdlib>
#include <fstream>

#include "functions.h"

using namespace std;

int main()
{
    srand(unsigned(time(NULL)));

    // 0: Setup
    Configuration configuration = Configuration("../src/input/settings.txt", "../src/input/guests.csv");
    Population population;
    Individual* champion = nullptr;

    // 1: Initialize Population
    InitializePopulation(population, configuration);

    // 2: Evaluate Fitness
    EvaluateFitness(population, configuration);
    champion = FindChampion(population);

    // 3: while not Termination do
    int numOfGenerations = 0;
    while (numOfGenerations < configuration.sMaxGenerations || champion->GetFitness() != 0) {
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

    // 9: Output
    GenerateOutputFile(champion, configuration);
}

DLL_PUBLIC void InitializePopulation(Population& population, Configuration& configuration)
{
    int numOfGuests = configuration.GetNumberOfGuests();
    int emptySeats = configuration.GetEmptySeats();
    int popSize = configuration.sPopulationSize;

    for (int i = 0; i < popSize; i++) {
        population.push_back(new Individual(numOfGuests, emptySeats));
    }
}

DLL_PUBLIC Individual* FindChampion(Population& population)
{
    Individual* fittest = nullptr;

    for (auto individual : population) {
        if (fittest == nullptr) {
            fittest = individual;
        } else if (individual->GetFitness() < fittest->GetFitness()) {
            fittest = individual;
        }
    }

    return fittest;
}

DLL_PUBLIC void EvaluateFitness(Population& population, Configuration& configuration)
{
    std::vector< std::vector<int> > preferences = configuration.GetPreferences();
    int numOfGuests = configuration.GetNumberOfGuests();
    int tableSize = configuration.GetTableSize();

    for (auto individual : population) {
        int fitness = 0;
        std::vector<int> arrangement = individual->GetSeatingArrangement();
        for (int i = 1; i <= numOfGuests; i++) {
            auto indexI = std::distance(arrangement.begin(), std::find(arrangement.begin(), arrangement.end(), i));

            for (int j = 1; j <= numOfGuests; j++) {
                if (i == j) continue;

                auto indexJ = std::distance(arrangement.begin(), std::find(arrangement.begin(), arrangement.end(), j));

                switch (preferences[i - 1][j - 1]) {
                case 1: { // Not near to
                    if ((indexI / tableSize) == (indexJ / tableSize)) {
                        int diff = std::abs(indexI - indexJ);
                        if (diff == 1 || diff == tableSize - 1) {
                            fitness += 15;
                        } else {
                            fitness += 10;
                        }
                    }
                    break;
                }
                case 2: { // Not next to
                    if ((indexI / tableSize) == (indexJ / tableSize)) {
                        int diff = std::abs(indexI - indexJ);
                        if (diff == 1 || diff == tableSize - 1) {
                            fitness += 15;
                        }
                    }
                    break;
                }
                case 3: { // Neutral
                    break;
                }
                case 4: { // Sits near to
                    if ((indexI / tableSize) != (indexJ / tableSize)) {
                        fitness += 10;
                    }
                    break;
                }
                case 5: { // Sits next to
                    if ((indexI / tableSize) != (indexJ / tableSize)) {
                        fitness += 20;
                    } else {
                        int diff = std::abs(indexI - indexJ);
                        if (diff != 1 && diff != tableSize - 1) {
                            fitness += 15;
                        }
                    }
                    break;
                }
                };
            }
        }

        individual->SetFitness(fitness);
    }
}

DLL_PUBLIC Population ParentSelection(Population& population, Configuration& configuration)
{
    // TODO: consider replacement
    int parentSize = configuration.sParentSize;
    Population parents;

    while ((int) parents.size() < parentSize) {
        int index = -1;
        Individual* fittest = nullptr;
        int popSize = population.size();

        for (int i = 0; i < 3; i++) {
            int random = rand() % popSize;
            Individual* temp = population[random];

            if (fittest == nullptr) {
                fittest = temp;
                index = random;
            } else if (fittest->GetFitness() > temp->GetFitness()) {
                fittest = temp;
                index = random;
            }
        }

        parents.push_back(fittest);
        population.erase(population.begin() + index);
    }

    return parents;
}

DLL_PUBLIC Population GenerateOffspring(Population& parents, Configuration& configuration)
{
    Population offspring;
    int offspringSize = configuration.sOffspringSize;

    while ((int) offspring.size() < offspringSize) {
        EvolutionType type;
        float evolve = (float)rand() / (float) RAND_MAX;
        float temp = 0.0f;

        for (int i = 0; i < 3; i++) {
            temp += configuration.sEvolutionParameters[i];
            if (evolve <= temp) {
                type = static_cast<EvolutionType>(i);
                break;
            }
        }

        switch (type) {
        case CLONE: {
            offspring.push_back(new Individual(*parents[rand() % configuration.sParentSize]));
        }
        case MUTATION: {
            offspring.push_back(new Individual(*parents[rand() % configuration.sParentSize]));
            offspring.back()->Mutate(static_cast<MutationType>(rand() % 4));
        }
        case CROSSOVER: {
            Children children = Crossover(PMX, parents[rand() % configuration.sParentSize], parents[rand() % configuration.sParentSize]);
            offspring.push_back(children.first);
            offspring.push_back(children.second);
        }
        };
    }

    return offspring;
}

DLL_PUBLIC Children Crossover(CrossoverType type, Individual* father, Individual* mother)
{
    switch (type) {
    case ORDER: {
        break;
    }
    case PMX: {
        int idx1 = 0, idx2 = 0;
        int size = father->GetSeatingArrangement().size();
        while (idx1 == idx2) {
            idx1 = rand() % size;
            idx2 = rand() % size;
        }

        if (idx1 > idx2) {
            std::swap(idx1, idx2);
        }

        std::vector<int> fatherSeats = father->GetSeatingArrangement();
        std::vector<int> motherSeats = mother->GetSeatingArrangement();

        std::vector<int> sonSeats(size, std::numeric_limits<int>::min());
        std::vector<int> daughterSeats(size, std::numeric_limits<int>::min());

        for (int i = idx1; i <= idx2; i++) {
            sonSeats[i] = fatherSeats[i];
            daughterSeats[i] = motherSeats[i];
        }

        int index = idx1;
        while (std::find(sonSeats.begin(), sonSeats.end(), std::numeric_limits<int>::min()) != sonSeats.end()) {
            auto it = std::find(sonSeats.begin(), sonSeats.end(), motherSeats[index]);
            if (it == sonSeats.end()) { // Not in vector
                int idx = index;
                while (sonSeats[idx] != std::numeric_limits<int>::min()) {
                    idx = std::distance(motherSeats.begin(), std::find(motherSeats.begin(), motherSeats.end(), fatherSeats[idx]));
                }

                sonSeats[idx] = motherSeats[index];
            }

            index++;
            if (index == size) {
                index = 0;
            }
        }

        index = idx1;
        while (std::find(daughterSeats.begin(), daughterSeats.end(), std::numeric_limits<int>::min()) != daughterSeats.end()) {
            auto it = std::find(daughterSeats.begin(), daughterSeats.end(), fatherSeats[index]);
            if (it == daughterSeats.end()) { // Not in vector
                int idx = index;
                while (daughterSeats[idx] != std::numeric_limits<int>::min()) {
                    idx = std::distance(fatherSeats.begin(), std::find(fatherSeats.begin(), fatherSeats.end(), motherSeats[idx]));
                }

                daughterSeats[idx] = fatherSeats[index];
            }

            index++;
            if (index == size) {
                index = 0;
            }
        }

        return std::make_pair(new Individual(sonSeats), new Individual(daughterSeats));
    }
    case CYCLE:
        break;
    case EDGE:
        break;
    };

    return std::make_pair(nullptr, nullptr);
}

DLL_PUBLIC Population SurvivorSelection(Population& parents, Population& offspring, Configuration& configuration)
{
    Population nextGeneration;
    Population currentGeneration;
    currentGeneration.insert(currentGeneration.end(), parents.begin(), parents.end());
    currentGeneration.insert(currentGeneration.end(), offspring.begin(), offspring.end());
    std::random_shuffle(currentGeneration.begin(), currentGeneration.end());

    int popSize = configuration.sPopulationSize;

    while ((int)nextGeneration.size() < popSize) {
        int index = -1;
        Individual* fittest = nullptr;
        int currentSize = currentGeneration.size();

        for (int i = 0; i < 3; i++) {
            int random = rand() % currentSize;
            Individual* temp = currentGeneration[random];

            if (fittest == nullptr) {
                fittest = temp;
                index = random;
            }
            else if (fittest->GetFitness() > temp->GetFitness()) {
                fittest = temp;
                index = random;
            }
        }

        nextGeneration.push_back(fittest);
        currentGeneration.erase(currentGeneration.begin() + index);
    }

    return nextGeneration;
}

DLL_PUBLIC void GenerateOutputFile(Individual* champion, Configuration& configuration)
{
    std::ofstream file;
    file.open("../src/output/output.csv");
    file << "Name,Table Number,Seat Number\n";

    int numOfGuests = configuration.GetNumberOfGuests();
    std::vector<std::string> names = configuration.GetNames();
    std::vector<int> seatingArrangement = champion->GetSeatingArrangement();
    int tableSize = configuration.GetTableSize();

    for (int i = 0; i < numOfGuests; i++) {
        std::string temp = names[i];
        auto index = std::distance(seatingArrangement.begin(), std::find(seatingArrangement.begin(), seatingArrangement.end(), i + 1));
        temp.append(",");
        temp.append(std::to_string((index / tableSize) + 1));
        temp.append(",");
        temp.append(std::to_string((index % tableSize) + 1));
        temp.append("\n");
        file << temp;
    }

    file.close();
}