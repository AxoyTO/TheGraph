#pragma once
#ifndef _GRAPH_
#define _GRAPH_

#include <cassert>
#include <string>
#include <vector>

namespace graph_structures {
using VertexId = int;
using EdgeId = int;
using VertexDepth = int;

struct Vertex {
 public:
  const VertexId id{};
  VertexDepth depth = 0;

  explicit Vertex(const VertexId& _id) : id(_id) {}

  void add_edge_id(const EdgeId& _id);
  const std::vector<EdgeId>& get_edge_ids() const;

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
}  // namespace graph_structures

class Graph {
 public:
  graph_structures::VertexId insert_vertex();
  void insert_edge(const graph_structures::VertexId& source,
                   const graph_structures::VertexId& destination,
                   const graph_structures::Edge::Color& color =
                       graph_structures::Edge::Color::Gray);

  bool does_color_match_vertices(
      const graph_structures::Vertex& source,
      const graph_structures::Vertex& destination,
      const graph_structures::Edge::Color& color) const;

  bool are_vertices_connected(
      const graph_structures::VertexId& source,
      const graph_structures::VertexId& destination) const;

  int total_edges_of_color(const graph_structures::Edge::Color color) const;
  int depth() const;
  const std::vector<graph_structures::Vertex>& get_vertices() const;
  const std::vector<graph_structures::Edge>& get_edges() const;
  const std::vector<graph_structures::VertexId>& get_vertices_in_depth(
      const graph_structures::VertexDepth& depth) const;
  std::string to_JSON() const;

 private:
  std::vector<graph_structures::Edge> edges_;
  std::vector<graph_structures::Vertex> vertices_;
  std::vector<std::vector<graph_structures::VertexId>> depth_map_;
  graph_structures::VertexId vertex_id_counter_ = 0;
  graph_structures::EdgeId edge_id_counter_ = 0;

  graph_structures::VertexId get_new_vertex_id_() {
    return vertex_id_counter_++;
  }
  graph_structures::EdgeId get_new_edge_id_() { return edge_id_counter_++; }
};

#endif
