#include <fstream>
#include <iterator>
#include <map>
#include <utility>
#include <vector>

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  explicit Vertex(VertexId init_id = 0) : ID_(init_id) {}
  VertexId get_id() const { return ID_; }
  void add_edge(EdgeId edge_id) { connected_edges_.push_back(edge_id); }
  operator std::string() {
    std::string result = "";
    result += "    {\n      \"id\": " + std::to_string(ID_);
    result += ",\n      \"edge_ids\": [";

    for (int i = 0; i < connected_edges_.size(); i++) {
      result += std::to_string(connected_edges_[i]);
      if (i == connected_edges_.size() - 1) {
        result += "]\n";
      } else {
        result += ", ";
      }
    }
    result += "\n    }";
    return result;
  }

 private:
  const VertexId ID_;
  std::vector<EdgeId> connected_edges_;
};

class Edge {
 public:
  Edge(EdgeId init_id, VertexId v1_init, VertexId v2_init)
      : ID_(init_id), vertex1_id_(v1_init), vertex2_id_(v2_init){};
  EdgeId get_id() const { return ID_; }
  VertexId get_v1_id() const { return vertex1_id_; }
  VertexId get_v2_id() const { return vertex2_id_; }
  operator std::string() {
    std::string result = "";
    result += "    {\n      \"id\": " + std::to_string(ID_);
    result += ",\n      \"vertex_ids\": ";

    result += "[" + std::to_string(vertex1_id_);
    result += ", " + std::to_string(vertex2_id_);
    result += "]\n    }";

    return result;
  }

 private:
  const EdgeId ID_;
  VertexId vertex1_id_;
  VertexId vertex2_id_;
};

class Graph {
 public:

  int get_vertices_amount() const { return vertices_.size(); }
  int get_edges_amount() const { return edges_.size(); }

  void add_new_vertex() { vertices_.push_back(Vertex(vertices_.size())); }

  void bind_vertices(VertexId id1, VertexId id2) {
    const auto& edge = edges_.emplace_back(edges_.size(), id1, id2);
    connections_map_[id1][edge.get_id()] = id2;
    connections_map_[id2][edge.get_id()] = id1;
    vertices_[id1].add_edge(edge.get_id());
    vertices_[id2].add_edge(edge.get_id());
  }

  operator std::string() {
    std::string result = "{\n  \"vertices\": [\n";

    for (int i = 0; i < vertices_.size(); i++) {
      std::string vertex_string = (std::string)vertices_[i];

      result += vertex_string;
      if (i != vertices_.size() - 1)
        result += ",\n";
    }

    result += "\n  ],\n  \"edges\": [\n";

    for (int i = 0; i < edges_.size(); i++) {
      std::string edge_string = (std::string)edges_[i];

      result += edge_string;
      if (i != edges_.size() - 1)
        result += ",\n";
    }

    result += "\n  ]\n}";
    return result;
  }

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;

  // connections_map: vertex1 -> edge -> vertex2
  std::map<VertexId, std::map<EdgeId, VertexId>> connections_map_;
};

Graph generateCustomGraph(int vert_number,
                         const std::vector<std::pair<VertexId, VertexId>> &connections) {
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

  Graph graph = generateCustomGraph(14, g_connections);

  std::ofstream file;
  file.open("graph.json");

  if (file.is_open()) {
    file << (std::string)graph << std::endl;
    file.close();
  }

  return 0;
}
