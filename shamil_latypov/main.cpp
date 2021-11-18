#include <assert.h>
#include <stdlib.h>
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

constexpr int NUM_VERTEX = 14;
constexpr int NUM_EDGE = 18;

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  explicit Vertex(const VertexId& id) : id_(id) {}

  // Возврат значений
  VertexId get_id() const { return id_; }
  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }

  // Добавить id ребра в edge_ids
  void add_edge_id(const EdgeId& id) {
    assert(has_edge_id(id) && "This edge_id has already been added\n");
    edge_ids_.push_back(id);
  }

  bool has_edge_id(const EdgeId& id) const {
    for (const auto& edge_id : edge_ids_) {
      if (edge_id == id)
        return false;
    }
    return true;
  }

 private:
  VertexId id_;
  std::vector<EdgeId> edge_ids_;
};

class Edge {
 public:
  Edge(const VertexId& v1, const VertexId& v2, const EdgeId& id)
      : v1_(v1), v2_(v2), id_(id) {}

  // Возврат значений
  EdgeId get_id() const { return id_; }
  VertexId get_vertex1_id() const { return v1_; }
  VertexId get_vertex2_id() const { return v2_; }

 private:
  EdgeId id_;
  VertexId v1_;
  VertexId v2_;
};

class Graph {
 public:
  // Добавляет ребро в graph
  void add_edge(const VertexId& v1, const VertexId& v2) {
    assert(has_vertex_id(v1) && "Vertex 1 doesnt exist\n");
    assert(has_vertex_id(v2) && "Vertex 2 doesnt exist\n");
    assert(vertices_connected(v1, v2) && "Vertices are connected\n");
    const auto& new_edge = edges_.emplace_back(v1, v2, get_new_edge_id());
    vertices_[v1].add_edge_id(new_edge.get_id());
    vertices_[v2].add_edge_id(new_edge.get_id());
  }

  // Добавляет вершину в граф
  void add_vertex() { vertices_.emplace_back(get_new_vertex_id()); }

  // Возврат векторов
  const std::vector<Vertex>& get_vertices() const { return vertices_; }
  const std::vector<Edge>& get_edges() const { return edges_; }

  bool vertices_connected(const VertexId& v1, const VertexId& v2) const {
    for (const auto& edge_id1 : vertices_[v1].get_edge_ids()) {
      for (const auto& edge_id2 : vertices_[v2].get_edge_ids()) {
        if (edge_id1 == edge_id2) {
          return false;
        }
      }
    }
    return true;
  }

  bool has_vertex_id(const VertexId& id) const {
    for (const auto& vertex_id : vertices_) {
      if (vertex_id.get_id() == id) {
        return false;
      }
    }
    return true;
  }

 private:
  VertexId vert_num_ = 0;
  EdgeId edge_num_ = 0;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;

  VertexId get_new_vertex_id() { return vert_num_++; }
  EdgeId get_new_edge_id() { return edge_num_++; }
};

class GraphPrinter {
 public:
  std::string print(const Graph& graph) const {
    std::stringstream result;
    result << "{\n  \"vertices\": [\n    ";
    bool check_first_comma = false;

    for (const auto& vertex : graph.get_vertices()) {
      if (check_first_comma) {
        result << ",\n    ";
      }

      result << print_vertex(vertex);

      check_first_comma = true;
    }

    result << "\n  ],\n  \"edges\": [\n    ";
    check_first_comma = false;

    for (const auto& edge : graph.get_edges()) {
      if (check_first_comma) {
        result << ",\n    ";
      }
      check_first_comma = true;

      result << print_edge(edge);
    }

    result << "\n  ]\n}\n";

    return result.str();
  }

  std::string print_vertex(const Vertex& vertex) const {
    std::stringstream ss;
    ss << "{\n      \"id\": " << vertex.get_id() << ",\n      \"edge_ids\": [";

    int check_first_comma = false;
    for (const auto& i : vertex.get_edge_ids()) {
      if (check_first_comma) {
        ss << ", ";
      }
      check_first_comma = true;
      ss << i;
    }

    ss << "]\n    }";
    return ss.str();
  }

  std::string print_edge(const Edge& edge) const {
    std::stringstream ss;
    ss << "{\n      \"id\": " << edge.get_id() << ",\n      \"vertex_ids\": ["
       << edge.get_vertex1_id() << ", " << edge.get_vertex2_id() << "]\n    }";
    return ss.str();
  }
};

int main() {
  Graph graph;

  for (int i = 0; i < NUM_VERTEX; i++) {
    graph.add_vertex();
  }

  std::array<std::pair<VertexId, VertexId>, NUM_EDGE> graph_data{{{0, 1},
                                                                  {0, 2},
                                                                  {0, 3},
                                                                  {1, 4},
                                                                  {1, 5},
                                                                  {1, 6},
                                                                  {2, 7},
                                                                  {2, 8},
                                                                  {3, 9},
                                                                  {4, 10},
                                                                  {5, 10},
                                                                  {6, 10},
                                                                  {7, 11},
                                                                  {8, 11},
                                                                  {9, 12},
                                                                  {10, 13},
                                                                  {11, 13},
                                                                  {12, 13}}};

  for (const auto& connection : graph_data) {
    graph.add_edge(connection.first, connection.second);
  }

  GraphPrinter output_graph;

  std::ofstream out_json("out.json");
  out_json << output_graph.print(graph);
  out_json.close();

  return 0;
}
