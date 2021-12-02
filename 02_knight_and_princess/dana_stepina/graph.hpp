#pragma once

#include <vector>

using Depth = int;
using VertexId = int;
using EdgeId = int;

struct Vertex {
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
  enum class Color { Grey, Green, Yellow, Red };

  const EdgeId id = 0;
  const VertexId vertex_start = 0;
  const VertexId vertex_end = 0;
  const Color color = Color::Grey;

  Edge(const EdgeId& edge_id,
       const VertexId& vertex_connection_start,
       const VertexId& vertex_connection_end,
       const Color& edge_color);
};

class Graph {
 public:
  const Vertex& add_vertex();
  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);

  const std::vector<Edge>& get_edges() const { return edges_; }
  const std::vector<Vertex>& get_vertices() const { return vertices_; }
  const std::vector<std::vector<VertexId>>& get_depth_map() const {
    return depth_map_;
  }
  const std::vector<VertexId>& get_vertex_ids_at(const Depth& depth) const {
    return depth_map_[depth];
  }
  const Vertex& get_vertex(const VertexId& id) const { return vertices_[id]; }
  const Edge& get_edge(const EdgeId& id) const { return edges_[id]; }

  bool has_vertex_id(const VertexId& id) const;
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

  void set_vertex_depth(const VertexId& from_vertex_id,
                        const VertexId& to_vertex_id);
  Edge::Color get_edge_color(const Vertex& from_vertex,
                             const Vertex& to_vertex);
};
