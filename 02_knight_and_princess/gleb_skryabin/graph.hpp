#include <array>
#include <unordered_map>
#include <unordered_set>

using EdgeId = int;
using VertexId = int;
constexpr int INVALID_ID = -1;

class Graph {
 public:
  struct Edge {
    EdgeId id = INVALID_ID;
    std::pair<VertexId, VertexId> verticeIds;
  };

  struct Vertex {
    VertexId id = INVALID_ID;
    std::unordered_set<EdgeId> edgeIds;
  };

  template <std::size_t SIZE>
  explicit Graph(const std::array<Edge, SIZE>& inpEdges) {
    for (auto newEdge : inpEdges) {
      // изначально edges_ пустой, так что надо добавлять каждый элемент
      edges_.emplace(newEdge.id, newEdge);
    }
    for (auto edge : edges_) {
      auto vs = edge.second.verticeIds;
      for (VertexId vsid : std::array<VertexId, 2>{vs.first, vs.second}) {
        auto v = vertices_.find(vsid);
        if (v == vertices_.end()) {
          // add vertex
          Vertex newVertex = {.id = vsid, .edgeIds = {edge.first}};
          vertices_.emplace(vsid, newVertex);
        } else {
          // edit vertex
          v->second.edgeIds.insert(edge.first);
        }
      }
    }
  }

  std::string toJSON() {
    std::string json;
    json = "{\n  \"vertices\": [\n    ";
    // я не использую все возможности for, так как мне
    // все равно придется сравнивать итератор с vertices_.end()
    for (auto pVertexPair = vertices_.begin();;) {
      auto v = pVertexPair->second;
      json += "{\n      \"id\": " + std::to_string(v.id);
      json += ",\n      \"edge_ids\": [";

      for (auto pEdgeId = v.edgeIds.begin();;) {
        json += std::to_string(*pEdgeId);
        if (++pEdgeId != v.edgeIds.end()) {
          json += ", ";
        } else {
          break;
        }
      }
      json += "]\n    }";
      if (++pVertexPair != vertices_.end()) {
        json += ", ";
      } else {
        break;
      }
    }

    json += "\n  ],\n  \"edges\": [\n    ";
    for (auto pEdgePair = edges_.begin();;) {
      EdgeId edgeId = pEdgePair->first;
      auto vs = pEdgePair->second.verticeIds;
      json += "{\n      \"id\": " + std::to_string(edgeId);
      json += ",\n      \"vertex_ids\": [";
      json += std::to_string(vs.first) + ", ";
      json += std::to_string(vs.second) + "]\n    }";

      if (++pEdgePair != edges_.end()) {
        json += ", ";
      } else {
        break;
      }
    }

    json += "\n  ]\n}\n";
    return json;
  }

 private:
  std::unordered_map<EdgeId, Edge> edges_;
  std::unordered_map<VertexId, Vertex> vertices_;
};
