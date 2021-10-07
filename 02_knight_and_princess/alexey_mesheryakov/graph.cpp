#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

constexpr int LEN = 14;

using std::cout;
using std::pair;
using std::vector;
using EdgeId = int;
using VertexId = int;

class Graph {
 public:
  struct Edge {
    EdgeId id = 0;
    pair<VertexId, VertexId> vertex = {-1, -1};
  };

  struct Vertex {
    int id = -1;
    vector<EdgeId> edges = {};
  };

  void add_vertex(const Vertex& v) { vertex_ids_.push_back(v); }

  void add_edge(Edge edge) {
    edges_.push_back(edge);
    try {
      vertex_ids_[edge.vertex.first].edges.push_back(edge.id);
      vertex_ids_[edge.vertex.second].edges.push_back(edge.id);
    } catch (std::out_of_range) {
      cout << "ERROR: id does'n exist\n";
    }
  }

  void set_params(const vector<Edge>& new_edges = {},
                  const vector<int>& vertex_ids = {}) {
    for (const auto& id : vertex_ids)
      add_vertex({id, {}});
    for (const auto& edge : new_edges)
      add_edge(edge);
  }

  std::string to_string() const {
    std::stringstream buffer;
    for (auto edge : edges_)
      buffer << edge.id << " " << vertex_ids_[edge.vertex.first].id << " "
             << vertex_ids_[edge.vertex.second].id << "\n";
    return buffer.str();
  }

  std::string to_json() const {
    std::stringstream buffer;
    buffer << "{\"vertices\":[";
    for (int j = 0; j < vertex_ids_.size(); j++) {
      Vertex v = vertex_ids_[j];
      buffer << "{\"id\":" << v.id << ",\"edge_ids\":[";
      for (int i = 0; i < v.edges.size() - 1; i++)
        buffer << v.edges[i] << ",";
      buffer << v.edges[v.edges.size() - 1] << "]}";
      if (j != vertex_ids_.size() - 1)
        buffer << ",";
    }
    buffer << "],\"edges\":[";
    for (int j = 0; j < edges_.size(); j++) {
      Edge e = edges_[j];
      buffer << "{\"id\":" << e.id << ",\"vertex_ids\":[" << e.vertex.first
             << "," << e.vertex.second << "]}";
      if (j != edges_.size() - 1)
        buffer << ",";
    }
    buffer << "]}\n";
    return buffer.str();
  }

 private:
  vector<Edge> edges_ = {};
  vector<Vertex> vertex_ids_ = {};

  void set_vertex_ids(vector<Vertex> ids) { vertex_ids_ = ids; }
};

int main() {
  vector<int> info(LEN);
  for (int i = 0; i < LEN; i++)
    info[i] = i;
  const vector<Graph::Edge> edges = {
      {0, {0, 1}},    {1, {0, 2}},   {2, {0, 3}},   {3, {1, 4}},
      {4, {1, 5}},    {5, {1, 6}},   {6, {2, 7}},   {7, {2, 8}},
      {8, {3, 9}},    {9, {4, 10}},  {10, {5, 10}}, {11, {6, 10}},
      {12, {7, 11}},  {13, {8, 11}}, {14, {9, 12}}, {15, {10, 13}},
      {16, {11, 13}}, {17, {12, 13}}};
  Graph graph;
  graph.set_params(edges, info);
  cout << graph.to_string();
  std::ofstream out("graph.json");
  std::streambuf* old = cout.rdbuf();
  cout.rdbuf(out.rdbuf());
  cout << graph.to_json();
  cout.rdbuf(old);
  return 0;
}
