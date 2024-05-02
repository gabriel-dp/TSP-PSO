#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

#define INVALID_LENGTH 0

// Base graph types
typedef int Vertex;
typedef double Length;
typedef std::tuple<Vertex, Vertex, Length> Edge;

// Works like a two-dimesion matrix, but mapping the values
// Complexity of insert, search and delete are O(1)
// Complexity of space is O(V(V-1)/2)
typedef std::unordered_map<Vertex, std::unordered_map<Vertex, Length>> EdgesType;

// Set of vertexes
class VertexGroup {
   private:
    std::set<Vertex> vertexes;  // Avoids repeating a vertex

   public:
    std::set<Vertex> getVertexes() {
        return vertexes;
    }

    std::vector<Vertex> getVertexesVector() {
        return std::vector(vertexes.begin(), vertexes.end());
    }

    void insert(Vertex v) {
        vertexes.insert(v);
    }

    void remove(Vertex v) {
        vertexes.erase(v);
    }

    bool exists(Vertex v) {
        return vertexes.find(v) != vertexes.end();
    }

    void clear() {
        vertexes.clear();
    }

    void print() {
        for (Vertex v : vertexes) {
            std::cout << v << "\n";
        }
    }
};

// Non-directional edges
class EdgeGroup {
   private:
    EdgesType edges;

    // Orders two vertexes based on deterministic criteria
    std::pair<Vertex, Vertex> vertexes(Vertex v1, Vertex v2) {
        return std::pair(std::min(v1, v2), std::max(v1, v2));
    }

   public:
    EdgesType getEdges() {
        return edges;
    }

    // Converts the mapping list into a vector of edges
    std::vector<Edge> getEdgesVector() {
        std::vector<Edge> edgesVector;
        for (auto firstVertex : edges) {
            for (auto secondVertex : firstVertex.second) {
                Edge newEdge = std::tuple(firstVertex.first, secondVertex.first, secondVertex.second);
                edgesVector.push_back(newEdge);
            }
        }
        return edgesVector;
    }

    void insert(Vertex v1, Vertex v2, Length length) {
        // Avoids self-loops
        if (v1 == v2) return;

        edges[vertexes(v1, v2).first][vertexes(v1, v2).second] = length;
    }

    void remove(Vertex v1, Vertex v2) {
        edges[vertexes(v1, v2).first].erase(vertexes(v1, v2).second);

        // Removes the first vertex if it does not have edges anymore
        if (edges[vertexes(v1, v2).first].empty()) {
            edges.erase(vertexes(v1, v2).first);
        }
    }

    Length getLength(Vertex v1, Vertex v2) {
        // Tries to get the first vertex of the edge if it exists
        auto firstVertex = edges.find(vertexes(v1, v2).first);
        if (firstVertex == edges.end()) {
            return INVALID_LENGTH;
        }

        // Tries to get the second vertex of the edge if it exists
        auto secondVertex = firstVertex->second.find(vertexes(v1, v2).second);
        if (secondVertex == firstVertex->second.end()) {
            return INVALID_LENGTH;
        }

        // Returns the length of the found edge
        return secondVertex->second;
    }

    void print() {
        for (Edge e : getEdgesVector()) {
            std::cout << std::get<0>(e) << " <-> " << std::get<1>(e) << " (" << std::get<2>(e) << ")\n";
        }
    }
};

class Path {
   private:
    std::vector<Vertex> vertexes;
    Length cost = -1;

   public:
    std::vector<Vertex> getVertexes() {
        return vertexes;
    }

    Length getCost() {
        return cost;
    }

    void insert(Vertex v) {
        vertexes.push_back(v);
    }

    void swap(int index1, int index2, EdgeGroup edges) {
        // Check if swap is valid
        if (index1 < 0 || index2 < 0 || vertexes.size() - 1 < index1 || vertexes.size() - 1 < index2) {
            return;
        }

        Vertex aux = vertexes[index1];
        vertexes[index1] = vertexes[index2];
        vertexes[index2] = aux;

        calculateCost(edges);
    }

    Length calculateCost(EdgeGroup edges) {
        if (vertexes.empty()) {
            cost = -1;
        } else {
            cost = 0;
            for (auto i = vertexes.begin(); std::next(i) != vertexes.end(); i++) {
                cost += edges.getLength(*i, *std::next(i));
            }
            cost += edges.getLength(*vertexes.rbegin(), *vertexes.begin());
        }

        return getCost();
    }

    void print() {
        if (vertexes.empty()) {
            std::cout << "Empty path\n";
            return;
        }

        for (Vertex v : vertexes) {
            std::cout << v << " -> ";
        }
        std::cout << vertexes.at(0) << " (" << cost << ")\n";
    }
};

// Non-directional graph
class Graph {
   private:
    VertexGroup vertexes;
    EdgeGroup edges;

   public:
    VertexGroup getVertexes() {
        return vertexes;
    }

    EdgeGroup getEdges() {
        return edges;
    }

    void addVertex(Vertex v) {
        vertexes.insert(v);
    }

    void addEdge(Vertex v1, Vertex v2, Length length) {
        addVertex(v1);
        addVertex(v2);
        edges.insert(v1, v2, length);
    }

    Length edgeLength(Vertex v1, Vertex v2) {
        return edges.getLength(v1, v2);
    }

    void print() {
        vertexes.print();
        edges.print();
    }
};

#endif