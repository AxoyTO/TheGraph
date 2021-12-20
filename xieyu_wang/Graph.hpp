#pragma once
#include "Edge.hpp"
#include "Vertex.hpp"
namespace uni_cpp_practice {
class Graph {
    public:
        const std::vector<Vertex>& getVertices() const;
        const Vertex& addVertex();
        void addEdge(int fromVertexId, int toVertexId, const Edge::Color color);
        std::string toString() const;
        bool hasVertex(int idFind);
        bool isConnected(int fromVertexId, int toVertexId);
        std::vector<int> getVertexIdsAtDepth(int depth);
        int getNumEdgeByColor(Edge::Color color);
        const std::vector<Edge>& getEdges();

    private:
        Vertex& getVertex(int id);
        const Edge& getEdge(int id);
        int getNextEdgeId() { return vertexIdCounter_++; }
        int getNextVertexId() { return edgeIdCounter_++; }
        int vertexIdCounter_ = 0;
        int edgeIdCounter_ = 0;
        std::vector<Vertex> vertices_;
        std::vector<Edge> edges_;
    };
}

