#pragma once

#include <array>
#include <cassert>
#include <string>
#include <vector>

namespace uni_cpp_practice {

using EdgeId = int;
using VertexId = int;

namespace {
constexpr int INVALID_ID = -1;
}

enum class Color { Gray, Green, Blue, Yellow, Red };

std::string color_to_string(const Color& color);
bool is_edge_id_included(const EdgeId& id, const std::vector<EdgeId>& edge_ids);

struct Edge {
  const EdgeId id = INVALID_ID;
  const std::array<VertexId, 2> connected_vertices;
  const Color color = Color::Gray;

  Edge(const VertexId& start,
       const VertexId& end,
       const EdgeId& _id,
       const Color& _color)
      : id(_id), connected_vertices({start, end}), color(_color) {}

  std::string to_json() const;
};

struct Vertex {
 public:
  int depth = 0;

  explicit Vertex(const VertexId& _id) : id_(_id) {}

  std::string to_json() const;

  void add_edge_id(const EdgeId& _id) {
    assert(!is_edge_id_included(_id, edges_ids_));
    edges_ids_.push_back(_id);
  }

  const std::vector<EdgeId>& get_edges_ids() const { return edges_ids_; }

  const VertexId& get_id() const { return id_; }

 private:
  const VertexId id_ = INVALID_ID;
  std::vector<EdgeId> edges_ids_;
};

class Graph {
 public:
  std::string to_json() const;

  void add_vertex() { vertices_.emplace_back(get_next_vertex_id()); }

  bool is_vertex_exist(const VertexId& vertex_id) const;

  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const;

  void connect_vertices(const VertexId& from_vertex_id,
                        const VertexId& to_vertex_id,
                        const bool& initialization);

  const std::vector<Edge>& get_edges() const { return edges_; }
  const std::vector<Vertex>& get_vertices() const { return vertices_; }

  int get_depth() const { return depth_; }
  int get_vertices_num() const { return vertices_.size(); }
  int get_edges_num() const { return edges_.size(); }

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  int depth_ = 0;
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  VertexId get_next_vertex_id() { return vertex_id_counter_++; }
  VertexId get_next_edge_id() { return edge_id_counter_++; }
};

}  // namespace uni_cpp_practice
