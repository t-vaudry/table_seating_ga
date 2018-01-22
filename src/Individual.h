#pragma once

#ifndef __INDIVIDUAL_H__
#define __INDIVIDUAL_H__

#include "dll.h"

#include <vector>
#include <algorithm>

class Individual;
typedef std::vector<Individual&> Population;

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
public:
    DLL_PUBLIC Individual(int size);
    DLL_PUBLIC Individual(std::vector<int> arrangement) : mSeatingArrangement(arrangement), mSize(arrangement.size()) {}
    DLL_PUBLIC Individual(const Individual& individual);

    inline DLL_PUBLIC std::vector<int> GetSeatingArrangement() { return mSeatingArrangement; }

    DLL_PUBLIC void Mutate(MutationType type);
};

#endif // __INDIVIDUAL_H__