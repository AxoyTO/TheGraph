#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

using VertexId = int;
using EdgeId = int;

enum class GraphParameters { VertexNum = 14, EdgeNum = 18 };

struct Vertex {
  const VertexId id;

  Vertex(const VertexId& _id) : id(_id) {}
  void NewEdge(const EdgeId& edge_id) { edge_ids.push_back(edge_id); }
  EdgeId IthEdgeId(int i) { return edge_ids[i]; }
  int Length() { return edge_ids.size(); }

 private:
  std::vector<EdgeId> edge_ids;
};

struct Edge {
  const EdgeId id;
  std::array<VertexId, 2> vertex_ids;
  Edge(EdgeId _id, std::array<VertexId, 2> _vertex_ids)
      : id(_id), vertex_ids(_vertex_ids) {}
};

class Graph {
 public:
  bool InGraph(const VertexId& vertex_id) {
    for (auto it = vertexes.begin(); it < vertexes.end(); it++) {
      if ((*it).id == vertex_id) {
        return true;
      }
    }
    return false;
  }

  bool AreConnected(const VertexId& vertex_id1, const VertexId& vertex_id2) {
    assert(InGraph(vertex_id1) && InGraph(vertex_id2) &&
           "Vertex index is out of range");
    for (int i = 0; i < vertexes[vertex_id1].Length(); i++) {
      int j = (i + 1) * (vertex_id1 == vertex_id2);
      for (; j < vertexes[vertex_id2].Length(); j++) {
        if (vertexes[vertex_id1].IthEdgeId(i) ==
            vertexes[vertex_id2].IthEdgeId(j)) {
          return true;
        }
      }
    }
    return false;
  }

  void AddVert() {
    vertexes.emplace_back(vertex_num);
    vertex_num++;
  }

  void AddEdge(const VertexId& vertex1, const VertexId& vertex2) {
    assert(!AreConnected(vertex1, vertex2) &&
           "These vertexes are already connected");
    edges.emplace_back(edge_num,
                       std::array<int, 2>{std::min(vertex1, vertex2),
                                          std::max(vertex1, vertex2)});
    vertexes[vertex1].NewEdge(edge_num);
    vertexes[vertex2].NewEdge(edge_num);
    edge_num++;
  }

 private:
  std::vector<Vertex> vertexes;
  std::vector<Edge> edges;
  int vertex_num = 0, edge_num = 0;
};

int main() {
  const std::vector<std::array<VertexId, 2>> vertex = {
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};
  Graph graph;
  for (int i = 0; i < static_cast<int>(GraphParameters::VertexNum); i++) {
    graph.AddVert();
  }
  for (int i = 0; i < static_cast<int>(GraphParameters::EdgeNum); i++) {
    graph.AddEdge(vertex[i][0], vertex[i][1]);
  }
  return 0;
}
