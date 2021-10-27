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
  /*template <std::size_t SIZE>
  explicit Graph(const std::array<GraphInputEdge, SIZE>& inpEdges) {
    for (GraphInputEdge inpEdge : inpEdges) {
      addEdge(inpEdge);
    }
    for (auto [edgeId, edge] : edges_) {
      auto vs = edge.getVertexIds();
      for (VertexId vsid : std::array<VertexId, 2>{vs.first, vs.second}) {
        compliteVertex(vsid, edgeId);
      }
    }
  }

  void addEdge(GraphInputEdge newEdge) {
    if (newEdge.id != INVALID_ID) {
      auto edge = edges_.find(newEdge.id);
      if (edge == edges_.end()) {
        // add new edge
        edges_.emplace(newEdge.id, Edge(newEdge.id, newEdge.vertexIds));
      } else {
        // edit edge
        // edge->second.vertexIds = newEdge.vertexIds;
        // редактирование ребра пока что не предусматривается
        std::cout << "warning: edge with id " << newEdge.id
                  << " is already in the graph.\n";
      }
    }
  }

  void compliteVertex(VertexId vertexId, EdgeId edgeId) {
    auto v = vertices_.find(vertexId);
    if (v == vertices_.end()) {
      // add new vertex
      Vertex newVertex = Vertex(vertexId);
      newVertex.addEdgeId(edgeId);
      vertices_.emplace(vertexId, newVertex);
    } else {
      // edit vertex
      v->second.addEdgeId(edgeId);
    }
  }
  */

  template <std::size_t SIZE>
  explicit Graph(const std::array<GraphInputEdge, SIZE>& inpEdgesVertices) {
    /*for (GraphInputEdge newEdgeVertices : inpEdgesVertices) {
      addEdge(newEdgeVertices);
    }
    for (auto [edgeId, edge] : edges_) {
      auto vs = edge.getVertexIds();
      compliteVertex(vs.first, edgeId);
      compliteVertex(vs.second, edgeId);
    }*/

    for (auto [vertexSrcId, vertexTrgId] : inpEdgesVertices) {
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
    auto v = vertices_.find(vertexId);
    // assert(v == vertices_.end() && "Unexpected behavior: vertex already
    // exist");
    if (v == vertices_.end()) {
      vertices_.emplace(vertexId, Vertex(vertexId));
    }
  }

  void compliteVertex(VertexId vertexId, EdgeId edgeId) {
    auto v = vertices_.find(vertexId);
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
