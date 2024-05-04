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
double randomZeroToOne();

class SwapOperator {
   private:
    Vertex vertex;
    int indexOrigin, indexCorrect;
    double coefficient;

   public:
    SwapOperator(Vertex vertex, int indexOrigin, int indexCorrect, double coefficient) : vertex(vertex), indexOrigin(indexOrigin), indexCorrect(indexCorrect), coefficient(coefficient) {}

    Vertex getVertex() {
        return vertex;
    }

    int getIndexOrigin() {
        return indexOrigin;
    }

    int getIndexCorrect() {
        return indexCorrect;
    }

    double getCoefficient() {
        return coefficient;
    }
};

class Velocity {
   private:
    std::vector<SwapOperator> swapOperators;

   public:
    std::vector<SwapOperator> getSwapOperators() {
        return swapOperators;
    }

    void insert(Vertex vertex, int indexOrigin, int indexCorrect, double coefficient) {
        SwapOperator newSwap = SwapOperator(vertex, indexOrigin, indexCorrect, coefficient);
        swapOperators.push_back(newSwap);
    }

    void clear() {
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

    void swap(int index1, int index2, EdgeGroup edges) {
        actualPath.swap(index1, index2, edges);
        if (actualPath.getCost() < personalBestPath.getCost()) {
            personalBestPath = actualPath;
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

    double c1 = 0.5;
    double c2 = 0.75;

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
            printAllParticles();
            Path gbest = getBestGlobalParticle()->getPersonalBestPath();

            for (Particle& particle : particles) {
                Path pbest = Path(particle.getPersonalBestPath());

                for (int i = 0; i < graph.getVertexes().length(); i++) {
                    Vertex particleVertexAtIndex = particle.getActualPath().getVertexes().at(i);

                    Vertex pbestVertexAtIndex = pbest.getVertexes().at(i);
                    Vertex pbestVertexCorrectIndex = particle.getActualPath().getPosition(pbestVertexAtIndex);
                    if (particleVertexAtIndex != pbestVertexAtIndex) {
                        particle.getVelocity().insert(pbestVertexAtIndex, i, pbestVertexCorrectIndex, c1);
                    }

                    Vertex gbestVertexAtIndex = gbest.getVertexes().at(i);
                    Vertex gbestVertexCorrectIndex = particle.getActualPath().getPosition(gbestVertexAtIndex);
                    if (particleVertexAtIndex != gbestVertexAtIndex) {
                        particle.getVelocity().insert(gbestVertexAtIndex, gbestVertexCorrectIndex, i, c2);
                    }
                }

                for (SwapOperator swap : particle.getVelocity().getSwapOperators()) {
                    if (randomZeroToOne() < swap.getCoefficient()) {
                        Vertex actualVertex = particle.getActualPath().getVertexes().at(swap.getIndexCorrect());
                        if (actualVertex != swap.getVertex()) {
                            particle.swap(swap.getIndexOrigin(), swap.getIndexCorrect(), graph.getEdges());
                        }
                    }
                }

                particle.getVelocity().clear();
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

    void printAllParticles() {
        std::cout << "All particles: \n";
        for (Particle p : particles) {
            p.getActualPath().print();
        }
        std::cout << "\n";
    }
};

#endif