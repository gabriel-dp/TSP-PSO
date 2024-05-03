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
    Particle(Path& path) : actualPath(path), personalBestPath(Path(path)) {}

    Path& getActualPath() {
        return actualPath;
    }

    Path& getPersonalBestPath() {
        return personalBestPath;
    }

    void swap(int index1, int index2, EdgeGroup edges) {
        actualPath.swap(index1, index2, edges);
        if (actualPath.getCost() < personalBestPath.getCost()) {
            personalBestPath = Path(actualPath);
        }
    }

    static bool comparePersonalBestCost(Particle& a, Particle& b) {
        return a.getPersonalBestPath().getCost() < b.getPersonalBestPath().getCost();
    }

    static bool compareActualCost(Particle& a, Particle& b) {
        return a.getActualPath().getCost() < b.getActualPath().getCost();
    }
};

class PSO {
   private:
    Graph graph;
    int iterations = 0;
    int populationSize = 0;
    std::vector<Particle> particles;
    Particle* bestGlobalParticle = nullptr;

    std::vector<Path> generateRandomPaths(std::vector<Vertex> vertexes, int quantity) {
        std::vector<Path> paths;

        // Obtains a random seed
        std::random_device rd;
        std::mt19937 seed(rd());

        // Creates all random paths considering the graph is complete
        while (quantity--) {
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

    void run() {
        for (int i = 0; i < iterations; i++) {
            Particle* bestActualParticle = &(*std::min_element(particles.begin(), particles.end(), Particle::compareActualCost));

            for (Particle particle : particles) {
                /*
                    SINGLE PARTICLE LOGIC
                */
            }
        }
    }

   public:
    PSO(Graph graph, int populationSize, int iterations) : graph(graph) {
        this->populationSize = populationSize;
        this->iterations = iterations;

        // Creates the particles using random paths
        std::vector<Vertex> vertexes = graph.getVertexes().getVertexesVector();
        std::vector<Path> randomPaths = generateRandomPaths(vertexes, populationSize);
        for (Path path : randomPaths) {
            particles.push_back(Particle(path));
        }

        run();
    }

    Particle* getBestGlobalParticle() {
        return &(*std::min_element(particles.begin(), particles.end(), Particle::comparePersonalBestCost));
    }
};

#endif