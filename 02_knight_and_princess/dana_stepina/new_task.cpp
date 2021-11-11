#include <iostream>
#include <stdlib.h> // rand()
#include <fstream>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <unordered_set>

using std::cout;
using std::endl;

using VertexId = int;
using EdgeId = int;
using DepthGraph = int;

constexpr int INVALID_ID = -1;

const std::string JSON_FILENAME = "graph.json";

enum ColorEdge { GRAY, GREEN, BLUE, YELLOW, RED };

struct Vertex {
  const VertexId id = 0;
  const DepthGraph depth = 0;
  std::vector<EdgeId> edges_id;
};
struct Edge {
  const EdgeId id = INVALID_ID;
  const VertexId vertex_start = 0;
  const VertexId vertex_end = 0;

  Edge() {}
};

class Graph {
 public:
  void add_edge(const VertexId& from_vertex_id, const VertexId& to_vertex_id);
  void add_vertex(const VertexId& new_vertex_id);
  void add_vertices_connection(const Vertex& vertex);
  void increase_graph_depth() { depth_++; }

  //getters
  const std::vector<Edge>& get_edges() const { return edges_; }
  const std::vector<Vertex>& get_vertices() const { return vertices_; }
  const VertexId& get_depth() const { return depth_; }
  const VertexId& get_vertex_id() const { return vertex_id_counter_; }

 private:
  DepthGraph depth_ = 0;
  std::vector<Edge> edges_;

  std::vector<Vertex> vertices_ = {{0, depth_}};
  
  EdgeId edge_id_counter_ = INVALID_ID;
  VertexId vertex_id_counter_ = 0;
  
  EdgeId get_edge_id_() { return ++edge_id_counter_; }
  void increase_vertex_id_counter_() { vertex_id_counter_++; }
  void remove_duplicate_items_edges_id_(std::vector<EdgeId>& edges_id) {
    std::unordered_set<int> temp_set(edges_id.begin(), edges_id.end());
    edges_id.assign(temp_set.begin(), temp_set.end());
    std::reverse(edges_id.begin(), edges_id.end());
  }
};

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  edges_.emplace_back(get_edge_id_(), from_vertex_id, to_vertex_id);
}
void Graph::add_vertex(const VertexId& new_vertex_id) {
  increase_vertex_id_counter_();

  vertices_.push_back({new_vertex_id, depth_+1});
  add_vertices_connection(vertices_[new_vertex_id]);
  }
void Graph::add_vertices_connection(const Vertex& vertex) {
    for (const auto& edge : edges_) 
      if ((edge.vertex_start == vertex.id) || (edge.vertex_end == vertex.id))
        vertices_[vertex.id].edges_id.push_back(edge.id);
  
  //Убираем повторяющиеся элементы в векторе из id ребер
  remove_duplicate_items_edges_id_(vertices_[vertex.id].edges_id);
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
  std::ofstream out(JSON_FILENAME);
  out << "{\n\t";

  // write graph depth
  out << "\"depth\": " << graph.get_depth() <<",\n\t";

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

// Генерация нового графа
int check_number_new_vertices(const VertexId& vertices_count, const int& percent) {
  if (rand()%100 <= percent) return vertices_count;
  else 
    if (vertices_count == 0) return 0;
    else return check_number_new_vertices(vertices_count - 1, percent);
}
void generate_graph(Graph& graph, const DepthGraph& depth, const VertexId& vertices_count) {
  int percent_current_depth;
  VertexId new_vertices_count;

  for (DepthGraph current_depth = 0; current_depth <= depth; current_depth++) {
    //процент генерации вершины на текущей глубине
    //change name
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

int main() {
  const DepthGraph depth = 4;
  const int new_vertices_num = 3;

  Graph graph;
  generate_graph(graph, depth, new_vertices_num);
  write_graph_json_file(graph);
  // graph.print_vertex_id();
  return 0;
}
