#include <fstream>
#include <assert.h>
#include <sstream>
#include <string>
#include <vector>

constexpr int VERTEX_CNT = 14;

using std::pair;
using std::vector;
using EdgeId = int;
using VertexId = int;

class Graph {
 public:
  class Edge {
   public:
    Edge(const pair<VertexId, VertexId>& vertex_ids, const EdgeId& edge_id)
        : vertex_ids_(vertex_ids), id_(edge_id) {}

    std::string to_string() const {
      std::stringstream buffer;
      buffer << "{\"id\":" << id_ << ",\"vertex_ids\":[" << vertex_ids_.first
             << "," << vertex_ids_.second << "]}";
      return buffer.str();
    }

   private:
    EdgeId id_ = 0;
    pair<VertexId, VertexId> vertex_ids_ = {-1, -1};
  };

  class Vertex {
   public:
    Vertex(const VertexId& id) : id_(id) {}

    std::string to_string() const {
      std::stringstream buffer;
      buffer << "{\"id\":" << id_ << ",\"edge_ids\":[";
      for (int i = 0; i < edges_.size() - 1; i++)
        buffer << edges_[i] << ",";
      buffer << edges_[edges_.size() - 1] << "]}";
      return buffer.str();
    }

    void add_edge(const EdgeId& edge_id) { edges_.push_back(edge_id); }

   private:
    VertexId id_ = -1;
    vector<EdgeId> edges_ = {};
  };

  bool vertex_exist(const VertexId& id) {
    return (0 <= id) && (id < vertex_id_counter_);
  }

  void add_vertex() { vertices_.push_back(Vertex(vertex_id_counter_++)); }

  void add_edge(const pair<VertexId, VertexId>& vertices) {
    assert(vertex_exist(vertices.first) && "Source vertex id doesn't exist");
    assert(vertex_exist(vertices.second) &&
           "Destination vertex id doesn't exist");

    edges_.push_back(Edge(vertices, edge_id_counter_));
    vertices_[vertices.first].add_edge(edge_id_counter_);
    vertices_[vertices.second].add_edge(edge_id_counter_);
    edge_id_counter_++;
  }

  std::string to_json() const {
    std::stringstream buffer;
    buffer << "{\"vertices\":[";
    for (int j = 0; j < vertices_.size(); j++) {
      Vertex vertex = vertices_[j];
      buffer << vertex.to_string();
      if (j != vertices_.size() - 1)
        buffer << ",";
    }
    buffer << "],\"edges\":[";
    for (int j = 0; j < edges_.size(); j++) {
      Edge edge = edges_[j];
      buffer << edge.to_string();
      if (j != edges_.size() - 1)
        buffer << ",";
    }
    buffer << "]}\n";
    return buffer.str();
  }

 private:
  vector<Edge> edges_ = {};
  vector<Vertex> vertices_ = {};
  VertexId vertex_id_counter_ = 0;
  EdgeId edge_id_counter_ = 0;
};

int main() {
  Graph graph;
  const vector<pair<VertexId, VertexId> > edges = {
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};
  for (int i = 0; i < VERTEX_CNT; i++) {
    graph.add_vertex();
  }
  for (const auto& edge : edges) {
    graph.add_edge(edge);
  }
  std::ofstream file;
  file.open("graph.json", std::fstream::out | std::fstream::trunc);
  file << graph.to_json();
  file.close();
  return 0;
}
