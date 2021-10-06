#include <fstream>
#include <iterator>
#include <map>
#include <utility>
#include <vector>

using VertexId = int;
using EdgeId = int;

class Vertex {
 public:
  explicit Vertex(int init_id = 0) : id(init_id) {}
  int get_id() const { return id; }
  void add_edge(int edge_id) { connected_edges.push_back(edge_id); }
  operator std::string() {
    std::string result = "";
    result += "    {\n      \"id\": " + std::to_string(id);
    result += ",\n      \"edge_ids\": [";

    for (int i = 0; i < connected_edges.size(); i++) {
      result += std::to_string(connected_edges[i]);
      if (i == connected_edges.size() - 1) {
        result += "]\n";
      } else {
        result += ", ";
      }
    }
    result += "\n    }";
    return result;
  }

 private:
  const VertexId id;
  std::vector<int> connected_edges;
};

class Edge {
 public:
  Edge(int init_id, int v1_init, int v2_init)
      : id(init_id), vertex1_id(v1_init), vertex2_id(v2_init){};
  int get_id() const { return id; }
  int get_v1_id() const { return vertex1_id; }
  int get_v2_id() const { return vertex2_id; }
  operator std::string() {
    std::string result = "";
    result += "    {\n      \"id\": " + std::to_string(id);
    result += ",\n      \"vertex_ids\": ";

    result += "[" + std::to_string(vertex1_id);
    result += ", " + std::to_string(vertex2_id);
    result += "]\n    }";

    return result;
  }

 private:
  const EdgeId id;
  int vertex1_id;
  int vertex2_id;
};

class Graph {
 public:
  Graph() {}

  int get_vertices_amount() const { return vertices.size(); }
  int get_edges_amount() const { return edges.size(); }

  void add_new_vertex() { vertices.push_back(Vertex(vertices.size())); }

  void bind_vertices(int id1, int id2) {
    auto& edge = edges.emplace_back(edges.size(), id1, id2);
    connections_map[id1][edge.get_id()] = id2;
    connections_map[id2][edge.get_id()] = id1;
    vertices[id1].add_edge(edge.get_id());
    vertices[id2].add_edge(edge.get_id());
  }

  operator std::string() {
    std::string result = "{\n  \"vertices\": [\n";

    for (int i = 0; i < vertices.size(); i++) {
      std::string vertex_string = (std::string)vertices[i];

      result += vertex_string;
      if (i != vertices.size() - 1)
        result += ",\n";
    }

    result += "\n  ],\n  \"edges\": [\n";

    for (int i = 0; i < edges.size(); i++) {
      std::string edge_string = (std::string)edges[i];

      result += edge_string;
      if (i != edges.size() - 1)
        result += ",\n";
    }

    result += "\n  ]\n}";
    return result;
  }

 private:
  std::vector<Edge> edges;
  std::vector<Vertex> vertices;

  // connections_map: vertex1 -> edge -> vertex2
  std::map<int, std::map<int, int>> connections_map;
};

void generateCustomGraph(Graph& g,
                         int vert_number,
                         std::vector<std::pair<int, int>> connections) {
  for (int i = 0; i < vert_number; i++) {
    g.add_new_vertex();
  }
  for (int i = 0; i < connections.size(); i++) {
    g.bind_vertices(connections[i].first, connections[i].second);
  }
}

int main() {
  Graph graph = Graph();

  const std::vector<std::pair<int, int>> g_connections{
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};

  generateCustomGraph(graph, 14, g_connections);

  std::ofstream file;
  file.open("graph.json");

  if (file.is_open()) {
    file << (std::string)graph << std::endl;
  }

  return 0;
}
