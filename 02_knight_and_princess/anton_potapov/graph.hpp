#pragma once

#include <algorithm>
#include <cassert>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>

enum class EdgeColor { Gray, Green, Blue, Yellow, Red };

std::string get_edge_color_string(const EdgeColor& color) {
  switch(color) {
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
    default:
      return "unknown_color";
  }
}

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  size_t depth;

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
  size_t max_depth() const { return max_depth_; }

  std::set<VertexId> vertex_ids() const {
    std::set<VertexId> vertex_ids;
    std::transform(vertices_.begin(), vertices_.end(),
                   std::inserter(vertex_ids, vertex_ids.end()),
                   [](auto pair) { return pair.first; });
    return vertex_ids;
  }

  const std::set<VertexId>& get_vertices_at_depth(size_t depth) {
    update_vertices_depth();
    return vertices_at_depth_[depth];
  }

  const std::map<size_t, std::set<VertexId>>& depth_distribution() {
    update_vertices_depth();
    return vertices_at_depth_;
  }

  bool is_vertex_exists(const VertexId& vertex_id) const {
    return vertices_.find(vertex_id) != vertices_.end();
  }

  bool is_id_neighbor(const VertexId& vertex1_id,
                      const VertexId& vertex2_id) const {
    return vertex1_id + 1 == vertex2_id || vertex2_id + 1 == vertex1_id;
  }

  bool is_connected(const VertexId& vertex1_id,
                    const VertexId& vertex2_id) const {
    assert(is_vertex_exists(vertex1_id) && "Vertex 1 doesn't exist");
    assert(is_vertex_exists(vertex2_id) && "Vertex 2 doesn't exist");
    if (vertex1_id == vertex2_id) {
      auto vertex_pair_iterator = vertices_.find(vertex1_id);
      for (const auto& vertex_edge_id :
           vertex_pair_iterator->second.connected_edges()) {
        const auto& vertex_edge = edges_.find(vertex_edge_id)->second;
        if (vertex_edge.vertex1_id == vertex_edge.vertex2_id) {
          return true;
        }
      }
      return false;
    } else {
      for (const auto& vertex1_edge_id :
           vertices_.find(vertex1_id)->second.connected_edges()) {
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

  EdgeId add_edge(const VertexId& vertex1,
                  const VertexId& vertex2,
                  EdgeColor edge_color = EdgeColor::Gray) {
    assert(is_vertex_exists(vertex1) && "Vertex 1 doesn't exist");
    assert(is_vertex_exists(vertex2) && "Vertex 2 doesn't exist");
    assert(!is_connected(vertex1, vertex2) && "Vertices already connected");

    const EdgeId new_edge_id = get_next_edge_id();

    auto it_vertex1 = vertices_.find(vertex1);
    it_vertex1->second.add_edge(new_edge_id);
    auto it_vertex2 = vertices_.find(vertex2);
    if (it_vertex1 != it_vertex2) {
      it_vertex2->second.add_edge(new_edge_id);
    }

    edges_.emplace(new_edge_id,
                   Edge(new_edge_id, vertex1, vertex2, edge_color));
    return new_edge_id;
  }

  std::string get_json_string() {
    update_vertices_depth();
    std::stringstream json_stringstream;
    json_stringstream << "{\"depth\":" << max_depth_ << ",";
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

 private:
  VertexId next_vertex_id_{};
  EdgeId next_edge_id_{};
  std::map<VertexId, Vertex> vertices_;
  std::map<EdgeId, Edge> edges_;
  std::map<size_t, std::set<VertexId>> vertices_at_depth_;
  size_t max_depth_;

  VertexId get_next_vertex_id() {
    const VertexId new_vertex_id = next_vertex_id_;
    ++next_vertex_id_;
    return new_vertex_id;
  }

  EdgeId get_next_edge_id() {
    const EdgeId new_edge_id = next_edge_id_;
    ++next_edge_id_;
    return new_edge_id;
  }

  void update_vertices_depth() {
    const VertexId first_vertex_id = vertices_.begin()->first;

    std::map<VertexId, size_t> depths;
    depths.emplace(first_vertex_id, 0);

    std::queue<VertexId> bfs_queue;
    bfs_queue.push(first_vertex_id);

    std::set<VertexId> used;
    used.insert(first_vertex_id);

    while (!bfs_queue.empty()) {
      const VertexId current_vertex_id = bfs_queue.front();
      bfs_queue.pop();
      for (const auto& connected_edge_id :
           vertices_.find(current_vertex_id)->second.connected_edges()) {
        const Edge connected_edge = edges_.find(connected_edge_id)->second;
        const VertexId vertex1_id = connected_edge.vertex1_id;
        const VertexId vertex2_id = connected_edge.vertex2_id;
        const VertexId connected_vertex_id =
            (current_vertex_id == vertex1_id ? vertex2_id : vertex2_id);
        if (used.find(connected_vertex_id) == used.end()) {
          used.insert(connected_vertex_id);
          depths[connected_vertex_id] = depths[current_vertex_id] + 1;
          bfs_queue.push(connected_vertex_id);
        }
      }
    }
    size_t new_max_depth = 0;
    for (const auto& [vertex_id, depth] : depths) {
      vertices_.find(vertex_id)->second.depth = depth;
      if (depth > new_max_depth) {
        new_max_depth = depth;
      }
    }
    max_depth_ = new_max_depth;
    for (const auto& [vertex_id, depth] : depths) {
      vertices_at_depth_[depth].insert(vertex_id);
    }
  }
};
