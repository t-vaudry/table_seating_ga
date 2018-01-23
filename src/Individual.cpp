#include "Individual.h"

DLL_PUBLIC Individual::Individual(int size, int empty) : mSize(size + empty) {
    for (int i = 1; i <= size; i++) {
        mSeatingArrangement.push_back(i);
    }

    for (int i = 0; i < empty; i++) {
        mSeatingArrangement.push_back(0 - i);
    }

    std::random_shuffle(mSeatingArrangement.begin(), mSeatingArrangement.end());
}

DLL_PUBLIC Individual::Individual(const Individual& parent) {
    mSeatingArrangement = parent.mSeatingArrangement;
    mSize = parent.mSize;
    mFitness = parent.mFitness;
}

DLL_PUBLIC void Individual::Mutate(MutationType type)
{
    int idx1 = 0, idx2 = 0;
    int value = -1;
    while (idx1 == idx2) {
        idx1 = rand() % mSize;
        idx2 = rand() % mSize;
    }

    if (idx1 > idx2) {
        std::swap(idx1, idx2);
    }

    switch (type) {
    case SWAP:
        std::swap(mSeatingArrangement[idx1], mSeatingArrangement[idx2]);
        break;
    case INSERT:
        value = mSeatingArrangement[idx2];
        mSeatingArrangement.erase(mSeatingArrangement.begin() + idx2);
        mSeatingArrangement.insert(mSeatingArrangement.begin() + idx1 + 1, value);
        break;
    case SCRAMBLE:
        std::random_shuffle(mSeatingArrangement.begin() + idx1, mSeatingArrangement.begin() + idx2 + 1);
        break;
    case INVERSION:
        std::reverse(mSeatingArrangement.begin() + idx1, mSeatingArrangement.begin() + idx2 + 1);
        break;
    };
}