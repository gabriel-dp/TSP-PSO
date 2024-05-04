#include "../include/pso.hpp"

#include <cstdlib>
#include <ctime>
#include <vector>

#include "graph.hpp"

double randomZeroToOne() {
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(nullptr)));
        seeded = true;
    }

    // Generate a random number between 0 and RAND_MAX
    int randNum = rand();

    // Normalize the random number to be between 0 and 1
    return static_cast<double>(randNum) / RAND_MAX;
}
