#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <utility>
#include <vector>

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  explicit Vertex(const VertexId& init_id = 0) : id(init_id) {}
  bool check_edge_presence(const EdgeId& edge_id) const {
    for (const auto& id : connected_edges_) {
      if (edge_id == id) {
        return true;
      }
    }
    return false;
  }
  void add_edge(const EdgeId& edge_id) {
    assert(!check_edge_presence(edge_id) &&
           "Attemptig to add added edge to vertex: Error.");
    connected_edges_.push_back(edge_id);
  }
  operator std::string() const {
    std::string result = "";
    result += "{\n\"id\": " + std::to_string(id);
    result += ",\n\"edge_ids\": [";

    for (int i = 0; i < connected_edges_.size(); i++) {
      result += std::to_string(connected_edges_[i]);
      if (i == connected_edges_.size() - 1) {
        result += "]\n";
      } else {
        result += ", ";
      }
    }
    result += "}";
    return result;
  }

  const VertexId id;

 private:
  std::vector<EdgeId> connected_edges_;
};

class Edge {
 public:
  Edge(const EdgeId& init_id, const VertexId& v1_init, const VertexId& v2_init)
      : id(init_id), vertex1_id(v1_init), vertex2_id(v2_init){};
  operator std::string() const {
    std::string result = "";
    result += "{\n\"id\": " + std::to_string(id);
    result += ",\n\"vertex_ids\": ";

    result += "[" + std::to_string(vertex1_id);
    result += ", " + std::to_string(vertex2_id);
    result += "]\n}";

    return result;
  }

  const EdgeId id;
  const VertexId vertex1_id;
  const VertexId vertex2_id;
};

class Graph {
 public:
  Graph(): vertex_id_counter(0), edge_id_counter(0) {}
  int get_vertices_amount() const { return vertices_.size(); }
  int get_edges_amount() const { return edges_.size(); }

  VertexId get_new_vertex_id() { return vertex_id_counter++; }
  EdgeId get_new_edge_id() { return edge_id_counter++; }

  void add_new_vertex() { vertices_.emplace_back(get_new_vertex_id()); }

  bool check_vertex_existence(const VertexId& vertex_id) const {
    for (const auto& vertex : vertices_) {
      if (vertex_id == vertex.id) {
        return true;
      }
    }
    return false;
  }

  bool are_vertices_connected(const VertexId& id1, const VertexId& id2) {
    assert(check_vertex_existence(id1) &&
           "Attemptig to access to nonexistent vertex: Error.");
    assert(check_vertex_existence(id2) &&
           "Attemptig to access to nonexistent vertex: Error.");
    for (const auto& edge : connections_map_[id1]) {
      if (edge->vertex1_id == id2 || edge->vertex2_id == id2) {
        return true;
      }
    }
    return false;
  }

  void bind_vertices(const VertexId& id1, const VertexId& id2) {
    assert(check_vertex_existence(id1) &&
           "Attemptig to access to nonexistent vertex: Error.");
    assert(check_vertex_existence(id2) &&
           "Attemptig to access to nonexistent vertex: Error.");
    assert(!are_vertices_connected(id1, id2) &&
           "Attemptig to connect connected vertices: Error.");
    const auto& edge = edges_.emplace_back(get_new_edge_id(), id1, id2);
    connections_map_[id1].push_back(&edge);
    connections_map_[id2].push_back(&edge);
    vertices_[id1].add_edge(edge.id);
    vertices_[id2].add_edge(edge.id);
  }

  operator std::string() const {
    std::string result = "{\n  \"vertices\": [\n";

    for (int i = 0; i < vertices_.size(); i++) {

      result += std::string(vertices_[i]);
      if (i != vertices_.size() - 1)
        result += ",\n";
    }

    result += "\n  ],\n  \"edges\": [\n";

    for (int i = 0; i < edges_.size(); i++) {

      result += std::string(edges_[i]);
      if (i != edges_.size() - 1)
        result += ",\n";
    }

    result += "\n  ]\n}";
    return result;
  }

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;

  VertexId vertex_id_counter;
  EdgeId edge_id_counter;

  // connections_map: vertex1 -> edge -> vertex2
  std::map<VertexId, std::vector<const Edge*>> connections_map_;
};

Graph generateCustomGraph(
    int vert_number,
    const std::vector<std::pair<VertexId, VertexId>>& connections) {
  Graph graph = Graph();
  for (int i = 0; i < vert_number; i++) {
    graph.add_new_vertex();
  }
  for (const auto& connection : connections) {
    graph.bind_vertices(connection.first, connection.second);
  }
  return graph;
}

int main() {
  const std::vector<std::pair<VertexId, VertexId>> g_connections{
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};

  const Graph graph = generateCustomGraph(14, g_connections);

  std::ofstream file;
  file.open("graph.json");

  if (file.is_open()) {
    file << std::string(graph) << std::endl;
    file.close();
  }

  return 0;
}
