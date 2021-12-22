#pragma once
#include <stdexcept>
#include <vector>

namespace uni_course_cpp {
using EdgeId = int;
using VertexId = int;
using Depth = int;

constexpr int INVALID_ID = -1;

class Edge {
 public:
  enum class Color { Gray, Green, Red, Blue, Yellow };
  Edge(const std::pair<VertexId, VertexId>& new_vertex_ids,
       const EdgeId& edge_id,
       const Color& new_color = Color::Gray)
      : vertex_ids(new_vertex_ids), id(edge_id), color(new_color) {}

  const EdgeId id = INVALID_ID;
  const std::pair<VertexId, VertexId> vertex_ids = {INVALID_ID, INVALID_ID};
  const Color color = Color::Gray;
};

class Vertex {
 public:
  explicit Vertex(const VertexId& new_id) : id(new_id) {}

  void add_edge_id(const EdgeId& edge_id);

  bool has_edge_id(const EdgeId& new_edge_id) const;
  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }
  const VertexId id = INVALID_ID;
  Depth depth = 0;

 private:
  std::vector<EdgeId> edge_ids_ = {};
};

class Graph {
 public:
  bool vertex_exist(const VertexId& id) const;

  bool vertex_has_depth(const VertexId& id) const;

  bool edge_exist(const VertexId& first, const VertexId& second) const;

  VertexId add_vertex();

  void add_edge(const VertexId& first, const VertexId& second);

  const std::vector<Edge>& get_edges() const { return edges_; }
  const std::vector<Vertex>& get_vertices() const { return vertices_; }
  Depth get_depth() const {
    return levels_.size();
  }  // without copy gives segfault
  const std::vector<VertexId>& get_vertex_ids_at_depth(
      const Depth& depth) const;
  int get_count_of_colored_edges(const Edge::Color& color) const;

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;
  EdgeId get_new_edge_id() { return edge_id_counter_++; }
  std::vector<std::vector<VertexId> > levels_;
  VertexId get_new_vertex_id() { return vertex_id_counter_++; }
  const Vertex& get_vertex(const VertexId& vertex_id) const;
  const Edge& get_edge(const EdgeId& edge_id) const;
  Vertex& get_vertex(const VertexId& vertex_id);
  Edge& get_edge(const EdgeId& edge_id);
  void update_vertex_depth(const VertexId& from_vertex_id,
                           const VertexId& to_vertex_id);
  Edge::Color calculate_color(const VertexId& first_id,
                              const VertexId& second_id);
};
}  // namespace uni_course_cpp
