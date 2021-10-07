#include <assert.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  explicit Vertex(const VertexId& init_id = -1) : id_(init_id) {}

  void add_edge_id(const EdgeId& init_id) { edge_ids_.push_back(init_id); }

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

  VertexId get_id() const { return id_; }

 private:
  using EdgeIdVector = std::vector<EdgeId>;
  VertexId id_;
  EdgeIdVector edge_ids_;
};

class Edge {
 public:
  Edge() = default;
  Edge(const VertexId& init_ver_id1,
       const EdgeId& init_id,
       const VertexId& init_ver_id2)
      : ver_id1_(init_ver_id1), id_(init_id), ver_id2_(init_ver_id2) {}

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

  EdgeId get_id() const { return id_; }

 private:
  VertexId ver_id1_;
  EdgeId id_;
  VertexId ver_id2_;
};

class Graph {
 public:
  Graph() = default;

  void add_vertex() {
    vertex_map_[default_ver_id_] = Vertex(default_ver_id_);
    ++default_ver_id_;
  }

  void add_edge(const VertexId& init_ver1_id, const VertexId& init_ver2_id) {
    assert(vertex_map_.find(init_ver1_id) != vertex_map_.end() &&
           "Vertex doesn't exists");
    assert(vertex_map_.find(init_ver2_id) != vertex_map_.end() &&
           "Vertex doesn't exists");
    edge_map_[default_edge_id_] =
        Edge(init_ver1_id, default_edge_id_, init_ver2_id);
    vertex_map_[init_ver1_id].add_edge_id(default_edge_id_);
    vertex_map_[init_ver2_id].add_edge_id(default_edge_id_);
    ++default_edge_id_;
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
  using VertexMap = std::unordered_map<VertexId, Vertex>;
  using EdgeMap = std::unordered_map<EdgeId, Edge>;
  VertexMap vertex_map_;
  EdgeMap edge_map_;
};

int main() {
  Graph graph = Graph();

  const int verteces_number = 14;
  for (int i = 0; i < verteces_number; ++i) {
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
