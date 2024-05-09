#include "../include/pso.hpp"

void updateVelocity(Particle& particle, Path& gbest, double c1, double c2) {
    particle.velocity.reset();

    for (int i = 0; i < (int)particle.actualPath.getVertexes().size(); i++) {
        Vertex particleVertexAtIndex = particle.actualPath.getVertexes().at(i);

        // Personal best swaps
        Vertex pbestVertexAtIndex = particle.personalBestPath.getVertexes().at(i);
        Vertex pbestVertexCorrectIndex = particle.actualPath.getPosition(pbestVertexAtIndex);
        if (particleVertexAtIndex != pbestVertexAtIndex) {
            particle.velocity.insert(pbestVertexAtIndex, i, pbestVertexCorrectIndex, c1);
        }

        // Global best swaps
        Vertex gbestVertexAtIndex = gbest.getVertexes().at(i);
        Vertex gbestVertexCorrectIndex = particle.actualPath.getPosition(gbestVertexAtIndex);
        if (particleVertexAtIndex != gbestVertexAtIndex) {
            particle.velocity.insert(gbestVertexAtIndex, gbestVertexCorrectIndex, i, c2);
        }
    }
}

void makeSwaps(Particle& particle, EdgeGroup& edges) {
    for (SwapOperator& swapOperator : particle.velocity.getSwapOperators()) {
        // Calculates the swap probability using the coefficient
        bool isPossible = randomZeroToOne() < swapOperator.coefficient;

        // Checks if the vertex is already on its correct position
        bool isNecessary = particle.actualPath.getVertexes().at(swapOperator.indexCorrect) != swapOperator.vertex;

        if (isPossible && isNecessary) {
            particle.swap(swapOperator.indexOrigin, swapOperator.indexCorrect, edges);
        }
    }

    // Avoids repeating same swaps
    particle.velocity.reset();
}