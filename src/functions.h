#pragma once

#include "dll.h"
#include "Individual.h"

DLL_PUBLIC std::pair<Individual, Individual> Crossover(CrossoverType type, Individual& father, Individual& mother);