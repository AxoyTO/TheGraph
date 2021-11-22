#pragma once

#include <string>
#include <unordered_map>
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

  bool has_edge_id(const EdgeId& edge_id, const std::vector<EdgeId>& edge_ids) {
    for (const auto& edge : edge_ids) {
      if (edge_id == edge) {
        return true;
      }
    }
    return false;
  }

 private:
  std::vector<EdgeId> edge_ids_;
};

struct Edge {
 public:
  // unknown нужно для return val of calculate_edge_color
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
};

std::string color_to_string(const Edge::Color& color);

class Graph {
 public:
  VertexId insert_vertex();
  void insert_edge(const VertexId& source_id, const VertexId& destination_id);

  bool does_vertex_exist(const VertexId& id) const;

  bool are_vertices_connected(const VertexId& source,
                              const VertexId& destination) const;
  const std::vector<EdgeId>& get_colored_edges(const Edge::Color& color) const;
  int depth() const;
  const std::vector<Vertex>& get_vertices() const;
  const std::vector<Edge>& get_edges() const;
  const std::vector<VertexId>& get_vertices_in_depth(
      const VertexDepth& depth) const;
  Vertex& get_vertex(const VertexId& id);

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;
  std::vector<std::vector<VertexId>> depth_map_;
  std::unordered_map<Edge::Color, std::vector<EdgeId>> colored_edges_map_;
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;

  Edge::Color calculate_color_for_edge(const Vertex& source,
                                       const Vertex& destination) const;
  VertexId get_new_vertex_id() { return vertex_id_counter_++; }
  EdgeId get_new_edge_id() { return edge_id_counter_++; }
};
}  // namespace uni_cpp_practice
