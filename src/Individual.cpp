#include "Individual.h"
#include "functions.h"

DLL_PUBLIC Individual::Individual(int size) : mSize(size) {
    for (int i = 1; i <= size; i++) {
        mSeatingArrangement.push_back(i);
    }

    std::random_shuffle(mSeatingArrangement.begin(), mSeatingArrangement.end());
}

DLL_PUBLIC Individual::Individual(const Individual& parent) {
    mSeatingArrangement = parent.mSeatingArrangement;
    mSize = parent.mSize;
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

DLL_PUBLIC std::pair<Individual, Individual> Crossover(CrossoverType type, Individual& father, Individual& mother)
{
    switch (type) {
    case ORDER: {
        break;
    }
    case PMX: {
        int idx1 = 0, idx2 = 0;
        int size = father.GetSeatingArrangement().size();
        while (idx1 == idx2) {
            idx1 = rand() % size;
            idx2 = rand() % size;
        }

        if (idx1 > idx2) {
            std::swap(idx1, idx2);
        }

        std::vector<int> fatherSeats = father.GetSeatingArrangement();
        std::vector<int> motherSeats = mother.GetSeatingArrangement();

        std::vector<int> sonSeats(size, -1);
        std::vector<int> daughterSeats(size, -1);

        for (int i = idx1; i <= idx2; i++) {
            sonSeats[i] = fatherSeats[i];
            daughterSeats[i] = motherSeats[i];
        }

        int index = idx1;
        while (std::find(sonSeats.begin(), sonSeats.end(), -1) != sonSeats.end()) {
            auto it = std::find(sonSeats.begin(), sonSeats.end(), motherSeats[index]);
            if (it == sonSeats.end()) { // Not in vector
                int idx = index;
                while (sonSeats[idx] != -1) {
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
        while (std::find(daughterSeats.begin(), daughterSeats.end(), -1) != daughterSeats.end()) {
            auto it = std::find(daughterSeats.begin(), daughterSeats.end(), fatherSeats[index]);
            if (it == daughterSeats.end()) { // Not in vector
                int idx = index;
                while (daughterSeats[idx] != -1) {
                    idx = std::distance(fatherSeats.begin(), std::find(fatherSeats.begin(), fatherSeats.end(), motherSeats[idx]));
                }

                daughterSeats[idx] = fatherSeats[index];
            }

            index++;
            if (index == size) {
                index = 0;
            }
        }

        return std::make_pair(Individual(sonSeats), Individual(daughterSeats));
    }
    case CYCLE:
        break;
    case EDGE:
        break;
    };

    return std::make_pair(NULL, NULL);
}