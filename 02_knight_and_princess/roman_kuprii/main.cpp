#include <array>
#include <fstream>
#include <iostream>
#include <vector>

using std::endl;
using std::to_string;
using std::vector;

using EdgeId = int;
using VertexId = int;

constexpr int INVALID_ID = -1;

struct Edge {
  EdgeId id;
  std::array<VertexId, 2> connected_vertices;

  Edge(VertexId start, VertexId end, EdgeId _id) : id(_id), connected_vertices({start, end}) {}

  std::string to_json() const {
    std::string res;
    res = "{ \"id\": ";
    res += to_string(id);
    res += ", \"vertex_ids\": [";
    res += to_string(connected_vertices[0]);
    res += ", ";
    res += to_string(connected_vertices[1]);
    res += "] }";
    return res;
  }
};

// TODO check uniqueness of ID field
struct Vertex {
  VertexId id = INVALID_ID;
  std::vector<EdgeId> edges_ids;

  explicit Vertex(VertexId _id) : id(_id) {}

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
  Graph(const vector<Edge>& init_edges, const vector<Vertex>& init_vertices)
      : vertices_(init_vertices), edges_(init_edges), size_(vertices_.size()) {}

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
  vector<Vertex> vertices_;
  vector<Edge> edges_;
  int size_ = 0;
};

void write_graph(const std::string& json_name, const Graph& A) {
  std::ofstream out;
  out.open(json_name, std::ofstream::out | std::ofstream::trunc);

  out << A.to_json();

  out.close();
}

int main() {
  // edge structure: {first vertex, second vertex, edge id}
  const vector<Edge> init_edges = {
      {0, 1, 0},    {0, 2, 1},    {0, 3, 2},   {1, 4, 3},   {1, 5, 4},
      {1, 6, 5},    {2, 7, 6},    {2, 8, 7},   {3, 9, 8},   {4, 10, 9},
      {5, 10, 10},  {6, 10, 11},  {7, 11, 12}, {8, 11, 13}, {9, 12, 14},
      {10, 13, 15}, {11, 13, 16}, {12, 13, 17}};

  vector<Vertex> init_vertices;

  for (const auto& init_edge : init_edges) {
    // 1. check if verex id is uniq
    bool if_uniq0 = true, if_uniq1 = true;
    for (const auto& v_it : init_vertices) {
      if_uniq0 &= init_edge.connected_vertices[0] != v_it.id;
      if_uniq1 &= init_edge.connected_vertices[1] != v_it.id;
    }

    // 2. add new vertex, if needed
    if (if_uniq0) {
//      init_vertices.push_back(Vertex(init_edge.connected_vertices[0]));
      init_vertices.emplace_back(init_edge.connected_vertices[0]);
    }
    if (if_uniq1) {
      init_vertices.emplace_back(init_edge.connected_vertices[1]);
    }

    // 3. add info about edge id into connected vetices
    init_vertices[init_edge.connected_vertices[0]].edges_ids.push_back(init_edge.id);
    init_vertices[init_edge.connected_vertices[1]].edges_ids.push_back(init_edge.id);
  }

  Graph A(init_edges, init_vertices);
  write_graph("graph.json", A);

  return 0;
}
