#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

// #include "Snake.hpp"
#include <catch.hpp>


unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "move that is already valid", "[snake]" ) {
}
