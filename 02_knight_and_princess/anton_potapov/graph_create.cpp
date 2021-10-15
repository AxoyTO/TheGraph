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

  const std::set<EdgeId>& connected_edges() const { return connected_edges_; }

  std::string get_json_string() const {
    std::stringstream json_stringstream;
    json_stringstream << "{\"id\":" << id_ << ","
                      << "\"edge_ids\":[";
    for (auto it = connected_edges_.begin(); it != connected_edges_.end();
         ++it) {
      json_stringstream << *it;
      if (std::next(it) != connected_edges_.end()) {
        json_stringstream << ",";
      }
    }
    json_stringstream << "]}";
    return json_stringstream.str();
  }
  bool has_edge_id(const EdgeId& edge_id) const {
    return connected_edges_.find(edge_id) != connected_edges_.end();
  }
  void add_edge(const EdgeId& edge_id) {
    assert(!has_edge_id(edge_id) && "edge that is to be added already exists");
    connected_edges_.insert(edge_id);
  }

 private:
  const VertexId id_;
  std::set<EdgeId> connected_edges_;
};

class Edge {
 public:
  const VertexId vertex1_id, vertex2_id;
  Edge(const EdgeId& edge_id, const VertexId& vertex1, const VertexId& vertex2)
      : vertex1_id(vertex1), vertex2_id(vertex2), id_(edge_id) {}
  std::string get_json_string() const {
    std::stringstream json_stringstream;
    json_stringstream << "{\"id\":" << id_ << ","
                      << "\"vertex_ids\":[" << vertex1_id << "," << vertex2_id
                      << "]}";
    return json_stringstream.str();
  }

 private:
  const EdgeId id_;
};

class Graph {
 public:
  bool is_vertex_exists(const VertexId& vertex) const {
    return vertices_.find(vertex) != vertices_.end();
  }

  bool is_connected(const VertexId& vertex1, const VertexId& vertex2) const {
    assert(is_vertex_exists(vertex1) && "Vertex 1 doesn't exist");
    assert(is_vertex_exists(vertex2) && "Vertex 2 doesn't exist");
    auto it_vertex1 = vertices_.find(vertex1);
    for (const auto& vertex1_edge : it_vertex1->second.connected_edges()) {
      auto it_vertex2 = vertices_.find(vertex2);
      if (it_vertex2->second.has_edge_id(vertex1_edge)) {
        return true;
      }
    }
    return false;
  }

  VertexId add_vertex() {
    const VertexId new_vertex_id = get_next_vertex_id_();
    vertices_.emplace(new_vertex_id, new_vertex_id);
    return new_vertex_id;
  }

  EdgeId add_edge(const VertexId& vertex1, const VertexId& vertex2) {
    assert(is_vertex_exists(vertex1) && "Vertex 1 doesn't exist");
    assert(is_vertex_exists(vertex2) && "Vertex 2 doesn't exist");
    assert(!is_connected(vertex1, vertex2) && "Vertices already connected");

    const EdgeId new_edge_id = get_next_edge_id_();

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
  VertexId get_next_vertex_id_() {
    VertexId new_vertex_id = next_vertex_id_;
    ++next_vertex_id_;
    return new_vertex_id;
  }
  EdgeId get_next_edge_id_() {
    EdgeId new_edge_id = next_edge_id_;
    ++next_edge_id_;
    return new_edge_id;
  }
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
  if (!json_file.is_open()) {
    throw std::runtime_error("file not created");
  }
  json_file << json_string << std::endl;
  json_file.close();
  return 0;
}
