#include <iostream>
#include <stdlib.h> // rand()
#include <fstream>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <cassert>

using std::cout;
using std::endl;

using DepthGraph = int;
using VertexId = int;
using EdgeId = int;

constexpr int INVALID_ID = -1;
const bool TRUE_MEANING = true;
const bool FALSE_MEANING = false;

struct Vertex {
  const VertexId id = INVALID_ID;
  const DepthGraph depth = 0;

  const bool& has_edge(const EdgeId& edge_id) {
    for (const auto id : edge_ids_)
      if (edge_id == id)
        return TRUE_MEANING;
      else
        continue;
    return FALSE_MEANING;
  }
  const std::vector<EdgeId>& get_edge_ids() const { return edge_ids_; }
  void add_edge_id(const EdgeId& edge_id) {
    assert(has_edge(edge_id) && "Such an edge already exists!");
    edge_ids_.push_back(edge_id);
  }
  Vertex(VertexId id) : id(id) {}

 private:
  std::vector<EdgeId> edge_ids_;
};
struct Edge {
  const EdgeId id = INVALID_ID;
  const VertexId vertex_start = 0;
  const VertexId vertex_end = 0;

  Edge(EdgeId id, VertexId vertex_start, VertexId vertex_end)
      : id(id), vertex_start(vertex_start), vertex_end(vertex_end) {}
};

class Graph {
 public:
  void add_vertex();
  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);
  const std::vector<Edge>& get_edges() const { return edges_; }
  const std::vector<Vertex>& get_vertices() const { return vertices_; }
  DepthGraph get_new_depth() { return ++depth_; }
  const bool& has_vertex(VertexId vertex_id) {
    for (const auto vertex : vertices_)
      if (vertex_id == vertex.id)
        return TRUE_MEANING;
      else
        continue;
    return FALSE_MEANING;
  }


 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  DepthGraph depth_;
  EdgeId edge_id_counter_ = INVALID_ID;
  EdgeId vertex_id_counter_ = INVALID_ID;
  void add_vertices_connection(const VertexId& from_vertex_id,
                               const VertexId& to_vertex_id,
                               const EdgeId& edge);

  EdgeId get_new_edge_id() { return ++edge_id_counter_; }
  EdgeId get_new_vertex_id() { return ++vertex_id_counter_; }
};

void Graph::add_vertex() {
  vertices_.emplace_back(get_new_vertex_id());
}
void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  assert(!has_vertex(from_vertex_id) &&
         "There is no such vertex in the graph.");
  assert(!has_vertex(to_vertex_id) && "There is no such vertex in the graph.");

  edges_.emplace_back(get_new_edge_id(), from_vertex_id, to_vertex_id);
  Edge new_edge = edges_.back();
  add_vertices_connection(from_vertex_id, to_vertex_id, new_edge.id);
}
void Graph::add_vertices_connection(const VertexId& from_vertex_id,
                                    const VertexId& to_vertex_id,
                                    const EdgeId& edge) {
  vertices_[from_vertex_id].add_edge_id(edge);
  vertices_[to_vertex_id].add_edge_id(edge);
}

//ГЕНЕРАЦИЯ ГРАФА
//Шанс создания vertices_count вершин на данной глубине
int check_number_new_vertices(const VertexId& vertices_count, const int& percent) {
  if (rand()%100 <= percent) return vertices_count;
  else 
    if (vertices_count == 0) return 0;
    else return check_number_new_vertices(vertices_count - 1, percent);
}
//Генератор графа
void generate_graph(Graph& graph, const DepthGraph& depth, const VertexId& vertices_count) {
  int percent_current_depth;
  VertexId new_vertices_count;
  //Нулевая вершина
  graph.add_vertex();

  for (DepthGraph current_depth = 0; current_depth <= depth; current_depth++) {
    //процент генерации вершины на текущей глубине
    percent_current_depth = floor(100 * (depth - current_depth) / depth);
    //вектор из вершин текущей глубины
    std::vector<Vertex> vertices_in_current_depth = {};

    for (const auto vertex : graph.get_vertices()) { 
      if (vertex.depth == current_depth) vertices_in_current_depth.push_back(vertex); 
    }

    for (const auto &vertex : vertices_in_current_depth) {
      new_vertices_count = check_number_new_vertices(vertices_count, percent_current_depth);
      VertexId last_new_vertex_id = graph.get_vertex_id() + new_vertices_count;

      for (VertexId new_vertex_id = graph.get_vertex_id() + 1; 
           new_vertex_id <= last_new_vertex_id;
           new_vertex_id++) 
           {
             graph.add_edge(vertex.id, new_vertex_id);
             graph.add_vertex(new_vertex_id);
            }
      
      graph.add_vertices_connection(vertex);
    }
    if (current_depth != depth) 
      graph.increase_graph_depth();
    //checks
    // cout<<"New"<<"depth - "<<current_depth<<endl;
    // for (const auto vertex : graph.get_vertices()) {
    //   cout<<" "<< vertex.id<<" " <<vertex.depth<<"+d - ";
    //   for (const auto& edge_id : vertex.edges_id) 
    //     cout<<edge_id<<", ";
    //   cout<<endl;
    // }
  }
}



//ВЫВОД В ФАЙЛ JSON
std::string get_vertex_string(const Vertex& vertex) {
  std::string str_vertex =
      "\t\t{\"id\":" + std::to_string(vertex.id) + ",\"edge_ids\":[";
  for (const auto& edge_id : vertex.get_edge_ids()) {
    str_vertex += std::to_string(edge_id);
    ((edge_id != vertex.get_edge_ids().back()) ? str_vertex += ","
                                               : str_vertex += "]}");
  }
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
  std::string str_graph = "{\n\t\"vertices\": [\n";

  // write vertices
  for (const auto& vertex : graph.get_vertices()) {
    ((vertex.id != graph.get_vertices().size() - 1)
         ? str_graph += get_vertex_string(vertex) + ",\n"
         : str_graph += get_vertex_string(vertex) + "\n");
  }

  // write edges
  str_graph += "\t\t],\n\t\"edges\": [\n";
  for (const auto& edge : graph.get_edges()) {
    ((edge.id != graph.get_edges().back().id)
         ? str_graph += get_edge_string(edge) + ",\n"
         : str_graph += get_edge_string(edge) + "\n");
  }

  str_graph += "\t\t]\n}\n";
  return str_graph;
}
void write_graph_json_file(const Graph& graph) {
  std::ofstream out("graph.json");
  out << get_graph_string(graph);
  out.close();
}


int main() {
  const DepthGraph depth = 4;
  const int new_vertices_num = 3;

  // GRAPH
  const int vertex_count = 14;
  const std::vector<std::array<int, 2>> vertex_connections = {
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13},
  };

  Graph graph;

  for (int vertex_id = 0; vertex_id < vertex_count; vertex_id++)
    graph.add_vertex();
  for (const auto& vertex_connection : vertex_connections)
    graph.add_edge(vertex_connection.front(), vertex_connection.back());

  write_graph_json_file(graph);

  return 0;
}
