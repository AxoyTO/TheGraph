#include <array>
#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

using VertexId = int;
using EdgeId = int;

struct Vertex {
  const VertexId id = 0;
  std::vector<int> edges_id;
};
struct Edge {
  const EdgeId id = 0;
  int vertex_start = 0;
  int vertex_end = 0;
};

class Graph {
 public:
  void add_edges(std::vector<std::array<int, 2>> graph);
  void add_vertices();
  const std::vector<Edge>& get_edges() { return edges_; }
  const std::vector<Vertex>& get_vertices() { return vertices_; }

 private:
  std::vector<Edge> edges_;
  std::vector<Vertex> vertices_;
};

void Graph::add_edges(std::vector<std::array<int, 2>> vertex_connections) {
  for (int it = 0; it < vertex_connections.size(); it++)
    edges_.push_back(
        {it, vertex_connections[it][0], vertex_connections[it][1]});
}
void Graph::add_vertices() {
  int count_vertices = edges_.back().vertex_end;
  for (VertexId vertex = 0; vertex <= count_vertices; vertex++) {
    vertices_.push_back({vertex});
    for (auto edge : edges_) {
      if ((edge.vertex_start == vertex) || (edge.vertex_end == vertex)) {
        vertices_[vertex].edges_id.push_back(edge.id);
      }
    }
  }
}

//Вывод в файл json
void print_vertex_json(std::ofstream& out, Graph& graph) {
  out << "\"vertices\": [\n";
  for (auto vertex : graph.get_vertices()) {
    out << "\t\t{\"id\":" << vertex.id << ",\"edge_ids\":[";
    for (auto idEdges : vertex.edges_id)
      out << idEdges
          << ((idEdges != vertex.edges_id.back())
                  ? ","
                  : ((vertex.id != graph.get_vertices().size() - 1) ? "]},\n"
                                                                    : "]}\n"));
  }
}
void print_edge_json(std::ofstream& out, Graph& graph) {
  out << "\"edges\": [\n";
  for (auto edge : graph.get_edges()) {
    out << "\t\t{\"id\":" << edge.id << ",\"vertex_ids\":[" << edge.vertex_start
        << "," << edge.vertex_end
        << ((edge.id != graph.get_edges().back().id) ? "]},\n" : "]}\n");
  }
  out << "\t]\n";
}
void print_graph_json(Graph& graph) {
  std::ofstream out("graph.json");
  out << "{\n\t";
  print_vertex_json(out, graph);
  out << "\t\t],\n\t";
  print_edge_json(out, graph);
  out << "}\n";
  out.close();
}

int main() {
  // GRAPH
  const std::vector<std::array<int, 2>> vertex_connections = {
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13},
  };

  Graph graph;
  graph.add_edges(vertex_connections);
  graph.add_vertices();
  print_graph_json(graph);

  return 0;
}
