#pragma once
#include <unordered_map>
#include "Edge.hpp"
#include "Vertex.hpp"
namespace uni_course_cpp {
class Graph {
 public:
  const std::vector<Vertex>& getVertices() const;
  const Vertex& addVertex();
  void addEdge(int fromVertexId, int toVertexId, const Edge::Color color);
  bool hasVertex(int idFind) const;
  bool isConnected(int fromVertexId, int toVertexId) const;
  std::vector<int> getVertexIdsAtDepth(int depth);
  const std::vector<int>& getEdgesByColor(const Edge::Color& color) const;
  const std::vector<Edge>& getEdges() const;
  int getDepth();

 private:
  const Vertex& getVertex(int id) const;
  Vertex& getVertex(int id);
  const Edge& getEdge(int id) const;
  int getNextEdgeId() { return vertexIdCounter_++; }
  int getNextVertexId() { return edgeIdCounter_++; }
  int vertexIdCounter_ = 0;
  int edgeIdCounter_ = 0;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::vector<std::vector<int>> depthMap_ = {{}};
  std::unordered_map<Edge::Color, std::vector<int>> coloredEdges_;
};
}  // namespace uni_course_cpp
