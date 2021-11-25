#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

using std::cout;
using std::endl;

using VertexId = int;
using EdgeId = int;

constexpr int VERTEX_COUNT = 14;
constexpr int EDGES_COUNT = 18;

struct Vertex {
  const VertexId id = 0;

  explicit Vertex(const VertexId& vertex_id) : id(vertex_id) {}

  void add_edge_id(const EdgeId& edge_id) {
    assert(!has_edge_id(edge_id) && "Such an edge already exists!");
    edge_ids_.push_back(edge_id);
  }
  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }
  bool has_edge_id(const EdgeId& edge_id) const {
    for (const auto& id : edge_ids_)
      if (edge_id == id)
        return true;
    return false;
  }

 private:
  std::vector<EdgeId> edge_ids_;
};
struct Edge {
  const EdgeId id = 0;
  const VertexId vertex_start = 0;
  const VertexId vertex_end = 0;

  Edge(const EdgeId& edge_id,
       const VertexId& vertex_connection_start,
       const VertexId& vertex_connection_end)
      : id(edge_id),
        vertex_start(vertex_connection_start),
        vertex_end(vertex_connection_end) {}
};

class Graph {
 public:
  void add_vertex();
  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);
  const std::vector<Edge>& get_edges() const { return edges_; }
  const std::vector<Vertex>& get_vertices() const { return vertices_; }
  bool has_vertex_id(const VertexId& vertex_id) const;
  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) const;

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  EdgeId edge_id_counter_ = 0;
  VertexId vertex_id_counter_ = 0;

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }
  EdgeId get_new_edge_id() { return edge_id_counter_++; }
};

void Graph::add_vertex() {
  vertices_.emplace_back(get_new_vertex_id());
}
void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  assert(has_vertex_id(from_vertex_id) &&
         "There is no such vertex in the graph.");
  assert(has_vertex_id(to_vertex_id) &&
         "There is no such vertex in the graph.");
  assert(!is_connected(from_vertex_id, to_vertex_id) &&
         "These vertices are already connected");

  const auto& new_edge =
      edges_.emplace_back(get_new_edge_id(), from_vertex_id, to_vertex_id);
  vertices_[from_vertex_id].add_edge_id(new_edge.id);
  vertices_[to_vertex_id].add_edge_id(new_edge.id);
}
bool Graph::has_vertex_id(const VertexId& vertex_id) const {
  for (const auto& vertex : vertices_)
    if (vertex_id == vertex.id)
      return true;
  return false;
}
bool Graph::is_connected(const VertexId& from_vertex_id,
                         const VertexId& to_vertex_id) const {
  assert(has_vertex_id(from_vertex_id) &&
         "There is no such vertex in the graph.");
  assert(has_vertex_id(to_vertex_id) &&
         "There is no such vertex in the graph.");

  for (const auto& edge_id_from_vertex :
       vertices_[from_vertex_id].get_edge_ids())
    for (const auto& edge_id_to_vertex : vertices_[to_vertex_id].get_edge_ids())
      if (edge_id_from_vertex == edge_id_to_vertex)
        return true;
  return false;
}

//ВЫВОД В ФАЙЛ JSON
std::string get_vertex_string(const Vertex& vertex) {
  std::string str_vertex =
      "\t\t{\"id\":" + std::to_string(vertex.id) + ",\"edge_ids\":[";
  for (const auto& edge_id : vertex.get_edge_ids()) {
    str_vertex += std::to_string(edge_id);
    if (edge_id != vertex.get_edge_ids().back())
      str_vertex += ",";
  }
  str_vertex += "]}";
  return str_vertex;
}
std::string get_edge_string(const Edge& edge) {
  return "\t\t{\"id\":" + std::to_string(edge.id) + ",\"vertex_ids\":[" +
         std::to_string(edge.vertex_start) + "," +
         std::to_string(edge.vertex_end) + "]}";
}
std::string get_graph_string(const Graph& graph) {
  std::string str_graph = "{";

  // write vertices
  str_graph += "\n\t\"vertices\": [\n";
  for (const auto& vertex : graph.get_vertices()) {
    str_graph += get_vertex_string(vertex);
    if (vertex.id != graph.get_vertices().back().id)
      str_graph += ",\n";
  }
  str_graph += "\n\t\t],";

  // write edges
  str_graph += "\n\t\"edges\": [\n";
  for (const auto& edge : graph.get_edges()) {
    str_graph += get_edge_string(edge);
    if (edge.id != graph.get_edges().back().id)
      str_graph += ",\n";
  }
  str_graph += "\n\t\t]";

  str_graph += "\n}\n";
  return str_graph;
}
void write_graph_json_file(const Graph& graph) {
  std::ofstream out("graph.json");
  out << get_graph_string(graph);
  out.close();
}

int main() {
  // GRAPH
  const std::array<std::pair<VertexId, VertexId>, EDGES_COUNT>
      vertex_connections = {{
          {0, 1},
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
          {12, 13},
      }};

  auto graph = Graph();

  for (VertexId vertex_id = 0; vertex_id < VERTEX_COUNT; vertex_id++) {
    graph.add_vertex();
  }
  for (const auto& vertex_connection : vertex_connections)
    graph.add_edge(vertex_connection.first, vertex_connection.second);

  write_graph_json_file(graph);

  return 0;
}
