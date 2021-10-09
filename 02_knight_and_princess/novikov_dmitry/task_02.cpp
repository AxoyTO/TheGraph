#include <assert.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

constexpr int VERTICES_NUMBER = 14;

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  explicit Vertex(const VertexId& new_vertex_id) : id_(new_vertex_id) {}

  void add_edge_id(const EdgeId& new_edge_id) {
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
    ss_out << "]\n" << tab_2 << "}";
    return ss_out.str();
  }

 private:
  VertexId id_ = 0;
  std::vector<EdgeId> edge_ids_;
};

class Edge {
 public:
  Edge(const VertexId& from_vertex_id,
       const VertexId& to_vertex_id,
       const EdgeId& new_edge_id)
      : ver_id1_(from_vertex_id), ver_id2_(to_vertex_id), id_(new_edge_id) {}

  std::string to_string() const {
    std::stringstream ss_out;
    std::string tab_1 = "    ";
    std::string tab_2 = tab_1 + tab_1;
    ss_out << tab_2 << "{\n";
    ss_out << tab_2 << tab_1 << "\"id\": " << id_ << ",\n";
    ss_out << tab_2 << tab_1 << "\"vertex_ids\": [";
    ss_out << ver_id1_ << ", " << ver_id2_;
    ss_out << "]\n" << tab_2 << "}";
    return ss_out.str();
  }

  std::pair<VertexId, VertexId> get_binded_vertices() const {
    return {ver_id1_, ver_id2_};
  }

 private:
  VertexId ver_id1_ = 0;
  VertexId ver_id2_ = 0;
  EdgeId id_ = 0;
};

class Graph {
 public:
  void add_vertex() {
    vertex_map_.insert({default_ver_id_, Vertex(default_ver_id_)});
    ++default_ver_id_;
  }

  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id) {
    assert(vertex_map_.find(from_vertex_id) != vertex_map_.end() &&
           "Vertex doesn't exists");
    assert(vertex_map_.find(to_vertex_id) != vertex_map_.end() &&
           "Vertex doesn't exists");
    if (check_binding(from_vertex_id, to_vertex_id)) {
      return;
    }
    const auto new_edge =
        edge_map_.insert({default_edge_id_, Edge(from_vertex_id, to_vertex_id,
                                                 default_edge_id_)});
    vertex_map_.at(from_vertex_id).add_edge_id(new_edge.first->first);
    vertex_map_.at(to_vertex_id).add_edge_id(new_edge.first->first);
    ++default_edge_id_;
  }

  bool check_binding(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) const {
    for (const auto& item : edge_map_) {
      if (item.second.get_binded_vertices() ==
          std::pair(from_vertex_id, to_vertex_id)) {
        return true;
      }
    }
    return false;
  }

  std::string to_string() const {
    std::stringstream ss_out;
    std::string tab_1 = "    ";

    ss_out << "{\n";
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

  std::unordered_map<VertexId, Vertex> vertex_map_;
  std::unordered_map<EdgeId, Edge> edge_map_;
};

int main() {
  auto graph = Graph();

  for (int i = 0; i < VERTICES_NUMBER; ++i) {
    graph.add_vertex();
  }
  const std::vector<std::vector<int> > edge_vec = {
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};
  for (const auto& item : edge_vec) {
    graph.add_edge(item[0], item[1]);
  }

  std::ofstream file_out;
  file_out.open("Graph.json", std::fstream::out | std::fstream::trunc);
  file_out << graph.to_string();
  file_out.close();

  return 0;
}
