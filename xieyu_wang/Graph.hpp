#pragma once
#include "Edge.hpp"
#include "Vertex.hpp"
namespace uni_course_cpp {
class Graph {
 public:
  const std::vector<Vertex>& getVertices() const;
  const Vertex& addVertex();
  void addEdge(int fromVertexId, int toVertexId, const Edge::Color color);
  bool hasVertex(int idFind);
  bool isConnected(int fromVertexId, int toVertexId);
  std::vector<int> getVertexIdsAtDepth(int depth);
  std::vector<Edge> getNumEdgeByColor(const Edge::Color& color);
  const std::vector<Edge>& getEdges() const;

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
}  // namespace uni_course_cpp
