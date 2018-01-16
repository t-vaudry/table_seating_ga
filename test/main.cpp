#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "functions.h"

TEST_CASE("Test1") {
    int a = 1;
    int b = 2;
    REQUIRE(a != b);
}

TEST_CASE("Test2", "[add]") {
    int a = 1;
    int b = 2;
    REQUIRE(add(a, b) == 3);
}