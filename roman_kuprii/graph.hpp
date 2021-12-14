#pragma once

#include <array>
#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>

namespace uni_cpp_practice {

using EdgeId = int;
using VertexId = int;

constexpr int INVALID_ID = -1;

struct Edge {
  enum class Color { Gray, Green, Blue, Yellow, Red };

  const EdgeId id = INVALID_ID;
  const std::array<VertexId, 2> connected_vertices;
  const Color color = Color::Gray;

  Edge(const VertexId& start,
       const VertexId& end,
       const EdgeId& _id,
       const Color& _color)
      : id(_id), connected_vertices({start, end}), color(_color) {}
};

struct Vertex {
 public:
  int depth = 0;

  explicit Vertex(const VertexId& _id) : id_(_id) {}

  void add_edge_id(const EdgeId& _id);

  const std::vector<EdgeId>& get_edges_ids() const { return edges_ids_; }

  const VertexId& get_id() const { return id_; }

 private:
  const VertexId id_ = INVALID_ID;
  std::vector<EdgeId> edges_ids_;
};

class Graph {
 public:
  VertexId add_vertex();

  bool is_vertex_exist(const VertexId& vertex_id) const;

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const;

  void connect_vertices(const VertexId& from_vertex_id,
                        const VertexId& to_vertex_id,
                        bool initialization);

  const std::unordered_map<EdgeId, Edge>& get_edges_map() const {
    return edges_map_;
  }
  const std::unordered_map<VertexId, Vertex>& get_vertices_map() const {
    return vertices_map_;
  }
  const std::vector<VertexId>& get_vertex_ids_at_depth(int depth) const;

  int get_depth() const { return depth_; }
  int get_vertices_num() const { return vertices_map_.size(); }
  int get_edges_num() const { return edges_map_.size(); }

  std::vector<EdgeId> get_edge_ids_with_color(const Edge::Color& color) const;

 private:
  std::unordered_map<VertexId, Vertex> vertices_map_;
  std::unordered_map<EdgeId, Edge> edges_map_;
  std::vector<std::vector<VertexId>> depth_map_ = {{0}};
  int depth_ = 0;
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  VertexId get_next_vertex_id() { return vertex_id_counter_++; }
  VertexId get_next_edge_id() { return edge_id_counter_++; }
};

}  // namespace uni_cpp_practice
