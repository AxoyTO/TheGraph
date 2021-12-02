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

  explicit Vertex(const VertexId& vertex_id) : id_(vertex_id) {}

  const std::set<EdgeId>& connected_edges() const { return connected_edges_; }

  std::string get_json_string() const {
    std::stringstream json_stringstream;
    json_stringstream << "{\"id\":" << id_ << ","
                      << "\"edge_ids\":[";
    for (auto it = connected_edges_.begin(); it != connected_edges_.end();
         ++it) {
      json_stringstream << *it;
      if (std::next(it) != connected_edges_.end()) {
        json_stringstream << ",";
      }
    }
    json_stringstream << "],"
                      << "\"depth\":" << depth << "}";
    return json_stringstream.str();
  }

  bool has_edge_id(const EdgeId& edge_id) const {
    return connected_edges_.find(edge_id) != connected_edges_.end();
  }

  void add_edge(const EdgeId& edge_id) {
    assert(!has_edge_id(edge_id) && "edge that is to be added already exists");
    connected_edges_.insert(edge_id);
  }

 private:
  const VertexId id_;
  std::set<EdgeId> connected_edges_;
};

class Edge {
 public:
  const VertexId vertex1_id, vertex2_id;
  Edge(const EdgeId& edge_id,
       const VertexId& vertex1,
       const VertexId& vertex2,
       const EdgeColor& edge_color)
      : vertex1_id(vertex1),
        vertex2_id(vertex2),
        color_(edge_color),
        id_(edge_id) {}

  std::string get_json_string() const {
    std::stringstream json_stringstream;
    std::string color_string = get_edge_color_string(color_);
    json_stringstream << "{\"id\":" << id_ << ","
                      << "\"vertex_ids\":[" << vertex1_id << "," << vertex2_id
                      << "],\"color\":\"" << color_string << "\""
                      << "}";
    return json_stringstream.str();
  }

 private:
  const EdgeColor color_;
  const EdgeId id_;
};

class Graph {
 public:
  Graph() = default;

  Graph& operator=(const Graph&) = delete;

  Graph& operator=(Graph&& other_graph);

  Graph(const Graph&) = delete;

  Graph(Graph&& other_graph);

  ~Graph() = default;

  const Vertex& get_vertex(const VertexId& id) const {
    return vertices_.at(id);
  }

  Vertex& get_vertex(const VertexId& id) {
    const auto& const_this = *this;
    return const_cast<Vertex&>(const_this.get_vertex(id));
  }

  const Edge& get_edge(const EdgeId& id) const { return edges_.at(id); }

  Edge& get_edge(const EdgeId& id) {
    const auto& const_this = *this;
    return const_cast<Edge&>(const_this.get_edge(id));
  }

  int max_depth() {
    update_vertices_depth();
    const auto& const_this = *this;
    return const_this.max_depth();
  }

  int max_depth() const {
    return std::max((int)vertices_at_depth_.size() - 1, 0);
  }

  const std::map<VertexId, Vertex>& vertices() const { return vertices_; }

  const std::set<VertexId>& get_vertices_at_depth(int depth);

  const std::set<VertexId>& get_vertices_at_depth(int depth) const;

  bool is_vertex_exists(const VertexId& vertex_id) const;

  bool is_connected(const VertexId& vertex1_id,
                    const VertexId& vertex2_id) const;

  VertexId add_vertex();

  EdgeId add_edge(const VertexId& vertex1_id,
                  const VertexId& vertex2_id,
                  const EdgeColor& edge_color = EdgeColor::Gray);

  std::string get_json_string() {
    update_vertices_depth();
    const auto& const_this = *this;
    return const_this.get_json_string();
  }

  std::string get_json_string() const {
    std::stringstream json_stringstream;
    json_stringstream << "{\"depth\":" << max_depth() << ",";
    json_stringstream << "\"vertices\":[";
    for (auto it = vertices_.begin(); it != vertices_.end(); ++it) {
      json_stringstream << it->second.get_json_string();
      if (std::next(it) != vertices_.end()) {
        json_stringstream << ",";
      }
    }
    json_stringstream << "],\"edges\":[";
    for (auto it = edges_.begin(); it != edges_.end(); ++it) {
      json_stringstream << it->second.get_json_string();
      if (std::next(it) != edges_.end()) {
        json_stringstream << ",";
      }
    }
    json_stringstream << "]}";
    return json_stringstream.str();
  }

  const VertexId& get_root_vertex_id() const {
    return vertices_.begin()->first;
  }

  void update_vertices_depth() {
    if (!is_depth_dirty_) {
      return;
    }
    const auto& depths = GraphTraverser::dynamic_bfs(*this, updated_depth_);
    update_vertices_at_depth_map(depths);
    is_depth_dirty_ = false;
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
                    const VertexId& vertex2_id) const {
    const auto vertex1_depth = get_vertex(vertex1_id).depth;
    const auto vertex2_depth = get_vertex(vertex2_id).depth;
    if (vertex1_depth == INIT_DEPTH && vertex2_depth == INIT_DEPTH) {
      return INIT_DEPTH;
    }
    if (vertex1_depth == INIT_DEPTH) {
      return vertex2_depth + 1;
    } else if (vertex2_depth == INIT_DEPTH) {
      return vertex1_depth + 1;
    } else {
      return std::min(vertex1_depth, vertex2_depth) + 1;
    }
  }

  bool new_edge_color_is_correct(const VertexId& vertex1_id,
                                 const VertexId& vertex2_id,
                                 const EdgeColor& color) {
    const auto vertices_at_depth_buffer = vertices_at_depth_;
    update_vertices_depth();
    bool is_correct;
    switch (color) {
      case EdgeColor::Gray:
        is_correct = vertices_.at(vertex1_id).connected_edges().empty() ||
                     vertices_.at(vertex2_id).connected_edges().empty();
        break;
      case EdgeColor::Green:
        is_correct = vertex1_id == vertex2_id;
        break;
      case EdgeColor::Blue:
        is_correct =
            get_vertex(vertex1_id).depth == get_vertex(vertex2_id).depth;
        break;
      case EdgeColor::Yellow:
        is_correct = (std::abs(get_vertex(vertex1_id).depth -
                               get_vertex(vertex2_id).depth) == 1) &&
                     !is_connected(vertex1_id, vertex2_id);
        break;
      case EdgeColor::Red:
        is_correct = std::abs(get_vertex(vertex1_id).depth -
                              get_vertex(vertex2_id).depth) == 2;
        break;
    }
    vertices_at_depth_ = vertices_at_depth_buffer;
    return is_correct;
  }

  void update_vertices_at_depth_map(const std::map<VertexId, int>& depths) {
    if (updated_depth_ == INIT_DEPTH) {
      vertices_at_depth_.clear();
    } else {
      vertices_at_depth_.erase(vertices_at_depth_.find(updated_depth_),
                               vertices_at_depth_.end());
    }
    for (const auto& [vertex_id, depth] : depths) {
      get_vertex(vertex_id).depth = depth;
      vertices_at_depth_[depth].insert(vertex_id);
    }
  }
};

VertexId get_random_vertex_id(const std::set<VertexId>& vertex_id_set);
