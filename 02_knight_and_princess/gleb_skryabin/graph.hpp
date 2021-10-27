#include <assert.h>
#include <array>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using EdgeId = int;
using VertexId = int;
using GraphInputEdge = std::pair<int, int>;
constexpr int INVALID_ID = -1;

class Edge {
 public:
  Edge(EdgeId inpId, VertexId vertexSrcId, VertexId vertexTrgId)
      : id_(inpId), vertexIds_(vertexSrcId, vertexTrgId) {}

  std::pair<VertexId, VertexId> getVertexIds() const { return vertexIds_; }

  std::string toJSON() const {
    std::string json;
    json += "{\n      \"id\": " + std::to_string(id_);
    json += ",\n      \"vertex_ids\": [";
    json += std::to_string(vertexIds_.first) + ", ";
    json += std::to_string(vertexIds_.second) + "]\n    }";
    return json;
  }

 private:
  const EdgeId id_ = INVALID_ID;
  const std::pair<VertexId, VertexId> vertexIds_ = {INVALID_ID, INVALID_ID};
};

class Vertex {
 public:
  Vertex(int inpId) : id_(inpId) {}

  void addEdgeId(EdgeId inpEdgeId) { edgeIds_.insert(inpEdgeId); }

  std::string toJSON() const {
    std::string json;
    json += "{\n      \"id\": " + std::to_string(id_);
    json += ",\n      \"edge_ids\": [";

    for (auto pEdgeId = edgeIds_.begin(); pEdgeId != edgeIds_.end();
         pEdgeId++) {
      json += pEdgeId != edgeIds_.begin() ? ", " : "";
      json += std::to_string(*pEdgeId);
    }

    json += "]\n    }";
    return json;
  }

 private:
  const EdgeId id_ = INVALID_ID;
  std::unordered_set<EdgeId> edgeIds_ = {};
};

class Graph {
 public:
  template <std::size_t SIZE>
  explicit Graph(const std::array<GraphInputEdge, SIZE>& inpEdgesVertices) {
    for (const auto& [vertexSrcId, vertexTrgId] : inpEdgesVertices) {
      addVertex(vertexSrcId);
      addVertex(vertexTrgId);
      EdgeId newEdgeId = addEdge(vertexSrcId, vertexTrgId);
      compliteVertex(vertexSrcId, newEdgeId);
      compliteVertex(vertexTrgId, newEdgeId);
    }
  }

  EdgeId addEdge(VertexId vertexSrcId, VertexId vertexTrgId) {
    assert(vertices_.find(vertexSrcId) != vertices_.end() &&
           "Unexpected behavior: vertex doesn't exists");
    assert(vertices_.find(vertexTrgId) != vertices_.end() &&
           "Unexpected behavior: vertex doesn't exists");

    for (const auto& [edgeId, edge] : edges_) {
      auto vs = edge.getVertexIds();
      if (vs.first == vertexSrcId && vs.second == vertexTrgId) {
        assert(false && "Unexpected behavior: edge doesn't exists");
      }
    }

    EdgeId newEdgeId = edges_.size();
    edges_.emplace(newEdgeId, Edge(newEdgeId, vertexSrcId, vertexTrgId));
    return newEdgeId;
  }

  void addVertex(VertexId vertexId) {
    const auto& v = vertices_.find(vertexId);
    if (v == vertices_.end()) {
      vertices_.emplace(vertexId, Vertex(vertexId));
    }
  }

  void compliteVertex(VertexId vertexId, EdgeId edgeId) {
    const auto& v = vertices_.find(vertexId);
    v->second.addEdgeId(edgeId);
  }

  std::string toJSON() const {
    std::string json;
    json = "{\n  \"vertices\": [\n    ";
    for (auto pVertexPair = vertices_.begin(); pVertexPair != vertices_.end();
         pVertexPair++) {
      json += pVertexPair != vertices_.begin() ? ", " : "";
      json += pVertexPair->second.toJSON();
    }

    json += "\n  ],\n  \"edges\": [\n    ";
    for (auto pEdgePair = edges_.begin(); pEdgePair != edges_.end();
         pEdgePair++) {
      json += pEdgePair != edges_.begin() ? ", " : "";
      json += pEdgePair->second.toJSON();
    }

    json += "\n  ]\n}\n";
    return json;
  }

 private:
  std::unordered_map<EdgeId, Edge> edges_;
  std::unordered_map<VertexId, Vertex> vertices_;
};
