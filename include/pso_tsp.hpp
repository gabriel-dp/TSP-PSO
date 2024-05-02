#ifndef PSO_TSP_H

#include "graph.hpp"

typedef double Coordinate;
typedef std::pair<Coordinate, Coordinate> Coordinates;
typedef struct {
    Vertex vertex;
    Coordinates position;
} City;

double calculateDistance(Coordinates coords1, Coordinates coords2);
Graph generateCompleteGraph(std::vector<City> cities);

#endif