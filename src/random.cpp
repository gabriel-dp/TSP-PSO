#include "../include/random.hpp"

#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

double randomZeroToOne() {
    // Lazy "singleton" seed generation
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

mt19937 randomSeed() {
    random_device rd;
    mt19937 seed(rd());
    return seed;
}