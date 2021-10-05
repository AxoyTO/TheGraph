#include <assert.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  explicit Vertex(const VertexId& init_id = -1) : id_(init_id) {}

  void add_edge_id(EdgeId init_id) { edge_ids_.push_back(init_id); }

  std::string to_str() const {
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
  Edge() {}
  Edge(VertexId init_ver_id1, EdgeId init_id, VertexId init_ver_id2)
      : ver_id1_(init_ver_id1), id_(init_id), ver_id2_(init_ver_id2) {}

  std::string to_str() const {
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
  using VertexMap = std::unordered_map<VertexId, Vertex>;
  using EdgeMap = std::unordered_map<EdgeId, Edge>;

  VertexMap vertex_map;
  EdgeMap edge_map;

  Graph() = default;

  void add_vertex(VertexId init_ver_id = default_ver_id_--) {
    assert(vertex_map.find(init_ver_id) == vertex_map.end() &&
           "Vertex already exists");
    vertex_map[init_ver_id] = Vertex(init_ver_id);
  }

  void add_edge(VertexId init_ver1_id,
                EdgeId init_edge_id,
                VertexId init_ver2_id) {
    assert(!(vertex_map.find(init_ver1_id) == vertex_map.end()) &&
           "Vertex already not exists");
    assert(!(vertex_map.find(init_ver2_id) == vertex_map.end()) &&
           "Vertex already not exists");
    edge_map[init_edge_id] = Edge(init_ver1_id, init_edge_id, init_ver2_id);
    vertex_map[init_ver1_id].add_edge_id(init_edge_id);
    vertex_map[init_ver2_id].add_edge_id(init_edge_id);
  }

  std::string to_str() const {
    std::stringstream ss_out;
    std::string tab_1 = "    ";

    ss_out << "{\n";
    ss_out << tab_1 << "\"vertices\": [\n";
    for (auto it = vertex_map.begin(); it != vertex_map.end(); ++it) {
      if (it != vertex_map.begin()) {
        ss_out << ", ";
      }
      ss_out << it->second.to_str();
    }
    ss_out << std::endl << tab_1 << "],\n";

    ss_out << tab_1 << "\"edges\": [\n";
    for (auto it = edge_map.begin(); it != edge_map.end(); ++it) {
      if (it != edge_map.begin()) {
        ss_out << ", ";
      }
      ss_out << it->second.to_str();
    }
    ss_out << std::endl << tab_1 << "]\n";
    ss_out << "}" << std::endl;
    return ss_out.str();
  }

 private:
  inline static VertexId default_ver_id_ = -1;
};

int main() {
  Graph G = Graph();
  const std::vector<VertexId> verteces = {0, 1, 2, 3,  4,  5,  6,
                                          7, 8, 9, 10, 11, 12, 13};
  for (const auto& item : verteces) {
    G.add_vertex(item);
  }
  const std::vector<std::vector<int> > g_vec = {
      {0, 0, 1},    {0, 1, 2},    {0, 2, 3},   {1, 3, 4},   {1, 4, 5},
      {1, 5, 6},    {2, 6, 7},    {2, 7, 8},   {3, 8, 9},   {4, 9, 10},
      {5, 10, 10},  {6, 11, 10},  {7, 12, 11}, {8, 13, 11}, {9, 14, 12},
      {10, 15, 13}, {11, 16, 13}, {12, 17, 13}};
  for (const auto& item : g_vec) {
    G.add_edge(item[0], item[1], item[2]);
  }

  std::ofstream file_out;
  file_out.open("Graph.json", std::fstream::out | std::fstream::trunc);
  file_out << G.to_str();
  file_out.close();

  return 0;
}
