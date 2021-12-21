#pragma once
#include <array>
#include <string>
#include <unordered_map>
#include <vector>

using VertexId = int;
using EdgeId = int;

namespace uni_cource_cpp {
class Vertex {
 public:
  const VertexId id;
  int depth = 0;

  explicit Vertex(const VertexId& _id) : id(_id) {}

  void add_edge_id(const EdgeId& id);

  bool has_edge_id(const EdgeId& id) const;

  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

 private:
  std::vector<EdgeId> edge_ids_;
};

class Edge {
 public:
  const EdgeId id;
  const VertexId vertex_id1;
  const VertexId vertex_id2;
  enum class Color { Gray, Green, Yellow, Red };
  const Color color;

  Edge(const EdgeId& _id,
       const VertexId& _vertex_id1,
       const VertexId& _vertex_id2,
       const Color& _color)
      : id(_id),
        vertex_id1(_vertex_id1),
        vertex_id2(_vertex_id2),
        color(_color) {}
};

class Graph {
 public:
  VertexId add_vertex();

  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);

  Edge::Color define_edge_color(const VertexId& from_vertex_id,
                                const VertexId& to_vertex_id);

  bool has_vertex(const VertexId& id) const;

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const;

  const Vertex& get_vertex(const VertexId& id) const;

  Vertex& get_vertex(const VertexId& id);

  const Edge& get_edge(const EdgeId& id) const;

  void update_vertex_depth(const VertexId& id, const int depth);

  const std::vector<EdgeId>& get_colored_edges(const Edge::Color& color) const;

  const std::vector<VertexId>& get_vertex_ids_in_depth(int depth) const;

  const std::vector<EdgeId>& get_edge_ids(const VertexId& id) const;

  int get_depth() const { return vertices_ids_in_depth_.size(); }

  const std::vector<Vertex>& get_vertices() const { return vertices_; }

  const std::vector<Edge>& get_edges() const { return edges_; }

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  std::vector<std::vector<VertexId>> vertices_ids_in_depth_;
  std::unordered_map<Edge::Color, std::vector<EdgeId>> edges_color_map_;

  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }

  EdgeId get_new_edge_id() { return edge_id_counter_++; }
};
}  // namespace uni_cource_cpp
