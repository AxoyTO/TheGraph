#pragma once
#include <array>
#include <string>
#include <vector>

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  const VertexId id;

  explicit Vertex(const VertexId& _id) : id(_id) {}

  void add_edge_id(const EdgeId& id);

  bool has_edge_id(const EdgeId& id) const;

  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

  std::string json_string() const;

 private:
  std::vector<EdgeId> edge_ids_;
};

class Edge {
 public:
  const EdgeId id;
  const VertexId vertex_id1;
  const VertexId vertex_id2;

  Edge(const EdgeId& _id,
       const VertexId& _vertex_id1,
       const VertexId& _vertex_id2)
      : id(_id), vertex_id1(_vertex_id1), vertex_id2(_vertex_id2) {}

  std::string json_string() const;
};

class Graph {
 public:
  void add_vertex();

  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);

  bool has_vertex(const VertexId& id) const;

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const;

  const Vertex& get_vertex(const VertexId& id) const;

  Vertex& get_vertex(const VertexId& id);

  std::string json_string() const;

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;

  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }

  EdgeId get_new_edge_id() { return edge_id_counter_++; }
};
