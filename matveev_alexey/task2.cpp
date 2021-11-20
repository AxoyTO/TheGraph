#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

using VertexId = int;
using EdgeId = int;

struct Vertex {
  const VertexId id;

  explicit Vertex(const VertexId& _id) : id(_id) {}
};

struct Edge {
  const EdgeId id;
  const VertexId vertex_id1;
  const VertexId vertex_id2;
  Edge(const EdgeId& _id, VertexId _vertex_id1, VertexId _vertex_id2)
      : id(_id), vertex_id1(_vertex_id1), vertex_id2(_vertex_id2) {}
};

class Graph {
 public:
  bool hasVertex(const VertexId& vertex_id) const {
    for (const auto& vertex : vertexes_) {
      if (vertex.id == vertex_id) {
        return true;
      }
    }
    return false;
  }

  bool hasEdge(const EdgeId& edge_id) const {
    for (const auto& edge : edges_) {
      if (edge.id == edge_id) {
        return true;
      }
    }
    return false;
  }

  bool areConnected(const VertexId& vertex_id1,
                    const VertexId& vertex_id2) const {
    assert(hasVertex(vertex_id1) && "Vertex1 index is out of range");
    assert(hasVertex(vertex_id2) && "Vertex2 index is out of range");
    if (vertex_id1 == vertex_id2) {
      for (const auto& edge_id : connection_list_.at(vertex_id1)) {
        const auto& edge = getEdge(edge_id);
        if (edge.vertex_id1 == vertex_id1 && edge.vertex_id2 == vertex_id1) {
          return true;
        }
      }
    } else {
      for (const auto& edge_id : connection_list_.at(vertex_id1)) {
        const auto& edge = getEdge(edge_id);
        if (edge.vertex_id1 == vertex_id2 || edge.vertex_id2 == vertex_id2) {
          return true;
        }
      }
    }
    return false;
  }

  void addVertex() {
    const auto& new_vertex = vertexes_.emplace_back(getNewVertexId());
    connection_list_.insert({new_vertex.id, std::vector<EdgeId>()});
  }

  void addEdge(const VertexId& vertex_id1, const VertexId& vertex_id2) {
    assert(hasVertex(vertex_id1) && "Vertex1 index is out of range");
    assert(hasVertex(vertex_id2) && "Vertex2 index is out of range");
    assert(!areConnected(vertex_id1, vertex_id2) &&
           "These vertexes are already connected");
    const auto& new_edge =
        edges_.emplace_back(getNewEdgeId(), vertex_id1, vertex_id2);
    connection_list_[vertex_id1].push_back(new_edge.id);
    if (vertex_id1 != vertex_id2) {
      connection_list_[vertex_id2].push_back(new_edge.id);
    }
  }

  const Edge& getEdge(const EdgeId& edge_id) const {
    assert(hasEdge(edge_id) && "Edge id is out of range.");
    for (const auto& edge : edges_) {
      if (edge.id == edge_id) {
        return edge;
      }
    }
    throw std::runtime_error("Cannot be reached.");
  }

 private:
  std::vector<Vertex> vertexes_;
  std::vector<Edge> edges_;
  int vertex_new_id_ = 0, edge_new_id_ = 0;
  std::unordered_map<VertexId, std::vector<EdgeId>> connection_list_;
  VertexId getNewVertexId() { return vertex_new_id_++; }
  EdgeId getNewEdgeId() { return edge_new_id_++; }
};

constexpr int VERTEX_NUMBER = 14, EDGE_NUMBER = 18;

int main() {
  const std::array<std::pair<VertexId, VertexId>, EDGE_NUMBER> connections = {
      {{0, 1},
       {0, 2},
       {0, 3},
       {1, 4},
       {1, 5},
       {1, 6},
       {2, 7},
       {2, 8},
       {3, 9},
       {4, 10},
       {5, 10},
       {6, 10},
       {7, 11},
       {8, 11},
       {9, 12},
       {10, 13},
       {11, 13},
       {12, 13}}};
  Graph graph;
  for (int i = 0; i < VERTEX_NUMBER; i++) {
    graph.addVertex();
  }
  for (const auto& connection : connections) {
    graph.addEdge(connection.first, connection.second);
  }
  return 0;
}
