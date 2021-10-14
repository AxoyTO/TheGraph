#include <array>
#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

using VertexId = int;
using EdgeId = int;

struct Vertex {
  VertexId id = 0;
  std::vector<int> edges_id;
};
struct Edge {
  EdgeId id = 0;
  int vertex_start = 0;
  int vertex_end = 0;
};

class Graph {
 public:
  void add_edges(std::vector<std::array<int, 2>> graph);
  void add_vertices(int count_vertices);
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
void Graph::add_vertices(int count_vertices) {
  for (int it = 0; it <= count_vertices; it++) {
    vertices_.push_back({it});
    for (int edge = 0; edge < edges_.size(); edge++) {
      if ((edges_[edge].vertex_start == it) ||
          (edges_[edge].vertex_end == it)) {
        vertices_[it].edges_id.push_back(edge);
      }
    }
  }
}

//Вывод в файл json
void print_vertex_json(std::ofstream& out, Graph& graph) {
  out << "\"vertices\": [\n";
  for (auto it_vertex : graph.get_vertices()) {
    out << "\t\t{\"id\":" << it_vertex.id << ",\"edge_ids\":[";
    for (auto it_idEdges : it_vertex.edges_id)
      out << it_idEdges
          << ((it_vertex.id != graph.get_vertices().size() - 1)
                  ? ((it_idEdges != it_vertex.edges_id.back()) ? "," : "]},\n")
                  : ((it_idEdges != it_vertex.edges_id.back()) ? "," : "]}\n"));
  }
}
void print_edge_json(std::ofstream& out, Graph& graph) {
  out << "\"edges\": [\n";
  for (auto it_edge : graph.get_edges()) {
    out << "\t\t{\"id\":" << it_edge.id << ",\"vertex_ids\":["
        << it_edge.vertex_start << "," << it_edge.vertex_end
        << ((it_edge.id != graph.get_edges().back().id) ? "]},\n" : "]}\n");
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

  // NUMBER OF GRAPH VERTEX
  int count_vertices = vertex_connections.at(0).at(0);
  for (const auto& elem_row : vertex_connections)
    for (const auto& elem_col : elem_row)
      if (elem_col > count_vertices)
        count_vertices = elem_col;

  Graph graph;
  graph.add_edges(vertex_connections);
  graph.add_vertices(count_vertices);
  print_graph_json(graph);

  return 0;
}
