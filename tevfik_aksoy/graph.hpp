#pragma once

#include <string>
#include <vector>

namespace uni_cpp_practice {
using VertexId = int;
using EdgeId = int;
using VertexDepth = int;

struct Vertex {
 public:
  const VertexId id{};
  VertexDepth depth = 0;

  explicit Vertex(const VertexId& id) : id(id) {}

  void add_edge_id(const EdgeId& id);
  const std::vector<EdgeId>& get_edge_ids() const;

  bool has_edge_id(const uni_cpp_practice::EdgeId& edge_id,
                   const std::vector<uni_cpp_practice::EdgeId>& edge_ids) {
    for (const auto& edge : edge_ids) {
      if (edge_id == edge) {
        return true;
      }
    }
    return false;
  }

  std::string to_JSON() const;

 private:
  std::vector<EdgeId> edge_ids_;
};

struct Edge {
 public:
  enum class Color { Gray, Green, Blue, Yellow, Red };
  const EdgeId id{};
  const Color color{};
  const VertexId source{};
  const VertexId destination{};

  Edge(const VertexId& _source,
       const VertexId& _destination,
       const VertexId& _id,
       const Color& _color)
      : id(_id), color(_color), source(_source), destination(_destination) {}

  std::string to_JSON() const;
};
}  // namespace uni_cpp_practice

std::string color_to_string(const uni_cpp_practice::Edge::Color& color);

class Graph {
 public:
  uni_cpp_practice::VertexId insert_vertex();
  void insert_edge(const uni_cpp_practice::VertexId& source,
                   const uni_cpp_practice::VertexId& destination,
                   const uni_cpp_practice::Edge::Color& color =
                       uni_cpp_practice::Edge::Color::Gray);

  bool does_vertex_exist(
      const uni_cpp_practice::VertexId& id,
      const std::vector<uni_cpp_practice::Vertex>& vertices) const;

  bool does_color_match_vertices(
      const uni_cpp_practice::Vertex& source,
      const uni_cpp_practice::Vertex& destination,
      const uni_cpp_practice::Edge::Color& color) const;

  bool are_vertices_connected(
      const uni_cpp_practice::VertexId& source,
      const uni_cpp_practice::VertexId& destination) const;

  int total_edges_of_color(const uni_cpp_practice::Edge::Color color) const;
  int depth() const;
  const std::vector<uni_cpp_practice::Vertex>& get_vertices() const;
  const std::vector<uni_cpp_practice::Edge>& get_edges() const;
  const std::vector<uni_cpp_practice::VertexId>& get_vertices_in_depth(
      const uni_cpp_practice::VertexDepth& depth) const;

 private:
  std::vector<uni_cpp_practice::Edge> edges_;
  std::vector<uni_cpp_practice::Vertex> vertices_;
  std::vector<std::vector<uni_cpp_practice::VertexId>> depth_map_;
  uni_cpp_practice::VertexId vertex_id_counter_ = 0;
  uni_cpp_practice::EdgeId edge_id_counter_ = 0;

  uni_cpp_practice::VertexId get_new_vertex_id_() {
    return vertex_id_counter_++;
  }
  uni_cpp_practice::EdgeId get_new_edge_id_() { return edge_id_counter_++; }
};
