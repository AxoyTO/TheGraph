#pragma once

#include <map>
#include <set>
#include <string>
#include <unordered_map>

namespace uni_cource_cpp {
using VertexId = int;
using EdgeId = int;

enum class EdgeColor { Gray, Green, Blue, Yellow, Red };
constexpr size_t EDGE_COLORS_COUNT = 5;

std::string get_edge_color_string(const EdgeColor& color);

VertexId get_random_vertex_id(const std::set<VertexId>& vertex_id_set);

constexpr int INIT_DEPTH = 0;

class Vertex {
 public:
  int depth = INIT_DEPTH;
  const VertexId id;
  explicit Vertex(const VertexId& vertex_id);
  const std::set<EdgeId>& connected_edges() const;
  bool has_edge_id(const EdgeId& edge_id) const;
  void add_edge(const EdgeId& edge_id);

 private:
  std::set<EdgeId> connected_edges_;
};

class Edge {
 public:
  const VertexId vertex1_id, vertex2_id;
  const EdgeColor color;
  const EdgeId id;
  Edge(const EdgeId& edge_id,
       const VertexId& vertex1,
       const VertexId& vertex2,
       const EdgeColor& edge_color);
};

class Graph {
 public:
  Vertex& get_vertex(const VertexId& id);

  const Edge& get_edge(const EdgeId& id) const;
  Edge& get_edge(const EdgeId& id);

  int max_depth();

  const std::map<VertexId, Vertex>& vertices();

  const std::map<EdgeId, Edge>& edges() const;

  const std::set<EdgeId> connected_edges(const VertexId& vertex_id) const;

  const std::set<VertexId>& get_vertices_at_depth(int depth);

  bool is_vertex_exists(const VertexId& vertex_id) const;

  bool is_connected(const VertexId& vertex1_id,
                    const VertexId& vertex2_id) const;

  VertexId add_vertex();
  EdgeId add_edge(const VertexId& vertex1_id,
                  const VertexId& vertex2_id,
                  const EdgeColor& edge_color = EdgeColor::Gray);

  const std::set<EdgeId>& edge_ids_with_color(
      const EdgeColor& edge_color) const;

  const VertexId& get_root_vertex_id() const;

 private:
  bool is_depth_dirty_ = true;
  int updated_depth_ = INIT_DEPTH;
  VertexId next_vertex_id_{};
  EdgeId next_edge_id_{};
  std::map<VertexId, Vertex> vertices_;
  std::map<EdgeId, Edge> edges_;
  std::unordered_map<EdgeColor, std::set<EdgeId>> edge_color_map_;
  std::map<int, std::set<VertexId>> vertices_at_depth_;

  VertexId get_next_vertex_id();
  EdgeId get_next_edge_id();

  int get_new_depth(const VertexId& vertex1_id,
                    const VertexId& vertex2_id) const;

  bool new_edge_color_is_correct(const VertexId& vertex1_id,
                                 const VertexId& vertex2_id,
                                 const EdgeColor& color);

  void update_vertices_depth();
};
}  // namespace uni_cource_cpp
