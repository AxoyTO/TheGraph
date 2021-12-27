#pragma once

#include <vector>

using VertexId = int;
using EdgeId = int;

struct Vertex {
  explicit Vertex(const VertexId& _id) : id(_id) {}

  const VertexId id = 0;
};

struct Edge {
  explicit Edge(const EdgeId& _id,
                const VertexId& _from_vertex_id,
                const VertexId& _to_vertex_id)
      : id(_id), from_vertex_id(_from_vertex_id), to_vertex_id(_to_vertex_id) {}

  const EdgeId id = 0;
  const VertexId from_vertex_id = 0;
  const VertexId to_vertex_id = 0;
};

class Graph {
 public:
  void add_vertex();
  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);

 private:
  VertexId vertex_id_counter_ = 0;
  VertexId get_new_vertex_id() { return vertex_id_counter_++; }

  EdgeId edge_id_counter_ = 0;
  EdgeId get_new_edge_id() { return edge_id_counter_++; }

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
};
