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

  void addEdgeId(EdgeId edgeId) {
    assert(!containEdge(edgeId) && "ERROR: vertex already contain edge");
    edgeIds_.insert(edgeId);
  }

  bool containEdge(EdgeId edgeId) {
    return edgeIds_.find(edgeId) != edgeIds_.end();
  }

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
  EdgeId addEdge(VertexId vertexSrcId, VertexId vertexTrgId) {
    assert(containVertex(vertexSrcId) && "ERROR: Vertex doesn't exists");
    assert(containVertex(vertexTrgId) && "ERROR: vertex doesn't exists");
    assert(!checkConnectoin(vertexSrcId, vertexTrgId) &&
           "ERROR: edge doesn't exists");

    EdgeId newEdgeId = generateEdgeId();
    edges_.emplace(newEdgeId, Edge(newEdgeId, vertexSrcId, vertexTrgId));
    return newEdgeId;
  }

  bool checkConnectoin(VertexId vertexSrcId, VertexId vertexTrgId) {
    assert(containVertex(vertexSrcId) && "ERROR: Vertex doesn't exists");
    assert(containVertex(vertexTrgId) && "ERROR: vertex doesn't exists");
    for (const auto& [edgeId, edge] : edges_) {
      const auto& vs = edge.getVertexIds();
      if (vs.first == vertexSrcId && vs.second == vertexTrgId) {
        return true;
      }
    }
    return false;
  }

  VertexId addVertex() {
    VertexId newVertexId = generateVertexId();
    vertices_.emplace(newVertexId, newVertexId);
    return newVertexId;
  }

  void addVertex(VertexId vertexId) {
    assert(!containVertex(vertexId) && "ERROR: vertex already exists");
    vertices_.emplace(vertexId, vertexId);
  }

  void compliteVertex(VertexId vertexId, EdgeId edgeId) {
    vertices_.at(vertexId).addEdgeId(edgeId);
  }

  bool containVertex(EdgeId vertexId) {
    return vertices_.find(vertexId) != vertices_.end();
  }

  bool containEdge(EdgeId edgeId) {
    return edges_.find(edgeId) != edges_.end();
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
  EdgeId nextEdgeId = 0;
  VertexId nextVertexId = 0;
  EdgeId generateEdgeId() { return nextEdgeId++; }
  VertexId generateVertexId() { return nextVertexId++; }

  std::unordered_map<EdgeId, Edge> edges_;
  std::unordered_map<VertexId, Vertex> vertices_;
};
