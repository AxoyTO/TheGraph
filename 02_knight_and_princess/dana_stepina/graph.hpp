#pragma once

#include <algorithm>
#include <vector>

using Depth = int;
using VertexId = int;
using EdgeId = int;

struct Vertex {
  enum class Color { Grey, Green, Blue, Yellow, Red };

  const VertexId id = 0;
  Depth depth = 0;

  explicit Vertex(const VertexId& vertex_id);

  void add_edge_id(const EdgeId& edge_id);
  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }
  bool has_edge_id(const EdgeId& edge_id) const;

 private:
  std::vector<EdgeId> edge_ids_;
};

struct Edge {
  const EdgeId id = 0;
  const VertexId vertex_start = 0;
  const VertexId vertex_end = 0;

  Edge(const EdgeId& edge_id,
       const VertexId& vertex_connection_start,
       const VertexId& vertex_connection_end);
};

class Graph {
 public:
  Vertex& add_vertex();
  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);

  const std::vector<Edge>& get_edges() const { return edges_; }
  const std::vector<Vertex>& get_vertices() const { return vertices_; }
  const VertexId& get_vertex_id_counter() const { return vertex_id_counter_; }
  std::vector<VertexId> get_vertex_ids_at(const Depth& depth) const;

  bool has_vertex_id(const VertexId& vertex_id) const;
  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const;

 private:
  std::vector<std::vector<VertexId>> depth_map_;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;

  EdgeId edge_id_counter_ = 0;
  VertexId vertex_id_counter_ = 0;

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }
  EdgeId get_new_edge_id() { return edge_id_counter_++; }

  void add_vertex_id_to_depth_map() {
    depth_map_[depth_map_.size() - 1].push_back(vertex_id_counter_);
  }
  void change_size_depth_map() { depth_map_.resize(depth_map_.size() + 1); }
};
