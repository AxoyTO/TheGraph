#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  explicit Vertex(const VertexId& vertex_id) : id_(vertex_id) {}
  std::string get_json_string() const {
    std::stringstream json_stringstream;
    json_stringstream << "{\"id\":" << id_ << ","
                      << "\"edge_ids\":[";
    for (auto it = adjacted_edges_.begin(); it != adjacted_edges_.end(); ++it) {
      json_stringstream << *it;
      if (std::next(it) != adjacted_edges_.end()) {
        json_stringstream << ",";
      }
    }
    json_stringstream << "]}";
    return json_stringstream.str();
  }
  bool is_edge_adjacted(const EdgeId& edge_id) const {
    return adjacted_edges_.find(edge_id) != adjacted_edges_.end();
  }
  void add_edge(const EdgeId& edge_id) {
    assert(!is_edge_adjacted(edge_id) &&
           "edge that is to be added already exists");
    adjacted_edges_.insert(edge_id);
  }

 private:
  const VertexId id_;
  std::set<EdgeId> adjacted_edges_;
};

class Edge {
 public:
  Edge(const EdgeId& edge_id, const VertexId& vertex1, const VertexId& vertex2)
      : id_(edge_id), vertex1_id_(vertex1), vertex2_id_(vertex2) {}
  VertexId get_first_vertex() const { return vertex1_id_; }
  VertexId get_second_vertex() const { return vertex2_id_; }
  std::string get_json_string() const {
    std::stringstream json_stringstream;
    json_stringstream << "{\"id\":" << id_ << ","
                      << "\"vertex_ids\":[" << vertex1_id_ << "," << vertex2_id_
                      << "]}";
    return json_stringstream.str();
  }

 private:
  const EdgeId id_;
  const VertexId vertex1_id_, vertex2_id_;
};

class Graph {
 public:
  bool is_vertex_exists(const VertexId& vertex) const {
    return vertices_.find(vertex) != vertices_.end();
  }

  bool is_connected(const VertexId& vertex1, const VertexId& vertex2) const {
    for (const auto& edge : edges_) {
      if ((edge.second.get_first_vertex() == vertex1 &&
           edge.second.get_second_vertex() == vertex2) ||
          (edge.second.get_first_vertex() == vertex2 &&
           edge.second.get_second_vertex() == vertex1)) {
        return true;
      }
    }
    return false;
  }

  VertexId add_vertex() {
    VertexId new_vertex_id = next_vertex_id_;
    update_next_vertex_id_();
    vertices_.emplace(new_vertex_id, new_vertex_id);
    return new_vertex_id;
  }

  EdgeId add_edge(const VertexId& vertex1, const VertexId& vertex2) {
    assert(is_vertex_exists(vertex1) && "Vertex 1 doesn't exist");
    assert(is_vertex_exists(vertex2) && "Vertex 2 doesn't exist");
    assert(!is_connected(vertex1, vertex2) && "Vertices already connected");

    EdgeId new_edge_id = next_edge_id_;
    update_next_edge_id_();

    auto it_vertex1 = vertices_.find(vertex1);
    it_vertex1->second.add_edge(new_edge_id);

    auto it_vertex2 = vertices_.find(vertex2);
    it_vertex2->second.add_edge(new_edge_id);

    edges_.emplace(new_edge_id, Edge(new_edge_id, vertex1, vertex2));
    return new_edge_id;
  }

  std::string get_json_string() const {
    std::stringstream json_stringstream;
    json_stringstream << "{\"vertices\":[";
    for (auto it = vertices_.begin(); it != vertices_.end(); ++it) {
      json_stringstream << it->second.get_json_string();
      if (std::next(it) != vertices_.end()) {
        json_stringstream << ",";
      }
    }
    json_stringstream << "],\"edges\":[";
    for (auto it = edges_.begin(); it != edges_.end(); ++it) {
      json_stringstream << it->second.get_json_string();
      if (std::next(it) != edges_.end()) {
        json_stringstream << ",";
      }
    }
    json_stringstream << "]}";
    return json_stringstream.str();
  }

 private:
  VertexId next_vertex_id_{};
  EdgeId next_edge_id_{};
  std::map<VertexId, Vertex> vertices_;
  std::map<EdgeId, Edge> edges_;
  void update_next_vertex_id_() { ++next_vertex_id_; }
  void update_next_edge_id_() { ++next_edge_id_; }
};

Graph task_02_get_graph() {
  Graph task_02_graph;
  const size_t n = 14;
  std::vector<VertexId> added_vertices;
  for (size_t i = 0; i < n; ++i) {
    auto new_vertex = task_02_graph.add_vertex();
    added_vertices.push_back(new_vertex);
  }
  const std::vector<std::pair<size_t, size_t>> edge_vertices_indexes{
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};
  for (const auto& edge : edge_vertices_indexes) {
    VertexId vertex1 = added_vertices[edge.first];
    VertexId vertex2 = added_vertices[edge.second];
    task_02_graph.add_edge(vertex1, vertex2);
  }
  return task_02_graph;
}

int main() {
  const Graph task_02_graph = task_02_get_graph();
  const std::string json_string = task_02_graph.get_json_string();

  std::fstream json_file;
  json_file.open("graph.json", std::ios::out);
  if (!json_file) {
    throw std::runtime_error("file not created");
  }
  json_file << json_string << std::endl;
  json_file.close();
  return 0;
}
