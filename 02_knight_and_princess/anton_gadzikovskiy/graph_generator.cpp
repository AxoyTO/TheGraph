#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using std::array;
using std::string;
using std::to_string;
using std::vector;
using VertexId = int;
using EdgeId = int;
constexpr int kVerticesCount = 14;

struct Vertex {
  VertexId id;
  vector<int> edge_ids;
};
struct Edge {
  EdgeId id;
  std::pair<int, int> vertex_ids;
};
class Graph {
 public:
  vector<Vertex> vertices;
  vector<Edge> edges;
  void add_vertex() {
    Vertex new_vertex;
    new_vertex.id = vertices.size();
    new_vertex.edge_ids = {};
    vertices.push_back(new_vertex);
  }
  void add_edge(const VertexId from_vertex_id, const VertexId to_vertex_id) {
    EdgeId new_edge_id = edges.size();
    Edge new_edge;
    new_edge.id = new_edge_id;
    new_edge.vertex_ids = std::make_pair(from_vertex_id, to_vertex_id);
    edges.push_back(new_edge);
    vertices[from_vertex_id].edge_ids.push_back(new_edge_id);
    vertices[to_vertex_id].edge_ids.push_back(new_edge_id);
  };
};

string print_vertex(const Vertex& vertex) {
  string vertex_output =
      "{\n\t\t\t\"id\": " + to_string(vertex.id) + ",\n\t\t\t\"edge_ids\": [";
  for (const auto& edge_id : vertex.edge_ids) {
    if (edge_id != vertex.edge_ids.back()) {
      vertex_output = vertex_output + to_string(edge_id) + ", ";
    } else {
      vertex_output = vertex_output + to_string(edge_id);
    }
  }
  vertex_output += "]\n\t\t}";
  return vertex_output;
}

string print_edge(const Edge& edge) {
  return "{\n\t\t\t\"id\": " + std::to_string(edge.id) +
         ",\n\t\t\t\"vertex_ids\": [" + to_string(edge.vertex_ids.first) +
         ", " + to_string(edge.vertex_ids.second) + "]\n\t\t}";
}

string print_graph(const Graph& graph) {
  string graph_output = "{\n\t\"vertices\": [\n\t\t";
  for (const auto& vertex : graph.vertices) {
    graph_output += print_vertex(vertex);
    if (vertex.id != graph.vertices.back().id) {
      graph_output += ",\n\t\t";
    } else {
      graph_output += "\n\t";
    }
  }
  graph_output += "],\n\t\"edges\": [\n\t\t";
  for (const auto& edge : graph.edges) {
    graph_output += print_edge(edge);
    if (edge.id != graph.edges.back().id) {
      graph_output += ",\n\t\t";
    } else {
      graph_output += "\n\t";
    }
  }
  graph_output += "]\n}\n";
  return graph_output;
}

int main() {
  auto graph = Graph();

  for (int i = 0; i < kVerticesCount; i++) {
    graph.add_vertex();
  }

  graph.add_edge(0, 1);
  graph.add_edge(0, 2);
  graph.add_edge(0, 3);
  graph.add_edge(1, 4);
  graph.add_edge(1, 5);
  graph.add_edge(1, 6);
  graph.add_edge(2, 7);
  graph.add_edge(2, 8);
  graph.add_edge(3, 9);
  graph.add_edge(4, 10);
  graph.add_edge(5, 10);
  graph.add_edge(6, 10);
  graph.add_edge(7, 11);
  graph.add_edge(8, 11);
  graph.add_edge(9, 12);
  graph.add_edge(10, 13);
  graph.add_edge(11, 13);
  graph.add_edge(12, 13);

  std::ofstream graph_file;
  graph_file.open("graph.json");
  graph_file << print_graph(graph);
}
