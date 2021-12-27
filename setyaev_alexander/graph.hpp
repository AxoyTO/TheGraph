#pragma once

#include <unordered_map>

#include "edge.hpp"
#include "vertex.hpp"
namespace uni_course_cpp {

class Graph {
 public:
  using Depth = int;

  VertexId get_new_vertex_id();
  EdgeId get_new_edge_id();

  Vertex add_vertex();
  void add_edge(VertexId first_vertex_id, VertexId second_vertex_id);

  const std::vector<Vertex>& get_vertices() const;
  const std::vector<Edge>& get_edges() const;
  const Edge& get_edge(EdgeId id) const;
  const std::vector<VertexId>& get_vertex_ids_at_depth(Depth depth) const {
    return depth_map_[depth];
  }
  Depth get_depth() const { return depth_map_.size(); }
  Depth get_vertex_depth(VertexId vertex_id) const;
  const std::vector<Edge>& get_edges_ids() const { return edges_; }
  const std::vector<EdgeId>& get_connected_edges_ids(VertexId vertex_id) const {
    return adjacency_list_.at(vertex_id);
  }

  bool is_connected(VertexId from_vertex_id, VertexId to_vertex_id) const;

  const std::vector<EdgeId>& get_colored_edge_ids(
      const Edge::Color& color) const;

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;

  std::unordered_map<VertexId, std::vector<EdgeId>> adjacency_list_;
  std::unordered_map<VertexId, Depth> vertices_depth_;

  Edge::Color get_edge_color(VertexId from_vertex_id, VertexId to_vertex_id);

  Vertex& get_vertex(const VertexId& id);

  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;
  std::vector<std::vector<VertexId>> depth_map_;
  std::unordered_map<Edge::Color, std::vector<EdgeId>> colored_edges_;
};

}  // namespace uni_course_cpp
