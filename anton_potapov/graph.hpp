#pragma once

#include <algorithm>
#include <cassert>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "graph_traverser.hpp"

enum class EdgeColor { Gray, Green, Blue, Yellow, Red };

std::string get_edge_color_string(const EdgeColor& color) {
  switch (color) {
    case EdgeColor::Gray:
      return "gray";
    case EdgeColor::Green:
      return "green";
    case EdgeColor::Blue:
      return "blue";
    case EdgeColor::Yellow:
      return "yellow";
    case EdgeColor::Red:
      return "red";
  }
}

using VertexId = int;
using EdgeId = int;

constexpr int INIT_DEPTH = 0;

class Vertex {
 public:
  int depth = INIT_DEPTH;
  const VertexId id;

  explicit Vertex(const VertexId& vertex_id) : id(vertex_id) {}

  const std::set<EdgeId>& connected_edges() const { return connected_edges_; }

  bool has_edge_id(const EdgeId& edge_id) const {
    return connected_edges_.find(edge_id) != connected_edges_.end();
  }

  void add_edge(const EdgeId& edge_id) {
    assert(!has_edge_id(edge_id) && "edge that is to be added already exists");
    connected_edges_.insert(edge_id);
  }

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
       const EdgeColor& edge_color)
      : vertex1_id(vertex1),
        vertex2_id(vertex2),
        color(edge_color),
        id(edge_id) {}
};

class Graph {
 public:
  Graph() = default;

  Graph& operator=(const Graph&) = delete;

  Graph& operator=(Graph&& other_graph);

  Graph(const Graph&) = delete;

  Graph(Graph&& other_graph);

  ~Graph() = default;

  Vertex get_vertex(const VertexId& id) const {
    Vertex vertex_copy = vertices_.at(id);
    vertex_copy.depth = GraphTraverser::dynamic_bfs(*this, updated_depth_).at(vertex_copy.id);
    return vertex_copy;
  }

  Vertex& get_vertex(const VertexId& id) {
    update_vertices_depth();
    return vertices_.at(id);
  }

  const Edge& get_edge(const EdgeId& id) const { return edges_.at(id); }

  Edge& get_edge(const EdgeId& id) {
    const auto& const_this = *this;
    return const_cast<Edge&>(const_this.get_edge(id));
  }

  int max_depth() {
    update_vertices_depth();
    return vertices_at_depth_.size() - 1;
  }

  int max_depth() const {
    const auto updated_depths = GraphTraverser::dynamic_bfs(*this, updated_depth_);
    int ans = 0;
    for (const auto& [vertex_id, vertex_depth] : updated_depths) {
      if (vertex_depth > ans) {
        ans = vertex_depth;
      }
    }
    return ans;
  }

  std::map<VertexId, Vertex> vertices() const {
    auto vertices_updated_copy = vertices_;
    const auto updated_depths = GraphTraverser::dynamic_bfs(*this, updated_depth_);
    for (auto& [vertex_id, vertex] : vertices_updated_copy) {
      vertex.depth = updated_depths.at(vertex_id);
    }
    return vertices_updated_copy;
  }
  const std::map<VertexId, Vertex>& vertices() {
    update_vertices_depth();
    return vertices_;
  }
  const std::map<EdgeId, Edge>& edges() const { return edges_; }

  const std::set<VertexId>& get_vertices_at_depth(int depth);
  std::set<VertexId> get_vertices_at_depth(int depth) const;

  bool is_vertex_exists(const VertexId& vertex_id) const;

  bool is_connected(const VertexId& vertex1_id,
                    const VertexId& vertex2_id) const;

  VertexId add_vertex();

  EdgeId add_edge(const VertexId& vertex1_id,
                  const VertexId& vertex2_id,
                  const EdgeColor& edge_color = EdgeColor::Gray);

  const VertexId& get_root_vertex_id() const {
    return vertices_.begin()->first;
  }

 private:
  bool is_depth_dirty_ = true;
  int updated_depth_ = INIT_DEPTH;
  VertexId next_vertex_id_{};
  EdgeId next_edge_id_{};
  std::map<VertexId, Vertex> vertices_;
  std::map<EdgeId, Edge> edges_;
  std::map<int, std::set<VertexId>> vertices_at_depth_;

  VertexId get_next_vertex_id() { return next_vertex_id_++; }
  EdgeId get_next_edge_id() { return next_edge_id_++; }

  int get_new_depth(const VertexId& vertex1_id,
                    const VertexId& vertex2_id) const;

  bool new_edge_color_is_correct(const VertexId& vertex1_id,
                                 const VertexId& vertex2_id,
                                 const EdgeColor& color);

  void update_vertices_depth();
  void update_vertices_depth() const;

  void update_vertices_at_depth_map(const std::map<VertexId, int>& depths);
};

VertexId get_random_vertex_id(const std::set<VertexId>& vertex_id_set);
