#ifndef PSO_TSP_H
#define PSO_TSP_H

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "graph.hpp"

double randomZeroToOne();

typedef struct {
    Vertex vertex;
    int indexOrigin, indexCorrect;
    double coefficient;  // 0-1 scale
} SwapOperator;

class Velocity {
   private:
    std::vector<SwapOperator> swapOperators;

   public:
    std::vector<SwapOperator> getSwapOperators() {
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
   private:
    Path actualPath, personalBestPath;
    Velocity velocity;

   public:
    Particle(Path path) : actualPath(path), personalBestPath(Path(path)) {}

    Path& getActualPath() {
        return actualPath;
    }

    Path& getPersonalBestPath() {
        return personalBestPath;
    }

    Velocity& getVelocity() {
        return velocity;
    }

    static bool comparePersonalBestCost(Particle& a, Particle& b) {
        return a.getPersonalBestPath().getCost() < b.getPersonalBestPath().getCost();
    }

    static bool compareActualCost(Particle& a, Particle& b) {
        return a.getActualPath().getCost() < b.getActualPath().getCost();
    }

    void swap(int index1, int index2, EdgeGroup& edges) {
        actualPath.swap(index1, index2, edges);
        if (actualPath.getCost() < personalBestPath.getCost()) {
            personalBestPath = actualPath;
        }
    }

    void calculateVelocity(Path& gbest, double c1, double c2) {
        velocity.reset();

        for (int i = 0; i < (int)actualPath.getVertexes().size(); i++) {
            Vertex particleVertexAtIndex = actualPath.getVertexes().at(i);

            // Personal best swaps
            Vertex pbestVertexAtIndex = personalBestPath.getVertexes().at(i);
            Vertex pbestVertexCorrectIndex = actualPath.getPosition(pbestVertexAtIndex);
            if (particleVertexAtIndex != pbestVertexAtIndex) {
                velocity.insert(pbestVertexAtIndex, i, pbestVertexCorrectIndex, c1);
            }

            // Global best swaps
            Vertex gbestVertexAtIndex = gbest.getVertexes().at(i);
            Vertex gbestVertexCorrectIndex = actualPath.getPosition(gbestVertexAtIndex);
            if (particleVertexAtIndex != gbestVertexAtIndex) {
                velocity.insert(gbestVertexAtIndex, gbestVertexCorrectIndex, i, c2);
            }
        }
    }

    void makeSwaps(EdgeGroup& edges) {
        for (SwapOperator& swapOperator : velocity.getSwapOperators()) {
            // Calculates the swap probability using the coefficient
            bool isPossible = randomZeroToOne() < swapOperator.coefficient;

            // Checks if the vertex is already on its correct position
            bool isNecessary = actualPath.getVertexes().at(swapOperator.indexCorrect) != swapOperator.vertex;

            if (isPossible && isNecessary) {
                swap(swapOperator.indexOrigin, swapOperator.indexCorrect, edges);
            }
        }

        // Avoids repeating same swaps
        velocity.reset();
    }
};

class PSO {
   private:
    Graph graph;
    int iterations = 0;
    int populationSize = 0;
    std::vector<Particle> particles;

    const double c1 = 0.5;
    const double c2 = 0.5;

    std::vector<Path> generateRandomPaths(std::vector<Vertex>& vertexes, int quantity) {
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
            printAllParticles();

            Path gbest = getBestGlobalParticle()->getPersonalBestPath();
            for (Particle& particle : particles) {
                particle.calculateVelocity(gbest, c1, c2);
                particle.makeSwaps(graph.getEdges());
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
        for (Path& path : randomPaths) {
            particles.push_back(Particle(path));
        }

        run();
    }

    Particle* getBestGlobalParticle() {
        return &(*std::min_element(particles.begin(), particles.end(), Particle::comparePersonalBestCost));
    }

    void printAllParticles() {
        std::cout << "All particles: \n";
        for (Particle& p : particles) {
            p.getActualPath().print();
        }
        std::cout << "\n";
    }
};

#endif