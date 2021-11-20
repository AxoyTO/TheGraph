#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

using VertexId = int;
using EdgeId = int;

struct Vertex {
  Vertex(const VertexId& _id) : id(_id) {}

  const VertexId id = 0;
};

struct Edge {
  Edge(const EdgeId& _id,
       const VertexId& _from_vertex_id,
       const VertexId& _to_vertex_id)
      : id_(_id),
        from_vertex_id_(_from_vertex_id),
        to_vertex_id_(_to_vertex_id) {}

  const EdgeId id_ = 0;
  const VertexId from_vertex_id_ = 0;
  const VertexId to_vertex_id_ = 0;
};

class Graph {
 public:
  void add_vertex();
  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);

 private:
  std::vector<Vertex> vertexVec_;
  std::vector<Edge> edgeVec_;
};

#endif  // GRAPH_H
