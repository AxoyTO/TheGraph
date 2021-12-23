#pragma once
#include <map>
#include <vector>
namespace uni_course_cpp {
using std::map;
using std::vector;
using VertexId = int;
using EdgeId = int;
using Depth = int;

class Graph {
 public:
  struct ColorsDistribution {
    int Gray = 0;
    int Green = 0;
    int Yellow = 0;
    int Red = 0;
  };
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
  vector<EdgeId> const& getConnections(VertexId vertexId) const {
    return edgeConectionMap_.at(vertexId);
  }
  Depth const& getDepth(VertexId vertexId) const {
    return vertexDepthMap_.at(vertexId);
  }
  void setDepth(VertexId vertexId, Depth depth) {
    vertexDepthMap_[vertexId] = depth;
  }
  vector<EdgeId> const& getVertexIdByDepth(int depth) const {
    return vertexIdByDepth_[depth];
  }
  int getCurrentDepth() const { return vertexIdByDepth_.size(); }
  ColorsDistribution const& getColorDistribution() const {
    return colorsDistribution;
  }
  vector<Vertex> const& getVertexes() const { return vertexes_; }
  vector<Edge> const& getEdges() const { return edges_; }
  Graph::Edge const& getEdge(EdgeId id) const { return edges_[id]; }
  Graph::Vertex const& getVertex(VertexId id) const { return vertexes_[id]; }
  void addEdge(VertexId fromVertexId, VertexId toVertexId);
  Edge::Color calculate_color(VertexId fromVertexId, VertexId toVertexId);
  VertexId addVertex();
  VertexId spawnVertex(VertexId parentId);
  ColorsDistribution colorsDistribution;
  bool isConnected(VertexId firstVertexId, VertexId secondVertexId) const;

 private:
  vector<Vertex> vertexes_;
  vector<Edge> edges_;
  map<VertexId, Depth> vertexDepthMap_;
  map<VertexId, vector<EdgeId>> edgeConectionMap_;
  VertexId vertexIdCounter_ = 0;
  EdgeId edgeIdCounter_ = 0;
  VertexId getNewVertexId() { return vertexIdCounter_++; }
  EdgeId getNewEdgeId() { return edgeIdCounter_++; }
  vector<vector<VertexId>> vertexIdByDepth_;
};
}  // namespace uni_course_cpp
