#pragma once

#include <assert.h>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

constexpr int DEFAULT_DEPTH = 0;

using VertexId = int;
using EdgeId = int;
using Depth = int;

class Vertex {
 public:
  Vertex(const VertexId& new_vertex_id, const Depth& new_vertex_depth)
      : id_(new_vertex_id), depth_(new_vertex_depth) {}

  void set_depth(const Depth& new_vertex_depth) { depth_ = new_vertex_depth; }

  const Depth& get_depth() { return depth_; }

  const VertexId& get_id() { return id_; }

  void add_edge_id(const EdgeId& new_edge_id) {
    assert(!has_edge_id(new_edge_id) && "Edge id already exists");
    edge_ids_.push_back(new_edge_id);
  }

  bool has_edge_id(const EdgeId& new_edge_id) const {
    if (std::find(edge_ids_.begin(), edge_ids_.end(), new_edge_id) !=
        edge_ids_.end()) {
      return true;
    }
    return false;
  }

  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

  std::string to_string() const {
    std::stringstream ss_out;
    std::string tab_1 = "    ";
    std::string tab_2 = tab_1 + tab_1;
    ss_out << tab_2 << "{\n";
    ss_out << tab_2 << tab_1 << "\"id\": " << id_ << ",\n";
    ss_out << tab_2 << tab_1 << "\"edge_ids\": [";
    for (auto it = edge_ids_.begin(); it != edge_ids_.end(); ++it) {
      if (it != edge_ids_.begin()) {
        ss_out << ", ";
      }
      ss_out << *it;
    }
    ss_out << "],\n";
    ss_out << tab_2 << tab_1 << "\"depth\": " << depth_ << "\n";
    ss_out << tab_2 << "}";
    return ss_out.str();
  }

 private:
  const VertexId id_ = 0;
  Depth depth_ = 0;
  std::vector<EdgeId> edge_ids_;
};

class Edge {
 public:
  enum class Color { GRAY, GREEN, BLUE, YELLOW, RED };

  Edge(const VertexId& from_vertex_id,
       const VertexId& to_vertex_id,
       const EdgeId& new_edge_id,
       const Color& new_edge_color)
      : ver_id1_(from_vertex_id),
        ver_id2_(to_vertex_id),
        id_(new_edge_id),
        color_(new_edge_color) {}

  std::pair<VertexId, VertexId> get_binded_vertices() const {
    return {ver_id1_, ver_id2_};
  }

  std::string color_to_string(const Color& color) const {
    switch (color) {
      case Color::GRAY:
        return "gray";
        break;
      case Color::GREEN:
        return "green";
        break;
      case Color::BLUE:
        return "blue";
        break;
      case Color::YELLOW:
        return "yellow";
        break;
      case Color::RED:
        return "red";
        break;
      default:
        assert("Unexpected behavior");
        return "";
        break;
    }
  }

  std::string to_string() const {
    std::stringstream ss_out;
    std::string tab_1 = "    ";
    std::string tab_2 = tab_1 + tab_1;
    ss_out << tab_2 << "{\n";
    ss_out << tab_2 << tab_1 << "\"id\": " << id_ << ",\n";
    ss_out << tab_2 << tab_1 << "\"vertex_ids\": [";
    ss_out << ver_id1_ << ", " << ver_id2_;
    ss_out << "],\n";
    ss_out << tab_2 << tab_1 << "\"color\": "
           << "\"" << color_to_string(color_) << "\""
           << "\n";
    ss_out << tab_2 << "}";
    return ss_out.str();
  }

 private:
  const VertexId ver_id1_ = 0;
  const VertexId ver_id2_ = 0;
  const EdgeId id_ = 0;
  const Color color_;
};

class Graph {
 public:
  const VertexId& add_vertex() {
    auto new_vertex_id = get_default_vertex_id();
    auto new_vertex = vertex_map_.insert(
        {new_vertex_id, Vertex(new_vertex_id, DEFAULT_DEPTH)});
    depth_map_.at(DEFAULT_DEPTH).push_back(new_vertex.first->first);
    return new_vertex.first->first;
  }

  void set_vertex_depth(const VertexId& from_vertex_id,
                        const VertexId& to_vertex_id) {
    auto new_vertex_depth = vertex_map_.at(from_vertex_id).get_depth() + 1;
    auto& to_vertex = vertex_map_.at(to_vertex_id);

    auto& depth_map_default_level = depth_map_.at(to_vertex.get_depth());
    auto vertex_iter_at_depth_map =
        std::find(depth_map_default_level.begin(),
                  depth_map_default_level.end(), to_vertex.get_id());
    depth_map_default_level.erase(vertex_iter_at_depth_map);
    to_vertex.set_depth(new_vertex_depth);

    if (depth_map_.size() <= new_vertex_depth) {
      depth_map_.push_back(std::vector<VertexId>({to_vertex.get_id()}));
    } else {
      depth_map_.at(new_vertex_depth).push_back(to_vertex.get_id());
    }
  }

  void add_edge(const VertexId& from_vertex_id,
                const VertexId& to_vertex_id,
                const Edge::Color& new_edge_color = Edge::Color::GRAY) {
    assert(!check_binding(from_vertex_id, to_vertex_id) &&
           "Vertices already binded");

    if (new_edge_color == Edge::Color::GRAY) {
      set_vertex_depth(from_vertex_id, to_vertex_id);
    }

    auto new_edge_id = get_default_edge_id();
    const auto new_edge =
        edge_map_.insert({new_edge_id, Edge(from_vertex_id, to_vertex_id,
                                            new_edge_id, new_edge_color)});
    vertex_map_.at(from_vertex_id).add_edge_id(new_edge.first->first);
    if (from_vertex_id != to_vertex_id) {
      vertex_map_.at(to_vertex_id).add_edge_id(new_edge.first->first);
    }
  }

  bool check_binding(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) const {
    assert(vertex_map_.find(from_vertex_id) != vertex_map_.end() &&
           "Vertex doesn't exists");
    assert(vertex_map_.find(to_vertex_id) != vertex_map_.end() &&
           "Vertex doesn't exists");
    const auto& from_vertex = vertex_map_.at(from_vertex_id);
    const auto& to_vertex = vertex_map_.at(to_vertex_id);
    if (from_vertex_id == to_vertex_id) {
      for (const auto& from_vertex_edge_id : from_vertex.get_edge_ids()) {
        const auto& binded_vertices =
            edge_map_.at(from_vertex_edge_id).get_binded_vertices();
        if (binded_vertices.first == binded_vertices.second) {
          return true;
        }
      }
    } else {
      for (const auto& from_vertex_edge_id : from_vertex.get_edge_ids()) {
        if (to_vertex.has_edge_id(from_vertex_edge_id)) {
          return true;
        }
      }
    }
    return false;
  }

  int get_vertices_count() const { return vertex_map_.size(); }

  Depth get_depth() const {
    return (depth_map_.size() > DEFAULT_DEPTH) ? (depth_map_.size() - 1)
                                               : DEFAULT_DEPTH;
    ;
  }

  const std::vector<VertexId>& get_vertices_at_depth(const Depth& depth) {
    return depth_map_[depth];
  }

  std::string to_string() const {
    std::stringstream ss_out;
    std::string tab_1 = "    ";

    ss_out << "{\n";
    ss_out << tab_1 << "\"depth\": " << this->get_depth() << ",\n";
    ss_out << tab_1 << "\"vertices\": [\n";
    for (auto it = vertex_map_.begin(); it != vertex_map_.end(); ++it) {
      if (it != vertex_map_.begin()) {
        ss_out << ", ";
      }
      ss_out << it->second.to_string();
    }
    ss_out << std::endl << tab_1 << "],\n";

    ss_out << tab_1 << "\"edges\": [\n";
    for (auto it = edge_map_.begin(); it != edge_map_.end(); ++it) {
      if (it != edge_map_.begin()) {
        ss_out << ", ";
      }
      ss_out << it->second.to_string();
    }
    ss_out << std::endl << tab_1 << "]\n";
    ss_out << "}" << std::endl;
    return ss_out.str();
  }

 private:
  VertexId default_vertex_id_ = 0;
  EdgeId default_edge_id_ = 0;
  std::unordered_map<VertexId, Vertex> vertex_map_;
  std::unordered_map<EdgeId, Edge> edge_map_;
  std::vector<std::vector<VertexId>> depth_map_ = {{}};

  VertexId get_default_vertex_id() { return default_vertex_id_++; }

  EdgeId get_default_edge_id() { return default_edge_id_++; }
};
