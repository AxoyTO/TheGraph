#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <unordered_map>
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
  bool HasVertex(const VertexId& vertex_id) const {
    for (const auto& vertex : vertexes_) {
      if (vertex.id == vertex_id) {
        return true;
      }
    }
    return false;
  }

  bool AreConnected(const VertexId& _vertex_id1, const VertexId& _vertex_id2) {
    assert(HasVertex(_vertex_id1) && "Vertex1 index is out of range");
    assert(HasVertex(_vertex_id2) && "Vertex2 index is out of range");
    if (_vertex_id1 == _vertex_id2) {
      for (const auto& edge_id : connection_list_[_vertex_id1]) {
        if (edges_[edge_id].vertex_id1 == _vertex_id1 &&
            edges_[edge_id].vertex_id2 == _vertex_id1) {
          return true;
        }
      }
    } else {
      for (const auto& edge_id : connection_list_[_vertex_id1]) {
        if (edges_[edge_id].vertex_id1 == _vertex_id2 ||
            edges_[edge_id].vertex_id2 == _vertex_id1) {
          return true;
        }
      }
    }
    return false;
  }

  void AddVertex() {
    connection_list_.insert({vertex_new_id_, std::vector<EdgeId>()});
    vertexes_.emplace_back(GetNewVertexId());
  }

  void AddEdge(const VertexId& _vertex_id1, const VertexId& _vertex_id2) {
    assert(HasVertex(_vertex_id1) && "Vertex1 index is out of range");
    assert(HasVertex(_vertex_id2) && "Vertex2 index is out of range");
    assert(!AreConnected(_vertex_id1, _vertex_id2) &&
           "These vertexes are already connected");
    connection_list_[_vertex_id1].push_back(edge_new_id_);
    if (_vertex_id1 != _vertex_id2) {
      connection_list_[_vertex_id2].push_back(edge_new_id_);
    }
    edges_.emplace_back(edge_new_id_++, std::min(_vertex_id1, _vertex_id2),
                        std::max(_vertex_id1, _vertex_id2));
  }

 private:
  std::vector<Vertex> vertexes_;
  std::vector<Edge> edges_;
  int vertex_new_id_ = 0, edge_new_id_;
  std::unordered_map<VertexId, std::vector<EdgeId>> connection_list_;
  VertexId GetNewVertexId() { return vertex_new_id_++; }
};

constexpr int VERTEX_NUMBER = 14;

int main() {
  const std::vector<std::pair<VertexId, VertexId>> connections = {
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};
  Graph graph;
  for (int i = 0; i < VERTEX_NUMBER; i++) {
    graph.AddVertex();
  }
  for (const auto& connection : connections) {
    graph.AddEdge(connection.first, connection.second);
  }
  return 0;
}
