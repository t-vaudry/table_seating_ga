#pragma once

#include "dll.h"
#include "config.h"
#include "Individual.h"

DLL_PUBLIC Children Crossover(CrossoverType type, Individual* father, Individual* mother);
DLL_PUBLIC void EvaluateFitness(Population& population, Configuration& configuration);
DLL_PUBLIC Individual* FindChampion(Population& population);
DLL_PUBLIC Population GenerateOffspring(Population& parents, Configuration& configuration);
DLL_PUBLIC void GenerateOutputFile(Individual* champion, Configuration& configuration);
DLL_PUBLIC void InitializePopulation(Population& population, Configuration& configuration);
DLL_PUBLIC Population ParentSelection(Population& population, Configuration& configuration);
DLL_PUBLIC Population SurvivorSelection(Population& parents, Population& offspring, Configuration& configuration);