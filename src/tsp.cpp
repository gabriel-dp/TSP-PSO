#include "../include/tsp.hpp"

#include <cmath>
#include <vector>

// Euclidian distance between two points
double calculateDistance(Coordinates coords1, Coordinates coords2) {
    Coordinate firstDiff = coords1.first - coords2.first;
    Coordinate secondDiff = coords1.second - coords2.second;
    Length distance = sqrt(firstDiff * firstDiff + secondDiff * secondDiff);
    return distance;
}

// Insert and connect all cities in the graph
Graph generateCompleteGraph(std::vector<City>& cities) {
    Graph graph = Graph();
    for (auto i = cities.begin(); i != cities.end(); i++) {
        for (auto j = std::next(i); j != cities.end(); j++) {
            Length distance = calculateDistance(i->position, j->position);
            graph.addEdge(i->vertex, j->vertex, distance);
        }
    }
    return graph;
}