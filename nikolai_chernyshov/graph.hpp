#pragma once

#include <vector>

namespace uni_course_cpp {
using VertexId = int;
using EdgeId = int;

struct Edge {
  enum class Color { Gray, Green, Yellow, Red };

  const EdgeId id;
  const VertexId vertex1_id, vertex2_id;
  const Color color;

  Edge(const VertexId& _vertex1_id,
       const VertexId& _vertex2_id,
       const EdgeId& _id,
       const Color& _color = Color::Gray)
      : id(_id),
        vertex1_id(_vertex1_id),
        vertex2_id(_vertex2_id),
        color(_color) {}
};

struct Vertex {
 public:
  const VertexId id;
  int depth = 0;

  explicit Vertex(const VertexId& _id) : id(_id) {}

  bool has_edge_id(const EdgeId& id) const;
  void add_edge_id(const EdgeId& id);
  const std::vector<EdgeId>& get_edge_ids() const;

 private:
  std::vector<EdgeId> edge_ids_;
};

class Graph {
 public:
  bool are_connected(const VertexId& vertex1_id,
                     const VertexId& vertex2_id) const;
  bool has_vertex(const VertexId& vertex_id) const;
  const Edge& get_edge(const EdgeId& edge_id) const;
  const Vertex& get_vertex(const VertexId& vertex_id) const;
  Vertex& get_vertex(const VertexId& vertex_id);
  void add_edge(const VertexId& vertex1_id, const VertexId& vertex2_id);
  const VertexId& add_vertex();
  const std::vector<Edge>& get_edges() const;
  const std::vector<Vertex>& get_vertices() const;
  const std::vector<VertexId>& get_vertex_ids_in_depth(int depth) const;
  const std::vector<std::vector<VertexId>>& get_depth_map() const;
  const int get_depth() const;
  const int get_edges_count_by_color(const Edge::Color color) const;
  std::vector<std::pair<Edge::Color, int>> get_color_edge_count_pairs() const;

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;
  std::vector<std::vector<VertexId>> depth_map_;
  VertexId vertex_id_max_ = 0;
  EdgeId edge_id_max_ = 0;
  int gray_edges_count_ = 0;
  int green_edges_count_ = 0;
  int yellow_edges_count_ = 0;
  int red_edges_count_ = 0;

  void set_vertex_depth(const VertexId& vertex_id, int depth);
  const Edge::Color calculate_edge_color(const VertexId& vertex1_id,
                                         const VertexId& vertex2_id) const;
  const EdgeId get_max_edge_id();
  const VertexId get_max_vertex_id();
};
}  // namespace uni_course_cpp
