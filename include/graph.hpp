#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <map>
#include <set>
#include <vector>

#define INVALID_LENGTH 0

// Base graph types
typedef int Vertex;
typedef double Length;
typedef std::tuple<Vertex, Vertex, Length> Edge;

// Set of vertexes
class VertexGroup {
   private:
    // Avoids repeating a vertex
    std::set<Vertex> vertexes;

   public:
    std::set<Vertex> getVertexes() {
        return vertexes;
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
    // Works like a two-dimesion matrix
    std::map<Vertex, std::map<Vertex, Length>> edges;

    // Orders two vertexes based on deterministic criteria
    std::pair<Vertex, Vertex> vertexes(Vertex v1, Vertex v2) {
        return std::pair(std::min(v1, v2), std::max(v1, v2));
    }

   public:
    // Converts the matrix into a vector of edges
    std::vector<Edge> getEdges() {
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
        for (Edge e : getEdges()) {
            std::cout << std::get<0>(e) << " <-> " << std::get<1>(e) << " (" << std::get<2>(e) << ")\n";
        }
    }
};

// Non-directional graph
class Graph {
   private:
    VertexGroup vertexes;
    EdgeGroup edges;

   public:
    std::set<Vertex> getVertexes() {
        return vertexes.getVertexes();
    }

    std::vector<Edge> getEdges() {
        return edges.getEdges();
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