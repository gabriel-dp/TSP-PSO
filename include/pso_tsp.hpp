#ifndef PSO_TSP_H

#include <algorithm>
#include <iostream>
#include <random>
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
Graph generateCompleteGraph(std::vector<City> cities);

class Particle {
   private:
    Path actualPath, personalBestPath;

   public:
    Particle(Path& path) : actualPath(path) {}
};

class PSO {
   private:
    Graph graph;
    int iterations;
    int populationSize;
    std::vector<Particle> particles;

    std::vector<Path> generateRandomPaths(std::vector<Vertex> vertexes, int times) {
        std::vector<Path> paths;

        // Obtain a random seed
        std::random_device rd;
        std::mt19937 seed(rd());

        while (times--) {
            std::shuffle(vertexes.begin(), vertexes.end(), seed);

            Path newPath = Path();
            for (Vertex v : vertexes) {
                newPath.insert(v);
            }
            newPath.calculateCost(graph.getEdges());

            paths.push_back(newPath);
        }

        return paths;
    }

   public:
    PSO(Graph graph, int populationSize, int iterations) : graph(graph), populationSize(populationSize), iterations(iterations) {
        std::vector<Vertex> vertexes = graph.getVertexes().getVertexesVector();
        std::vector<Path> randomPaths = generateRandomPaths(vertexes, populationSize);
        for (Path path : randomPaths) {
            path.print();
            particles.push_back(Particle(path));
        }
    }
};

#endif