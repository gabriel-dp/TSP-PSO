#ifndef PSO_TSP_H
#define PSO_TSP_H

#include <algorithm>
#include <iostream>
#include <vector>

#include "graph.hpp"
#include "random.hpp"

using namespace std;

// Swap operators works like a "swap request"
typedef struct {
    Vertex vertex;
    int indexOrigin, indexCorrect;
    double coefficient;  // 0-1 scale
} SwapOperator;

class Velocity {
   private:
    vector<SwapOperator> swapOperators;

   public:
    vector<SwapOperator> getSwapOperators() {
        return swapOperators;
    }

    void insert(Vertex vertex, int indexOrigin, int indexCorrect, double coefficient) {
        SwapOperator newSwap;
        newSwap.vertex = vertex;
        newSwap.indexOrigin = indexOrigin;
        newSwap.indexCorrect = indexCorrect;
        newSwap.coefficient = coefficient;

        swapOperators.push_back(newSwap);
    }

    void reset() {
        swapOperators.clear();
    }
};

class Particle {
   public:
    Path actualPath, personalBestPath;
    Velocity velocity;
    Particle(Path path) : actualPath(path), personalBestPath(Path(path)) {}

    static bool comparePersonalBestCost(Particle& a, Particle& b) {
        return a.personalBestPath.getCost() < b.personalBestPath.getCost();
    }

    static bool compareActualCost(Particle& a, Particle& b) {
        return a.actualPath.getCost() < b.actualPath.getCost();
    }

    void swap(int index1, int index2, EdgeGroup& edges) {
        actualPath.swap(index1, index2, edges);
        if (actualPath.getCost() < personalBestPath.getCost()) {
            personalBestPath = actualPath;
        }
    }
};

// Essential functions
void updateVelocity(Particle& particle, Path& gbest, double c1, double c2);
void makeSwaps(Particle& particle, EdgeGroup& edges);

class PSO {
   private:
    Graph graph;
    int iterations = 0;
    int populationSize = 0;
    vector<Particle> particles;

    const double c1 = 0.5;
    const double c2 = 0.8;

    vector<Path> generateRandomPaths(vector<Vertex>& vertexes, int quantity) {
        vector<Path> paths;

        mt19937 seed = randomSeed();

        // Creates all random paths considering the graph is complete
        while (quantity--) {
            shuffle(vertexes.begin(), vertexes.end(), seed);

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
            printAllParticles();

            Path gbest = getBestGlobalParticle()->personalBestPath;
            for (Particle& particle : particles) {
                updateVelocity(particle, gbest, c1, c2);
                makeSwaps(particle, graph.getEdges());
            }
        }
    }

   public:
    PSO(Graph graph, int populationSize, int iterations) : graph(graph) {
        this->populationSize = populationSize;
        this->iterations = iterations;

        // Creates the particles using random paths
        vector<Vertex> vertexes = graph.getVertexes().getVertexesVector();
        vector<Path> randomPaths = generateRandomPaths(vertexes, populationSize);
        for (Path& path : randomPaths) {
            particles.push_back(Particle(path));
        }

        run();
    }

    Particle* getBestGlobalParticle() {
        return &(*min_element(particles.begin(), particles.end(), Particle::comparePersonalBestCost));
    }

    void printAllParticles() {
        cout << "All particles: \n";
        for (Particle& p : particles) {
            p.actualPath.print();
        }
        cout << "\n";
    }
};

#endif
