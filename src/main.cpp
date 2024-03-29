#include <iostream>
#include <time.h>
#include <algorithm>
#include <cstdlib>
#include <fstream>

#include "functions.h"

#define TESTING false

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void printProgress(float percentage)
{
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

using namespace std;

int main()
{
    srand(unsigned(time(NULL)));

    // Output diversity to file
    std::ofstream file;

    // 0: Setup
    Configuration configuration = Configuration("../src/input/settings.txt", "../src/input/guests.csv");
    //Configuration configuration = Configuration("C:/Users/thoma/Documents/GitHub/table_seating_ga/src/input/settings.txt", "C:/Users/thoma/Documents/GitHub/table_seating_ga/src/input/guests.csv");

    std::vector<Population> population;
    for (int i = 0; i < configuration.sNumberOfIslands; i++) {
        // 1: Initialize Population
        population.push_back(Population());
        InitializePopulation(population[i], configuration);

        // 2: Evaluate Fitness
        EvaluateFitness(population[i], configuration);
    }

    int numOfGenerations = 0;

    if (TESTING) {
        file.open("../src/output/diversity.csv", std::ios::app);
    }

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

        if (TESTING) {
            // 8: Evaluate Diversity
            file << EvaluateDiversity(population, configuration) << ",";
        }

        if (numOfGenerations % configuration.sEpochLength == 0) {
            // 9: Migration
            MigratePopulations(population, configuration);
        }

        numOfGenerations++;

        printProgress((float)numOfGenerations / (float)configuration.sMaxGenerations);
    }

    if (TESTING) {
        file.close();
    }

    Population mergedPopulation;
    for (int i = 0; i < configuration.sNumberOfIslands; i++) {
        mergedPopulation.insert(mergedPopulation.end(), population[i].begin(), population[i].end());
    }

    Individual* champion;
    champion = FindChampion(mergedPopulation);

    // 10: Output
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
            MutationType mutation;
            float mutate = (float)rand() / (float)RAND_MAX;
            temp = 0.0f;

            for (int i = 0; i < 4; i++) {
                temp += configuration.sMutationParameters[i];
                if (mutate <= temp) {
                    mutation = static_cast<MutationType>(i);
                    break;
                }
            }
            offspring.push_back(new Individual(*parents[rand() % configuration.sParentSize]));
            offspring.back()->Mutate(mutation);
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

    // Elitism
    Individual* elitist = nullptr;

    for (auto individual : currentGeneration) {
        if (elitist == nullptr) {
            elitist = individual;
        }
        else if (individual->GetFitness() < elitist->GetFitness()) {
            elitist = individual;
        }
    }

    nextGeneration.push_back(elitist);
    currentGeneration.erase(std::remove(currentGeneration.begin(), currentGeneration.end(), elitist), currentGeneration.end());

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

DLL_PUBLIC int EvaluateDiversity(std::vector<Population> populations, Configuration& configuration)
{
    std::vector<int> fittestIndividualsIndex;
    int populationSize = configuration.sPopulationSize * configuration.sNumberOfIslands;

    Population population;
    for (int i = 0; i < configuration.sNumberOfIslands; i++) {
        population.insert(population.end(), populations[i].begin(), populations[i].end());
    }

    Individual* champion = FindChampion(population);

    for (int i = 0; i < populationSize; i++) {
        if (population[i]->GetFitness() <= champion->GetFitness() + configuration.sDiversityRange) {
            fittestIndividualsIndex.push_back(i);
        }
    }

    // UNIQUE INDIVIDUALS
    for (size_t i = 0; i < fittestIndividualsIndex.size(); i++) {
        std::vector<int> indicesToRemove;
        for (size_t j = i + 1; j < fittestIndividualsIndex.size(); j++) {
            if (MeasureDiversity(population[fittestIndividualsIndex[i]], population[fittestIndividualsIndex[j]], configuration) == 0) {
                indicesToRemove.push_back(j);
            }

            while (indicesToRemove.size() != 0) {
                fittestIndividualsIndex.erase(fittestIndividualsIndex.begin() + indicesToRemove.back());
                indicesToRemove.pop_back();
            }
        }
    }

    int diversity = 0;
    int numOfFit = fittestIndividualsIndex.size();

    std::ofstream file;
    file.open("../src/output/numbers.csv", std::ios::app);
    file << numOfFit << ",";
    file.close();

    for (int i = 0; i < numOfFit; i++) {
        for (int j = i + 1; j < numOfFit; j++) {
            diversity += MeasureDiversity(population[fittestIndividualsIndex[i]], population[fittestIndividualsIndex[j]], configuration);
        }
    }

    return diversity;
}

DLL_PUBLIC int MeasureDiversity(Individual* a, Individual* b, Configuration& configuration)
{
    int diversity = 0;
    int numOfGuests = configuration.GetNumberOfGuests();
    int tableSize = configuration.GetTableSize();
    int numOfTables = configuration.GetNumberOfTables();

    std::vector<int> seatingA = a->GetSeatingArrangement();
    std::vector<int> seatingB = b->GetSeatingArrangement();

    int numOfSeats = tableSize * numOfTables;
    for (int i = 0; i < numOfSeats; i++) {
        if (seatingA[i] < 0) {
            seatingA[i] = 0;
        }

        if (seatingB[i] < 0) {
            seatingB[i] = 0;
        }
    }

    for (int i = 1; i <= numOfGuests; i++) {
        auto indexA = std::distance(seatingA.begin(), std::find(seatingA.begin(), seatingA.end(), i));
        auto indexB = std::distance(seatingB.begin(), std::find(seatingB.begin(), seatingB.end(), i));

        bool seatingMatched = false;
        int positionMatched = 0; // 0 for nextToR and 1 for nextToL

        int rightA = indexA + 1, rightB = indexB + 1;
        int leftA = indexA - 1, leftB = indexB - 1;

        if (indexA % tableSize == 0) { // first element in subarray
            leftA = indexA + tableSize - 1;
        } else if ((indexA + 1) % tableSize == 0) { // last element in subarray
            rightA = indexA - (tableSize - 1);
        }

        if (indexB % tableSize == 0) { // first element in subarray
            leftB = indexB + tableSize - 1;
        } else if ((indexB + 1) % tableSize == 0) { // last element in subarray
            rightB = indexB - (tableSize - 1);
        }

        if ((seatingA[rightA] != seatingB[rightB]) && (seatingA[leftA] != seatingB[leftB])) {
            diversity++;
        } else {
            seatingMatched = true;

            if (seatingA[rightA] == seatingB[rightB]) {
                positionMatched = 0;
            } else {
                positionMatched = 1;
            }
        }

        if (!seatingMatched) {
            if ((seatingA[leftA] != seatingB[rightB]) && (seatingA[rightA] != seatingB[leftB])) {
                diversity++;
            }
        } else {
            if ((!positionMatched) && (seatingA[leftA] != seatingB[leftB])) {
                diversity++;
            } else if ((positionMatched) && (seatingA[rightA] != seatingB[rightB])) {
                diversity++;
            }
        }

        int temp = 0;
        int emptyA = 0;
        int emptyB = 0;

        int tableA = indexA / tableSize;
        int tableB = indexB / tableSize;

        for (int j = tableA * tableSize; j < (tableA + 1) * tableSize; j++) {
            if (j == indexA) {
                continue;
            }

            if (seatingA[j] == 0) {
                emptyA++;
                continue;
            }

            auto indexBPrime = std::distance(seatingB.begin(), std::find(seatingB.begin(), seatingB.end(), seatingA[j]));
            if ((indexBPrime / tableSize) == tableB) {
                temp++;
            }
        }

        for (int j = tableB * tableSize; j < (tableB + 1) * tableSize; j++) {
            if (seatingB[j] == 0) {
                emptyB++;
            }
        }

        temp += std::min(emptyA, emptyB);
        diversity += (tableSize - 1 - temp);
    }

    return diversity;
}

DLL_PUBLIC void MigratePopulations(std::vector<Population> population, Configuration& configuration)
{
    std::vector<int> populationNumber;
    for (int i = 0; i < configuration.sNumberOfIslands; i++) {
        populationNumber.push_back(i);
    }

    std::random_shuffle(populationNumber.begin(), populationNumber.end());
    for (int i = 0; i < configuration.sNumberOfIslands; i += 2) {
        for (int j = 0; j < configuration.sMigrationSize; j++) {
            int random = rand() % configuration.sPopulationSize;
            std::swap(population[populationNumber[i]][random], population[populationNumber[i + 1]][random]);
        }
    }
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