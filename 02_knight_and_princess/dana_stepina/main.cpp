#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>  // для функций rand()
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <utility>
#include <vector>

using std::cout;
using std::endl;

using DepthGraph = int;
using VertexId = int;
using EdgeId = int;

struct Vertex {
  const VertexId id = 0;
  const DepthGraph depth = 0;

  Vertex(const VertexId& vertex_id, const DepthGraph& vertex_depth)
      : id(vertex_id), depth(vertex_depth) {}

  void add_edge_id(const EdgeId& edge_id) {
    assert(!has_edge_id(edge_id) && "Such an edge already exists!");
    edge_ids_.push_back(edge_id);
  }
  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }
  bool has_edge_id(const EdgeId& edge_id) {
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
  void increase_graph_depth() { depth_++; }
  void add_vertex();
  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);
  const DepthGraph get_graph_depth() const { return depth_; }
  const std::vector<Edge>& get_edges() const { return edges_; }
  const std::vector<Vertex>& get_vertices() const { return vertices_; }
  const VertexId& get_vertex_id_counter() const { return vertex_id_counter_; }
  bool has_vertex_id(const VertexId& vertex_id) {
    for (const auto& vertex : vertices_)
      if (vertex_id == vertex.id)
        return true;
    return false;
  }
  bool is_connected(const VertexId& from_vertex_id,
                    const VertexId& to_vertex_id) {
    for (const auto& edge : edges_)
      if ((from_vertex_id == edge.vertex_start &&
           to_vertex_id == edge.vertex_end) ||
          (from_vertex_id == edge.vertex_end &&
           to_vertex_id == edge.vertex_start))
        return true;
    return false;
  }

 private:
  DepthGraph depth_ = 0;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  EdgeId edge_id_counter_ = 0;
  VertexId vertex_id_counter_ = 0;

  VertexId get_new_vertex_id() { return vertex_id_counter_++; }
  EdgeId get_new_edge_id() { return edge_id_counter_++; }
};

void Graph::add_vertex() {
  vertices_.emplace_back(get_new_vertex_id(), get_graph_depth());
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

//ГЕНЕРАЦИЯ ГРАФА
//Шанс создания vertices_count вершин на данной глубине
int check_number_new_vertices(const VertexId& vertices_count,
                              const int& percent) {
  if (rand() % 100 <= percent)
    return vertices_count;
  else if (vertices_count == 0)
    return 0;
  else
    return check_number_new_vertices(vertices_count - 1, percent);
}
//Генератор графа
void generate_graph(Graph& graph,
                    const DepthGraph& depth = 0,
                    const VertexId& vertices_count = 0) {
  int percent_current_depth;
  VertexId count_new_vertices;
  //Нулевая вершина
  graph.add_vertex();

  for (DepthGraph current_depth = 0; current_depth <= depth; current_depth++) {
    if (current_depth != depth)
      graph.increase_graph_depth();

    //Процент генерации вершины на текущей глубине
    percent_current_depth = floor(100 * (depth - current_depth) / depth);
    //Вектор из вершин текущей глубины
    std::vector<Vertex> vertices_in_current_depth = {};
    for (const auto& vertex : graph.get_vertices())
      if (vertex.depth == current_depth)
        vertices_in_current_depth.push_back(vertex);

    //Создание новых вершин для каждой из вершин текущей глублины
    for (const auto& vertex : vertices_in_current_depth) {
      count_new_vertices =
          check_number_new_vertices(vertices_count, percent_current_depth);
      VertexId last_new_vertex_id =
          graph.get_vertex_id_counter() - 1 + count_new_vertices;

      for (VertexId new_vertex_id = graph.get_vertices().back().id + 1;
           new_vertex_id <= last_new_vertex_id; new_vertex_id++) {
        graph.add_vertex();
        graph.add_edge(vertex.id, new_vertex_id);
      }
    }
  }
}

//ВЫВОД В ФАЙЛ JSON
std::string get_vertex_string(const Vertex& vertex) {
  std::string str_vertex = "\t  {\n\t\t\"id\":" + std::to_string(vertex.id) +
                           ",\n\t\t\"edge_ids\":[";
  for (const auto& edge_id : vertex.get_edge_ids()) {
    str_vertex += std::to_string(edge_id);
    if (edge_id != vertex.get_edge_ids().back())
      str_vertex += ",";
  }
  str_vertex += "],\n\t\t\"depth\":" + std::to_string(vertex.depth) + "\n\t  }";
  return str_vertex;
}
std::string get_edge_string(const Edge& edge) {
  std::string str_edge = "\t\t{\"id\":" + std::to_string(edge.id) +
                         ",\"vertex_ids\":[" +
                         std::to_string(edge.vertex_start) + "," +
                         std::to_string(edge.vertex_end) + "]}";
  return str_edge;
}
std::string get_graph_string(const Graph& graph) {
  std::string str_graph = "{";

  // write graph depth
  str_graph +=
      "\n\t\"depth\": " + std::to_string(graph.get_graph_depth()) + ",";

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
  const DepthGraph depth = 4;
  const int new_vertices_num = 1;
  // GRAPH
  // const int vertex_count = 14;
  // const std::vector<std::pair<int, int>> vertex_connections = {
  //     {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
  //     {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
  //     {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13},
  // };

  auto graph = Graph();

  // for (VertexId vertex_id = 0; vertex_id < vertex_count; vertex_id++) {
  //   assert(!graph.has_vertex_id(vertex_id) &&
  //          "A vertex with this id already exists.");
  //   graph.add_vertex();
  // }
  // for (const auto& vertex_connection : vertex_connections)
  //   graph.add_edge(vertex_connection.first, vertex_connection.second);
  generate_graph(graph, depth, new_vertices_num);
  write_graph_json_file(graph);

  return 0;
}
