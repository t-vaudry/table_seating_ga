#pragma once

#ifndef __INDIVIDUAL_H__
#define __INDIVIDUAL_H__

#include "dll.h"

#include <vector>
#include <algorithm>

class Individual;
typedef std::vector<Individual*> Population;
typedef std::pair<Individual*, Individual*> Children;

enum EvolutionType {
    CLONE,
    MUTATION,
    CROSSOVER
};

enum MutationType {
    SWAP,
    INSERT,
    SCRAMBLE,
    INVERSION // Look for others
};

enum CrossoverType {
    ORDER,
    PMX,
    CYCLE,
    EDGE // Look for others
};

class Individual
{
private:
    std::vector<int> mSeatingArrangement;
    int mSize;
    int mFitness;
public:
    DLL_PUBLIC Individual(int size, int zeros = 0);
    DLL_PUBLIC Individual(std::vector<int> arrangement) : mSeatingArrangement(arrangement), mSize(arrangement.size()) {}
    DLL_PUBLIC Individual(const Individual& individual);

    inline DLL_PUBLIC std::vector<int> GetSeatingArrangement() { return mSeatingArrangement; }
    inline DLL_PUBLIC void SetFitness(int value) { mFitness = value; }
    inline DLL_PUBLIC int GetFitness() { return mFitness; }

    DLL_PUBLIC void Mutate(MutationType type);
};

#endif // __INDIVIDUAL_H__