#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

using VertexId = int;
using EdgeId = int;

struct Vertex {
  const VertexId id;

  explicit Vertex(const VertexId& _id) : id(_id) {}
  /*void AddEdgeId(const EdgeId& _edge_id) {
    assert(!HasEdge(_edge_id) && "This vertex already has this edge");
    edge_ids_.push_back(_edge_id);
  }
  EdgeId IthEdgeId(int i) { return edge_ids_[i]; }
  int ConnectedEdges() { return edge_ids_.size(); }*/

 private:
  //std::vector<EdgeId> edge_ids_;
  /*bool HasEdge(const EdgeId& _edge_id) {
    for (const auto& edge_id : edge_ids_) {
      if (edge_id == _edge_id) {
        return true;
      }
    }
    return false;
  }*/
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
  bool HasVertex(const VertexId& _vertex_id) {
    for (const auto& vertex : vertexes_) {
      if (vertex.id == _vertex_id) {
        return true;
      }
    }
    return false;
  }

  bool AreConnected(const VertexId& _vertex_id1, const VertexId& _vertex_id2) {
    assert(HasVertex(_vertex_id1) && "Vertex1 index is out of range");
    assert(HasVertex(_vertex_id2) && "Vertex2 index is out of range");
    for (const auto& edge : edges_) {
        if ((edge.vertex_id1 == _vertex_id1 && edge.vertex_id2 == _vertex_id2) ||
            (edge.vertex_id1 == _vertex_id2 && edge.vertex_id2 == _vertex_id1)) {
            return true;
        }
    }
    return false;
    /*if (_vertex_id1 == _vertex_id2) {
      for (int i = 0; i < vertexes_[_vertex_id1].ConnectedEdges(); i++) {
        if (edges_[vertexes_[_vertex_id1].IthEdgeId(i)].vertex_id1 ==
                _vertex_id2 &&
            edges_[vertexes_[_vertex_id1].IthEdgeId(i)].vertex_id2 ==
                _vertex_id2) {
          return true;
        }
      }
      return false;
    }
    for (int i = 0; i < vertexes_[_vertex_id1].ConnectedEdges(); i++) {
      for (int j = 0; j < vertexes_[_vertex_id2].ConnectedEdges(); j++) {
        if (vertexes_[_vertex_id1].IthEdgeId(i) ==
            vertexes_[_vertex_id2].IthEdgeId(j)) {
          return true;
        }
      }
    }
    return false;*/
  }

  void AddVertex() {
    vertexes_.emplace_back(vertex_new_id_++);
  }

  void AddEdge(const VertexId& _vertex_id1, const VertexId& _vertex_id2) {
    assert(HasVertex(_vertex_id1) && "Vertex1 index is out of range");
    assert(HasVertex(_vertex_id2) && "Vertex2 index is out of range");
    assert(!AreConnected(_vertex_id1, _vertex_id2) &&
           "These vertexes are already connected");
    edges_.emplace_back(edge_new_id_++, std::min(_vertex_id1, _vertex_id2),
                        std::max(_vertex_id1, _vertex_id2));
  }

 private:
  std::vector<Vertex> vertexes_;
  std::vector<Edge> edges_;
  int vertex_new_id_ = 0, edge_new_id_;
};

int main() {
  constexpr int VERTEX_NUMBER = 14, EDGE_NUMBER = 18;
  const std::vector<std::array<VertexId, 2>> vertex = {
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};
  Graph graph;
  for (int i = 0; i < VERTEX_NUMBER; i++) {
    graph.AddVertex();
  }
  for (int i = 0; i < EDGE_NUMBER; i++) {
    graph.AddEdge(vertex[i][0], vertex[i][1]);
  }
  return 0;
}
