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

  Graph& operator=(Graph&& other_graph) {
    other_graph.update_vertices_depth();
    vertices_ = std::move(other_graph.vertices_);
    edges_ = std::move(other_graph.edges_);
    next_vertex_id_ = std::move(other_graph.next_vertex_id_);
    next_edge_id_ = std::move(other_graph.next_edge_id_);
    vertices_at_depth_ = std::move(other_graph.vertices_at_depth_);
    return *this;
  }

  Graph(const Graph&) = delete;

  Graph(Graph&& other_graph) {
    other_graph.update_vertices_depth();
    vertices_ = std::move(other_graph.vertices_);
    edges_ = std::move(other_graph.edges_);
    next_vertex_id_ = std::move(other_graph.next_vertex_id_);
    next_edge_id_ = std::move(other_graph.next_edge_id_);
    vertices_at_depth_ = std::move(other_graph.vertices_at_depth_);
  }

  ~Graph() = default;

  int max_depth() {
    update_vertices_depth();
    const auto& const_this = *this;
    return const_this.max_depth();
  }

  int max_depth() const {
    return std::max((int)vertices_at_depth_.size() - 1, 0);
  }

  const std::map<VertexId, Vertex>& vertices() const { return vertices_; }

  const std::set<VertexId>& get_vertices_at_depth(int depth) {
    update_vertices_depth();
    const auto& const_this = *this;
    return const_this.get_vertices_at_depth(depth);
  }

  const std::set<VertexId>& get_vertices_at_depth(int depth) const {
    return vertices_at_depth_.at(depth);
  }

  bool is_vertex_exists(const VertexId& vertex_id) const {
    return vertices_.find(vertex_id) != vertices_.end();
  }

  bool is_connected(const VertexId& vertex1_id,
                    const VertexId& vertex2_id) const {
    assert(is_vertex_exists(vertex1_id) && "Vertex 1 doesn't exist");
    assert(is_vertex_exists(vertex2_id) && "Vertex 2 doesn't exist");
    if (vertex1_id == vertex2_id) {
      for (const auto& vertex_edge_id :
           get_vertex(vertex1_id).connected_edges()) {
        const auto& vertex_edge = get_edge(vertex_edge_id);
        if (vertex_edge.vertex1_id == vertex_edge.vertex2_id) {
          return true;
        }
      }
      return false;
    } else {
      for (const auto& vertex1_edge_id :
           get_vertex(vertex1_id).connected_edges()) {
        if (vertices_.find(vertex2_id)->second.has_edge_id(vertex1_edge_id)) {
          return true;
        }
      }
      return false;
    }
  }

  VertexId add_vertex() {
    const VertexId new_vertex_id = get_next_vertex_id();
    vertices_.emplace(new_vertex_id, new_vertex_id);
    return new_vertex_id;
  }

  EdgeId add_edge(const VertexId& vertex1_id,
                  const VertexId& vertex2_id,
                  const EdgeColor& edge_color = EdgeColor::Gray) {
    assert(is_vertex_exists(vertex1_id) && "Vertex 1 doesn't exist");
    assert(is_vertex_exists(vertex2_id) && "Vertex 2 doesn't exist");
    assert(!is_connected(vertex1_id, vertex2_id) &&
           "Vertices already connected");
    assert(new_edge_color_is_correct(vertex1_id, vertex2_id, edge_color) &&
           "the new edge's color is incorrect");

    const EdgeId new_edge_id = get_next_edge_id();
    edges_.emplace(new_edge_id,
                   Edge(new_edge_id, vertex1_id, vertex2_id, edge_color));

    get_vertex(vertex1_id).add_edge(new_edge_id);
    if (vertex1_id != vertex2_id) {
      get_vertex(vertex2_id).add_edge(new_edge_id);
    }

    is_depth_dirty_ = true;
    if (updated_depth_ != INIT_DEPTH) {
      updated_depth_ =
          std::min(updated_depth_, get_new_depth(vertex1_id, vertex2_id));
    }

    return new_edge_id;
  }

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

  void update_vertices_depth() {
    if (!is_depth_dirty_) {
      return;
    }
    std::map<VertexId, int> depths;
    std::queue<VertexId> bfs_queue;
    std::set<VertexId> used;
    if (updated_depth_ == INIT_DEPTH) {
      const VertexId& first_vertex_id = vertices_.begin()->first;
      depths.emplace(first_vertex_id, 0);
      bfs_queue.push(first_vertex_id);
      used.insert(first_vertex_id);
    } else {
      int max_correct_depth = updated_depth_ - 1;
      for (const auto& vertex_id : vertices_at_depth_.at(max_correct_depth)) {
        depths.emplace(vertex_id, max_correct_depth);
        bfs_queue.push(vertex_id);
        used.insert(vertex_id);
      }
      if (max_correct_depth > 0) {
        for (const auto& vertex_id :
             vertices_at_depth_.at(max_correct_depth - 1)) {
          used.insert(vertex_id);
        }
      }
    }

    while (!bfs_queue.empty()) {
      const VertexId& current_vertex_id = bfs_queue.front();
      bfs_queue.pop();
      for (const auto& connected_edge_id :
           get_vertex(current_vertex_id).connected_edges()) {
        const Edge& connected_edge = get_edge(connected_edge_id);
        const VertexId& vertex1_id = connected_edge.vertex1_id;
        const VertexId& vertex2_id = connected_edge.vertex2_id;
        const VertexId& connected_vertex_id =
            (current_vertex_id == vertex1_id ? vertex2_id : vertex2_id);
        if (used.find(connected_vertex_id) == used.end()) {
          used.insert(connected_vertex_id);
          depths[connected_vertex_id] = depths[current_vertex_id] + 1;
          bfs_queue.push(connected_vertex_id);
        }
      }
    }

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

VertexId get_random_vertex_id(const std::set<VertexId>& vertex_id_set) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, (int)vertex_id_set.size() - 1);
  auto vertex_id_set_it = vertex_id_set.begin();
  std::advance(vertex_id_set_it, dist(gen));
  return *vertex_id_set_it;
}
