#include <array>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using EdgeId = int;
using VertexId = int;
constexpr int INVALID_ID = -1;

class Edge {
 public:
  Edge(int inpId, std::pair<VertexId, VertexId> inpVertexIds) {
    id = inpId;
    vertexIds.first = inpVertexIds.first;
    vertexIds.second = inpVertexIds.second;
  }

  std::string toJSON() {
    std::string json;
    json += "{\n      \"id\": " + std::to_string(id);
    json += ",\n      \"vertex_ids\": [";
    json += std::to_string(vertexIds.first) + ", ";
    json += std::to_string(vertexIds.second) + "]\n    }";
    return json;
  }

  EdgeId id = INVALID_ID;
  std::pair<VertexId, VertexId> vertexIds = {INVALID_ID, INVALID_ID};
};

class Vertex {
 public:
  Vertex(int inpId, EdgeId inpEdgeId) {
    id = inpId;
    addEdge(inpEdgeId);
  }

  void addEdge(EdgeId inpEdgeId) { edgeIds.insert(inpEdgeId); }

  std::string toJSON() {
    std::string json;
    json += "{\n      \"id\": " + std::to_string(id);
    json += ",\n      \"edge_ids\": [";

    for (auto pEdgeId = edgeIds.begin(); pEdgeId != edgeIds.end(); pEdgeId++) {
      json += pEdgeId != edgeIds.begin() ? ", " : "";
      json += std::to_string(*pEdgeId);
    }

    json += "]\n    }";
    return json;
  }

  EdgeId id = INVALID_ID;
  std::unordered_set<EdgeId> edgeIds = {};
};

class Graph {
 public:
  // структура ребра для входных данных
  struct InputEdge {
    EdgeId id = INVALID_ID;
    std::pair<VertexId, VertexId> vertexIds;
  };

  // структура вершины для входных данных
  // (пока не используется)
  struct InputVertex {
    VertexId id = INVALID_ID;
    std::unordered_set<EdgeId> edgeIds;
  };

  template <std::size_t SIZE>
  explicit Graph(const std::array<InputEdge, SIZE>& inpEdges) {
    for (InputEdge inpEdge : inpEdges) {
      addEdge(inpEdge);
    }
    for (auto [edgeId, edge] : edges_) {
      auto vs = edge.vertexIds;
      for (VertexId vsid : std::array<VertexId, 2>{vs.first, vs.second}) {
        compliteVertex(vsid, edgeId);
      }
    }
  }

  void addEdge(InputEdge newEdge) {
    auto edge = edges_.find(newEdge.id);
    if (edge == edges_.end()) {
      // add new edge
      edges_.emplace(newEdge.id, Edge(newEdge.id, newEdge.vertexIds));
    } else {
      // edit edge
      // edge->second.vertexIds = newEdge.vertexIds;
      std::cout << "warning: edge with id " << newEdge.id
                << " is already in the graph.\n";
    }
  }

  void compliteVertex(VertexId vertexId, EdgeId edgeId) {
    auto v = vertices_.find(vertexId);
    if (v == vertices_.end()) {
      // add new vertex
      vertices_.emplace(vertexId, Vertex(vertexId, edgeId));
    } else {
      // edit vertex
      v->second.addEdge(edgeId);
    }
  }

  std::string toJSON() {
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
