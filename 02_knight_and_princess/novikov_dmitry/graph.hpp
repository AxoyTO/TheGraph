#pragma once

#include <assert.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace uni_cpp_practice {
constexpr int DEFAULT_DEPTH = 0;

using VertexId = int;
using EdgeId = int;
using Depth = int;

class Vertex {
 public:
  Depth depth = 0;
  const VertexId id = 0;

  explicit Vertex(const VertexId& new_vertex_id) : id(new_vertex_id) {}

  void add_edge_id(const EdgeId& new_edge_id) {
    assert(!has_edge_id(new_edge_id) && "Edge id already exists");
    edge_ids_.push_back(new_edge_id);
  }

  bool has_edge_id(const EdgeId& new_edge_id) const;

  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

  std::string to_string() const;

 private:
  std::vector<EdgeId> edge_ids_;
};

class Edge {
 public:
  enum class Color { Gray, Green, Blue, Yellow, Red };

  const Color color;

  Edge(const VertexId& from_vertex_id,
       const VertexId& to_vertex_id,
       const EdgeId& new_edge_id,
       const Color& new_edge_color)
      : color(new_edge_color),
        ver_id1_(from_vertex_id),
        ver_id2_(to_vertex_id),
        id_(new_edge_id) {}

  std::pair<VertexId, VertexId> get_binded_vertices() const {
    return {ver_id1_, ver_id2_};
  }

  std::string to_string() const;

 private:
  const VertexId ver_id1_ = 0;
  const VertexId ver_id2_ = 0;
  const EdgeId id_ = 0;
};

std::string color_to_string(const Edge::Color& color);

bool check_color_valid(const Vertex& first_vertex,
                       const Vertex& second_vertex,
                       const Edge::Color& color);

class Graph {
 public:
  VertexId add_vertex();

  void add_edge(const VertexId& from_vertex_id,
                const VertexId& to_vertex_id,
                const Edge::Color& new_edge_color = Edge::Color::Gray);

  bool check_binding(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) const;

  std::string to_string() const;

  bool has_vertex(const VertexId& id) const {
    return vertex_map_.find(id) != vertex_map_.end();
  }

  bool has_edge(const EdgeId& id) const {
    return edge_map_.find(id) != edge_map_.end();
  }

  const std::unordered_map<VertexId, Vertex>& get_vertex_map() const {
    return vertex_map_;
  }
  const std::unordered_map<EdgeId, Edge>& get_edge_map() const {
    return edge_map_;
  }

  int count_edges_of_color(const Edge::Color& color) const;

  Depth get_depth() const {
    return (depth_map_.size() > DEFAULT_DEPTH) ? (depth_map_.size() - 1)
                                               : DEFAULT_DEPTH;
  }

  const std::vector<VertexId>& get_vertices_at_depth(const Depth& depth) const {
    assert(depth <= get_depth() && "Depth level doesn't exist");
    return depth_map_.at(depth);
  }

  const Vertex& get_vertex(const VertexId& id) const {
    assert(has_vertex(id) && "Vertex doesn't exist");
    return vertex_map_.at(id);
  }

  const Edge& get_edge(const EdgeId& id) const {
    assert(has_edge(id) && "Edge doesn't exist");
    return edge_map_.at(id);
  }

 private:
  VertexId default_vertex_id_ = 0;
  EdgeId default_edge_id_ = 0;
  std::unordered_map<VertexId, Vertex> vertex_map_;
  std::unordered_map<EdgeId, Edge> edge_map_;
  std::vector<std::vector<VertexId>> depth_map_ = {{}};

  VertexId get_default_vertex_id() { return default_vertex_id_++; }

  EdgeId get_default_edge_id() { return default_edge_id_++; }

  Vertex& get_mutable_vertex(const VertexId& id) {
    return const_cast<Vertex&>(get_vertex(id));
  }

  Edge& get_mutable_edge(const EdgeId& id) {
    return const_cast<Edge&>(get_edge(id));
  }

  std::vector<VertexId>& get_mutable_vertices_at_depth(const Depth& depth) {
    return const_cast<std::vector<VertexId>&>(get_vertices_at_depth(depth));
  }

  void set_vertex_depth(const VertexId& from_vertex_id,
                        const VertexId& to_vertex_id);
};
}  // namespace uni_cpp_practice
