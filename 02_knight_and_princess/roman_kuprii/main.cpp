#include <array>
#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::to_string;
using std::vector;

// using namespace std;

struct Edge {
  int id;
  std::array<int, 2> nodes;

  Edge(int start, int end, int _id) : id(_id) {
    nodes[0] = start;
    nodes[1] = end;
  }

  std::string to_json() const {
    std::string res;
    res = "{ \"id\": ";
    res += to_string(id);
    res += ", \"vertex_ids\": [";
    res += to_string(nodes[0]);
    res += ", ";
    res += to_string(nodes[1]);
    res += "] }";
    return res;
  }
};

// TODO check uniqueness of ID field
struct Vertex {
  int id;
  std::vector<int> edges_ids;

  Vertex(int _id = -1) : id(_id) {}

  std::string to_json() const {
    std::string res;
    res = "{ \"id\": ";
    res += to_string(id) + ", \"edge_ids\": [";
    for (int n : edges_ids) {
      res += to_string(n);
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += "] }";
    return res;
  }
};

class Graph {
 public:
  Graph() : size_(0) {}

  Graph(const vector<Edge>& init_edges, const vector<Vertex>& init_vertices) {
    vertices_ = init_vertices;
    edges_ = init_edges;
    size_ = vertices_.size();
  }

  std::string to_json() const {
    std::string res;
    res = "{ \"vertices\": [ ";
    for (const auto& v_it : vertices_) {
      res += v_it.to_json();
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += " ], \"edges\": [ ";
    for (const auto& e_it : edges_) {
      res += e_it.to_json();
      res += ", ";
    }
    res.pop_back();
    res.pop_back();
    res += " ] }\n";
    return res;
  }

 private:
  int size_;
  vector<Vertex> vertices_;
  vector<Edge> edges_;
};

int write_graph(const std::string json_name, Graph A) {
  std::ofstream out;
  out.open(json_name, std::ofstream::out | std::ofstream::trunc);

  out << A.to_json();

  out.close();
  return 0;
}

int main() {
  // edge structure: {first vertex, second vertex, edge id}
  vector<Edge> init_edges = {
      {0, 1, 0},    {0, 2, 1},    {0, 3, 2},   {1, 4, 3},   {1, 5, 4},
      {1, 6, 5},    {2, 7, 6},    {2, 8, 7},   {3, 9, 8},   {4, 10, 9},
      {5, 10, 10},  {6, 10, 11},  {7, 11, 12}, {8, 11, 13}, {9, 12, 14},
      {10, 13, 15}, {11, 13, 16}, {12, 13, 17}};

  vector<Vertex> init_vertices;

  for (const auto& it : init_edges) {
    // 1. check if verex id is uniq
    bool if_uniq0 = true, if_uniq1 = true;
    for (const auto& v_it : init_vertices) {
      if_uniq0 *= it.nodes[0] != v_it.id;
      if_uniq1 *= it.nodes[1] != v_it.id;
    }

    // 2. add new vertex, if needed
    if (if_uniq0) {
      init_vertices.push_back(it.nodes[0]);
    }
    if (if_uniq1) {
      init_vertices.push_back(it.nodes[1]);
    }

    // 3. add info about edge id into connected vetices
    init_vertices[it.nodes[0]].edges_ids.push_back(it.id);
    init_vertices[it.nodes[1]].edges_ids.push_back(it.id);
  }

  Graph A(init_edges, init_vertices);
  write_graph("test.json", A);

  return 0;
}
