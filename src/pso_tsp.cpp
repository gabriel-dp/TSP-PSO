#include "../include/pso_tsp.hpp"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "graph.hpp"

// Euclidian distance between two points
double calculateDistance(Coordinates coords1, Coordinates coords2) {
    Coordinate firstDiff = coords1.first - coords2.first;
    Coordinate secondDiff = coords1.second - coords2.second;
    Length distance = sqrt(firstDiff * firstDiff + secondDiff * secondDiff);
    return distance;
}

// Insert and connect all cities in the graph
Graph generateCompleteGraph(std::vector<City> cities) {
    Graph graph = Graph();
    for (auto i = cities.begin(); i != cities.end(); i++) {
        for (auto j = std::next(i); j != cities.end(); j++) {
            Length distance = calculateDistance(i->position, j->position);
            graph.addEdge(i->vertex, j->vertex, distance);
        }
    }
    return graph;
}

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
