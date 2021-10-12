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
using EdgeColor = std::string;

class Vertex {
 public:
  Vertex(const VertexId& new_vertex_id, const Depth& new_vertex_depth)
      : id_(new_vertex_id), depth_(new_vertex_depth) {}

  void add_edge_id(const EdgeId& new_edge_id) {
    if (std::find(edge_ids_.begin(), edge_ids_.end(), new_edge_id) !=
        edge_ids_.end()) {
      return;
    }
    edge_ids_.push_back(new_edge_id);
  }

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
  VertexId id_ = 0;
  Depth depth_ = 0;
  std::vector<EdgeId> edge_ids_;
};

class Edge {
 public:
  Edge(const VertexId& from_vertex_id,
       const VertexId& to_vertex_id,
       const EdgeId& new_edge_id,
       const EdgeColor& new_edge_color)
      : ver_id1_(from_vertex_id),
        ver_id2_(to_vertex_id),
        id_(new_edge_id),
        color_(new_edge_color) {}

  std::pair<VertexId, VertexId> get_binded_vertices() const {
    return {ver_id1_, ver_id2_};
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
           << "\"" << color_ << "\""
           << "\n";
    ss_out << tab_2 << "}";
    return ss_out.str();
  }

 private:
  VertexId ver_id1_ = 0;
  VertexId ver_id2_ = 0;
  EdgeId id_ = 0;
  EdgeColor color_;
};

class Graph {
 public:
  void set_depth(const Depth& new_graph_depth = DEFAULT_DEPTH) {
    depth_ = new_graph_depth;
  }

  void add_vertex(const Depth& new_vertex_depth = DEFAULT_DEPTH) {
    vertex_map_.insert(
        {default_ver_id_, Vertex(default_ver_id_, new_vertex_depth)});
    ++default_ver_id_;
  }

  void add_edge(const VertexId& from_vertex_id,
                const VertexId& to_vertex_id,
                const EdgeColor& new_edge_color = "gray") {
    assert(vertex_map_.find(from_vertex_id) != vertex_map_.end() &&
           "Vertex doesn't exists");
    assert(vertex_map_.find(to_vertex_id) != vertex_map_.end() &&
           "Vertex doesn't exists");
    if (check_binding(from_vertex_id, to_vertex_id)) {
      return;
    }
    const auto new_edge = edge_map_.insert(
        {default_edge_id_,
         Edge(from_vertex_id, to_vertex_id, default_edge_id_, new_edge_color)});
    vertex_map_.at(from_vertex_id).add_edge_id(new_edge.first->first);
    vertex_map_.at(to_vertex_id).add_edge_id(new_edge.first->first);
    ++default_edge_id_;
  }

  bool check_binding(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) const {
    for (const auto& item : edge_map_) {
      if (item.second.get_binded_vertices() ==
          std::pair<VertexId, VertexId>(from_vertex_id, to_vertex_id)) {
        return true;
      }
    }
    return false;
  }

  int get_vertices_count() const { return default_ver_id_; }

  Depth get_depth() const { return depth_; }

  std::string to_string() const {
    std::stringstream ss_out;
    std::string tab_1 = "    ";

    ss_out << "{\n";
    ss_out << tab_1 << "\"depth\": " << depth_ << ",\n";
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
  VertexId default_ver_id_ = 0;
  EdgeId default_edge_id_ = 0;
  Depth depth_ = 0;
  std::unordered_map<VertexId, Vertex> vertex_map_;
  std::unordered_map<EdgeId, Edge> edge_map_;
};
