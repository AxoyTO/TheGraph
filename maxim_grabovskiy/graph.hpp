#pragma once
#include <map>
#include <vector>
namespace uni_course_cpp {
using VertexId = int;
using EdgeId = int;
using Depth = int;

class Graph {
 public:
  struct Edge {
    enum class Color { Gray, Green, Yellow, Red };
    EdgeId const id;
    VertexId const fromVertexId;
    VertexId const toVertexId;
    Color const color;
    Edge(EdgeId _id, VertexId _fromVertexId, VertexId _toVertexId, Color _color)
        : id(_id),
          fromVertexId(_fromVertexId),
          toVertexId(_toVertexId),
          color(_color) {}
  };
  struct Vertex {
    VertexId const id;
    explicit Vertex(VertexId _id) : id(_id) {}
  };
  std::vector<EdgeId> const& getConnections(VertexId vertexId) const {
    return edgeConectionMap_.at(vertexId);
  }
  Depth const& getDepth(VertexId vertexId) const {
    return vertexDepthMap_.at(vertexId);
  }
  void setDepth(VertexId vertexId, Depth depth) {
    vertexDepthMap_[vertexId] = depth;
  }
  std::vector<EdgeId> const& getVertexIdByDepth(int depth) const {
    return vertexIdByDepth_[depth];
  }
  int getCurrentDepth() const { return vertexIdByDepth_.size(); }

  std::vector<EdgeId> const& getColorDistribution(Edge::Color color) const {
    return colorsDistributionMap.at(color);
  }
  std::map<Edge::Color, std::vector<EdgeId>> colorsDistributionMap;
  std::vector<Vertex> const& getVertexes() const { return vertexes_; }
  std::vector<Edge> const& getEdges() const { return edges_; }
  Graph::Edge const& getEdge(EdgeId id) const { return edges_[id]; }
  Graph::Vertex const& getVertex(VertexId id) const { return vertexes_[id]; }
  void addEdge(VertexId fromVertexId, VertexId toVertexId);
  VertexId addVertex();
  VertexId spawnVertex(VertexId parentId);
  bool isConnected(VertexId firstVertexId, VertexId secondVertexId) const;

 private:
  std::vector<Vertex> vertexes_;
  std::vector<Edge> edges_;
  std::map<VertexId, Depth> vertexDepthMap_;
  std::map<VertexId, std::vector<EdgeId>> edgeConectionMap_;
  VertexId vertexIdCounter_ = 0;
  EdgeId edgeIdCounter_ = 0;
  VertexId getNewVertexId() { return vertexIdCounter_++; }
  EdgeId getNewEdgeId() { return edgeIdCounter_++; }
  std::vector<std::vector<VertexId>> vertexIdByDepth_;
  Edge::Color calculate_color(VertexId fromVertexId, VertexId toVertexId);
};
}  // namespace uni_course_cpp
