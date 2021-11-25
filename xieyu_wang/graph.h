#ifndef TASK_04_GRAPH_H
#define TASK_04_GRAPH_H
#include <cassert>
#include <stdexcept>
#include <vector>
#include "edge.h"
#include "vertex.h"
class Graph {
 public:
  Graph();
  Vertex addVertex();
  void addEdge(int fromVertexId, int toVertexId, Edge::Color color);
  std::string toString() const;
  bool hasVertex(int idFind);
  bool isConnected(int fromVertexId, int toVertexId);
  int getPresentVertexId();

  std::vector<int> getVertexIdsAtDepth(int depth);

 private:
  Vertex& getVertex(int id);
  int getNextEdgeId() { return vertexIdCounter_++; }
  int getNextVertexId() { return edgeIdCounter_++; }
  int vertexIdCounter_ = 0;
  int edgeIdCounter_ = 0;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
};
#endif  // TASK_04_GRAPH_H
