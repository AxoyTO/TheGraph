#ifndef TASK_04_GRAPH_H
#define TASK_04_GRAPH_H
#include <cassert>
#include <vector>
#include "vertex.h"
#include "edge.h"
#include <stdexcept>
class Graph {
public:
    void addVertex();
    void addEdge(int fromVertexId, int toVertexId);
    std::string toString() const;
    bool hasVertex(int idFind);
    bool isConnected(int fromVertexId, int toVertexId);
    int getPresentVertexID();
private:
    Vertex& getVertex(int id);
    int getNextEdgeId() { return vertexIdCounter_++; }
    int getNextVertexId() { return edgeIdCounter_++; }
    int vertexIdCounter_ = 0;
    int edgeIdCounter_ = 0;
    std::vector<Vertex> vertices_;
    std::vector<Edge> edges_;
};
#endif //TASK_04_GRAPH_H
