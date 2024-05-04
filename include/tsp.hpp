#ifndef TSP_H
#define TSP_H

#include <vector>

#include "graph.hpp"

// Base problem types
typedef double Coordinate;
typedef std::pair<Coordinate, Coordinate> Coordinates;
typedef struct {
    Vertex vertex;
    Coordinates position;
} City;

// Essential functions
double calculateDistance(Coordinates coords1, Coordinates coords2);
Graph generateCompleteGraph(std::vector<City>& cities);

#endif