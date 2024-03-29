#pragma once

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <math.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "dll.h"

class Configuration
{
private:
    int mTableSize; // size of each table
    int mNumOfGuests; // number of guests
    std::vector<std::string> mNames;
    std::vector< std::vector<int> > mPreferences;
    int mNumOfTables;
public:
    DLL_PUBLIC Configuration(int k, int n, std::vector<std::string> names, std::vector< std::vector<int> > preferences)
    {
        mTableSize = k;
        mNumOfGuests = n;
        mNames = names;
        mPreferences = preferences;

        mNumOfTables = (int) std::ceil((double)mNumOfGuests / (double)mTableSize);
    }

    DLL_PUBLIC Configuration(char* settings, char* guests)
    {
        std::ifstream set(settings);
        if (!set) {
            std::cout << "ERROR";
            return;
        }

        std::string line;
        getline(set, line);
        mTableSize = std::stoi(line);
        getline(set, line);
        mNumOfGuests = std::stoi(line);

        mNumOfTables = (int)std::ceil((double)mNumOfGuests / (double)mTableSize);

        set.close();

        std::ifstream guest(guests);
        if (!guest) {
            std::cout << "ERROR";
            return;
        }

        getline(guest, line, ','); // ignore first empty space
        int index = 0;
        while (index < mNumOfGuests - 1) {
            getline(guest, line, ',');
            mNames.push_back(line);
            index++;
        }

        getline(guest, line);
        mNames.push_back(line);

        for (int i = 0; i < mNumOfGuests; i++) {
            getline(guest, line, ','); // ignore first empty space
            index = 0;
            std::vector<int> preferences;
            while (index < mNumOfGuests - 1) {
                getline(guest, line, ',');
                preferences.push_back(line.compare("") == 0 ? 0 : std::stoi(line));
                index++;
            }

            getline(guest, line);
            preferences.push_back(line.compare("") == 0 ? 0 : std::stoi(line));

            mPreferences.push_back(preferences);
        }

        guest.close();
    }

    inline DLL_PUBLIC int GetNumberOfGuests() { return mNumOfGuests; }
    inline DLL_PUBLIC int GetEmptySeats() { return mNumOfTables * mTableSize - mNumOfGuests; }
    inline DLL_PUBLIC std::vector< std::vector<int> > GetPreferences() { return mPreferences; }
    inline DLL_PUBLIC int GetTableSize() { return mTableSize; }
    inline DLL_PUBLIC std::vector<std::string> GetNames() { return mNames; }
    inline DLL_PUBLIC int GetNumberOfTables() { return mNumOfTables; }

    static const int sPopulationSize = 100;
    static const int sParentSize = 20;
    static const int sOffspringSize = 300;
    static const int sMaxGenerations = 1000;
    static const int sEpochLength = 50;
    static const int sDiversityRange = 0;
    static const int sNumberOfIslands = 6;
    static const int sMigrationSize = 5;

    static const float sClone;
    static const float sMutation;
    static const float sCrossover;
    static const float sEvolutionParameters[3];

    static const float sMutationSwap;
    static const float sMutationInsert;
    static const float sMutationScramble;
    static const float sMutationInversion;
    static const float sMutationParameters[4];
};

const float Configuration::sClone = 0.00f;
const float Configuration::sMutation = 0.85f;
const float Configuration::sCrossover = 0.15f;
const float Configuration::sEvolutionParameters[3] = { sClone, sMutation, sCrossover };

const float Configuration::sMutationSwap = 0.25f;
const float Configuration::sMutationInsert = 0.10f;
const float Configuration::sMutationScramble = 0.05f;
const float Configuration::sMutationInversion = 0.60f;
const float Configuration::sMutationParameters[4] = { sMutationSwap, sMutationInsert, sMutationScramble, sMutationInversion };

#endif