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

static VertexId default_ver_id = 0;

class Vertex {
  using EdgeIdVector = std::vector<EdgeId>;

 public:
  Vertex() {}
  explicit Vertex(VertexId init_id) : id_(init_id) {}

  void add_edge_id(EdgeId init_id) { vec_.push_back(init_id); }

  std::string to_str() const {
    std::stringstream ss_out;
    std::string tab_1 = "    ";
    std::string tab_2 = tab_1 + tab_1;
    ss_out << tab_2 << "{\n";
    ss_out << tab_2 << tab_1 << "\"id\": " << id_ << ",\n";
    ss_out << tab_2 << tab_1 << "\"edge_ids\": [";
    for (auto it = vec_.begin(); it != vec_.end(); ++it) {
      if (it != vec_.begin()) {
        ss_out << ", ";
      }
      ss_out << *it;
    }
    ss_out << "]\n" << tab_2 << "}";
    return ss_out.str();
  }
  void get_info() const {
    std::string tab_1 = "    ";
    std::cout << "Vertex {\n";
    std::cout << tab_1 << "VertexId id;\n";
    std::cout << tab_1 << "EdgeIdVector ver;\n";
    std::cout << "}";
  }
  VertexId get_id() const { return id_; }

 private:
  VertexId id_;
  EdgeIdVector vec_;
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

  void get_info() const {
    std::string str = std::string(4, ' ');
    std::cout << "Edge {" << std::endl;
    std::cout << str << "VertexId ver_id1;\n";
    std::cout << str << "EdgeId id;\n";
    std::cout << str << "VertexId ver_id2;\n";
    std::cout << "}";
  }

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

  Graph() {}

  void add_vertex(VertexId init_ver_id = default_ver_id) {
    if (vertex_map.find(init_ver_id) == vertex_map.end()) {
      if (init_ver_id == default_ver_id) {
        --default_ver_id;
      }
      vertex_map[init_ver_id] = Vertex(init_ver_id);
    }
  }

  void add_edge(VertexId init_ver1_id,
                EdgeId init_edge_id,
                VertexId init_ver2_id) {
    this->add_vertex(init_ver1_id);
    this->add_vertex(init_ver2_id);
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
};

int main() {
  const std::vector<std::vector<int> > G_vec = {
      {0, 0, 1},    {0, 1, 2},    {0, 2, 3},   {1, 3, 4},   {1, 4, 5},
      {1, 5, 6},    {2, 6, 7},    {2, 7, 8},   {3, 8, 9},   {4, 9, 10},
      {5, 10, 10},  {6, 11, 10},  {7, 12, 11}, {8, 13, 11}, {9, 14, 12},
      {10, 15, 13}, {11, 16, 13}, {12, 17, 13}};
  Graph G = Graph();
  for (const auto& item : G_vec) {
    G.add_edge(item[0], item[1], item[2]);
  }

  std::ofstream file_out;
  file_out.open("Graph.json", std::fstream::out | std::fstream::trunc);
  file_out << G.to_str();
  file_out.close();

  // std::cout << G.to_str();
  // std::cout << std::endl;
  return 0;
}
