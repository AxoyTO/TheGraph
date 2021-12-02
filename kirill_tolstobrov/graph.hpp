#pragma once

#include <map>
#include <vector>

namespace uni_cpp_practice {

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  explicit Vertex(const VertexId& init_id = 0) : id(init_id) {}
  const std::vector<EdgeId>& get_edge_ids() const;
  bool check_edge_presence(const EdgeId& edge_id) const;
  void add_edge_id(const EdgeId& edge_id);

  const VertexId id;
  int depth = 0;

 private:
  std::vector<EdgeId> edge_ids_;
};

class Edge {
 public:
  enum class Color { Grey, Green, Blue, Yellow, Red };

  Edge(const EdgeId& init_id,
       const VertexId& v1_init,
       const VertexId& v2_init,
       const Color& color_init)
      : id(init_id),
        vertex1_id(v1_init),
        vertex2_id(v2_init),
        color(color_init){};

  const EdgeId id;
  const VertexId vertex1_id;
  const VertexId vertex2_id;
  const Color color;
};

class Graph {
 public:
  const std::vector<Vertex>& get_vertices() const;
  const std::vector<Edge>& get_edges() const;
  const std::map<Edge::Color, std::vector<EdgeId>>& get_colors_map() const;

  const Vertex& get_vertex(const VertexId& id) const;
  Vertex& get_vertex(const VertexId& id);
  VertexId add_new_vertex();
  bool check_vertex_existence(const VertexId& vertex_id) const;
  bool are_vertices_connected(const VertexId& id1, const VertexId& id2) const;

  Edge::Color get_binding_color(const VertexId& id1, const VertexId& id2) const;

  void bind_vertices(const VertexId& id1, const VertexId& id2);

  std::vector<std::vector<VertexId>> depths_map_;

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;

  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  // connections_map: vertex1 -> edge -> vertex2
  std::map<VertexId, std::vector<EdgeId>> connections_map_;
  std::map<Edge::Color, std::vector<EdgeId>> colors_map_;

  VertexId get_new_vertex_id();
  EdgeId get_new_edge_id();
};
}  // namespace uni_cpp_practice
