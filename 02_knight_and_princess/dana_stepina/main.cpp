#include <fstream>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

using VertexId = int;
using EdgeId = int;

struct Vertex {
  VertexId id;
  std::vector<int> edges_id;
};
struct Edge {
  EdgeId id;
  int vertex_start;
  int vertex_end;
};

class Graph {
 public:
  int count_edges = 0;
  int count_vertices = 0;
  void create_Graph(std::vector<std::vector<int>> graph) {
    add_Edges(graph);
    add_Vertices(graph);
  }
  std::vector<Edge> get_edges() { return graph_edges_; }
  std::vector<Vertex> get_vertices() { return graph_vertices_; }

 private:
  std::vector<Edge> graph_edges_;
  std::vector<Vertex> graph_vertices_;

  void add_Edges(std::vector<std::vector<int>> graph);
  void add_Vertices(std::vector<std::vector<int>> graph);
};

void Graph::add_Edges(std::vector<std::vector<int>> graph) {
  for (int it = 0; it < graph.size(); it++)
    graph_edges_.push_back({it, graph[it][0], graph[it][1]});
  count_edges = graph.size();
}
void Graph::add_Vertices(std::vector<std::vector<int>> graph) {
  count_vertices = graph.at(0).at(0);
  for (const auto& elem_row : graph)
    for (const auto& elem_col : elem_row)
      if (elem_col > count_vertices)
        count_vertices = elem_col;

  for (int it = 0; it <= count_vertices; it++) {
    graph_vertices_.push_back({it});
    for (int edge = 0; edge < graph.size(); edge++) {
      if ((graph_edges_[edge].vertex_start == it) or
          (graph_edges_[edge].vertex_end == it)) {
        graph_vertices_[it].edges_id.push_back(edge);
      }
    }
  }
}

//Вывод в файл json
void inputGraph_json(Graph& graph);
void printGraph_json(std::ofstream& out, Graph& graph);
void printVertex_json(std::ofstream& out, Graph& graph);
void printEdge_json(std::ofstream& out, Graph& graph);

int main() {
  // GRAPH
  const std::vector<std::vector<int>> temp_graph = {
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13},
  };

  Graph graph;
  graph.create_Graph(temp_graph);
  inputGraph_json(graph);

  return 0;
}

void inputGraph_json(Graph& graph) {
  std::ofstream out("graph.json");
  printGraph_json(out, graph);
  out.close();
}
void printGraph_json(std::ofstream& out, Graph& graph) {
  out << "{\n\t";
  printVertex_json(out, graph);
  out << "\t\t],\n\t";
  printEdge_json(out, graph);
  out << "}\n";
}
void printVertex_json(std::ofstream& out, Graph& graph) {
  out << "\"vertices\": [\n";
  for (auto it_vertex : graph.get_vertices()) {
    out << "\t\t{\"id\":" << it_vertex.id << ",\"edge_ids\":[";
    for (auto it_idEdges : it_vertex.edges_id)
      out << it_idEdges
          << ((it_vertex.id != graph.count_vertices)
                  ? ((it_idEdges != it_vertex.edges_id.back()) ? "," : "]},\n")
                  : ((it_idEdges != it_vertex.edges_id.back()) ? "," : "]}\n"));
  }
}
void printEdge_json(std::ofstream& out, Graph& graph) {
  out << "\"edges\": [\n";
  for (auto it_edge : graph.get_edges()) {
    out << "\t\t{\"id\":" << it_edge.id << ",\"vertex_ids\":["
        << it_edge.vertex_start << "," << it_edge.vertex_end
        << ((it_edge.id != graph.get_edges().back().id) ? "]},\n" : "]}\n");
  }
  out << "\t]\n";
}
