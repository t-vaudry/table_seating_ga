#define CATCH_CONFIG_MAIN
#include "catch.hpp"

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
    Individual father = Individual(8);
    Individual mother = Individual(8);

    auto children = Crossover(PMX, father, mother);

    // for (auto x : children.first.GetSeatingArrangement()) std::cout << x << ' '; std::cout << std::endl;
    // for (auto x : children.second.GetSeatingArrangement()) std::cout << x << ' '; std::cout << std::endl;
}