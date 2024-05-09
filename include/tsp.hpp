#ifndef TSP_H
#define TSP_H

#include <vector>

using namespace std;

#include "graph.hpp"

// Base problem types
typedef double Coordinate;
typedef pair<Coordinate, Coordinate> Coordinates;
typedef struct {
    Vertex vertex;
    Coordinates position;
} City;

// Essential functions
double calculateDistance(Coordinates coords1, Coordinates coords2);
Graph generateCompleteGraph(vector<City>& cities);

#endif