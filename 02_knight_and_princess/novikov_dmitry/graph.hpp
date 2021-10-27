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

namespace {}

class Vertex {
 public:
  Depth depth = 0;
  const VertexId id = 0;

  Vertex(const VertexId& new_vertex_id) : id(new_vertex_id) {}

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
    ss_out << tab_2 << tab_1 << "\"id\": " << id << ",\n";
    ss_out << tab_2 << tab_1 << "\"edge_ids\": [";
    for (auto it = edge_ids_.begin(); it != edge_ids_.end(); ++it) {
      if (it != edge_ids_.begin()) {
        ss_out << ", ";
      }
      ss_out << *it;
    }
    ss_out << "],\n";
    ss_out << tab_2 << tab_1 << "\"depth\": " << depth << "\n";
    ss_out << tab_2 << "}";
    return ss_out.str();
  }

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

std::string color_to_string(const Edge::Color& color) {
  switch (color) {
    case Edge::Color::Gray:
      return "gray";
      break;
    case Edge::Color::Green:
      return "green";
      break;
    case Edge::Color::Blue:
      return "blue";
      break;
    case Edge::Color::Yellow:
      return "yellow";
      break;
    case Edge::Color::Red:
      return "red";
      break;
    default:
      assert(false && "Unexpected behavior");
      return "";
      break;
  }
}

std::string Edge::to_string() const {
  std::stringstream ss_out;
  std::string tab_1 = "    ";
  std::string tab_2 = tab_1 + tab_1;
  ss_out << tab_2 << "{\n";
  ss_out << tab_2 << tab_1 << "\"id\": " << id_ << ",\n";
  ss_out << tab_2 << tab_1 << "\"vertex_ids\": [";
  ss_out << ver_id1_ << ", " << ver_id2_;
  ss_out << "],\n";
  ss_out << tab_2 << tab_1 << "\"color\": "
         << "\"" << color_to_string(color) << "\""
         << "\n";
  ss_out << tab_2 << "}";
  return ss_out.str();
}

class Graph {
 public:
  VertexId add_vertex() {
    const auto new_vertex_id = get_default_vertex_id();
    vertex_map_.insert({new_vertex_id, Vertex(new_vertex_id)});
    depth_map_.at(DEFAULT_DEPTH).push_back(new_vertex_id);
    return new_vertex_id;
  }

  void set_vertex_depth(const VertexId& from_vertex_id,
                        const VertexId& to_vertex_id) {
    assert(vertex_map_.find(from_vertex_id) != vertex_map_.end() &&
           "Vertex doesn't exists");
    assert(vertex_map_.find(to_vertex_id) != vertex_map_.end() &&
           "Vertex doesn't exists");
    assert(check_binding(from_vertex_id, to_vertex_id) &&
           "Vertices already binded");
    VertexId parent_vertex_id = from_vertex_id;
    VertexId son_vertex_id = to_vertex_id;

    if (from_vertex_id > to_vertex_id) {
      parent_vertex_id = to_vertex_id;
      son_vertex_id = from_vertex_id;
    }
    const auto new_son_vertex_depth =
        vertex_map_.at(parent_vertex_id).depth + 1;

    auto& son_vertex = vertex_map_.at(son_vertex_id);
    auto& depth_map_default_level = depth_map_.at(son_vertex.depth);

    const auto vertex_iter_at_depth_map =
        std::remove(depth_map_default_level.begin(),
                    depth_map_default_level.end(), son_vertex.id);
    depth_map_default_level.erase(vertex_iter_at_depth_map);
    son_vertex.depth = new_son_vertex_depth;

    if (depth_map_.size() <= new_son_vertex_depth) {
      depth_map_.push_back(std::vector<VertexId>({son_vertex.id}));
    } else {
      depth_map_.at(new_son_vertex_depth).push_back(son_vertex.id);
    }
  }

  void add_edge(const VertexId& from_vertex_id,
                const VertexId& to_vertex_id,
                const Edge::Color& new_edge_color = Edge::Color::Gray) {
    assert(vertex_map_.find(from_vertex_id) != vertex_map_.end() &&
           "Vertex doesn't exists");
    assert(vertex_map_.find(to_vertex_id) != vertex_map_.end() &&
           "Vertex doesn't exists");
    assert(!check_binding(from_vertex_id, to_vertex_id) &&
           "Vertices already binded");

    const auto new_edge_id = get_default_edge_id();
    const auto new_edge =
        edge_map_.insert({new_edge_id, Edge(from_vertex_id, to_vertex_id,
                                            new_edge_id, new_edge_color)});
    vertex_map_.at(from_vertex_id).add_edge_id(new_edge.first->first);
    if (from_vertex_id != to_vertex_id) {
      vertex_map_.at(to_vertex_id).add_edge_id(new_edge.first->first);
    }
    assert(check_color_valid(new_edge.first->second) && "Not valid color");
    if (new_edge_color == Edge::Color::Gray) {
      set_vertex_depth(from_vertex_id, to_vertex_id);
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

  Depth get_depth() const {
    return (depth_map_.size() > DEFAULT_DEPTH) ? (depth_map_.size() - 1)
                                               : DEFAULT_DEPTH;
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

  bool check_gray_valid(const Edge& edge) const {
    const auto& binded_vertices = edge.get_binded_vertices();
    if (vertex_map_.at(binded_vertices.second).get_edge_ids().size() ==
        1) {  //только текущее ребро
      return true;
    }
    return false;
  }

  bool check_green_valid(const Edge& edge) const {
    const auto& binded_vertices = edge.get_binded_vertices();
    const auto& first_vertex = vertex_map_.at(binded_vertices.first);
    const auto& second_vertex = vertex_map_.at(binded_vertices.second);
    if (first_vertex.depth == second_vertex.depth) {
      return true;
    }
    return false;
  }

  bool check_blue_valid(const Edge& edge) const {
    const auto& binded_vertices = edge.get_binded_vertices();
    const auto& first_vertex = vertex_map_.at(binded_vertices.first);
    const auto& second_vertex = vertex_map_.at(binded_vertices.second);
    if (first_vertex.depth == second_vertex.depth) {
      const auto& depth_map_level = depth_map_.at(first_vertex.depth);
      const auto& first_vertex_iter = std::find(
          depth_map_level.begin(), depth_map_level.end(), first_vertex.id);
      if ((first_vertex_iter == depth_map_level.begin() &&
           *(first_vertex_iter + 1) == second_vertex.id) ||
          (first_vertex_iter == (depth_map_level.end() - 1) &&
           *(first_vertex_iter - 1) == second_vertex.id) ||
          ((*(first_vertex_iter + 1) == second_vertex.id) ||
           *(first_vertex_iter - 1) == second_vertex.id)) {
        return true;
      }
    }
    return false;
  }

  bool check_yellow_valid(const Edge& edge) const {
    const auto& binded_vertices = edge.get_binded_vertices();
    const auto& first_vertex = vertex_map_.at(binded_vertices.first);
    const auto& second_vertex = vertex_map_.at(binded_vertices.second);
    if (std::abs(first_vertex.depth - second_vertex.depth) == 1) {
      return true;
    }
    return false;
  }

  bool check_red_valid(const Edge& edge) const {
    const auto& binded_vertices = edge.get_binded_vertices();
    const auto& first_vertex = vertex_map_.at(binded_vertices.first);
    const auto& second_vertex = vertex_map_.at(binded_vertices.second);
    if (std::abs(first_vertex.depth - second_vertex.depth) == 2) {
      return true;
    }
    return false;
  }

  bool check_color_valid(const Edge& edge) const {
    switch (edge.color) {
      case Edge::Color::Gray:
        return check_gray_valid(edge);
        break;
      case Edge::Color::Green:
        return check_green_valid(edge);
        break;
      case Edge::Color::Blue:
        return check_blue_valid(edge);
        break;
      case Edge::Color::Yellow:
        return check_yellow_valid(edge);
        break;
      case Edge::Color::Red:
        return check_red_valid(edge);
        break;
      default:
        assert(false && "Unexpected behavior");
        return false;
        break;
    }
  }
};
