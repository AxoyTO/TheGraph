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
  std::vector<EdgeId> edges_id;
};
struct Edge {
  const EdgeId id = -1;
  const VertexId vertex_start = 0;
  const VertexId vertex_end = 0;
};

class Graph {
 public:
  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);
  void add_vertices();
  const std::vector<Edge>& get_edges() const { return edges_; }
  const std::vector<Vertex>& get_vertices() const { return vertices_; }

 private:
  std::vector<Edge> edges_;
  EdgeId edge_id_counter = -1;
  EdgeId get_edge_id() { return ++edge_id_counter; }
  std::vector<Vertex> vertices_;
};

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  edges_.push_back({get_edge_id(), from_vertex_id, to_vertex_id});
}
void Graph::add_vertices() {
  int count_vertices = edges_.back().vertex_end;
  for (VertexId vertex = 0; vertex <= count_vertices; vertex++) {
    vertices_.push_back({vertex});
    for (const auto& edge : edges_) {
      if ((edge.vertex_start == vertex) || (edge.vertex_end == vertex)) {
        vertices_[vertex].edges_id.push_back(edge.id);
      }
    }
  }
}

//Вывод в файл json
std::string get_vertex_string(const Vertex& vertex) {
  std::string str_vertex =
      "\t\t{\"id\":" + std::to_string(vertex.id) + ",\"edge_ids\":[";
  for (const auto& edge_id : vertex.edges_id) {
    str_vertex += std::to_string(edge_id);
    ((edge_id != vertex.edges_id.back()) ? str_vertex += ","
                                         : str_vertex += "]}");
  }
  return str_vertex;
}
std::string get_edge_json(const Edge& edge) {
  std::string str_edge = "\t\t{\"id\":" + std::to_string(edge.id) +
                         ",\"vertex_ids\":[" +
                         std::to_string(edge.vertex_start) + "," +
                         std::to_string(edge.vertex_end) + "]}";
  return str_edge;
}
void write_graph_json_file(const Graph& graph) {
  std::ofstream out("graph.json");
  out << "{\n\t";

  // write vertices
  out << "\"vertices\": [\n";
  for (const auto& vertex : graph.get_vertices()) {
    ((vertex.id != graph.get_vertices().size() - 1)
         ? out << get_vertex_string(vertex) << ",\n"
         : out << get_vertex_string(vertex) << "\n");
  }

  // write edges
  out << "\t\t],\n\t"
      << "\"edges\": [\n";
  for (const auto& edge : graph.get_edges()) {
    ((edge.id != graph.get_edges().back().id)
         ? out << get_edge_json(edge) << ",\n"
         : out << get_edge_json(edge) << "\n");
  }
  out << "\t\t]\n";

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
  for (const auto& vertex_connection : vertex_connections) {
    graph.add_edge(vertex_connection.front(), vertex_connection.back());
  }
  graph.add_vertices();
  write_graph_json_file(graph);

  return 0;
}
